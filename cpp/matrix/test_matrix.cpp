#include <iostream>
#include <gsl/gsl_matrix.h>
#include "matrix.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
  GslMatrix a(3,4);
  GslMatrix b(4,5);
  cout << a;
  //a[1][':'] = b[':'][2];
  return 0;
}