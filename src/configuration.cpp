// CONFIGURATION SOURCE

#include "include/configuration.h"

#include "gflags/gflags.h"

namespace sqlcheck {

std::string RiskLevelToString(const RiskLevel& risk_level){

  switch (risk_level) {
    case RISK_LEVEL_HIGH:
      return "HIGH RISK";
    case RISK_LEVEL_MEDIUM:
      return "MEDIUM RISK";
    case RISK_LEVEL_LOW:
      return "LOW RISK";
    case RISK_LEVEL_NONE:
      return "HINTS";
    case RISK_LEVEL_ALL:
      return "ALL ANTI-PATTERNS";

    case RISK_LEVEL_INVALID:
    default:
      return "INVALID";
  }

}

std::string RiskLevelToDetailedString(const RiskLevel& risk_level){

  switch (risk_level) {
    case RISK_LEVEL_HIGH:
      return "ONLY HIGH RISK ANTI-PATTERNS";
    case RISK_LEVEL_MEDIUM:
      return "ONLY MEDIUM AND HIGH RISK ANTI-PATTERNS";
    case RISK_LEVEL_LOW:
      return "ONLY ANTI-PATTERNS";
    case RISK_LEVEL_NONE:
      return "ALL ANTI-PATTERNS AND HINTS";
    case RISK_LEVEL_ALL:
      return "ALL ANTI-PATTERNS";

    case RISK_LEVEL_INVALID:
    default:
      return "INVALID";
  }

}


std::string PatternTypeToString(const PatternType& pattern_type){

  switch (pattern_type) {
    case PATTERN_TYPE_PHYSICAL_DATABASE_DESIGN:
      return "PHYSICAL_DATABASE_DESIGN ANTI-PATTERN";
    case PATTERN_TYPE_LOGICAL_DATABASE_DESIGN:
      return "LOGICAL_DATABASE_DESIGN ANTI-PATTERN";
    case PATTERN_TYPE_QUERY:
      return "QUERY ANTI-PATTERN";
    case PATTERN_TYPE_APPLICATION:
      return "APPLICATION ANTI-PATTERN";

    case PATTERN_TYPE_INVALID:
    default:
      return "INVALID";
  }

}

std::string GetBooleanString(const bool& status){
  if(status == true){
    return "ENABLED";
  }
  else {
    return "DISABLED";
  }
}

void ValidateRiskLevel(const Configuration &state) {
  if (state.risk_level < RISK_LEVEL_ALL || state.risk_level > RISK_LEVEL_HIGH) {
    printf("INVALID RISK LEVEL :: %d\n", state.risk_level);
    exit(EXIT_FAILURE);
  }
  else {
    printf("> %s :: %s\n", "RISK LEVEL   ",
           RiskLevelToDetailedString(state.risk_level).c_str());
  }
}

void ValidateFileName(const Configuration &state) {
  if (state.file_name.empty() == false) {
    printf("> %s :: %s\n", "SQL FILE NAME",
           state.file_name.c_str());
  }
}


void ValidateColorMode(const Configuration &state) {
    printf("> %s :: %s\n", "COLOR MODE   ",
           GetBooleanString(state.color_mode).c_str());
}

void ValidateVerbose(const Configuration &state) {
    printf("> %s :: %s\n", "VERBOSE MODE ",
         GetBooleanString(state.verbose).c_str());
}

void ValidateDelimiter(const Configuration &state) {
    printf("> %s :: %s\n", "DELIMITER    ",
         state.delimiter.c_str());
}

}  // namespace sqlcheck
