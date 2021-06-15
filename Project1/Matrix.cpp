//*******************************************************************
//
//   Matrix.h
//
// 3D matrix class in the namespace fvt
//
//*******************************************************************


#include <iostream>
#include <cmath>

#include "Matrix.h"

using namespace lux;

const Matrix Matrix::operator+        (const Matrix& v) const
{
	double newmat[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newmat[i][j] = m[i][j] + v.m[i][j];
		}
	}
	return Matrix(newmat);
}

const Matrix Matrix::operator-        (const Matrix& v) const
{
	double newmat[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newmat[i][j] = m[i][j] - v.m[i][j];
		}
	}
	return Matrix(newmat);
}

const Matrix lux::operator-        (const Matrix& m)
{
	return m * (-1);;
}

const Matrix Matrix::operator*        (const double v) const
{
	double newmat[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newmat[i][j] = m[i][j] * v;
		}
	}
	return Matrix(newmat);
}

const Matrix lux::operator* (const double w, const Matrix& v)
{
	return v * w;
}

const Matrix Matrix::operator/        (const double v) const
{
	double newmat[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newmat[i][j] = m[i][j] / v;
		}
	}
	return Matrix(newmat);
}

const Matrix Matrix::operator*        (const Matrix& v) const
{
	double newmat[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newmat[i][j] = 0;
			for (int k = 0; k < 3; k++)
			{
				newmat[i][j] += m[i][k] * v.m[k][j];
			}
		}
	}
	return Matrix(newmat);
}

Matrix& Matrix::operator=      (const Matrix& v)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i][j] = v.m[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator+=      (const Matrix& v)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i][j] += v.m[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator-=      (const Matrix& v)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i][j] -= v.m[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator*=      (const double v)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i][j] *= v;
		}
	}
	return *this;
}

Matrix& Matrix::operator/=      (const double v)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i][j] /= v;
		}
	}
	return *this;
}

Matrix& Matrix::operator*=      (const Matrix& v)
{
	double newmat[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newmat[i][j] = 0;
			for (int k = 0; k < 3; k++)
			{
				newmat[i][j] += m[i][k] * v.m[k][j];
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i][j] = newmat[i][j];
		}
	}
	return *this;
}

const Matrix Matrix::operator&&      (const Matrix& m) const
{
	return ((*this) * m + m * (*this));
}

const Matrix Matrix::operator||      (const Matrix& m) const
{
	return ((*this) * m - m * (*this));
}

const Matrix Matrix::transpose() const
{
	double newmat[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newmat[i][j] = m[j][i];
		}
	}
	return Matrix(newmat);
}

const double Matrix::det() const
{
	return (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
		+ m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2])
		+ m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

const double Matrix::trace() const
{
	return (m[0][0] + m[1][1] + m[2][2]);
}

const Matrix Matrix::exp() const
{
	Matrix expm;
	expm.Set(0, 0, 1);
	expm.Set(1, 1, 1);
	expm.Set(2, 2, 1);
	Matrix mc(*this);

	double factorial = 1;

	const int terms = 25;
	for (int t = 1; t <= terms; t++)
	{
		factorial *= t;
		expm += mc / factorial;
		mc *= (*this);
	}
	return expm;
}


const bool Matrix::operator==  (const Matrix& v) const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m[j][i] != v.m[i][j]) { return false; }
		}
	}
	return true;
}

const bool Matrix::operator!=  (const Matrix& v) const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m[j][i] != v.m[i][j]) { return true; }
		}
	}
	return false;
}

// These tests are comparisons of the determinant
const bool Matrix::operator>          (const Matrix& v) const
{
	return (det() > v.det());
}

const bool Matrix::operator>=         (const Matrix& v) const
{
	return (det() >= v.det());
}

const bool Matrix::operator<          (const Matrix& v) const
{
	return (det() < v.det());
}

const bool Matrix::operator<=         (const Matrix& v) const
{
	return (det() <= v.det());
}

const Matrix Matrix::inverse() const
{
	double determinant = det();
	if (determinant > 0)
	{
		double newmat[3][3];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				newmat[i][j] = cofactor(i, j) / determinant;
			}
		}
		return Matrix(newmat);
	}
	else
	{
		return Matrix();
	}
}

const double Matrix::cofactor(int i, int j) const
{
	double small[2][2];
	int ip = 0, jp = 0;
	for (int ir = 0; ir < 3; ir++)
	{
		if (ir != i)
		{
			jp = 0;
			for (int jr = 0; jr < 3; jr++)
			{
				if (jr != j)
				{
					small[ip][jp] = m[ir][jr];
					jp++;
				}
			}
			ip++;
		}
	}
	return pow(-1.0, (i + j)) * (small[0][0] * small[1][1] - small[0][1] * small[1][0]);
}

const Matrix Matrix::OuterProduct(const Vector& v1, const Vector& v2)
{
	Matrix mat = Matrix();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat.Set(i, j, v1[i] * v2[j]);
		}
	}
	return mat;
}

const int Matrix::LeviCivita(int i, int j, int k)
{
	if (i == 0 && j == 1 && k == 2) return 1;
	if (i == 1 && j == 2 && k == 0) return 1;
	if (i == 2 && j == 0 && k == 1) return 1;

	if (i == 2 && j == 1 && k == 0) return -1;
	if (i == 0 && j == 2 && k == 1) return -1;
	if (i == 1 && j == 0 && k == 2) return -1;

	return 0;
}

const Matrix Matrix::PauliMatrix(int k)
{
	Matrix mat = Matrix();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat.Set(i, j, Matrix::LeviCivita(i, j, k));
		}
	}

	return mat;
}

const Matrix Matrix::Identity()
{
	Matrix mat = Matrix();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == j) mat.Set(i, j, 1);
			else mat.Set(i, j, 0);
		}
	}

	return mat;
}

const Vector Matrix::operator*(const Vector& vec) const
{
	lux::Vector result = lux::Vector();
	double val = 0.0;

	for (int i = 0; i < 3; i++)
	{
		val = 0.0;
		for (int j = 0; j < 3; j++)
		{
			val += m[i][j] * vec[j];
		}

		result[i] = val;
	}

	return result;
}