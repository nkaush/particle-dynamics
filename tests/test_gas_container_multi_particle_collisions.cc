//
// Created by Neil Kaushikkar on 3/12/21.
//

#include <catch2/catch.hpp>
#include <gas_container.h>

using idealgas::GasParticle;
using idealgas::GasContainer;
using idealgas::ParticleSpecs;

using std::vector;
using glm::vec2;
using glm::equal;
using glm::all;
using glm::abs;
using glm::lessThan;

static constexpr float kFloatEqualityThreshold = 0.00001f;
static const vec2 kExpectedAccuracy = vec2(kFloatEqualityThreshold,
                                           kFloatEqualityThreshold);

GasParticle CreateParticle(float x_pos, float y_pos, float x_velo,
                           float y_velo, const ParticleSpecs& specs) {
  return GasParticle(vec2(x_pos, y_pos), vec2(x_velo, y_velo), specs);
}

bool IsVelocityAsExpected(const GasParticle& particle, const vec2& expected) {
  vec2 accuracy = abs(particle.GetVelocity() - expected);
  return all(lessThan(accuracy, kExpectedAccuracy));
}

bool AreVectorsAccurate(const vec2& vector_one, const vec2& vector_two) {
  bool is_vector_one_accurate =
      all(lessThan(vector_one, kExpectedAccuracy));
  bool is_vector_two_accurate =
      all(lessThan(vector_two, kExpectedAccuracy));

  return is_vector_one_accurate && is_vector_two_accurate;
}

TEST_CASE("Testing Touching Particle on Particle Collisions") {
  ParticleSpecs specs = {1, 255, 255, 255, "test"};

  SECTION("Touching particles colliding perfectly diagonal") {
    GasParticle particle_one = CreateParticle(200, 200, 0.1, 0, specs);
    GasParticle particle_two = CreateParticle(201.4, 201.4, -0.1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Touching particles head-on colliding parallel to x-axis") {
    GasParticle particle_one = CreateParticle(200, 200, 1, 0, specs);
    GasParticle particle_two = CreateParticle(202, 200, -1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(-1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(1, 0));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Touching particles head-on colliding parallel to y-axis") {
    GasParticle particle_one = CreateParticle(200, 200, 0, 1, specs);
    GasParticle particle_two = CreateParticle(200, 202, 0, -1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 1));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Touching particles collide in T-bone crash") {
    GasParticle particle_one = CreateParticle(200, 200, 1, 0, specs);
    GasParticle particle_two = CreateParticle(200, 202, 0, -1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(1, -1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Faster particle hits slower particle moving in same direction") {
    GasParticle particle_one = CreateParticle(200, 200, 3, 0, specs);
    GasParticle particle_two = CreateParticle(202, 200, 1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(3, 0));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Particles moving on perpendicular diagonals collide") {
    GasParticle particle_one = CreateParticle(200, 200, 1, -1, specs);
    GasParticle particle_two = CreateParticle(202, 200, -1, -1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(-1, -1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(1, -1));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }
}

TEST_CASE("Testing Overlapping Particle on Particle Collisions") {
  ParticleSpecs specs = {1, 255, 255, 255, "test"};

  SECTION("Overlapping particles head-on colliding parallel to y-axis") {
    GasParticle particle_one = CreateParticle(200, 200, 0, 0.1, specs);
    GasParticle particle_two = CreateParticle(200, 201, 0, -0.1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    bool did_begin_moving_away =
        AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    container.AdvanceOneFrame();

    velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    bool did_continue_moving_away =
        AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    bool is_behavior_expected =
        did_begin_moving_away && did_continue_moving_away;
    REQUIRE(is_behavior_expected);
  }

  SECTION("Overlapping particles head-on colliding parallel to x-axis") {
    GasParticle particle_one = CreateParticle(200, 200, 0.1, 0, specs);
    GasParticle particle_two = CreateParticle(201, 200, -0.1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(-0.1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0.1, 0));

    bool did_begin_moving_away =
        AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    container.AdvanceOneFrame();

    velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(-0.1, 0));
    velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0.1, 0));

    bool did_continue_moving_away =
        AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    bool is_behavior_expected =
        did_begin_moving_away && did_continue_moving_away;
    REQUIRE(is_behavior_expected);
  }

  SECTION("Overlapping particles colliding perfectly diagonal") {
    GasParticle particle_one = CreateParticle(200, 200, 0.1, 0, specs);
    GasParticle particle_two = CreateParticle(200.7, 200.7, -0.1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    bool did_begin_moving_away =
        AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    container.AdvanceOneFrame();

    velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    bool did_continue_moving_away =
        AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    bool is_behavior_expected =
        did_begin_moving_away && did_continue_moving_away;

    REQUIRE(is_behavior_expected);
  }
}

TEST_CASE("Particles Are Not Colliding") {
  ParticleSpecs specs = {1, 255, 255, 255, "test"};
  SECTION("Particles are not colliding") {
    GasParticle particle_one = CreateParticle(200, 200, 0, 1, specs);
    GasParticle particle_two = CreateParticle(200, 300, 0, -1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, 1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, -1));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Particles are overlapping but moving away on different axis") {
    GasParticle particle_one = CreateParticle(200, 200, 1, 0, specs);
    GasParticle particle_two = CreateParticle(200, 201, -1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(-1, 0));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Particles are overlapping but moving same direction") {
    GasParticle particle_one = CreateParticle(200, 200, 1, 0, specs);
    GasParticle particle_two = CreateParticle(200, 201, 1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(1, 0));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }
}