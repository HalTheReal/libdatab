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
#include <DateTime.h>
#include <Stats.h>

namespace Spectrometry {

  class Spectrum {

    public:
      Spectrum();

      template <typename T>
      Spectrum(const std::vector <T> &hist);

      template <typename T>
      Spectrum(const std::vector <T> &hist, const Epoch::DateTime &start, float tm);

      double binAt(int b1) const;
      int channels() const;
      double getM() const;
      double getQ() const;
      Epoch::DateTime getDateTime() const;
      double getDT() const;

      Spectrum& calibrateWith(double m, double q);
      Spectrum& rebin(double gain);
      Spectrum& rebin(double gain, unsigned seed);

    private:
      std::vector <float> bin;
      int canali;
      double mCal;
      double qCal;
      Epoch::DateTime startTime;
      float dT;
  };

  template <typename T>
  Spectrum::Spectrum(const std::vector <T> &hist)
      : Spectrum()
  {
    bin.reserve(hist.size());
    for(auto val : hist) {
      bin.push_back(val);
    }
    canali = bin.size();
  }

  template <typename T>
    Spectrum::Spectrum(const std::vector <T> &hist, const Epoch::DateTime &start, float tm)
      : Spectrum(hist)
  {
    startTime = start;
    dT = tm;
  }

  int energyToBin(double m, double q, double en);
  int energyToBin(const Spectrum &sp, double en);

  double binToEnergy(double m, double q, int bin);
  double binToEnergy(const Spectrum &sp, int bin);

  Spectrum sum(const Spectrum &sp1, const Spectrum &sp2);
  Spectrum subtract(const Spectrum &sp1, const Spectrum &sp2);

  Spectrum readSPE(const char * nomeFile);
  Spectrum readSPT(const char * nomeFile);
  Spectrum readLST(const char * nomeFile);
  Spectrum readTXT(const char * nomeFile);

  void writeSPE(const Spectrum &sp, const char * nomeFile);
  void writeSPT(const Spectrum &sp, const char * nomeFile);
  void writeTXT(const Spectrum &sp, const char * nomeFile);

  Spectrum medianFilter(const Spectrum &sp, unsigned width);
  Spectrum movingAvg(const Spectrum &sp, unsigned width);

}
#endif
