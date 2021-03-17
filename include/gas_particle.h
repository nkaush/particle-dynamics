#pragma once

#include "cinder/gl/gl.h"
#include <vector>

namespace idealgas {

/**
 * This struct is used as a way to quickly initialize multiple
 * particles with the same properties when testing to define the same
 * particle specifications.
 */
struct ParticleSpecs {
  float radius;
  float red;
  float green;
  float blue;
  std::string name;
};

/**
 * This class is used as an abstraction to represent a single gas particle.
 */
class GasParticle {
 public:
  /**
   * Creates a GasParticle. This constructor takes in the ParticleSpecs struct
   * to define basic particle details.
   * @param initial_pos - a vec2 containing the particle's initial position
   * @param initial_velo - a vec2 containing the particle's initial velocity
   * @param specs - a ParticleSpecs struct that hold info about this particle
   */
  GasParticle(const glm::vec2& initial_pos, const glm::vec2& initial_velo,
              const ParticleSpecs& specs);

  /**
   * Creates a GasParticle using the initial state and appearance info provided.
   * @param initial_pos - a vec2 containing the particle's initial position
   * @param initial_velo - a vec2 containing the particle's initial velocity
   * @param radius - a float indicating the radius of the particle
   * @param red - a float from 0-1 indicating amount of red to color particle
   * @param green - a float from 0-1 indicating amount of green to color particle
   * @param blue - a float from 0-1 indicating amount of blue to color particle
   * @param name - the name assigned to this particle when deserializing json
   */
  GasParticle(const glm::vec2& initial_pos, const glm::vec2& initial_velo,
              float radius, float mass, float red, float green, float blue,
              const std::string& name);

  /**
   * Increment's this particle's position after 1 unit of time using the
   * particle's current velocity vec2.
   */
  void UpdatePosition();

  /**
   * Draws this particle based on the particle's current position in the color
   * that is defined by the floats defining red, green, and blue intensity.
   */
  void DrawParticle() const;

  glm::vec2 GetVelocity() const;

  glm::vec2 GetPosition() const;

  float GetRadius() const;

  float GetMass() const;

  float GetRedIntensity() const;

  float GetGreenIntensity() const;

  float GetBlueIntensity() const;

  std::string GetTypeName() const;

  void SetVelocity(const glm::vec2& new_velocity);

 private:
  // The particle's current location
  glm::vec2 position_;
  // The particle's current velocity
  glm::vec2 velocity_;

  float radius_;
  float mass_;
  // Colors to display between 0 and 1 (1 translates to 255 in decimal RGB)
  float red_color_intensity_;
  float green_color_intensity_;
  float blue_color_intensity_;

  // The name assigned to this particle when deserializing it from json
  std::string particle_type_name_;
};

}  // namespace idealgas