#ifndef MY_RANGE_H
#define MY_RANGE_H

#include <ostream>
#include <sstream>
#include <utility>    // std::swap

template <typename T>
class Range {
  public:
    Range();
    Range(T leftEdge, T rightEdge);
    T lower() const;
    T upper() const;
    Range& setLower(T set);
    Range& setUpper(T set);
  private:
    // Left <= right
    T left;
    T right;
};

template <typename T>
Range<T>::Range()
  : left()
  , right()
{}

template <typename T>
Range<T>::Range(T leftEdge, T rightEdge)
  : left(leftEdge)
  , right(rightEdge)
{
  if(left > right) {
    std::swap(left, right);
  }
}

template <typename T>
T Range<T>::lower() const {
  return left;
}

template <typename T>
T Range<T>::upper() const {
  return right;
}

template <typename T>
Range<T>& Range<T>::setLower(T set) {
  left = set;
  if (left > right) {
    right = left;
  }
  return *this;
}

template <typename T>
Range<T>& Range<T>::setUpper(T set) {
  right = set;
  if (left > right) {
    left = right;
  }
  return *this;
}

template <typename T>
T width(const Range<T> &rng) {
  return rng.upper() - rng.lower();
}

template <typename T, typename R>
Range<T>& inflate(Range<T> &rng, R inf) {
  rng.setUpper(rng.upper() + inf);
  rng.setLower(rng.lower() - inf);
  return rng;
}

template <typename T, typename R>
Range<T>& shift(Range<T> &rng, R inf) {
  rng.setUpper(rng.upper() + inf);
  rng.setLower(rng.lower() + inf);
  return rng;
}

template <typename T>
bool isWithin(Range<T> &rng, T &el) {
  if (el > rng.upper() || el < rng.lower()) {
    return false;
  }
  return true;
}

template <typename T>
std::string to_string(const Range<T> &rng, char sep = '-') {
  std::stringstream ss;
  ss << rng.lower() << sep << rng.upper();
  return ss.str();
}

template <typename T>
std::ostream& operator << (std::ostream &stream, const Range<T> &rng) {
  stream << to_string(rng);
  return stream;
}

#endif
