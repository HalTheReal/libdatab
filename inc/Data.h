#ifndef DATA_H
#define DATA_H

#include <tools.h>
#include <string>
#include <vector>
#include <stdexcept>    // Eccezioni
#include <iostream>     // std::cout
#include <math.h>       // fmod

class Data {
	private:
    int giorno;
    int mese;
    int anno;
    int ora;
    int minuti;
    int secondi;
    int millisecondi;

    void defaultInit();
    void init(std::vector <std::string> &dtToks, std::vector <std::string> &hrToks);

    void sortDate(std::vector <std::string> &data, char style);
    Data& add(int sec);
    Data& subtract(int sec);
    static int daysInMonth(int month, int year);
    static int daysInYear(int year);
    static int daysFromNewYear(int day, int month, int year);
    static int daysToNewYear(int day, int month, int year);

  public:
    Data(const char * data, const char * ora, char dataSep = '/', char oraSep = ':', char style = 'L');
    Data(std::string data, std::string ora, char dataSep = '/', char oraSep = ':', char style = 'L');
    Data(int uTime = 0);

    bool operator == (const Data &dt);
    bool operator != (const Data &dt);
    bool operator > (const Data &dt);
    bool operator < (const Data &dt);
    bool operator >= (const Data &dt);
    bool operator <= (const Data &dt);

    static bool isLeap(int year);

    Data& print();
    Data& sum(int sec);
    Data& sumDays(int d);
    int getGpsOffset();
    int toUnix();
    std::string toString(char sep1 = '/', char sep2 = ':', char style = 'L');
    std::string dateToString(char sep = '/', char style = 'L');
    std::string hourToString(char sep = ':');
    // Deprecated
    std::string toRevString(char sep = '/', char sep2 = ':');
    std::string dateToRevString(char sep = '/');
};
#endif
