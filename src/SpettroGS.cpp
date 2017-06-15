#include "SpettroGS.h"

SpettroGS::SpettroGS()
  : Spettro()
{}

int SpettroGS::readFile(const char * nomeFile) {
  std::string filename(nomeFile);
  std::string::size_type idx;

  idx = filename.rfind('.');

  if(idx != std::string::npos)
  {
    std::string extension = filename.substr(idx+1);
    if (extension.compare("txt") == 0) {
      return readLST(nomeFile);
    }
  }
  std::cout << nomeFile << " is not a valid file!\n";
  return 0;
}

int SpettroGS::readLST(const char * nomeFile) {
  std::ifstream file(nomeFile);
  if (!file) {
    fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
    return 0;
  }
  clk.clear();
  event.clear();
  bin.clear();

  canali = 2048;
  bin.resize(canali, 0);

  std::string riga;
  while (std::getline(file,riga)) {
    std::vector <std::string> toks;
    splitWhite(riga, toks);
    if (toks.size() == 3 && toks[0][0] != '#') {
      dT = stol(toks[0]) * 16E-9;
      ++bin[stoi(toks[1])];
      clk.push_back(stol(toks[0]));
      event.push_back(stod(toks[1]));
    }
    else if (toks[0].compare("#StartTime:") == 0) {
      std::vector <std::string> dateToks = split(toks[1], 'T');
      dataSpt = Data(dateToks[0], dateToks[1], '-', ':', 'B');
    }
  }
  file.close();
  return 1;
}

void SpettroGS::writeLST(const char * nomeFile) {
  using namespace std;
  ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << "#GammaStream 1.0 LIST\n";
    outfile << "#StartTime: ";
    outfile << dataSpt.dateToString('-','B') << "T";
    outfile << dataSpt.hourToString() << endl;
    outfile << "#Fields\tTime\tEnergy\tGain" << endl;
    for (unsigned i = 0; i < clk.size(); ++i) {
      outfile << right << setw(9) << clk[i] << '\t';
      outfile << setw(6) << event[i] << '\t';
      outfile << "1.000" << endl;
    }
  }
}

SpettroGS& SpettroGS::updateBin() {
  bin.resize(canali, 0);
  for (int val : event) {
    ++bin[val];
  }
  return *this;
}

SpettroGS& SpettroGS::timeCut(int from, int to) {
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
  dataSpt.sum(from);
  dT = (clk.back() - clk.front()) * 16E-9;
  updateBin();
  return *this;
}

SpettroGS& SpettroGS::timeCut(Data &from, Data &to) {
  int fromInt = from.toUnix() - dataSpt.toUnix();
  int toInt = from.toUnix() - to.toUnix();
  return timeCut(fromInt, toInt);
}

SpettroGS& SpettroGS::append(const SpettroGS & toApp) {
  if (dataSpt < toApp.dataSpt) {
    event.insert(event.end(), toApp.event.begin(), toApp.event.end());
    long last = clk.back();
    for (long val : toApp.clk) {
      clk.push_back(val + last);
    }
  }
  else {
    event.insert(event.begin(), toApp.event.begin(), toApp.event.end());
    std::vector <long> tmp;
    tmp = clk;
    clk = toApp.clk;
    long last = clk.back();
    for (long val : tmp) {
      clk.push_back(val + last);
    }
  }
  this->Spettro::append(toApp);
  return *this;
}
