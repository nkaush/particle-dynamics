//
// Created by Neil Kaushikkar on 5/7/21.
//

#include "json_helper.h"
#include <vector>

namespace nlohmann {

using std::vector;
using std::string;
using glm::vec2;

void adl_serializer<glm::vec2>::to_json(json& json_array, const vec2& vec) {
  json_array = {vec.x, vec.y};
}

void adl_serializer<glm::vec2>::from_json(const json& json_array, vec2& vec) {
  auto values = json_array.get<vector<float>>();

  // If the 1st value is missing, set it to 0, otherwise, fill with 1st value
  if (values.empty()) {
    vec.x = 0;
  } else {
    vec.x = values.at(0);
  }

  // If the 2nd value is missing, set it to 0, otherwise, fill with 2nd value
  if (values.size() < 2) {
    vec.y = 0;
  } else {
    vec.y = values.at(1);
  }
}

const string adl_serializer<ci::Color8u>::kColorJsonRedKey = "red";
const string adl_serializer<ci::Color8u>::kColorJsonGreenKey = "green";
const string adl_serializer<ci::Color8u>::kColorJsonBlueKey = "blue";

void adl_serializer<ci::Color8u>::to_json(
    json& json_object, const ci::Color8u& color) {
  json_object = json {
      {kColorJsonRedKey, color.r},
      {kColorJsonGreenKey, color.g},
      {kColorJsonBlueKey, color.b}
  };
}

void adl_serializer<ci::Color8u>::from_json(
    const json& json_object, ci::Color8u& color) {
  vector<uint8_t*> colors = {&color.r, &color.g, &color.b};
  vector<string> keys =
      {kColorJsonRedKey, kColorJsonGreenKey, kColorJsonBlueKey};

  for (size_t i = 0; i < colors.size(); i++) {
    try {
      json_object.at( keys.at(i) ).get_to( *colors.at(i) );
    } catch (json::exception& e) {
      *colors.at(i) = 0;
    }
  }
}

} // namespace nlohmann
