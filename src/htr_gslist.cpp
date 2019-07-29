#include "htr_gslist.h"
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

void timeSort(EventList &evl) {
  evl.sort([](const TimedEvent &ev1, const TimedEvent &ev2) -> bool {
    return ev1.time() < ev2.time();
    });
}

////EventList timeCut(EventList &evl, long long from, long long to) {
//  if(to < from) {
//    std::swap(from, to);
//  }
//  auto cutFrom = std::lower_bound(evl.begin(), evl.end(), from, isBeforeTime);
//  auto cutTo = std::lower_bound(evl.begin(), evl.end(), to, isBeforeTime);
//  EventList cut;
//  cut.splice(cut.begin(), evl, cutFrom, cutTo);
//  return cut;
//}
//
//EventList timeCopy(const EventList &evl, long long from, long long to) {
//  if(to < from) {
//    std::swap(from, to);
//  }
//  auto copyFrom = std::lower_bound(evl.begin(), evl.end(), from, isBeforeTime);
//  auto copyTo = std::lower_bound(evl.begin(), evl.end(), to, isBeforeTime);
//  return EventList(copyFrom, copyTo);
//}
//
//void timeShift(EventList &evl, long long offset) {
//  for (auto &event : evl) {
//    shift(event, offset);
//  }
//}
//
//void append(EventList &dest, EventList &toApp) {
//  long long end = dest.back().time();
//  timeShift(toApp, end);
//  dest.merge(toApp, isBeforeEvent);
//}
//
//std::vector <int> toHistogram(const EventList &evl, std::size_t channels) {
//  std::vector <int> hist(channels, 0);
//  for (auto &evt : evl) {
//    if (evt.energy() < channels) {
//      ++hist[evt.energy()];
//    }
//  }
//  return hist;
//}
//
//EventList readASCII(const char * filename);

//GSList::GSList()
//  : eventList()
//  , dataGS()
//{}
//
//GSList::GSList(const EventList &evl, const Epoch::DateTime &start)
//  : eventList(evl)
//  , dataGS(start)
//{}
//
//GSList GSList::copy(int fromSec, int toSec) const {
//  if(toSec < fromSec) {
//    std::swap(fromSec, toSec);
//  }
//  if(toSec <= 0) {
//    return GSList();
//  }
//  EventList newList = timeCopy(eventList, fromSec, toSec);
//  Epoch::DateTime dtt = dataGS;
//  if(fromSec > 0) {
//    dtt.addSec(fromSec);
//    timeShift(newList, - tousec(fromSec));
//  }
//  return GSList(newList, dtt);
//}
//
//long GSList::tousec(int sec) const {
//  return sec / 1E-9;
//}
//
//
//std::vector <int> GSList::getHistogram() const {
//  return toHistogram(eventList, 2048);
//}
//
//Epoch::DateTime GSList::getDateTime() const {
//  return dataGS;
//}
//
//double GSList::getDT() const {
//  return eventList.back().time() * 1E-9;
//}
//
//GSList& GSList::erase(long from, long to) {
//}
//
//GSList& GSList::merge(const GSList &gsl) {
//  long offset = tousec(std::abs(Epoch::toUnix(dataGS) - Epoch::toUnix(gsl.dataGS)));
//  if(dataGS > gsl.dataGS) {
//    for(auto &evt : eventList) {
//      evt.first += offset;
//    }
//    for(auto evt : gsl.eventList) {
//      eventList.push_back(evt);
//    }
//    dataGS = gsl.dataGS;
//  }
//  else {
//    for(auto evt : gsl.eventList) {
//      evt.first += offset;
//      eventList.push_back(evt);
//    }
//  }
//  std::sort(eventList.begin(), eventList.end());
//  return *this;
//}
//
//void GSList::writeGSL(const char * nomeFile) const {
//  std::ofstream outfile;
//  outfile.open(nomeFile);
//  if (outfile) {
//    outfile << "#GammaStream 1.5 LIST\n";
//    outfile << "#StartTime: ";
//    outfile << dataGS.year() << '-' << dataGS.month() << '-';
//    outfile << dataGS.day() << 'T' << Epoch::toTime(dataGS) << '\n';
//    outfile << "#Fields: Time\tEnergy\tGain" << '\n';
//    for (auto &evt : this->eventList) {
//      outfile << std::right << std::setw(9) << evt.time() << '\t';
//      outfile << std::setw(6) << evt.energy() << '\t';
//      outfile << "1.000" << '\n';
//    }
//  }
//}
//
//Spectrometry::Spectrum toSpectrum(const GSList &gsl) {
//  return Spectrometry::Spectrum(gsl.getHistogram(), gsl.getDateTime(), gsl.getDT());
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

}
