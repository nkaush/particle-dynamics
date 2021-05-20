#include "json_manager.h"
#include <map>

namespace idealgas {

using std::string;
using nlohmann::json;
using glm::vec2;

const string JsonManager::kJsonSchemaParticleStatesKey = "particle_states";
const string JsonManager::kJsonSchemaParticleTypesKey = "particle_types";
const string JsonManager::kJsonSchemaParticleCountsKey = "particle_counts";

// These keys access info about a particle's motion
const string JsonManager::kJsonSchemaVelocityKey = "velocity";
const string JsonManager::kJsonSchemaPositionKey = "position";

// These keys access info about the visuals of the particles
const string JsonManager::kJsonSchemaTypeKey = "type";
const string JsonManager::kJsonSchemaRedKey = "red";
const string JsonManager::kJsonSchemaGreenKey = "green";
const string JsonManager::kJsonSchemaBlueKey = "blue";
const string JsonManager::kJsonSchemaRadiusKey = "radius";
const string JsonManager::kJsonSchemaMassKey = "mass";

// These fields are used as constraints for generating GasContainers randomly
const string JsonManager::kJsonSchemaCountKey = "count";
const string JsonManager::kJsonSchemaMaxVelocityKey = "max_velocity";

JsonManager::JsonManager() = default;

GasContainer JsonManager::LoadContainerFromJson(
    const string& json_file_path) const {
  ValidateFilePath(json_file_path);

  // Code below adapted from https://stackoverflow.com/a/2602060
  std::ifstream loaded_file(json_file_path);
  json json_data;
  loaded_file >> json_data;

  GasContainer container = json_data.get<GasContainer>();
  container.Configure();

  return container;
}

GasContainer JsonManager::GenerateRandomContainerFromJson(
    const string& json_file_path) const {
  ValidateFilePath(json_file_path);

  // Load the file as json if the path is valid
  std::ifstream loaded_file(json_file_path);
  json json_data;
  loaded_file >> json_data;

//  ValidateRandomGenerationJson(json_data);

  auto particle_specifications =
      json_data["particle_types"].get<std::map<std::string, ParticleSpecs>>();
  auto container_specifications =
      json_data["particle_counts"].get<std::vector<ContainerSpecifications>>();

  std::vector<GasParticle> gas_particles;
  ci::Rand random = ci::Rand();

  // Go through each particle type requested to create and create all of them
  for (const ContainerSpecifications& specs : container_specifications) {
    for (size_t idx = 0; idx < specs.count; idx++) {
      GasParticle particle = GenerateRandomParticle(
          random, specs.max_velocity,
          particle_specifications.at(specs.particle_name));
      gas_particles.push_back(particle);
    }
  }

  return GasContainer(gas_particles, particle_specifications);
}

GasParticle JsonManager::GenerateRandomParticle(
    ci::Rand& random, float max_velo, const ParticleSpecs& specifications) const {
  // velocity is a vec2 of values between -max_velocity and max_velocity
  vec2 velocity = vec2(random.posNegFloat(0, max_velo),
                       random.posNegFloat(0, max_velo));

  // Generate a random position within the bounds of the container
  float x_position = random.nextFloat(GasContainer::kContainerLeftBound,
                                      GasContainer::kContainerRightBound);
  float y_position = random.nextFloat(GasContainer::kContainerUpperBound,
                                      GasContainer::kContainerLowerBound);
  vec2 position = vec2(x_position, y_position);

  return GasParticle(position, velocity, specifications);
}

void JsonManager::ValidateRandomGenerationJson(const json& to_validate) {
  /*// Check if the 2 json sub-objects containing all important info exist
  try {
    to_validate.at(kJsonSchemaParticleCountsKey);
    to_validate.at(kJsonSchemaParticleTypesKey);
  } catch (json::out_of_range& e) {
    throw std::invalid_argument("The provided json is invalid");
  }

  // Check the particle counts schema
  for (const json& particle_count : to_validate[kJsonSchemaParticleCountsKey]) {
    try {
      // All we need to do is check if the keys exist, otherwise throw error
      particle_count.at(kJsonSchemaCountKey);
      particle_count.at(kJsonSchemaMaxVelocityKey);
      string type_name = particle_count.at(kJsonSchemaTypeKey);
      to_validate.at(kJsonSchemaParticleTypesKey).at(type_name);
    } catch (json::out_of_range& e) {
      throw std::invalid_argument("The provided json is invalid");
    }
  }

  // Check the particle definitions schema
  for (const json& particle_def : to_validate[kJsonSchemaParticleTypesKey]) {
    try {
      particle_def.at(kJsonSchemaRedKey);
      particle_def.at(kJsonSchemaRadiusKey);
      particle_def.at(kJsonSchemaMassKey);
      particle_def.at(kJsonSchemaGreenKey);
      particle_def.at(kJsonSchemaBlueKey);
    } catch (json::out_of_range& e) {
      throw std::invalid_argument("The provided json is invalid");
    }
  }*/
}

void JsonManager::ValidateFilePath(const string& file_path) {
  std::ifstream located_file(file_path);

  if (!located_file.is_open()) {
    throw std::invalid_argument("The file path is not valid");
  }
}

void JsonManager::WriteContainerToJson(const GasContainer& container,
                                       const string& save_file_path) const {
  json serialized_container = container;

  // write the json to the saved file
  std::ofstream output_file(save_file_path);
  output_file << std::setw(2) << serialized_container << std::endl;
}

} // namespace idealgas
