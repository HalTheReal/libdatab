#ifndef SPECTRUMACQ_H
#define SPECTRUMACQ_H

#include <Spectrum.h>
#include <Data.h>

namespace physics {

  class SpectAcq : public Spectrum {

    public:
      SpectAcq();
      SpectAcq(const std::vector <int> &hist, const Data &date, float t);
      SpectAcq(const std::vector <float> &hist, const Data &date, float t);
      SpectAcq(const std::vector <double> &hist, const Data &date, float t);

      float getDT() const;
      Data getDate() const;

    private:
      Data dataSp;
      float dT;
  };

  SpectAcq readSPE(const char * nomeFile);
  SpectAcq readSPT(const char * nomeFile);
  SpectAcq readLST(const char * nomeFile);

  double cps(const SpectAcq &sp, double en1);
  double cps(const SpectAcq &sp, double en1, double en2);

  void writeSPE(const SpectAcq &sp, const char * nomeFile);
  void writeSPT(const SpectAcq &sp, const char * nomeFile);

}
#endif
