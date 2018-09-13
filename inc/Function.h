#ifndef MY_MMZ
#define MY_MMZ

#include <vector>
#include <chrono>
#include <random>

namespace MMZ {

class Function {
  public:
    Function(const std::vector <double> &prs);
    double eval(double in) const;
    int parNum() const;
    double par(unsigned idx) const;
  private:
    std::vector <double> parameters;
    virtual double myEval(double xx) const = 0;
};

class Line : public Function {
  public:
    Line(const std::vector <double> &prs);
    Line(double ml, double mq);
    double getM() const;
    double getQ() const;
  private:
    double myEval(double xx) const override;
};

template <typename Fn>
class FunctionGenerator {
  public:
    FunctionGenerator(const Fn &lower, const Fn &upper);
    Fn generate();  // NON è const perchè si modifica generator!
  private:
    unsigned seed;
    std::default_random_engine generator;
    std::vector<std::uniform_real_distribution<double>> distributions;
};

template <typename Fn>
FunctionGenerator<Fn>::FunctionGenerator(const Fn &lower, const Fn &upper)
  : seed(std::chrono::system_clock::now().time_since_epoch().count())
  , generator(seed)
  , distributions(0)
{
  // check lower.parNum() == upper.parNum() ?
  for(int i = 0; i < lower.parNum(); ++i) {
    distributions.push_back(std::uniform_real_distribution <double>(lower.par(i), upper.par(i)));
  }
}

template <typename Fn>
Fn FunctionGenerator<Fn>::generate() {
  std::vector <double> newPars(distributions.size());
  for(int i = 0; i < distributions.size(); ++i) {
    newPars[i] = distributions[i](generator);
  }
  return Fn(newPars);
}

}

#endif
