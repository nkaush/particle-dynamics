//
// Created by Neil Kaushikkar on 3/12/21.
//

#include <catch2/catch.hpp>
#include "test_helper.h"

using idealgas::GasParticle;
using idealgas::GasContainer;
using idealgas::ParticleSpecs;

using idealgas_test::CreateParticle;
using idealgas_test::IsVelocityAccurate;

using glm::vec2;
using std::vector;

TEST_CASE("Testing Particles With Different Mass Colliding Parallel To Axes") {
  ParticleSpecs specs_one = {1, 1, 255, 255, 255, "test"};
  ParticleSpecs specs_two = {1, 2, 255, 255, 255, "test"};

  SECTION("Touching particles colliding perfectly diagonal") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 0.1, 0, specs_one));
    particles.push_back(CreateParticle(351.4, 351.4, -0.1, 0, specs_two));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_velocities_accurate =
        IsVelocityAccurate(particles.at(0), vec2(0, -0.13333));
    are_velocities_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(0, 0.06666));

    REQUIRE(are_velocities_accurate);
  }

  SECTION("Touching particles head-on colliding parallel to x-axis") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 1, 0, specs_one));
    particles.push_back(CreateParticle(352, 350, -1, 0, specs_two));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_velocities_accurate =
        IsVelocityAccurate(particles.at(0), vec2(-1.3333, 0));
    are_velocities_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(0.66666, 0));

    REQUIRE(are_velocities_accurate);
  }

  SECTION("Touching particles head-on colliding parallel to y-axis") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 0, 1, specs_one));
    particles.push_back(CreateParticle(350, 352, 0, -1, specs_two));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_velocities_accurate =
        IsVelocityAccurate(particles.at(0), vec2(0, -1.3333));
    are_velocities_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(0, 0.66666));

    REQUIRE(are_velocities_accurate);
  }
}

TEST_CASE("Testing Combinations of Particle Masses and Speeds") {
  ParticleSpecs specs_one = {1, 1, 255, 255, 255, "test"};
  ParticleSpecs specs_two = {1, 2, 255, 255, 255, "test"};

  SECTION("Touching particles collide in T-bone crash") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 1, 0, specs_one));
    particles.push_back(CreateParticle(350, 352, 0, -1, specs_two));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_velocities_accurate =
        IsVelocityAccurate(particles.at(0), vec2(1.3333, -1.3333));
    are_velocities_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(0, 0));

    REQUIRE(are_velocities_accurate);
  }

  SECTION("Faster lighter particle hits slower heavier particle from behind") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 3, 0, specs_one));
    particles.push_back(CreateParticle(352, 350, 1, 0, specs_two));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_velocities_accurate =
        IsVelocityAccurate(particles.at(0), vec2(0.33333, 0));
    are_velocities_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(2.33333, 0));

    REQUIRE(are_velocities_accurate);
  }

  SECTION("Faster heavier particle hits slower lighter particle from behind") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 3, 0, specs_two));
    particles.push_back(CreateParticle(351, 350, 1, 0, specs_one));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_velocities_accurate =
        IsVelocityAccurate(particles.at(0), vec2(1.66666, 0));
    are_velocities_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(3.66666, 0));

    REQUIRE(are_velocities_accurate);
  }

  SECTION("Particles moving on perpendicular diagonals collide") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 1, -1, specs_one));
    particles.push_back(CreateParticle(352, 350, -1, -1, specs_two));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_velocities_accurate =
        IsVelocityAccurate(particles.at(0), vec2(-1.66666, -1));
    are_velocities_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(0.33333, -1));

    REQUIRE(are_velocities_accurate);
  }
}

TEST_CASE("Testing Multi-Particles With Different Mass Collisions") {
  ParticleSpecs specs_one = {1, 1, 255, 255, 255, "test"};
  ParticleSpecs specs_two = {1, 2, 255, 255, 255, "test"};
  ParticleSpecs specs_three = {1, 3, 255, 255, 255, "test"};

  SECTION("Heavy particles collide with third light stationary particle") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 0, 0, specs_one));
    particles.push_back(CreateParticle(348, 350, 1, 0, specs_two));
    particles.push_back(CreateParticle(350, 352, 0, -1, specs_three));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_results_accurate =
        IsVelocityAccurate(particles.at(0), vec2(1.33333, -1.5));
    are_results_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(0.33333, 0));
    are_results_accurate &= IsVelocityAccurate(particles.at(2), vec2(0, -0.5));

    REQUIRE(are_results_accurate);
  }

  SECTION("Light particles collide with third heavy stationary particle") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 0, 0, specs_three));
    particles.push_back(CreateParticle(348, 350, 1, 0, specs_one));
    particles.push_back(CreateParticle(350, 352, 0, -1, specs_one));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_results_accurate =
        IsVelocityAccurate(particles.at(0), vec2(0.5, -0.5));
    are_results_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(-0.5, 0));
    are_results_accurate &= IsVelocityAccurate(particles.at(2), vec2(0, 0.5));

    REQUIRE(are_results_accurate);
  }

  SECTION("Particles of varying mass all converge at common point") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, -1, -1, specs_one));
    particles.push_back(CreateParticle(348, 350, 1, -1, specs_two));
    particles.push_back(CreateParticle(349, 348.6, 0, 1.4, specs_three));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();
    particles = container.GetAllParticles();

    bool are_results_accurate =
        IsVelocityAccurate(particles.at(0), vec2(2.524772, 0.201343));
    are_results_accurate &=
        IsVelocityAccurate(particles.at(1), vec2(-1.449041, 0.561984));
    are_results_accurate &=
        IsVelocityAccurate(particles.at(2), vec2(0.4577698, -0.04177));

    REQUIRE(are_results_accurate);
  }

  SECTION("Particle collision chain with lighter particles in edges") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 1, 0, specs_one));
    particles.push_back(CreateParticle(352, 350, 0, 0, specs_three));
    particles.push_back(CreateParticle(354, 350, 0, 0, specs_three));
    particles.push_back(CreateParticle(356, 350, 0, 0, specs_three));
    particles.push_back(CreateParticle(358, 350, 0, 0, specs_one));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();

    bool are_results_accurate = true;
    particles = container.GetAllParticles();
    vector<vec2> expected = {vec2(-0.5, 0), vec2(0, 0), vec2(0, 0),
                             vec2(0.25, 0), vec2(0.75, 0)};

    for (size_t i = 0; i < particles.size(); i++) {
      are_results_accurate &=
          IsVelocityAccurate(particles.at(i), expected.at(i));
    }

    REQUIRE(are_results_accurate);
  }

  SECTION("Particle collision chain with heavier particles in edges") {
    vector<GasParticle> particles;
    particles.push_back(CreateParticle(350, 350, 1, 0, specs_three));
    particles.push_back(CreateParticle(352, 350, 0, 0, specs_one));
    particles.push_back(CreateParticle(354, 350, 0, 0, specs_one));
    particles.push_back(CreateParticle(356, 350, 0, 0, specs_one));
    particles.push_back(CreateParticle(358, 350, 0, 0, specs_three));

    GasContainer container = GasContainer(particles);
    container.AdvanceOneFrame();

    bool are_results_accurate = true;
    particles = container.GetAllParticles();
    vector<vec2> expected = {vec2(0.5, 0), vec2(0, 0), vec2(0, 0),
                             vec2(-0.75, 0), vec2(0.75, 0)};

    for (size_t i = 0; i < particles.size(); i++) {
      are_results_accurate &=
          IsVelocityAccurate(particles.at(i), expected.at(i));
    }

    REQUIRE(are_results_accurate);
  }
}