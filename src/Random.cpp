#include "Random.h"

Numerical Random::samplingChecker(1e-10);

double Random::uniform(double a, double b)
{
  if ( a > b )
    {
      if ( samplingChecker.isPos(a-b) )
	{
	  cerr << "Uniform value cannot be sampled in the range [" << a << ", " << b << "]" << endl;
	  throw SamplingException();
	}
      else
	{
	  // they are negligibly close
	  // just return one of the limits
	  return b;
	}
    }

  double u = rand() % 1000000 / 999999.0;
  return u * (b-a) + a;
}

double Random::standardNormal()
{
  double u1 = uniform(0.0, 1.0);
  double u2 = uniform(0.0, 1.0);

  double z = sqrt( -2.0 * log(u1) ) * cos(2*Pi*u2);

  return z;
}

double Random::normal(double mean, double var)
{
  return standardNormal() * sqrt(var) + mean;
}

int Random::inRange(int a, int b)
{
  if ( b < a )
    throw SamplingException();

  if ( b == a )
    return a;

  return rand() % (b-a) + a;
}

void Random::fillRandomUniform(Array<double> & lhs, double low, double high)
{
  for (int k=0; k<lhs.size(); k++)
    {
      lhs[k] = uniform(low, high);
    }
}
