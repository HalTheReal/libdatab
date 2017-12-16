#include "Date.h"

namespace Chrono {

  Date::Date()
    : dDay(1)
    , dMonth(1)
    , dYear(1970)
  {}

  Date::Date(int dy, int mt, int yr)
    : dDay(dy)
    , dMonth(mt)
    , dYear(yr)
  {
    if (!isValid()) {
      dDay = 1;
      dMonth = 1;
      dYear = 1970;
      throw std::runtime_error("Invalid Date");
    }
  }

  bool Date::isValid() const {
    if (dYear < 0 || dMonth < 1 || dDay < 1) {
      return false;
    }
    if (dMonth > 12 || dDay > daysInMonth(*this)) {
      return false;
    }
    return true;
  }

  int Date::day() const {
    return dDay;
  }

  int Date::month() const {
    return dMonth;
  }

  int Date::year() const {
    return dYear;
  }

  bool isLeap(const Date &dt) {
    if (dt.year() % 4 != 0) {
      return false;
    }
    else if (dt.year() % 100 != 0) {
      return true;
    }
    else if (dt.year() % 400 != 0) {
      return false;
    }
    else {
      return true;
    }
  }

  int daysInMonth(const Date &dt) {
    int days = 0;
    if (dt.month() == 2) {
      return isLeap(dt) ? 29 : 28;
    }
    if (dt.month() <= 7) {
      days = dt.month() % 2 == 0 ? 30 : 31;
    }
    else if (dt.month() > 7) {
      days = dt.month() % 2 == 0 ? 31 : 30;
    }
    return days;
  }

  int daysInYear(const Date &dt) {
    return (isLeap(dt) ? 366 : 365);
  }

  std::string toString(const Date &dt, char sep) {
    std::stringstream ss;
    ss << dt.day() << sep;
    ss << dt.month() << sep;
    ss << dt.year();
    return ss.str();
  }
}
