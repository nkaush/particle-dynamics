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

TEST_CASE("Testing Particle on Wall Collisions Parallel to Axes") {
  SECTION("Particle collides parallel to y-axis with upper wall") {
  }

  SECTION("Particle collides parallel to y-axis with lower wall") {
  }

  SECTION("Particle collides parallel to x-axis with left wall") {
  }

  SECTION("Particle collides parallel to x-axis with right wall") {
  }
}

TEST_CASE("Testing Particle on Wall Collisions Diagonally Bounce Off Walls") {
  SECTION("Particle diagonally collides with upper wall") {
  }

  SECTION("Particle diagonally collides with lower wall") {
  }

  SECTION("Particle diagonally collides with left wall") {
  }

  SECTION("Particle diagonally collides with right wall") {
  }
}

TEST_CASE("Testing Particles Overlapping Walls on Collision") {
  SECTION("Particle overlaps left wall") {

  }

  SECTION("Particle overlaps right wall") {

  }

  SECTION("Particle overlaps upper wall") {

  }

  SECTION("Particle overlaps lower wall") {

  }

  SECTION("Particle overlaps corner of lower and left walls") {

  }

  SECTION("Particle overlaps corner of lower and right walls") {

  }

  SECTION("Particle overlaps corner of upper and left walls") {

  }

  SECTION("Particle overlaps corner of upper and right walls") {

  }
}

TEST_CASE("Testing Particle on Wall Collisions in the Corners of Container") {
  SECTION("Particle collides at corner upper and left walls") {

  }

  SECTION("Particle collides at corner lower and left walls") {

  }

  SECTION("Particle collides at corner upper and right walls") {

  }

  SECTION("Particle collides at corner lower and right walls") {

  }
}