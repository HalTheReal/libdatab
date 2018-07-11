#include <ROI.h>

namespace Spectrometry {

ROIB::ROIB(int leftEdge, int rightEdge)
  : ROI<int>(leftEdge, rightEdge)
{}

ROIE::ROIE(double leftEdge, double rightEdge)
  : ROI<double>(leftEdge, rightEdge)
{}

int width(const ROIB &roi) {
  return roi.upper() - roi.lower() + 1;
}

ROIE centerByWidth(double center, double width) {
  double left = center - (width / 2.0);
  double right = center + (width / 2.0);
  return ROIE(left, right);
}

std::istream& operator >> (std::istream &stream, ROIE &roi) {
    char s1;
    double low, up;
    stream >> low >> s1 >> up;
    if (!stream) {
      return stream;
    }
    roi = ROIE(low, up);
    return stream;
}

ROIB centerByWidth(int center, int width) {
  int left = center - (width / 2);
  int right = center + (width / 2);
  if (width % 2 == 0) {
    --right;
  }
  return ROIB(left, right);
}

std::istream& operator >> (std::istream &stream, ROIB &roi) {
    char s1;
    int low, up;
    stream >> low >> s1 >> up;
    if (!stream) {
      return stream;
    }
    roi = ROIB(low, up);
    return stream;
}

ROIE toROIE(const ROIB &roi, double m, double q) {
  double left = binToEnergy(m, q, roi.lower());
  double right = binToEnergy(m, q, roi.upper());
  return ROIE(left, right);
}

ROIB toROIB(const ROIE &roi, double m, double q) {
  int left = energyToBin(m, q, roi.lower());
  int right = energyToBin(m, q, roi.upper());
  return ROIB(left, right);
}

ROIE toROIE(const ROIB &roi, const Spectrum &sp) {
  return toROIE(roi, sp.getM(), sp.getQ());
}

ROIB toROIB(const ROIE &roi, const Spectrum &sp) {
  return toROIB(roi, sp.getM(), sp.getQ());
}

double integral(const Spectrum &sp, const ROIB &roi) {
  double tot = 0;
  for(int i = roi.lower(); i <= roi.upper(); ++i) {
    tot += sp.binAt(i);
  }
  return tot;
}

double integral(const Spectrum &sp, const ROIE &roi) {
  ROIB roib = toROIB(roi, sp.getM(), sp.getQ());
  return integral(sp, roib);
}

double cps(const Spectrum &sp, const ROIB &roi) {
  return integral(sp, roi) / sp.getDT();
}

double cps(const Spectrum &sp, const ROIE &roi) {
  ROIB roib = toROIB(roi, sp.getM(), sp.getQ());
  return cps(sp, roib);
}

}
