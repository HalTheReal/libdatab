#include "GSList.h"
namespace Spectrometry {

GSList::GSList()
  : dT(0)
  , dataGS()
{}

GSList::GSList(const std::vector <std::pair <long, int>> &events, const Epoch::DateTime &start)
  : GSList()
{
  clk.reserve(events.size());
  event.reserve(events.size());
  for(auto &pr : events) {
    clk.push_back(pr.first);
    event.push_back(pr.second);
  }
  dataGS = start;
  // Si assume che sia ordinato!
  dT = clk.back() * 16E-9;
}

Spectrometry::SpectAcq GSList::toSpectrum() const {
  std::vector <int> bin(2048, 0);
  for (int val : this->event) {
    ++bin[val];
  }
  return Spectrometry::SpectAcq(bin, this->dataGS, this->dT);
}

void GSList::writeLST(const char * nomeFile) const {
  using namespace std;
  ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << "#GammaStream 1.0 LIST\n";
    outfile << "#StartTime: ";
    outfile << dataGS.year() << '-' << dataGS.month() << '-';
    outfile << dataGS.day() << 'T' << Epoch::toTime(dataGS) << '\n';
    outfile << "#Fields: Time\tEnergy\tGain" << '\n';
    for (unsigned i = 0; i < clk.size(); ++i) {
      outfile << right << setw(9) << clk[i] << '\t';
      outfile << setw(6) << event[i] << '\t';
      outfile << "1.000" << '\n';
    }
  }
}

double GSList::getDT() const {
  return dT;
}

Epoch::DateTime GSList::getDateTime() const {
  return dataGS;
}

GSList readGSL(const char * nomeFile) {
  std::ifstream file(nomeFile);
  if (!file) {
    throw std::invalid_argument("Unable to open file!");
  }
  std::vector <std::pair <long, int>> coppie;
  Epoch::DateTime dataGS;

  std::string token;
  do {
    file >> token;
    if (token.compare("#StartTime:") == 0) {
      int yr, mn, dy;
      char sep;
      Epoch::Time tm;
      file >> yr >> sep >> mn >> sep >> dy >> sep >> tm;
      Epoch::Date dt(dy, mn, yr);
      dataGS = Epoch::DateTime(dt, tm);
    }
  } while (token.compare("Gain") != 0);
  long timeTok;
  double gainTok;
  int energyTok;
  while (file >> timeTok >> energyTok >> gainTok) {
    coppie.push_back(std::make_pair(timeTok, energyTok));
  }
  file.close();
  return GSList(coppie, dataGS);
}

void writeSPE(const GSList &lst, const char * nomeFile) {
  return writeSPE(lst.toSpectrum(), nomeFile);
}

void writeSPT(const GSList &lst, const char * nomeFile) {
  return writeSPT(lst.toSpectrum(), nomeFile);
}

}
