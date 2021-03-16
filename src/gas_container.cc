#include "gas_container.h"

namespace idealgas {

using glm::vec2;
using std::vector;

GasContainer::GasContainer(const vector<GasParticle>& particles) :
      all_particles_(particles) {}

void GasContainer::Display() const {
  // This function has a lot of magic numbers; be sure to design your code in a way that avoids this.
  ci::gl::color(ci::Color("white"));
  vec2 top_left_point = vec2(kContainerLeftBound, kContainerUpperBound);
  vec2 bottom_right_point = vec2(kContainerRightBound, kContainerLowerBound);
  ci::gl::drawStrokedRect(ci::Rectf(top_left_point, bottom_right_point));

  for (const GasParticle& particle : all_particles_) {
    ci::gl::color(ci::Color(particle.GetRedIntensity(),
                            particle.GetGreenIntensity(),
                            particle.GetBlueIntensity()));
    ci::gl::drawSolidCircle(particle.GetPosition(), particle.GetRadius());
  }
}

vector<GasParticle> GasContainer::GetAllParticles() const {
  return all_particles_;
}

void GasContainer::AdvanceOneFrame() {
  HandleParticleInteractions();

  for (GasParticle& particle : all_particles_) {
    particle.UpdatePosition();
  }
}

void GasContainer::HandleParticleInteractions() {
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
}

bool GasContainer::AreParticlesColliding(const GasParticle& particle_one,
                                         const GasParticle& particle_two)  {
  vec2 velocity_difference = particle_one.GetVelocity()
                              - particle_two.GetVelocity();
  vec2 position_difference = particle_one.GetPosition()
                              - particle_two.GetPosition();

  float center_distance =
      glm::distance(particle_one.GetPosition(), particle_two.GetPosition());
  float radius_sum = particle_two.GetRadius() + particle_one.GetRadius();

  bool are_moving_towards_each_other =
      glm::dot(velocity_difference, position_difference) < 0;

  return are_moving_towards_each_other && center_distance <= radius_sum;
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

vec2 GasContainer::CalculateParticleVelocityAfterWallCollision(
    const GasParticle& particle) {
  vec2 initial_velo = particle.GetVelocity();
  vec2 new_velocity = vec2(initial_velo);

  size_t x_axis_idx = 0;
  size_t y_axis_idx = 1;

  bool is_collision_at_vertical_wall =
      IsParticleCollidingWithWalls(particle, x_axis_idx, kContainerLeftBound,
                                   kContainerRightBound);

  if (is_collision_at_vertical_wall) {
    // invert the x-velocity by multiplying that component by -1
    new_velocity[x_axis_idx] = initial_velo[x_axis_idx] * -1;
  }

  bool is_collision_at_horizontal_wall =
      IsParticleCollidingWithWalls(particle, y_axis_idx, kContainerUpperBound,
                                   kContainerLowerBound);

  if (is_collision_at_horizontal_wall) {
    // invert the y-velocity by multiplying that component by -1
    new_velocity[y_axis_idx] = initial_velo[y_axis_idx] * -1;
  }

  return new_velocity;
}

bool GasContainer::IsParticleCollidingWithWalls(const GasParticle& particle,
                                                size_t axis_idx,
                                                float min_wall_bound,
                                                float max_wall_bound) {
  // Check if particle is at or past the specified bounds
  float position_component = particle.GetPosition()[axis_idx];
  float radius = particle.GetRadius();
  bool is_colliding_at_lower_bound_wall = position_component - radius
                                          <= min_wall_bound;
  bool is_colliding_at_upper_bound_wall = position_component + radius
                                          >= max_wall_bound;

  float velocity_component = particle.GetVelocity()[axis_idx];
  // Ensure the particle is moving away from the walls to avoid getting stuck
  is_colliding_at_lower_bound_wall &= velocity_component < 0;
  is_colliding_at_upper_bound_wall &= velocity_component > 0;

  // Only change the velocity if the particle is approaching a wall
  return is_colliding_at_lower_bound_wall || is_colliding_at_upper_bound_wall;
}

}  // namespace idealgas
