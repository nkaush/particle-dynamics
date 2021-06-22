#include "gas_container.h"

#include <nlohmann/json.hpp>

namespace idealgas {

using std::vector;
using std::string;
using std::map;

using glm::vec2;

using nlohmann::json;

// Define the non-literal constants in this class
const char* GasContainer::kWallColor = "white";

GasContainer::GasContainer()
    : wall_color_(kWallColor),
      wall_bound_(vec2(kContainerLeftBound, kContainerUpperBound),
                  vec2(kContainerRightBound, kContainerLowerBound)) {}

GasContainer::GasContainer(const vector<GasParticle>& particles,
                           const map<string, ParticleSpecs>& specifications)
    : all_particles_(particles),
      particle_specifications_(specifications),
      wall_color_(kWallColor),
      wall_bound_(vec2(kContainerLeftBound, kContainerUpperBound),
                  vec2(kContainerRightBound, kContainerLowerBound)) {
  ConfigureTypePartition();
}

void GasContainer::ConfigureTypePartition() {
  std::sort(all_particles_.begin(),
            all_particles_.end(),
            [] (const GasParticle& a, const GasParticle& b) {
              return a.GetTypeName() < b.GetTypeName();
            });

  std::string current_type;

  if (all_particles_.size() > 0) {
    current_type = all_particles_.at(0).GetTypeName();
  }

  for (size_t idx = 0; idx < all_particles_.size(); idx++) {
    if (all_particles_.at(idx).GetTypeName() != current_type) {
      current_type = all_particles_.at(idx).GetTypeName();
      type_partition_.push_back(idx);
    }
  }

  type_partition_.push_back(all_particles_.size());
}

std::istream& operator>>(std::istream& input, GasContainer& container) {
  json json_data;
  input >> json_data;

  container = json_data.get<GasContainer>();

  for (GasParticle& particle : container.all_particles_) {
    particle.Configure(
        container.particle_specifications_.at(particle.GetTypeName()));
  }

  container.ConfigureTypePartition();

  return input;
}

std::ostream& operator<<(std::ostream& output, const GasContainer& container) {
  json serialized_container = container;
  output << std::setw(2) << serialized_container << std::endl;

  return output;
}


void GasContainer::Display() const {
  ci::gl::color(wall_color_);
  ci::gl::drawStrokedRect(wall_bound_);

  for (const GasParticle& particle : all_particles_) {
    particle.DrawParticle();
  }
}

const vector<GasParticle>& GasContainer::GetAllParticles() const {
  return all_particles_;
}

const std::vector<size_t>& GasContainer::GetTypePartition() const {
  return type_partition_;
}

void GasContainer::AdvanceOneFrame() {
  HandleParticleWallInteractions();
  HandleMultiParticleInteractions();

  for (GasParticle& particle : all_particles_) {
    particle.UpdatePosition();
  }
}

void GasContainer::HandleParticleWallInteractions() {
  for (GasParticle& particle : all_particles_) {
    bool is_colliding_at_vertical_walls = IsParticleCollidingWithAnyWallsOnAxis(
        particle, kXAxis, kContainerLeftBound, kContainerRightBound);

    bool is_colliding_at_horizontal_walls =
        IsParticleCollidingWithAnyWallsOnAxis(particle, kYAxis,
          kContainerUpperBound, kContainerLowerBound);

    particle.SetVelocity(CalculateParticleVelocityAfterWallCollision(particle,
      is_colliding_at_vertical_walls, is_colliding_at_horizontal_walls));
  }
}

vec2 GasContainer::CalculateParticleVelocityAfterWallCollision(
    const GasParticle& particle, bool is_collision_at_vertical_wall,
    bool is_collision_at_horizontal_wall) {

  vec2 initial_velo = particle.GetVelocity();
  vec2 new_velocity = vec2(initial_velo);

  // if colliding w/ vertical, invert x-velo by multiplying x component by -1
  if (is_collision_at_vertical_wall) {
    new_velocity[kXAxis] = initial_velo[kXAxis] * -1;
  }

  // if colliding w/ horizontal, invert y-velo by multiplying y component by -1
  if (is_collision_at_horizontal_wall) {
    new_velocity[kYAxis] = initial_velo[kYAxis] * -1;
  }

  return new_velocity;
}

bool GasContainer::IsParticleCollidingWithAnyWallsOnAxis(
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

  // Check if particles' relative velocities are opposite relative displacement
  if (glm::dot(velocity_difference, position_difference) >= 0) {
    return false;
  }

  float center_distance =
      glm::distance(particle_one.GetPosition(), particle_two.GetPosition());
  float radius_sum = particle_two.GetRadius() + particle_one.GetRadius();

  return center_distance <= radius_sum;
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

  return unique_types;
}

bool GasContainer::DoesParticleHaveSpecifications(
    const GasParticle& particle, const ParticleSpecs& specification) const {
  bool has_same_label = particle.GetTypeName() == specification.name;
  bool has_same_mass = particle.GetMass() == specification.mass;
  bool has_same_radius = particle.GetRadius() == specification.radius;

  bool has_same_characteristics = has_same_radius && has_same_label
                                  && has_same_mass;

  bool has_same_color = particle.GetColor() == specification.color;

  return has_same_characteristics && has_same_color;
}

}  // namespace idealgas
