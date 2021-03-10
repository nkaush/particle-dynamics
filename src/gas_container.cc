#include "gas_container.h"

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer() {

}

void GasContainer::Display() const {
  // This function has a lot of magic numbers; be sure to design your code in a way that avoids this.
  ci::gl::color(ci::Color("orange"));
  ci::gl::drawSolidCircle(vec2(dummy_variable_, 200), 10);
  ci::gl::color(ci::Color("white"));
  ci::gl::drawStrokedRect(ci::Rectf(vec2(100, 100), vec2(600, 400)));
}

void GasContainer::AdvanceOneFrame() {
  ++dummy_variable_;
}

}  // namespace idealgas
