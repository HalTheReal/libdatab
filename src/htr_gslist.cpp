#include "htr_gslist.h"
namespace Spectrometry {

TimedEvent::TimedEvent()
  : evTime(0)
  , evEnergy(0)
{}

TimedEvent::TimedEvent(long long tstamp, unsigned energy)
  : evTime(tstamp)
  , evEnergy(energy)
{}

long long TimedEvent::time() const {
  return evTime;
}

unsigned TimedEvent::energy() const {
  return evEnergy;
}

TimedEvent& TimedEvent::setTime(long long time) {
  evTime = time;
  return *this;
}

TimedEvent& TimedEvent::setEnergy(unsigned energy) {
  evEnergy = energy;
  return *this;
}

void shift(TimedEvent &ev, long long offset) {
  ev.setTime(ev.time() + offset);
}

bool isBefore(const TimedEvent &ev1, const TimedEvent &ev2) {
  return ev1.time() < ev2.time();
}

bool isAfter(const TimedEvent &ev1, const TimedEvent &ev2) {
  return !isBefore(ev1, ev2);
}

bool isSync(const TimedEvent &ev1, const TimedEvent &ev2) {
  return ev1.time() == ev2.time();
}

bool isBefore(const TimedEvent &ev1, long long time) {
  return ev1.time() < time;
}

bool isAfter(const TimedEvent &ev1, long long time) {
  return !isBefore(ev1, time);
}

bool isSync(const TimedEvent &ev1, long long time) {
  return ev1.time() == time;
}

bool isBeforeEvent(const TimedEvent &ev1, const TimedEvent &ev2) {
  return isBefore(ev1, ev2);
}

bool isBeforeTime(const TimedEvent &ev1, long long time) {
  return isBefore(ev1, time);
}

void timeSort(EventList &evl) {
  evl.sort(isBeforeEvent);
}

EventList timeCut(EventList &evl, long long from, long long to) {
  if(to < from) {
    std::swap(from, to);
  }
  auto cutFrom = std::lower_bound(evl.begin(), evl.end(), from, isBeforeTime);
  auto cutTo = std::lower_bound(evl.begin(), evl.end(), to, isBeforeTime);
  EventList cut;
  cut.splice(cut.begin(), evl, cutFrom, cutTo);
  return cut;
}

EventList timeCopy(const EventList &evl, long long from, long long to) {
  if(to < from) {
    std::swap(from, to);
  }
  auto copyFrom = std::lower_bound(evl.begin(), evl.end(), from, isBeforeTime);
  auto copyTo = std::lower_bound(evl.begin(), evl.end(), to, isBeforeTime);
  return EventList(copyFrom, copyTo);
}

void timeShift(EventList &evl, long long offset) {
  for (auto &event : evl) {
    shift(event, offset);
  }
}

void append(EventList &dest, EventList &toApp) {
  long long end = dest.back().time();
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

EventList readASCII(const char * filename);

//GSList::GSList()
//  : evtList()
//  , dataGS()
//{}
//
//GSList::GSList(const std::vector <TimedEvent> &events, const Epoch::DateTime &start)
//  : evtList(events)
//  , dataGS(start)
//{}
//
//GSList GSList::copy(ll_int from, ll_int to) const {
//  if(to < from) {
//    std::swap(from, to);
//  }
//  if(to <= 0) {
//    return GSList();
//  }
//  auto newList = evtList.copy(from, to);
//  Epoch::DateTime dtt = dataGS;
//  if(from > 0) {
//    dtt.addSec(from);
//    for(auto &evt : copied) {
//      evt.first -= tousec(from);
//    }
//  }
//  return GSList(copied, dtt);
//}
//
//long tousec(long sec) {
//  return sec / 1E-9;
//}
//
//std::vector <int> GSList::getEventHist() const {
//  std::vector <int> bin(2048, 0);
//  for (auto &evt : this->evtList) {
//    ++bin[evt.second];
//  }
//  return bin;
//}
//
//Epoch::DateTime GSList::getDateTime() const {
//  return dataGS;
//}
//
//double GSList::getDT() const {
//  return evtList.back().first * 1E-9;
//}
//
//GSList& GSList::erase(long from, long to) {
//  if(to < from) {
//    std::swap(from, to);
//  }
//  if(to <= 0) {
//    return *this;
//  }
//  auto eraseFrom = std::lower_bound(evtList.begin(), evtList.end(), tousec(from), isBefore);
//  auto eraseTo = std::lower_bound(evtList.begin(), evtList.end(), tousec(to), isBefore);
//  evtList.erase(eraseFrom, eraseTo);
//  if(from <= 0) {
//    dataGS.addSec(to);
//    for(auto &evt : evtList) {
//      evt.first -= tousec(to);
//    }
//  }
//  return *this;
//}
//
//GSList& GSList::merge(const GSList &gsl) {
//  long offset = tousec(std::abs(Epoch::toUnix(dataGS) - Epoch::toUnix(gsl.dataGS)));
//  if(dataGS > gsl.dataGS) {
//    for(auto &evt : evtList) {
//      evt.first += offset;
//    }
//    for(auto evt : gsl.evtList) {
//      evtList.push_back(evt);
//    }
//    dataGS = gsl.dataGS;
//  }
//  else {
//    for(auto evt : gsl.evtList) {
//      evt.first += offset;
//      evtList.push_back(evt);
//    }
//  }
//  std::sort(evtList.begin(), evtList.end());
//  return *this;
//}
//
//void GSList::writeGSL(const char * nomeFile) const {
//  std::ofstream outfile;
//  outfile.open(nomeFile);
//  if (outfile) {
//    outfile << "#GammaStream 1.0 LIST\n";
//    outfile << "#StartTime: ";
//    outfile << dataGS.year() << '-' << dataGS.month() << '-';
//    outfile << dataGS.day() << 'T' << Epoch::toTime(dataGS) << '\n';
//    outfile << "#Fields: Time\tEnergy\tGain" << '\n';
//    for (auto &evt : this->evtList) {
//      outfile << std::right << std::setw(9) << evt.first / 16 << '\t';   // 1 ciclo clock = 16us
//      outfile << std::setw(6) << evt.second << '\t';
//      outfile << "1.000" << '\n';
//    }
//  }
//}
//
//Spectrometry::Spectrum toSpectrum(const GSList &gsl) {
//  return Spectrometry::Spectrum(gsl.getEventHist(), gsl.getDateTime(), gsl.getDT());
//}
//
//void writeSPE(const GSList &lst, const char * nomeFile) {
//  return writeSPE(toSpectrum(lst), nomeFile);
//}
//
//void writeSPT(const GSList &lst, const char * nomeFile) {
//  return writeSPT(toSpectrum(lst), nomeFile);
//}
//
//GSList readGSL(const char * nomeFile) {
//  std::ifstream file(nomeFile);
//  if (!file) {
//    throw std::invalid_argument("Unable to open file!");
//  }
//  std::vector <TimedEvent> coppie;
//  Epoch::DateTime dataGS;
//
//  std::string token;
//  do {
//    file >> token;
//    if (token.compare("#StartTime:") == 0) {
//      int yr, mn, dy;
//      char sep;
//      Epoch::Time tm;
//      file >> yr >> sep >> mn >> sep >> dy >> sep >> tm;
//      Epoch::Date dt(dy, mn, yr);
//      dataGS = Epoch::DateTime(dt, tm);
//    }
//  } while (token.compare("Gain") != 0);
//  long timeTok;
//  double gainTok;
//  int energyTok;
//  while (file >> timeTok >> energyTok >> gainTok) {
//    coppie.push_back(std::make_pair(timeTok * 16, energyTok));  // 1 ciclo clock = 16us
//  }
//  file.close();
//  return GSList(coppie, dataGS);
//}
//
//bool isBefore(Event evt, ll_int val) {
//  return (evt.first < val);
//}
//
}
