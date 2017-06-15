#include "GSList.h"

GSList::GSList()
  : dT(0)
  , dataGS(0)
{}

int GSList::readFile(const char * nomeFile) {
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
    std::vector <std::string> toks;
    splitWhite(riga, toks);
    if (toks.size() == 3 && toks[0][0] != '#') {
      dT = stol(toks[0]) * 16E-9;
      clk.push_back(stol(toks[0]));
      event.push_back(stod(toks[1]));
    }
    else if (toks[0].compare("#StartTime:") == 0) {
      std::vector <std::string> dateToks = split(toks[1], 'T');
      dataGS = Data(dateToks[0], dateToks[1], '-', ':', 'B');
    }
  }
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

GSList& GSList::timeCut(int from, int to) {
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
  dT = (clk.back() - clk.front()) * 16E-9;
  return *this;
}

GSList& GSList::timeCut(Data &from, Data &to) {
  int fromInt = from.toUnix() - dataGS.toUnix();
  int toInt = from.toUnix() - to.toUnix();
  return timeCut(fromInt, toInt);
}

GSList& GSList::append(const GSList & toApp) {
  if (dataGS < toApp.dataGS) {
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
    dataGS = toApp.dataGS;
  }
  dT += toApp.dT;
  return *this;
}

void GSList::splitWhite(const std::string &toSplit, std::vector <std::string> &res) {
  char sp = ' ';
  char tb = '\t';
  const char *str = toSplit.c_str();
  do
  {
    const char *begin = str;
    while ((*begin == sp || *begin == tb) && *begin) {
      begin++;
    }
    str = begin;
    while (*str != sp && *str != tb && *str) {
      str++;
    }
    res.push_back(std::string(begin, str));
  } while (0 != *str++);
}

// Splitta la stringa di input in base al carattere c di separazione fornito
// in un vettore. Successioni di caratteri c saranno scartate
std::vector <std::string> GSList::split(std::string toSplit, char c) {
  std::vector <std::string> result;
  const char *str = toSplit.c_str();
  do
  {
    const char *begin = str;
    while (*begin == c && *begin) {
      begin++;
    }
    str = begin;
    while (*str != c && *str) {
      str++;
    }
    result.push_back(std::string(begin, str));
  } while (0 != *str++);
  return result;
}

