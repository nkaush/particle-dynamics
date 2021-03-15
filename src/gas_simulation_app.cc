#include "gas_simulation_app.h"

namespace idealgas {

IdealGasApp::IdealGasApp() : engine_(SimulationEngine(false)) {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void IdealGasApp::draw() {
  ci::Color background_color("black");
  ci::gl::clear(background_color);

  engine_.Render();
}

void IdealGasApp::update() {
  engine_.AdvanceToNextFrame();
}

}  // namespace idealgas
