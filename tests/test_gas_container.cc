//
// Created by Neil Kaushikkar on 3/12/21.
//

#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasParticle;
using idealgas::GasContainer;

using glm::vec2;
using glm::equal;
using glm::all;

TEST_CASE("Testing Wall Collisions") {
  SECTION("Increment position with positive x-axis velocity") {

//    FindWallsCollidingWithParticle
//    vec2 position = vec2(GasContainer::kContainerRightWall.wall_coordinate - 10,
//                           GasContainer::kContainerUpperWall.wall_coordinate + 100);
//    GasParticle particle = GasParticle(position, vec2(0.1, 0), 10);
//
//    GasContainer container = GasContainer();
//    REQUIRE(container.CalculateParticleVelocityAfterWallCollision(particle) == 1);
  }

// TODO: Add tests for diagonal movements
}

TEST_CASE("Testing Particle Collisions") {
  SECTION("Colliding parallel to x-axis") {
    GasParticle particle_one = GasParticle(vec2(20, 20), vec2(0.1, 0), 2);
    GasParticle particle_two = GasParticle(vec2(21.4, 21.4), vec2(-0.1, 0), 2);

    vec2 velocity_one =
        GasContainer::CalculateParticleVelocityAfterCollision(particle_one,
                                                              particle_two);

    vec2 velocity_two =
        GasContainer::CalculateParticleVelocityAfterCollision(particle_two,
                                                              particle_one);
    bool is_velocity_one_accurate = all(equal(velocity_one, vec2(0, -0.1)));
    bool is_velocity_two_accurate = all(equal(velocity_two, vec2(0, 0.1)));
    bool does_expected_equal_actual = is_velocity_one_accurate
                                      && is_velocity_two_accurate;
    REQUIRE(does_expected_equal_actual);
  }
}