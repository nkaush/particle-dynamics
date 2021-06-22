//
// Created by Neil Kaushikkar on 3/12/21.
//

#include "gas_particle.h"

namespace idealgas {

using glm::vec2;
using std::string;

GasParticle::GasParticle(const vec2& initial_pos, const vec2& initial_velo,
                         const ParticleSpecs& specs) :
      position_(initial_pos), velocity_(initial_velo), radius_(specs.radius),
      mass_(specs.mass), color_(specs.color), particle_type_name_(specs.name) {}

GasParticle::GasParticle(const vec2& initial_pos, const vec2& initial_velo,
                         float radius_to_set, float mass_to_set, float red,
                         float green, float blue, const string& name) :
      position_(initial_pos), velocity_(initial_velo), radius_(radius_to_set),
      mass_(mass_to_set), color_(red * 255.0, green * 255.0, blue * 255.0), particle_type_name_(name) {
}

void GasParticle::Configure(const ParticleSpecs& specs) {
  radius_ = specs.radius;
  mass_ = specs.mass;
  color_ = specs.color;
}

void GasParticle::UpdatePosition() {
  position_ += velocity_;
}

void GasParticle::DrawParticle() const {
  ci::gl::color(color_);
  ci::gl::drawSolidCircle(position_, radius_);
}

void GasParticle::SetVelocity(const glm::vec2& new_velocity) {
  velocity_ = new_velocity;
}

const glm::vec2& GasParticle::GetVelocity() const {
  return velocity_;
}

const glm::vec2& GasParticle::GetPosition() const {
  return position_;
}

float GasParticle::GetRadius() const {
  return radius_;
}

float GasParticle::GetMass() const {
  return mass_;
}

const string& GasParticle::GetTypeName() const {
    return particle_type_name_;
}

const ci::Color8u& GasParticle::GetColor() const {
  return color_;
}

ParticleSpecs GasParticle::GetParticleTypeDetails() const {
  return {radius_, mass_, color_, particle_type_name_};
}

} // namespace idealgas

