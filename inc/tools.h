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

namespace sta {
  template <typename T>
  double mean(const std::vector <T> &data) {
    double tot = 0;
    for (size_t i = 0; i < data.size(); ++i) {
      tot += data[i];
    }
    return tot / data.size();
  }

  template <typename T>
  double mean(const std::vector <T> &data, T nodata) {
    double tot = 0;
    int count = 0;
    for (size_t i = 0; i < data.size(); ++i) {
      if (data[i] != nodata) {
        tot += data[i];
        ++count;
      }
    }
    return count == 0 ? nodata : tot / count;
  }

  template <typename T>
  double stDev(const std::vector <T> &data) {
    double media = mean(data);
    double tot = 0;
    for (size_t i = 0; i < data.size(); ++i) {
      tot += pow(data[i] - media, 2);
    }
    return sqrt(tot / data.size());
  }

  template <typename T>
  double stDev(const std::vector <T> &data, T nodata) {
    double media = mean(data, nodata);
    double tot = 0;
    int count = 0;
    for (size_t i = 0; i < data.size(); ++i) {
      if (data[i] != nodata) {
        tot += pow(data[i] - media, 2);
        ++count;
      }
    }
    return (count == 0 ? nodata : sqrt(tot / count));
  }
  template <typename T>
  T sum(const std::vector <T> &data) {
    T tot = 0;
    for (T val : data) {
      tot += val;
    }
    return tot;
  }
  template <typename T>
  T sum(const std::vector <T> &data, T nodata) {
    T tot = 0;
    for (T val : data) {
      if (val != nodata) {
        tot += val;
      }
    }
    return tot;
  }
}
#endif
