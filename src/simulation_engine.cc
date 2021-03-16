#include "simulation_engine.h"
#include "cinder/Rand.h"

#include <nlohmann/json.hpp>

namespace idealgas {

using glm::vec2;
using nlohmann::json;
using std::string;

const string SimulationEngine::kBaseFilePath =
    "/Users/neilkaushikkar/Cinder/my-projects/ideal-gas-nkaush/";

const string SimulationEngine::kJsonStateFilePath = kBaseFilePath
    + "src/data/saved_simulation.json";

const string SimulationEngine::kJsonRandomSimulationFilePath = kBaseFilePath
    + "src/data/random_simulation_generator.json";

const string SimulationEngine::kJsonSchemaParticleStatesKey = "particle_states";
const string SimulationEngine::kJsonSchemaParticleTypesKey = "particle_types";

// These keys access info about a particle's motion
const string SimulationEngine::kJsonSchemaVelocityKey = "velocity";
const string SimulationEngine::kJsonSchemaPositionKey = "position";

// These keys access info about the visuals of the particles
const string SimulationEngine::kJsonSchemaTypeKey = "type";
const string SimulationEngine::kJsonSchemaRedKey = "red";
const string SimulationEngine::kJsonSchemaGreenKey = "green";
const string SimulationEngine::kJsonSchemaBlueKey = "blue";
const string SimulationEngine::kJsonSchemaRadiusKey = "radius";

// These fields are used as constraints for generating GasContainers randomly
const string SimulationEngine::kJsonSchemaParticleCountKey = "particle_count";
const string SimulationEngine::kJsonSchemaMaxVelocityKey = "max_velocity";

SimulationEngine::SimulationEngine(bool load_from_saved_file) :
      container_(ContainerInitializer(load_from_saved_file)) {}

GasContainer SimulationEngine::ContainerInitializer(
    bool load_from_saved_file) const {
  if (load_from_saved_file) {
    return LoadContainerFromJson(kJsonStateFilePath);
  } else {
    return GenerateRandomContainerFromJson(kJsonRandomSimulationFilePath);
  }
}

GasContainer SimulationEngine::LoadContainerFromJson(
    const string& json_file_path) const {
  // Code below adapted from https://stackoverflow.com/a/2602060
  std::ifstream loaded_file(json_file_path);
  json json_data;
  loaded_file >> json_data;

  // Access the particles definitions and particle instances sub-objects
  json particle_types = json_data[kJsonSchemaParticleTypesKey];
  json particles = json_data[kJsonSchemaParticleStatesKey];

  std::vector<GasParticle> gas_particles_vector;

  // Iterate through all objects in the array of particle instances
  for (json::iterator it = particles.begin(); it != particles.end(); ++it) {
    json particle_state = *it;

    // Convert a json array to a vec2 for position and velocity
    json position_array = particle_state[kJsonSchemaPositionKey];
    json velocity_array = particle_state[kJsonSchemaVelocityKey];
    vec2 init_position = vec2(position_array[GasContainer::kXAxis],
                              position_array[GasContainer::kYAxis]);
    vec2 init_velocity = vec2(velocity_array[GasContainer::kXAxis],
                              velocity_array[GasContainer::kYAxis]);

    // Get the particle definition this object is referring to
    string particle_type_name = particle_state[kJsonSchemaTypeKey];
    json type_details = particle_types[particle_type_name];

    // Create particle with particle definition and vectors created above
    GasParticle gas_particle = GasParticle(init_position, init_velocity,
      type_details[kJsonSchemaRadiusKey], type_details[kJsonSchemaRedKey],
      type_details[kJsonSchemaGreenKey], type_details[kJsonSchemaBlueKey],
      particle_type_name);
    gas_particles_vector.push_back(gas_particle);
  }

  return GasContainer(gas_particles_vector);
}

GasContainer SimulationEngine::GenerateRandomContainerFromJson(
    const string& json_file_path) const {
  std::ifstream loaded_file(json_file_path);
  json json_data;
  loaded_file >> json_data;

  json particle_types = json_data[kJsonSchemaParticleTypesKey];
  json count_info = json_data[kJsonSchemaParticleStatesKey];

  std::vector<GasParticle> gas_particles_vector;
  ci::Rand random = ci::Rand();

  for (json::iterator it = count_info.begin(); it != count_info.end(); ++it) {
    json particle_specification = *it;

    string particle_type_key = particle_specification[kJsonSchemaTypeKey];
    json type_details = particle_types[particle_type_key];

    size_t specified_count = particle_specification[kJsonSchemaParticleCountKey];
    float max_velocity = particle_specification[kJsonSchemaMaxVelocityKey];

    for (size_t idx = 0; idx < specified_count; idx++) {
      // velocity is a vec2 of values between -max_velocity and max_velocity
      vec2 velocity = vec2(random.posNegFloat(0, max_velocity),
                           random.posNegFloat(0, max_velocity));

      float x_position = random.nextFloat(GasContainer::kContainerLeftBound,
                                          GasContainer::kContainerRightBound);
      float y_position = random.nextFloat(GasContainer::kContainerUpperBound,
                                          GasContainer::kContainerLowerBound);
      vec2 position = vec2(x_position, y_position);

      GasParticle gas_particle = GasParticle(position, velocity,
        type_details[kJsonSchemaRadiusKey], type_details[kJsonSchemaRedKey],
        type_details[kJsonSchemaGreenKey], type_details[kJsonSchemaBlueKey],
        particle_type_key);
      gas_particles_vector.push_back(gas_particle);
    }
  }

  return GasContainer(gas_particles_vector);
}

void SimulationEngine::SaveContainerToJson(const string& save_file_path) const {
  json particle_types;
  json particle_array;

  for (const GasParticle& particle : container_.GetAllParticles()) {
    json serialized_particle;
    string type_name = particle.GetTypeName();
    serialized_particle[kJsonSchemaTypeKey] = type_name;

    // Add the particle state definition to the json if it is not defined yet
    try {
      particle_types.at(type_name);
      // no way to actually check if a key exists so use try/catch
    } catch (json::exception& e) {
      particle_types[type_name] = {
          {kJsonSchemaRadiusKey, particle.GetRadius()},
          {kJsonSchemaRedKey, particle.GetRedIntensity()},
          {kJsonSchemaGreenKey, particle.GetGreenIntensity()},
          {kJsonSchemaBlueKey, particle.GetBlueIntensity()}
      };
    }

    json position_array;  // save the position array
    position_array.push_back(particle.GetPosition()[GasContainer::kXAxis]);
    position_array.push_back(particle.GetPosition()[GasContainer::kYAxis]);
    serialized_particle[kJsonSchemaPositionKey] = position_array;

    json velocity_array;  // save the velocity array
    velocity_array.push_back(particle.GetVelocity()[GasContainer::kXAxis]);
    velocity_array.push_back(particle.GetVelocity()[GasContainer::kYAxis]);
    serialized_particle[kJsonSchemaVelocityKey] = velocity_array;

    particle_array.push_back(serialized_particle);
  }

  json output;
  output[kJsonSchemaParticleStatesKey] = particle_array;
  output[kJsonSchemaParticleTypesKey] = particle_types;

  // write the json to the saved file
  std::ofstream output_file(save_file_path);
  output_file << std::setw(4) << output << std::endl;
}

void SimulationEngine::AdvanceToNextFrame() {
  container_.AdvanceOneFrame();
}

void SimulationEngine::Render() {
  container_.Display();
}

}  // namespace idealgas