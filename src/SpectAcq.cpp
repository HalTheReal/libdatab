#include "SpectAcq.h"

namespace Spectrometry {

  SpectAcq::SpectAcq(const std::vector <int> &hist, const Chrono::DateTime &date, float t)
    : Spectrum(hist)
    , dataSp(date)
    , dT(t)
  {}

  SpectAcq::SpectAcq(const std::vector <float> &hist, const Chrono::DateTime &date, float t)
    : Spectrum(hist)
    , dataSp(date)
    , dT(t)
  {}

  SpectAcq::SpectAcq(const std::vector <double> &hist, const Chrono::DateTime &date, float t)
    : Spectrum(hist)
    , dataSp(date)
    , dT(t)
  {}

  float SpectAcq::getDT() const {
    return dT;
  }

  Chrono::DateTime SpectAcq::getDateTime() const {
    return dataSp;
  }

  double cps(const SpectAcq &sp, double en1) {
    return (counts(sp, en1) / sp.getDT());
  }

  double cps(const SpectAcq &sp, double en1, double en2) {
    return (counts(sp, en1, en2) / sp.getDT());
  }

  SpectAcq readSPE(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    std::vector <double> bin;
    Chrono::DateTime dataSpt;
    float dT;
    std::string riga;
    while (file >> riga) {
      if (riga.compare("$DATE_MEA:") == 0) {
        int yr, mn, dy;
        char sep;
        Chrono::Time tm;
        file >> yr >> sep >> mn >> sep >> dy;
        file >> tm;
        Chrono::Date dt(dy, mn, yr);
        dataSpt = Chrono::DateTime(dt, tm);
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
    int channels;
    float dT;
    double qS, mS;
    std::string riga;
    file >> channels >> dT >> riga >> qS >> mS;

    Chrono::Time acqTime;
    char sep;
    int yr, mn, dy;
    file >> sep >> riga >> riga >> yr >> sep >> mn >> sep >> dy >> acqTime;
    Chrono::Date acqDate(dy, mn, yr);
    file.ignore(1, '\n');
    file.ignore(256, '\n');

    std::vector <double> bin;
    bin.reserve(channels);
    double events;
    while (file >> events) {
      bin.push_back(events);
    }
    file.close();
    SpectAcq ret(bin, Chrono::DateTime(acqDate, acqTime), dT);
    ret.calibrateWith(mS, qS);
    return ret;
  }

  SpectAcq readLST(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    std::vector <int> bin(2048, 0);
    Chrono::DateTime acqDate;
    float dT;
    std::string token;
    do {
      file >> token;
      if (token.compare("#StartTime:") == 0) {
        int yr, mn, dy;
        char sep;
        Chrono::Time tm;
        file >> yr >> sep >> mn >> sep >> dy >> sep >> tm;
        Chrono::Date dt(dy, mn, yr);
        acqDate = Chrono::DateTime (dt, tm);
      }
    } while (token.compare("Gain") != 0);

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
      outfile << "$DATE_MEA:\n";
      Chrono::DateTime dtt = sp.getDateTime();
      outfile << dtt.year() << '-' << dtt.month() << '-' << dtt.day() << ' ';
      outfile << dtt.hour() << ':' << dtt.min() << ':' << dtt.sec() << '\n';
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
      outfile << "# S_TIME: 000 ";
      Chrono::DateTime dtt = sp.getDateTime();
      outfile << dtt.year() << '-' << dtt.month() << '-' << dtt.day() << ' ';
      outfile << toTime(dtt) << '\n';
      outfile << "# ";
      outfile << dtt.year() << '-' << dtt.month() << '-' << dtt.day() << ' ';
      outfile << toTime(dtt) << "# DET # Spectrum.cpp" << '\n';

      std::cout << "Canali letti: " << sp.channels() << '\n';
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
