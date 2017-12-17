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
      
    private:
      Date dt;
      Time tm;

  };

  int toUnix(const DateTime &dtt);

}

#endif
