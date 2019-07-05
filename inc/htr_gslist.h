#ifndef GSLIST
#define GSLIST

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <iomanip>      //  setw, setprecision
#include <exception>
#include <utility>      //  std::pair
#include <algorithm>    //  std::sort
#include <cmath>

#include <htr_datetime.h>
#include <htr_spectrum.h>
#include <htr_tools.h>

namespace Spectrometry {

  class TimedEvent {
    public:
      TimedEvent();
      TimedEvent(long long tstamp, unsigned energy);
      long long time() const;
      unsigned energy() const;
      TimedEvent& setTime(long long time);
      TimedEvent& setEnergy(unsigned energy);

    private:
      long long evTime;
      unsigned evEnergy;
  };

  void shift(TimedEvent &ev, long long offset);
  bool isBefore(const TimedEvent &ev1, const TimedEvent &ev2);
  bool isAfter(const TimedEvent &ev1, const TimedEvent &ev2);
  bool isSync(const TimedEvent &ev1, const TimedEvent &ev2);
  bool isBefore(const TimedEvent &ev1, long long time);
  bool isAfter(const TimedEvent &ev1, long long time);
  bool isSync(const TimedEvent &ev1, long long time);

  using EventList = std::list<TimedEvent>;

  bool isBeforeEvent(const TimedEvent &ev1, const TimedEvent &ev2);
  bool isBeforeTime(const TimedEvent &ev1, long long time);
  EventList timeCut(EventList &evl, long long from, long long to);
  EventList timeCopy(const EventList &evl, long long from, long long to);
  void timeShift(EventList &evl, long long offset);
  void append(EventList &dest, EventList &toApp);
  std::vector <int> toHistogram(const EventList &evl, std::size_t channels);

  EventList readASCII(const char * filename);

//  class GSList {
//
//    public:
//      GSList();
//      GSList(const std::vector <Event> &events, const Epoch::DateTime &start);
//
//      GSList copy(ll_int from, ll_int to) const;
//      GSList& merge(const GSList &gsl);
//
//      std::vector <int> getEventHist() const;
//      Epoch::DateTime getDateTime() const;
//      double getDT() const;
//
//      void writeGSL(const char * nomeFile) const;
//
//    private:
//      EventList evtList;
//      Epoch::DateTime dataGS;
//  };
//
//  Spectrometry::Spectrum toSpectrum();
//  void writeSPE(const GSList &lst, const char * nomeFile);
//  void writeSPT(const GSList &lst, const char * nomeFile);
//  GSList readGSL(const char * nomeFile);
//
//  bool isBefore(Event evt, ll_int val);
//  long tousec(long sec);
}

#endif
