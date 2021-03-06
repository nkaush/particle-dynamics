//
// Created by Neil Kaushikkar on 3/12/21.
//

#include <catch2/catch.hpp>
#include "test_helper.h"

using idealgas::GasParticle;
using idealgas::GasContainer;
using idealgas::ParticleSpecs;

using idealgas_test::CreateParticle;
using idealgas_test::AreResultsAccurate;
using idealgas_test::IsVelocityAccurate;

using glm::vec2;
using std::vector;

TEST_CASE("Testing Touching Particle on Particle Collisions With Same Mass") {
  ParticleSpecs specs = {1, 1, 255, 255, 255, "test"};

  SECTION("Touching particles colliding perfectly diagonal") {
    GasParticle particle_one = CreateParticle(350, 350, 0.1, 0, specs);
    GasParticle particle_two = CreateParticle(351.4, 351.4, -0.1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    REQUIRE(AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Touching particles head-on colliding parallel to x-axis") {
    GasParticle particle_one = CreateParticle(350, 350, 1, 0, specs);
    GasParticle particle_two = CreateParticle(352, 350, -1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(-1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(1, 0));

    REQUIRE(AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Touching particles head-on colliding parallel to y-axis") {
    GasParticle particle_one = CreateParticle(350, 350, 0, 1, specs);
    GasParticle particle_two = CreateParticle(350, 352, 0, -1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 1));

    REQUIRE(AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Touching particles collide in T-bone crash") {
    GasParticle particle_one = CreateParticle(350, 350, 1, 0, specs);
    GasParticle particle_two = CreateParticle(350, 352, 0, -1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(1, -1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0));

    REQUIRE(AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Faster particle hits slower particle moving in same direction") {
    GasParticle particle_one = CreateParticle(350, 350, 3, 0, specs);
    GasParticle particle_two = CreateParticle(352, 350, 1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(3, 0));

    REQUIRE(AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Particles moving on perpendicular diagonals collide") {
    GasParticle particle_one = CreateParticle(350, 350, 1, -1, specs);
    GasParticle particle_two = CreateParticle(352, 350, -1, -1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(-1, -1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(1, -1));

    REQUIRE(AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }
}

TEST_CASE("Testing Overlapping Particle on Particle Collisions With Same Mass") {
  ParticleSpecs specs = {1, 1, 255, 255, 255, "test"};

  SECTION("Overlapping particles head-on colliding parallel to y-axis") {
    GasParticle particle_one = CreateParticle(350, 350, 0, 0.1, specs);
    GasParticle particle_two = CreateParticle(350, 351, 0, -0.1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    bool did_begin_moving_away =
        AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    container.AdvanceOneFrame();

    velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    bool did_continue_moving_away =
        AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    bool is_behavior_expected =
        did_begin_moving_away && did_continue_moving_away;
    REQUIRE(is_behavior_expected);
  }

  SECTION("Overlapping particles head-on colliding parallel to x-axis") {
    GasParticle particle_one = CreateParticle(350, 350, 0.1, 0, specs);
    GasParticle particle_two = CreateParticle(351, 350, -0.1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(-0.1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0.1, 0));

    bool did_begin_moving_away =
        AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    container.AdvanceOneFrame();

    velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(-0.1, 0));
    velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0.1, 0));

    bool did_continue_moving_away =
        AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    bool is_behavior_expected =
        did_begin_moving_away && did_continue_moving_away;
    REQUIRE(is_behavior_expected);
  }

  SECTION("Overlapping particles colliding perfectly diagonal") {
    GasParticle particle_one = CreateParticle(350, 350, 0.1, 0, specs);
    GasParticle particle_two = CreateParticle(350.7, 350.7, -0.1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    bool did_begin_moving_away =
        AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    container.AdvanceOneFrame();

    velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, -0.1));
    velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, 0.1));

    bool did_continue_moving_away =
        AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy);

    bool is_behavior_expected =
        did_begin_moving_away && did_continue_moving_away;

    REQUIRE(is_behavior_expected);
  }
}

TEST_CASE("Particles With Same Mass Are Not Colliding") {
  ParticleSpecs specs = {1, 1, 255, 255, 255, "test"};
  SECTION("Particles are not colliding") {
    GasParticle particle_one = CreateParticle(350, 350, 0, 1, specs);
    GasParticle particle_two = CreateParticle(350, 300, 0, -1, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(0, 1));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(0, -1));

    REQUIRE(AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Particles are overlapping but moving away on different axis") {
    GasParticle particle_one = CreateParticle(350, 350, 1, 0, specs);
    GasParticle particle_two = CreateParticle(350, 351, -1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(-1, 0));

    REQUIRE(AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }

  SECTION("Particles are overlapping but moving same direction") {
    GasParticle particle_one = CreateParticle(350, 350, 1, 0, specs);
    GasParticle particle_two = CreateParticle(350, 351, 1, 0, specs);

    GasContainer container = GasContainer({particle_one, particle_two});
    container.AdvanceOneFrame();

    vec2 velocity_one_accuracy =
        abs(container.GetAllParticles()[0].GetVelocity() - vec2(1, 0));
    vec2 velocity_two_accuracy =
        abs(container.GetAllParticles()[1].GetVelocity() - vec2(1, 0));

    REQUIRE(AreResultsAccurate(velocity_one_accuracy, velocity_two_accuracy));
  }
}

TEST_CASE("Testing Multi-Particles With Same Mass Collisions") {
  ParticleSpecs specs = {1, 1, 255, 255, 255, "test"};

  SECTION("Particles collide with third stationary particle on two axes") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 0, 0, specs));
    particles.push_back(CreateParticle(348, 350, 1, 0, specs));
    particles.push_back(CreateParticle(350, 352, 0, -1, specs));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_results_accurate = IsVelocityAccurate(particles.at(0), vec2(1, -1));
    are_results_accurate &= IsVelocityAccurate(particles.at(1), vec2(0, 0));
    are_results_accurate &= IsVelocityAccurate(particles.at(2), vec2(0, 0));

    REQUIRE(are_results_accurate);
  }

  SECTION("Particles of same mass all converge at common point") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, -1, -1, specs));
    particles.push_back(CreateParticle(348, 350, 1, -1, specs));
    particles.push_back(CreateParticle(349, 348.6, 0, 1.4, specs));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_results_accurate =
        IsVelocityAccurate(particles.at(0), vec2(1.797298, 0.11621));
    are_results_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(-1.538717, -0.245799));
    are_results_accurate &=
        IsVelocityAccurate(particles.at(2), vec2(-0.25858, -0.4704119));

    REQUIRE(are_results_accurate);
  }

  SECTION("Particles collide like in Newton's cradle") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 1, 0, specs));
    particles.push_back(CreateParticle(352, 350, 0, 0, specs));
    particles.push_back(CreateParticle(354, 350, 0, 0, specs));
    particles.push_back(CreateParticle(356, 350, 0, 0, specs));
    particles.push_back(CreateParticle(358, 350, 0, 0, specs));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();

    bool are_results_accurate = true;
    particles = container.GetAllParticles();

    for (size_t i = 0; i < particles.size() - 1; i++) {
      are_results_accurate &= IsVelocityAccurate(particles.at(i), vec2(0, 0));
    }

    are_results_accurate &= IsVelocityAccurate(particles.at(4), vec2(1, 0));

    REQUIRE(are_results_accurate);
  }
}