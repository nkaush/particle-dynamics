#include "simulation_engine.h"

namespace idealgas {

using glm::vec2;
using std::string;

const string SimulationEngine::kBaseFilePath =
    "/Users/neilkaushikkar/Cinder/my-projects/ideal-gas-nkaush/";

const string SimulationEngine::kJsonSavedFilePath = kBaseFilePath
    + "src/data/saved_simulation.json";

const string SimulationEngine::kJsonRandomSimulationFilePath = kBaseFilePath
    + "src/data/random_simulation_generator.json";

SimulationEngine::SimulationEngine(bool load_from_saved_file) :
      json_manager_(), container_(ContainerInitializer(load_from_saved_file)) {}

GasContainer SimulationEngine::ContainerInitializer(
    bool load_from_saved_file) const {
  if (load_from_saved_file) {
    return json_manager_.LoadContainerFromJson(kJsonSavedFilePath);
  } else {
    return json_manager_.GenerateRandomContainerFromJson(
        kJsonRandomSimulationFilePath);
  }
}

void SimulationEngine::SaveSimulation() {
  json_manager_.WriteContainerToJson(container_, kJsonSavedFilePath);
}

void SimulationEngine::AdvanceToNextFrame() {
  container_.AdvanceOneFrame();
}

void SimulationEngine::Render() {
  container_.Display();
}

}  // namespace idealgas