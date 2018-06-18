#include <ROI.h>

namespace Spectrometry {

int width(const ROIB &roi) {
  return roi.upper() - roi.lower() + 1;
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

ROIB toROIB(const ROIB &roi, double m, double q) {
  int left = (roi.lower() - q) / m;
  int right = (roi.upper() - q) / m;
  return ROIB(left, right);
}

}
