#include "gas_container.h"

namespace idealgas {

using glm::vec2;
using std::vector;

// Define the non-literal constants in this class
const char* GasContainer::kWallColor = "white";

GasContainer::GasContainer(
    const vector<GasParticle>& particles) : all_particles_(particles) {}

void GasContainer::Display() const {
  ci::gl::color(ci::Color(kWallColor));
  // find the bounding rectangle points using the wall bounds
  vec2 top_left_point = vec2(kContainerLeftBound, kContainerUpperBound);
  vec2 bottom_right_point = vec2(kContainerRightBound, kContainerLowerBound);
  ci::gl::drawStrokedRect(ci::Rectf(top_left_point, bottom_right_point));

  for (const GasParticle& particle : all_particles_) {
    particle.DrawParticle();
  }
}

vector<GasParticle> GasContainer::GetAllParticles() const {
  return all_particles_;
}

void GasContainer::AdvanceOneFrame() {
  HandleParticleWallInteractions();
  HandleMultiParticleInteractions();

  for (GasParticle& particle : all_particles_) {
    particle.UpdatePosition();
  }
}

void GasContainer::HandleParticleWallInteractions() {
  size_t num_particles = all_particles_.size();

  for (size_t i = 0; i < num_particles; i++) {
    GasParticle& particle_one = all_particles_[i];

    // Check for a new velocity as if the particle is near a wall
    vec2 velocity_after_wall_collision =
        CalculateParticleVelocityAfterWallCollision(particle_one);

    // TODO: refactor this so that we determine which walls are being collided with here in this method
    // TODO: pass in the walls colliding with (as bools?) to a method that completes the velocity change

    glm::bvec2 axis_valus_equality =
        glm::equal(velocity_after_wall_collision, particle_one.GetVelocity());

    // If at least 1 of velocity values (x, y) changed, we must update velocity
    if (!glm::all(axis_valus_equality)) {
      particle_one.SetVelocity(velocity_after_wall_collision);
    }
  }
}

vec2 GasContainer::CalculateParticleVelocityAfterWallCollision(
    const GasParticle& particle) {
  vec2 initial_velo = particle.GetVelocity();
  vec2 new_velocity = vec2(initial_velo);

  bool is_collision_at_vertical_wall = IsParticleCollidingWithWallsOnAxis(
      particle, kXAxis, kContainerLeftBound, kContainerRightBound);

  // if colliding w/ vertical, invert x-velo by multiplying x component by -1
  if (is_collision_at_vertical_wall) {
    new_velocity[kXAxis] = initial_velo[kXAxis] * -1;
  }

  bool is_collision_at_horizontal_wall = IsParticleCollidingWithWallsOnAxis(
      particle, kYAxis, kContainerUpperBound, kContainerLowerBound);

  // if colliding w/ horizontal, invert y-velo by multiplying y component by -1
  if (is_collision_at_horizontal_wall) {
    new_velocity[kYAxis] = initial_velo[kYAxis] * -1;
  }

  return new_velocity;
}

bool GasContainer::IsParticleCollidingWithWallsOnAxis(
    const GasParticle& particle, size_t axis_index,
    float min_wall_bound, float max_wall_bound) {
  // Check if particle is at or past the specified bounds
  float position_component = particle.GetPosition()[axis_index];
  float radius = particle.GetRadius();
  // If the particle is behind/at the wall closer to the origin
  bool is_colliding_at_min_wall_bound = position_component - radius
                                        <= min_wall_bound;
  // If the particle is past/at the wall furthest from the origin
  bool is_colliding_at_max_wall_bound = position_component + radius
                                        >= max_wall_bound;

  float velocity_component = particle.GetVelocity()[axis_index];
  // Ensure the particle is moving away from the walls to avoid getting stuck
  is_colliding_at_min_wall_bound &= velocity_component < 0;
  is_colliding_at_max_wall_bound &= velocity_component > 0;

  // Only change the velocity if the particle is approaching a wall
  return is_colliding_at_min_wall_bound || is_colliding_at_max_wall_bound;
}


void GasContainer::HandleMultiParticleInteractions() {
  size_t num_particles = all_particles_.size();

  for (size_t i = 0; i < num_particles; i++) {
    GasParticle& particle_one = all_particles_[i];

    // Only check particles AFTER current one since already checked ones BEFORE
    for (size_t k = i + 1; k < num_particles; k++) {
      GasParticle& particle_two = all_particles_[k];

      // If particles are colliding, update their velocities accordingly
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

  // Check if particles' relative velocities are opposite relative displacement
  bool are_moving_towards_each_other =
      glm::dot(velocity_difference, position_difference) < 0;

  return are_moving_towards_each_other && center_distance <= radius_sum;
}

vec2 GasContainer::CalculateParticleVelocityAfterCollision(
    const GasParticle& particle_one, const GasParticle& particle_two) {
  vec2 velo_diff = particle_one.GetVelocity() - particle_two.GetVelocity();
  vec2 pos_diff = particle_one.GetPosition() - particle_two.GetPosition();
  float mass_sum = particle_one.GetMass() + particle_two.GetMass();

  float velo_pos_dot_product = dot(velo_diff, pos_diff);
  float pos_diff_length = glm::length(pos_diff);
  float mass_scalar = (2 * particle_two.GetMass()) / mass_sum;

  float squared_pos_diff_length = pos_diff_length * pos_diff_length;
  float velo_change_scalar = velo_pos_dot_product / squared_pos_diff_length;
  vec2 velocity_change = mass_scalar * velo_change_scalar * pos_diff;

  return particle_one.GetVelocity() - velocity_change;
}

vector<ParticleSpecs> GasContainer::FindUniqueParticleTypes() const {
  vector<ParticleSpecs> unique_types;

  for (const GasParticle& particle : all_particles_) {
    bool similar_particle_already_found = false;

    for (const ParticleSpecs& specs : unique_types) {
      if (DoesParticleHaveSpecifications(particle, specs)) {
        similar_particle_already_found = true;
        break;
      }
    }

    if (!similar_particle_already_found) {
      unique_types.push_back(particle.GetParticleTypeDetails());
    }
  }
ef
  return unique_types;
}

bool GasContainer::DoesParticleHaveSpecifications(
    const GasParticle& particle_one, const ParticleSpecs& specification) const {
  // TODO: implement this
  return true;
}

}  // namespace idealgas
