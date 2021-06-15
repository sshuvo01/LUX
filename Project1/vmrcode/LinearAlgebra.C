//*******************************************************************
//
//   LinearAlgebra.C
//
//
//   Performs linear algebra operation: vectors combined with matrices
//   Also other operations such as exponentiation, trace, determinant
//   and outer product.
//
//*******************************************************************




#include "LinearAlgebra.h"



namespace lux {

const Vector operator* ( const Vector& v, const Matrix& m )
{
   const double a = v(0)*m(0,0) + v(1)*m(1,0) + v(2)*m(2,0);
   const double b = v(0)*m(0,1) + v(1)*m(1,1) + v(2)*m(2,1);
   const double c = v(0)*m(0,2) + v(1)*m(1,2) + v(2)*m(2,2);
   return Vector( a, b, c );
}

const Vector operator* ( const Matrix& m, const Vector& v )
{
   const double a = v(0)*m(0,0) + v(1)*m(0,1) + v(2)*m(0,2);
   const double b = v(0)*m(1,0) + v(1)*m(1,1) + v(2)*m(1,2);
   const double c = v(0)*m(2,0) + v(1)*m(2,1) + v(2)*m(2,2);
   return Vector( a, b, c );
}
	

// outer product
const Matrix operator& (const Vector& v1, const Vector& v2 )
{
    double m[3][3];
    m[0][0] = v1(0)*v2(0);
    m[1][0] = v1(1)*v2(0);
    m[2][0] = v1(2)*v2(0);
    m[0][1] = v1(0)*v2(1);
    m[1][1] = v1(1)*v2(1);
    m[2][1] = v1(2)*v2(1);
    m[0][2] = v1(0)*v2(2);
    m[1][2] = v1(1)*v2(2);
    m[2][2] = v1(2)*v2(2);
    return Matrix( m );
}

// outer product in place
void outer_product( const Vector& v1, const Vector& v2, Matrix& m )
{
    m.Set( 0, 0, v1(0)*v2(0) );
    m.Set( 0, 1, v1(0)*v2(1) );
    m.Set( 0, 2, v1(0)*v2(2) );
    m.Set( 1, 0, v1(1)*v2(0) );
    m.Set( 1, 1, v1(1)*v2(1) );
    m.Set( 1, 2, v1(1)*v2(2) );
    m.Set( 2, 0, v1(2)*v2(0) );
    m.Set( 2, 1, v1(2)*v2(1) );
    m.Set( 2, 2, v1(2)*v2(2) );
}



// constructing a rotation matrix

const Matrix rotation( const Vector& axis, const double angle )
{
   const double cosa = cos(angle);
   const double sina = sin(angle);
   Vector ax = axis / sqrt( axis*axis ); 
   Matrix op;
   outer_product( ax, ax, op );
   Matrix result = unitMatrix() * cosa 
                 + op * ( 1.0 - cosa )
                 + PauliMatrix[0]*axis[0] * sina 
                 + PauliMatrix[1]*axis[1] * sina 
                 + PauliMatrix[2]*axis[2] * sina;
   return result;
}


const Matrix unitMatrix()
{
  Matrix one;
  one.Set(0,0, 1.0);
  one.Set(1,1, 1.0);
  one.Set(2,2, 1.0);
  return one;
}



const Matrix exp( const Matrix& m ){ return m.exp(); }
const Matrix inverse( const Matrix& m ) { return m.inverse(); }
const double det( const Matrix& m ) { return m.det(); }
const double trace( const Matrix& m ) { return m.trace(); }

 
}




