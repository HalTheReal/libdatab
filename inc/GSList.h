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

namespace Spectrometry {

  class GSList {

    public:
      GSList();
      GSList(const std::vector <std::pair <long, int>> &events, const Epoch::DateTime &start);
      GSList(const char * nomeFile);

      int readFile(const char * nomeFile);

      GSList& append(const GSList& toApp);
      GSList& timeCut(int from, int to);
      GSList& timeCut(const Epoch::DateTime &from, int to);
      GSList& timeCut(const Epoch::DateTime &from, const Epoch::DateTime &to);

      double getDT() const;
      Epoch::DateTime getDateTime() const;

      void writeLST(const char * nomeFile) const;

      Spectrometry::SpectAcq toSpectrum() const;

    private:
      float dT;
      std::vector <long> clk;
      std::vector <int> event;
      Epoch::DateTime dataGS;

      void defaultInit();
      bool isEmpty() const;

      int readLST(const char * nomeFile);

  };

  GSList readGSL(const char * nomeFile);
  void writeSPE(const GSList &lst, const char * nomeFile);
  void writeSPT(const GSList &lst, const char * nomeFile);

}
#endif
