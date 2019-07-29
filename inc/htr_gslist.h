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
#include <chrono>       //  std::duration
#include <cmath>

#include <htr_datetime.h>
#include <htr_spectrum.h>
#include <htr_tools.h>

namespace Spectrometry {

  class TimedEvent {
    public:
      TimedEvent();
      TimedEvent(int64_t tstamp, unsigned energy);

      template <typename T>
      TimedEvent(const T &tstamp, unsigned energy)
        : evTime(std::chrono::duration_cast<std::chrono::nanoseconds> (tstamp))
        , evEnergy(energy)
      {}

      std::chrono::nanoseconds time() const;
      unsigned energy() const;

      template <typename T>
      TimedEvent& setTime(const T &tstamp) {
        evTime = std::chrono::duration_cast<std::chrono::nanoseconds> (tstamp);
        return *this;
      }

      TimedEvent& setEnergy(unsigned energy);

    private:
      std::chrono::nanoseconds evTime;
      unsigned evEnergy;
  };

  template <typename T>
  void shift(TimedEvent &evt, const T &offset) {
    evt.setTime(evt.time() + std::chrono::duration_cast<std::chrono::nanoseconds> (offset));
  }

  bool isBefore(const TimedEvent &ev1, const TimedEvent &ev2);
  bool isAfter(const TimedEvent &ev1, const TimedEvent &ev2);
  bool isSync(const TimedEvent &ev1, const TimedEvent &ev2);

  template <typename T>
  bool isBefore(const TimedEvent &evt, const T &tstamp) {
    return evt.time() < std::chrono::duration_cast<std::chrono::nanoseconds> (tstamp);
  }

  template <typename T>
  bool isAfter(const TimedEvent &evt, const T &tstamp) {
    return !isBefore(evt, tstamp);
  }

  template <typename T>
  bool isSync(const TimedEvent &evt, const T &tstamp) {
    return evt.time() == std::chrono::duration_cast<std::chrono::nanoseconds> (tstamp);
  }

  std::ostream& operator << (std::ostream &stream, const TimedEvent &evt);
  std::istream& operator >> (std::istream &stream, TimedEvent &evt);

  using EventList = std::list<TimedEvent>;

  void timeSort(EventList &evl);
//
//  bool isBeforeEvent(const TimedEvent &ev1, const TimedEvent &ev2);
//  bool isBeforeTime(const TimedEvent &ev1, long long time);
//  EventList timeCut(EventList &evl, long long from, long long to);
//  EventList timeCopy(const EventList &evl, long long from, long long to);
//  void timeShift(EventList &evl, long long offset);
//  void append(EventList &dest, EventList &toApp);
//  std::vector <int> toHistogram(const EventList &evl, std::size_t channels);
//
//  EventList readASCII(const char * filename);
//
//  class GSList {
//
//    public:
//      GSList();
//      GSList(const EventList &evl, const Epoch::DateTime &start);
//
//      GSList copy(int fromSec, int toSec) const;
//      GSList& erase(const GSList &gsl, int fromSec, int toSec);
//      GSList& merge(const GSList &gsl);
//
//      std::vector <int> getHistogram() const;
//      Epoch::DateTime getDateTime() const;
//      double getDT() const;
//
//      void writeGSL(const char * nomeFile) const;
//
//    private:
//      EventList eventList;    // 1E-9 [s], [keV]
//      Epoch::DateTime dataGS;
//      
//      long tousec(int sec) const;
//  };
//
//  Spectrometry::Spectrum toSpectrum();
//  void writeSPE(const GSList &lst, const char * nomeFile);
//  void writeSPT(const GSList &lst, const char * nomeFile);
//  GSList readGSL(const char * nomeFile);
}

#endif
