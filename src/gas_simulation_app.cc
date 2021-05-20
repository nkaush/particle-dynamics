#include "gas_simulation_app.h"

namespace idealgas {

using cinder::app::KeyEvent;

IdealGasApp::IdealGasApp() : engine_(SimulationEngine(true)) {
  ci::app::setWindowSize(kWindowWidth, kWindowHeight);
}

void IdealGasApp::draw() {
  ci::Color background_color("black");
  ci::gl::clear(background_color);

  engine_.Render();
}

void IdealGasApp::update() {
  engine_.AdvanceToNextFrame();
}

void IdealGasApp::keyDown(KeyEvent event) {
  if (event.getChar() == kSaveToJsonKey) {
    engine_.SaveSimulation();
    console() << "Simulation Saved!" << std::endl;
  }
}

}  // namespace idealgas
