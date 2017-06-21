#include "Tabella.h"

// Il costruttore necessita di un nome del file e di un carattere
// separatore
Tabella::Tabella(std::string nomeFile, char separator) {
  currentLine = -1;
  nRighe = -1;
  fileName.assign(nomeFile);
  sep = separator;
  for (int i = 0; i < cEnt; i++) {
    cacheIdx[i] = -1;
  }
}

// Costruttore equivalente al primo
Tabella::Tabella(const char * nomeFile, char separator) {
  currentLine = -1;
  nRighe = -1;
  fileName.assign(std::string(nomeFile));
  sep = separator;
  for (int i = 0; i < cEnt; i++) {
    cacheIdx[i] = -1;
  }
}

Tabella& Tabella::operator = (const Tabella & src) {
  if (this == &src) {
    return *this;
  }
  for (int i = 0; i < cEnt; i++) {
    cacheIdx[i] = src.cacheIdx[i];
    cache[i] = src.cache[i];
  }
  nRighe = src.nRighe;
  sep = src.sep;
  fileName = src.fileName;
  currentLine = -1;
  return *this;
}

Tabella::Tabella(const Tabella & src) {
  for (int i = 0; i < cEnt; i++) {
    cacheIdx[i] = src.cacheIdx[i];
    cache[i] = src.cache[i];
  }
  nRighe = src.nRighe;
  sep = src.sep;
  fileName = src.fileName;
  currentLine = -1;
}

Tabella::~Tabella() {
  closeFile();
}

bool Tabella::openFile() {
  if (!fp.is_open()) {
    fp.open(fileName.c_str());
    currentLine = 0;
    if (!fp) {
      fprintf(stderr, "Impossibile aprire il file:\n%s\n", this->fileName.c_str());
      return false;
    }
  }
  return true;
}

bool Tabella::closeFile() {
  fp.close();
  fp.clear();
  currentLine = -1;
  return true;
}

bool Tabella::contaRighe() {
  if (this->nRighe >= 0) {
    return true;
  }
  if (!openFile()) {
    return false;
  }

  int righeContate = -1;
  while (fp.ignore(std::numeric_limits<std::streamsize>::max(), fp.widen('\n'))) {
    righeContate++;
  }

  closeFile();

  this->nRighe = righeContate;
  return true;
}

bool Tabella::skipTo(int n) {
  if (n == 0 || currentLine == n - 1) {    // 0 sta per "prossima riga"
    currentLine++;
    return true;
  }
  if (n < currentLine) {
    closeFile();
    if (!openFile()) {
      return false;
    }
  }
  while (currentLine < n - 1 && fp.ignore(std::numeric_limits<std::streamsize>::max(), fp.widen('\n'))) {
    currentLine++;
  }
  if (currentLine != n - 1) {
    return false;
  }
  currentLine = n;
  return true; 
}

int Tabella::readLine(std::string &str, int n) {
  if (!openFile()) {
    return -1;
  }
  if (!skipTo(n)) {
    closeFile();
    return -1;
  }
  return (std::getline(fp, str) ? 1 : 0);
}

//int Tabella::getLine(int line, std::string& str) {
//  std::vector <std::string> tokens;
//  if (cacheFetch(line, tokens) == 0) {
//    str = "";
//    for (std::vector <std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
//      str += *it;
//      if (it != tokens.end() - 1) {
//        str += ' ';
//      }
//    }
//    return 0;
//  }
//  return 1;
//}

int Tabella::getField(int line, int field, std::string& str) {
  int cacheRet = readCache(line, field, str);
  if (cacheRet == -3) {
    std::cout << "Requested field is out of range!\n";
  }
  return cacheRet;
}

double Tabella::readDouble(int line, int field) {
  std::string tmp;
  getField(line, field, tmp);
  return stod(tmp);
}

void Tabella::cacheInvalidation() {
  for (int i = 0; i < this->cEnt; i++) {
    this->cache[i].clear();
  }
}

int Tabella::readCache(int line, int field, std::string &str) {
  if (cacheFetch(line)) {
    int lineSize = cache[(line -1) % cEnt].size();
    if (field < 0) {
      field += lineSize + 1;
    }
    if (field > 0 && field <= lineSize) {
      str = cache[(line - 1) % cEnt][field - 1];
      return 1;     // tutto ok
    }
    else {
      return -3;    // out of range
    }
  }
  return -1;        // impossibile leggere file
}

// Metodo privato di aggiornamento della cache
bool Tabella::cacheFetch(int line) {
  if (cacheIdx[(line - 1) % cEnt] != line) {      // Cache MISS
    if(!contaRighe()) {                           // Conto righe totali
      return false;
    }
    std::string riga;
    if (!readLine(riga, line)) {
      return false;
    }
    cache[(line - 1) % cEnt].clear();
    cache[(line - 1) % cEnt] = split(riga, this->sep);
    cacheIdx[(line - 1) % cEnt] = line;
  }
  return true;
}

int Tabella::getLineNum() {
  if(!contaRighe()) {
    return -1;
  }
  return nRighe;
}

// Splitta la stringa di input in base al carattere c di separazione fornito
// in un vettore. Successioni di caratteri c saranno scartate
std::vector <std::string> Tabella::split(std::string toSplit, char c) {
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

std::string Tabella::getTabName(){
  return this->fileName;
}
