//
// Created by Neil Kaushikkar on 3/19/21.
//

#ifndef IDEAL_GAS_HISTOGRAM_H
#define IDEAL_GAS_HISTOGRAM_H

#include "gas_particle.h"

namespace idealgas {

class Histogram {
 public:
  Histogram();

  void UpdateGraph();

  std::vector<std::vector<float>> SortVelocitiesToBins(std::vector<GasParticle>);

 private:

};

}

#endif  // IDEAL_GAS_HISTOGRAM_H
