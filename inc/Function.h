#ifndef MY_MMZ
#define MY_MMZ

#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <iostream>       // DA RIMUOVERE
#include <stdexcept>

namespace MMZ {

class Function {
  public:
    Function();                                   // Mandatorio per classi derivate
    Function(std::initializer_list<double> il);
    Function(const std::vector <double> &prs);    // Mandatorio per classi derivate
    virtual ~Function();
    double operator()(double in) const;
    unsigned parNum() const;
    double par(unsigned idx) const;
  private:
    std::vector <double> parameters;
    virtual double myEval(double xx) const = 0;   // Mandatorio per classi derivate
};

class Gauss : public Function {
  public:
    Gauss();
    Gauss(const std::vector <double> &prs);
    Gauss(double aa, double bb, double cc);
  private:
    double myEval(double xx) const override;
};

class Poly : public Function {
  public:
    Poly();
    Poly(const std::vector <double> &prs);
    Poly(std::initializer_list<double> il);
    virtual ~Poly();
  private:
    double myEval(double xx) const override;
};

class Line : public Poly {
  public:
    Line();
    Line(const std::vector <double> &prs);
    Line(double ml, double ql);
    double getM() const;
    double getQ() const;
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
  // check lower.parNum() == upper.parNum() ? non serve perchè Fn dev'essere lo stesso tipo
  // serve se confronto due polinomi di grado diverso
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
double mse(const std::vector <double> &xx, const std::vector <double> &ob, const Function &fn);
double rmse(const std::vector <double> &xx, const std::vector <double> &ob, const Function &fn);

// Confronto generale sul secondo elemento di std::pair
template <typename T1, typename T2, typename Pred = std::less<T2>>
struct sort_second_pred {
  bool operator()(const std::pair<T1, T2> &left, const std::pair<T1, T2> &right) {
    Pred p;
    return p(left.second, right.second);
  }
};

template <typename Fn>
bool isWithin(const Fn &test, const Fn &lower, const Fn &upper) {
  for( int i = 0; i < test.parNum(); ++i) {
    if(test.par(i) < lower.par(i)) {
      return false;
    }
    if(test.par(i) > upper.par(i)) {
      return false;
    }
  }
  return true;
}

template <typename Fn>
Fn fnMix(const std::vector<std::pair<Fn, double>> &shuf, size_t n) {
  int nPars = shuf.begin()->first.parNum();
  std::vector<double> newPars(nPars, 0);
  for (int i = 0; i < n-1; ++i) {
    for(int j = 0; j < nPars; ++j) {
      newPars[j] += shuf[i].first.par(j);
    }
  }
  for(int i = 0; i < nPars; ++i) {
    newPars[i] /= (n - 1);              // Calcolo centroide G
    newPars[i] *= 2;                    // 2G
    newPars[i] -= shuf[n].first.par(i); // 2G - shuf[end]
  }
  return Fn(newPars);
}

template <typename Fn>
Fn CRSFit(const Fn &fL, const Fn &fH, const std::vector <double> &xx, const std::vector <double> &ob, fitnessFunction fitF = mse) {
  FunctionGenerator <Fn> fnGen(fL, fH);
  int N = 10 * (fL.parNum() + 1);                 // Numero soluzioni random iniziali
  std::vector<std::pair<Fn, double>> NSol(N);     // Insieme delle soluzioni random e valore del fit
  for(int i = 0; i < N; ++i) {
    auto frnd = fnGen.generate();
    NSol[i] = std::make_pair(frnd, fitF(xx, ob, frnd));
  }
  int iter = 0;
  while(iter < 100000) {
    std::sort(NSol.begin(), NSol.end(), sort_second_pred<Fn, double>()); // LE = *NSol.begin(), ME = *NSol.end()
    if(fabs(NSol[0].second - NSol[N-1].second) < 1e-6) {
      break;
    }
    auto shuffled = NSol;                                                // Vettore da mescolare
    std::random_shuffle(shuffled.begin() + 1, shuffled.end());  // Mescolo il vettore senza toccare il primo elemento, cioè LE!
    auto newFn = fnMix(shuffled, fL.parNum() + 1);              // Servono almeno parNum + 1 punti per convergere velocemente
    if(isWithin(newFn, fL, fH)) {
      double fitness = fitF(xx, ob, newFn);
      if(fitness < NSol[N-1].second) {
        NSol[N-1] = std::make_pair(newFn, fitness);
      }
    }
    ++iter;
  }
  std::sort(NSol.begin(), NSol.end(), sort_second_pred<Fn, double>());
//  for(auto p : NSol) {    // Print del set finale di soluzioni
//    for(int i = 0; i < fL.parNum(); ++i) {
//      std::cout << "par(" << i << "): " << p.first.par(i) << ' ';
//    }
//    std::cout << "F: " << p.second << '\n';
//  }
  return NSol[0].first; // Ritorno il migliore
}

double changeProb(double en1, double en2, double temp);

template <typename Fn>
Fn SAFit(const Fn &fL, const Fn &fH, const std::vector <double> &xx, const std::vector <double> &ob, fitnessFunction fitF = mse) {
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
