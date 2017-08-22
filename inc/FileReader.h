#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>

#include <tools.h>

class FileReader {
  private:
    int currentLine;
    int totalLines;
    const char * nome;
    std::ifstream file;

    FileReader& defaultInit();
    FileReader& closeFile();
    FileReader& skipTo(int riga);
    int countLines();
  public:
    FileReader();
    FileReader(const char *nomeFile);
    FileReader(const std::string  &nomeFile);
    FileReader(const FileReader & src);
    FileReader& operator = (const FileReader & src);

    FileReader& openFile(const char * nomeFile);
    FileReader& openFile(const std::string &nomeFile);
    
    std::string readLine(int riga);
    const char * getName();
    int getLineNum();
};

#endif
