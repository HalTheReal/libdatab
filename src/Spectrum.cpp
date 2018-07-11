#include "Spectrum.h"

namespace Spectrometry {

  Spectrum::Spectrum()
      : canali(0)
      , mCal(1)
      , qCal(0)
      , startTime()
      , dT(0)
  {}

  double Spectrum::getM() const {
    return mCal;
  }

  double Spectrum::getQ() const {
    return qCal;
  }

  Spectrum& Spectrum::calibrateWith(double m, double q) {
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

  int Spectrum::channels() const {
    return canali;
  }

  Epoch::DateTime Spectrum::getDateTime() const {
    return startTime;
  }

  double Spectrum::getDT() const {
    return dT;
  }

  int energyToBin(double m, double q, double en) {
    return round((en - q) / m);
  }

  int energyToBin(const Spectrum &sp, double en) {
    return energyToBin(sp.getM(), sp.getQ(), en);
  }

  double binToEnergy(double m, double q, int bin) {
    return (bin * m) + q;
  }

  double binToEnergy(const Spectrum &sp, int bin) {
    return binToEnergy(sp.getM(), sp.getQ(), bin);
  }

  Spectrum sum(const Spectrum &sp1, const Spectrum &sp2) {
    if (sp1.channels() != sp2.channels()) {
      throw std::invalid_argument("Channels must be the same");
    }
    double scale = sp1.getDT() / sp2.getDT();
    std::vector <double> newBins(sp1.channels(), 0);
    for(int i = 0; i < sp1.channels(); ++i) {
      newBins[i] = sp1.binAt(i) + scale * sp2.binAt(i);
    }
    Spectrum ret(newBins, sp1.getDateTime(), sp1.getDT());
    ret.calibrateWith(sp1.getM(), sp1.getQ());
    return ret;
  }

  Spectrum subtract(const Spectrum &sp1, const Spectrum &sp2) {
    if (sp1.channels() != sp2.channels()) {
      throw std::invalid_argument("Channels must be the same");
    }
    double scale = sp1.getDT() / sp2.getDT();
    std::vector <double> newBins(sp1.channels(), 0);
    for(int i = 0; i < sp1.channels(); ++i) {
      newBins[i] = sp1.binAt(i) - scale * sp2.binAt(i);
      if (newBins[i] < 0) {
        newBins[i] = 0;
      }
    }
    Spectrum ret(newBins, sp1.getDateTime(), sp1.getDT());
    ret.calibrateWith(sp1.getM(), sp1.getQ());
    return ret;
  }

  Spectrum readSPE(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    std::vector <double> bin;
    Epoch::DateTime dataSpt;
    float dT;
    std::string riga;
    while (file >> riga) {
      if (riga.compare("$DATE_MEA:") == 0) {
        int yr, mn, dy;
        char sep;
        Epoch::Time tm;
        file >> yr >> sep >> mn >> sep >> dy;
        file >> tm;
        Epoch::Date dt(dy, mn, yr);
        dataSpt = Epoch::DateTime(dt, tm);
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
    Spectrum ret(bin, dataSpt, dT);
    return ret;
  }

  Spectrum readSPT(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    int channels;
    float dT;
    double qS, mS;
    std::string riga;
    file >> channels >> dT >> riga >> qS >> mS;

    Epoch::Time acqTime;
    char sep;
    int yr, mn, dy;
    file >> sep >> riga >> riga >> yr >> sep >> mn >> sep >> dy >> acqTime;
    Epoch::Date acqDate(dy, mn, yr);
    file.ignore(1, '\n');
    file.ignore(256, '\n');

    std::vector <double> bin;
    bin.reserve(channels);
    double events;
    while (file >> events) {
      bin.push_back(events);
    }
    file.close();
    Spectrum ret(bin, Epoch::DateTime(acqDate, acqTime), dT);
    ret.calibrateWith(mS, qS);
    return ret;
  }

  Spectrum readLST(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    std::vector <int> bin(2048, 0);
    Epoch::DateTime acqDate;
    float dT;
    std::string token;
    do {
      file >> token;
      if (token.compare("#StartTime:") == 0) {
        int yr, mn, dy;
        char sep;
        Epoch::Time tm;
        file >> yr >> sep >> mn >> sep >> dy >> sep >> tm;
        Epoch::Date dt(dy, mn, yr);
        acqDate = Epoch::DateTime (dt, tm);
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
    Spectrum ret(bin, acqDate, dT);
    return ret;
  }

  void writeSPE(const Spectrum &sp, const char * nomeFile) {
    std::ofstream outfile;
    outfile.open(nomeFile);
    if (outfile) {
      outfile << "$SPEC_ID:\nSpectrum.cpp\n";
      outfile << "$DATE_MEA:\n";
      Epoch::DateTime dtt = sp.getDateTime();
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

  void writeSPT(const Spectrum &sp, const char * nomeFile) {
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
      Epoch::DateTime dtt = sp.getDateTime();
      outfile << dtt.year() << '-' << dtt.month() << '-' << dtt.day() << ' ';
      outfile << toTime(dtt) << '\n';
      outfile << "# ";
      outfile << dtt.year() << '-' << dtt.month() << '-' << dtt.day() << ' ';
      outfile << toTime(dtt) << "# DET # Spectrum.cpp" << '\n';

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
