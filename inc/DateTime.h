#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>    // Eccezioni
#include <iostream>     // std::cout
#include <Time.h>
#include <Date.h>

namespace Chrono {

  class DateTime {

    public:
      DateTime();
      DateTime(int dy, int mn, int yr, int hr, int mi, int sc);
      DateTime(const Time &tm, const Date &dt);

      DateTime& addDay(int toAdd);
      DateTime& addSec(int toAdd);
      int day() const;
      int month() const;
      int year() const;
      int hour() const;
      int min() const;
      int sec() const;
      
    private:
      Date dateNow;
      Time timeNow;

  };

  Date toDate(const DateTime &dtt);
  Time toTime(const DateTime &dtt);

  int toUnix(const DateTime &dtt);
  std::string toString(const DateTime &dtt, char dateSep = '/');

  bool operator == (const DateTime &dtt1, const DateTime &dtt2);
  bool operator != (const DateTime &dtt1, const DateTime &dtt2);
  bool operator < (const DateTime &dtt1, const DateTime &dtt2);
  bool operator <= (const DateTime &dtt1, const DateTime &dtt2);
  bool operator > (const DateTime &dtt1, const DateTime &dtt2);
  bool operator >= (const DateTime &dtt1, const DateTime &dtt2);

}

#endif
