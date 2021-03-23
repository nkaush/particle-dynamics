#include <catch2/catch.hpp>
#include <histogram.h>

using idealgas::Histogram;
using std::vector;

TEST_CASE("Test Constructor And Exception Throwing For Invalid Parameters") {
  SECTION("Specified bin range is less than 0") {
    REQUIRE_THROWS_AS(Histogram("data", 1, -5, 0, 0, 0, 0, 0),
                      std::invalid_argument);
  }

  SECTION("Specified bin range is 0") {
    REQUIRE_THROWS_AS(Histogram("data", 1, 0, 0, 0, 0, 0, 0),
                      std::invalid_argument);
  }

  // Can't test for negative bin count since it is unsigned long (no negatives)
  SECTION("Specified bin count is 0") {
    REQUIRE_THROWS_AS(Histogram("data", 0, 1, 0, 0, 0, 0, 0),
                      std::invalid_argument);
  }
}

TEST_CASE("Testing Histogram Bin Limits and Edges") {
  size_t bin_count = Histogram::kDefaultBinCount;

  SECTION("Empty vector provided yields empty histogram") {
    Histogram hist = Histogram("data", bin_count, 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({});
    vector<size_t> bin_values = hist.GetBinValues();

    REQUIRE(std::accumulate(bin_values.begin(), bin_values.end(), 0ul) == 0ul);
  }

  SECTION("All zeroes belong in first bin") {
    Histogram hist = Histogram("data", bin_count, 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({0, 0, 0, 0});

    REQUIRE(hist.GetBinValues().at(0) == 4);
  }

  SECTION("All values at bin range threshold belong in first bin") {
    Histogram hist = Histogram("data", bin_count, 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({0.5, 0.5, 0.5, 0.5});

    REQUIRE(hist.GetBinValues().at(0) == 4);
  }

  SECTION("All values past bin range threshold belong in second bin") {
    Histogram hist = Histogram("data", bin_count, 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({0.51, 0.51, 0.51, 0.51});

    bool is_first_bin_empty = hist.GetBinValues().at(0) == 0;
    bool is_second_bin_empty = hist.GetBinValues().at(1) == 4;
    bool is_result_as_expected = is_first_bin_empty && is_second_bin_empty;

    REQUIRE(is_result_as_expected);
  }

  SECTION("Bins should exclude values past max bin") {
    Histogram hist = Histogram("data", bin_count, 0.1, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({2.01, 2.01, 2.01, 2.01, 2.01, 2.01});
    vector<size_t> bin_values = hist.GetBinValues();

    REQUIRE(std::accumulate(bin_values.begin(), bin_values.end(), 0ul) == 0ul);
  }

  SECTION("Bins should exclude negative values (before min bin)") {
    Histogram hist = Histogram("data", bin_count, 0.1, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({-1, -1, -2, -3, -4});
    vector<size_t> bin_values = hist.GetBinValues();

    REQUIRE(std::accumulate(bin_values.begin(), bin_values.end(), 0ul) == 0ul);
  }
}

TEST_CASE("Testing Bin Updating for Sorted Vectors") {
  size_t bin_count = Histogram::kDefaultBinCount;

  SECTION("Sorted values are placed into multiple bins") {
    Histogram hist = Histogram("data", bin_count, 1, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({1, 1, 2, 2, 2, 2, 3, 3, 3, 4});
    vector<size_t> bin_values = hist.GetBinValues();

    bool are_bins_accurate = bin_values.at(0) == 2;
    are_bins_accurate &= bin_values.at(1) == 4;
    are_bins_accurate &= bin_values.at(2) == 3;
    are_bins_accurate &= bin_values.at(3) == 1;

    REQUIRE(are_bins_accurate);
  }

  SECTION("Sorted values placed into bins and values past bin limit excluded") {
    Histogram hist = Histogram("data", bin_count, 1, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({0, 10, 10, 10, 10, 15, 15, 20, 20, 20, 21});
    vector<size_t> bin_values = hist.GetBinValues();

    bool are_bins_accurate = bin_values.at(0) == 1;
    are_bins_accurate &= bin_values.at(9) == 4;
    are_bins_accurate &= bin_values.at(14) == 2;
    are_bins_accurate &= bin_values.at(19) == 3;
    are_bins_accurate &=
        std::accumulate(bin_values.begin(), bin_values.end(), 0ul) == 10ul;

    REQUIRE(are_bins_accurate);
  }

  SECTION("Sorted values placed into bins exclude less than minimum value") {
    Histogram hist = Histogram("data", bin_count, 1, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({-10, -5, 0, 10, 10, 10, 10, 15, 15, 20, 20});
    vector<size_t> bin_values = hist.GetBinValues();

    bool are_bins_accurate = bin_values.at(0) == 1;
    are_bins_accurate &= bin_values.at(9) == 4;
    are_bins_accurate &= bin_values.at(14) == 2;
    are_bins_accurate &= bin_values.at(19) == 2;
    are_bins_accurate &=
        std::accumulate(bin_values.begin(), bin_values.end(), 0ul) == 9ul;

    REQUIRE(are_bins_accurate);
  }

  SECTION("Bin counts are reset after each update") {
    Histogram hist = Histogram("data", bin_count, 0.5, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({0.5, 0.5, 0.5, 0.5});

    bool is_first_iteration_as_expected = hist.GetBinValues().at(0) == 4;
    is_first_iteration_as_expected &= hist.GetBinValues().at(1) == 0;

    hist.UpdateBinDistribution({0.51, 0.51, 0.51, 0.51});

    bool is_second_iteration_as_expected = hist.GetBinValues().at(0) == 0;
    is_second_iteration_as_expected &= hist.GetBinValues().at(1) == 4;

    bool is_result_as_expected = is_first_iteration_as_expected
                                 && is_second_iteration_as_expected;

    REQUIRE(is_result_as_expected);
  }
}

TEST_CASE("Testing Bin Updating for Unsorted Vectors") {
  size_t bin_count = Histogram::kDefaultBinCount;

  SECTION("Unsorted values are placed into multiple bins") {
    Histogram hist = Histogram("data", bin_count, 1, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({3, 1, 4, 2, 3, 3, 2, 2, 1, 3, 1, 4, 2, 3});
    vector<size_t> bin_values = hist.GetBinValues();

    bool are_bins_accurate = bin_values.at(0) == 3;
    are_bins_accurate &= bin_values.at(1) == 4;
    are_bins_accurate &= bin_values.at(2) == 5;
    are_bins_accurate &= bin_values.at(3) == 2;

    REQUIRE(are_bins_accurate);
  }

  SECTION("Unsorted values placed into bins exclude greater than bin limit") {
    Histogram hist = Histogram("data", 5, 4, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({0, 5, 2, 54, 20, 17, 0, 2, 4, 35, 6, 2, 4, 5});
    vector<size_t> bin_values = hist.GetBinValues();

    bool are_bins_accurate = bin_values.at(0) == 7;
    are_bins_accurate &= bin_values.at(1) == 3;
    are_bins_accurate &= bin_values.at(4) == 2;
    are_bins_accurate &=
        std::accumulate(bin_values.begin(), bin_values.end(), 0ul) == 12ul;

    REQUIRE(are_bins_accurate);
  }

  SECTION("Unsorted values placed in bins exclude less than bin minimum") {
    Histogram hist = Histogram("data", 2, 5, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({5, 3, -10, 0, 10, 1, -5, 0, -10, 6, 4, 9, 8});
    vector<size_t> bin_values = hist.GetBinValues();

    bool are_bins_accurate = bin_values.at(0) == 6;
    are_bins_accurate &= bin_values.at(1) == 4;
    are_bins_accurate &=
        std::accumulate(bin_values.begin(), bin_values.end(), 0ul) == 10ul;

    REQUIRE(are_bins_accurate);
  }

  SECTION("Unsorted values placed in bins exclude values out of range") {
    Histogram hist = Histogram("data", 2, 5, 0, 0, 0, 0, 0);

    hist.UpdateBinDistribution({5, 13, -1, 0, 10, 1, -5, 0, -0.1, 6, 4, 11, 8});
    vector<size_t> bin_values = hist.GetBinValues();

    bool are_bins_accurate = bin_values.at(0) == 5;
    are_bins_accurate &= bin_values.at(1) == 3;
    are_bins_accurate &=
        std::accumulate(bin_values.begin(), bin_values.end(), 0ul) == 8ul;

    REQUIRE(are_bins_accurate);
  }
}