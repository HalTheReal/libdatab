#ifndef SPETTRO_H
#define SPETTRO_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>      //setw, setprecision
#include "Data.h"
#include "Tabella.h"

class SpettroGS {
  private:
  float dT;
  int canali;
  std::vector <int> eventE;
  std::vector <long int> nsec;
  Data dataSpt;

  void readDate(std::string &token);

  // Splitting delle stringhe
  static std::vector <std::string> split(std::string toSplit, char c);
  static void splitWhite(const std::string &toSplit, std::vector <std::string> &res);

  public:
  SpettroGS();
  SpettroGS(const char * nomeFile);

  SpettroGS& operator+=(const SpettroGS& rhs);

  int readFile(const char * nomeFile);
  void write(const char * nomeFile);

  SpettroGS cut(int from, int to);
  SpettroGS cut(Data &from, Data &to);
  
  void printContent();

  Data getDate();
  float getdT();
};

SpettroGS operator+(SpettroGS lhs, const SpettroGS& rhs);

#endif
