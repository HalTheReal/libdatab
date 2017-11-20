#include "GSList.h"

GSList::GSList()
  : dT(0)
  , dataGS(0)
{}

GSList::GSList(const char * nomeFile)
  : dT(0)
  , dataGS(0)
{
  readFile(nomeFile);
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

void GSList::defaultInit() {
  dT = 0;
  dataGS = Data(0);
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

  std::string riga;
  while (std::getline(file,riga)) {
    std::vector <std::string> toks = tls::splitWhite(riga);
    if (toks[0].compare("#StartTime:") == 0) {
      std::vector <std::string> dateToks = tls::split(toks[1], 'T');
      dataGS = Data(dateToks[0], dateToks[1], '-', ':', 'B');
    }
    else if (toks[0].compare("#Fields:") == 0) {
      break;
    }
  }
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
    outfile << dataGS.dateToString('-','B') << "T";
    outfile << dataGS.hourToString() << endl;
    outfile << "#Fields\tTime\tEnergy\tGain" << endl;
    for (unsigned i = 0; i < clk.size(); ++i) {
      outfile << right << setw(9) << clk[i] << '\t';
      outfile << setw(6) << event[i] << '\t';
      outfile << "1.000" << endl;
    }
  }
}

void GSList::writeSPE(const char * nomeFile) {
  int canali = 2048;
  std::vector <int> bin(canali, 0);
  for (int val : event) {
    ++bin[val];
  }
  using namespace std;
  ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << "$SPEC_ID:\nGSList.cpp\n";
    outfile << "$DATE_MEA:\n" << dataGS.toString('-', ':', 'B') << endl;
    outfile << "$MEAS_TIM:\n" << round(dT) << " " << round(dT) << endl;
    outfile << "$DATA:\n" << 0 << " " << canali - 1 << endl;
    for (int i = 0; i < canali; ++i) {
      outfile << bin[i] << endl;
    }
  }
  outfile.close();
}

void GSList::writeSPT(const char * nomeFile) {
  int canali = 2048;
  std::vector <int> bin(canali, 0);
  for (int val : event) {
    ++bin[val];
  }
  using namespace std;
  ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << canali << " " << round(dT);
    outfile << " false " << "0 1.0" << endl;
    outfile << "# S_TIME: 000 " << dataGS.toString('-', ':', 'B') << endl;
    outfile << "# " << dataGS.toString('-', ':', 'B') << "# DET # Spettro.cpp" << endl;
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
  dataGS.sum(from);
  if (clk.size() != 0) {
    dT = clk.back() * 16E-9;
  }
  else {
    dT = 0;
  }
  return *this;
}

GSList& GSList::timeCut(Data &from, int to) {
  Data toData = from;
  toData.sum(to);
  return timeCut(from, toData);
}

GSList& GSList::timeCut(Data &from, Data &to) {
  int fromInt = from.toUnix() - dataGS.toUnix();
  int toInt = to.toUnix() - dataGS.toUnix();
  return timeCut(fromInt, toInt);
}

GSList& GSList::append(const GSList & toApp) {
  if (dataGS > toApp.dataGS) {
    std::cout << "Unable to append!\n";
    return *this;
  }
  event.insert(event.end(), toApp.event.begin(), toApp.event.end());
  long last = (toApp.dataGS.toUnix() - dataGS.toUnix()) / 16E-9;
  for (long val : toApp.clk) {
    clk.push_back(val + last);
  }
  dT += toApp.dT;
  return *this;
}

float GSList::getdT() const {
  return dT;
}

Data GSList::getDate() const {
  return dataGS;
}
