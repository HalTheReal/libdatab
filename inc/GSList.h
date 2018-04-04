#ifndef GSLIST
#define GSLIST

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>      //  setw, setprecision
#include <exception>
#include <utility>      //  std::pair

#include <GSList.h>
#include <DateTime.h>
#include <SpectAcq.h>
#include <tools.h>

class GSList {
  private:
    float dT;
    std::vector <long> clk;
    std::vector <int> event;
    Epoch::DateTime dataGS;

    void defaultInit();
    bool isEmpty() const;

    int readLST(const char * nomeFile);

  public:
    GSList();
    GSList(std::pair <std::vector <long>, std::vector <int>> events, const Epoch::DateTime &start);
    GSList(const char * nomeFile);

    Spectrometry::SpectAcq toSpectrum() const;

    int readFile(const char * nomeFile);

    GSList& append(const GSList& toApp);
    GSList& timeCut(int from, int to);
    GSList& timeCut(const Epoch::DateTime &from, int to);
    GSList& timeCut(const Epoch::DateTime &from, const Epoch::DateTime &to);

    float getDT() const;
    Epoch::DateTime getDateTime() const;

    void writeLST(const char * nomeFile);
};

#endif
