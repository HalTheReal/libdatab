#include "Tabella.h"

Tabella::Tabella(std::string nomeFile, char separator)
  : sep(separator)
  , totalFields(0)
  , fileR(nomeFile)
{
  totalFields = countFields();
}

Tabella::Tabella(const char * nomeFile, char separator)
  : sep(separator)
  , totalFields(0)
  , fileR(nomeFile)
{
  totalFields = countFields();
}

std::string Tabella::getLine(int riga) {
  return fileR.getLine(riga);
}

std::string Tabella::getField(int line, int field) {
  auto toks = tls::split(getLine(line), sep);
  if (field < 0) {
    return toks[totalFields + field];
  }
  return toks[field - 1];
}

int Tabella::getField(int line, int field, std::string& str) {
  str = getField(line, field);
  return 1;
}

int Tabella::countFields() {
  auto toks = tls::split(getLine(2), sep);
  return toks.size();
}

double Tabella::readDouble(int line, int field) {
  std::string tmp = getField(line, field);
  return stod(tmp);
}

int Tabella::getLineNum() {
  return fileR.getLineNum();
}

int Tabella::getFieldNum() {
  return totalFields;
}

std::string Tabella::getTabName(){
  std::string nome(fileR.getName());
  return nome;
}
