#ifndef HTR_LISTMODE
#define HTR_LISTMODE

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
    evt.setTime(evt.time() + offset);
  }

  bool isBefore(const TimedEvent &ev1, const TimedEvent &ev2);
  bool isAfter(const TimedEvent &ev1, const TimedEvent &ev2);
  bool isSync(const TimedEvent &ev1, const TimedEvent &ev2);

  template <typename T>
  bool isBefore(const TimedEvent &evt, const T &tstamp) {
    return evt.time() < tstamp;
  }

  template <typename T>
  bool isAfter(const TimedEvent &evt, const T &tstamp) {
    return !isBefore(evt, tstamp);
  }

  template <typename T>
  bool isSync(const TimedEvent &evt, const T &tstamp) {
    return evt.time() == tstamp;
  }

  std::ostream& operator << (std::ostream &stream, const TimedEvent &evt);
  std::istream& operator >> (std::istream &stream, TimedEvent &evt);

  using EventList = std::vector<TimedEvent>;

  bool isBeforeEvent(const TimedEvent &ev1, const TimedEvent &ev2);

  void timeSort(EventList &evl);

  template <typename T>
  void timeShift(EventList &evl, const T &tstamp) {
    for (auto &event : evl) {
      shift(event, tstamp);
    }
  }

  template <typename T>
  bool isBeforeTime(const TimedEvent &ev1, const T &tstamp) {
    return isBefore(ev1, tstamp);
  }

  template <typename T1, typename T2>
  EventList timeCut(EventList &evl, const T1 &fromTime, const T2 &toTime) {
    EventList cut;
    if (fromTime < toTime) {
      auto cutFrom = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T1>);
      auto cutTo = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T2>);
      cut = EventList(cutFrom, cutTo);
      evl.erase(cutFrom, cutTo);
    }
    else {
      auto cutFrom = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T2>);
      auto cutTo = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T1>);
      cut = EventList(cutFrom, cutTo);
      evl.erase(cutFrom, cutTo);
    }
    return cut;
  }

  template <typename T1, typename T2>
  EventList timeCopy(const EventList &evl, const T1 &fromTime, const T2 &toTime) {
    if (fromTime < toTime) {
      auto copyFrom = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T1>);
      auto copyTo = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T2>);
      return EventList(copyFrom, copyTo);
    }
    else {
      auto copyFrom = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T2>);
      auto copyTo = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T1>);
      return EventList(copyFrom, copyTo);
    }
  }

  template <typename T1, typename T2>
  void timeErase(EventList &evl, const T1 &fromTime, const T2 &toTime) {
    if (fromTime < toTime) {
      auto eraseFrom = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T1>);
      auto eraseTo = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T2>);
      evl.erase(eraseFrom, eraseTo);
    }
    else {
      auto eraseFrom = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T2>);
      auto eraseTo = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T1>);
      evl.erase(eraseFrom, eraseTo);
    }
  }

  void append(EventList &dest, const EventList &toApp);
  std::vector <int> toHistogram(const EventList &evl, std::size_t channels);
  EventList readASCII(const char * filename);
  EventList readASCII(const std::string &filename);

  class GSList {

    public:
      GSList();
      GSList(EventList evl, Epoch::DateTime start);

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
      EventList eventList;
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
