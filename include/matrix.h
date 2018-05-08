#pragma once
#include "headerdef.h"

#define A (*this)

template <class T>
class Matrix
{
public:
	Matrix(std::string, int, int);
	~Matrix();
	inline int width() const;
	inline int height() const;
	inline T &operator() (int, int);
	void LU();
	void CG(T *, T *, double);
	void BiCG(T *, T *, double);
	void Cholesky();
	void solve(T *, T *);
	void solveSPD(T *, T *);
	void print();
private:
	std::string _label;
	int _height, _width;
	T *_matrix;
	int *_P;
};

template <class T>
inline int Matrix<T>::width() const
{
	return _width;
}

template <class T>
inline int Matrix<T>::height() const
{
	return _height;
}

template <class T>
inline T &Matrix<T>::operator()(int i, int j)
{
	return _matrix[i * _width + j];
}

template <class T>
Matrix<T>::Matrix(std::string label, int height, int width) : _height(height), _width(width), _label(label)
{
	_matrix = new T[_height * _width]();
	_P = new int[_width];
	for(auto i = 0; i < _width; ++i) _P[i] = i;
}

template <class T>
Matrix<T>::~Matrix()
{
//	delete [] _P, _matrix;
}

template <class T>
void Matrix<T>::print()
{
	std::cout << std::endl << _label << std::endl;
	for(int i = 1; i < _height; ++i)
	{
		for(int j = 1; j < _width; ++j)
			printf("%.3f\t", A(i,j));
		std::cout << std::endl;
	}

}

template <class T>
void Matrix<T>::LU()
{
	_label = _label + " (LU)";
	int i=0, j=0, k=0, max_pos=1, temp=0;
	T max_val = std::abs(A(1, 1));
	
	for(i = 2; i < _width; ++i)
	{
		if(std::abs(A(i, 1)) > max_val)
		{
			max_val = std::abs(A(i, 1));
			max_pos = i;
		}
	}

	for(k = 1; k < _width; ++k)
	{
		temp = _P[k];
		_P[k] = max_pos;
		_P[max_pos] = temp;

		max_val = 0;
		max_pos = k+1;

		for(i = k + 1; i < _width; ++i)
		{	
			A(_P[i], k)   /= A(_P[k], k);
			A(_P[i], k+1) -= A(_P[i], k) * A(_P[k], k+1);

			if (std::abs(A(_P[i], k+1)) > max_val) {
				max_val = std::abs(A(_P[i], k+1));
				max_pos = i;
			}

			for(j = k + 2; j < _width; ++j)
				A(_P[i], j) -= A(_P[i], k) * A(_P[k], j);
		}
	}
}

template <class T>
void Matrix<T>::solve(T *x, T *b)
{
	unsigned int k, j;
	T *y = new T[_width];
	// Forward Solve
	for(k = 1; k < _width; ++k)
	{  
		y[k] = b[_P[k]];
		for(j = 1; j < k; ++j)
			y[k] -= A(_P[k], j) * y[j];
	}
	// Backward Solve
	for(k = _width - 1; k > 0; --k)
	{
		for(j = k + 1; j < _width; ++j)
			y[k] -= A(_P[k], j) * x[j];
		x[k] = y[k] / A(_P[k], k);
	}
	delete[] y;
}

template <class T>
void Matrix<T>::Cholesky()
{
	_label = _label + " (Cholesky)";
	unsigned int i, j, k;
	T sum;

	for(k = 1; k < _width; ++k)
	{
		sum = 0.0;
		for(j = 1; j < k; ++j)
			sum += A(k, j) * A(k, j);
		sum = A(k, k) - sum;

		A(k, k) = sqrt(sum);
		for(i = k + 1; i < _width; ++i)
		{
			sum = 0.0;
			for(j = 1; j < k; ++j)
				sum += A(i, j) * A(k, j);
			sum = A(k, i) - sum;

			A(i, k) = (sum / A(k, k));
		}
	}
}

template <class T>
void Matrix<T>::solveSPD(T *x, T *b)
{
	unsigned int k, j;
	T *y = new T[_width];
	// Forward Solve
	for(k = 1; k < _width; k++)
	{
		y[k] = b[k];
		for(j = 1; j < k; j++)
			y[k] -= A(k, j) * y[j];
		y[k] /= A(k, k);
	}
	// Backward Solve
	for(k = _width - 1; k > 0; k--)
	{
		for(j = k + 1; j < _width; j++)
			y[k] -= A(j, k) * x[j];
		x[k] = y[k] / A(k, k);
	}
	delete[] y;
}

