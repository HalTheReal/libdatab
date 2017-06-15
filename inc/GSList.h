#ifndef GSLIST
#define GSLIST

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>      //setw, setprecision

#include "GSList.h"
#include "Data.h"

class GSList {
  private:
    float dT;
    std::vector <long> clk;
    std::vector <int> event;
    Data dataGS;

    static void splitWhite(const std::string &toSplit, std::vector <std::string> &res);
    static std::vector <std::string> split(std::string toSplit, char c);

  protected:
    int readLST(const char * nomeFile);

  public:
    GSList();
    GSList(const char * nomeFile);

    int readFile(const char * nomeFile);

    GSList& append(const GSList& toApp);
    GSList& timeCut(int from, int to);
    GSList& timeCut(Data &from, Data &to);

    void writeLST(const char * nomeFile);
};

#endif
