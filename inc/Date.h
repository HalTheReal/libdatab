#ifndef DATE_H
#define DATE_H

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>    // Eccezioni
#include <iostream>     // std::cout
#include <tools.h>
#include <math.h>       // fmod

namespace Epoch {

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

  bool operator == (const Date &dt1, const Date &dt2);
  bool operator != (const Date &dt1, const Date &dt2);
  bool operator < (const Date &dt1, const Date &dt2);
  bool operator <= (const Date &dt1, const Date &dt2);
  bool operator > (const Date &dt1, const Date &dt2);
  bool operator >= (const Date &dt1, const Date &dt2);

  std::ostream& operator << (std::ostream &stream, const Date &dt);
  std::istream& operator >> (std::istream &stream, Date &dt);

  std::string to_string(const Date &dt, char sep = '/');
  Date strToDate(const std::string &str);

}

#endif
