/*
BinomoalQueue.h
二项式队列实现优先级队列
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
	BinomialQueue()	   // 队列的大小为默认值
		:theTrees(DEFAULT_TREES), currentSize{0}
	{
		for (auto & treeRoot: theTrees)	 // 将根节点全部置空
		{
			treeRoot = nullptr;
		}
	}

	BinomialQueue(const Comparable & item)   // 用于创建只包含单个节点的树的队列
		:theTrees(1), currentSize{ 1 }
	{
		theTrees[0] = new BinomialNode{ item, nullptr, nullptr };
	}

	BinomialQueue(const BinomialQueue & rhs)	// 赋值构造函数
		:theTrees(rhs.theTrees.size()), currentSize{rhs.currentSize}
	{
		for (int i = 0; i < theTrees.size(); ++i)
		{
			theTrees[i] = clone(rhs.theTrees[i]);
		}
	}

	BinomialQueue(BinomialQueue && rhs)	  // 利用vector的move构造函数
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

	// 利用merge进行插入
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

		// 构建H"
		BinomialQueue deletedQueue;
		deletedQueue.theTrees.resize(minIndex);
		deletedQueue.currentSize = (1 << minIndex) - 1;
		for (int i = minIndex - 1; i >= 0; --i)
		{
			deletedQueue.theTrees[i] = leftSubTree;
			leftSubTree = leftSubTree->nextSibling;
			deletedQueue.theTrees[i]->nextSibling = nullptr;
		}

		// 构建H'
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

	// 将rhs与*this合并
	void merge(BinomialQueue & rhs)
	{
		if (this == &rhs)  // 无法自己与自己合并
		{
			return;
		}
		// 更新当前元素数量
		currentSize += rhs.currentSize;
		// 判断是否超出*this的容量
		if (currentSize > capacity())
		{
			int oldNumTrees = theTrees.size();
			int newNumTrees = std::max(theTrees.size(), rhs.theTrees.size()) + 1;
			theTrees.resize(newNumTrees);
			for (int i = oldNumTrees; i < newNumTrees; ++i)	 // 初始化新增加的树
			{
				theTrees[i] = nullptr;
			}
		}

		// 下面逐个处理*this与rhs中对应的tree
		BinomialNode *carry = nullptr;   // 上次合并的结果（形成的新树）
		// 当当前树的容量大于元素总数时停止
		for (int i = 0, j = 1; j <= currentSize; ++i, j *= 2)	 
		{
			BinomialNode *t1 = theTrees[i];
			BinomialNode *t2 = i < rhs.theTrees.size() ? rhs.theTrees[i] : nullptr;

			// 提前停止条件 (t2无树剩余，且carray为空）
			if (i >= rhs.theTrees.size() && carry == nullptr)
			{
				break;
			}

			int whichCase = (t1 == nullptr ? 0 : 1);
			whichCase += (t2 == nullptr ? 0 : 2);
			whichCase += (carry == nullptr ? 0 : 4);

			// 共8中情况处理
			switch (whichCase)
			{
			case 0:	//无树需要处理
				break;  
			case 1: //只有*this
				break;
			case 2: // 只有rhs
				theTrees[i] = t2;
				rhs.theTrees[i] = nullptr;
				break;
			case 4:  // 只有carray
				theTrees[i] = carry;
				carry = nullptr;
				break;
			case 3:  // *this与rhs
				carry = combineTrees(t1, t2);
				theTrees[i] = nullptr;
				rhs.theTrees[i] = nullptr;
				break;
			case 5: // *this与carry
				carry = combineTrees(t1, carry);
				theTrees[i] = nullptr;
				break;
			case 6:  // rhs与carry
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

		// 清空rhs
		for (auto & treeRoot: rhs.theTrees)
		{
			treeRoot = nullptr;
		}
		rhs.currentSize = 0;
	}


private:
	// 定义节点
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

	vector<BinomialNode *> theTrees;   // 保存每个树的根节点
	int currentSize;    // 元素数

	static const int DEFAULT_TREES = 1;  // 默认树的数量
	int findMinIndex() const
	{
		// 前提元素数不为0
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

	// 计算theTrees的容量
	int capacity() const
	{
		return (1 << theTrees.size()) - 1;
	}

	// 合并两个子树
	BinomialNode * combineTrees(BinomialNode *t1, BinomialNode *t2)	 
	{
		 if (t2->element < t1->element)	   // 确保t1树的根节点元素值为较小者
		 {
			 return combineTrees(t2, t1);
		 }
		 t2->nextSibling = t1->leftChild;   // t2的兄弟节点为t1的左节点
		 t1->leftChild = t2;     // t1的左节点为t2
		 return t1;
	}

	// 清空内存	采用后序遍历
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