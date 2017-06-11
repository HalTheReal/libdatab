#ifndef SPETTROGS_H
#define SPETTROGS_H

#include "Spettro.h"

class SpettroGS : public Spettro {
  private:
    std::vector <long> clock;
    std::vector <int> event;

  protected:
    int readLST(const char * nomeFile) override;

  public:
    SpettroGS();
    SpettroGS(const char * nomeFile);

    int readFile(const char * nomeFile) override;

    SpettroGS& timeCut(int from, int to);
    SpettroGS& timeCut(Data &from, Data &to);

    void writeLST(const char * nomeFile);
};

#endif
