#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <math.h>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  float value;
  float total_used_memory = 0;
  float mem_total = 0;
  float mem_free = 0;
  float buffers = 0;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "MemTotal:") {
        //convert to GB
        mem_total = value;
    }
    if (key == "MemFree:") {
        mem_free = value;
    }
    if (key == "Buffers:") {
        buffers = value;
    }
  }
  total_used_memory = mem_total - mem_free + buffers;
  return total_used_memory/mem_total;
}

long LinuxParser::UpTime() {
  string line;
  long up_time = 0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> up_time;
  }
  return up_time;
}

//// TODO: Read and return the number of jiffies for the system
//long LinuxParser::Jiffies() { return 0; }

//// TODO: Read and return the number of active jiffies for a PID
//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

//// TODO: Read and return the number of active jiffies for the system
//long LinuxParser::ActiveJiffies() { return 0; }

//// TODO: Read and return the number of idle jiffies for the system
//long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() {
  string key;
  string line;
  std::ifstream file(kProcDirectory + kStatFilename);
  while(std::getline(file, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "cpu"){
        std::istream_iterator<string> begin(linestream), end;
        vector<string> line_content(begin, end);
        return line_content;
    }
  }
  return {};
}

int LinuxParser::TotalProcesses() {
  string key;
  string line;
  int processes{0};
  std::ifstream file(kProcDirectory + kStatFilename);
  while(std::getline(file, line)) {
    std::istringstream linestream(line);
    linestream >> key >> processes;
    if (key == "processes"){
        return processes;
    }
  }
  return processes;
}

int LinuxParser::RunningProcesses() {
  string key;
  string line;
  int procs_running{0};
  std::ifstream file(kProcDirectory + kStatFilename);
  while(std::getline(file, line)) {
    std::istringstream linestream(line);
    linestream >> key >> procs_running;
    if (key == "procs_running"){
        return procs_running;
    }
  }
  return procs_running;
}

string LinuxParser::Command(int pid) {
  std::string line;
  std::ifstream file(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(file) {
    std::getline(file, line);
  }
  return line;
}

string LinuxParser::Ram(int pid) {
  string key;
  string line;
  string ram;
  int ram_used{0};
  std::ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  while(std::getline(file, line)) {
    std::istringstream linestream(line);
    linestream >> key >> ram;
    if (key == "VmSize:"){
        //Convert to MB
        ram_used = static_cast<int>(stoi(ram) / 1000);
    }
  }
  return to_string(ram_used);
}

string LinuxParser::Uid(int pid) {
  string key;
  string line;
  int Uid{0};
  std::ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  while(std::getline(file, line)) {
    std::istringstream linestream(line);
    linestream >> key >> Uid;
    if (key == "Uid:"){
        return to_string(Uid);
    }
  }
  return to_string(Uid);
}

string LinuxParser::User(int pid) {
  string key, dummy;
  string line, user;
  string uid;
  std::ifstream file(kPasswordPath);
  while(std::getline(file, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> user >> dummy >> uid;
    //std::cout << user << ":" << uid << std::endl;
    if (uid == LinuxParser::Uid(pid)){
        return user;
    }
  }
  return user;
}

string LinuxParser::CpuUtilization(int pid) {
  string key;
  string line;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  while(std::getline(file, line)) {
    std::istringstream linestream(line);
    std::istream_iterator<string> begin(linestream), end;
    vector<string> line_content(begin, end);

    //Total cpu usage
    //https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    float total_time = stof(line_content[13]) +
                       stof(line_content[14]) +
                       stof(line_content[15]) +
                       stof(line_content[16]); // utime + stime
    float seconds = LinuxParser::UpTime() -
                    (stof(line_content[21]) / sysconf(_SC_CLK_TCK));

    return to_string((total_time / sysconf(_SC_CLK_TCK)) / seconds);
  }
  return {};
}

long int LinuxParser::UpTime(int pid) {
  string key;
  string line;
  std::ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  while(std::getline(file, line)) {
    std::istringstream linestream(line);
    std::istream_iterator<string> begin(linestream), end;
    vector<string> line_content(begin, end);
    //Convert to seconds
    return stol(line_content[21])/sysconf(_SC_CLK_TCK);
  }
  return 0;
}


