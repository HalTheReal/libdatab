#include "htr_date.h"

namespace Epoch {

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

  Date& Date::addDay(int toAdd) {
    while (toAdd < 0) {
      ++toAdd;
      --dDay;
      if (dDay == 0) {
        addMonth(-1);
        dDay += daysInMonth(*this);
      }
    }
    while (toAdd > 0) {
      --toAdd;
      ++dDay;
      if (dDay > daysInMonth(*this)) {
        addMonth(1);
        dDay = 1;
      }
    }
    return *this;
  }

  Date& Date::addMonth(int toAdd) {
    while (toAdd < 0) {
      ++toAdd;
      --dMonth;
      if (dMonth == 0) {
        dMonth = 12;
        addYear(-1);
      }
    }
    while (toAdd > 0) {
      --toAdd;
      ++dMonth;
      if (dMonth == 13) {
        dMonth = 1;
        addYear(1);
      }
    }
    return *this; 
  }

  Date& Date::addYear(int toAdd) {
    dYear += toAdd;
    // Succede solo se aggiungo 1 anno al 29/2
    if (dMonth == 2 && dDay > daysInMonth(*this)) {
      addDay(1);
    }
    // Eccezione se year < 0 ?
    return *this;
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

  bool operator == (const Date &dt1, const Date &dt2) {
    if(dt1.day() == dt2.day() && dt1.month() == dt2.month() && dt1.year() == dt2.year()) {
      return true;
    }
    return false;
  }

  bool operator != (const Date &dt1, const Date &dt2) {
    return !(dt1 == dt2);
  }

  bool operator < (const Date &dt1, const Date &dt2) {
    if (dt1.year() < dt2.year()) {
      return true;
    }
    if (dt1.year() > dt2.year()) {
      return false;
    }
    if (dt1.month() < dt2.month()) {
      return true;
    }
    if (dt1.month() > dt2.month()) {
      return false;
    }
    if (dt1.day() < dt2.day()) {
      return true;
    }
    if (dt1.day() > dt2.day()) {
      return false;
    }
    return false;
  }

  bool operator <= (const Date &dt1, const Date &dt2) {
    return (dt1 < dt2 || dt1 == dt2);
  }

  bool operator > (const Date &dt1, const Date &dt2) {
    return !(dt1 <= dt2);
  }

  bool operator >= (const Date &dt1, const Date &dt2) {
    return !(dt1 < dt2);
  }

  std::ostream& operator << (std::ostream &stream, const Date &dt) {
    stream << to_string(dt);
    return stream;
  }

  std::istream& operator >> (std::istream &stream, Date &dt) {
    char s1, s2;
    int day, month, year;
    stream >> day >> s1 >> month >> s2 >> year;
    if (!stream) {
      return stream;
    }
    if (s1 != s2) {
      stream.setstate(std::ios_base::failbit);
      return stream;
    }
    try {
      dt = Date(day, month, year);
    }
    catch (const std::exception &e) {
      stream.setstate(std::ios_base::failbit);
    }
    return stream;
  }

  std::string to_string(const Date &dt) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << dt.year();
    ss << '-';
    ss << std::setfill('0') << std::setw(2) << dt.month();
    ss << '-';
    ss << std::setfill('0') << std::setw(2) << dt.day();
    return ss.str();
  }

  Date strToDate(const std::string &str) {
    std::stringstream ss(str);
    Date dt;
    if (ss >> dt) {
      return dt;
    }
    throw std::invalid_argument("Invalid format");
  }

}

int width(const Range<Epoch::Date> &rng) {
  int ret = 0;
  auto dt = rng.lower();
  while (dt < rng.upper()) {
    dt.addDay(1);
    ++ret;
  }
  return ret;
}

template <>
Range<Epoch::Date>& shift<Epoch::Date>(Range<Epoch::Date> &rng, int day) {
  rng.setUpper(rng.upper().addDay(day));
  rng.setLower(rng.lower().addDay(day));
  return rng;
}

