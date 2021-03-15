//
// Created by Neil Kaushikkar on 3/15/21.
//

#include <nlohmann/json.hpp>

#include "gas_container.h"

#ifndef IDEAL_GAS_SIMULATION_ENGINE_H
#define IDEAL_GAS_SIMULATION_ENGINE_H

#endif  // IDEAL_GAS_SIMULATION_ENGINE_H

namespace idealgas {

class SimulationEngine {
 public:
  SimulationEngine(bool load_from_saved_file);

  void AdvanceToNextFrame();

  void Render();

  GasContainer GenerateRandomContainerFromJson() const;

  GasContainer LoadContainerFromJson() const;

  GasContainer SaveContainerToJson() const;

 private:
  static const std::string kBaseDataFilePath;
  static const std::string kJsonStateFilePath;
  static const std::string kJsonRandomSimulationFilePath;

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
  static const std::string kJsonSchemaVelocityRangeKey;

  GasContainer container_;
};

}  // namespace idealgas