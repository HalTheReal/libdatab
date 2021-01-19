#ifndef HTR_GSLIST
#define HTR_GSLIST

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>      //  setw, setprecision
#include <exception>
#include <chrono>       //  std::duration
#include <cmath>

#include <htr_listmode.h>
#include <htr_datetime.h>
#include <htr_spectrum.h>
#include <htr_tools.h>

namespace Spectrometry {

  std::vector <int> toHistogram(const ListMode<std::chrono::nanoseconds, unsigned> &evl, std::size_t channels);

  class GSList {

    public:
      GSList();
      GSList(ListMode<std::chrono::nanoseconds, unsigned> evl, Epoch::DateTime start);

      bool isEmpty() const;

      GSList copy(int fromSec, int toSec) const;
      GSList copy(const Epoch::DateTime from, int toSec) const;
      GSList copy(int fromSec, const Epoch::DateTime &to) const;
      GSList copy(const Epoch::DateTime from, const Epoch::DateTime &to) const;

      void erase(int fromSec, int toSec);
      void erase(const Epoch::DateTime from, int toSec);
      void erase(int fromSec, const Epoch::DateTime &to);
      void erase(const Epoch::DateTime from, const Epoch::DateTime &to);

      GSList cut(int fromSec, int toSec);
      GSList cut(const Epoch::DateTime from, int toSec);
      GSList cut(int fromSec, const Epoch::DateTime &to);
      GSList cut(const Epoch::DateTime from, const Epoch::DateTime &to);

      GSList& merge(GSList &gsl);
      GSList& append(GSList &gsl);

      std::vector <int> toHistogram() const;
      Epoch::DateTime getDateTime() const;
      void setDateTime(const Epoch::DateTime &dt);

      std::chrono::seconds getLT() const;
      std::chrono::milliseconds getLTMilliseconds() const;
      std::chrono::nanoseconds getLTNanoseconds() const;

      void writeGSL(const char * nomeFile) const;
      void writeGSL(const std::string &nomeFile) const;

    private:
      ListMode<std::chrono::nanoseconds, unsigned> eventList;
      Epoch::DateTime dataGS;
  };

  Epoch::DateTime getCentroid(const GSList &gsl);

  GSList readGSL(const char * nomeFile);
  GSList readGSL(const std::string &nomeFile);

  Spectrum toSpectrum(const GSList &gsl);

  void writeSPE(const GSList &lst, const char * nomeFile);
  void writeSPE(const GSList &lst, const std::string &nomeFile);
  void writeSPT(const GSList &lst, const char * nomeFile);
  void writeSPT(const GSList &lst, const std::string &nomeFile);
}

#endif
