#include "Spettro.h"

// Costruttore
Spettro::Spettro()
  : dT(0)
  , canali(0)
  , mCal(1)
  , qCal(0)
  , dataSpt(0)
{}

int Spettro::readFile(const char * nomeFile) {
  std::string filename(nomeFile);
  std::string::size_type idx;

  idx = filename.rfind('.');

  if(idx != std::string::npos)
  {
    std::string extension = filename.substr(idx+1);
    if (extension.compare("spt") == 0) {
      return readSPT(nomeFile);
    }
    else if (extension.compare("spe") == 0) {
      return readSPE(nomeFile);
    }
  }
  else {
    std::cout << nomeFile << " is not a valid file!\n";
  }
  return 0;
}

int Spettro::readSPE(const char * nomeFile) {
  std::ifstream file(nomeFile);
  if (!file) {
    fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
    return 0;
  }
  bin.clear();
  std::string riga;
  while (std::getline(file,riga)) {
    if (riga[0] == '$') {
      std::vector <std::string> toks;
      if (riga.compare("$DATE_MEA:") == 0) {
        std::getline(file, riga);
        splitWhite(riga, toks);
        dataSpt = Data(toks[0], toks[1], '-', ':', 'B');
      }
      else if (riga.compare("$MEAS_TIM:") == 0) {
        std::getline(file, riga);
        splitWhite(riga, toks);
        dT = std::stof(toks[0]);
      }
      else if (riga.compare("$DATA:") == 0) {
        std::getline(file, riga);
        splitWhite(riga, toks);
        canali = std::stod(toks[1]) + 1;
      }
      else {
        std::getline(file, riga);
      }
    }
    else {
      bin.push_back(stof(riga));
    }
  }
  file.close();
  return 1;
}

int Spettro::readSPT(const char * nomeFile) {
  std::ifstream file(nomeFile);
  if (!file) {
    fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
    return 0;
  }
  bin.clear();
  std::string riga;
  while (std::getline(file,riga)) {
    std::vector <std::string> toks;
    splitWhite(riga, toks);
    if (toks.size() == 5) {
      if (toks[1].compare("S_Time:") == 0) {
        dataSpt = Data(toks[3], toks[4], '-', ':', 'B');
      }
      else {
        canali = std::stod(toks[0]);
        dT = std::stof(toks[1]);
        qCal = std::stod(toks[3]);
        mCal = std::stod(toks[4]);
      }
    }
    else if (toks.size() == 8) {
      for (int i = 0; i < 8; ++i) {
        bin.push_back(std::stof(toks[i]));
      }
    }
  }
  file.close();
  return 1;
}


Spettro& Spettro::operator+=(const Spettro& rhs) {
  for (int i = 0; i < canali; ++i) {
    bin[i] += rhs.bin[i];
  }
  if (dataSpt > rhs.dataSpt) {
    dataSpt = rhs.dataSpt;
  }
  dT += rhs.dT;
  return *this;
}

Spettro operator+(Spettro lhs, const Spettro& rhs) {
  lhs += rhs;
  return lhs;
}

Spettro Spettro::binCut(int from, int to) {
  if (from < to) {
    int tmp = from;
    from = to;
    to = tmp;
  }
  Spettro spt = *this;
  for (int i = 0; i < spt.canali; ++i) {
    if (i < from || i > to) {
      spt.bin[i] = 0;
    }
  }
  return spt;
}

Spettro Spettro::energyCut(float from, float to) {
  return binCut(energyToBin(from), energyToBin(to));
}

int Spettro::energyToBin(float en) {
  return (en - qCal) / mCal;
}

void Spettro::writeSPT(const char * nomeFile) {
}

void Spettro::writeSPE(const char * nomeFile) {
}

void Spettro::splitWhite(const std::string &toSplit, std::vector <std::string> &res) {
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
std::vector <std::string> Spettro::split(std::string toSplit, char c) {
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

Data Spettro::getDate() {
  return dataSpt;
}

float Spettro::getdT() {
  return dT;
}

void Spettro::printContent() {
  std::cout << dataSpt.toString() << std::endl;
  std::cout << "Canali: " << canali << std::endl;
  std::cout << "m: " << mCal << std::endl;
  std::cout << "q: " << qCal << std::endl;
  std::cout << "dT: " << dT << std::endl;
  std::cout << "Primo bin: " << bin[0] << std::endl;
  std::cout << "Ultimo bin: " << bin[canali - 1] << std::endl;
}
