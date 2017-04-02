// WORKLOAD SOURCE

#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>

#include "macros.h"
#include "workload.h"
#include "distribution.h"
#include "configuration.h"
#include "device.h"
#include "cache.h"

namespace machine {

const static std::string OUTPUT_FILE = "outputfile.summary";
std::ofstream out(OUTPUT_FILE);

size_t query_itr;

void RunMachineTest() {

  // Run the benchmark once
  MachineHelper();

}

}  // namespace machine

