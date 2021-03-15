#include "gas_container.h"

namespace idealgas {

using glm::vec2;
using std::vector;

GasContainer::GasContainer() {
  GasParticle g = GasParticle(vec2(300, 200), vec2(-1, 0), 10, 200, 100, 200);
  GasParticle f = GasParticle(vec2(500, 200), vec2(1, 0), 10, 200, 100, 200);

  all_particles_.push_back(g);
  all_particles_.push_back(f);
}

void GasContainer::Display() const {
  // This function has a lot of magic numbers; be sure to design your code in a way that avoids this.
  ci::gl::color(ci::Color("white"));
  vec2 top_left_point = vec2(kContainerLeftBound, kContainerUpperBound);
  vec2 bottom_right_point = vec2(kContainerRightBound, kContainerLowerBound);
  ci::gl::drawStrokedRect(ci::Rectf(top_left_point, bottom_right_point));

  for (const GasParticle& particle : all_particles_) {
    ci::gl::color(ci::Color("orange"));
    ci::gl::drawSolidCircle(particle.GetPosition(), 10);
  }
}

void GasContainer::AdvanceOneFrame() {
  // TODO: MOVE LOGIC to new simulation logic class
  size_t num_particles = all_particles_.size();

  for (size_t i = 0; i < num_particles; i++) {
    GasParticle& particle_one = all_particles_[i];

    vec2 velocity_after_wall_collision =
        CalculateParticleVelocityAfterWallCollision(particle_one);

    glm::bvec2 axis_valus_equality =
        glm::equal(velocity_after_wall_collision, particle_one.GetVelocity());

    // If at least 1 of velocity values (x, y) changed, we must update velocity
    if (!glm::all(axis_valus_equality)) {
      particle_one.SetVelocity(velocity_after_wall_collision);
    }

    for (size_t k = i + 1; k < num_particles; k++) {
      GasParticle& particle_two = all_particles_[k];

      if (AreParticlesColliding(particle_one, particle_two)) {
        vec2 particle_one_new_velocity =
            CalculateParticleVelocityAfterCollision(particle_one, particle_two);
        vec2 particle_two_new_velocity =
            CalculateParticleVelocityAfterCollision(particle_two, particle_one);

        particle_one.SetVelocity(particle_one_new_velocity);
        particle_two.SetVelocity(particle_two_new_velocity);
      }
    }
  }

  for (GasParticle& particle : all_particles_) {
    particle.UpdatePosition();
  }
}

bool GasContainer::AreParticlesColliding(GasParticle& particle_one,
                                             GasParticle& particle_two) const {
  vec2 velocity_difference = particle_one.GetVelocity()
                              - particle_two.GetVelocity();
  vec2 position_difference = particle_one.GetPosition()
                              - particle_two.GetPosition();

  float center_distance =
      glm::distance(particle_one.GetPosition(), particle_two.GetPosition());
  float radius_sum = particle_two.GetRadius() + particle_one.GetRadius();

  return glm::dot(velocity_difference, position_difference) < 0 && center_distance <= radius_sum;
}

vec2 GasContainer::CalculateParticleVelocityAfterWallCollision(
    const GasParticle& particle) {
  vec2 initial_velocity = particle.GetVelocity();
  vec2 new_velocity = vec2(initial_velocity);
  vec2 position = particle.GetPosition();
  float radius = particle.GetRadius();

  // If particle collides with vertical walls (x-axis = index 0)
  bool is_colliding_at_left_wall = position[0] - radius <= kContainerLeftBound;
  bool is_colliding_at_right_wall = position[0] + radius >= kContainerRightBound;

  if (is_colliding_at_left_wall || is_colliding_at_right_wall) {
    new_velocity[0] = initial_velocity[0] * -1;
  }

  // If particle collides with horizontal walls (y-axis = index 1)
  bool is_colliding_at_upper_wall = position[1] - radius <= kContainerUpperBound;
  bool is_colliding_at_lower_wall = position[1] + radius >= kContainerLowerBound;

  if (is_colliding_at_upper_wall || is_colliding_at_lower_wall) {
    new_velocity[1] = initial_velocity[1] * -1;
  }

  return new_velocity;
}

vec2 GasContainer::CalculateParticleVelocityAfterCollision(
    const GasParticle& particle_one, const GasParticle& particle_two) {
  vec2 velo_diff = particle_one.GetVelocity() - particle_two.GetVelocity();
  vec2 pos_diff = particle_one.GetPosition() - particle_two.GetPosition();

  float velo_pos_dot_product = dot(velo_diff, pos_diff);
  float pos_diff_length = glm::length(pos_diff);
  float squared_pos_diff_length = pos_diff_length * pos_diff_length;
  float velo_change_scalar = velo_pos_dot_product / squared_pos_diff_length;
  vec2 velocity_change = velo_change_scalar * pos_diff;

  return particle_one.GetVelocity() - velocity_change;
}

}  // namespace idealgas
