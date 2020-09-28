#include <htr_roi.h>

namespace Spectrometry {

BinRange::BinRange()
  : Range<int>()
{}

BinRange::BinRange(int leftEdge, int rightEdge)
  : Range<int>(leftEdge, rightEdge)
{
  if(lower() < 0 || upper() < 0) {
    throw std::runtime_error("Invalid BinRange");
  }
}

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
  double right = binToEnergy(m, q, rng.upper() + 1);
  return EnrRange(left, right);
}

BinRange toBinRange(const EnrRange &rng, double m, double q) {
  int left = energyToBin(m, q, rng.lower());
  int right = energyToBin(m, q, rng.upper()) - 1;
  if(right < left) {
    right = left;
  }
  return BinRange(left, right);
}

EnrRange toEnrRange(const ROI &roi) {
  return EnrRange(roi.lowerEnr(), roi.upperEnr());
}

BinRange toBinRange(const ROI &roi) {
  return BinRange(roi.lowerBin(), roi.upperBin());
}

ROI::ROI(const BinRange &rng, double m, double q)
  : brange(rng)
  , erange(toEnrRange(rng, m, q))
  , mcal(m)
  , qcal(q)
{}

ROI::ROI(const EnrRange &rng, double m, double q)
  : brange(toBinRange(rng, m, q))
  , erange(toEnrRange(brange, m, q))      // Il range di energia è fatto da multipli di mcal!
  , mcal(m)                               // altrimenti modificare successivamente i bin può
  , qcal(q)                               // avere conseguenze sulle energie
{}

int ROI::lowerBin() const {
  return brange.lower();
}

int ROI::upperBin() const {
  return brange.upper();
}

double ROI::lowerEnr() const {
  return erange.lower();
}

double ROI::upperEnr() const {
  return erange.upper();
}

double ROI::getM() const {
  return mcal;
}

double ROI::getQ() const {
  return qcal;
}

ROI& ROI::setLowerBin(int lw) {
  brange.setLower(lw);
  erange = toEnrRange(brange, mcal, qcal);
  return *this;
}

ROI& ROI::setUpperBin(int up) {
  brange.setUpper(up);
  erange = toEnrRange(brange, mcal, qcal);
  return *this;
}

ROI& ROI::setLowerEnr(double enr) {
  erange.setLower(enr);
  brange = toBinRange(erange, mcal, qcal);
  erange = toEnrRange(brange, mcal, qcal);
  return *this;
}

ROI& ROI::setUpperEnr(double enr) {
  erange.setUpper(enr);
  brange = toBinRange(erange, mcal, qcal);
  erange = toEnrRange(brange, mcal, qcal);
  return *this;
}

int widthBin(const ROI &roi) {
  return roi.upperBin() - roi.lowerBin() + 1;
}

double widthEnr(const ROI &roi) {
  return roi.upperEnr() - roi.lowerEnr();
}

ROI& inflateBin(ROI &roi, int bin) {
  int newLower = roi.lowerBin() - bin;
  int newUpper = roi.upperBin() + bin;
  if (newLower < 0) {
    newLower = 0;
  }
  // Si verifica solo se bin è troppo negativo, la roi collassa
  // al centro, larghezza 1
  if (newLower > roi.upperBin() || newUpper < roi.lowerBin()) {
    int center = round((roi.upperBin() + roi.lowerBin()) / 2.0);
    newLower = center;
    newUpper = center;
  }
  roi.setLowerBin(newLower);
  roi.setUpperBin(newUpper);
  return roi;
}

ROI& inflateEnr(ROI &roi, double enr) {
  double newLower = roi.lowerEnr() - enr;
  double newUpper = roi.upperEnr() + enr;
  if (newLower < 0) {
    newLower = 0;
  }
  // Si verifica solo se enr è troppo negativo, la roi collassa
  // al centro, larghezza 1
  if (newLower > roi.upperEnr() || newUpper < roi.lowerEnr()) {
    double center = round((roi.upperEnr() + roi.lowerEnr()) / 2.0);
    newLower = center;
    newUpper = center;
  }
  roi.setLowerEnr(newLower);
  roi.setUpperEnr(newUpper);
  return roi;
}

ROI& shiftBin(ROI &roi, int bin) {
  roi.setLowerBin(roi.lowerBin() + bin);
  roi.setUpperBin(roi.upperBin() + bin);
  return roi;
}

ROI& shiftEnr(ROI &roi, double enr) {
  roi.setLowerEnr(roi.lowerEnr() + enr);
  roi.setUpperEnr(roi.upperEnr() + enr);
  return roi;
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

double integral(const Spectrum &sp, const ROI &roi) {
  return integral(sp, toBinRange(roi));
}

double cps(const Spectrum &sp, const BinRange &rng) {
  return integral(sp, rng) / sp.getLT();
}

double cps(const Spectrum &sp, const EnrRange &rng) {
  BinRange brng = toBinRange(rng, sp.getM(), sp.getQ());
  return cps(sp, brng);
}

double cps(const Spectrum &sp, const ROI &roi) {
  return cps(sp, toBinRange(roi));
}

}
