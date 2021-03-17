//
// Created by Neil Kaushikkar on 3/16/21.
//

#include "test_helper.h"

namespace idealgas_test {

using idealgas::GasParticle;
using idealgas::ParticleSpecs;
using glm::vec2;

GasParticle CreateParticle(float x_pos, float y_pos, float x_velo,
                           float y_velo, const ParticleSpecs& specs) {
  return GasParticle(vec2(x_pos, y_pos), vec2(x_velo, y_velo), specs);
}

bool AreResultsAccurate(const vec2& vector_one, const vec2& vector_two) {
  bool is_vector_one_accurate =
      all(lessThan(vector_one, kExpectedAccuracy));
  bool is_vector_two_accurate =
      all(lessThan(vector_two, kExpectedAccuracy));

  return is_vector_one_accurate && is_vector_two_accurate;
}
bool IsVelocityAccurate(const GasParticle& particle, const vec2& velocity) {
  vec2 difference = particle.GetVelocity() - velocity;
  return all(lessThan(difference, kExpectedAccuracy));
}

} // namespace idealgas_test
