#include <iostream>
#include <gsl/gsl_matrix.h>
#include "matrix.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
  GslMatrix a(3,4);
  a.set(0,0,1);
  a.set(0,1,2);
  a.set(0,2,3);
  a.set(0,3,4);
  cout << a;
  //a[1][':'] = b[':'][2];
  GslMatrix::iterator it = a.getRow(0);
  cout << it.get();
  it.next();
  cout << it.get();
  it.next();
  it.set(9);
  cout << "\n" << a;
  a.getRow(2) = a.getRow(0);
  cout << "\n" << a;
  GslMatrix b(4,5);
  b.getCol(0) = a.getRow(0);
  cout << "\n" << b;
  return 0;
}