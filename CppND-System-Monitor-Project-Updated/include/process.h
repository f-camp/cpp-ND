#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int id): id_(id),
                   user_(LinuxParser::User(id)),
                   command_(LinuxParser::Command(id)),
                   ram_(LinuxParser::Ram(id)),
                   cpu_use_(LinuxParser::CpuUtilization(id)),
                   age_(LinuxParser::UpTime(id)){}

  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator>(Process const& a) const;

 private:
  int id_;
  std::string user_, command_, ram_, cpu_use_;
  long int age_;
};

#endif
