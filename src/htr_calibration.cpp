#include "htr_calibration.h"

MMZ::Line annealingCal(const Spectrometry::Spectrum &sp, double minM, double maxM, double minQ, double maxQ) {
  double temp = 4000;
  std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution <double> distChange(0, 1);
  std::uniform_real_distribution <double> distM(minM, maxM);
  std::uniform_real_distribution <double> distQ(minQ, maxQ);
  double mEst = distM(generator);
  double qEst = distQ(generator);
  double energy = sysEnergy(sp, mEst, qEst);
  do {
    double mEstNew = distM(generator);
    double qEstNew = distQ(generator);
    double newEnergy = sysEnergy(sp, mEstNew, qEstNew);
    if(stateChangeProb(energy, newEnergy, temp) >= distChange(generator)) {
      energy = newEnergy;
      mEst = mEstNew;
      qEst = qEstNew;
    }
    temp -= 0.05;
  } while (temp > 0);
  return MMZ::Line(mEst, qEst);
}

// Ottimizzata per spettri Gammastream
double sysEnergy(const Spectrometry::Spectrum &sp, double mC, double qC) {
  std::vector <double> egy {1460, 2614};
  double points = 0;
  for (auto en : egy) {
    int middle = (en - qC) / mC;
    int lower = middle - 5;
    int upper = middle + 5;
    if (lower >= 0 && upper < sp.channels()) {
      for(int i = lower; i <= upper; ++i) {
        points += log(sp.binAt(i)) * sqrt(i);
      }
    }
  }
  return - points;
}

double stateChangeProb(double en1, double en2, double temp) {
  if(en2 < en1) {
    return 1;
  }
  return exp(-(en2 - en1) / temp);
}
