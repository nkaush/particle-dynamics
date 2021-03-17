#include <catch2/catch.hpp>
#include <simulation_engine.h>

using idealgas::SimulationEngine;
using nlohmann::json;

TEST_CASE("Test Missing File") {
  SECTION("Test Invalid File Path") {
    REQUIRE_THROWS_AS(SimulationEngine::ValidateFilePath("does/not/exist.json"),
                      std::invalid_argument);
  }
}

TEST_CASE("Testing JSON Deserialization With Invalid Count Specifications") {
  SECTION("Missing particle counts key") {
    json to_check = {{"particle_counts", {}}};
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }

  SECTION("Missing particle counts") {
    json to_check =
        "{\"particle_types\": {\"oxygen\": {\"red\": 0.2, "
        "\"green\": 0.79, \"blue\": 0.5, \"radius\": 7}},"
        "\"particle_counts\": [{\"type\": \"oxygen\", "
        "\"max_velocity\": 2}]}"_json;
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }

  SECTION("Missing particle type") {
    json to_check =
        "{\"particle_types\": {\"oxygen\": {\"red\": 0.2, "
        "\"green\": 0.79, \"blue\": 0.5, \"radius\": 7}},"
        "\"particle_counts\": [{\"count\": 20, "
        "\"max_velocity\": 2}]}"_json;
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }

  SECTION("Missing particle max velocity") {
    json to_check =
        "{\"particle_types\": {\"carbon\": {\"red\": 0.2, "
        "\"green\": 0.79, \"blue\": 0.5, \"radius\": 7}},"
        "\"particle_counts\": [{\"count\": 20, "
        "\"type\": \"carbon\"}]}"_json;
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }

  SECTION("Specified particle type not in defined particle types") {
    json to_check =
        "{\"particle_types\": {\"carbon\": {\"red\": 0.2, "
        "\"green\": 0.79, \"blue\": 0.5, \"radius\": 7}},"
        "\"particle_counts\": [{\"count\": 20, "
        "\"type\": \"oxygen\", \"max_velocity\": 2}]}"_json;
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }
}

TEST_CASE("Testing JSON Deserialization With Invalid Type Specifications") {
  SECTION("Missing particle types key") {
    json to_check = {{"particle_types", {}}};
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }

  SECTION("Missing particle type red color") {
    json to_check =
        "{\"particle_types\": {\"oxygen\": {"
        "\"green\": 0.79, \"blue\": 0.5, \"radius\": 7}},"
        "\"particle_counts\": [{\"count\": 20, "
        "\"type\": \"oxygen\", \"max_velocity\": 2}]}"_json;
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }

  SECTION("Missing particle type green color") {
    json to_check =
        "{\"particle_types\": {\"oxygen\": {\"red\": 0.2, "
        "\"blue\": 0.5, \"radius\": 7}},\"particle_counts\": [{\"count\": 20, "
        "\"type\": \"oxygen\", \"max_velocity\": 2}]}"_json;
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }

  SECTION("Missing particle type blue color") {
    json to_check =
        "{\"particle_types\": {\"oxygen\": {\"red\": 0.2, "
        "\"green\": 0.79, \"radius\": 7}},"
        "\"particle_counts\": [{\"count\": 20, "
        "\"type\": \"oxygen\", \"max_velocity\": 2}]}"_json;
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }

  SECTION("Missing particle type radius") {
    json to_check =
        "{\"particle_types\": {\"carbon\": {\"red\": 0.2, "
        "\"green\": 0.79, \"blue\": 0.5}},"
        "\"particle_counts\": [{\"count\": 20, "
        "\"type\": \"oxygen\", \"max_velocity\": 2}]}"_json;
    REQUIRE_THROWS_AS(SimulationEngine::ValidateRandomGenerationJson(to_check),
                      std::invalid_argument);
  }
}