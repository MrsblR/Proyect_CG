#include "GeometryTypes.hpp"

// Devuelve la transposici�n de la matriz 4x4.

Matrix44 Matrix44::getTransposed() const
{
  Matrix44 t;
  
  for (int i=0;i<4; i++)
    for (int j=0;j<4;j++)
      t.mat[i][j] = mat[j][i];
    
  return t;
}
// Devuelve una matriz identidad 4x4.
Matrix44 Matrix44::identity()
{
  Matrix44 eye;
  
  for (int i=0;i<4; i++)
    for (int j=0;j<4;j++)
      eye.mat[i][j] = i == j ? 1 : 0;
  
  return eye;
}
// Devuelve la matriz 4x4 que representa la inversi�n de la rotaci�n y la traslaci�n.
Matrix44 Matrix44::getInvertedRT() const
{
  Matrix44 t = identity();
  
  for (int col=0;col<3; col++)
  {
    for (int row=0;row<3;row++)
    { 
      t.mat[row][col] = mat[col][row];
    }
    
    t.mat[3][col] = - mat[3][col];
  }
  return t;
}
// Devuelve una matriz identidad 3x3.
Matrix33 Matrix33::identity()
{
  Matrix33 eye;
  
  for (int i=0;i<3; i++)
    for (int j=0;j<3;j++)
      eye.mat[i][j] = i == j ? 1 : 0;
  
  return eye;
}
// Devuelve la transposici�n de la matriz 3x3.
Matrix33 Matrix33::getTransposed() const
{
  Matrix33 t;
  
  for (int i=0;i<3; i++)
    for (int j=0;j<3;j++)
      t.mat[i][j] = mat[j][i];
  
  return t;
}
// Devuelve un vector 3D con todos los componentes en 0.
Vector3 Vector3::zero()
{
  Vector3 v = { 0,0,0 };
  return v;
}
// Devuelve el vector 3D negado.
Vector3 Vector3::operator-() const
{
  Vector3 v = { -data[0],-data[1],-data[2] };
  return v;
}

Transformation::Transformation()
: m_rotation(Matrix33::identity())
, m_translation(Vector3::zero())
{
  
}
// Constructor que inicializa la transformaci�n con una matriz de rotaci�n y un vector de traslaci�n dados.
Transformation::Transformation(const Matrix33& r, const Vector3& t)
: m_rotation(r)
, m_translation(t)
{
  
}
// Devuelve una referencia a la matriz de rotaci�n en la transformaci�n.
Matrix33& Transformation::r()
{
  return m_rotation;
}
// Devuelve una referencia al vector de traslaci�n en la transformaci�n.
Vector3&  Transformation::t()
{
  return  m_translation;
}
// Devuelve una referencia constante a la matriz de rotaci�n en la transformaci�n.
const Matrix33& Transformation::r() const
{
  return m_rotation;
}
// Devuelve una referencia constante al vector de traslaci�n en la transformaci�n.
const Vector3&  Transformation::t() const
{
  return  m_translation;
}
// Devuelve la transformaci�n como una matriz 4x4.
Matrix44 Transformation::getMat44() const
{
  Matrix44 res = Matrix44::identity();
  
  for (int col=0;col<3;col++)
  {
    for (int row=0;row<3;row++)
    {
      res.mat[row][col] = m_rotation.mat[row][col];
    }
    
    res.mat[3][col] = m_translation.data[col];
  }
  
  return res;
}
// Devuelve la transformaci�n invertida (rotaci�n transpuesta y traslaci�n negada).
Transformation Transformation::getInverted() const
{
  return Transformation(m_rotation.getTransposed(), -m_translation); 
}