#include "udouble.h"

udouble::udouble(double val, double unc)
  : value(val)
  , uncert(fabs(unc))
{
  if(!isValid()) {
    throw std::runtime_error("Invalid udouble");
  }
}

bool udouble::isValid() {
  if(uncert > value) {
    return false;
  }
  return true;
}

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
