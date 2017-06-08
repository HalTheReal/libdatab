#include "Spettro.h"

SpettroGS operator+(SpettroGS lhs, const SpettroGS& rhs) {
  lhs += rhs;
  return lhs;
}

// Costruttore
SpettroGS::SpettroGS() : dataSpt(0) {
  dT = 0;
  canali = 0;
}

int SpettroGS::readFile(const char * nomeFile) {
  std::ifstream file(nomeFile);
  if (!file) {
    fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
    return 1;
  }
  dT = 0;
  canali = 0;
  nsec.clear();
  eventE.clear();

  std::string riga;
  while (std::getline(file,riga)) {
    std::vector <std::string> toks;
    splitWhite(riga, toks);
    if (toks.size() == 3 && toks[0][0] != '#') {
      nsec.push_back(stol(toks[0]));
      eventE.push_back(stod(toks[1]));
    }
    else if (toks[0].compare("#StartTime:") == 0) {
      readDate(toks[1]);
    }
  }
  file.close();
  dT = (nsec.back() - nsec.front()) * 16E-9;
  return 0;
}

SpettroGS& SpettroGS::operator+=(const SpettroGS& rhs) {
  if (dataSpt < rhs.dataSpt) {
    eventE.insert(eventE.end(), rhs.eventE.begin(), rhs.eventE.end());
    long last = nsec.back();
    for (long val : rhs.nsec) {
      nsec.push_back(val + last);
    }
  }
  else {
    eventE.insert(eventE.begin(), rhs.eventE.begin(), rhs.eventE.end());
    std::vector <long> tmp;
    dataSpt = rhs.dataSpt;
    tmp = nsec;
    nsec = rhs.nsec;
    long last = nsec.back();
    for (long val : tmp) {
      nsec.push_back(val + last);
    }
  }

  dT = (nsec.back() - nsec.front()) * 16E-9;
  return *this;
}

void SpettroGS::readDate(std::string &token) {
  std::vector <std::string> dateToks = split(token, 'T');
  dataSpt = Data(dateToks[0], dateToks[1], '-', ':', 'B');
}

SpettroGS SpettroGS::cut(int from, int to) {
  SpettroGS cutted;
  for (unsigned i = 0; i < nsec.size(); i++) {
    if (nsec[i] * 16E-9 >= from && nsec[i] * 16E-9 <= to) {
      cutted.nsec.push_back(nsec[i] - (from / 16E-9));
      cutted.eventE.push_back(eventE[i]);
    }
  }
  cutted.dataSpt = this->dataSpt;
  cutted.dataSpt.sum(from);
  cutted.dT = (cutted.nsec.back() - cutted.nsec.front()) * 16E-9;
  return cutted;
}

SpettroGS SpettroGS::cut(Data &from, Data &to) {
  int fromInt = from.toUnix() - dataSpt.toUnix();
  int toInt = from.toUnix() - to.toUnix();
  return cut(fromInt, toInt);
}

void SpettroGS::write(const char * nomeFile) {
  using namespace std;
  ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << "#GammaStream 1.0 LIST\n";
    outfile << "#StartTime: ";
    outfile << dataSpt.dateToString('-','B') << "T";
    outfile << dataSpt.hourToString() << endl;
    outfile << "#Fields\tTime\tEnergy\tGain" << endl;
    for (unsigned i = 0; i < nsec.size(); ++i) {
      outfile << right << setw(9) << nsec[i] << '\t';
      outfile << setw(6) << eventE[i] << '\t';
      outfile << "1.000" << endl;
    }
  }
}

void SpettroGS::splitWhite(const std::string &toSplit, std::vector <std::string> &res) {
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
std::vector <std::string> SpettroGS::split(std::string toSplit, char c) {
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

Data SpettroGS::getDate() {
  return dataSpt;
}

float SpettroGS::getdT() {
  return dT;
}

void SpettroGS::printContent() {
  std::cout << "nsec # " << nsec.size() << std::endl;
  std::cout << "evnt # " << eventE.size() << std::endl;
  std::cout << nsec.back() << '\t' << eventE.back() << std::endl;
  std::cout << "dT: " << dT << std::endl;
  std::cout << "Data " << dataSpt.toString() << std::endl;
}
