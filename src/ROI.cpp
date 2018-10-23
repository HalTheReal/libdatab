#include <ROI.h>

namespace Spectrometry {

BinRange::BinRange()
  : Range<int>()
{}

BinRange::BinRange(int leftEdge, int rightEdge)
  : Range<int>(leftEdge, rightEdge)
{}

int width(const BinRange &rng) {
  return rng.upper() - rng.lower() + 1;
}

BinRange centerByWidth(int center, int width) {
  int left = center - (width / 2);
  int right = center + (width / 2);
  if (width % 2 == 0) {
    --right;
  }
  return BinRange(left, right);
}

std::istream& operator >> (std::istream &stream, BinRange &rng) {
    char s1;
    int low, up;
    stream >> low >> s1 >> up;
    if (!stream) {
      return stream;
    }
    rng = BinRange(low, up);
    return stream;
}

EnrRange::EnrRange()
  : Range<double>()
{}

EnrRange::EnrRange(double leftEdge, double rightEdge)
  : Range<double>(leftEdge, rightEdge)
{}

double width(const EnrRange &rng) {
  return rng.upper() - rng.lower();
}

EnrRange centerByWidth(double center, double width) {
  double left = center - (width / 2.0);
  double right = center + (width / 2.0);
  return EnrRange(left, right);
}

std::istream& operator >> (std::istream &stream, EnrRange &rng) {
    char s1;
    double low, up;
    stream >> low >> s1 >> up;
    if (!stream) {
      return stream;
    }
    rng = EnrRange(low, up);
    return stream;
}

EnrRange toEnrRange(const BinRange &rng, double m, double q) {
  double left = binToEnergy(m, q, rng.lower());
  double right = binToEnergy(m, q, rng.upper());
  return EnrRange(left, right);
}

BinRange toBinRange(const EnrRange &rng, double m, double q) {
  int left = energyToBin(m, q, rng.lower());
  int right = energyToBin(m, q, rng.upper());
  return BinRange(left, right);
}

double integral(const Spectrum &sp, const BinRange &rng) {
  double tot = 0;
  for(int i = rng.lower(); i <= rng.upper(); ++i) {
    tot += sp.binAt(i);
  }
  return tot;
}

double integral(const Spectrum &sp, const EnrRange &rng) {
  BinRange brng = toBinRange(rng, sp.getM(), sp.getQ());
  return integral(sp, brng);
}

double cps(const Spectrum &sp, const BinRange &rng) {
  return integral(sp, rng) / sp.getDT();
}

double cps(const Spectrum &sp, const EnrRange &rng) {
  BinRange brng = toBinRange(rng, sp.getM(), sp.getQ());
  return cps(sp, brng);
}

ROI::ROI(const BinRange &rng, double m, double q)
  : br(rng)
  , er(toEnrRange(rng, m, q))
{}

ROI::ROI(const EnrRange &rng, double m, double q)
  : br(toBinRange(rng, m, q))
  , er(rng)
{}

}
