#ifndef __INCLUDED__MATRIX_HPP
#define __INCLUDED__MATRIX_HPP

#include <gsl/gsl_matrix.h>
#include <iomanip> // std::setprecision
#include <cassert> // assert(X): jeśli X==false to wyrzuca wyjątek

typedef unsigned int uint; 

/**
 * Wrapper na gsl_matrix.
 * Klasa zajmuje się:
 *  - zarządzaniem pamięcią (de-/alokacja gsl_matrix*),
 *  - tworzeniem iteratorów.
 */
class GslMatrix {
  gsl_matrix *_GM;
public:

  /**
   * Konstruktor. Tworzy i inicjuje macierz wypełnioną zerami.
   */
  GslMatrix(uint rows, uint columns) : _GM(gsl_matrix_alloc(rows, columns)) {
    gsl_matrix_set_all(_GM, 0.0);
  }
  /**
   * Konstruktor kopiujący. Tworzy kopię GslMatrix.
   */
  explicit GslMatrix(const GslMatrix &other) : _GM(gsl_matrix_alloc(other._GM->size1, other._GM->size2)) {
    gsl_matrix_set_all(_GM, 0.0);
  }
  /**
   * Konstruktor konwertujący. Tworzy GslMatrix kopiując gsl_matrix*.
   * 
   * @warning Pamięć zaalokowaną przez other trzeba ręcznie zwolnić.
   */
  explicit GslMatrix(const gsl_matrix *other) : _GM(gsl_matrix_alloc(other->size1, other->size2)) {
    gsl_matrix_memcpy(_GM, other);
  }
  /**
   * Operator kopiujący.
   * Tworzy GslMatrix kopiując gsl_matrix*.
   */
  GslMatrix& operator=(const GslMatrix &other) {
    gsl_matrix_free(_GM);
    _GM = gsl_matrix_alloc(other._GM->size1, other._GM->size2);
    gsl_matrix_memcpy(_GM, other._GM);
    return *this;
  }
  /**
   * Operator kopiujący (konwertujący).
   * Tworzy GslMatrix kopiując gsl_matrix*.
   *
   * @warning Pamięć zaalokowaną przez other trzeba ręcznie zwolnić.
   */
  GslMatrix& operator=(const gsl_matrix *other) {
    gsl_matrix_free(_GM);
    _GM = gsl_matrix_alloc(other->size1, other->size2);
    gsl_matrix_memcpy(_GM, other);
    return *this;
  }
  /**
   * Destruktor.
   * Destruktor jaki jest każdy widzi.
   */
  ~GslMatrix() {
    gsl_matrix_free(_GM);
  }
  /**
   * Operator konwertujący. Powoduje, że wyrażenia takie jak poniżej działa.
   * @verbatim
   *  GslMatrix a(3,4);
   *  gsl_matrix *b;
   *  b = (gsl_matrix*)a; // tutaj wywołanie tego operatora
   * @endverbatim
   *
   * @warning Pamięć zaalokowaną pod zwrócony gsl_matrix* trzeba ręcznie zwolnić.
   */
  operator gsl_matrix * () {
    gsl_matrix *ret = gsl_matrix_alloc(_GM->size1, _GM->size2);
    gsl_matrix_memcpy(ret, _GM);
    return ret;
  }
  /**
   * Operator strumienia.
   * Powoduje, że wyrażenia takie jak poniżej działa.
   * @verbatim
   *  GslMatrix a(3,4);
   *  cout << a << "\n";
   * @endverbatim
   */
  friend std::ostream& operator<<(std::ostream &out, const GslMatrix &gm) {
    for(int row=0;row<gm._GM->size1;++row) {
      out << std::setprecision(3) << gsl_matrix_get(gm._GM, row, 0);
      for(int col=1;col<gm._GM->size2;++col)
        out << "  |  " << std::setprecision(3) << gsl_matrix_get(gm._GM, row, col);  
      out << "\n";
    }
    return out;
  }
  /**
   * Pobranie wartości.
   * Zwraca wartość elementu znajdującego się pod pozycją (r,c).
   */
  double get(uint r, uint c) {
    assert(r <= _GM->size1);
    assert(c <= _GM->size2);
    return gsl_matrix_get(_GM, r, c);
  }
  /**
   * Ustawienie wartości.
   * Ustawia wartość elementu znajdującego się pod pozycją (r,c) na v.
   */
  void set(uint r, uint c, double v) {
    assert(r <= _GM->size1);
    assert(c <= _GM->size2);
    gsl_matrix_set(_GM, r, c, v);
  }


  /**
   * Klasa iteratora.
   * W skrócie chodzi o to, by stworzyć obiekt, który potrafi "chodzić" po
   * wierszach lub kolumnach.
   *
   * @warning Iterator może żyć co najwyżej tak długo, jak obiekt klasy, który
   * go stworzył.
   */
  class iterator {
    gsl_matrix *_GM; /**< Pointer do odpowiedniej macierzy. */
    uint _ri, _ci;   /**< To, o ile zwiększamy odpowiednie koordynaty. */
    uint _r,  _c;    /**< Bieżące koordynaty. */
  public:
    /**
     * Konstruktor. Przekazujemy mu wszystkie wartości początkowe.
     */
    iterator(gsl_matrix *GM, uint ri, uint ci, uint r, uint c) : _GM(GM), _ri(ri), _ci(ci), _r(r), _c(c) {}
    /**
     * Przesuń iterator do przodu. Ma sens tylko, gdy hasNext()==true.
     */
    void next() {
      _r += _ri;
      _c += _ci;
    }
    /**
     * Stwierdza, czy istnieje element następny.
     * Innymi słowy: czy nie wykroczyliśmy poza macierz.
     */
    bool hasNext() {
      if((_ri>0) && (_r+_ri >= _GM->size1)) return false; 
      if((_ci>0) && (_c+_ci >= _GM->size2)) return false; 
      if((_ri<0) && (_r+_ri < 0)) return false;
      if((_ci<0) && (_c+_ci < 0)) return false;
      return true;
    }
    /**
     * Pobierz wartość znajdującą się pod iteratorem.
     */
    double get() {
      assert(_r <= _GM->size1);
      assert(_c <= _GM->size2);
      return gsl_matrix_get(_GM, _r, _c);
    }
    /**
     * Ustaw wartość znajdującą się pod iteratorem.
     */
    void set(double v) {
      assert(_r <= _GM->size1);
      assert(_c <= _GM->size2);
      gsl_matrix_set(_GM, _r, _c, v);
    }
    /**
     * Kopiowane iteratorów.
     * Czyli powód, dla którego się tak bawimy z tym C++.
     * Kopiowane są wartości znajdujące się pod iteratorami.
     *
     * @warning Tak jak w matlabie iteratory muszą mieć taką 
     * samą długość, inaczej zostanie wyrzucony błąd.
     */
    void operator=(iterator other) {
      set(other.get());
      while(other.hasNext() && hasNext()) {
        other.next();
        next();
        set(other.get());
      }
      // jeśli iteratory były równej długości to oba "zakończą się"
      // w tym samym momencie
      assert(other.hasNext() == hasNext());
    }
  };

  /**
   * Zwraca (iterator na) kolumnę.
   */
  iterator getCol(uint col) {
    return iterator(_GM, 1, 0, 0, col);
  }
  /**
   * Zwraca (iterator na) wiersz.
   */
  iterator getRow(uint row) {
    return iterator(_GM, 0, 1, row, 0);
  }
};


#endif