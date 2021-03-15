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
    + "saved_simulation_state.json";

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

SimulationEngine::SimulationEngine(bool load_from_saved_file) {
  if (load_from_saved_file) {
    container_ = LoadContainerFromJson();
  } else {
    container_ = GenerateRandomContainerFromJson();
  }
}

void SimulationEngine::AdvanceToNextFrame() {
  container_.AdvanceOneFrame();
}

void SimulationEngine::Render() {
  container_.Display();
}

GasContainer SimulationEngine::LoadContainerFromJson() const {
  // Code below adapted from https://stackoverflow.com/a/2602060
  std::ifstream loaded_file(kJsonStateFilePath);
  json json_data;
  loaded_file >> json_data;

  json particle_types = json_data[kJsonSchemaParticleTypesKey];
  json particles = json_data[kJsonSchemaParticleStatesKey];

  std::vector<GasParticle> gas_particles_vector;

  for (json::iterator it = particles.begin(); it != particles.end(); ++it) {
    json particle_state = *it;

    json position_array = particle_state[kJsonSchemaPositionKey];
    json velocity_array = particle_state[kJsonSchemaVelocityKey];
    vec2 init_position = vec2(position_array[0], position_array[1]);
    vec2 init_velocity = vec2(velocity_array[0], velocity_array[1]);

    string particle_type_name = particle_state[kJsonSchemaTypeKey];
    json type_details = particle_types[particle_type_name];

    GasParticle gas_particle = GasParticle(init_position, init_velocity,
      type_details[kJsonSchemaRadiusKey], type_details[kJsonSchemaRedKey],
      type_details[kJsonSchemaGreenKey], type_details[kJsonSchemaBlueKey]);
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
        type_details[kJsonSchemaGreenKey], type_details[kJsonSchemaBlueKey]);
      gas_particles_vector.push_back(gas_particle);
    }
  }

  return GasContainer(gas_particles_vector);
}

}  // namespace idealgas