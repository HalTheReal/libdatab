#ifndef TABELLA_H
#define TABELLA_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <FileReader.h>

#include <tools.h>

class Tabella {

  private:

    char sep;                                 // Separatore dei campi tabella, ad es. ' '
    int totalFields;
    FileReader fileR;

    std::string readLine(int riga);
    int countFields();

  protected:

  public:
    Tabella(std::string nomeFile, char separator);
    Tabella(const char * nomeFile, char separator);
    int getLineNum();
    int getLine(int riga, std::string& str);
    std::string getField(int riga, int field);
    int getField(int riga, int field, std::string& str);
    double readDouble(int riga, int field);
    std::string getTabName();
};

#endif
