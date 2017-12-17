#include "DateTime.h"

namespace Chrono {

  DateTime::DateTime()
    : dateNow()
    , timeNow()
  {}

  DateTime::DateTime(int dy, int mn, int yr, int hr, int mi, int sc)
    : dateNow(dy, mn, yr)
    , timeNow(hr, mi, sc)
  {}

  DateTime::DateTime(const Time &tm, const Date &dt)
    : DateTime()
  {
    dateNow = dt;
    timeNow = tm;
  }

  std::string DateTime::toString() const {
    return (Chrono::toString(dateNow) + ' ' + Chrono::toString(timeNow));
  }

}
