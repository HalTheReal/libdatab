#ifndef HTR_CALIBRATION
#define HTR_CALIBRATION

#include "htr_spectrum.h"
#include "htr_function.h"

MMZ::Line annealingCal(const Spectrometry::Spectrum &sp, double minM, double maxM, double minQ, double maxQ);
double sysEnergy(const Spectrometry::Spectrum &sp, double mC, double qC);
double stateChangeProb(double en1, double en2, double temp);

#endif

