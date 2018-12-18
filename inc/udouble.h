#ifndef MY_UDOUBLE
#define MY_UDOUBLE

#include <ostream>
#include <sstream>
#include <cmath>
#include <stdexcept>    // Eccezioni

class udouble {
  public:
    udouble(double val, double unc);
    double val() const;
    udouble & val(double newval);
    double unc() const;
    udouble & unc(double newunc);
  private:
    double value;
    double uncert;
    bool isValid();
};

std::ostream& operator << (std::ostream &stream, const udouble &ud);
std::istream& operator >> (std::istream &stream, udouble &dt);

#endif
