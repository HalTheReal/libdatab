#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>    // Eccezioni
#include <iostream>     // std::cout
#include <chrono>
#include <ctime>
#include <htr_time.h>
#include <htr_date.h>

namespace Epoch {

  class DateTime {

    public:
      DateTime();
      DateTime(int dy, int mn, int yr, int hr, int mi, int sc);
      DateTime(int dy, int mn, int yr, int hr, int mi, int sc, const TimeZone &tz);
      DateTime(const Date &dt, const Time &tm);

      DateTime& addDay(int toAdd);
      DateTime& addSec(int toAdd);
      int day() const;
      int month() const;
      int year() const;
      int hour() const;
      int mnt() const;
      int sec() const;
      TimeZone zone() const;
      
    private:
      Date dateNow;
      Time timeNow;
  };

  Date toDate(const DateTime &dtt);
  Time toTime(const DateTime &dtt);
  DateTime toTimeZone(DateTime dtt, const TimeZone &tz); 
  DateTime toUTC(DateTime dtt); 

  int toUnix(DateTime dtt);
  std::string to_string(const DateTime &dtt);
  DateTime strToDateTime(const std::string &str);

  bool operator == (const DateTime &dtt1, const DateTime &dtt2);
  bool operator != (const DateTime &dtt1, const DateTime &dtt2);
  bool operator < (const DateTime &dtt1, const DateTime &dtt2);
  bool operator <= (const DateTime &dtt1, const DateTime &dtt2);
  bool operator > (const DateTime &dtt1, const DateTime &dtt2);
  bool operator >= (const DateTime &dtt1, const DateTime &dtt2);

  std::ostream& operator << (std::ostream &stream, const DateTime &dtt);
  std::istream& operator >> (std::istream &stream, DateTime &dtt);

  DateTime to_DateTime(const std::tm &tm);
  DateTime local();
  DateTime gmt();

}

int width(const Range<Epoch::DateTime> &rng);
template <>
Range<Epoch::DateTime>& shift<Epoch::DateTime>(Range<Epoch::DateTime> &rng, int sec);

#endif
