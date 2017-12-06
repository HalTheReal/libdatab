#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>      //setw, setprecision
#include <random>
#include <chrono>
#include <Data.h>
#include <tools.h>

namespace physics {

  class Spectrum {

    public:
      Spectrum();
      Spectrum(const std::vector <int> &hist);
      Spectrum(const std::vector <float> &hist);
      Spectrum(const std::vector <double> &hist);

      Spectrum& operator+=(const Spectrum& rhs);
      Spectrum& operator-=(const Spectrum& rhs);

      Spectrum& calibrate(double m, double q);
      Spectrum& rebin(double gain);
      Spectrum& rebin(double gain, unsigned seed);

      int energyToBin(double en) const;

      double binAt(int b1) const;
      double binAt(int b1, int b2) const;

    private:
      std::vector <float> bin;
      int canali;
      double mCal;
      double qCal;
  };

  Spectrum operator+(Spectrum lhs, const Spectrum& rhs);
  Spectrum operator-(Spectrum lhs, const Spectrum& rhs);

}
#endif
