#include "json_manager.h"

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

  // Access the particles definitions and particle instances sub-objects
  json particle_types = json_data[kJsonSchemaParticleTypesKey];
  std::vector<GasParticle> gas_particles_vector;

  // Iterate through all objects in the array of particle instances
  for (const json& particle_state : json_data[kJsonSchemaParticleStatesKey]) {
    GasParticle gas_particle =
        CreateSpecifiedParticle(particle_state, particle_types);
    gas_particles_vector.push_back(gas_particle);
  }

  return GasContainer(gas_particles_vector);
}

GasParticle JsonManager::CreateSpecifiedParticle(
    const json& particle_state, const json& particle_types) const {
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
    type_details[kJsonSchemaRadiusKey], type_details[kJsonSchemaMassKey],
    type_details[kJsonSchemaRedKey], type_details[kJsonSchemaGreenKey],
    type_details[kJsonSchemaBlueKey], particle_type_name);

  return gas_particle;
}

GasContainer JsonManager::GenerateRandomContainerFromJson(
    const string& json_file_path) const {
  ValidateFilePath(json_file_path);

  // Load the file as json if the path is valid
  std::ifstream loaded_file(json_file_path);
  json json_data;
  loaded_file >> json_data;

  ValidateRandomGenerationJson(json_data);

  json particle_types = json_data[kJsonSchemaParticleTypesKey];

  std::vector<GasParticle> gas_particles_vector;
  ci::Rand random = ci::Rand();

  // Go through each particle type requested to create and create all of them
  for (json particle_specification : json_data[kJsonSchemaParticleCountsKey]) {
    // Load the color and radius json for the newest particle to create
    string particle_type_key = particle_specification[kJsonSchemaTypeKey];
    json type_details = particle_types[particle_type_key];

    size_t specified_count = particle_specification[kJsonSchemaCountKey];
    float max_velocity = particle_specification[kJsonSchemaMaxVelocityKey];

    for (size_t idx = 0; idx < specified_count; idx++) {
      GasParticle particle = GenerateRandomParticle(random, max_velocity,
                                                    type_details,
                                                    particle_type_key);
      gas_particles_vector.push_back(particle);
    }
  }

  return GasContainer(gas_particles_vector);
}

GasParticle JsonManager::GenerateRandomParticle(
    ci::Rand& random, float max_velo, const json& type_details,
    const string& type_key) const {
  // velocity is a vec2 of values between -max_velocity and max_velocity
  vec2 velocity = vec2(random.posNegFloat(0, max_velo),
                       random.posNegFloat(0, max_velo));

  // Generate a random position within the bounds of the container
  float x_position = random.nextFloat(GasContainer::kContainerLeftBound,
                                      GasContainer::kContainerRightBound);
  float y_position = random.nextFloat(GasContainer::kContainerUpperBound,
                                      GasContainer::kContainerLowerBound);
  vec2 position = vec2(x_position, y_position);

  GasParticle gas_particle = GasParticle(position, velocity,
    type_details[kJsonSchemaRadiusKey], type_details[kJsonSchemaMassKey],
    type_details[kJsonSchemaRedKey], type_details[kJsonSchemaGreenKey],
    type_details[kJsonSchemaBlueKey], type_key);
  return gas_particle;
}

void JsonManager::ValidateRandomGenerationJson(const json& to_validate) {
  // Check if the 2 json sub-objects containing all important info exist
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
      particle_def.at(kJsonSchemaGreenKey);
      particle_def.at(kJsonSchemaBlueKey);
    } catch (json::out_of_range& e) {
      throw std::invalid_argument("The provided json is invalid");
    }
  }
}

void JsonManager::ValidateFilePath(const string& file_path) {
  std::ifstream located_file(file_path);

  if (!located_file.is_open()) {
    throw std::invalid_argument("The file path is not valid");
  }
}

void JsonManager::WriteContainerToJson(const GasContainer& container,
                                       const string& save_file_path) const {
  json particle_types;
  json particle_array;

  for (const GasParticle& particle : container.GetAllParticles()) {
    json serialized_particle = SerializeParticle(particle, particle_types);
    particle_array.push_back(serialized_particle);
  }

  json output;
  output[kJsonSchemaParticleStatesKey] = particle_array;
  output[kJsonSchemaParticleTypesKey] = particle_types;

  // write the json to the saved file
  std::ofstream output_file(save_file_path);
  output_file << std::setw(4) << output << std::endl;
}

json JsonManager::SerializeParticle(const GasParticle& particle,
                                         json& particle_types) const {
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
        {kJsonSchemaMassKey, particle.GetMass()},
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

  return serialized_particle;
}

} // namespace idealgas