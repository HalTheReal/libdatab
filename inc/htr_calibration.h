#ifndef HTR_CALIBRATION
#define HTR_CALIBRATION

#include "htr_spectrum.h"
#include "htr_function.h"

typedef double (*calFunction)(const Spectrometry::Spectrum &, double, double);
double NaI(const Spectrometry::Spectrum &sp, double mC, double qC);
double CeBr3(const Spectrometry::Spectrum &sp, double mC, double qC);

double stateChangeProb(double en1, double en2, double temp);

MMZ::Line annealingCal(const Spectrometry::Spectrum &sp, double minM, double maxM, double minQ, double maxQ, calFunction sysEnergy);
MMZ::Line NaICal(const Spectrometry::Spectrum &sp, double minM, double maxM, double minQ, double maxQ);
MMZ::Line CeBr3Cal(const Spectrometry::Spectrum &sp, double minM, double maxM, double minQ, double maxQ);

#endif

