#include <vector>
#include <string>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

float Processor::Utilization() {
  vector<string> cpu_vstring = LinuxParser::CpuUtilization();

  // equation obtained from
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  float idle = std::stof(cpu_vstring[LinuxParser::kIdle_]) + std::stof(cpu_vstring[LinuxParser::kIOwait_]);
  float nonIdle = std::stof(cpu_vstring[LinuxParser::kUser_]) +
                  std::stof(cpu_vstring[LinuxParser::kNice_]) +
                  std::stof(cpu_vstring[LinuxParser::kSystem_]) +
                  std::stof(cpu_vstring[LinuxParser::kIRQ_]) +
                  std::stof(cpu_vstring[LinuxParser::kSoftIRQ_]) +
                  std::stof(cpu_vstring[LinuxParser::kSteal_]);

  float total = idle + nonIdle;
  return nonIdle / total;
}
