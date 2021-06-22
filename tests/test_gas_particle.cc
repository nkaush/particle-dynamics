//
// Created by Neil Kaushikkar on 3/12/21.
//

#include <catch2/catch.hpp>
#include <gas_particle.h>

using idealgas::GasParticle;
using idealgas::ParticleSpecs;
using glm::vec2;

TEST_CASE("Testing Particle Position Updating") {
  ParticleSpecs specs = {1, 1, ci::ColorA8u(255, 255, 255), "test"};
  SECTION("Increment position with positive x-axis velocity") {
    GasParticle particle = GasParticle(vec2(19.9, 20),
                                       vec2(0.1, 0), specs);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position with negative x-axis velocity") {
    GasParticle particle = GasParticle(vec2(20.1, 20),
                                       vec2(-0.1, 0), specs);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position with positive y-axis velocity") {
    GasParticle particle = GasParticle(vec2(20, 19.9),
                                       vec2(0, 0.1), specs);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position with negative y-axis velocity") {
    GasParticle particle = GasParticle(vec2(20, 20.1),
                                       vec2(0, -0.1), specs);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position diagonally up and right") {
    GasParticle particle = GasParticle(vec2(19.9, 20.1),
                                       vec2(0.1, -0.1), specs);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position diagonally up and left") {
    GasParticle particle = GasParticle(vec2(20.1, 20.1),
                                       vec2(-0.1, -0.1), specs);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position diagonally down and left") {
    GasParticle particle = GasParticle(vec2(20.1, 19.9),
                                       vec2(-0.1, 0.1), specs);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }

  SECTION("Increment position diagonally down and right") {
    GasParticle particle = GasParticle(vec2(19.9, 19.9),
                                       vec2(0.1, 0.1), specs);
    particle.UpdatePosition();
    REQUIRE(particle.GetPosition() == vec2(20, 20));
  }
}
