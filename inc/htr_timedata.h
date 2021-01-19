#ifndef HTR_TIMEDATA
#define HTR_TIMEDATA

#include <vector>
#include <algorithm>    //  std::sort

namespace Epoch {

template <typename T, typename V>
class TimeData {
  public:
    TimeData();
    TimeData(const T& time, const V& data);

    T time() const;
    TimeData & setTime(const T& time);
    V data() const;
    TimeData & setData(const V& data);

  private:
    T timestamp;
    V value;
};

template <typename T, typename V>
TimeData<T, V>::TimeData()
  : timestamp()
  , value()
{}

template <typename T, typename V>
TimeData<T, V>::TimeData(const T& time, const V& data)
  : timestamp(time)
  , value(data)
{}

template <typename T, typename V>
T TimeData<T, V>::time() const {
  return timestamp;
}

template <typename T, typename V>
V TimeData<T, V>::data() const {
  return value;
}

template <typename T1, typename V1, typename T2, typename V2>
bool isBefore(const TimeData<T1, V1> &ev1, const TimeData<T2, V2> &ev2) {
  return ev1.time() < ev2.time();
}

template <typename T1, typename V1, typename T2, typename V2>
bool isAfter(const TimeData<T1, V1> &ev1, const TimeData<T2, V2> &ev2) {
  return ev1.time() > ev2.time();
}

template <typename T1, typename V1, typename T2, typename V2>
bool isSync(const TimeData<T1, V1> &ev1, const TimeData<T2, V2> &ev2) {
  return ev1.time() == ev2.time();
}

template <typename T, typename V, typename S>
bool isBefore(const TimeData<T, V> &ev1, const S &time) {
  return ev1.time() < time;
}

template <typename T, typename V, typename S>
bool isAfter(const TimeData<T, V> &ev1, const S &time) {
  return ev1.time() > time;
}

template <typename T, typename V, typename S>
bool isSync(const TimeData<T, V> &ev1, const S &time) {
  return ev1.time() == time;
}

template <typename T, typename V, typename S>
void shift(TimeData<T, V> &evt, const S &offset) {
  evt.setTime(evt.time() + offset);
}

template <typename T, typename V>
using TDVector = std::vector<TimeData<T, V>>;

template <typename T, typename V>
void timeSort(TDVector<T, V> &lmd) {
  std::sort(lmd.begin(), lmd.end(), isBefore<T, V, T, V>);
}

template <typename T, typename V, typename S>
void timeShift(TDVector<T, V> &evl, const S &time) {
  for (auto &event : evl) {
    shift(event, time);
  }
}

template <typename T, typename V, typename R, typename S>
TDVector<T, V> timeCut(TDVector<T, V> &evl, const R &fromTime, const S &toTime) {
  TDVector<T, V> cut;
  if (fromTime < toTime) {
    auto cutFrom = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    auto cutTo = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    cut = TDVector<T, V>(cutFrom, cutTo);
    evl.erase(cutFrom, cutTo);
  }
  else {
    auto cutFrom = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    auto cutTo = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    cut = TDVector<T, V>(cutFrom, cutTo);
    evl.erase(cutFrom, cutTo);
  }
  return cut;
}

template <typename T, typename V, typename R, typename S>
TDVector<T, V> timeCopy(const TDVector<T, V> &evl, const R &fromTime, const S &toTime) {
  if (fromTime < toTime) {
    auto copyFrom = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    auto copyTo = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    return TDVector<T, V>(copyFrom, copyTo);
  }
  else {
    auto copyFrom = std::lower_bound(evl.begin(), evl.end(), toTime, isBefore<T, V, S>);
    auto copyTo = std::lower_bound(evl.begin(), evl.end(), fromTime, isBefore<T, V, R>);
    return TDVector<T, V>(copyFrom, copyTo);
  }
}

template <typename T, typename V, typename R, typename S>
void timeErase(TDVector<T, V> &evl, const R &fromTime, const S &toTime) {
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

}
#endif
