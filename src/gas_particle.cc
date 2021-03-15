//
// Created by Neil Kaushikkar on 3/12/21.
//
#include <gas_particle.h>

namespace idealgas {

using glm::vec2;
using std::vector;
using glm::dot;

GasParticle::GasParticle(const vec2& initial_pos, const vec2& initial_velo,
                         float radius) :
    position_(initial_pos), velocity_(initial_velo), radius_(radius),
    red_color_intensity_(255), green_color_intensity_(255),
    blue_color_intensity_(255) {}

GasParticle::GasParticle(const vec2& initial_pos, const vec2& initial_velo,
                         float radius_to_set, int red, int green, int blue) :
      position_(initial_pos), velocity_(initial_velo), radius_(radius_to_set),
      red_color_intensity_(red), green_color_intensity_(green),
      blue_color_intensity_(blue) {}

void GasParticle::UpdatePosition() {
  position_ += velocity_;
}

glm::vec2 GasParticle::GetVelocity() const {
  return velocity_;
}

glm::vec2 GasParticle::GetPosition() const {
  return position_;
}

float GasParticle::GetRadius() const {
  return radius_;
}

void GasParticle::DrawParticle() const {
  ci::gl::color(ci::Color(red_color_intensity_,
                          green_color_intensity_,
                          blue_color_intensity_));
  ci::gl::drawSolidCircle(position_, radius_);
}

void GasParticle::SetVelocity(const glm::vec2& new_velocity) {
  velocity_ = new_velocity;
}

} // namespace idealgas

