//
// Created by Neil Kaushikkar on 3/12/21.
//

#include <catch2/catch.hpp>

#include <gas_particle.h>

using idealgas::GasParticle;
using glm::vec2;

TEST_CASE("Testing Particle Position Updating") {
  SECTION("Increment position with positive x-axis velocity") {
    GasParticle particle = GasParticle(vec2(19.9, 20), vec2(0.1, 0), 1);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position with negative x-axis velocity") {
    GasParticle particle = GasParticle(vec2(20.1, 20), vec2(-0.1, 0), 1);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position with positive y-axis velocity") {
    GasParticle particle = GasParticle(vec2(20, 19.9), vec2(0, 0.1), 1);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position with negative y-axis velocity") {
    GasParticle particle = GasParticle(vec2(20, 20.1), vec2(0, -0.1), 1);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  // TODO: Add tests for diagonal movements
}