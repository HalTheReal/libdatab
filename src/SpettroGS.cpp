#include "SpettroGS.h"

// Costruttore
SpettroGS::SpettroGS()
  : Spettro()
{}

int SpettroGS::readFile(const char * nomeFile) {
  std::string filename(nomeFile);
  std::string::size_type idx;

  idx = filename.rfind('.');

  if(idx != std::string::npos)
  {
    std::string extension = filename.substr(idx+1);
    if (extension.compare("txt") == 0) {
      return readLST(nomeFile);
    }
  }
  std::cout << nomeFile << " is not a valid file!\n";
  return 0;
}

int SpettroGS::readLST(const char * nomeFile) {
  std::ifstream file(nomeFile);
  if (!file) {
    fprintf(stderr, "Impossibile aprire il file:\n%s\n", nomeFile);
    return 0;
  }
  clock.clear();
  event.clear();
  bin.clear();

  canali = 2048;
  bin.resize(canali, 0);

  std::string riga;
  while (std::getline(file,riga)) {
    std::vector <std::string> toks;
    splitWhite(riga, toks);
    if (toks.size() == 3 && toks[0][0] != '#') {
      dT = stol(toks[0]) * 16E-9;
      ++bin[stoi(toks[1])];
      clock.push_back(stol(toks[0]));
      event.push_back(stod(toks[1]));
    }
    else if (toks[0].compare("#StartTime:") == 0) {
      std::vector <std::string> dateToks = split(toks[1], 'T');
      dataSpt = Data(dateToks[0], dateToks[1], '-', ':', 'B');
    }
  }
  file.close();
  return 1;
}

void SpettroGS::writeLST(const char * nomeFile) {
  using namespace std;
  ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << "#GammaStream 1.0 LIST\n";
    outfile << "#StartTime: ";
    outfile << dataSpt.dateToString('-','B') << "T";
    outfile << dataSpt.hourToString() << endl;
    outfile << "#Fields\tTime\tEnergy\tGain" << endl;
    for (unsigned i = 0; i < clock.size(); ++i) {
      outfile << right << setw(9) << clock[i] << '\t';
      outfile << setw(6) << event[i] << '\t';
      outfile << "1.000" << endl;
    }
  }
}
