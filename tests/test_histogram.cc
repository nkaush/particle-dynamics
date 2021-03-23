#include <catch2/catch.hpp>
#include <histogram.h>

using idealgas::Histogram;
using std::vector;

TEST_CASE("Testing Histogram Edge Cases") {
  SECTION("Specified bin range is less than 0") {
    REQUIRE_THROWS_AS(Histogram("data", -5, 0, 0, 0, 0, 0),
                      std::invalid_argument);
  }

  SECTION("Specified bin range is 0") {
    REQUIRE_THROWS_AS(Histogram("data", 0, 0, 0, 0, 0, 0),
                      std::invalid_argument);
  }

  SECTION("Empty vector provided yields empty histogram") {
    Histogram hist = Histogram("data", 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinValues({});
    vector<size_t> bin_values = hist.GetBinValues();

    REQUIRE(std::accumulate(bin_values.begin(), bin_values.end(), 0) == 0);
  }

  SECTION("All zeroes belong in first bin") {
    Histogram hist = Histogram("data", 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinValues({0, 0, 0, 0});

    REQUIRE(hist.GetBinValues().at(0) == 4);
  }

  SECTION("All values at bin range threshold belong in first bin") {
    Histogram hist = Histogram("data", 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinValues({0.5, 0.5, 0.5, 0.5});

    REQUIRE(hist.GetBinValues().at(0) == 4);
  }

  SECTION("All values past bin range threshold belong in second bin") {
    Histogram hist = Histogram("data", 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinValues({0.51, 0.51, 0.51, 0.51});

    bool is_first_bin_empty = hist.GetBinValues().at(0) == 0;
    bool is_second_bin_empty = hist.GetBinValues().at(1) == 4;
    bool is_result_as_expected = is_first_bin_empty && is_second_bin_empty;

    REQUIRE(is_result_as_expected);
  }
}

TEST_CASE("Testing Bin Updating for Sorted Vectors") {
  SECTION("Values are placed into multiple bins") {
    Histogram hist = Histogram("data", 1, 0, 0, 0, 0, 0);

    hist.UpdateBinValues({1, 1, 2, 2, 2, 2, 3, 3, 3, 4});
    vector<size_t> bin_values = hist.GetBinValues();

    bool are_bins_accurate = bin_values.at(0) == 2;
    are_bins_accurate &= bin_values.at(1) == 4;
    are_bins_accurate &= bin_values.at(2) == 3;
    are_bins_accurate &= bin_values.at(3) == 1;

    REQUIRE(are_bins_accurate);
  }

  SECTION("Bin counts are reset after each update") {
    Histogram hist = Histogram("data", 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinValues({0.5, 0.5, 0.5, 0.5});

    bool is_first_iteration_as_expected = hist.GetBinValues().at(0) == 4;
    is_first_iteration_as_expected &= hist.GetBinValues().at(1) == 0;

    hist.UpdateBinValues({0.51, 0.51, 0.51, 0.51});

    bool is_second_iteration_as_expected = hist.GetBinValues().at(0) == 0;
    is_second_iteration_as_expected &= hist.GetBinValues().at(1) == 4;

    bool is_result_as_expected = is_first_iteration_as_expected
                                 && is_second_iteration_as_expected;

    REQUIRE(is_result_as_expected);
  }

  SECTION("Bins should exclude values past max bin") {
    Histogram hist = Histogram("data", 0.1, 0, 0, 0, 0, 0);

    hist.UpdateBinValues({2.01, 2.01, 2.01, 2.01, 2.01, 2.01});
    vector<size_t> bin_values = hist.GetBinValues();

    REQUIRE(std::accumulate(bin_values.begin(), bin_values.end(), 0) == 0);
  }
}

TEST_CASE("Testing Bin Updating for Unsorted Vectors") {

}