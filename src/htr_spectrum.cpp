#include "htr_spectrum.h"

namespace Spectrometry {

  Spectrum::Spectrum()
      : canali(0)
      , mCal(1)
      , qCal(0)
      , startTime()
      , liveTime(0)
      , realTime(0)
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
    std::vector <double> newBin(canali, 0);
    for (std::size_t i = 0; i < canali; ++i) {
      double E0 = i * mCal + 0.5 * mCal + qCal;
      double Emin = E0 - 0.5 * mCal;
      double Emax = E0 + 0.5 * mCal;
      std::uniform_real_distribution <double> distribution(Emin, Emax);
      for (int j = 0; j < bin[i]; ++j) {
        double rnd = distribution(generator);
        rnd = ceil(rnd / gain);
        if (rnd >= 0 && rnd < canali) {
          ++newBin[rnd];
        }
      }
    }
    mCal = gain;
    qCal = 0;
    bin = newBin;
    return *this;
  }

  double Spectrum::binAt(int b1) const {
    return bin.at(b1);
  }

  std::size_t Spectrum::channels() const {
    return canali;
  }

  Epoch::DateTime Spectrum::getDateTime() const {
    return startTime;
  }

  double Spectrum::getLT() const {
    return liveTime;
  }
  
  double Spectrum::getRT() const {
    return realTime;
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

  Epoch::DateTime getCentroid(const Spectrum &sp) {
    Epoch::DateTime ret = sp.getDateTime();
    int LT = static_cast<int>(std::round(sp.getLT()));
    ret.addSec(LT / 2); // Divisione INTERA!
    return ret;
  }

  Spectrum sum(const Spectrum &sp1, const Spectrum &sp2) {
    if (sp1.channels() != sp2.channels()) {
      throw std::invalid_argument("Channels must be the same");
    }
    double scale = sp1.getLT() / sp2.getLT();
    std::vector <double> newBins(sp1.channels(), 0);
    for(std::size_t i = 0; i < sp1.channels(); ++i) {
      newBins[i] = sp1.binAt(i) + scale * sp2.binAt(i);
    }
    Spectrum ret(newBins, sp1.getDateTime(), sp1.getLT(), sp1.getRT());
    ret.calibrateWith(sp1.getM(), sp1.getQ());
    return ret;
  }

  Spectrum subtract(const Spectrum &sp1, const Spectrum &sp2) {
    if (sp1.channels() != sp2.channels()) {
      throw std::invalid_argument("Channels must be the same");
    }
    double scale = sp1.getLT() / sp2.getLT();
    std::vector <double> newBins(sp1.channels(), 0);
    for(std::size_t i = 0; i < sp1.channels(); ++i) {
      newBins[i] = sp1.binAt(i) - scale * sp2.binAt(i);
      if (newBins[i] < 0) {
        newBins[i] = 0;
      }
    }
    Spectrum ret(newBins, sp1.getDateTime(), sp1.getLT(), sp1.getRT());
    ret.calibrateWith(sp1.getM(), sp1.getQ());
    return ret;
  }

  Spectrum append(const Spectrum &sp1, const Spectrum &sp2) {
    if (sp1.channels() != sp2.channels()) {
      throw std::invalid_argument("Channels must be the same");
    }
    std::vector <double> newBins(sp1.channels(), 0);
    for(std::size_t i = 0; i < sp1.channels(); ++i) {
      newBins[i] = sp1.binAt(i) + sp2.binAt(i);
    }
    double liveTime = sp1.getLT() + sp2.getLT();
    double realTime = sp1.getRT() + sp2.getRT();
    Spectrum ret(newBins, sp1.getDateTime(), liveTime, realTime);
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
    double liveT, realT;
    double q = 0, m = 1;
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
      else if (riga.compare("$MEAS_TIM:") == 0) {
        file >> liveT >> realT;
      }
      else if (riga.compare("$DATA:") == 0) {
        int fstChn, lstChn;
        file >> fstChn >> lstChn;
        bin.reserve(lstChn + 1);
        for(int i = 0; i <= lstChn; ++i) {
          file >> riga;
          bin.push_back(std::stof(riga));
        }
      }
      else if (riga.compare("$ENER_FIT:") == 0) {
        file >> q >> m;
      }
    }
    file.close();
    Spectrum ret(bin, dataSpt, liveT, realT);
    ret.calibrateWith(m, q);
    return ret;
  }

  Spectrum readSPT(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    int channels;
    double LT;
    double qS, mS;
    std::string riga;
    file >> channels >> LT >> riga >> qS >> mS;

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
    Spectrum ret(bin, Epoch::DateTime(acqDate, acqTime), LT);
    ret.calibrateWith(mS, qS);
    return ret;
  }

  Spectrum readTXT(const char * nomeFile) {
    std::ifstream file(nomeFile);
    if (!file) {
      throw std::runtime_error("Unable to open file!");
    }
    std::vector <double> bin;
    double counts;
    while (file >> counts) {
      bin.push_back(counts);
    }
    file.close();
    return Spectrum(bin);
  }

  Spectrum readSPE(const std::string &nomeFile) {
    return readSPE(nomeFile.c_str());
  }

  Spectrum readSPT(const std::string &nomeFile) {
    return readSPT(nomeFile.c_str());
  }

  Spectrum readTXT(const std::string &nomeFile) {
    return readTXT(nomeFile.c_str());
  }

  void writeSPE(const Spectrum &sp, const char * nomeFile) {
    std::ofstream outfile;
    outfile.open(nomeFile);
    if (outfile) {
      outfile << "$SPEC_ID:\nSpectrum.cpp\n";
      outfile << "$DATE_MEA:\n";
      Epoch::DateTime dtt = sp.getDateTime();
      outfile << dtt.year() << '-' << dtt.month() << '-' << dtt.day() << ' ';
      outfile << toTime(dtt) << '\n';
      outfile << "$MEAS_TIM:\n" << sp.getLT() << " " << sp.getRT() << '\n';
      outfile << "$DATA:\n" << 0 << " " << sp.channels() - 1 << '\n';
      for (std::size_t i = 0; i < sp.channels(); ++i) {
        outfile << sp.binAt(i) << '\n';
      }
      outfile << "$ENER_FIT:\n";
      outfile << sp.getQ() << ' ' << sp.getM() << '\n';
    }
    outfile.close();
  }

  void writeSPT(const Spectrum &sp, const char * nomeFile) {
    std::ofstream outfile;
    outfile.open(nomeFile);
    if (outfile) {
      outfile << sp.channels() << " " << sp.getLT();
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

      for (std::size_t i = 0; i < sp.channels(); ++i) {
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

  void writeTXT(const Spectrum &sp, const char * nomeFile) {
    std::ofstream outfile;
    outfile.open(nomeFile);
    if (outfile) {
      for (std::size_t i = 0; i < sp.channels(); ++i) {
        outfile << sp.binAt(i) << '\n';
      }
    }
    outfile.close();
  }

  void writeSPE(const Spectrum &sp, const std::string &str) {
    return writeSPE(sp, str.c_str());
  }

  void writeSPT(const Spectrum &sp, const std::string &str) {
    return writeSPT(sp, str.c_str());
  }

  void writeTXT(const Spectrum &sp, const std::string &str) {
    return writeTXT(sp, str.c_str());
  }

  Spectrum medianFilter(const Spectrum &sp, unsigned width) {
    std::vector<double> bins(sp.channels(), 0);
    for (unsigned i = 0; i < bins.size(); ++i) {
      bins[i] = sp.binAt(i);
    }
    for (unsigned i = 0; i < bins.size(); ++i) {
      auto beg = bins.begin() + i;
      auto lower = beg - width / 2;
      auto upper = beg + width / 2;
      if (i - width / 2 > 0 && i + width / 2 < bins.size()) {
        bins[i] = Stats::median(lower, upper);
      }
    }
    return Spectrum(bins, sp.getDateTime(), sp.getLT(), sp.getRT());
  }

  Spectrum movingAvg(const Spectrum &sp, unsigned width) {
    std::vector<double> bins(sp.channels(), 0);
    for (unsigned i = 0; i < bins.size(); ++i) {
      bins[i] = sp.binAt(i);
    }
    for (unsigned i = 0; i < bins.size(); ++i) {
      auto beg = bins.begin() + i;
      auto lower = beg - width / 2;
      auto upper = beg + width / 2;
      if (i - width / 2 > 0 && i + width / 2 < bins.size()) {
        bins[i] = Stats::mean(lower, upper);
      }
    }
    return Spectrum(bins, sp.getDateTime(), sp.getLT(), sp.getRT());
  }

}
