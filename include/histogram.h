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

  void DrawBins(float lower_right_y_coordinate) const;

  void DrawXAxisTicks(float lower_right_y_coord) const;

  void DrawYAxisTicks() const;

  void DrawAxisLabels() const;

  void UpdateBinValues(const std::vector<float>& updated_values);

  std::string GetDataLabel() const;

  std::vector<size_t> GetBinValues() const;

  static constexpr float kBinCount = 20;
  static constexpr float kDefaultSingleBinRange = 0.5;

  static constexpr float kDefaultBinHeightIncrement = 5;
  static constexpr float kAxisTickLength = 5;

  static constexpr float kDefaultXCoordinate = 50;
  static constexpr float kDefaultGraphWidth = 200;
  static constexpr float kDefaultGraphHeight = 100;

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

  float graph_bounding_box_width_;
  float graph_bounding_box_height_;

  static const char* kGraphBoundColor;
};

}

#endif  // IDEAL_GAS_HISTOGRAM_H
