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

      void writeSPE(const char * nomeFile);
      void writeSPT(const char * nomeFile);

      int energyToBin(double en) const;

      double getCounts(double e1) const;
      double getCounts(double e1, double e2) const;
      double getCps(double e1) const;
      double getCps(double e1, double e2) const;
      double binAt(int b1) const;
      double binAt(int b1, int b2) const;

      Data getDate();
      float getdT();

    private:

      float dT;
      int canali;
      double mCal;
      double qCal;
      std::vector <float> bin;
      Data dataSpt;

      void defaultInit();
  };

  Spectrum operator+(Spectrum lhs, const Spectrum& rhs);
  Spectrum operator-(Spectrum lhs, const Spectrum& rhs);

}
#endif
