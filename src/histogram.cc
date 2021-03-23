#include "histogram.h"
#include "cinder/gl/gl.h"

namespace idealgas {

using std::vector;
using std::string;
using glm::vec2;

const char* Histogram::kGraphBoundColor = "white";

Histogram::Histogram(const string& label, float single_bin_range,
                     float top_left_x, float top_left_y,
                     float red, float green, float blue) :
      data_label_(label), red_intensity_(red), green_intensity_(green),
      blue_intensity_(blue), bin_values_(),
      single_bin_range_span_(single_bin_range),
      bin_display_height_increment_(kDefaultBinHeightIncrement),
      upper_left_x_coordinate_(top_left_x),
      upper_left_y_coordinate_(top_left_y),
      graph_bounding_box_width_(kDefaultGraphWidth),
      graph_bounding_box_height_(kDefaultGraphHeight) {

  if (single_bin_range <= 0) {
    throw std::invalid_argument("Bin range must be greater than 0.");
  }

  bin_display_width_ = graph_bounding_box_width_ / kBinCount;
  vector<size_t>(kBinCount, 0).swap(bin_values_);
}

std::string Histogram::GetDataLabel() const {
  return data_label_;
}

std::vector<size_t> Histogram::GetBinValues() const {
  return bin_values_;
}


void Histogram::Draw() const {
  float lower_right_x_coord =
      upper_left_x_coordinate_ + graph_bounding_box_width_;
  float lower_right_y_coord =
      upper_left_y_coordinate_ + graph_bounding_box_height_;

  DrawBins(lower_right_y_coord);
  DrawXAxisTicks(lower_right_y_coord);
  // TODO: DrawYAxisTicks();
  // TODO: DrawAxisLabels();

  vec2 top_left_point = vec2(upper_left_x_coordinate_, upper_left_y_coordinate_);
  vec2 bottom_right_point = vec2(lower_right_x_coord, lower_right_y_coord);

  ci::gl::color(ci::Color(kGraphBoundColor));
  ci::gl::drawStrokedRect(ci::Rectf(top_left_point, bottom_right_point));
}

void Histogram::DrawBins(float lower_right_y_coord) const {
  // TODO: MAKE TICKS FIXED
  ci::gl::color(ci::Color(red_intensity_, green_intensity_, blue_intensity_));
  for (size_t bin_idx = 0; bin_idx < bin_values_.size(); bin_idx++) {
    float distance_from_origin =
        upper_left_x_coordinate_ + (bin_idx * bin_display_width_);

    float bin_height = bin_values_[bin_idx] * bin_display_height_increment_;
    float bin_height_y_coord = lower_right_y_coord - bin_height;

    vec2 bin_top_left_point = vec2(distance_from_origin, bin_height_y_coord);
    vec2 bin_lower_right_point =
        vec2(distance_from_origin + bin_display_width_, lower_right_y_coord);

    ci::gl::drawSolidRect(ci::Rectf(bin_top_left_point, bin_lower_right_point));
  }
}

void Histogram::DrawXAxisTicks(float lower_right_y_coord) const {
  ci::gl::color(ci::Color(kGraphBoundColor));

  for (size_t bin_idx = 0; bin_idx <= kBinCount; bin_idx++) {
    float distance_from_origin =
        upper_left_x_coordinate_ + (bin_idx * bin_display_width_);

    vec2 starting_point = vec2(distance_from_origin, lower_right_y_coord);
    vec2 ending_point =
        vec2(distance_from_origin, lower_right_y_coord + kAxisTickLength);

    ci::gl::drawLine(starting_point, ending_point);
//    ci::gl::drawStringCentered();
  }

  ci::gl::drawStringCentered("hi this is a test testing width", vec2(150, lower_right_y_coord + 25));
}

void Histogram::DrawYAxisTicks() const {
}

void Histogram::DrawAxisLabels() const {

}

void Histogram::UpdateBinValues(const std::vector<float>& updated_values) {
  // make a shallow copy of all values in the vector so we can sort it in place
  vector<float> sorted_values;
  sorted_values.assign(updated_values.begin(), updated_values.end());

  // sort values first to achieve O(n log n) instead of O(n^2) runtime
  std::sort(sorted_values.begin(), sorted_values.end(), std::less<float>());

  vector<size_t>(kBinCount, 0).swap(bin_values_);

  size_t value_idx = 0;

  // Go through each bin in the histogram...
  for (size_t bin_idx = 0; bin_idx < bin_values_.size(); bin_idx++) {
    // The 0th bin ends at value <single_bin_range_span_>, so add 1 to get end
    while (value_idx < sorted_values.size() &&
           sorted_values[value_idx] <= (bin_idx + 1) * single_bin_range_span_) {
      // ...and update bin as long as there is a value small enough to fit in it
      bin_values_[bin_idx]++;
      value_idx++;
    }
  }
}

} // namespace idealgas
