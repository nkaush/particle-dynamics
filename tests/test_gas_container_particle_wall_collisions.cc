#include <catch2/catch.hpp>
#include "test_helper.h"

using idealgas::GasParticle;
using idealgas::GasContainer;
using idealgas::ParticleSpecs;

using idealgas_test::CreateParticle;
using idealgas_test::IsVelocityAccurate;

using glm::vec2;

TEST_CASE("Testing Particle on Wall Collisions Parallel to Axes") {
  float radius = 1;
  ParticleSpecs specs = {radius, 1, 255, 255, 255, "test"};

  SECTION("Particle collides parallel to y-axis with upper wall") {
    float wall_bound = GasContainer::kContainerUpperBound + radius;
    GasParticle particle_one = CreateParticle(400, wall_bound, 20, -0.1, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(20, 0.1)));
  }

  SECTION("Particle collides parallel to y-axis with lower wall") {
    float wall_bound = GasContainer::kContainerLowerBound - radius;
    GasParticle particle_one = CreateParticle(400, wall_bound, 8, 0.1, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(8, -0.1)));
  }

  SECTION("Particle collides parallel to x-axis with left wall") {
    float wall_bound = GasContainer::kContainerLeftBound + radius;
    GasParticle particle_one = CreateParticle(wall_bound, 400, -27, 0, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(27, 0)));
  }

  SECTION("Particle collides parallel to x-axis with right wall") {
    float wall_bound = GasContainer::kContainerRightBound - radius;
    GasParticle particle_one = CreateParticle(wall_bound, 400, 12, 0, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(-12, 0)));
  }
}

TEST_CASE("Testing Particle on Wall Collisions Diagonally Bounce Off Walls") {
  float radius = 1;
  ParticleSpecs specs = {1, 1, 255, 255, 255, "test"};

  SECTION("Particle diagonally collides with upper wall") {
    float wall_bound = GasContainer::kContainerUpperBound + radius;
    GasParticle particle_one = CreateParticle(400, wall_bound, -19, -8, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(-19, 8)));
  }

  SECTION("Particle diagonally collides with lower wall") {
    float wall_bound = GasContainer::kContainerLowerBound - radius;
    GasParticle particle_one = CreateParticle(400, wall_bound, 12, 27, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(12, -27)));
  }

  SECTION("Particle diagonally collides with left wall") {
    float wall_bound = GasContainer::kContainerLeftBound + radius;
    GasParticle particle_one = CreateParticle(wall_bound, 400, -2, 1, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(2, 1)));
  }

  SECTION("Particle diagonally collides with right wall") {
    float wall_bound = GasContainer::kContainerRightBound - radius;
    GasParticle particle_one = CreateParticle(wall_bound, 400, 12, 12, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(-12, 12)));
  }
}

TEST_CASE("Testing Particle on Wall Collisions in the Corners of Container") {
  float radius = 1;
  ParticleSpecs specs = {radius, 1, 255, 255, 255, "test"};

  SECTION("Particle collides at corner upper and left walls") {
    float x_axis_bound = GasContainer::kContainerLeftBound + radius;
    float y_axis_bound = GasContainer::kContainerUpperBound + radius;
    GasParticle particle_one =
        CreateParticle(x_axis_bound, y_axis_bound, -2, -4, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(2, 4)));
  }

  SECTION("Particle collides at corner lower and left walls") {
    float x_axis_bound = GasContainer::kContainerLeftBound + radius;
    float y_axis_bound = GasContainer::kContainerLowerBound - radius;
    GasParticle particle_one =
        CreateParticle(x_axis_bound, y_axis_bound, -8, 19, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(8, -19)));
  }

  SECTION("Particle collides at corner upper and right walls") {
    float x_axis_bound = GasContainer::kContainerRightBound - radius;
    float y_axis_bound = GasContainer::kContainerUpperBound + radius;
    GasParticle particle_one =
        CreateParticle(x_axis_bound, y_axis_bound, 19, -68, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(-19, 68)));
  }

  SECTION("Particle collides at corner lower and right walls") {
    float x_axis_bound = GasContainer::kContainerRightBound - radius;
    float y_axis_bound = GasContainer::kContainerLowerBound - radius;
    GasParticle particle_one =
        CreateParticle(x_axis_bound, y_axis_bound, 2, 8, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    REQUIRE(IsVelocityAccurate(container.GetAllParticles()[0], vec2(-2, -8)));
  }
}

