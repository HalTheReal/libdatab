#ifndef GSLIST
#define GSLIST

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>      //  setw, setprecision
#include <exception>
#include <utility>      //  std::pair
#include <algorithm>    //  std::sort
#include <cmath>

#include <htr_datetime.h>
#include <htr_spectrum.h>
#include <htr_tools.h>

namespace Spectrometry {

  using ll_int = long long;
  using Event = std::pair<ll_int, int>;
  using VecEvent = std::vector <std::pair <ll_int, int>>;

  class EventList {

    public:
      EventList();
      EventList(const std::vector <Event> &events);

      EventList copy(ll_int from, ll_int to) const;
      EventList& merge(const EventList &gsl);
      EventList& shift(ll_int start);

      ll_int first() const;
      ll_int last() const;
      std::size_t entries() const;

      std::vector <int> getHist(std::size_t channels) const;

    private:
      VecEvent evtList;
  };

  class GSList {

    public:
      GSList();
      GSList(const std::vector <Event> &events, const Epoch::DateTime &start);

      std::vector <int> getEventHist() const;
      Epoch::DateTime getDateTime() const;
      double getDT() const;

      GSList& erase(long from, long to);
      GSList& merge(const GSList &gsl);
      GSList copy(long from, long to) const;

      void writeGSL(const char * nomeFile) const;

    private:
      std::vector <Event> evtList;  // time [s / 10^9], energy [keV]
      Epoch::DateTime dataGS;
  };

  Spectrometry::Spectrum toSpectrum();
  void writeSPE(const GSList &lst, const char * nomeFile);
  void writeSPT(const GSList &lst, const char * nomeFile);
  GSList readGSL(const char * nomeFile);

  bool isBefore(Event evt, ll_int val);
  long tousec(long sec);
}
#endif
