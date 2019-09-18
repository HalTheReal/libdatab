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
#include <htr_datetime.h>
#include <htr_stats.h>

namespace Spectrometry {

  class Spectrum {

    public:
      Spectrum();

      template <typename T>
      Spectrum(const std::vector <T> &hist);

      template <typename T>
      Spectrum(const std::vector <T> &hist, const Epoch::DateTime &start, double tm);

      double binAt(int b1) const;
      std::size_t channels() const;
      double getM() const;
      double getQ() const;
      Epoch::DateTime getDateTime() const;
      double getDT() const;

      Spectrum& calibrateWith(double m, double q);
      Spectrum& rebin(double gain);
      Spectrum& rebin(double gain, unsigned seed);

    private:
      std::vector <double> bin;
      std::size_t canali;
      double mCal;
      double qCal;
      Epoch::DateTime startTime;
      double dT;
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
    Spectrum::Spectrum(const std::vector <T> &hist, const Epoch::DateTime &start, double tm)
      : Spectrum(hist)
  {
    startTime = start;
    dT = tm;
  }


  int energyToBin(double m, double q, double en);
  int energyToBin(const Spectrum &sp, double en);

  double binToEnergy(double m, double q, int bin);
  double binToEnergy(const Spectrum &sp, int bin);

  Epoch::DateTime getCentroid(const Spectrum &sp);

  Spectrum sum(const Spectrum &sp1, const Spectrum &sp2);
  Spectrum subtract(const Spectrum &sp1, const Spectrum &sp2);

  Spectrum readSPE(const char * nomeFile);
  Spectrum readSPT(const char * nomeFile);
  Spectrum readLST(const char * nomeFile);
  Spectrum readTXT(const char * nomeFile);

  Spectrum readSPE(const std::string &nomeFile);
  Spectrum readSPT(const std::string &nomeFile);
  Spectrum readLST(const std::string &nomeFile);
  Spectrum readTXT(const std::string &nomeFile);

  void writeSPE(const Spectrum &sp, const char * nomeFile);
  void writeSPT(const Spectrum &sp, const char * nomeFile);
  void writeTXT(const Spectrum &sp, const char * nomeFile);

  void writeSPE(const Spectrum &sp, const std::string &str);
  void writeSPT(const Spectrum &sp, const std::string &str);
  void writeTXT(const Spectrum &sp, const std::string &str);

  Spectrum medianFilter(const Spectrum &sp, unsigned width);
  Spectrum movingAvg(const Spectrum &sp, unsigned width);

}
#endif
