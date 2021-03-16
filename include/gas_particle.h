#pragma once

#include "cinder/gl/gl.h"
#include <vector>

namespace idealgas {

struct ParticleSpecs {
  float radius;
  int red;
  int green;
  int blue;
  std::string name;
};

class GasParticle {
 public:
  GasParticle(const glm::vec2& initial_pos, const glm::vec2& initial_velo,
              const ParticleSpecs& specs);

  GasParticle(const glm::vec2& initial_pos, const glm::vec2& initial_velo,
              float radius, int red, int green, int blue,
              const std::string& name);

  void UpdatePosition();

  glm::vec2 GetVelocity() const;

  glm::vec2 GetPosition() const;

  float GetRadius() const;

  int GetRedIntensity() const;

  int GetGreenIntensity() const;

  int GetBlueIntensity() const;

  std::string GetTypeName() const;

  void DrawParticle() const;

  void SetVelocity(const glm::vec2& new_velocity);

 private:
  glm::vec2 position_;
  glm::vec2 velocity_;

  float radius_;
  int red_color_intensity_;
  int green_color_intensity_;
  int blue_color_intensity_;

  std::string particle_type_name_;
};

}  // namespace idealgas