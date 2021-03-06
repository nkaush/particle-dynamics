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
  /**
   * Creates a simulation.
   */
  IdealGasApp();

  /**
   * Draws the current state of the simulation.
   */
  void draw() override;

  /**
   * Advances the simulation 1 unit of time forward.
   */
  void update() override;

  /**
   * Detect key press events and route to relevant functions to perform action.
   * @param event - the KeyEvent triggered by a physical key press
   */
  void keyDown(cinder::app::KeyEvent event) override;

  // Define the size of the window to display the simulation
  static constexpr int kWindowWidth = 750;
  static constexpr int kWindowHeight = 500;

  const int kMargin = 100;

  static constexpr char kSaveToJsonKey = 's';

 private:
  // Stores the logic that runs the simulation
  SimulationEngine engine_;
};

}  // namespace idealgas
