/*
BinomoalQueue.h
����ʽ����ʵ�����ȼ�����
2016/10/18
*/

#ifndef BINOMIALQUEUE_H_
#define BINOMIALQUEUE_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include "dsexception.h"
using std::vector;

template<typename Comparable>
class BinomialQueue
{
public:
	BinomialQueue()	   // ���еĴ�СΪĬ��ֵ
		:theTrees(DEFAULT_TREES), currentSize{0}
	{
		for (auto & treeRoot: theTrees)	 // �����ڵ�ȫ���ÿ�
		{
			treeRoot = nullptr;
		}
	}

	BinomialQueue(const Comparable & item)   // ���ڴ���ֻ���������ڵ�����Ķ���
		:theTrees(1), currentSize{ 1 }
	{
		theTrees[0] = new BinomialNode{ item, nullptr, nullptr };
	}

	BinomialQueue(const BinomialQueue & rhs)	// ��ֵ���캯��
		:theTrees(rhs.theTrees.size()), currentSize{rhs.currentSize}
	{
		for (int i = 0; i < theTrees.size(); ++i)
		{
			theTrees[i] = clone(rhs.theTrees[i]);
		}
	}

	BinomialQueue(BinomialQueue && rhs)	  // ����vector��move���캯��
		:theTrees{ std::move(rhs.theTrees) }, currentSize{ rhs.currentSize }
	{}

	~BinomialQueue()
	{
		makeEmpty();
	}

	BinomialQueue & operator=(const BinomialQueue & rhs)
	{
		BinomialQueue copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	BinomialQueue & operator=(BinomialQueue && rhs)
	{
		std::swap(rhs.theTrees, theTrees);
		std::swap(rhs.currentSize, currentSize);
		return *this;
	}

	bool isEmpty() const { return currentSize == 0; }

	const Comparable & findMin()  const
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		return theTrees[findMinIndex()]->element;
	}

	// ����merge���в���
	void insert(const Comparable & x)
	{
		BinomialQueue oneItem{ x };
		merge(oneItem);
	}
	void insert(Comparable && x)
	{
		BinomialQueue oneItem{ std::move(x) };
		merge(oneItem);
	}

	void deleteMin()
	{
		Comparable x;
		deleteMin(x);
	}

	void deleteMin(Comparable & minItem)
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}

		int minIndex = findMinIndex();
		minItem = theTrees[minIndex]->element;

		BinomialNode *oldRoot = theTrees[minIndex];
		BinomialNode *leftSubTree = oldRoot->leftChild;
		delete oldRoot;

		// ����H"
		BinomialQueue deletedQueue;
		deletedQueue.theTrees.resize(minIndex);
		deletedQueue.currentSize = (1 << minIndex) - 1;
		for (int i = minIndex - 1; i >= 0; --i)
		{
			deletedQueue.theTrees[i] = leftSubTree;
			leftSubTree = leftSubTree->nextSibling;
			deletedQueue.theTrees[i]->nextSibling = nullptr;
		}

		// ����H'
		theTrees[minIndex] = nullptr;
		currentSize -= deletedQueue.currentSize + 1;

		merge(deletedQueue);

	}

	void makeEmpty()
	{
		currentSize = 0;
		for (auto & treeRoot: theTrees)
		{
			makeEmpty(treeRoot);
		}
	}

	// ��rhs��*this�ϲ�
	void merge(BinomialQueue & rhs)
	{
		if (this == &rhs)  // �޷��Լ����Լ��ϲ�
		{
			return;
		}
		// ���µ�ǰԪ������
		currentSize += rhs.currentSize;
		// �ж��Ƿ񳬳�*this������
		if (currentSize > capacity())
		{
			int oldNumTrees = theTrees.size();
			int newNumTrees = std::max(theTrees.size(), rhs.theTrees.size()) + 1;
			theTrees.resize(newNumTrees);
			for (int i = oldNumTrees; i < newNumTrees; ++i)	 // ��ʼ�������ӵ���
			{
				theTrees[i] = nullptr;
			}
		}

		// �����������*this��rhs�ж�Ӧ��tree
		BinomialNode *carry = nullptr;   // �ϴκϲ��Ľ�����γɵ�������
		// ����ǰ������������Ԫ������ʱֹͣ
		for (int i = 0, j = 1; j <= currentSize; ++i, j *= 2)	 
		{
			BinomialNode *t1 = theTrees[i];
			BinomialNode *t2 = i < rhs.theTrees.size() ? rhs.theTrees[i] : nullptr;

			// ��ǰֹͣ���� (t2����ʣ�࣬��carrayΪ�գ�
			if (i >= rhs.theTrees.size() && carry == nullptr)
			{
				break;
			}

			int whichCase = (t1 == nullptr ? 0 : 1);
			whichCase += (t2 == nullptr ? 0 : 2);
			whichCase += (carry == nullptr ? 0 : 4);

			// ��8���������
			switch (whichCase)
			{
			case 0:	//������Ҫ����
				break;  
			case 1: //ֻ��*this
				break;
			case 2: // ֻ��rhs
				theTrees[i] = t2;
				rhs.theTrees[i] = nullptr;
				break;
			case 4:  // ֻ��carray
				theTrees[i] = carry;
				carry = nullptr;
				break;
			case 3:  // *this��rhs
				carry = combineTrees(t1, t2);
				theTrees[i] = nullptr;
				rhs.theTrees[i] = nullptr;
				break;
			case 5: // *this��carry
				carry = combineTrees(t1, carry);
				theTrees[i] = nullptr;
				break;
			case 6:  // rhs��carry
				carry = combineTrees(t2, carry);
				rhs.theTrees[i] = nullptr;
				break;
			case 7:	 // *this, carry, rhs
				theTrees[i] = carry;
				carry = combineTrees(t1, t2);
				rhs.theTrees[i] = nullptr;
				break;
			default:
				break;
			}
		}

		// ���rhs
		for (auto & treeRoot: rhs.theTrees)
		{
			treeRoot = nullptr;
		}
		rhs.currentSize = 0;
	}


