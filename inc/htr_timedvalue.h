#ifndef HTR_TIMEDVALUE
#define HTR_TIMEDVALUE

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

#endif