TEST_CASE("Testing Particles Overlapping Walls on Collision") {
  ParticleSpecs specs = {1, 1, 255, 255, 255, "test"};

  SECTION("Particle overlaps left wall") {
    float wall_bound = GasContainer::kContainerLeftBound;
    GasParticle particle_one = CreateParticle(wall_bound, 20, -0.1, 2, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    bool did_change_velocity =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(0.1, 2));

    container.AdvanceOneFrame();

    bool did_continue_moving_away =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(0.1, 2));

    bool is_result_expected = did_change_velocity && did_continue_moving_away;
    REQUIRE(is_result_expected);
  }

  SECTION("Particle overlaps right wall") {
    float wall_bound = GasContainer::kContainerRightBound;
    GasParticle particle_one = CreateParticle(wall_bound, 20, 0.1, 2, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    bool did_change_velocity =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(-0.1, 2));

    container.AdvanceOneFrame();

    bool did_continue_moving_away =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(-0.1, 2));

    bool is_result_expected = did_change_velocity && did_continue_moving_away;
    REQUIRE(is_result_expected);
  }

  SECTION("Particle overlaps upper wall") {
    float wall_bound = GasContainer::kContainerUpperBound;
    GasParticle particle_one = CreateParticle(400, wall_bound, 10, -0.1, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    bool did_change_velocity =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(10, 0.1));

    container.AdvanceOneFrame();

    bool did_continue_moving_away =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(10, 0.1));

    bool is_result_expected = did_change_velocity && did_continue_moving_away;
    REQUIRE(is_result_expected);
  }

  SECTION("Particle overlaps lower wall") {
    float wall_bound = GasContainer::kContainerLowerBound;
    GasParticle particle_one = CreateParticle(400, wall_bound, 8, 0.1, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    bool did_change_velocity =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(8, -0.1));

    container.AdvanceOneFrame();

    bool did_continue_moving_away =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(8, -0.1));

    bool is_result_expected = did_change_velocity && did_continue_moving_away;
    REQUIRE(is_result_expected);
  }

  SECTION("Particle overlaps corner of lower and left walls") {
    float x_axis_bound = GasContainer::kContainerLeftBound;
    float y_axis_bound = GasContainer::kContainerLowerBound;
    GasParticle particle_one =
        CreateParticle(x_axis_bound, y_axis_bound, -0.19, 0.68, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    bool did_change_velocity =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(0.19, -0.68));

    container.AdvanceOneFrame();

    bool did_continue_moving_away =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(0.19, -0.68));

    bool is_result_expected = did_change_velocity && did_continue_moving_away;
    REQUIRE(is_result_expected);
  }

  SECTION("Particle overlaps corner of lower and right walls") {
    float x_axis_bound = GasContainer::kContainerRightBound;
    float y_axis_bound = GasContainer::kContainerLowerBound;
    GasParticle particle_one =
        CreateParticle(x_axis_bound, y_axis_bound, 0.8, 0.2, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    bool did_change_velocity =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(-0.8, -0.2));

    container.AdvanceOneFrame();

    bool did_continue_moving_away =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(-0.8, -0.2));

    bool is_result_expected = did_change_velocity && did_continue_moving_away;
    REQUIRE(is_result_expected);
  }

  SECTION("Particle overlaps corner of upper and left walls") {
    float x_axis_bound = GasContainer::kContainerLeftBound;
    float y_axis_bound = GasContainer::kContainerUpperBound;
    GasParticle particle_one =
        CreateParticle(x_axis_bound, y_axis_bound, -0.12, -0.12, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    bool did_change_velocity =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(0.12, 0.12));

    container.AdvanceOneFrame();

    bool did_continue_moving_away =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(0.12, 0.12));

    bool is_result_expected = did_change_velocity && did_continue_moving_away;
    REQUIRE(is_result_expected);
  }

  SECTION("Particle overlaps corner of upper and right walls") {
    float x_axis_bound = GasContainer::kContainerRightBound;
    float y_axis_bound = GasContainer::kContainerUpperBound;
    GasParticle particle_one =
        CreateParticle(x_axis_bound, y_axis_bound, 0.12, -0.27, specs);

    GasContainer container = GasContainer({particle_one}, 1);
    container.AdvanceOneFrame();

    bool did_change_velocity =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(-0.12, 0.27));

    container.AdvanceOneFrame();

    bool did_continue_moving_away =
        IsVelocityAccurate(container.GetAllParticles()[0], vec2(-0.12, 0.27));

    bool is_result_expected = did_change_velocity && did_continue_moving_away;
    REQUIRE(is_result_expected);
  }
}
