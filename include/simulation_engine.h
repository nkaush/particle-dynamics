//
// Created by Neil Kaushikkar on 3/15/21.
//
#include "json_manager.h"
#include "histogram.h"

#ifndef IDEAL_GAS_SIMULATION_ENGINE_H
#define IDEAL_GAS_SIMULATION_ENGINE_H

namespace idealgas {

/**
 * This class manages the simulation by starting it, saving it, and rendering it.
 */
class SimulationEngine {
 public:
  // Store the file paths to read/save json from/to
  static const std::string kJsonSavedFilePath;
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
   * Called when the user prompts to save the current state of the simulation.
   * @param save_file_path - a string indicating the path to save the file to
   */
   void SaveSimulation();

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

  JsonManager json_manager_;
  GasContainer container_;
  std::vector<Histogram> histograms_;

  std::vector<float> velocities_;
};

}  // namespace idealgas

#endif  // IDEAL_GAS_SIMULATION_ENGINE_H