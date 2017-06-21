#ifndef TABELLA_H
#define TABELLA_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

class Tabella {
	
  private:
  
  const static int cEnt = 10;               // Cache entries
  int cacheIdx[cEnt];                       // Cache mapping
  int nRighe;                               // Righe nel file
  char sep;                                 // Separatore dei campi tabella, ad es. ' '
  int currentLine;
  std::string fileName;
  std::ifstream fp;
  std::vector <std::string> cache[cEnt];    // Cache di stringhe

  bool skipTo(int n);
  bool openFile();
  bool closeFile();
  bool contaRighe();

  void cacheInvalidation();
  bool cacheFetch(int line);
  int readCache(int line, int field, std::string &str);
  int readLine(std::string &str, int n = 0);

  protected:

  public:
  Tabella(std::string nomeFile, char separator);
  Tabella(const char * nomeFile, char separator);
  Tabella(const Tabella & src);
  ~Tabella();
  Tabella& operator = (const Tabella & src);
  int getLineNum();
  int getLine(int riga, std::string& str);
  int getField(int riga, int field, std::string& str);
  double readDouble(int riga, int field);
  std::string getTabName();
  static std::vector <std::string> split(std::string toSplit, char c);
};
#endif
