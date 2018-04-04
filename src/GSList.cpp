#include "GSList.h"

GSList::GSList()
  : dT(0)
  , dataGS()
{}

GSList::GSList(const char * nomeFile)
  : dT(0)
  , dataGS()
{
  readFile(nomeFile);
}

Spectrometry::SpectAcq GSList::toSpectrum() const {
  std::vector <int> bin(2048, 0);
  for (int val : this->event) {
    ++bin[val];
  }
  return Spectrometry::SpectAcq(bin, this->dataGS, this->dT);
}

int GSList::readFile(const char * nomeFile) {
  std::string filename(nomeFile);
  std::string::size_type idx;

  idx = filename.rfind('.');
  try {
    if(idx != std::string::npos) {
      std::string extension = filename.substr(idx+1);
      if (extension.compare("txt") == 0) {
        return readLST(nomeFile);
      }
    }
    std::cout << nomeFile << " is not a valid file!\n";
  }
  catch (std::invalid_argument& e) {
    std::cout << "Invalid argument exception while reading file:\n";
    std::cout << nomeFile << '\n';
    defaultInit();
  }
  catch (std::out_of_range& e) {
    std::cout << "Out of range exception while reading file:\n";
    std::cout << nomeFile << '\n';
    defaultInit();
  }
  return 0;
}

void GSList::defaultInit() {
  dT = 0;
  dataGS = Epoch::DateTime();
  event.clear();
  clk.clear();
}

bool GSList::isEmpty() const {
  if (clk.size() == 0 || event.size() == 0) {
    return true;
  }
  return false;
}

int GSList::readLST(const char * nomeFile) {
  std::ifstream file(nomeFile);
  if (!file) {
    fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
    return 0;
  }
  clk.clear();
  event.clear();

  std::string token;
  do {
    file >> token;
    if (token.compare("#StartTime:") == 0) {
      int yr, mn, dy;
      char sep;
      Epoch::Time tm;
      file >> yr >> sep >> mn >> sep >> dy >> sep >> tm;
      Epoch::Date dt(dy, mn, yr);
      dataGS = Epoch::DateTime (dt, tm);
    }
  } while (token.compare("Gain") != 0);
  long timeTok;
  double gainTok;
  int energyTok;
  while (file >> timeTok >> energyTok >> gainTok) {
    clk.push_back(timeTok);
    event.push_back(energyTok);
  }
  dT = timeTok * 16E-9;
  file.close();
  return 1;
}

void GSList::writeLST(const char * nomeFile) {
  using namespace std;
  ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << "#GammaStream 1.0 LIST\n";
    outfile << "#StartTime: ";
    outfile << dataGS.year() << '-' << dataGS.month() << '-';
    outfile << dataGS.day() << 'T' << Epoch::toTime(dataGS) << '\n';
    outfile << "#Fields: Time\tEnergy\tGain" << '\n';
    for (unsigned i = 0; i < clk.size(); ++i) {
      outfile << right << setw(9) << clk[i] << '\t';
      outfile << setw(6) << event[i] << '\t';
      outfile << "1.000" << '\n';
    }
  }
}

GSList& GSList::timeCut(int from, int to) {
  if (from < 0) {
    from = 0;
  }
  std::vector <long> tClock;
  std::vector <int> tEvent;
  for (unsigned i = 0; i < clk.size(); i++) {
    if (clk[i] * 16E-9 >= from && clk[i] * 16E-9 <= to) {
      tClock.push_back(clk[i] - (from / 16E-9));
      tEvent.push_back(event[i]);
    }
  }
  event = tEvent;
  clk = tClock;
  dataGS.addSec(from);
  if (clk.size() != 0) {
    dT = clk.back() * 16E-9;
  }
  else {
    dT = 0;
  }
  return *this;
}

GSList& GSList::timeCut(const Epoch::DateTime &from, int to) {
  Epoch::DateTime toDT = from;
  toDT.addSec(to);
  return timeCut(from, toDT);
}

GSList& GSList::timeCut(const Epoch::DateTime &from, const Epoch::DateTime &to) {
  int fromInt = toUnix(from) - toUnix(dataGS);
  int toInt = toUnix(to) - toUnix(dataGS);
  return timeCut(fromInt, toInt);
}

GSList& GSList::append(const GSList & toApp) {
  if (dataGS > toApp.dataGS) {
    std::cout << "Unable to append!\n";
    return *this;
  }
  event.insert(event.end(), toApp.event.begin(), toApp.event.end());
  long last = (toUnix(toApp.dataGS) - toUnix(dataGS)) / 16E-9;
  for (long val : toApp.clk) {
    clk.push_back(val + last);
  }
  dT += toApp.dT;
  return *this;
}

float GSList::getDT() const {
  return dT;
}

Epoch::DateTime GSList::getDateTime() const {
  return dataGS;
}
