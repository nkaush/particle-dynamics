//
// Created by Neil Kaushikkar on 3/19/21.
//

#ifndef IDEAL_GAS_HISTOGRAM_H
#define IDEAL_GAS_HISTOGRAM_H

#include <string>
#include <vector>

namespace idealgas {

class Histogram {
 public:
  Histogram(const std::string& label, float single_bin_range, float top_left_x,
            float top_left_y, float red, float green, float blue);

  void Draw() const;

  void DrawBins() const;

  void DrawXAxisTicksAndLabels() const;

  void DrawYAxisTicksAndLabels() const;

  void DrawAxisLabels() const;

  void UpdateBinValues(const std::vector<float>& updated_values);

  std::string GetDataLabel() const;

  std::vector<size_t> GetBinValues() const;

  static constexpr float kDefaultSingleBinRange = 0.5;

  static constexpr float kDefaultXCoordinate = 50;

  static constexpr float kDefaultGraphHeight = 100;
  static constexpr float kDefaultGraphWidth = 200;

 private:
  std::string data_label_;
  float red_intensity_;
  float green_intensity_;
  float blue_intensity_;

  // the number of values assigned to each bin
  std::vector<size_t> bin_values_;
  // the range of values that belong to the bin
  float single_bin_range_span_;

  // how wide to display the bin as
  float bin_display_width_;
  // sets how much 1 value pushes the bin height up
  float bin_display_height_increment_;

  float upper_left_x_coordinate_;
  float upper_left_y_coordinate_;
  float lower_right_x_coordinate_;
  float lower_right_y_coordinate_;

  float graph_bounding_box_width_;
  float graph_bounding_box_height_;

  static const char* kGraphBoundColor;

  static const std::string kXAxisLabelStart;
  static const std::string kXAxisLabelEnd;
  static constexpr float kXAxisLabelPadding = 25;

  static const std::string kYAxisLabel;
  static constexpr float kYAxisLabelRotation = M_PI / 2;
  static constexpr float kYAxisLabelHorizontalPadding = 50;
  static constexpr float kYAxisLabelVerticalPadding = 230;

  static constexpr float kAxisTickLength = 5;
  static constexpr float kAxisTickLabelPadding = 10;

  static constexpr size_t kXAxisTickDisplayIncrement = 2;
  static constexpr size_t kYAxisTickDisplayIncrement = 5;

  static constexpr float kBinCount = 20;
  static constexpr float kDefaultBinHeightIncrement = 5;
  // Determines how often to print the tick label - 2 means every other tick
};

} // namespace idealgas

#endif  // IDEAL_GAS_HISTOGRAM_H
