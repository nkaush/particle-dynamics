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
  /**
   * Creates a new Histogram with the specified characteristics.
   * @param label - a string indicating the label of the data shown in histogram
   * @param bin_count - a size_t indicating the number of bins to display
   * @param single_bin_range - a float indicating the span of a single bin
   * @param top_left_x - a float indicating top left x-coord of the histogram
   * @param top_left_y - a float indicating top left y-coord of the histogram
   * @param red - a float from 0-1 indicating amount of red to color particle
   * @param green - float from 0-1 indicating amount of green to color particle
   * @param blue - a float from 0-1 indicating amount of blue to color particle
   * @param min_value - a float indicating the minimum value of histogram bins
   */
  Histogram(const std::string& label, size_t bin_count, float single_bin_range,
            float top_left_x, float top_left_y, float red, float green,
            float blue, float min_value=0);

  /**
   * Draws the histogram's bins, axis ticks, and axis labels.
   */
  void Draw() const;

  /**
   * Recalculates the distribution of values within the histogram bins using
   * the provided vector of values.
   * @param updated_values - a vector of floats with which to update the
   * distribution of values within the bins of the histogram
   */
  void UpdateBinDistribution(const std::vector<float>& updated_values);

  std::string GetDataLabel() const;

  std::vector<size_t> GetBinValues() const;

  static constexpr float kDefaultSingleBinRange = 0.5;
  static constexpr float kDefaultBinCount = 20;

  static constexpr float kDefaultGraphHeight = 100;
  static constexpr float kDefaultGraphWidth = 200;

 private:
  std::string data_label_;
  float red_intensity_;
  float green_intensity_;
  float blue_intensity_;

  // the number of values assigned to each bin
  std::vector<size_t> bin_values_;
  float minimum_value_;
  // the range of values that belong to the bin
  float single_bin_range_span_;

  // how wide to display the bin as
  float bin_display_width_;
  // sets how much 1 value pushes the bin height up
  float bin_display_height_increment_;

  // These coordinates determine the location of the bounds of the histogram
  float upper_left_x_coordinate_;
  float upper_left_y_coordinate_;
  float lower_right_x_coordinate_;
  float lower_right_y_coordinate_;

  // These represent the dimensions of the histogram box
  float graph_bounding_box_width_;
  float graph_bounding_box_height_;

  static const char* kGraphBoundColor;

  // These determine how to display the labels on the x-axis
  static const std::string kXAxisLabelStart;
  static const std::string kXAxisLabelEnd;
  static constexpr float kXAxisLabelPadding = 25;

  // These determine how to display the labels on the y-axis
  static const std::string kYAxisLabel;
  static constexpr float kYAxisLabelRotation = M_PI / 2;
  static constexpr float kYAxisLabelHorizontalPadding = 50;
  static constexpr float kYAxisLabelVerticalPadding = 230;

  // These determine how to display the tick marks on both axes
  static constexpr float kAxisTickLength = 5;
  static constexpr float kAxisTickLabelPadding = 10;
  // Determines how often to print the tick label - 2 means every other tick
  static constexpr size_t kXAxisTickDisplayIncrement = 2;
  static constexpr size_t kYAxisTickDisplayIncrement = 5;

  static constexpr float kDefaultBinHeightIncrement = 4;

  /**
   * Draws the boxes that represent each of the bars of the histogram.
   */
  void DrawBins() const;

  /**
   * Draws the tick marks and their corresponding labels on the x-axis.
   */
  void DrawXAxisTicksAndLabels() const;

  /**
   * Draws the tick marks and their corresponding labels on the y-axis.
   */
  void DrawYAxisTicksAndLabels() const;

  /**
   * Draws the text labels for the x and y axes.
   */
  void DrawAxisLabels() const;
};

} // namespace idealgas

#endif  // IDEAL_GAS_HISTOGRAM_H
