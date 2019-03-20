#include "htr_function.h"

namespace MMZ {

Function::Function()
  : parameters(0)
{}

Function::Function(std::initializer_list<double> il)
 : parameters(il)
{}

Function::Function(const std::vector <double> &prs)
  : parameters(prs)
{}

Function::~Function()
{}

double Function::operator()(double in) const {
  return myEval(in);
}

unsigned Function::parNum() const {
  return parameters.size();
}

double Function::par(unsigned idx) const {
  return parameters[idx];
}

Gauss::Gauss()
  : Function()
{}

Gauss::Gauss(const std::vector <double> &prs)
  : Function(prs)
{
  if (prs.size() != 3) {
    throw std::runtime_error("Invalid Gauss parameters");
  }
}

Gauss::Gauss(double aa, double bb, double cc)
  : Function{aa, bb, cc}
{}

double Gauss::myEval(double xx) const {
  return par(0) * exp(- pow(xx - par(1), 2) / (2 * pow(par(2), 2)));
}

Poly::Poly()
  : Function()
{}

Poly::Poly(std::initializer_list<double> il)
  : Function(il)
{}

Poly::Poly(const std::vector <double> &prs)
  : Function(prs)
{}

Poly::~Poly()
{}

double Poly::myEval(double xx) const {
  double tot = 0;
  for(unsigned i = 0; i < parNum(); ++i) {
    tot += pow(xx, i) * par(i);
  }
  return tot;
}

Line::Line()
  : Poly()
{}

Line::Line(const std::vector <double> &prs)
  : Poly(prs)
{
  if (prs.size() != 2) {
    throw std::runtime_error("Invalid Line parameters");
  }
}

Line::Line(double ml, double ql)
  : Poly{ql, ml}
{}

double Line::getM() const {
  return par(1);
}

double Line::getQ() const {
  return par(0);
}

double mse(const std::vector <double> &xx, const std::vector <double> &ob, const Function &fn) {
  double tot = 0;
  for(unsigned i = 0; i < xx.size(); ++i) {
    tot += pow(ob[i] - fn(xx[i]), 2); 
  }
  return tot / xx.size();
}

double rmse(const std::vector <double> &xx, const std::vector <double> &ob, const Function &fn) {
  return sqrt(mse(xx, ob, fn));
}

double changeProb(double en1, double en2, double temp) {
  if(en2 < en1) {
    return 1;
  }
  return exp(-(en2 - en1) / temp);
}

}
