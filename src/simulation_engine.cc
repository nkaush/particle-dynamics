#include "simulation_engine.h"

namespace idealgas {

using glm::vec2;
using std::string;
using std::vector;
using std::map;

const string SimulationEngine::kBaseFilePath =
    "/Users/neilkaushikkar/Cinder/my-projects/ideal-gas-nkaush/";

const string SimulationEngine::kJsonSavedFilePath = kBaseFilePath
    + "src/data/saved_simulation.json";

const string SimulationEngine::kJsonRandomSimulationFilePath = kBaseFilePath
    + "src/data/random_simulation_generator.json";

SimulationEngine::SimulationEngine(bool load_from_saved_file) :
      json_manager_(), container_(ContainerInitializer(load_from_saved_file)),
      histograms_({}) {
  vector<ParticleSpecs> particle_types = container_.FindUniqueParticleTypes();
  float x_coordinate = Histogram::kDefaultXCoordinate;
  float y_coordinate = 50;

  for (const ParticleSpecs& specs : particle_types) {
    histograms_.emplace_back(specs.name, x_coordinate, y_coordinate,
                             specs.red_intensity, specs.green_intensity,
                             specs.blue_intensity);
    y_coordinate += Histogram::kDefaultGraphHeight + 50;
  }
}

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
  UpdateHistograms();
}

void SimulationEngine::UpdateHistograms() {
  map<string, vector<float>> particle_speeds;

  for (const GasParticle& particle : container_.GetAllParticles()) {
    float speed = glm::length(particle.GetVelocity());
    particle_speeds[particle.GetTypeName()].push_back(speed);
  }

  // Can't declare hist a const reference since we need to update internal state
  for (Histogram& hist : histograms_) {
    hist.UpdateBinCounts(particle_speeds.at(hist.GetDataLabel()));
  }
}

void SimulationEngine::Render() {
  container_.Display();
  for (const Histogram& hist : histograms_) {
    hist.Draw();
  }
}

}  // namespace idealgas