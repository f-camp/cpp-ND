#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <iostream>

#include <linux_parser.h>


int main(){
   std::vector<int> pids = LinuxParser::Pids();
   for ( auto pid: pids){
     std::cout << pid << ",";
   }
}

