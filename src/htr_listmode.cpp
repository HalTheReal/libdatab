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
  evl.sort(isBeforeEvent);
}

void append(EventList &dest, EventList &toApp) {
  auto end = dest.back().time();
  timeShift(toApp, end);
  dest.merge(toApp, isBeforeEvent);
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
  EventList copy = timeCopy(eventList, std::chrono::seconds(fromSec), std::chrono::seconds(toSec));
  Epoch::DateTime dt = dataGS;
  if(fromSec > 0) {
    dt.addSec(fromSec);
  }
  return GSList(copy, dataGS);
}

GSList GSList::copy(const Epoch::DateTime from, int toSec) const {
  int fromSec = toUnix(dataGS) - toUnix(from);
  return copy(fromSec, toSec);
}

GSList GSList::copy(int fromSec, const Epoch::DateTime &to) const {
  int toSec = toUnix(dataGS) - toUnix(to);
  return copy(fromSec, toSec);
}

GSList GSList::copy(const Epoch::DateTime from, const Epoch::DateTime &to) const {
  int fromSec = toUnix(dataGS) - toUnix(from);
  int toSec = toUnix(dataGS) - toUnix(to);
  return copy(fromSec, toSec);
}

GSList GSList::cut(int fromSec, int toSec) {
  EventList cut = timeCut(eventList, std::chrono::seconds(fromSec), std::chrono::seconds(toSec));
  Epoch::DateTime dt = dataGS;
  if(fromSec == 0) {
    dt.addSec(toSec);
  }
  return GSList(cut, dataGS);
}

GSList GSList::cut(const Epoch::DateTime from, int toSec) {
  int fromSec = toUnix(dataGS) - toUnix(from);
  return cut(fromSec, toSec);
}

GSList GSList::cut(int fromSec, const Epoch::DateTime &to) {
  int toSec = toUnix(dataGS) - toUnix(to);
  return cut(fromSec, toSec);
}

GSList GSList::cut(const Epoch::DateTime from, const Epoch::DateTime &to) {
  int fromSec = toUnix(dataGS) - toUnix(from);
  int toSec = toUnix(dataGS) - toUnix(to);
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
  eventList.merge(gsl.eventList, isBeforeEvent);
  return *this;
}

GSList& GSList::append(GSList &gsl) {
  std::chrono::seconds offset(gsl.getLT());
  timeShift(gsl.eventList, offset);
  eventList.merge(gsl.eventList, isBeforeEvent);
  return *this;
}

std::vector <int> GSList::toHistogram() const {
  return Spectrometry::toHistogram(eventList, 2048);
}

Epoch::DateTime GSList::getDateTime() const {
  return dataGS;
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

GSList readGSL(const std::string &nomeFile) {
  return readGSL(nomeFile.c_str());
}

Spectrometry::Spectrum toSpectrum(const GSList &gsl) {
  double LT = gsl.getLT<std::chrono::milliseconds>().count() / 1000.0;
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
