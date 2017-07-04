#ifndef SPETTRO_H
#define SPETTRO_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>      //setw, setprecision
#include <Data.h>
#include <tools.h>

class Spettro {
  private:
    int readSPE(const char * nomeFile);
    int readSPT(const char * nomeFile);
    int readLST(const char * nomeFile);

  protected:

    float dT;
    int canali;
    double mCal;
    double qCal;
    std::vector <float> bin;
    Data dataSpt;

    void defaultInit();
    int energyToBin(float en);

    // Splitting delle stringhe
    static void splitWhite(const std::string &toSplit, std::vector <std::string> &res);

  public:
    Spettro();
    Spettro(const char * nomeFile);

    Spettro& operator+=(const Spettro& rhs);
    Spettro& operator-=(const Spettro& rhs);

    Spettro& append(const Spettro& toApp);

    Spettro& binCut(int from, int to);
    Spettro& energyCut(float from, float to);

    int readFile(const char * nomeFile);

    void writeSPE(const char * nomeFile);
    void writeSPT(const char * nomeFile);

    double getCounts(int e1, int e2);
    double getCps(int e1, int e2);
    Data getDate();
    float getdT();

    void printContent();
};

Spettro operator+(Spettro lhs, const Spettro& rhs);
Spettro operator-(Spettro lhs, const Spettro& rhs);

#endif
