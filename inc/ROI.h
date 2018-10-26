#ifndef ROI_H
#define ROI_H

#include <utility>    // std::swap
#include <Spectrum.h>

namespace Spectrometry {

template <typename T>

class Range {
  public:
    Range();
    Range(T leftEdge, T rightEdge);
    T lower() const;
    T upper() const;
    Range& setLower(T set);
    Range& setUpper(T set);
  private:
    // Left <= right
    T left;
    T right;
};

template <typename T>
Range<T>::Range()
  : left()
  , right()
{}

template <typename T>
Range<T>::Range(T leftEdge, T rightEdge)
  : left(leftEdge)
  , right(rightEdge)
{
  if(left > right) {
    std::swap(left, right);
  }
}

template <typename T>
T Range<T>::lower() const {
  return left;
}

template <typename T>
T Range<T>::upper() const {
  return right;
}

template <typename T>
Range<T>& Range<T>::setLower(T set) {
  left = set;
  if (left > right) {
    right = left;
  }
  return *this;
}

template <typename T>
Range<T>& Range<T>::setUpper(T set) {
  right = set;
  if (left > right) {
    left = right;
  }
  return *this;
}

template <typename T, typename R>
Range<T>& inflate(Range<T> &rng, R inf) {
  rng.setUpper(rng.upper() + inf);
  rng.setLower(rng.lower() - inf);
  return rng;
}

template <typename T, typename R>
Range<T>& shift(Range<T> &rng, R inf) {
  rng.setUpper(rng.upper() + inf);
  rng.setLower(rng.lower() + inf);
  return rng;
}

template <typename T>
std::string to_string(const Range<T> &rng, char sep = '-') {
  std::stringstream ss;
  ss << rng.lower() << sep << rng.upper();
  return ss.str();
}

template <typename T>
std::ostream& operator << (std::ostream &stream, const Range<T> &rng) {
  stream << to_string(rng);
  return stream;
}

class BinRange : public Range<int> {
  public:
    BinRange();
    BinRange(int leftEdge, int rightEdge);
};

int width(const BinRange &rng);
BinRange centerByWidth(int center, int width);
std::istream& operator >> (std::istream &stream, BinRange &rng);

class EnrRange : public Range<double> {
  public:
    EnrRange();
    EnrRange(double leftEdge, double rightEdge);
};

double width(const EnrRange &rng);
EnrRange centerByWidth(double center, double width);
std::istream& operator >> (std::istream &stream, EnrRange &rng);

// ----- CONVERSIONI ----- //

EnrRange toEnrRange(const BinRange &rng, double m, double q);
BinRange toBinRange(const EnrRange &rng, double m, double q);

template <typename S>
EnrRange toEnrRange(const BinRange &rng, const S &sp) {
  return toEnrRange(rng, sp.getM(), sp.getQ());
}

template <typename S>
BinRange toBinRange(const EnrRange &rng, const S &sp) {
  return toBinRange(rng, sp.getM(), sp.getQ());
}

// -----             ----- //

// ----- Da mettere in Spectrum ? -----

double integral(const Spectrum &sp, const BinRange &rng);
double integral(const Spectrum &sp, const EnrRange &rng);
double cps(const Spectrum &sp, const BinRange &rng);
double cps(const Spectrum &sp, const EnrRange &rng);

// ------------------------------------

class ROI {
  public:
    ROI(const BinRange &rng, double m, double q);
    ROI(const EnrRange &rng, double m, double q);
    template <typename S>
      ROI(const BinRange &rng, const S &sp);
    template <typename S>
      ROI(const EnrRange &rng, const S &sp);

    int lowerBin() const;
    int upperBin() const;
    double lowerEnr() const;
    double upperEnr() const;
    double getM() const;
    double getQ() const;
    ROI& setLowerBin(int lw);
    ROI& setUpperBin(int up);
    ROI& setLowerEnr(double enr);
    ROI& setUpperEnr(double enr);
  private:
    BinRange brange;
    EnrRange erange;
    double mcal;
    double qcal;
};

template <typename S>
ROI::ROI(const BinRange &rng, const S &sp)
  : brange(rng)
  , erange(toEnrRange(rng, sp))
  , mcal(sp.getM())
  , qcal(sp.getQ())
{}

template <typename S>
ROI::ROI(const EnrRange &rng, const S &sp)
  : brange(toBinRange(rng, sp))
  , erange(toEnrRange(brange, sp))    // Il range di energia è fatto da multipli di mcal!
  , mcal(sp.getM())                   // altrimenti modificare successivamente i bin può
  , qcal(sp.getQ())                   // avere conseguenze sulle energie
{}

// La larghezza di una ROI è sempre almeno 1 bin
int widthBin(const ROI &roi);
double widthEnr(const ROI &roi);

ROI& inflateBin(ROI &roi, int bin);
ROI& inflateEnr(ROI &roi, double enr);
ROI& shiftBin(ROI &roi, int bin);
ROI& shiftEnr(ROI &roi, double enr);

}

#endif
