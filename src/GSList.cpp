#include "GSList.h"
namespace Spectrometry {

GSList::GSList()
  : evtList()
  , dataGS()
{}

GSList::GSList(const std::vector <std::pair <long, int>> &events, const Epoch::DateTime &start)
  : evtList(events)
  , dataGS(start)
{}

long tousec(long sec) {
  return sec / 1E-9;
}

std::vector <int> GSList::getEventHist() const {
  std::vector <int> bin(2048, 0);
  for (auto &pair : this->evtList) {
    ++bin[pair.second];
  }
  return bin;
}

Epoch::DateTime GSList::getDateTime() const {
  return dataGS;
}

double GSList::getDT() const {
  return evtList.back().first * 1E-9;
}

GSList& GSList::erase(long from, long to) {
  if(to < from) {
    std::swap(from, to);
  }
  if(to <= 0) {
    return *this;
  }
  auto eraseFrom = std::lower_bound(evtList.begin(), evtList.end(), tousec(from), isLess);
  auto eraseTo = std::lower_bound(evtList.begin(), evtList.end(), tousec(to), isLess);
  evtList.erase(eraseFrom, eraseTo);
  if(from <= 0) {
    dataGS.addSec(to);
    for(auto &pair : evtList) {
      pair.first -= tousec(to);
    }
  }
  return *this;
}

GSList& GSList::merge(const GSList &gsl) {
  long offset = tousec(std::abs(Epoch::toUnix(dataGS) - Epoch::toUnix(gsl.dataGS)));
  if(dataGS > gsl.dataGS) {
    for(auto &pair : evtList) {
      pair.first += offset;
    }
    for(auto pair : gsl.evtList) {
      evtList.push_back(pair);
    }
    dataGS = gsl.dataGS;
  }
  else {
    for(auto pair : gsl.evtList) {
      pair.first += offset;
      evtList.push_back(pair);
    }
  }
  std::sort(evtList.begin(), evtList.end());
  return *this;
}

GSList GSList::copy(long from, long to) const {
  if(to < from) {
    std::swap(from, to);
  }
  if(to <= 0) {
    return GSList();
  }
  auto copyFrom = std::lower_bound(evtList.begin(), evtList.end(), tousec(from), isLess);
  auto copyTo = std::lower_bound(evtList.begin(), evtList.end(), tousec(to), isLess);
  std::vector <std::pair <long, int>> copied(copyFrom, copyTo);
  Epoch::DateTime dtt = dataGS;
  if(from > 0) {
    dtt.addSec(from);
    for(auto &pair : copied) {
      pair.first -= tousec(from);
    }
  }
  return GSList(copied, dtt);
}

void GSList::writeGSL(const char * nomeFile) const {
  std::ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << "#GammaStream 1.0 LIST\n";
    outfile << "#StartTime: ";
    outfile << dataGS.year() << '-' << dataGS.month() << '-';
    outfile << dataGS.day() << 'T' << Epoch::toTime(dataGS) << '\n';
    outfile << "#Fields: Time\tEnergy\tGain" << '\n';
    for (auto &pair : this->evtList) {
      outfile << std::right << std::setw(9) << pair.first / 16 << '\t';   // 1 ciclo clock = 16us
      outfile << std::setw(6) << pair.second << '\t';
      outfile << "1.000" << '\n';
    }
  }
}

Spectrometry::Spectrum toSpectrum(const GSList &gsl) {
  return Spectrometry::Spectrum(gsl.getEventHist(), gsl.getDateTime(), gsl.getDT());
}

void writeSPE(const GSList &lst, const char * nomeFile) {
  return writeSPE(toSpectrum(lst), nomeFile);
}

void writeSPT(const GSList &lst, const char * nomeFile) {
  return writeSPT(toSpectrum(lst), nomeFile);
}

GSList readGSL(const char * nomeFile) {
  std::ifstream file(nomeFile);
  if (!file) {
    throw std::invalid_argument("Unable to open file!");
  }
  std::vector <std::pair <long, int>> coppie;
  Epoch::DateTime dataGS;

  std::string token;
  do {
    file >> token;
    if (token.compare("#StartTime:") == 0) {
      int yr, mn, dy;
      char sep;
      Epoch::Time tm;
      file >> yr >> sep >> mn >> sep >> dy >> sep >> tm;
      Epoch::Date dt(dy, mn, yr);
      dataGS = Epoch::DateTime(dt, tm);
    }
  } while (token.compare("Gain") != 0);
  long timeTok;
  double gainTok;
  int energyTok;
  while (file >> timeTok >> energyTok >> gainTok) {
    coppie.push_back(std::make_pair(timeTok * 16, energyTok));  // 1 ciclo clock = 16us
  }
  file.close();
  return GSList(coppie, dataGS);
}

bool isLess(std::pair <long, int> pair, long val) {
  return (pair.first < val);
}

}
