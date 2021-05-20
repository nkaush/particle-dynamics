#include "simulation_engine.h"

namespace idealgas {

using glm::vec2;
using std::string;
using std::vector;
using std::map;

const string SimulationEngine::kJsonSavedFilePath =
    "data/saved_simulation.json";

const string SimulationEngine::kJsonRandomSimulationFilePath =
    "data/random_simulation_generator.json";

SimulationEngine::SimulationEngine(bool load_from_saved_file) :
      json_manager_(), container_(ContainerInitializer(load_from_saved_file)),
      histograms_({}) {
  vector<ParticleSpecs> particle_types = container_.FindUniqueParticleTypes();

  float x_coordinate = kDefaultHistogramXCoordinate;
  float y_coordinate = kHistogramDisplayPadding;
  float bin_range = Histogram::kDefaultSingleBinRange;
  size_t bin_count = Histogram::kDefaultBinCount;

  for (const ParticleSpecs& specs : particle_types) {
    histograms_.emplace_back(specs.name, bin_count, bin_range,
                             x_coordinate, y_coordinate, specs.color);
    y_coordinate += Histogram::kDefaultGraphHeight + kHistogramDisplayPadding;
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
    hist.UpdateBinDistribution(particle_speeds.at(hist.GetDataLabel()));
  }
}

void SimulationEngine::Render() {
  container_.Display();
  for (const Histogram& hist : histograms_) {
    hist.Draw();
  }
}

}  // namespace idealgas
