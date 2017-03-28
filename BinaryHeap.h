/*
BinaryHeap.h
堆的实现
*/

#ifndef BINARYHAEP_H_
#define BINARYHEAP_H_

#include <vector>
#include <algorithm>
#include "dsexception.h"

using std::vector;

template<typename Comparable>
class BinaryHeap
{
public:
	explicit BinaryHeap(int capacity = 100)
		: array(capacity + 1), currentSize{ 0 }
	{}

	explicit BinaryHeap(const vector<Comparable> & items)
		: array(items.size() + 10), currentSize{ items.size() }
	{
		for (int i = 1; i <= currentSize; ++i)
		{
			array[i] = items[i - 1];
		}
		buildHeap();
	}

	bool isEmpty() const { return currentSize == 0; }

	const Comparable & findMin() const
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		return array[1];
	}

	void insert(const Comparable & x)
	{
		if (currentSize == array.size() - 1)  // 空间不足
		{
			array.resize(array.size() * 2);
		}

		int hole = ++currentSize;
		Comparable copy = x;

		array[0] = std::move(copy);
		for (; x < array[hole / 2]; hole /= 2)		 // 向上冒
		{
			array[hole] = std::move(array[hole / 2]);
		}
		array[hole] = std::move(array[0]);

	}

	void insert(Comparable && x)
	{
		if (currentSize == array.size() - 1)
		{
			array.resize(array.size() * 2);
		}

		int hole = ++currentSize;
		array[0] = std::move(x);
		for (; x < array[hole / 2]; hole /= 2)
		{
			array[hole] = std::move(array[hole / 2]);
		}
		array[hole] = std::move(array[0]);
	}

	// 删除最小值
	void deleteMin()
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		array[1] = std::move(array[currentSize--]);
		percolateDown(1);
	}

	// 删除最小值，并且保存到minItem中
	void deleteMin(Comparable & minItem)
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		minItem = std::move(array[1]);
		array[1] = std::move(array[currentSize--]);
		percolateDown(1);
	}

	void makeEmpty() { currentSize = 0; }

private:
	int currentSize;    // heap中元素数
	vector<Comparable> array;

	void buildHeap()  // 用于创建heap
	{
		for (int i = currentSize / 2; i >= 1; --i)
		{
			percolateDown(i);
		}
	}

	void percolateDown(int hole)  // 向下渗入
	{
		Comparable tmp = std::move(array[hole]);
		int child = 0;
		while (hole* 2 <= currentSize)
		{
			child = hole * 2;
			if (child != currentSize && array[child+1] < array[child])
			{
				++child;
			}
			if (array[child] < tmp)
			{
				array[hole] = std::move(array[child]);
				hole = child;
			}
			else
			{
				break;
			}
		}
		array[hole] = std::move(tmp);
	}
	/*
	int parent(int n) { return n / 2; }	  // 父节点位置
	int left(int n)	{ return 2 * n; }     // 左节点位置
	int right(int n) { return 2 * n + 1; }	// 右节点位置
	*/
};

#endif