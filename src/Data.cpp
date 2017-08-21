#include "Data.h"

Data::Data(std::string data, std::string ora, char dataSep, char oraSep, char style)
  : giorno(0)
  , mese(0)
  , anno(0)
  , ora(0)
  , minuti(0)
  , secondi(0)
  , millisecondi(0)
{
  std::vector <std::string> partiData = tls::split(data, dataSep);
  std::vector <std::string> partiOra = tls::split(ora, oraSep);
  if (partiData.size() != 3 || partiOra.size() != 3) {
    fprintf(stderr, "Invalid date/hour. Init to default...\n");
    defaultInit();
  }
  else {
    sortDate(partiData, style);
    try {
      init(partiData, partiOra);
    }
    catch (std::invalid_argument& e) {
      fprintf(stderr, "Invalid argument exception. Init to default...\n");
      defaultInit();
    }
    catch (std::out_of_range& e) {
      fprintf(stderr, "Out of range exception. Init to default...\n");
      defaultInit();
    }
  }
}

Data::Data(const char * data, const char * ora, char dataSep, char oraSep, char style)
  : giorno(0)
  , mese(0)
  , anno(0)
  , ora(0)
  , minuti(0)
  , secondi(0)
  , millisecondi(0)
{
  std::string dataStr(data);
  std::string oraStr(ora);
  std::vector <std::string> partiData = tls::split(dataStr, dataSep);
  std::vector <std::string> partiOra = tls::split(oraStr, oraSep);
  if (partiData.size() != 3 || partiOra.size() != 3) {
    fprintf(stderr, "Invalid date/hour. Init to default...\n");
    defaultInit();
  }
  else {
    sortDate(partiData, style);
    try {
      init(partiData, partiOra);
    }
    catch (std::invalid_argument& e) {
      fprintf(stderr, "Invalid argument exception. Init to default...\n");
      defaultInit();
    }
    catch (std::out_of_range& e) {
      fprintf(stderr, "Out of range exception. Init to default...\n");
      defaultInit();
    }
  }
}

Data::Data(int uTime)
  : giorno(0)
  , mese(0)
  , anno(0)
  , ora(0)
  , minuti(0)
  , secondi(0)
  , millisecondi(0)
{
  defaultInit();
  sum(uTime);
}

void Data::init(std::vector <std::string> &dtToks, std::vector <std::string> &hrToks) {
  giorno = std::stoi(dtToks[0]);
  mese = std::stoi(dtToks[1]);
  anno = std::stoi(dtToks[2]);
  if (anno <= 99 ) {
    anno += 2000;
  }
  ora = std::stoi(hrToks[0]);
  minuti = std::stoi(hrToks[1]);
  secondi = round(std::stoi(hrToks[2]));
  sum(0);
}

void Data::defaultInit() {
  giorno = 1;
  mese = 1;
  anno = 1970;
  ora = 0;
  minuti = 0;
  secondi = 0;
}

bool Data::operator==(const Data &dt) const {
  if (giorno == dt.giorno && mese == dt.mese && anno == dt.anno) {
    if (secondi == dt.secondi && minuti == dt.minuti && ora == dt.ora) {
      return true;
    }
  }
  return false;
}

bool Data::operator != (const Data &dt) const {
  return !(*this == dt);
}

bool Data::operator > (const Data &dt) const {
  if (anno > dt.anno) { return true; }
  if (anno < dt.anno) { return false; }
  if (mese > dt.mese) { return true; }
  if (mese < dt.mese) { return false; }
  if (giorno > dt.giorno) { return true; }
  if (giorno < dt.giorno) { return false; }
  if (ora > dt.ora) { return true; }
  if (ora < dt.ora) { return false; }
  if (minuti > dt.minuti) { return true; }
  if (minuti < dt.minuti) { return false; }
  if (secondi > dt.secondi) { return true; }
  else { return false; }
}

bool Data::operator < (const Data &dt) const {
  if (anno < dt.anno) { return true; }
  if (anno > dt.anno) { return false; }
  if (mese < dt.mese) { return true; }
  if (mese > dt.mese) { return false; }
  if (giorno < dt.giorno) { return true; }
  if (giorno > dt.giorno) { return false; }
  if (ora < dt.ora) { return true; }
  if (ora > dt.ora) { return false; }
  if (minuti < dt.minuti) { return true; }
  if (minuti > dt.minuti) { return false; }
  if (secondi < dt.secondi) { return true; }
  else { return false; }
}

bool Data::operator >= (const Data &dt) const {
  return (*this == dt || *this > dt);
}

bool Data::operator <= (const Data &dt) const {
  return (*this == dt || *this < dt);
}

bool Data::isLeap(int year) {
  if (year % 4 != 0) {
    return false;
  }
  else if (year % 100 != 0) {
    return true;
  }
  else if (year % 400 != 0) {
    return false;
  }
  else {
    return true;
  }
}

int Data::daysInYear(int year) {
  return ((isLeap(year) == true) ? 366 : 365);
}

int Data::daysInMonth(int month, int year) {
  if (month <= 0 || month > 12) {
    return -1;
  }
  int days;
  if (month <= 7) {
    days = month % 2 == 0 ? 30 : 31;
  }
  else if (month > 7) {
    days = month % 2 == 0 ? 31 : 30;
  }
  if (month == 2) {
    days = isLeap(year) ? 29 : 28;
  }
  return days;
}

int Data::daysFromNewYear(int day, int month, int year) {
  int tot = 0;
  if (month == 1) {
    tot = day;
  }
  else {
    tot = 31 + day;
    month--;
    while (month > 1) {
      tot += daysInMonth(month, year);
      month--;
    }
  }
  return tot - 1;
}

