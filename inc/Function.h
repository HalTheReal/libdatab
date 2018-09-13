#ifndef MY_MMZ
#define MY_MMZ

#include <vector>
#include <chrono>
#include <random>
#include <memory>

namespace MMZ {

class Function {
  public:
    Function(std::vector <double> prs);
    double eval(double in) const;
    double par(unsigned idx) const;
  private:
    std::vector <double> parameters;
    virtual double myEval(double xx) const = 0;
};

class Line : public Function {
  public:
    Line(double ml, double mq);
    double getM() const;
    double getQ() const;
  private:
    double myEval(double xx) const override;
};

}

#endif
