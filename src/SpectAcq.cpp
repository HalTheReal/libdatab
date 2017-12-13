#include "SpectAcq.h"

namespace physics {

  SpectAcq::SpectAcq(const std::vector <int> &hist, const Data &date, float t)
    : Spectrum(hist)
    , dataSp(date)
    , dT(t)
  {}

  SpectAcq::SpectAcq(const std::vector <float> &hist, const Data &date, float t)
    : Spectrum(hist)
    , dataSp(date)
    , dT(t)
  {}

  SpectAcq::SpectAcq(const std::vector <double> &hist, const Data &date, float t)
    : Spectrum(hist)
    , dataSp(date)
    , dT(t)
  {}

  float SpectAcq::getDT() const {
    return dT;
  }

  Data SpectAcq::getDate() const {
    return dataSp;
  }

  SpectAcq readSPE(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    std::vector <double> bin;
    Data dataSpt;
    float dT;
    std::string riga;
    while (file >> riga) {
      if (riga.compare("$DATE_MEA:") == 0) {
        std::string dtStr, hrStr;
        file >> dtStr >> hrStr;
        dataSpt = Data(dtStr, hrStr, '-', ':', 'B');
      }
      else if (riga.compare("$SPEC_ID:") == 0) {
        file >> riga;
      }
      else if (riga.compare("$MEAS_TIM:") == 0) {
        file >> dT >> dT;
      }
      else if (riga.compare("$DATA:") == 0) {
        int fstChn, lstChn;
        file >> fstChn >> lstChn;
        bin.reserve(lstChn + 1);
      }
      else {
        bin.push_back(stof(riga));
      }
    }
    file.close();
    SpectAcq ret(bin, dataSpt, dT);
    return ret;
  }

  SpectAcq readSPT(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    std::vector <double> bin;
    Data acqDate;
    float dT;
    double qS, mS;
    std::string riga;
    while (std::getline(file,riga)) {
      std::vector <std::string> toks = tls::splitWhite(riga);
      if (toks.size() == 5) {
        if (toks[1].compare("S_TIME:") == 0) {
          acqDate = Data(toks[3], toks[4], '-', ':', 'B');
        }
        else {
          dT = std::stof(toks[1]);
          qS = std::stod(toks[3]);
          mS = std::stod(toks[4]);
        }
      }
      else if (toks.size() == 8) {
        for (int i = 0; i < 8; ++i) {
          bin.push_back(std::stof(toks[i]));
        }
      }
    }
    file.close();
    SpectAcq ret(bin, acqDate, dT);
    ret.calibrateWith(mS, qS);
    return ret;
  }

  SpectAcq readLST(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    std::vector <int> bin(2048, 0);
    Data acqDate;
    float dT;
    std::string token;
    do {
      file >> token;
      if (token.compare("#StartTime:") == 0) {
        file >> token;
        std::vector <std::string> dateToks = tls::split(token, 'T');
        acqDate = Data(dateToks[0], dateToks[1], '-', ':', 'B');
      }
    } while (token != "Gain");

    long timeTok;
    double gainTok;
    int energyTok;
    while (file >> timeTok >> energyTok >> gainTok) {
      ++bin[energyTok];
    }
    file.close();
    dT = timeTok * 16E-9;
    SpectAcq ret(bin, acqDate, dT);
    return ret;
  }

  void writeSPE(const SpectAcq &sp, const char * nomeFile) {
    std::ofstream outfile;
    outfile.open(nomeFile);
    if (outfile) {
      outfile << "$SPEC_ID:\nSpectrum.cpp\n";
      outfile << "$DATE_MEA:\n" << sp.getDate().toString('-', ':', 'B') << '\n';
      outfile << "$MEAS_TIM:\n" << sp.getDT() << " " << sp.getDT() << '\n';
      outfile << "$DATA:\n" << 0 << " " << sp.channels() - 1 << '\n';
      for (int i = 0; i < sp.channels(); ++i) {
        outfile << sp.binAt(i) << '\n';
      }
    }
    outfile.close();
  }

  void writeSPT(const SpectAcq &sp, const char * nomeFile) {
    std::ofstream outfile;
    outfile.open(nomeFile);
    if (outfile) {
      outfile << sp.channels() << " " << sp.getDT();
      if (sp.getM() != 1) {
        outfile << " true ";
      }
      else {
        outfile << " false ";
      }
      outfile << sp.getQ() << " " << sp.getM() << '\n';
      outfile << "# S_TIME: 000 " << sp.getDate().toString('-', ':', 'B') << '\n';
      outfile << "# " << sp.getDate().toString('-', ':', 'B') << "# DET # Spectrum.cpp" << '\n';
      for (int i = 0; i < sp.channels(); ++i) {
        outfile << sp.binAt(i);
        if (i % 8 == 7) {
          outfile << '\n';
        }
        else {
          outfile << " ";
        }
      }
    }
    outfile.close();
  }

}
