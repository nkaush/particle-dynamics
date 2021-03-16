//
// Created by Neil Kaushikkar on 3/15/21.
//

#include <nlohmann/json.hpp>

#include "gas_container.h"
#include "cinder/Rand.h"

#ifndef IDEAL_GAS_SIMULATION_ENGINE_H
#define IDEAL_GAS_SIMULATION_ENGINE_H

#endif  // IDEAL_GAS_SIMULATION_ENGINE_H

namespace idealgas {

/**
 * This class manages the simulation by starting it, saving it, and rendering it.
 */
class SimulationEngine {
 public:
  // Store the file paths to read/save json from/to
  static const std::string kJsonStateFilePath;
  static const std::string kJsonRandomSimulationFilePath;

  /**
   * Creates a GasContainer for this simulation from either the saved
   * simulation json file or from the json file indicating the parameters
   * for a randomly generated simulation.
   * @param load_from_saved_file - indicates whether to load simulation from
   * a saved json file (true) or randomly generate simulation (false)
   */
  SimulationEngine(bool load_from_saved_file);

  /**
   * This is a helper function for the constructor that reroutes to the
   * necessary simulation generation function based on whether to load
   * a saved simulation or random simulation.
   * @param load_from_saved_file - indicates whether to load simulation from
   * a saved json file (true) or randomly generate simulation (false)
   * @return a GasContainer generated as specified
   */
  GasContainer ContainerInitializer(bool load_from_saved_file) const;

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
   * Saves the current state of the simulation in a json file.
   */
  void SaveContainerToJson(const std::string& save_file_path) const;

  /**
   * Steps the simulation 1 unit in time. Updates the GasContainer accordingly.
   */
  void AdvanceToNextFrame();

  /**
   * Display the current state of the simulation.
   */
  void Render();

 private:
  // The full directory path to this project
  static const std::string kBaseFilePath;

  // These keys access the subsections of the json: motion and visuals
  static const std::string kJsonSchemaParticleStatesKey;
  static const std::string kJsonSchemaParticleTypesKey;

  // These keys access info about a particle's motion
  static const std::string kJsonSchemaVelocityKey;
  static const std::string kJsonSchemaPositionKey;

  // These keys access info about the visuals of the particles
  static const std::string kJsonSchemaTypeKey;
  static const std::string kJsonSchemaRedKey;
  static const std::string kJsonSchemaGreenKey;
  static const std::string kJsonSchemaBlueKey;
  static const std::string kJsonSchemaRadiusKey;

  // These fields are used as constraints for generating GasContainers randomly
  static const std::string kJsonSchemaParticleCountKey;
  static const std::string kJsonSchemaMaxVelocityKey;

  GasContainer container_;
};

}  // namespace idealgas