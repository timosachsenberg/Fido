// member functions
#include <functional> 

template <typename T>
Array<T>::Array(int n) :
  data(n)
{
}

template <typename T>
Array<T>::Array(int n, const T & element) :
  data(n, element)
{
}

template <typename T>
const T & Array<T>::operator [] (int k) const
{
  boundsCheck(k);
  return data[k];
}

template <typename T>
T & Array<T>::operator [] (int k)
{
  boundsCheck(k);
  return data[k];
}

template <typename T>
void Array<T>::add(const T & element)
{
  data.push_back( element );
}

template <typename T>
void Array<T>::append(const Array<T> & elements)
{
  for (int k=0; k<elements.size(); k++)
    {
      add(elements[k]);
    }
}

template <typename T>
T & Array<T>::back()
{
  boundsCheck(0);
  return data.back();
}

template <typename T>
const T & Array<T>::back() const
{
  boundsCheck(0);
  return data.back();
}

template <typename T>
void Array<T>::clear()
{
  data.clear();
}

template <typename T>
void Array<T>::remove(int k)
{
  boundsCheck(k);
  
  int j;
  for (j=k; j<size()-1; j++)
    {
      data[j] = data[j+1];
    }
  
  resize( size() - 1 );
}


template <typename T>
void Array<T>::resize(int n)
{
  if ( n < 0 )
    throw ResizeException();

  data.resize(n);
}

template <typename T>
int Array<T>::size() const
{
  return data.size();
}

template <typename T>
bool Array<T>::inBounds(int i) const
{
  return i >= 0 && i < size();
}

template <typename T>
void Array<T>::boundsCheck(
#ifdef SAFE_ARRAYS
int i
#else
int
#endif
) const
{
#ifdef SAFE_ARRAYS
  if ( ! inBounds(i) )
    {
      cerr << endl << "size is " << size() << ", index queried was " << i << endl;
      throw OutOfBoundsException();
    }
#endif
}

template <typename T>
//Array<T> Array<T>::operator [](const Set & rhs) const
Array<T> Array<T>::operator [](const Array<int> & rhs) const
{
  Array<T> result(rhs.size());

  int counter = 0;
  for (Array<int>::Iterator iter = rhs.begin(); iter != rhs.end(); iter++, counter++)
    {
      result[ counter ] = (*this)[ *iter ];
    }

  return result;
}

// nonmember functions

// error checking
template <typename T, typename R>
void sizeCheck(const Array<T> & lhs, const Array<R> & rhs)
{
#ifdef SAFE_ARRAYS
  if ( lhs.size() != rhs.size() )
    {
      throw typename Array<T>::SizeException();
    }
#endif
}

// concatonate arrays together lhs to rhs
template <typename T>
Array<T> concatonate(const Array<T> & lhs, const Array<T> & rhs)
{
  Array<T> result(lhs.size() + rhs.size());
  int k;
  for (k=0; k<lhs.size(); k++)
    {
      result[k] = lhs[k];
    }
  
  for (k=0; k<rhs.size(); k++)
    {
      result[ k + lhs.size() ] = rhs[k];
    }
  return result;
}

template <typename T>
ostream & operator <<(ostream & os, const Array<T> & rhs)
{
  if ( rhs.size() == 0 )
    {
      os << '0';
      return os;
    }

  std::streamsize w = os.width();
  os << "{ ";

  int k;
  for (k=0; k<rhs.size(); k++)
    {
      os.width(4);
      os << rhs[k];
      if ( k != rhs.size() - 1 )
	os << " , ";
    }

  os << " }";

  os.width(w);
  return os;
}

template <typename T>
istream & operator >>(istream & is, Array<T> & rhs)
{
  char delim;
  is >> delim;

  rhs.clear();

  // the character for an empty array
  if ( delim == '0')
    return is;

  if ( delim != '{' )
    {
      throw typename Array<T>::FormatException();
    }
  T element;
  
  // now that a { character has been read
  // it is guaranteed that at least one
  // element will occur

  do
    {
      is >> element;
      rhs.add(element);
      is >> delim;
      
      if ( delim != ',' && delim != '}')
	throw typename Array<T>::FormatException();

    } while ( delim != '}' );
  return is;
}

template <typename T>
Set Array<T>::operator ==(const T & rhs) const
{
  Set result;

  for (int k=0; k<size(); k++)
    {
      if ( (*this)[k] == rhs )
	{
	  result.add(k);
	}
    }

  return result;
}

template <typename T>
bool Array<T>::operator ==(const Array<T> & rhs) const
{
  if ( size() != rhs.size() )
    return false;

  for (int k=0; k<size(); k++)
    {
      if ( (*this)[k] != rhs[k] )
	{
	  return false;
	}
    }

  return true;
}

template <typename T>
Array<int> Array<T>::sort()
{
  vector<pair<T, int> > sortie(size());

  int k;
  for (k=0; k<size(); k++)
    {
      sortie[k] = pair<T, int>( (*this)[k], k);
    }

  ::sort( sortie.begin(), sortie.end() , std::greater<pair<T, int> >() );
  
  Array<int> result(size());
  for (k=0; k<size(); k++)
    {
      (*this)[k] = sortie[k].first;
      result[k] = sortie[k].second;
    }

  return result;
}

template <typename T>
Array<int> Array<T>::sortA()
{
  vector<pair<T, int> > sortie(size());

  int k;
  for (k=0; k<size(); k++)
    {
      sortie[k] = pair<T, int>( (*this)[k], k);
    }

  ::sort( sortie.begin(), sortie.end() );
  
  Array<int> result(size());
  for (k=0; k<size(); k++)
    {
      (*this)[k] = sortie[k].first;
      result[k] = sortie[k].second;
    }

  return result;
}

template <typename T>
int Array<T>::sortedFind(T x) const
{
  if ( size() == 0 )
    return -1;
  return sortedFindHelper(0, size()-1, x);
}

template <typename T>
int Array<T>::sortedFindHelper(int low, int high, T x) const
{
  if ( low+1 == high || low == high )
    {
      if ( (*this)[low] == x )
	return low;
      if ( (*this)[high] == x )
	return high;

      //      cerr << "Warning: " << x << " couldn't be found" << endl;
      return -1;
    }

  int middleIndex = (low+high)/2;
  if ( (*this)[middleIndex] < x )
    {
      return sortedFindHelper(middleIndex, high, x);
    }
  else if ( (*this)[middleIndex] > x )
    {
      return sortedFindHelper(low, middleIndex, x);
    }
  else
    {
      return middleIndex;
    }
}

