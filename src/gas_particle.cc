//
// Created by Neil Kaushikkar on 3/12/21.
//
#include <gas_particle.h>

#include <nlohmann/json.hpp>

namespace idealgas {

using glm::vec2;
using std::string;
using nlohmann::json;

GasParticle::GasParticle(const vec2& initial_pos, const vec2& initial_velo,
                         const ParticleSpecs& specs) : position_(initial_pos),
      velocity_(initial_velo), radius_(specs.radius), mass_(1),
      red_color_intensity_(specs.red), green_color_intensity_(specs.green),
      blue_color_intensity_(specs.blue), particle_type_name_(specs.name){}

GasParticle::GasParticle(const vec2& initial_pos, const vec2& initial_velo,
                         float radius_to_set, float mass_to_set, float red,
                         float green, float blue, const string& name) :
      position_(initial_pos), velocity_(initial_velo), radius_(radius_to_set),
      mass_(mass_to_set), red_color_intensity_(red),
      green_color_intensity_(green), blue_color_intensity_(blue),
      particle_type_name_(name) {}

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

float GasParticle::GetMass() const {
  return mass_;
}

float GasParticle::GetRedIntensity() const {
  return red_color_intensity_;
}

float GasParticle::GetGreenIntensity() const {
  return green_color_intensity_;
}

float GasParticle::GetBlueIntensity() const {
  return blue_color_intensity_;
}

string GasParticle::GetTypeName() const {
    return particle_type_name_;
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

