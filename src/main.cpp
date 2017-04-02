// CONFIGURATION SOURCE

#include <algorithm>
#include <iomanip>
#include <mutex>

#include "configuration.h"
#include "cache.h"
#include "device.h"

namespace machine {

void Usage() {
  std::cout <<
      "\n"
      "Command line options : machine <options>\n"
      "   -a --hierarchy_type                 :  hierarchy type\n"
      "   -c --caching_type                   :  caching type\n"
      "   -f --migration_frequency            :  migration frequency from NVM to DRAM\n"
      "   -l --logging_type                   :  logging type\n"
      "   -s --machine_size                   :  machine size\n"
      "   -o --operation_count                :  operation count\n"
      "   -v --verbose                        :  verbose\n";
  exit(EXIT_FAILURE);
}

static struct option opts[] = {
    {"hierarchy_type", optional_argument, NULL, 'a'},
    {"caching_type", optional_argument, NULL, 'c'},
    {"migration_frequency", optional_argument, NULL, 'f'},
    {"logging_type", optional_argument, NULL, 'l'},
    {"machine_size", optional_argument, NULL, 's'},
    {"operation_count", optional_argument, NULL, 'o'},
    {"verbose", optional_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

static void ValidateHierarchyType(const configuration &state) {
  if (state.hierarchy_type < 1 || state.hierarchy_type > 4) {
    printf("Invalid hierarchy_type :: %d\n", state.hierarchy_type);
    exit(EXIT_FAILURE);
  }
  else {
    printf("%30s : %s\n", "caching_type",
           HierarchyTypeToString(state.hierarchy_type).c_str());
  }
}

static void ValidateCachingType(const configuration &state) {
  if (state.caching_type < 1 || state.caching_type > 4) {
    printf("Invalid caching_type :: %d\n", state.caching_type);
    exit(EXIT_FAILURE);
  }
  else {
    printf("%30s : %s\n", "caching_type",
           CachingTypeToString(state.caching_type).c_str());
  }
}

static void ValidateLoggingType(const configuration &state) {
  if (state.logging_type < 1 || state.logging_type > 2) {
    printf("Invalid logging_type :: %d\n", state.logging_type);
    exit(EXIT_FAILURE);
  }
  else {
    printf("%30s : %s\n", "logging_type",
           LoggingTypeToString(state.logging_type).c_str());
  }
}

static void ValidateMigrationFrequency(const configuration &state){
  printf("%30s : %lu\n", "migration_frequency", state.migration_frequency);
}

static void ValidateMachineSize(const configuration &state) {
  printf("%30s : %lu\n", "machine_size", state.machine_size);
}

static void ValidateOperationCount(const configuration &state) {
  printf("%30s : %lu\n", "operation_count", state.operation_count);
}

static void ConstructDeviceList(configuration &state){

  auto last_device_type = GetLastDevice(state.hierarchy_type);
  Device dram_device = DeviceFactory::GetDevice(DEVICE_TYPE_DRAM,
                                                state.caching_type,
                                                state.machine_size,
                                                last_device_type);
  Device nvm_device = DeviceFactory::GetDevice(DEVICE_TYPE_NVM,
                                               state.caching_type,
                                               state.machine_size,
                                               last_device_type);
  Device ssd_device = DeviceFactory::GetDevice(DEVICE_TYPE_SSD,
                                               state.caching_type,
                                               state.machine_size,
                                               last_device_type);

  switch (state.hierarchy_type) {
    case HIERARCHY_TYPE_NVM: {
      state.devices = {nvm_device};
      state.memory_devices = {nvm_device};
      state.storage_devices = {nvm_device};
    }
    break;
    case HIERARCHY_TYPE_DRAM_NVM: {
      state.devices = {dram_device, nvm_device};
      state.memory_devices = {dram_device, nvm_device};
      state.storage_devices = {nvm_device};
    }
    break;
    case HIERARCHY_TYPE_DRAM_SSD: {
      state.devices = {dram_device, ssd_device};
      state.memory_devices = {dram_device};
      state.storage_devices = {ssd_device};
    }
    break;
    case HIERARCHY_TYPE_DRAM_NVM_SSD: {
      state.devices = {dram_device, nvm_device, ssd_device};
      state.memory_devices = {dram_device, nvm_device};
      state.storage_devices = {nvm_device, ssd_device};
    }
    break;
    default:
      break;
  }

}


void ParseArguments(int argc, char *argv[], configuration &state) {

  // Default Values
  state.verbose = false;

  state.hierarchy_type = HIERARCHY_TYPE_DRAM_NVM_SSD;
  state.logging_type = LOGGING_TYPE_WBL;
  state.caching_type = CACHING_TYPE_FIFO;
  state.machine_size = 1000;
  state.migration_frequency = 3;
  state.operation_count = 1000;

  // Parse args
  while (1) {
    int idx = 0;
    int c = getopt_long(argc, argv,
                        "a:c:f:l:o:s:vh",
                        opts, &idx);

    if (c == -1) break;

    switch (c) {
      case 'a':
        state.hierarchy_type = (HierarchyType)atoi(optarg);
        break;
      case 'c':
        state.caching_type = (CachingType)atoi(optarg);
        break;
      case 'f':
        state.migration_frequency = atoi(optarg);
        break;
      case 'l':
        state.logging_type = (LoggingType)atoi(optarg);
        break;
      case 'o':
        state.operation_count = atoi(optarg);
        break;
      case 's':
        state.machine_size = atoi(optarg);
        break;
      case 'v':
        state.verbose = atoi(optarg);
        break;
      case 'h':
        Usage();
        break;

      default:
        printf("Unknown option: -%c-\n", c);
        Usage();
    }
  }

  // Run validators
  printf("//===----------------------------------------------------------------------===//\n");
  printf("//                               MACHINE                                      //\n");
  printf("//===----------------------------------------------------------------------===//\n");

  ValidateHierarchyType(state);
  ValidateLoggingType(state);
  ValidateCachingType(state);
  ValidateMachineSize(state);
  ValidateMigrationFrequency(state);
  ValidateOperationCount(state);

  printf("//===----------------------------------------------------------------------===//\n");

  // Construct device list
  ConstructDeviceList(state);

}

}  // namespace machine
