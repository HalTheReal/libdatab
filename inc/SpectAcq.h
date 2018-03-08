#ifndef SPECTRUMACQ_H
#define SPECTRUMACQ_H

#include <Spectrum.h>
#include <stdexcept>    // Eccezioni
#include <DateTime.h>

namespace Spectrometry {

  class SpectAcq : public Spectrum {

    public:
      SpectAcq();
      SpectAcq(const std::vector <int> &hist, const Epoch::DateTime &date, float t);
      SpectAcq(const std::vector <float> &hist, const Epoch::DateTime &date, float t);
      SpectAcq(const std::vector <double> &hist, const Epoch::DateTime &date, float t);

      float getDT() const;
      //Data getDate() const;   // Legacy support?
      Epoch::DateTime getDateTime() const;

    private:
      Epoch::DateTime dataSp;
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
