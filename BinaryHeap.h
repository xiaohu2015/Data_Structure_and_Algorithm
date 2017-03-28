/*
BinaryHeap.h
�ѵ�ʵ��
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
		if (currentSize == array.size() - 1)  // �ռ䲻��
		{
			array.resize(array.size() * 2);
		}

		int hole = ++currentSize;
		Comparable copy = x;

		array[0] = std::move(copy);
		for (; x < array[hole / 2]; hole /= 2)		 // ����ð
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

	// ɾ����Сֵ
	void deleteMin()
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		array[1] = std::move(array[currentSize--]);
		percolateDown(1);
	}

	// ɾ����Сֵ�����ұ��浽minItem��
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
	int currentSize;    // heap��Ԫ����
	vector<Comparable> array;

	void buildHeap()  // ���ڴ���heap
	{
		for (int i = currentSize / 2; i >= 1; --i)
		{
			percolateDown(i);
		}
	}

	void percolateDown(int hole)  // ��������
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
	int parent(int n) { return n / 2; }	  // ���ڵ�λ��
	int left(int n)	{ return 2 * n; }     // ��ڵ�λ��
	int right(int n) { return 2 * n + 1; }	// �ҽڵ�λ��
	*/
};

#endif