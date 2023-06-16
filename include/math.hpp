#ifndef MATH_HPP
#define MATH_HPP

#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>


template<class T>
class Matrix
{
public:
	Matrix(size_t n, size_t m) : m_elements(n, std::vector<T>(m)) {}
	Matrix(size_t n, size_t m, const std::vector<T>& elements);
	Matrix(const Matrix<T>& rhs);
	Matrix<T>& operator=(Matrix<T> &rhs);
	~Matrix() = default;

	size_t row_size() const { return m_elements.size(); }
	size_t column_size() const { return m_elements.empty() ? 0 : m_elements.front().size(); }
	bool empty() const { return row_size() == 0 || column_size() == 0; }

	std::vector<T>& operator[](size_t i) { return m_elements[i]; }
	const std::vector<T>& operator[](const size_t i) const { return m_elements[i]; }

	bool operator==(const Matrix<T>& rhs) const;
	Matrix<T> operator+(const Matrix<T>& rhs) const;
	Matrix<T> operator-(const Matrix<T>& rhs) const;
	Matrix<T> operator*(const Matrix<T>& rhs) const;
	Matrix<T> operator/(const T div) const;

private:
	std::vector<std::vector<T>> m_elements;
};

template<class T>
Matrix<T>::Matrix(size_t n, size_t m, const std::vector<T>& elements): m_elements(n, std::vector<T>(m))
{
	if (elements.size() != n * m)
	{
		return;
	}

	for (size_t i = 0; i < elements.size(); ++i)
	{
		size_t row = i / m;
		size_t column = i % m;
		m_elements[row][column] = elements[i];
	}
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& rhs)
{
	m_elements = std::vector<std::vector<T>>();

	if (rhs.empty())
	{
		return;
	}

	for (size_t i = 0; i < rhs.row_size(); ++i)
	{
		m_elements.push_back(std::vector<T>(rhs.column_size()));
		for (size_t j = 0; j < rhs.column_size(); ++j)
		{
			m_elements[i][j] = rhs[i][j];
		}
	}
}

template<class T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> &rhs)
{
	if (*this == rhs)
	{
		return *this;
	}

	m_elements = std::vector<std::vector<T>>();

	if (rhs.empty())
	{
		return *this;
	}

	for (size_t i = 0; i < rhs.row_size(); ++i)
	{
		m_elements.push_back(std::vector<T>(rhs.column_size()));
		for (size_t j = 0; j < rhs.column_size(); ++j)
		{
			m_elements[i][j] = rhs[i][j];
		}
	}

	return *this;
}

template<class T>
bool Matrix<T>::operator==(const Matrix<T>& rhs) const
{
	if (row_size() != rhs.row_size())
	{
		return false;
	}

	if (!empty() && column_size() != rhs.column_size())
	{
		return false;
	}

	for (size_t i = 0; i < row_size(); ++i)
	{
		for (size_t j = 0; j < column_size(); ++j)
		{
			if (m_elements[i][j] != rhs[i][j])
			{
				return false;
			}
		}
	}

	return true;
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs) const
{
	if (empty() || row_size() != rhs.row_size() || column_size() != rhs.column_size())
	{
		throw std::runtime_error("Matrix size error");
	}

	Matrix<T> result(*this);

	for (size_t i = 0; i < result.row_size(); ++i)
	{
		for (size_t j = 0; j < result.column_size(); ++j)
		{
			result[i][j] += rhs[i][j];
		}
	}

	return result;
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs) const
{
	if (empty() || row_size() != rhs.row_size() || column_size() != rhs.column_size())
	{
		throw std::runtime_error("Matrix size error");
	}

	Matrix<T> result(*this);

	for (size_t i = 0; i < result.row_size(); ++i)
	{
		for (size_t j = 0; j < result.column_size(); ++j)
		{
			result[i][j] -= rhs[i][j];
		}
	}

	return result;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs) const
{
	if (empty() || rhs.empty() || column_size() != rhs.row_size())
	{
		throw std::runtime_error("Matrix size error");
	}

	Matrix<T> result(row_size(), rhs.column_size());

	for (size_t i = 0; i < result.row_size(); ++i)
	{
		for (size_t j = 0; j < result.column_size(); ++j)
		{
			result[i][j] = T();

			for (size_t k = 0; k < column_size(); ++k)
			{
				result[i][j] += m_elements[i][k] * rhs[k][j];
			}
		}
	}

	return result;
}

template<class T>
Matrix<T> Matrix<T>::operator/(const T div) const
{
	if (!div)
	{
		throw std::runtime_error("divided by zero");
	}

	Matrix<T> result(*this);

	for (size_t i = 0; i < result.row_size(); ++i)
	{
		for (size_t j = 0; j < result.column_size(); ++j)
		{
			result[i][j] /= div;
		}
	}

	return result;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& rhs)
{
	for (size_t i = 0; i < rhs.row_size(); ++i)
	{
		for (size_t j = 0; j < rhs.column_size(); ++j)
		{
			if (j != 0)
			{
				os << ' ' << rhs[i][j];
			}
			else
			{
				if (i != 0)
				{
					os << '\n';
				}

				os << rhs[i][j];
			}
		}
	}

	return os;
}

template<class T>
Matrix<T> rotateVector2(const Matrix<T>& mat, const Matrix<T>& pivot, float angle)
{
	if (mat.row_size() != 1 || mat.column_size() != 2 || pivot.row_size() != 1 || pivot.column_size() != 2)
	{
		throw std::runtime_error("Matrix size error");
	}

	Matrix<T> result(mat - pivot);
	float temp_cos = std::cos(angle);
	float temp_sin = std::sin(angle);
	Matrix<T> rotation_matrix(2, 2, { temp_cos, -temp_sin, temp_sin, temp_cos });

	result = (result * rotation_matrix) + pivot;

	return result;
}

#endif // !MATH_HPP
