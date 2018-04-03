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
#include <stdexcept>    // Eccezioni
#include <tools.h>

namespace Spectrometry {

  class Spectrum {

    public:
      Spectrum();
      Spectrum(const std::vector <int> &hist);
      Spectrum(const std::vector <float> &hist);
      Spectrum(const std::vector <double> &hist);

      Spectrum& calibrateWith(double m, double q);
      Spectrum& rebin(double gain);
      Spectrum& rebin(double gain, unsigned seed);

      double getM() const;
      double getQ() const;
      int channels() const;

      double binAt(int b1) const;

    private:
      std::vector <float> bin;
      int canali;
      double mCal;
      double qCal;
  };

  int energyToBin(const Spectrum &sp, double en);
  double binIntegral(const Spectrum &sp, int from, int to);
  double counts(const Spectrum &sp, double en);
  double counts(const Spectrum &sp, double en1, double en2);

}
#endif
