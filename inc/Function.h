#ifndef MY_MMZ
#define MY_MMZ

#include <vector>
#include <chrono>
#include <random>

namespace MMZ {

class Function {
  public:
    Function();
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
    Line();
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

// Puntatore a funzione di fitness
typedef double (*fitnessFunction)(const std::vector <double> &, const std::vector <double> &, const Function &);
double r_squared(const std::vector <double> &xx, const std::vector <double> &ob, const Function &fn);

double changeProb(double en1, double en2, double temp);

template <typename Fn>
Fn SAFit(const Fn &fL, const Fn &fH, const std::vector <double> &xx, const std::vector <double> &ob, fitnessFunction fitF = r_squared) {
  double temp = 100;
  FunctionGenerator <Fn> fnGen(fL, fH);
  std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution <double> distChange(0, 1);
  Fn guess = fnGen.generate();
  double energy = fitF(xx, ob, guess);
  do {
    Fn newGuess = fnGen.generate();
    double newEnergy = fitF(xx, ob, newGuess);
    if(changeProb(energy, newEnergy, temp) >= distChange(generator)) {
      energy = newEnergy;
      guess = newGuess;
    }
    temp -= 0.0001;
  } while (temp > 0);
  return guess;
}

}

#endif