private:
	// ����ڵ�
	struct BinomialNode
	{
		Comparable element;
		BinomialNode *leftChild;
		BinomialNode *nextSibling;

		BinomialNode(const Comparable & x, BinomialNode *l, BinomialNode *ns)
			:element{ x }, leftChild{ l }, nextSibling{ ns }
		{}

		BinomialNode(Comparable && x, BinomialNode *l, BinomialNode *ns)
			:element{ std::move(x) }, leftChild{ l }, nextSibling{ ns }
		{}
	};

	vector<BinomialNode *> theTrees;   // ����ÿ�����ĸ��ڵ�
	int currentSize;    // Ԫ����

	static const int DEFAULT_TREES = 1;  // Ĭ����������
	int findMinIndex() const
	{
		// ǰ��Ԫ������Ϊ0
		int i = 0;
		int minIndex = -1;
		for (; theTrees[i] == nullptr; ++i)
		{}

		for (minIndex = i; i < theTrees.size(); ++i)
		{
			if (theTrees[i] != nullptr &&
				theTrees[i]->element < theTrees[minIndex]->element)
			{
				minIndex = i;
			}
		}
		return minIndex;
	}

	// ����theTrees������
	int capacity() const
	{
		return (1 << theTrees.size()) - 1;
	}

	// �ϲ���������
	BinomialNode * combineTrees(BinomialNode *t1, BinomialNode *t2)	 
	{
		 if (t2->element < t1->element)	   // ȷ��t1���ĸ��ڵ�Ԫ��ֵΪ��С��
		 {
			 return combineTrees(t2, t1);
		 }
		 t2->nextSibling = t1->leftChild;   // t2���ֵܽڵ�Ϊt1����ڵ�
		 t1->leftChild = t2;     // t1����ڵ�Ϊt2
		 return t1;
	}

	// ����ڴ�	���ú������
	void makeEmpty(BinomialNode * & t)
	{
		if (t != nullptr)
		{
			makeEmpty(t->leftChild);
			makeEmpty(t->nextSibling);
			delete t;
			t = nullptr;
		}
	}

	BinomialNode * clone(BinomialNode * t) const
	{
		if (t == nullptr)
		{
			return nullptr;
		}
		return new BinomialNode{ t->element, clone(t->leftChild), clone(t->nextSibling) };
	}
};

#endif