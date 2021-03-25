//
// Created by Neil Kaushikkar on 3/19/21.
//

#ifndef IDEAL_GAS_JSON_MANAGER_H
#define IDEAL_GAS_JSON_MANAGER_H

#include "gas_container.h"
#include "cinder/Rand.h"

#include <nlohmann/json.hpp>

namespace idealgas {

class JsonManager {
 public:
  /**
   * Default constructor. No parameters are needed on initialization.
   */
  JsonManager();

  /**
   * Generates a random simulation using the parameters specified in the
   * random simulation generator json file.
   * @param json_file_path - a string indicating the path load load json from
   * @return a randomly generated GasContainer
   */
  GasContainer GenerateRandomContainerFromJson(
      const std::string& json_file_path) const;

  /**
   * Generates a simulation using the saved particles states in the saved
   * simulation generator json file.
   * @param json_file_path - a string indicating the path load load json from
   * @return a GasContainer loaded from the saved json file
   */
  GasContainer LoadContainerFromJson(const std::string& json_file_path) const;

  /**
   * Ensures that the file corresponding to the provided file path exists.
   * @param file_path - a string indicating the file path
   * @throws std::invalid_argument if the file path does not exist
   */
  static void ValidateFilePath(const std::string& file_path);

  /**
   * Ensures that the provided json has a valid schema.
   * @param to_validate - the json object to validate
   * @throws std::invalid_argument when the json provided is invalid
   */
  static void ValidateRandomGenerationJson(const nlohmann::json& to_validate);

  /**
   * Saves the current state of the simulation in a json file.
   */
  void WriteContainerToJson(const GasContainer& container,
                            const std::string& save_file_path) const;

 private:
  // These keys access the subsections of the json: motion and visuals
  static const std::string kJsonSchemaParticleStatesKey;
  static const std::string kJsonSchemaParticleTypesKey;
  static const std::string kJsonSchemaParticleCountsKey;

  // These keys access info about a particle's motion
  static const std::string kJsonSchemaVelocityKey;
  static const std::string kJsonSchemaPositionKey;

  // These keys access info about the visuals of the particles
  static const std::string kJsonSchemaTypeKey;
  static const std::string kJsonSchemaRedKey;
  static const std::string kJsonSchemaGreenKey;
  static const std::string kJsonSchemaBlueKey;
  static const std::string kJsonSchemaRadiusKey;
  static const std::string kJsonSchemaMassKey;

  // These fields are used as constraints for generating GasContainers randomly
  static const std::string kJsonSchemaCountKey;
  static const std::string kJsonSchemaMaxVelocityKey;

  /**
   * Generates a particle with random velocity, as specified by the max velocity
   * constraint. Initializes the particle according to the specified type key
   * and the corresponding details in the type details json.Places the particle
   * on a random position within the bounds of the GasContainer.
   * @param random - a ci::Rand reference to use to generate random floats
   * @param max_velocity - the absolute value of the maximum velocity a particle
   *                       can have when starting to move
   * @param type_details - a json object containing information about a particle
   *                       and its color and radius
   * @param type_key - the name associated with this particle's information
   * @return a randomly generated GasParticle as specified
   */
  GasParticle GenerateRandomParticle(ci::Rand& random, float max_velocity,
                                     const nlohmann::json& type_details,
                                     const std::string& type_key) const;

  /**
   * Creates a GasParticle with the initial velocity, initial position, color,
   * and radius, as specified in the json objects provided.
   * @param particle_state - a json object containing information about this
   *                         particle's position and velocity
   * @param particle_types - a json object containing information about this
   *                         particle's color and radius
   * @return a GasParticle generated as specified
   */
  GasParticle CreateSpecifiedParticle(const nlohmann::json& particle_state,
                                      const nlohmann::json& particle_types)
  const;

  nlohmann::json SerializeParticle(const GasParticle& particle,
                                   nlohmann::json& particle_types) const;
};

} // namespace idealgas

#endif  // IDEAL_GAS_JSON_MANAGER_H
