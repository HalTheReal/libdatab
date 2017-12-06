#include "Spectrum.h"

namespace physics {

  Spectrum::Spectrum()
      : canali(0)
      , mCal(1)
      , qCal(0)
  {}

  Spectrum::Spectrum(const std::vector <int> &hist)
      : Spectrum()
  {
    bin.reserve(hist.size());
    for (int v : hist) {
      bin.push_back(v);
    }
    canali = bin.size();
  }

  Spectrum::Spectrum(const std::vector <float> &hist)
      : Spectrum()
  {
    bin.reserve(hist.size());
    for (float v : hist) {
      bin.push_back(v);
    }
    canali = bin.size();
  }

  Spectrum::Spectrum(const std::vector <double> &hist)
      : Spectrum()
  {
    bin.reserve(hist.size());
    for (double v : hist) {
      bin.push_back(v);
    }
    canali = bin.size();
  }
  
  Spectrum& Spectrum::operator+=(const Spectrum& rhs) {
    for (int i = 0; i < canali; ++i) {
      bin[i] += rhs.bin[i];
      if (bin[i] < 0) {
        bin[i] = 0;
      }
    }
    return *this;
  }

  Spectrum operator+(Spectrum lhs, const Spectrum& rhs) {
    lhs += rhs;
    return lhs;
  }

  Spectrum& Spectrum::operator-=(const Spectrum& rhs) {
    for (int i = 0; i < canali; ++i) {
      bin[i] -= rhs.bin[i];
      if (bin[i] < 0) {
        bin[i] = 0;
      }
    }
    return *this;
  }

  Spectrum operator-(Spectrum lhs, const Spectrum& rhs) {
    lhs -= rhs;
    return lhs;
  }

  int Spectrum::energyToBin(double en) const {
    return (en - qCal) / mCal;
  }

  Spectrum& Spectrum::calibrate(double m, double q) {
    mCal = m;
    qCal = q;
    return *this;
  }

  Spectrum& Spectrum::rebin(double gain) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    return rebin(gain, seed);
  }

  Spectrum& Spectrum::rebin(double gain, unsigned seed) {
    std::default_random_engine generator(seed);
    std::vector <float> newBin(canali, 0);
    for (int i = 0; i < canali; ++i) {
      double E0 = i * mCal + 0.5 * mCal + qCal;
      double Emin = E0 - 0.5 * mCal;
      double Emax = E0 + 0.5 * mCal;
      std::uniform_real_distribution <double> distribution(Emin, Emax);
      for (int j = 0; j < bin[i]; ++j) {
        double rnd = distribution(generator);
        int idx = ceil(rnd / gain);
        if (idx >= 0 && idx < canali) {
          ++newBin[idx];
        }
      }
    }
    mCal = gain;
    qCal = 0;
    bin = newBin;
    return *this;
  }

  double Spectrum::binAt(int b1) const {
    return bin[b1];
  }

  double Spectrum::binAt(int b1, int b2) const {
    double integral = 0;
    for(int i = b1; i <= b2; ++i) {
      integral += bin[i];
    }
    return integral;
  }
}
//  void Spectrum::writeSPT(const char * nomeFile) {
//    using namespace std;
//    ofstream outfile;
//    outfile.open(nomeFile);
//    if (outfile) {
//      outfile << canali << " " << dT;
//      if (mCal != 1) {
//        outfile << " true ";
//      }
//      else {
//        outfile << " false ";
//      }
//      outfile << qCal << " " << mCal << endl;
//      outfile << "# S_TIME: 000 " << dataSpt.toString('-', ':', 'B') << endl;
//      outfile << "# " << dataSpt.toString('-', ':', 'B') << "# DET # Spectrum.cpp" << endl;
//      for (int i = 0; i < canali; ++i) {
//        outfile << bin[i];
//        if (i % 8 == 7) {
//          outfile << endl;
//        }
//        else {
//          outfile << " ";
//        }
//      }
//    }
//    outfile.close();
//  }
//
//  void Spectrum::writeSPE(const char * nomeFile) {
//    using namespace std;
//    ofstream outfile;
//    outfile.open(nomeFile);
//    if (outfile) {
//      outfile << "$SPEC_ID:\nSpectrum.cpp\n";
//      outfile << "$DATE_MEA:\n" << dataSpt.toString('-', ':', 'B') << endl;
//      outfile << "$MEAS_TIM:\n" << dT << " " << dT << endl;
//      outfile << "$DATA:\n" << 0 << " " << canali - 1 << endl;
//      for (int i = 0; i < canali; ++i) {
//        outfile << bin[i] << endl;
//      }
//    }
//    outfile.close();
//  }
//int Spectrum::readSPE(const char * nomeFile) {
//  std::ifstream file(nomeFile);
//  if (!file) {
//    fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
//    return 0;
//  }
//  bin.clear();
//  std::string riga;
//  while (std::getline(file,riga)) {
//    if (riga[0] == '$') {
//      std::vector <std::string> toks;
//      if (riga.compare("$DATE_MEA:") == 0) {
//        std::getline(file, riga);
//        toks = tls::splitWhite(riga);
//        dataSpt = Data(toks[0], toks[1], '-', ':', 'B');
//      }
//      else if (riga.compare("$MEAS_TIM:") == 0) {
//        std::getline(file, riga);
//        toks = tls::splitWhite(riga);
//        dT = std::stof(toks[0]);
//      }
//      else if (riga.compare("$DATA:") == 0) {
//        std::getline(file, riga);
//        toks = tls::splitWhite(riga);
//        canali = std::stod(toks[1]) + 1;
//      }
//      else {
//        std::getline(file, riga);
//      }
//    }
//    else {
//      bin.push_back(stof(riga));
//    }
//  }
//  file.close();
//  return 1;
//}
//
//int Spectrum::readSPT(const char * nomeFile) {
//  std::ifstream file(nomeFile);
//  if (!file) {
//    fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
//    return 0;
//  }
//  bin.clear();
//  std::string riga;
//  while (std::getline(file,riga)) {
//    std::vector <std::string> toks = tls::splitWhite(riga);
//    if (toks.size() == 5) {
//      if (toks[1].compare("S_TIME:") == 0) {
//        dataSpt = Data(toks[3], toks[4], '-', ':', 'B');
//      }
//      else {
//        canali = std::stod(toks[0]);
//        dT = std::stof(toks[1]);
//        qCal = std::stod(toks[3]);
//        mCal = std::stod(toks[4]);
//      }
//    }
//    else if (toks.size() == 8) {
//      for (int i = 0; i < 8; ++i) {
//        bin.push_back(std::stof(toks[i]));
//      }
//    }
//  }
//  file.close();
//  return 1;
//}
//
//int Spectrum::readLST(const char * nomeFile) {
//  std::ifstream file(nomeFile);
//  if (!file) {
//    fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
//    return 0;
//  }
//  canali = 2048;
//  bin.clear();
//  bin.resize(canali, 0);
//  std::string token;
//  do {
//    file >> token;
//    if (token.compare("#StartTime:") == 0) {
//      file >> token;
//      std::vector <std::string> dateToks = tls::split(token, 'T');
//      dataSpt = Data(dateToks[0], dateToks[1], '-', ':', 'B');
//    }
//  } while (token != "Gain");
//
//  long timeTok;
//  double gainTok;
//  int energyTok;
//  while (file >> timeTok >> energyTok >> gainTok) {
//    ++bin[energyTok];
//  }
//  dT = timeTok * 16E-9;
//  file.close();
//  return 1;
//}
//
