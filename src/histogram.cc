#include "histogram.h"

namespace idealgas {

using std::vector;
using std::string;
using glm::vec2;

const char* Histogram::kGraphBoundColor = "white";

Histogram::Histogram(const string& label, float top_left_x, float top_left_y,
                     float red, float green, float blue) :
      data_label_(label), red_intensity_(red), green_intensity_(green),
      blue_intensity_(blue), bin_counts_(), bin_range_span_(0.5),
      bin_display_height_increment_(kDefaultBinHeightIncrement),
      upper_left_x_coordinate_(top_left_x),
      upper_left_y_coordinate_(top_left_y),
      graph_bounding_box_width_(kDefaultGraphWidth),
      graph_bounding_box_height_(kDefaultGraphHeight) {
  bin_display_width_ = graph_bounding_box_width_ / kBinCount;
  for (size_t i = 0; i < kBinCount; i++) {
    bin_counts_.push_back(0);
  }
}

void idealgas::Histogram::UpdateGraph() {

}

void Histogram::Draw() {
  float lower_right_x_coord =
      upper_left_x_coordinate_ + graph_bounding_box_width_;
  float lower_right_y_coord =
      upper_left_y_coordinate_ + graph_bounding_box_height_;

  vec2 top_left_point = vec2(upper_left_x_coordinate_, upper_left_y_coordinate_);
  vec2 bottom_right_point = vec2(lower_right_x_coord, lower_right_y_coord);

  ci::gl::color(ci::Color(kGraphBoundColor));
  ci::gl::drawStrokedRect(ci::Rectf(top_left_point, bottom_right_point));

  ci::gl::color(ci::Color(red_intensity_, blue_intensity_, green_intensity_));
  for (size_t idx = 0; idx < bin_counts_.size(); idx++) {
    float distance_from_origin =
        upper_left_x_coordinate_ + (idx * bin_display_width_);

    float bin_height = bin_counts_[idx] * bin_display_height_increment_;
    float bin_height_coord = lower_right_y_coord - bin_height;

    vec2 bin_top_left_point = vec2(distance_from_origin, bin_height_coord);
    vec2 bin_lower_right_point =
        vec2(distance_from_origin + bin_display_width_, lower_right_y_coord);

    ci::gl::drawSolidRect(ci::Rectf(bin_top_left_point, bin_lower_right_point));
  }
}

void Histogram::UpdateBinCounts(std::vector<float>& values) {
  std::sort(values.begin(), values.end(), std::less<float>());
  vector<size_t>().swap(bin_counts_);

  size_t current_bin_index = 0;
  size_t increment_index = 0;
  bin_counts_.push_back(0);
  for (float val : values) {
    bool was_changed = false;
    while (val > increment_index * bin_range_span_) {
      increment_index++;
      was_changed = true;
    }
    if (was_changed) {
      current_bin_index++;
      bin_counts_.push_back(0);
    }
    bin_counts_[current_bin_index]++;
  }
}

}

