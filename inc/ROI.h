#ifndef ROI_H
#define ROI_H

#include <Spectrum.h>

namespace Spectrometry {

template <typename T>
class ROI {
  public:
    ROI(T leftEdge, T rightEdge);
    T lower() const;
    T upper() const;
    ROI& setLower(T set);
    ROI& setUpper(T set);
  private:
    T left;
    T right;
};

typedef ROI<int> ROIB;
typedef ROI<double> ROIE;

template <typename T>
ROI<T>::ROI(T leftEdge, T rightEdge)
  : left(leftEdge)
  , right(rightEdge)
{}

template <typename T>
T ROI<T>::lower() const {
  return left;
}

template <typename T>
T ROI<T>::upper() const {
  return right;
}

template <typename T>
ROI<T>& ROI<T>::setLower(T set) {
  left = set;
  return *this;
}

template <typename T>
ROI<T>& ROI<T>::setUpper(T set) {
  right = set;
  return *this;
}

template <typename T>
T width(const ROI<T> &roi) {
  return roi.upper() - roi.lower();
}

int width(const ROIB &roi);

template <typename T, typename R>
ROI<T>& inflate(ROI<T> &roi, R inf) {
  roi.setUpper(roi.upper() + inf);
  roi.setLower(roi.lower() - inf);
  return roi;
}

template <typename T, typename R>
ROI<T>& shift(ROI<T> &roi, R inf) {
  roi.setUpper(roi.upper() + inf);
  roi.setLower(roi.lower() + inf);
  return roi;
}

ROIE centerByWidth(double center, double width);
ROIB centerByWidth(int center, int width);

ROIE toROIE(const ROIB &roi, double m, double q);
ROIB toROIB(const ROIE &roi, double m, double q);

double counts(const Spectrum &sp, const ROIB &roi);
double counts(const Spectrum &sp, const ROIE &roi);
double cps(const Spectrum &sp, const ROIB &roi);
double cps(const Spectrum &sp, const ROIE &roi);

}

#endif
