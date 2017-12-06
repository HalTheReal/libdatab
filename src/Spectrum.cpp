#include "Spectrum.h"

namespace physics {

  // Costruttore
  Spectrum::Spectrum()
    : dT(0)
      , canali(0)
      , mCal(1)
      , qCal(0)
      , dataSpt(0)
  {}

  Spectrum::Spectrum(const char * nomeFile)
    : dT(0)
      , canali(0)
      , mCal(1)
      , qCal(0)
      , dataSpt(0)
  {
    readFile(nomeFile);
  }

  void Spectrum::defaultInit() {
    dT = 0;
    canali = 0;
    mCal = 1;
    qCal = 0;
    dataSpt = Data(0);
    bin.clear();
  }

  int Spectrum::readFile(const char * nomeFile) {
    std::string filename(nomeFile);
    std::string::size_type idx;

    idx = filename.rfind('.');
    try {
      if(idx != std::string::npos) {
        std::string extension = filename.substr(idx+1);
        if (extension.compare("spt") == 0) {
          return readSPT(nomeFile);
        }
        else if (extension.compare("spe") == 0) {
          return readSPE(nomeFile);
        }
        else if (extension.compare("txt") == 0) {
          return readLST(nomeFile);
        }
      }
      std::cout << nomeFile << " is not a valid file!\n";
    }
    catch (std::invalid_argument& e) {
      std::cout << "Invalid argument exception while reading file:\n";
      std::cout << nomeFile << std::endl;
      defaultInit();
    }
    catch (std::out_of_range& e) {
      std::cout << "Out of range exception while reading file:\n";
      std::cout << nomeFile << std::endl;
      defaultInit();
    }
    return 0;
  }

