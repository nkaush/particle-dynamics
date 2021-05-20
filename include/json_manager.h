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
   * Saves the current state of the simulation in a json file.
   */
  void WriteContainerToJson(const GasContainer& container,
                            const std::string& save_file_path) const;

 private:
  // These keys access the subsections of the json: motion and visuals
  static const std::string kJsonSchemaParticleTypesKey;
  static const std::string kJsonSchemaParticleCountsKey;

  /**
   * Generates a particle with random velocity, as specified by the max velocity
   * constraint. Initializes the particle according to the specified type key
   * and the corresponding details in the type details json.Places the particle
   * on a random position within the bounds of the GasContainer.
   * @param random - a ci::Rand reference to use to generate random floats
   * @param max_velocity - the absolute value of the maximum velocity a particle
   *                       can have when starting to move
   * @param specifications -
   * @return a randomly generated GasParticle as specified
   */
  GasParticle GenerateRandomParticle(
      ci::Rand& random, float max_velo, const ParticleSpecs& specifications) const;
};

} // namespace idealgas

#endif  // IDEAL_GAS_JSON_MANAGER_H
