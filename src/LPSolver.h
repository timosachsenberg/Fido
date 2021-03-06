#ifndef _LPSolver_H
#define _LPSolver_H

#include <fstream>
#include "Numerical.h"
#include "GramSchmidt.h"
#include "Random.h"

class LPSolver
{
public:
  virtual ~LPSolver() {};
  virtual Vector solve() = 0;

  virtual void loadArray(istream & fin);
  virtual void load(istream & fin);
  //  void saveArray(ofstream & fout);

  virtual void printResult() const;

  virtual void outputMathematica(char * fname) const;

  class ZeroVectorException :public exception {
   virtual const char* what() const throw()
   {
    return "LPSolver: zero vector exception";
   }
  };

protected:
  virtual void normalizeLP();
  virtual void addPositivityConstraints();

  Matrix A;
  Vector f, b;
  Vector x;
  int n;
  int iterations;

  Array<string> names;
};

#endif