  int Spectrum::readSPE(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
      return 0;
    }
    bin.clear();
    std::string riga;
    while (std::getline(file,riga)) {
      if (riga[0] == '$') {
        std::vector <std::string> toks;
        if (riga.compare("$DATE_MEA:") == 0) {
          std::getline(file, riga);
          toks = tls::splitWhite(riga);
          dataSpt = Data(toks[0], toks[1], '-', ':', 'B');
        }
        else if (riga.compare("$MEAS_TIM:") == 0) {
          std::getline(file, riga);
          toks = tls::splitWhite(riga);
          dT = std::stof(toks[0]);
        }
        else if (riga.compare("$DATA:") == 0) {
          std::getline(file, riga);
          toks = tls::splitWhite(riga);
          canali = std::stod(toks[1]) + 1;
        }
        else {
          std::getline(file, riga);
        }
      }
      else {
        bin.push_back(stof(riga));
      }
    }
    file.close();
    return 1;
  }

  int Spectrum::readSPT(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
      return 0;
    }
    bin.clear();
    std::string riga;
    while (std::getline(file,riga)) {
      std::vector <std::string> toks = tls::splitWhite(riga);
      if (toks.size() == 5) {
        if (toks[1].compare("S_TIME:") == 0) {
          dataSpt = Data(toks[3], toks[4], '-', ':', 'B');
        }
        else {
          canali = std::stod(toks[0]);
          dT = std::stof(toks[1]);
          qCal = std::stod(toks[3]);
          mCal = std::stod(toks[4]);
        }
      }
      else if (toks.size() == 8) {
        for (int i = 0; i < 8; ++i) {
          bin.push_back(std::stof(toks[i]));
        }
      }
    }
    file.close();
    return 1;
  }

  int Spectrum::readLST(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
      return 0;
    }
    canali = 2048;
    bin.clear();
    bin.resize(canali, 0);
    std::string token;
    do {
      file >> token;
      if (token.compare("#StartTime:") == 0) {
        file >> token;
        std::vector <std::string> dateToks = tls::split(token, 'T');
        dataSpt = Data(dateToks[0], dateToks[1], '-', ':', 'B');
      }
    } while (token != "Gain");

    long timeTok;
    double gainTok;
    int energyTok;
    while (file >> timeTok >> energyTok >> gainTok) {
      ++bin[energyTok];
    }
    dT = timeTok * 16E-9;
    file.close();
    return 1;
  }


  Spectrum& Spectrum::operator+=(const Spectrum& rhs) {
    for (int i = 0; i < canali; ++i) {
      bin[i] += rhs.bin[i];
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
    }
    return *this;
  }

  Spectrum operator-(Spectrum lhs, const Spectrum& rhs) {
    lhs -= rhs;
    return lhs;
  }

  Spectrum& Spectrum::binCut(int from, int to) {
    if (from > to) {
      int tmp = from;
      from = to;
      to = tmp;
    }
    for (int i = 0; i < canali; ++i) {
      if (i < from || i > to) {
        bin[i] = 0;
      }
    }
    return *this;
  }

  Spectrum& Spectrum::energyCut(float from, float to) {
    return binCut(energyToBin(from), energyToBin(to));
  }

  int Spectrum::energyToBin(double en) const {
    return (en - qCal) / mCal;
  }

  Spectrum& Spectrum::append(const Spectrum& toApp) {
    *this += toApp;
    if (dataSpt > toApp.dataSpt) {
      dataSpt = toApp.dataSpt;
    }
    dT += toApp.dT;
    return *this;
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

  double Spectrum::getCounts(double e1) const {
    return getBinContent(energyToBin(e1));
  }

  double Spectrum::getCounts(double e1, double e2) const {
    return getBinContent(energyToBin(e1), energyToBin(e2));
  }

  double Spectrum::getCps(double e1) const {
    return getCounts(e1) / dT;
  }

  double Spectrum::getCps(double e1, double e2) const {
    return getCounts(e1, e2) / dT;
  }

  double Spectrum::getBinContent(int b1) const {
    return bin[b1];
  }

  double Spectrum::getBinContent(int b1, int b2) const {
    double integral = 0;
    for(int i = b1; i <= b2; ++i) {
      integral += bin[i];
    }
    return integral;
  }

  void Spectrum::writeSPT(const char * nomeFile) {
    using namespace std;
    ofstream outfile;
    outfile.open(nomeFile);
    if (outfile) {
      outfile << canali << " " << dT;
      if (mCal != 1) {
        outfile << " true ";
      }
      else {
        outfile << " false ";
      }
      outfile << qCal << " " << mCal << endl;
      outfile << "# S_TIME: 000 " << dataSpt.toString('-', ':', 'B') << endl;
      outfile << "# " << dataSpt.toString('-', ':', 'B') << "# DET # Spectrum.cpp" << endl;
      for (int i = 0; i < canali; ++i) {
        outfile << bin[i];
        if (i % 8 == 7) {
          outfile << endl;
        }
        else {
          outfile << " ";
        }
      }
    }
    outfile.close();
  }

  void Spectrum::writeSPE(const char * nomeFile) {
    using namespace std;
    ofstream outfile;
    outfile.open(nomeFile);
    if (outfile) {
      outfile << "$SPEC_ID:\nSpectrum.cpp\n";
      outfile << "$DATE_MEA:\n" << dataSpt.toString('-', ':', 'B') << endl;
      outfile << "$MEAS_TIM:\n" << dT << " " << dT << endl;
      outfile << "$DATA:\n" << 0 << " " << canali - 1 << endl;
      for (int i = 0; i < canali; ++i) {
        outfile << bin[i] << endl;
      }
    }
    outfile.close();
  }

  Data Spectrum::getDate() {
    return dataSpt;
  }

  float Spectrum::getdT() {
    return dT;
  }

  void Spectrum::printContent() {
    std::cout << dataSpt.toString() << std::endl;
    std::cout << "Canali: " << canali << std::endl;
    std::cout << "m: " << mCal << std::endl;
    std::cout << "q: " << qCal << std::endl;
    std::cout << "dT: " << dT << std::endl;
    std::cout << "Primo bin: " << bin[0] << std::endl;
    std::cout << "Ultimo bin: " << bin[canali - 1] << std::endl;
  }

}