int Data::daysToNewYear(int day, int month, int year) {
  int tot = daysInYear(year);
  tot -= daysFromNewYear(day, month, year);
  return tot - 1;
}

int Data::toUnix() const {
  int unix = 0;
  unix = this->secondi + this->minuti*60 + this->ora*60*60;
  int dayCounter = 0;
  int yy;
  if (this->anno >= 1970) {
    dayCounter = daysFromNewYear(this->giorno, this->mese, this->anno);
    yy = this->anno - 1;
    while (yy >= 1970) {
      dayCounter += daysInYear(yy);
      yy--;
    }
    unix += dayCounter*24*60*60;
  }
  else {
    dayCounter = daysToNewYear(this->giorno, this->mese, this->anno);
    yy = this->anno + 1;
    while (yy < 1970) {
      dayCounter += daysInYear(yy);
      yy++;
    }
    unix = -(86400 - unix);
    unix -= dayCounter*24*60*60;
  }
  return unix;
}

Data& Data::sum(int sec) {
  return (sec >= 0 ? add(sec) : subtract(sec));
}

Data& Data::add(int sec) {
  secondi += sec;
  if (secondi >= 60) {
    minuti += secondi / 60;
    secondi = secondi % 60;
  }
  if (minuti >= 60) {
    ora += minuti / 60;
    minuti = minuti % 60;
  }
  int giorniAggiunti = 0;
  if (ora >= 24) {
    giorniAggiunti = ora / 24;
    ora = ora % 24;
  }
  sumDays(giorniAggiunti);
  return *this;
}

Data& Data::subtract(int sec) {
  secondi += sec;
  int resto;
  if (secondi < 0) {
    minuti += floor((secondi / 60.0));    // floor(-1.8) = -2
    resto = secondi % 60;
    secondi = resto != 0 ? 60 + resto : 0;
  }
  if (minuti < 0) {
    ora += floor((minuti / 60.0));
    resto = minuti % 60;
    minuti = resto != 0 ? 60 + resto : 0;
  }
  int giorniAggiunti = 0;
  if (ora < 0) {
    giorniAggiunti = floor(ora / 24.0);
    resto = ora % 24;
    ora = resto != 0 ? 24 + resto : 0;
  }
  sumDays(giorniAggiunti);
  return *this;
}

Data& Data::sumDays(int d) {
  if (d >= 0) {
    giorno += d;
    while (giorno > daysInMonth(mese, anno)) {
      giorno -= daysInMonth(mese, anno);
      mese++;
      if (mese > 12) {
        anno++;
        mese = 1;
      }
    }
  }
  else {
    while (d < 0) {
      d++;
      giorno--;
      if (giorno == 0) {
        mese--;
        if (mese == 0) {
          anno--;
          mese = 12;
        }
        giorno = daysInMonth(mese, anno);
      }
    }
  }
  return *this;
}

int Data::getGpsOffset() const {
  Data dt1("1/7/2012", "00:00:00");
  Data dt2("30/6/2015", "23:59:59");
  Data dt3("1/7/2015", "00:00:00");
  Data dt4("31/12/2016", "23:59:59");
  Data dt5("1/1/2017", "00:00:00");
  if (*this >= dt1 && *this <= dt2) {
    return 16;
  }
  else if (*this >= dt3 && *this <= dt4) {
    return 17;
  }
  else if (*this >= dt5) {
    return 18;
  }
  else {
    return 0;
  }
}

void Data::print() const {
  std::cout << toString() << std::endl;
}

std::string Data::hourToString(char sep) const {
  std::string str;
  if (ora < 10) { str += "0"; }
  str += std::to_string(ora) + sep;
  if (minuti < 10) { str += "0"; }
  str += std::to_string(minuti) + sep;
  if (secondi < 10) { str += "0"; }
  str += std::to_string(secondi);
  return str;
}

std::string Data::dateToString(char sep, char style) const {
  std::string str;
  switch(style) {
    case 'L':   // Little Endian dd/mm/yy
      str = std::to_string(giorno) + sep + std::to_string(mese) + sep + std::to_string(anno);
      break;
    case 'B':   // Big Endian yy/mm/dd
      str = std::to_string(anno) + sep + std::to_string(mese) + sep + std::to_string(giorno);
      break;
    case 'M':   // Middle Endian mm/dd/yy
      str = std::to_string(mese) + sep + std::to_string(giorno) + sep + std::to_string(anno);
      break;
    default :
      str = std::to_string(giorno) + sep + std::to_string(mese) + sep + std::to_string(anno);
      break;
  }
  return str;
}

std::string Data::toString(char sep1, char sep2, char style) const {
  std::string str;
  str += dateToString(sep1, style) + " " + hourToString(sep2);
  return str;
}

// Metodo privato per l'inversione della data
void Data::sortDate(std::vector <std::string> &date, char style) {
  std::string tmp = date[0];
  switch(style) {
    case 'L':   // Little Endian dd/mm/yy
      break;
    case 'B':   // Big Endian yy/mm/dd
      date[0] = date[2];
      date[2] = tmp;
      break;
    case 'M':   // Middle Endian mm/dd/yy
      date[0] = date[1];
      date[1] = tmp;
      date[2] = date[2];
      break;
    default :
      break;
  }
}

// Deprecated
std::string Data::dateToRevString(char sep) {
  std::string str;
  str += std::to_string(anno) + sep + std::to_string(mese) + sep + std::to_string(giorno);
  return str;
}
// Deprecated
std::string Data::toRevString(char sep1, char sep2) {
  std::string str;
  str += dateToRevString(sep1) + " " + hourToString(sep2);
  return str;
}
