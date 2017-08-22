#include "FileReader.h"

FileReader::FileReader()
  : currentLine(0)
  , totalLines(0)
  , nome("")
{}

FileReader::FileReader(const char *nomeFile)
  : currentLine(0)
  , totalLines(0)
  , nome(nomeFile)
{
  openFile(nomeFile);
}

FileReader::FileReader(const std::string &nomeFile)
  : currentLine(0)
  , totalLines(0)
  , nome(nomeFile.c_str())
{
  openFile(nome);
}

FileReader& FileReader::defaultInit() {
  currentLine = 0;
  totalLines = 0;
  nome = "";
  return *this;
}

FileReader::FileReader(const FileReader & src)
  : currentLine(0)
  , totalLines(0)
  , nome(src.nome)
{
  openFile(nome);
}

FileReader& FileReader::operator=(const FileReader & src) {
  currentLine = 0;
  totalLines = 0;
  nome = src.nome;
  openFile(nome);
  return *this;
}

FileReader& FileReader::openFile(const char *nomeFile) {
  defaultInit();
  file.open(nomeFile);
  if(file.is_open()) {
    nome = nomeFile;
    totalLines = countLines();
    return *this;
  }
  else {
    throw std::runtime_error("Unable to open file!");
  }
}

FileReader& FileReader::openFile(const std::string &nomeFile) {
  return(openFile(nomeFile.c_str()));
}

FileReader& FileReader::closeFile() {
  file.close();
  return *this;
}

FileReader& FileReader::skipTo(int riga) {
  if (currentLine == riga - 1) {
    std::cout << "Prossima riga!\n";
    currentLine++;
    return *this;
  }
  if (riga <= currentLine) {
    std::cout << "Riga minore!\n";
    currentLine = 0;
    file.seekg(0, file.beg);
  }
  while (currentLine < riga - 1 && file.ignore(std::numeric_limits<std::streamsize>::max(), file.widen('\n'))) {
    currentLine++;
  }
  currentLine = riga;
  return *this;
}

std::string FileReader::readLine(int riga) {
  if (file.is_open()) {
    skipTo(riga);
    std::string str;
    std::getline(file, str);
    return str;
  }
  else {
    throw std::runtime_error("No file was opened!");
  }
}

int FileReader::countLines() {
  totalLines = -1;
  while (file.ignore(std::numeric_limits<std::streamsize>::max(), file.widen('\n'))) {
    ++totalLines;
  }
  file.clear();
  file.seekg(0, file.beg);
  return totalLines;
}

int FileReader::getLineNum() {
  return totalLines;
}

const char * FileReader::getName() {
  return nome;
}
