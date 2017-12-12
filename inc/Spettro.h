#ifndef SPETTRO_H
#define SPETTRO_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>      //setw, setprecision
#include <random>
#include <chrono>
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

  public:
    Spettro();
    Spettro(const char * nomeFile);

    Spettro& operator+=(const Spettro& rhs);
    Spettro& operator-=(const Spettro& rhs);

    Spettro& append(const Spettro& toApp);

    Spettro& binCut(int from, int to);
    Spettro& energyCut(float from, float to);
    Spettro& calibrate(double m, double q);
    Spettro& rebin(double gain);
    Spettro& rebin(double gain, unsigned seed);

    int readFile(const char * nomeFile);

    void writeSPE(const char * nomeFile);
    void writeSPT(const char * nomeFile);

    int energyToBin(double en) const;

    double getCounts(double e1) const;
    double getCounts(double e1, double e2) const;
    double getCps(double e1) const;
    double getCps(double e1, double e2) const;
    double getBinContent(int b1) const;
    double getBinContent(int b1, int b2) const;
    
    Data getDate() const;
    float getdT() const;

    void printContent() const;
};

Spettro operator+(Spettro lhs, const Spettro& rhs);
Spettro operator-(Spettro lhs, const Spettro& rhs);

#endif
