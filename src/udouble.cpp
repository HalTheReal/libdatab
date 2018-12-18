#include "udouble.h"

udouble::udouble(double val, double unc)
  : value(val)
  , uncert(fabs(unc))
{
  if(!isValid()) {
    //throw
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

std::ostream& operator << (std::ostream &stream, const udouble &ud) {
  std::stringstream ss;
  ss.copyfmt(stream);
  ss << ud.val() << " \u00B1 ";
  ss << ud.unc();
  stream << ss.str();
  return stream;
}
