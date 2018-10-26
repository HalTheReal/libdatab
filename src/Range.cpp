#include <Range.h>

template <>
Range<Epoch::Time>& shift<Epoch::Time>(Range<Epoch::Time> &rng, int sec) {
  rng.setUpper(rng.upper().addSec(sec));
  rng.setLower(rng.lower().addSec(sec));
  return rng;
}

template <>
Range<Epoch::Date>& shift<Epoch::Date>(Range<Epoch::Date> &rng, int day) {
  rng.setUpper(rng.upper().addDay(day));
  rng.setLower(rng.lower().addDay(day));
  return rng;
}

template <>
Range<Epoch::DateTime>& shift<Epoch::DateTime>(Range<Epoch::DateTime> &rng, int sec) {
  rng.setUpper(rng.upper().addSec(sec));
  rng.setLower(rng.lower().addSec(sec));
  return rng;
}
