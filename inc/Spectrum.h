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
    private:
      int readSPE(const char * nomeFile);
      int readSPT(const char * nomeFile);
      int readLST(const char * nomeFile);

      float dT;
      int canali;
      double mCal;
      double qCal;
      std::vector <float> bin;
      Data dataSpt;

      void defaultInit();

    public:
      Spectrum();
      Spectrum(const char * nomeFile);

      Spectrum& operator+=(const Spectrum& rhs);
      Spectrum& operator-=(const Spectrum& rhs);

      Spectrum& append(const Spectrum& toApp);

      Spectrum& binCut(int from, int to);
      Spectrum& energyCut(float from, float to);
      Spectrum& calibrate(double m, double q);
      Spectrum& rebin(double gain);
      Spectrum& rebin(double gain, unsigned seed);

      int readFile(const char * nomeFile);

      void writeSPE(const char * nomeFile);
      void writeSPT(const char * nomeFile);

      int energyToBin(double en) const;

      double getCounts(double e1) const;
      double getCounts(double e1, double e2) const;
      double getCps(double e1) const;
      double getCps(double e1, double e2) const;
      double getBinContent(int b1) const;
      double getBinContent(int b1, int b2) const;

      Data getDate();
      float getdT();

      void printContent();
  };

  Spectrum operator+(Spectrum lhs, const Spectrum& rhs);
  Spectrum operator-(Spectrum lhs, const Spectrum& rhs);

}
#endif
