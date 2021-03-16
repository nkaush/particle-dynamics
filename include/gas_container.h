#pragma once

#include "cinder/gl/gl.h"
#include "gas_particle.h"

namespace idealgas {

/**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
class GasContainer {
 public:
  static constexpr float kContainerUpperBound = 100;
  static constexpr float kContainerLowerBound = 400;
  static constexpr float kContainerLeftBound = 100;
  static constexpr float kContainerRightBound = 600;

  /**
   * TODO: Add more parameters to this constructor, and add documentation.
   */

  GasContainer(const std::vector<GasParticle>& particles);

  /**
   * Displays the container walls and the current positions of the particles.
   */
  void Display() const;

  void AdvanceOneFrame();

  std::vector<GasParticle> GetAllParticles() const;

  static glm::vec2 CalculateParticleVelocityAfterCollision(
      const GasParticle& particle_one, const GasParticle& particle_two);

  static bool AreParticlesColliding(const GasParticle& particle_one,
                                    const GasParticle& particle_two);

 private:
  std::vector<GasParticle> all_particles_;

  void HandleParticleInteractions();

  static glm::vec2 CalculateParticleVelocityAfterWallCollision(
      const GasParticle& particle);

  static bool IsParticleCollidingWithWalls(const GasParticle& particle,
                                           size_t axis_idx,
                                           float min_wall_bound,
                                           float max_wall_bound);
};

}  // namespace idealgas
