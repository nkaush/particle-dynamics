//
// Created by Neil Kaushikkar on 3/16/21.
//

#include <gas_container.h>

#ifndef TEST_HELPER_CC
#define TEST_HELPER_CC

namespace idealgas_test {

// This defines the allowable error in floating point math
static constexpr float kFloatEqualityThreshold = 0.00001f;
static const glm::vec2 kExpectedAccuracy = glm::vec2(kFloatEqualityThreshold,
                                                     kFloatEqualityThreshold);

/**
 * Allows for creation of a particle with only literals.
 * @param x_pos - a float indicating the starting x position
 * @param y_pos - a float indicating the starting y position
 * @param x_velo - a float indicating the starting x velocity
 * @param y_velo - a float indicating the starting y velocity
 * @param specs - a ParticleSpecs struct storing a particle's color and radius
 * @return a GasParticle created as specified
 */
idealgas::GasParticle CreateParticle(float x_pos, float y_pos,
                                     float x_velo, float y_velo,
                                     const idealgas::ParticleSpecs& specs);

/**
 * Determines whether 2 vectors are within the acceptable error
 * margin for floating point math as defined above.
 * @param vector_one - a vec2 of difference between actual and expected value
 * @param vector_two - a vec2 of difference between actual and expected value
 * @return a bool indicating whether the vec2 are within the margin of error
 */
bool AreResultsAccurate(const glm::vec2& vector_one,
                        const glm::vec2& vector_two);

/**
 * Determines whether the velocity of a particle is within acceptible margins
 * @param particle - the particle whose velocity we need to check
 * @param velocity - the expected velocity of the particle
 * @return a bool indicating whether the velocity is within the margin of error
 */
bool IsVelocityAccurate(const idealgas::GasParticle& particle,
                        const glm::vec2& expected_elocity);
} // namespace idealgas_test

#endif  // TEST_HELPER_CC
