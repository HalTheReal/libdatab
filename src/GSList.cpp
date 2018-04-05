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

Spectrometry::SpectAcq GSList::toSpectrum() const {
  std::vector <int> bin(2048, 0);
  for (auto &pair : this->evtList) {
    ++bin[pair.second];
  }
  return Spectrometry::SpectAcq(bin, this->dataGS, getDT());
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
  std::cerr << "Size: " << copied.size() << '\n';
  Epoch::DateTime dtt = dataGS;
  if(from > 0) {
    dtt.addSec(from);
    for(auto &pair : copied) {
      pair.first -= from / 16E-9;
    }
  }
  return GSList(copied, dtt);
}

void GSList::writeLST(const char * nomeFile) const {
  using namespace std;
  ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << "#GammaStream 1.0 LIST\n";
    outfile << "#StartTime: ";
    outfile << dataGS.year() << '-' << dataGS.month() << '-';
    outfile << dataGS.day() << 'T' << Epoch::toTime(dataGS) << '\n';
    outfile << "#Fields: Time\tEnergy\tGain" << '\n';
    for (auto &pair : this->evtList) {
      outfile << right << setw(9) << pair.first << '\t';
      outfile << setw(6) << pair.second << '\t';
      outfile << "1.000" << '\n';
    }
  }
}

double GSList::getDT() const {
  return evtList.back().first * 16E-9;
}

Epoch::DateTime GSList::getDateTime() const {
  return dataGS;
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

void writeSPE(const GSList &lst, const char * nomeFile) {
  return writeSPE(lst.toSpectrum(), nomeFile);
}

void writeSPT(const GSList &lst, const char * nomeFile) {
  return writeSPT(lst.toSpectrum(), nomeFile);
}

bool isLess(std::pair <long, int> pair, long val) {
  return (pair.first < val);
}

}