template <class T>
void Matrix<T>::CG(T *x, T *b, double ITOL)
{
	unsigned int i, j, iter=0;
	double r_norm, b_norm;
	T rho, rho1, beta, alpha;
	T * p = new T[_width];
	T * q = new T[_width];
	T * r = new T[_width];
	T * z = new T[_width];		
	for(i=1;i<_width;++i)
	{
		r[i] = b[i];
		for(j=1;j<_width;++j)
			r[i] -= A(i,j) * x[j];
	}

	r_norm = 0.0;
	for(i = 1; i < _width; ++i) r_norm += r[i] * r[i];
	r_norm = sqrt(r_norm);

	b_norm = 0.0;
	for(i = 1; i < _width; ++i) b_norm += b[i] * b[i];
	b_norm = sqrt(b_norm);
	if(b_norm == 0) b_norm = 1;
	
	while((r_norm/b_norm > ITOL) && (iter < _width))
	{
		iter++;

		for(i = 1; i < _width; ++i) z[i] = (A(i,i) != 0) ? r[i] / A(i,i) : r[i];

		rho = 0.0;
		for(i = 1; i < _width; ++i) rho += r[i] * z[i];

		if(iter == 1)
			for(i = 1; i < _width; ++i) p[i] = z[i];
		else
		{
			beta = rho / rho1;
			for(i = 1; i < _width; ++i) p[i] = z[i] + beta * p[i];
		}

		rho1 = rho;

		for(i = 1; i < _width; ++i)
		{
			q[i] = 0.0;
			for(j = 1; j < _width; ++j)
				q[i] += A(i,j) * p[j];
		}

		alpha = 0.0;
		for(i = 1; i < _width; ++i) alpha += p[i] * q[i];
		if(alpha == 0)
		{
			std::cout << "\033[1;31mERROR: CG Failed\033[0m \n";
			return;
		}
		alpha = rho / alpha;

		for(i = 1; i < _width; ++i) x[i] += alpha * p[i];
		for(i = 1; i < _width; ++i) r[i] -= alpha * q[i];
		r_norm = 0.0;
		for(i = 1; i < _width; ++i) r_norm += r[i] * r[i];
		r_norm = sqrt(r_norm);
	}
	delete[] p, q, r, z;
}


template <class T>
void Matrix<T>::BiCG(T *x, T *b, double ITOL)
{
	unsigned int i, j, iter=0;
	T rho, rho1, beta, alpha, r_norm, b_norm, omega;
	T * p = new T[_width];
	T * q = new T[_width];
	T * r = new T[_width];
	T * z = new T[_width];
	T * p_bar = new T[_width];
	T * q_bar = new T[_width];
	T * r_bar = new T[_width];
	T * z_bar = new T[_width];
	
	for(i = 1; i < _width; ++i)
	{
		r[i] = b[i];
		for(j = 1; j < _width; ++j)
			r[i] -= A(i,j) * x[j];
	}
	
	for(i = 1; i < _width; ++i) r_bar[i] = r[i];
		

	r_norm = 0.0;
	for(i = 1; i < _width; ++i) r_norm += r[i] * r[i];
	r_norm = sqrt(r_norm);

	b_norm = 0.0;
	for(i = 1; i < _width; ++i) b_norm += b[i] * b[i];
	b_norm = sqrt(b_norm);
	if(b_norm == 0) b_norm = 1;

	while((r_norm/b_norm > ITOL) && (iter < _width))
	{
		iter++;

		for(i = 1;i < _width; ++i) z[i] = (A(i,i) != 0) ? (T) r[i] / A(i,i) : r[i];
		for(i = 1;i < _width; ++i) z_bar[i] = (A(i,i) != 0) ? (T) r_bar[i] / A(i,i) : r_bar[i];

		rho = 0.0;
		for(i = 1; i < _width; ++i) rho += r_bar[i] * z[i];
		if(rho == 0)
		{
			std::cout << "\033[1;31mERROR: BiCG Failed\033[0m \n";
			return;
		}
		if(iter == 1)
		{
			for(i = 1;i < _width; ++i) p[i] = z[i];
			for(i = 1;i < _width; ++i) p_bar[i] = z_bar[i];
		}
		else
		{
			beta = rho / rho1;
			for(i = 1; i < _width; ++i) p[i] = z[i] + beta * p[i];
			for(i = 1; i < _width; ++i) p_bar[i] = z_bar[i] + beta * p_bar[i];
		}

		rho1 = rho;

		for(i = 1; i < _width; ++i)
		{
			q[i] = 0.0;
			for(j = 1; j < _width; ++j)
				q[i] += A(i,j) * p[j];
		}

		for(i = 1; i < _width; ++i)
		{
			q_bar[i] = 0.0;
			for(j = 1;j < _width; ++j)
				q_bar[i] += A(j,i) * p_bar[j];
		}

		alpha = 0.0;
		for(i = 1; i < _width; ++i) alpha += p_bar[i] * q[i];
		if(alpha == 0)
		{
			std::cout << "\033[1;31mERROR: BiCG Failed\033[0m \n";
			return;
		}
		alpha = (T) rho / alpha;

		for(i = 1; i < _width; ++i) x[i] += alpha * p[i];
		for(i = 1; i < _width; ++i) r[i] -= alpha * q[i];
		for(i = 1; i < _width; ++i) r_bar[i] -= alpha * q_bar[i];

		r_norm = 0.0;
		for(i = 1; i < _width;++i) r_norm += r[i] * r[i];
		r_norm = sqrt(r_norm);
	}
}