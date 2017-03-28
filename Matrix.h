/*
Matrix.h
使用vector实现矩阵
2016/10/12
*/
#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>
#include <algorithm>
using std::vector;
template<typename Object>
class matrix
{
	
public:
	matrix(int rows, int cols) :
		array{rows}
	{
		for (auto & thisRow: array)
		{
			thisRow.resize(cols);
		}
	}

	matrix(const vector<vector<Object>> & v)
		:array{v}
	{}

	matrix(vector<vector<Object>> && v)
		:array{std::swap(v)}
	{}

	vector<Object>& operator[](int row)
	{
		return array[row];
	}

	const vector<Object>& operator[](int row)  const
	{
		return array[row];
	}

	int rows()	const
	{
		return array.size();
	}
	int cols() const
	{
		return rows() == 0 ? 0 : array[0].size();
	}
private:
	vector<vector<Object>> array;
};
#endif