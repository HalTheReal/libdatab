#include "Spectrum.h"

namespace Spectrometry {

  Spectrum::Spectrum()
      : canali(0)
      , mCal(1)
      , qCal(0)
  {}

  Spectrum::Spectrum(const std::vector <int> &hist)
      : Spectrum()
  {
    bin.reserve(hist.size());
    for (int v : hist) {
      bin.push_back(v);
    }
    canali = bin.size();
  }

  Spectrum::Spectrum(const std::vector <float> &hist)
      : Spectrum()
  {
    bin.reserve(hist.size());
    for (float v : hist) {
      bin.push_back(v);
    }
    canali = bin.size();
  }

  Spectrum::Spectrum(const std::vector <double> &hist)
      : Spectrum()
  {
    bin.reserve(hist.size());
    for (double v : hist) {
      bin.push_back(v);
    }
    canali = bin.size();
  }
  
  double Spectrum::getM() const {
    return mCal;
  }

  double Spectrum::getQ() const {
    return qCal;
  }


  Spectrum& Spectrum::calibrateWith(double m, double q) {
    mCal = m;
    qCal = q;
    return *this;
  }

  Spectrum& Spectrum::rebin(double gain) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    return rebin(gain, seed);
  }

  Spectrum& Spectrum::rebin(double gain, unsigned seed) {
    std::default_random_engine generator(seed);
    std::vector <float> newBin(canali, 0);
    for (int i = 0; i < canali; ++i) {
      double E0 = i * mCal + 0.5 * mCal + qCal;
      double Emin = E0 - 0.5 * mCal;
      double Emax = E0 + 0.5 * mCal;
      std::uniform_real_distribution <double> distribution(Emin, Emax);
      for (int j = 0; j < bin[i]; ++j) {
        double rnd = distribution(generator);
        int idx = ceil(rnd / gain);
        if (idx >= 0 && idx < canali) {
          ++newBin[idx];
        }
      }
    }
    mCal = gain;
    qCal = 0;
    bin = newBin;
    return *this;
  }

  double Spectrum::binAt(int b1) const {
    return bin[b1];
  }

  int Spectrum::channels() const {
    return canali;
  }

  int energyToBin(const Spectrum &sp, double en) {
    return (en - sp.getQ()) / sp.getM();
  }

  double binIntegral(const Spectrum &sp, int from, int to) {
    double integral = 0;
    for (int i = from; i <= to; ++i) {
      integral += sp.binAt(i);
    }
    return (integral);
  }

  double counts(const Spectrum &sp, double en) {
    int binIdx = energyToBin(sp, en);
    return (sp.binAt(binIdx));
  }

  double counts(const Spectrum &sp, double en1, double en2) {
    int from = energyToBin(sp, en1);
    int to = energyToBin(sp, en2);
    return (binIntegral(sp, from, to));
  }

}
