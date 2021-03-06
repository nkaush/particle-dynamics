#ifndef IDEAL_GAS_GAS_CONTAINER_H
#define IDEAL_GAS_GAS_CONTAINER_H

#include "cinder/gl/gl.h"
#include "gas_particle.h"
#include <string>
#include <map>

namespace idealgas {

struct ContainerSpecifications {
  std::string particle_name;
  size_t count;
  float max_velocity;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    ContainerSpecifications, particle_name, count, max_velocity);

/**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
class GasContainer {
 public:
  // The color to display the walls in
  static const char* kWallColor;

  // Define the bounds of the container on top, bottom, left, and right walls
  static constexpr float kContainerUpperBound = 50;
  static constexpr float kContainerLowerBound = 450;
  static constexpr float kContainerLeftBound = 300;
  static constexpr float kContainerRightBound = 700;

  // Used to access values corresponding to an specific axis
  static constexpr size_t kXAxis = 0;
  static constexpr size_t kYAxis = 1;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(
      GasContainer, all_particles_, particle_specifications_);

  GasContainer();

  /**
   * Initializes a GasContainer and populates it with the particles given in the
   * provided vector of GasParticles.
   * @param particles - a vector of GasParticle to add to the container
   * @param specifications
   */
  GasContainer(const std::vector<GasParticle>& particles,
               const std::map<std::string, ParticleSpecs>& specifications);

  void Configure();

  /**
   * Displays the container walls and the current positions of the particles.
   */
  void Display() const;

  /**
   * Moves the simulation one step forward, allowing particles to interact with
   * each other and advance based on those interactions.
   */
  void AdvanceOneFrame();

  /**
   * Getter for the particles in this container.
   * @return a vector of GasParticles in this GasContainer
   */
  std::vector<GasParticle> GetAllParticles() const;

  /**
   * Find each of the unique particle types by comparing all of the particles in
   * this container for same mass, radius, and color (NOT velocity or position).
   * @return a vector of ParticleSpecs indicating the mass, radius, and color
   * intensity values of each unique particle in the container
   */
  std::vector<ParticleSpecs> FindUniqueParticleTypes() const;

 private:
  // stores the particles in the container
  std::vector<GasParticle> all_particles_;

  std::map<std::string, ParticleSpecs> particle_specifications_;

  ci::Color wall_color_;
  ci::Rectf wall_bound_;

  /**
   * Handles the logic of all particle interactions with walls and adjusts
   * particle velocities according to the laws of physics.
   */
  void HandleParticleWallInteractions();

  /**
   * Handles the logic of all particle interactions with other particles and
   * adjusts both particle velocities according to the laws of physics.
   */
  void HandleMultiParticleInteractions();

  /**
   * Calculates the velocity of a particle depending on which walls the particle
   * is moving towards and is touching or overlapping.
   * @param particle - the particle to calculate velocity for
   * @param is_collision_at_vertical_wall - a bool indicating whether the
   * particle has collided with a vertical wall
   * @param is_collision_at_horizontal_wall - a bool indicating whether the
   * particle has collided with a horizontal wall
   * @return a vec2 indicating the new velocity if a particle hit a wall
   */
  static glm::vec2 CalculateParticleVelocityAfterWallCollision(
      const GasParticle& particle, bool is_collision_at_vertical_wall,
      bool is_collision_at_horizontal_wall);

  /**
   * Checks whether a particle is colliding with any walls parallel to those
   * given by the specified axis index and the specified wall bounds.
   * @param particle - a particle to wall collisions for
   * @param axis_idx - the axis parallel to the the walls we are checking
   * @param min_wall_bound - the coordinate of the wall closest to the origin
   *                         and is parallel to the axis provided
   * @param max_wall_bound - the coordinate of the wall furthest from the origin
   *                         and is parallel to the axis provided
   * @return a bool indicating whether the given particle is colliding with the
   * given walls
   */
  static bool IsParticleCollidingWithAnyWallsOnAxis(
      const GasParticle& particle, size_t axis_idx,
      float min_wall_bound, float max_wall_bound);

  /**
   * Checks whether two particles are colliding - whether they are touching or
   * overlapping AND whether they are moving towards each other to collide.
   * @param particle_one - the first particle to check
   * @param particle_two - the second particle to check
   * @return a bool indicating whether the two particles are colliding
   */
  static bool AreParticlesColliding(const GasParticle& particle_one,
                                    const GasParticle& particle_two);

  /**
   * Computes the velocity of the 1st particle if it collides with the 2nd one.
   * @param particle_one - the particle to calculate a velocity for
   * @param particle_two - the particle the 1st particle is colliding with
   * @return a vec2 indicating the new velocity of the 1st particle
   */
  static glm::vec2 CalculateParticleVelocityAfterCollision(
      const GasParticle& particle_one, const GasParticle& particle_two);

  /**
   * Determines whether a particle's characteristics match those defined in the
   * provided ParticleSpecs struct.
   * @param particle_one - the particle to check for certain characteristics
   * @param specification - the particle characteristics to check for
   * @return a bool indicating whether the particle's characteristics match the
   * characteristics defined in the struct provided
   */
  bool DoesParticleHaveSpecifications(const GasParticle& particle_one,
                                      const ParticleSpecs& specification) const;
};

}  // namespace idealgas

#endif  // IDEAL_GAS_GAS_CONTAINER_H
