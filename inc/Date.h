#ifndef DATE_H
#define DATE_H

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>    // Eccezioni
#include <iostream>     // std::cout
#include <math.h>       // fmod

namespace Chrono {

  class Date {

    public:
      Date();
      Date(int dy, int mt, int yr);
      int day() const;
      int month() const;
      int year() const;
      Date& addDay(int toAdd);
      Date& addMonth(int toAdd);
      Date& addYear(int toAdd);
    
    private:
      int dDay;
      int dMonth;
      int dYear;

      bool isValid() const;
  };

  bool isLeap(const Date &dt);

  int daysInMonth(const Date &dt);

  int daysInYear(const Date &dt);

  std::string toString(const Date &dt, char sep = '/');
  Date strToDate(const std::string &str, char sep = '/');

}

#endif
