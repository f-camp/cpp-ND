#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return id_; }

float Process::CpuUtilization() { return std::stof(cpu_use_); }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return age_; }

bool Process::operator>(Process const& a) const {
    return std::stof(cpu_use_) > std::stof(a.cpu_use_);
}
