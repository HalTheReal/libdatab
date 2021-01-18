#ifndef HTR_LISTMODE
#define HTR_LISTMODE

#include <vector>
#include <algorithm>    //  std::sort

template <typename T, typename V>
class TimedValue {
  public:
    TimedValue();
    TimedValue(const T& tstamp, const V& val);

    T time() const;
    TimedValue & setTime(const T& tstamp);
    V value() const;
    TimedValue & setValue(const V& val);

  private:
    T timestamp;
    V data;
};

template <typename T, typename V>
TimedValue<T, V>::TimedValue()
  : timestamp()
  , data()
{}

template <typename T, typename V>
TimedValue<T, V>::TimedValue(const T& tstamp, const V& val)
  : timestamp(tstamp)
  , data(val)
{}

template <typename T, typename V>
T TimedValue<T, V>::time() const {
  return timestamp;
}

template <typename T, typename V>
V TimedValue<T, V>::value() const {
  return data;
}

template <typename T1, typename V1, typename T2, typename V2>
bool isBefore(const TimedValue<T1, V1> &ev1, const TimedValue<T2, V2> &ev2) {
  return ev1.time() < ev2.time();
}

template <typename T1, typename V1, typename T2, typename V2>
bool isAfter(const TimedValue<T1, V1> &ev1, const TimedValue<T2, V2> &ev2) {
  return ev1.time() > ev2.time();
}

template <typename T1, typename V1, typename T2, typename V2>
bool isSync(const TimedValue<T1, V1> &ev1, const TimedValue<T2, V2> &ev2) {
  return ev1.time() == ev2.time();
}

template <typename T, typename V, typename S>
bool isBefore(const TimedValue<T, V> &ev1, const S &tstamp) {
  return ev1.time() < tstamp;
}

template <typename T, typename V, typename S>
bool isAfter(const TimedValue<T, V> &ev1, const S &tstamp) {
  return ev1.time() > tstamp;
}

template <typename T, typename V, typename S>
bool isSync(const TimedValue<T, V> &ev1, const S &tstamp) {
  return ev1.time() == tstamp;
}

template <typename T, typename V, typename S>
void shift(TimedValue<T, V> &evt, const S &offset) {
  evt.setTime(evt.time() + offset);
}

template <typename T, typename V>
using ListMode = std::vector<TimedValue<T, V>>;

template <typename T, typename V>
void timeSort(ListMode<T, V> &lmd) {
  std::sort(lmd.begin(), lmd.end(), isBefore<T, V, T, V>);
}

template <typename T, typename V, typename S>
void timeShift(ListMode<T, V> &evl, const S &tstamp) {
  for (auto &event : evl) {
    shift(event, tstamp);
  }
}

template <typename T, typename V, typename R, typename S>
ListMode<T, V> timeCut(ListMode<T, V> &evl, const R &fromTime, const S &toTime) {
  ListMode<T, V> cut;
  if (fromTime < toTime) {
    auto cutFrom = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    auto cutTo = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    cut = ListMode<T, V>(cutFrom, cutTo);
    evl.erase(cutFrom, cutTo);
  }
  else {
    auto cutFrom = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    auto cutTo = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    cut = ListMode<T, V>(cutFrom, cutTo);
    evl.erase(cutFrom, cutTo);
  }
  return cut;
}

template <typename T, typename V, typename R, typename S>
ListMode<T, V> timeCopy(const ListMode<T, V> &evl, const R &fromTime, const S &toTime) {
  if (fromTime < toTime) {
    auto copyFrom = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    auto copyTo = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    return ListMode<T, V>(copyFrom, copyTo);
  }
  else {
    auto copyFrom = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    auto copyTo = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    return ListMode<T, V>(copyFrom, copyTo);
  }
}

template <typename T, typename V, typename R, typename S>
void timeErase(ListMode<T, V> &evl, const R &fromTime, const S &toTime) {
  if (fromTime < toTime) {
    auto eraseFrom = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    auto eraseTo = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    evl.erase(eraseFrom, eraseTo);
  }
  else {
    auto eraseFrom = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    auto eraseTo = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    evl.erase(eraseFrom, eraseTo);
  }
}

#endif
