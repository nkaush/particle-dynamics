#include "simulation_engine.h"
#include "cinder/Rand.h"

#include <nlohmann/json.hpp>

namespace idealgas {

using glm::vec2;
using nlohmann::json;
using std::string;
using std::map;

const string SimulationEngine::kBaseDataFilePath =
    "/Users/neilkaushikkar/Cinder/my-projects/ideal-gas-nkaush/apps/data/";

const string SimulationEngine::kJsonStateFilePath = kBaseDataFilePath
    + "my_simulation.json";

const string SimulationEngine::kJsonRandomSimulationFilePath = kBaseDataFilePath
    + "random_simulation_generator.json";

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
const string SimulationEngine::kJsonSchemaVelocityRangeKey = "velocity_range";

SimulationEngine::SimulationEngine(bool load_from_saved_file) :
      container_(ContainerInitializer(load_from_saved_file)) {}

GasContainer SimulationEngine::ContainerInitializer(
    bool load_from_saved_file) const {
  if (load_from_saved_file) {
    return LoadContainerFromJson();
  } else {
    return GenerateRandomContainerFromJson();
  }
}

GasContainer SimulationEngine::LoadContainerFromJson() const {
  // Code below adapted from https://stackoverflow.com/a/2602060
  std::ifstream loaded_file(kJsonStateFilePath);
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
    vec2 init_position = vec2(position_array[0], position_array[1]);
    vec2 init_velocity = vec2(velocity_array[0], velocity_array[1]);

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

GasContainer SimulationEngine::GenerateRandomContainerFromJson() const {
  std::ifstream loaded_file(kJsonRandomSimulationFilePath);
  json json_data;
  loaded_file >> json_data;

  json particle_types = json_data[kJsonSchemaParticleTypesKey];
  json count_info = json_data[kJsonSchemaParticleStatesKey];

  std::vector<GasParticle> gas_particles_vector;
  ci::Rand random = ci::Rand();

  for (json::iterator it = count_info.begin(); it != count_info.end(); ++it) {
    json particle_specs = *it;

    string particle_type_key = particle_specs[kJsonSchemaTypeKey];
    json type_details = particle_types[particle_type_key];

    size_t specified_count = particle_specs[kJsonSchemaParticleCountKey];
    json velocity_range = particle_specs[kJsonSchemaVelocityRangeKey];

    for (size_t idx = 0; idx < specified_count; idx++) {
      float x_velo = random.posNegFloat(velocity_range[0], velocity_range[1]);
      float y_velo = random.posNegFloat(velocity_range[0], velocity_range[1]);
      vec2 velocity = vec2(x_velo, y_velo);

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

void SimulationEngine::SaveContainerToJson() const {
  std::ifstream loaded_file(kJsonRandomSimulationFilePath);
  json json_data;
  loaded_file >> json_data;

  json output;
  output[kJsonSchemaParticleTypesKey] = json_data[kJsonSchemaParticleTypesKey];
  json particle_array;

  for (const GasParticle& particle : container_.GetAllParticles()) {
    json serialized_particle;
    serialized_particle[kJsonSchemaTypeKey] = particle.GetTypeName();

    json position_array;
    position_array.push_back(particle.GetPosition()[0]);
    position_array.push_back(particle.GetPosition()[1]);
    serialized_particle[kJsonSchemaPositionKey] = position_array;

    json velocity_array;
    velocity_array.push_back(particle.GetVelocity()[0]);
    velocity_array.push_back(particle.GetVelocity()[1]);
    serialized_particle[kJsonSchemaVelocityKey] = velocity_array;

    particle_array.push_back(serialized_particle);
  }

  output[kJsonSchemaParticleStatesKey] = particle_array;
  std::ofstream output_file("pretty.json");
  output_file << std::setw(4) << output << std::endl;
}

void SimulationEngine::AdvanceToNextFrame() {
  container_.AdvanceOneFrame();
}

void SimulationEngine::Render() {
  container_.Display();
}

}  // namespace idealgas