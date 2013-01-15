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

  GslMatrix c(2,3);
  GslMatrix d(3,2);
  GslMatrix cd(2,2);
  c.set( 0,0, 1);
  c.set( 0,1, 0);
  c.set( 0,2, 2);
  c.set( 1,0,-1);
  c.set( 1,1, 3);
  c.set( 1,2, 1);
  d.set( 0,0, 3);
  d.set( 0,1, 1);
  d.set( 1,0, 2);
  d.set( 1,1, 1);
  d.set( 2,0, 1);
  d.set( 2,1, 0);
  cd.set(0,0, 5);
  cd.set(0,1, 1);
  cd.set(1,0, 4);
  cd.set(1,1, 2);
  GslMatrix c2 = c.mul(d);
  cout << cd << "\n== ?\n" << c2 << "\n\n" << c.inv();

  return 0;
}