#include "histogram.h"
#include "cinder/gl/gl.h"

namespace idealgas {

using std::vector;
using std::string;
using glm::vec2;

// Define the non literal static variables
const char* Histogram::kGraphBoundColor = "white";
const string Histogram::kXAxisLabelStart = "Speed (pixels/frame) for \"";
const string Histogram::kXAxisLabelEnd = "\" Particles";
const string Histogram::kYAxisLabel = "Frequency";

Histogram::Histogram(const string& label, size_t num_bins, float single_bin_range,
                     float top_left_x, float top_left_y,
                     const ci::Color8u& color, float min_value) :
      data_label_(label), color_(color), bin_values_(), minimum_value_(min_value),
      single_bin_range_span_(single_bin_range),
      bin_display_height_increment_(kDefaultBinHeightIncrement),
      upper_left_x_coordinate_(top_left_x),
      upper_left_y_coordinate_(top_left_y),
      graph_bounding_box_width_(kDefaultGraphWidth),
      graph_bounding_box_height_(kDefaultGraphHeight) {

  if (single_bin_range <= 0) {
    throw std::invalid_argument("Bin range must be greater than 0.");
  } else if (num_bins <= 0) {
    throw std::invalid_argument("The number of bins must be at least 1.");
  }

  lower_right_x_coordinate_ = upper_left_x_coordinate_
                              + graph_bounding_box_width_;
  lower_right_y_coordinate_ = upper_left_y_coordinate_
                              + graph_bounding_box_height_;

  bin_display_width_ = graph_bounding_box_width_ / num_bins;
  vector<size_t>(num_bins, 0).swap(bin_values_);
}

std::string Histogram::GetDataLabel() const {
  return data_label_;
}

std::vector<size_t> Histogram::GetBinValues() const {
  return bin_values_;
}

void Histogram::UpdateBinDistribution(const std::vector<float>& updated_values) {
  // make a shallow copy of all values in the vector so we can sort it in place
  vector<float> sorted_values;
  sorted_values.assign(updated_values.begin(), updated_values.end());

  // sort values first so we only go through bins and values once sequentially
  std::sort(sorted_values.begin(), sorted_values.end(), std::less<float>());

  // reset the bin counts to 0, so we can fill it up again
  vector<size_t>(bin_values_.size(), 0).swap(bin_values_);

  size_t value_idx = 0;

  // Go through each bin in the histogram...
  for (size_t bin_idx = 0; bin_idx < bin_values_.size(); bin_idx++) {
    // The 0th bin ends at value <single_bin_range_span_>, so add 1 to get end
    while (value_idx < sorted_values.size() &&
           sorted_values[value_idx] <= (bin_idx + 1) * single_bin_range_span_) {
      // ...and update bin as long as there is a value small enough to fit in it
      if (sorted_values[value_idx] >= minimum_value_) {
        bin_values_[bin_idx]++;
      }
      value_idx++;
    }
  }
}

void Histogram::Draw() const {
  DrawBins();
  DrawXAxisTicksAndLabels();
  DrawYAxisTicksAndLabels();
  DrawAxisLabels();

  vec2 top_left_point = vec2(upper_left_x_coordinate_, upper_left_y_coordinate_);
  vec2 bottom_right_point = vec2(lower_right_x_coordinate_,
                                 lower_right_y_coordinate_);

  ci::gl::color(ci::Color(kGraphBoundColor));
  ci::gl::drawStrokedRect(ci::Rectf(top_left_point, bottom_right_point));
}

void Histogram::DrawBins() const {
  ci::gl::color(color_);
  for (size_t bin_idx = 0; bin_idx < bin_values_.size(); bin_idx++) {
    // find the location on the axis to draw the bin
    float distance_from_origin =
        upper_left_x_coordinate_ + (bin_idx * bin_display_width_);

    float bin_height = bin_values_[bin_idx] * bin_display_height_increment_;

    // If bin will overflow the limits, cut off the extra portion
    if (bin_height > graph_bounding_box_height_) {
      bin_height = graph_bounding_box_height_;
    }

    float bin_height_y_coord = lower_right_y_coordinate_ - bin_height;

    vec2 bin_top_left_point = vec2(distance_from_origin, bin_height_y_coord);
    vec2 bin_lower_right_point = vec2(distance_from_origin + bin_display_width_,
                                      lower_right_y_coordinate_);

    ci::gl::drawSolidRect(ci::Rectf(bin_top_left_point, bin_lower_right_point));
  }
}

void Histogram::DrawXAxisTicksAndLabels() const {
  ci::gl::color(ci::Color(kGraphBoundColor));

  for (size_t bin_idx = 0; bin_idx <= bin_values_.size(); bin_idx++) {
    float distance_from_origin =
        upper_left_x_coordinate_ + (bin_idx * bin_display_width_);

    // Calculate the coordinates of the tick start/ends
    vec2 starting_point(distance_from_origin, lower_right_y_coordinate_);
    vec2 ending_point
        (distance_from_origin, lower_right_y_coordinate_ + kAxisTickLength);

    ci::gl::drawLine(starting_point, ending_point);

    // Only label the tick if it is a multiple of the increment specified
    if (bin_idx % kXAxisTickDisplayIncrement == 0) {
      std::stringstream formatted_tick_label;
      formatted_tick_label << (bin_idx * single_bin_range_span_);

      ci::gl::drawStringCentered(formatted_tick_label.str(),
        vec2(distance_from_origin, lower_right_y_coordinate_
                                    + kAxisTickLabelPadding));
    }
  }
}

void Histogram::DrawYAxisTicksAndLabels() const {
  ci::gl::color(ci::Color(kGraphBoundColor));

  float max_bin_value =
      graph_bounding_box_height_ / bin_display_height_increment_;

  for (size_t bin_idx = 0; bin_idx <= max_bin_value; bin_idx++) {
    float distance_from_origin =
        lower_right_y_coordinate_ - (bin_idx * bin_display_height_increment_);

    // Calculate the coordinates of the tick start/ends
    vec2 starting_point(upper_left_x_coordinate_, distance_from_origin);
    vec2 ending_point
        (upper_left_x_coordinate_ - kAxisTickLength, distance_from_origin);

    ci::gl::drawLine(starting_point, ending_point);

    // Only label the tick if it is a multiple of the increment specified
    if (bin_idx % kYAxisTickDisplayIncrement == 0) {
      std::stringstream formatted_tick_label;
      formatted_tick_label << bin_idx;

      ci::gl::drawStringCentered(formatted_tick_label.str(),
        vec2(upper_left_x_coordinate_ - kAxisTickLabelPadding,
                                      distance_from_origin));
    }
  }
}

void Histogram::DrawAxisLabels() const {
  string x_label = kXAxisLabelStart + data_label_ + kXAxisLabelEnd;
  // Center the x coordinate by averaging the 2 bounds
  float centered_x = (upper_left_x_coordinate_ + lower_right_x_coordinate_) / 2;

  // Draw the x axis label
  ci::gl::drawStringCentered(
      x_label, vec2(centered_x, lower_right_y_coordinate_ + kXAxisLabelPadding));

  // Rotate -90 degrees to format Y axis label correctly
  ci::gl::rotate(-kYAxisLabelRotation);
  ci::gl::drawStringCentered(kYAxisLabel,
    vec2(kYAxisLabelHorizontalPadding - lower_right_y_coordinate_,
         lower_right_x_coordinate_ - kYAxisLabelVerticalPadding));
  ci::gl::rotate(kYAxisLabelRotation); // un-rotate to preserve other drawings
}

} // namespace idealgas
