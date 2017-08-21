#ifndef GSLIST
#define GSLIST

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>      //setw, setprecision

#include <GSList.h>
#include <Data.h>
#include <tools.h>

class GSList {
  private:
    float dT;
    std::vector <long> clk;
    std::vector <int> event;
    Data dataGS;

    void defaultInit();
    bool isEmpty() const;

  protected:
    int readLST(const char * nomeFile);

  public:
    GSList();
    GSList(const char * nomeFile);

    int readFile(const char * nomeFile);

    GSList& append(const GSList& toApp);
    GSList& timeCut(int from, int to);
    GSList& timeCut(Data &from, int to);
    GSList& timeCut(Data &from, Data &to);

    float getdT() const;
    Data getDate() const;

    void writeLST(const char * nomeFile);
    void writeSPE(const char * nomeFile);
    void writeSPT(const char * nomeFile);
};

#endif
