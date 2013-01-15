#ifndef __INCLUDED__MATRIX_HPP
#define __INCLUDED__MATRIX_HPP

#include "scpp_refcountptr.hpp"
#include <gsl/gsl_matrix.h>
#include <memory>
#include <iomanip>
#include <cassert>

typedef unsigned int uint; 

class GslMatrix {
  gsl_matrix *_GM;
public:
  GslMatrix(uint rows, uint columns) : _GM(gsl_matrix_alloc(rows, columns)) {}
  GslMatrix(const gsl_matrix *other) : _GM(gsl_matrix_alloc(other->size1, other->size2)) {
    gsl_matrix_memcpy(_GM, other);
  }
  GslMatrix(const GslMatrix &other) : _GM(gsl_matrix_alloc(other._GM->size1, other._GM->size2)) {
    gsl_matrix_set_all(_GM, 0.0);
  }
  GslMatrix& operator=(const gsl_matrix *other) {
    gsl_matrix_free(_GM);
    _GM = gsl_matrix_alloc(other->size1, other->size2);
    gsl_matrix_memcpy(_GM, other);
  }
  GslMatrix& operator=(const GslMatrix &other) {
    gsl_matrix_free(_GM);
    _GM = gsl_matrix_alloc(other._GM->size1, other._GM->size2);
    gsl_matrix_memcpy(_GM, other._GM);
  }
  ~GslMatrix() {
    gsl_matrix_free(_GM);
  }
  operator gsl_matrix * () {
    gsl_matrix *ret = gsl_matrix_alloc(_GM->size1, _GM->size2);
    gsl_matrix_memcpy(ret, _GM);
    return ret;
  }
  friend std::ostream& operator<<(std::ostream &out, const GslMatrix &gm) {
    for(int row=0;row<gm._GM->size1;++row) {
      out << std::setprecision(3) << gsl_matrix_get(gm._GM, row, 0);
      for(int col=1;col<gm._GM->size2;++col)
        out << " | " << std::setprecision(3) << gsl_matrix_get(gm._GM, row, col);  
      out << "\n";
    }
    return out;
  }
  // friend void swap(GslMatrix& first, GslMatrix& second) {
  //   // http://stackoverflow.com/a/3279550/547223
  //   using std::swap;

  //   swap(first.mSize, second.mSize); 
  //   swap(first.mArray, second.mArray);
  // }
};


#endif