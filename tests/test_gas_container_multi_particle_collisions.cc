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

bool AreVectorsAccurate(const vec2& vector_one, const vec2& vector_two) {
  bool is_vector_one_accurate =
      all(lessThan(vector_one, kExpectedAccuracy));
  bool is_vector_two_accurate =
      all(lessThan(vector_two, kExpectedAccuracy));

  return is_vector_one_accurate && is_vector_two_accurate;
}

TEST_CASE("Testing Particle on Particle Collisions") {
  ParticleSpecs specs = {1, 255, 255, 255, "test"};

  SECTION("Particles colliding perfectly diagonal") {
    GasParticle particle_one = CreateParticle(20, 20, 0.1, 0, specs);
    GasParticle particle_two = CreateParticle(21.4, 21.4, -0.1, 0, specs);

    vector<GasParticle> particles = {particle_one, particle_two};
    GasContainer container = GasContainer(particles);

    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Particles colliding parallel to x-axis") {
    GasParticle particle_one = CreateParticle(200, 200, 1, 0, specs);
    GasParticle particle_two = CreateParticle(202, 200, -1, 0, specs);

    vector<GasParticle> particles = {particle_one, particle_two};
    GasContainer container = GasContainer(particles);

    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(-1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(1, 0));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Particles colliding parallel to y-axis") {
    GasParticle particle_one = CreateParticle(200, 200, 0, 1, specs);
    GasParticle particle_two = CreateParticle(200, 202, 0, -1, specs);

    vector<GasParticle> particles = {particle_one, particle_two};
    GasContainer container = GasContainer(particles);

    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 1));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Particles are not colliding") {
    GasParticle particle_one = CreateParticle(200, 200, 0, 1, specs);
    GasParticle particle_two = CreateParticle(200, 300, 0, -1, specs);

    vector<GasParticle> particles = {particle_one, particle_two};
    GasContainer container = GasContainer(particles);

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

    vector<GasParticle> particles = {particle_one, particle_two};
    GasContainer container = GasContainer(particles);

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

    vector<GasParticle> particles = {particle_one, particle_two};
    GasContainer container = GasContainer(particles);

    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(1, 0));

    REQUIRE(AreVectorsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }
}