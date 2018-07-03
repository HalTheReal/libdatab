#ifndef ROI_H
#define ROI_H

#include <Spectrum.h>

namespace Spectrometry {

template <typename T>

class ROI {
  public:
    T lower() const;
    T upper() const;
    ROI& setLower(T set);
    ROI& setUpper(T set);
  protected:
    ROI(T leftEdge, T rightEdge);
  private:
    T left;
    T right;
};

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

class ROIB : public ROI<int> {
  public:
    ROIB(int leftEdge, int rightEdge);
};

int width(const ROIB &roi);
ROIB centerByWidth(int center, int width);

class ROIE : public ROI<double> {
  public:
    ROIE(double leftEdge, double rightEdge);
};

double width(const ROIE &roi);
ROIE centerByWidth(double center, double width);

// ------ Da mettere in Spectrum ------

ROIE toROIE(const ROIB &roi, double m, double q);
ROIB toROIB(const ROIE &roi, double m, double q);

double counts(const Spectrum &sp, const ROIB &roi);
double counts(const Spectrum &sp, const ROIE &roi);
double cps(const Spectrum &sp, const ROIB &roi);
double cps(const Spectrum &sp, const ROIE &roi);

// ------------------------------------

}

#endif
