#include "Function.h"

namespace MMZ {

Function::Function(const std::vector <double> &prs)
  : parameters(prs)
{}

double Function::eval(double in) const {
  return myEval(in);
}

int Function::parNum() const {
  return parameters.size();
}

double Function::par(unsigned idx) const {
  return parameters[idx];
}

Line::Line(const std::vector <double> &prs)
  : Function(prs)
{
  // prs.size == 2, altrimenti eccezione!
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

}
