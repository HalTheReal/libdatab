#include "htr_listmode.h"

namespace Spectrometry {

TimedEvent::TimedEvent()
  : evTime(0)
  , evEnergy(0)
{}

TimedEvent::TimedEvent(int64_t tstamp, unsigned energy)
  : evTime(tstamp)
  , evEnergy(energy)
{}

std::chrono::nanoseconds TimedEvent::time() const {
  return evTime;
}

unsigned TimedEvent::energy() const {
  return evEnergy;
}

TimedEvent& TimedEvent::setEnergy(unsigned energy) {
  evEnergy = energy;
  return *this;
}

bool isBefore(const TimedEvent &ev1, const TimedEvent &ev2) {
  return ev1.time() < ev2.time();
}

bool isAfter(const TimedEvent &ev1, const TimedEvent &ev2) {
  return ev1.time() > ev2.time();
}

bool isSync(const TimedEvent &ev1, const TimedEvent &ev2) {
  return ev1.time() == ev2.time();
}

std::ostream& operator << (std::ostream &stream, const TimedEvent &evt) {
  std::stringstream ss;
  ss.copyfmt(stream);
  ss << evt.time().count() << ' ' << evt.energy();
  stream << ss.str();
  return stream;
}

std::istream& operator >> (std::istream &stream, TimedEvent &evt) {
  int64_t tstamp;
  unsigned energy;
  stream >> tstamp >> energy;
  if (!stream) {
    return stream;
  }
  evt = TimedEvent(std::chrono::nanoseconds(tstamp), energy);
  return stream;
}

bool isBeforeEvent(const TimedEvent &ev1, const TimedEvent &ev2) {
  return isBefore(ev1, ev2);
}

void timeSort(EventList &evl) {
  std::sort(evl.begin(), evl.end(), isBeforeEvent);
}

void append(EventList &dest, const EventList &toApp) {
  dest.insert(dest.end(), toApp.begin(), toApp.end());
}

std::vector <int> toHistogram(const EventList &evl, std::size_t channels) {
  std::vector <int> hist(channels, 0);
  for (auto &evt : evl) {
    if (evt.energy() < channels) {
      ++hist[evt.energy()];
    }
  }
  return hist;
}

EventList readASCII(const char * filename) {
  EventList evtList;
  TimedEvent evt;
  std::ifstream file(filename);
  while(file >> evt) {
    evtList.push_back(evt);
  }
  return evtList;
}

EventList readASCII(const std::string &filename) {
  return readASCII(filename.c_str());
}

GSList::GSList()
  : eventList()
  , dataGS()
{}

GSList::GSList(EventList evl, Epoch::DateTime start)
  : eventList(evl)
  , dataGS(start)
{}

GSList GSList::copy(int fromSec, int toSec) const {
  fromSec = std::max(fromSec, 0);
  toSec = std::max(toSec, 0);
  if (fromSec > toSec) {
    std::swap(fromSec, toSec);
  }
  EventList copy = timeCopy(eventList, std::chrono::seconds(fromSec), std::chrono::seconds(toSec));
  Epoch::DateTime dt = dataGS;
  if(fromSec > 0) {
    dt.addSec(fromSec);
    timeShift(copy, std::chrono::seconds(-fromSec));
  }
  return GSList(copy, dt);
}

GSList GSList::copy(const Epoch::DateTime from, int toSec) const {
  int fromSec = toUnix(from) - toUnix(dataGS);
  return copy(fromSec, toSec);
}

GSList GSList::copy(int fromSec, const Epoch::DateTime &to) const {
  int toSec = toUnix(to) - toUnix(dataGS);
  return copy(fromSec, toSec);
}

GSList GSList::copy(const Epoch::DateTime from, const Epoch::DateTime &to) const {
  int fromSec = toUnix(from) - toUnix(dataGS);
  int toSec = toUnix(to) - toUnix(dataGS);
  return copy(fromSec, toSec);
}

void GSList::erase(int fromSec, int toSec) {
  fromSec = std::max(fromSec, 0);
  toSec = std::max(toSec, 0);
  if (fromSec > toSec) {
    std::swap(fromSec, toSec);
  }
  timeErase(eventList, std::chrono::seconds(fromSec), std::chrono::seconds(toSec));
  if(fromSec == 0) {
    dataGS.addSec(toSec);
    timeShift(eventList, std::chrono::seconds(-toSec));
  }
}

void GSList::erase(const Epoch::DateTime from, int toSec) {
  int fromSec = toUnix(from) - toUnix(dataGS);
  return erase(fromSec, toSec);
}

void GSList::erase(int fromSec, const Epoch::DateTime &to) {
  int toSec = toUnix(to) - toUnix(dataGS);
  return erase(fromSec, toSec);
}

void GSList::erase(const Epoch::DateTime from, const Epoch::DateTime &to) {
  int fromSec = toUnix(from) - toUnix(dataGS);
  int toSec = toUnix(to) - toUnix(dataGS);
  return erase(fromSec, toSec);
}

GSList GSList::cut(int fromSec, int toSec) {
  fromSec = std::max(fromSec, 0);
  toSec = std::max(toSec, 0);
  if (fromSec > toSec) {
    std::swap(fromSec, toSec);
  }
  EventList cutted = timeCut(eventList, std::chrono::seconds(fromSec), std::chrono::seconds(toSec));
  Epoch::DateTime dt = dataGS;
  if(fromSec > 0) {
    dt.addSec(fromSec);
    timeShift(cutted, std::chrono::seconds(-fromSec));
  }
  return GSList(cutted, dt);
}

GSList GSList::cut(const Epoch::DateTime from, int toSec) {
  int fromSec = toUnix(from) - toUnix(dataGS);
  return cut(fromSec, toSec);
}

GSList GSList::cut(int fromSec, const Epoch::DateTime &to) {
  int toSec = toUnix(to) - toUnix(dataGS);
  return cut(fromSec, toSec);
}

GSList GSList::cut(const Epoch::DateTime from, const Epoch::DateTime &to) {
  int fromSec = toUnix(from) - toUnix(dataGS);
  int toSec = toUnix(to) - toUnix(dataGS);
  return cut(fromSec, toSec);
}

GSList& GSList::merge(GSList &gsl) {
  std::chrono::seconds offset(std::abs(Epoch::toUnix(dataGS) - Epoch::toUnix(gsl.dataGS)));
  if(dataGS > gsl.dataGS) {
    timeShift(eventList, offset);
    dataGS = gsl.dataGS;
  }
  else {
    timeShift(gsl.eventList, offset);
  }
  Spectrometry::append(eventList, gsl.eventList);
  timeSort(eventList);
  return *this;
}

GSList& GSList::append(GSList &gsl) {
  Spectrometry::append(eventList, gsl.eventList);
  return *this;
}

std::vector <int> GSList::toHistogram() const {
  return Spectrometry::toHistogram(eventList, 2048);
}

Epoch::DateTime GSList::getDateTime() const {
  return dataGS;
}

void GSList::setDateTime(const Epoch::DateTime &dt) {
  dataGS = dt;
}

std::chrono::seconds GSList::getLT() const {
  // duration_cast tronca SEMPRE! quindi aggiungo mezzo secondo prima della conversione
  std::chrono::nanoseconds half(500000000);
  return std::chrono::duration_cast<std::chrono::seconds> (eventList.back().time() + half);
}

std::chrono::milliseconds GSList::getLTMilliseconds() const {
  // duration_cast tronca SEMPRE! quindi aggiungo mezzo millisecondo prima della conversione
  std::chrono::nanoseconds half(500000);
  return std::chrono::duration_cast<std::chrono::milliseconds> (eventList.back().time() + half);
}

std::chrono::nanoseconds GSList::getLTNanoseconds() const {
  return eventList.back().time();
}

void GSList::writeGSL(const char * nomeFile) const {
  std::ofstream outfile;
  outfile.open(nomeFile);
  if (outfile) {
    outfile << "#GammaStream 1.5 LIST\n";
    outfile << "#StartTime: ";
    outfile << dataGS.year() << '-' << dataGS.month() << '-';
    outfile << dataGS.day() << 'T' << Epoch::toTime(dataGS) << '\n';
    outfile << "#Fields: Time\tEnergy\tGain" << '\n';
    for (auto &evt : this->eventList) {
      outfile << std::right << std::setw(9) << evt.time().count() << '\t';
      outfile << std::setw(6) << evt.energy() << '\t';
      outfile << "1.000" << '\n';
    }
  }
}

void GSList::writeGSL(const std::string &nomeFile) const {
  return writeGSL(nomeFile.c_str());
}

GSList readGSL(const char * nomeFile) {
  std::ifstream file(nomeFile);
  if (!file) {
    throw std::invalid_argument("Unable to open file!");
  }
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
  int64_t timeTok;
  double gainTok;
  int energyTok;
  EventList listmode;
  while (file >> timeTok >> energyTok >> gainTok) {
    listmode.push_back(TimedEvent(timeTok, energyTok));
    //listmode.push_back(TimedEvent(timeTok * 16, energyTok));      // 1 ciclo clock = 16E-9 sec
  }
  file.close();
  return GSList(std::move(listmode), dataGS);
}

Epoch::DateTime getCentroid(const GSList &gsl) {
  Epoch::DateTime ret = gsl.getDateTime();
  double LT = gsl.getLTMilliseconds().count() / 1000.0;
  ret.addSec(std::round(LT / 2.0));
  return ret;
}

GSList readGSL(const std::string &nomeFile) {
  return readGSL(nomeFile.c_str());
}

Spectrum toSpectrum(const GSList &gsl) {
  double LT = gsl.getLTMilliseconds().count() / 1000.0;
  return Spectrometry::Spectrum(gsl.toHistogram(), gsl.getDateTime(), LT);
}

void writeSPE(const GSList &lst, const char * nomeFile) {
  return writeSPE(toSpectrum(lst), nomeFile);
}

void writeSPE(const GSList &lst, const std::string &nomeFile) {
  return writeSPE(toSpectrum(lst), nomeFile.c_str());
}

void writeSPT(const GSList &lst, const char * nomeFile) {
  return writeSPT(toSpectrum(lst), nomeFile);
}

void writeSPT(const GSList &lst, const std::string &nomeFile) {
  return writeSPT(toSpectrum(lst), nomeFile.c_str());
}

}
