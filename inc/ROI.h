#ifndef ROI_H
#define ROI_H

#include <utility>    // std::swap
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
    ROI();
    ROI(T leftEdge, T rightEdge);
  private:
    void invariant();
    T left;
    T right;
};

template <typename T>
ROI<T>::ROI()
  : left(0)
  , right(0)
{}

template <typename T>
ROI<T>::ROI(T leftEdge, T rightEdge)
  : left(leftEdge)
  , right(rightEdge)
{
  invariant();
}

template <typename T>
void ROI<T>::invariant() {
  if(left > right) {
    std::swap(left, right);
  }
}

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
  invariant();
  return *this;
}

template <typename T>
ROI<T>& ROI<T>::setUpper(T set) {
  right = set;
  invariant();
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

template <typename T>
std::string to_string(const ROI<T> &roi, char sep = '-') {
  std::stringstream ss;
  ss << roi.lower() << sep << roi.upper();
  return ss.str();
}

template <typename T>
std::ostream& operator << (std::ostream &stream, const ROI<T> &roi) {
  stream << to_string(roi);
  return stream;
}

class ROIB : public ROI<int> {
  public:
    ROIB();
    ROIB(int leftEdge, int rightEdge);
};

int width(const ROIB &roi);
ROIB centerByWidth(int center, int width);
std::istream& operator >> (std::istream &stream, ROIB &roi);

class ROIE : public ROI<double> {
  public:
    ROIE();
    ROIE(double leftEdge, double rightEdge);
};

double width(const ROIE &roi);
ROIE centerByWidth(double center, double width);
std::istream& operator >> (std::istream &stream, ROIE &roi);

ROIE toROIE(const ROIB &roi, double m, double q);
ROIB toROIB(const ROIE &roi, double m, double q);

// ----- Da mettere in Spectrum ? -----

double integral(const Spectrum &sp, const ROIB &roi);
double integral(const Spectrum &sp, const ROIE &roi);
double cps(const Spectrum &sp, const ROIB &roi);
double cps(const Spectrum &sp, const ROIE &roi);

ROIE toROIE(const ROIB &roi, const Spectrum &sp);
ROIB toROIB(const ROIE &roi, const Spectrum &sp);

// ------------------------------------

}

#endif
