#ifndef MY_UDOUBLE
#define MY_UDOUBLE

#include <ostream>
#include <sstream>
#include <cmath>
#include <stdexcept>    // Eccezioni

class udouble {
  public:
    udouble();
    udouble(double val, double unc);
    double val() const;
    udouble & val(double newval);
    double unc() const;
    udouble & unc(double newunc);
    udouble & operator +=(const udouble &rhs);
    udouble & operator -=(const udouble &rhs);
    udouble & operator *=(const udouble &rhs);
    udouble & operator /=(const udouble &rhs);
  private:
    double value;
    double uncert;
};

udouble operator + (udouble lhs, const udouble &rhs);
udouble operator - (udouble lhs, const udouble &rhs);
udouble operator * (udouble lhs, const udouble &rhs);
udouble operator / (udouble lhs, const udouble &rhs);
std::ostream& operator << (std::ostream &stream, const udouble &ud);
std::istream& operator >> (std::istream &stream, udouble &dt);

#endif
