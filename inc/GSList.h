#ifndef GSLIST
#define GSLIST

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>      //  setw, setprecision
#include <exception>
#include <utility>      //  std::pair
#include <algorithm>    //  std::sort
#include <cmath>

#include <DateTime.h>
#include <SpectAcq.h>
#include <tools.h>

namespace Spectrometry {

  class GSList {

    public:
      GSList();
      GSList(const std::vector <std::pair <long, int>> &events, const Epoch::DateTime &start);

      double getDT() const;
      Epoch::DateTime getDateTime() const;
      Spectrometry::SpectAcq toSpectrum() const;

      GSList& erase(long from, long to);
      GSList& merge(const GSList &gsl);
      GSList copy(long from, long to) const;

      void writeLST(const char * nomeFile) const;

    private:
      float dT;
      std::vector <std::pair <long, int>> evtList;  // clk, energy
      Epoch::DateTime dataGS;


  };

  bool isLess(std::pair <long, int> pair, long val);
  GSList readGSL(const char * nomeFile);
  void writeSPE(const GSList &lst, const char * nomeFile);
  void writeSPT(const GSList &lst, const char * nomeFile);

}
#endif
