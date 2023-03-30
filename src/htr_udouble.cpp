#include "htr_udouble.h"

udouble::udouble()
  : value(0)
  , uncert(0)
{}

udouble::udouble(double val)
  : value(val)
  , uncert(0)
{}

udouble::udouble(double val, double unc)
  : value(val)
  , uncert(fabs(unc))
{}

double udouble::val() const {
  return value;
}

udouble & udouble::val(double newval) {
  value = newval;
  return *this;
}

double udouble::unc() const {
  return uncert;
}

udouble & udouble::unc(double newunc) {
  uncert = newunc;
  return *this;
}

udouble & udouble::operator +=(const udouble &rhs) {
  value += rhs.val();
  uncert = sqrt(pow(uncert, 2) + pow(rhs.unc(), 2));
  return *this;
}

udouble & udouble::operator -=(const udouble &rhs) {
  value -= rhs.val();
  uncert = sqrt(pow(uncert, 2) + pow(rhs.unc(), 2));
  return *this;
}

udouble & udouble::operator *=(const udouble &rhs) {
  uncert = pow(rhs.val(), 2) * pow(uncert, 2) + pow(value, 2) * pow(rhs.unc(), 2);
  uncert = sqrt(uncert);
  value *= rhs.val();
  return *this;
}

udouble & udouble::operator /=(const udouble &rhs) {
  uncert =  pow(uncert, 2) / pow(rhs.val(), 2) +
            pow(value, 2) * pow(rhs.unc(), 2) / pow(rhs.val(), 4);
  uncert = sqrt(uncert);
  value /= rhs.val();
  return *this;
}

udouble udouble::operator -() const {
    return udouble(-value, uncert);
}

udouble operator + (udouble lhs, const udouble &rhs) {
  lhs += rhs;
  return lhs;
}

udouble operator - (udouble lhs, const udouble &rhs) {
  lhs -= rhs;
  return lhs;
}

udouble operator * (udouble lhs, const udouble &rhs) {
  lhs *= rhs;
  return lhs;
}

udouble operator / (udouble lhs, const udouble &rhs) {
  lhs /= rhs;
  return lhs;
}

bool operator == (const udouble &lhs, const udouble &rhs) {
    if (lhs.val() == rhs.val()) {
        return true;
    }
    return false;
}

bool operator != (const udouble &lhs, const udouble &rhs) {
    return !(lhs == rhs);
}

bool operator < (const udouble &lhs, const udouble &rhs) {
    return lhs.val() < rhs.val();
}

bool operator <= (const udouble &lhs, const udouble &rhs) {
    return (lhs < rhs || lhs == rhs);
}

bool operator > (const udouble &lhs, const udouble &rhs) {
    return !(lhs <= rhs);
}

bool operator >= (const udouble &lhs, const udouble &rhs) {
    return !(lhs < rhs);
}

std::istream& operator >> (std::istream &stream, udouble &ud) {
    std::string sep;
    double value, uncert;
    stream >> value >> sep >> uncert;
    if (!stream) {
      return stream;
    }
    if (sep.compare("+/-") != 0) {
      stream.setstate(std::ios_base::failbit);
      return stream;
    }
    try {
      ud = udouble(value, uncert);
    }
    catch (const std::exception &e) {
      stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

std::ostream& operator << (std::ostream &stream, const udouble &ud) {
  std::stringstream ss;
  ss.copyfmt(stream);
  ss << ud.val() << " +/- ";
  ss << ud.unc();
  stream << ss.str();
  return stream;
}

udouble sqrt(const udouble &ud) {
    double newVal = sqrt(ud.val());
    double newUnc = ud.unc() / (2 * sqrt(ud.val()));
    return udouble(newVal, newUnc);
}
