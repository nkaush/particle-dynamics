//
// Created by Neil Kaushikkar on 3/16/21.
//

#include <gas_container.h>

#ifndef TEST_HELPER_CC
#define TEST_HELPER_CC

namespace idealgas_test {

static constexpr float kFloatEqualityThreshold = 0.00001f;
static const glm::vec2 kExpectedAccuracy = glm::vec2(kFloatEqualityThreshold,
                                                     kFloatEqualityThreshold);

idealgas::GasParticle CreateParticle(float x_pos, float y_pos,
                                     float x_velo, float y_velo,
                                     const idealgas::ParticleSpecs& specs);

bool AreResultsAccurate(const glm::vec2& vector_one,
                        const glm::vec2& vector_two);

bool IsVelocityAccurate(const idealgas::GasParticle& particle,
                        const glm::vec2& velocity);

} // namespace idealgas_test

#endif  // TEST_HELPER_CC
