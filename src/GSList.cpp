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
  return evtList.back().first * 16E-9;
}

GSList& GSList::erase(long from, long to) {
  if(to < from) {
    std::swap(from, to);
  }
  if(to <= 0) {
    return *this;
  }
  auto eraseFrom = std::lower_bound(evtList.begin(), evtList.end(), from / 16E-9, isLess);
  auto eraseTo = std::lower_bound(evtList.begin(), evtList.end(), to / 16E-9, isLess);
  evtList.erase(eraseFrom, eraseTo);
  if(from <= 0) {
    dataGS.addSec(to);
    for(auto &pair : evtList) {
      pair.first -= to / 16E-9;
    }
  }
  return *this;
}

GSList& GSList::merge(const GSList &gsl) {
  int offset = std::abs(Epoch::toUnix(dataGS) - Epoch::toUnix(gsl.dataGS));
  if(dataGS > gsl.dataGS) {
    for(auto &pair : evtList) {
      pair.first += offset / 16E-9;
    }
    for(auto pair : gsl.evtList) {
      evtList.push_back(pair);
    }
    dataGS = gsl.dataGS;
  }
  else {
    for(auto pair : gsl.evtList) {
      pair.first += offset / 16E-9;
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
  auto copyFrom = std::lower_bound(evtList.begin(), evtList.end(), from / 16E-9, isLess);
  auto copyTo = std::lower_bound(evtList.begin(), evtList.end(), to / 16E-9, isLess);
  std::vector <std::pair <long, int>> copied(copyFrom, copyTo);
  Epoch::DateTime dtt = dataGS;
  if(from > 0) {
    dtt.addSec(from);
    for(auto &pair : copied) {
      pair.first -= from / 16E-9;
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
      outfile << std::right << std::setw(9) << pair.first << '\t';
      outfile << std::setw(6) << pair.second << '\t';
      outfile << "1.000" << '\n';
    }
  }
}

Spectrometry::SpectAcq toSpectrum(const GSList &gsl) {
  return Spectrometry::SpectAcq(gsl.getEventHist(), gsl.getDateTime(), gsl.getDT());
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
    coppie.push_back(std::make_pair(timeTok, energyTok));
  }
  file.close();
  return GSList(coppie, dataGS);
}

bool isLess(std::pair <long, int> pair, long val) {
  return (pair.first < val);
}

}
