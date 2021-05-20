//
// Created by Neil Kaushikkar on 5/7/21.
//

#ifndef IDEAL_GAS_JSON_HELPER_H
#define IDEAL_GAS_JSON_HELPER_H

#include <nlohmann/json.hpp>
#include <cinder/gl/gl.h>
#include <glm/vec2.hpp>
#include <string>

namespace nlohmann {

template<> struct adl_serializer<glm::vec2> {
  static void to_json(json& json_array, const glm::vec2& vec);

  static void from_json(const json& json_array, glm::vec2& vec);
};

template<> struct adl_serializer<ci::Color8u> {
  static const std::string kColorJsonRedKey;
  static const std::string kColorJsonGreenKey;
  static const std::string kColorJsonBlueKey;

  static void to_json(json& json_array, const ci::Color8u& vec);

  static void from_json(const json& json_array, ci::Color8u& vec);
};

} // namespace nlohmann

#endif  // IDEAL_GAS_JSON_HELPER_H
