#include "Function.h"

namespace MMZ {

Function::Function()
  : parameters(0)
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

Poly::Poly()
  : Function()
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
  : Function()
{}

Line::Line(const std::vector <double> &prs)
  : Function(prs)
{
  if (prs.size() != 2) {
    throw std::runtime_error("Invalid Line parameters");
  }
}

Line::Line(double ml, double mq)
  : Function(std::vector <double> {ml, mq})
{}

double Line::getM() const {
  return par(0);
}

double Line::getQ() const {
  return par(1);
}

double Line::myEval(double xx) const {
  return xx * this->getM() + this->getQ();
}

double r_squared(const std::vector <double> &xx, const std::vector <double> &ob, const Function &fn) {
  double rq = 0;
  for(unsigned i = 0; i < xx.size(); ++i) {
    rq += pow(ob[i] - fn(xx[i]), 2); 
  }
  return rq;
}

double changeProb(double en1, double en2, double temp) {
  if(en2 < en1) {
    return 1;
  }
  return exp(-(en2 - en1) / temp);
}

}
