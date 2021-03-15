#pragma once

#include "Cinder/app/App.h"
#include "Cinder/app/RendererGl.h"
#include "Cinder/gl/gl.h"
#include "gas_container.h"
#include "simulation_engine.h"

namespace idealgas {

/**
 * An app for visualizing the behavior of an ideal gas.
 */
class IdealGasApp : public ci::app::App {
 public:
  IdealGasApp();

  void draw() override;
  void update() override;

  // TODO: Delete this comment. Feel free to play around with these variables
  // provided that you can see the entire UI on your screen.
  const int kWindowSize = 700;
  const int kMargin = 100;

 private:
  SimulationEngine engine_;
};

}  // namespace idealgas
