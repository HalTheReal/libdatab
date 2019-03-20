#ifndef MYTOOLS_H
#define MYTOOLS_H

#include <math.h>
#include <string>
#include <vector>
#include <iostream>     // std::cout

namespace tls {
  std::vector <std::string> split(const std::string &toSplit, char c);
  std::vector <std::string> splitWhite(const std::string &toSplit);
}

#endif
