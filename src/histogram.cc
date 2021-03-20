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

std::string Histogram::GetDataLabel() const {
  return data_label_;
}

void idealgas::Histogram::UpdateGraph() {

}

void Histogram::Draw() const {
  float lower_right_x_coord =
      upper_left_x_coordinate_ + graph_bounding_box_width_;
  float lower_right_y_coord =
      upper_left_y_coordinate_ + graph_bounding_box_height_;

  ci::gl::color(ci::Color(red_intensity_, green_intensity_, blue_intensity_));
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

  vec2 top_left_point = vec2(upper_left_x_coordinate_, upper_left_y_coordinate_);
  vec2 bottom_right_point = vec2(lower_right_x_coord, lower_right_y_coord);

  ci::gl::color(ci::Color(kGraphBoundColor));
  ci::gl::drawStrokedRect(ci::Rectf(top_left_point, bottom_right_point));
}

void Histogram::UpdateBinCounts(std::vector<float>& values) {
  // sort values first to achieve O(n log n) instead of O(n^2) runtime
  std::sort(values.begin(), values.end(), std::less<float>());
  vector<size_t>().swap(bin_counts_);

  // The index of the bin starting at the bin with the min value
  size_t bin_idx_from_min_val = 0;
  // The index of the bins starting from 0 to the max value
  size_t bin_idx_all_bins = 0;

  // Advance the current bin index to the index of the minimum value
  while (values.at(0) > bin_idx_all_bins * bin_range_span_) {
    bin_idx_all_bins++;
  }

  for (float val : values) {
    // if the current value is too big for the current bin, move to next bins
    while (val > bin_idx_all_bins * bin_range_span_) {
      bin_idx_all_bins++;
      bin_idx_from_min_val++;
      bin_counts_.push_back(0);
    }

    // if min value is 0, we have not advanced bins, so need to create 1st bin
    if (bin_counts_.empty()) {
      bin_counts_.push_back(0);
    }

    bin_counts_[bin_idx_from_min_val]++;
  }
}

}

