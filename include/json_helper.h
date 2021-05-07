//
// Created by Neil Kaushikkar on 5/7/21.
//

#ifndef IDEAL_GAS_JSON_HELPER_H
#define IDEAL_GAS_JSON_HELPER_H

#include <nlohmann/json.hpp>
#include <glm/vec2.hpp>

namespace nlohmann {

template<> struct adl_serializer<glm::vec2> {
  static void to_json(json& json_array, const glm::vec2& vec);

  static void from_json(const json& json_array, glm::vec2& vec);
};

} // namespace nlohmann

#endif  // IDEAL_GAS_JSON_HELPER_H
