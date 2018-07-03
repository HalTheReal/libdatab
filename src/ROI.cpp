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

ROIB centerByWidth(int center, int width) {
  int left = center - (width / 2);
  int right = center + (width / 2);
  if (width % 2 == 0) {
    --right;
  }
  return ROIB(left, right);
}

ROIE toROIE(const ROIB &roi, double m, double q) {
  double left = (roi.lower() * m ) + q;
  double right = (roi.upper() * m) + q;
  return ROIE(left, right);
}

ROIB toROIB(const ROIE &roi, double m, double q) {
  int left = (roi.lower() - q) / m;
  int right = (roi.upper() - q) / m;
  return ROIB(left, right);
}

double counts(const Spectrum &sp, const ROIB &roi) {
  double tot = 0;
  for(int i = roi.lower(); i <= roi.upper(); ++i) {
    tot += sp.binAt(i);
  }
  return tot;
}

double counts(const Spectrum &sp, const ROIE &roi) {
  ROIB roib = toROIB(roi, sp.getM(), sp.getQ());
  return counts(sp, roib);
}

double cps(const Spectrum &sp, const ROIB &roi) {
  return counts(sp, roi) / sp.getDT();
}

double cps(const Spectrum &sp, const ROIE &roi) {
  ROIB roib = toROIB(roi, sp.getM(), sp.getQ());
  return cps(sp, roib);
}

}
