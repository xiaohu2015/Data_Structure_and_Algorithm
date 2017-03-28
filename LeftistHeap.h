/*
LestistHeap.h
左倾堆
2016/10/17
*/

#ifndef LEFTIST_H_
#define LEFTIST_H_

#include <algorithm>
#include <ostream>
#include "dsexception.h"

template<typename Comparable>
class LeftistHeap
{
public:
	LeftistHeap() :root{ nullptr } {}

	LeftistHeap(const LeftistHeap & rhs)   // copy construct function
		:root{nullptr}
	{
		root = clone(rhs.root);
	}

	LeftistHeap(LeftistHeap && rhs)		  // move constrcut function
		:root{rhs.root}
	{
		rhs.root = nullptr;
	}

	LeftistHeap & operator=(const LeftistHeap & rhs)
	{
		LeftistHeap copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	LeftistHeap& operator=(LeftistHeap && rhs)
	{
		std::swap(root, rhs.root);
		return *this;
	}

	~LeftistHeap()
	{
		makeEmpty();
	}

	bool isEmpty()	const { return root == nullptr; }

	void makeEmpty()
	{
		reclaimMemory(root);
		root = nullptr;
	}

	void insert(const Comparable & x)
	{
		root = merge(new LeftistNode{ x }, root);
	}

	void insert(Comparable && x)
	{
		root = merge(new LeftistNode{ std::move(x) }, root);
	}

	const Comparable & findMin() const
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		return root->element;
	}

	void deleteMin()
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		LeftistNode *tmp = root;
		root = merge(root->left, root->right);
		delete tmp;
	}

	void deleteMin(Comparable & minItem)
	{
		minItem = findMin();
		deleteMin();
	}

	void merge(LeftistHeap & rhs)   // 合并两个堆
	{
		if (this == &rhs)	// 不能自己与自己合并
		{
			return;
		}
		merge(root, rhs.root);
		rhs.root = nullptr;
	}

private:
	// 节点
	struct LeftistNode
	{
		Comparable element;
		LeftistNode *left;
		LeftistNode *right;
		int  npl;   // null path length

		LeftistNode(const Comparable & x, LeftistNode *l = nullptr, LeftistNode *r = nullptr, int _npl = 0)
			:element{ x }, left{ l }, right{ r }, npl{ _npl }
		{}

		LeftistNode(Comparable && x, LeftistNode *l = nullptr, LeftistNode *r = nullptr, int _npl = 0)
			:element{ std::move(x) }, left{ l }, right{ r }, npl{ _npl }
		{}

	};

	LeftistNode *root;   // 根节点

	void swapChildNode(LeftistNode *p)	  // 交换节点的左右孩子的位置
	{
		LeftistNode *tmp = p->left;
		p->left = p->right;
		p->right = tmp;
	}

	LeftistNode * merge(LeftistNode *h1, LeftistNode *h2 )
	{
		// 首先处理一个堆为空的情况
		if (h1 == nullptr)   
		{
			return h2;
		}
		if (h2 == nullptr)
		{
			return h1;
		}
		// 确保h1的根节点元素较小
		if (h1->element > h2->element)
		{
			return merge(h2, h1);
		}

		if (h1->left == nullptr)	  // 单个节点
		{
			h1->left = h2;   // 将h2设置为h1的左节点即可
		}
		else  // 递归
		{
			h1->right = merge(h1->right, h2);  // 合并h1的右子树与h2,并设置为h1的右子树
			// 此时可能会造成root节点违反左倾堆的性质
			if (h1->left->npl < h1->right->npl)
			{
				swapChildNode(h1);
			}
			// 更新h1的npl
			h1->npl = h1->right->npl + 1;
		}
		return h1;
	}

	// 回收内存
	void reclaimMemory(LeftistNode *p)
	{
		// 使用后序遍历，但是对于不平衡树来说，可能导致栈空间不足
		if (p != nullptr)
		{
			reclaimMemory(p->left);
			reclaimMemory(p->right);
			delete p;
		}
	}

	LeftistNode * clone(LeftistNode *p)	const  // 复制节点
	{
		if (p == nullptr)
		{
			return nullptr;
		}
		return new LeftistNode{ p->element, clone(p->left), clone(p->right), p->npl };
	}
};
#endif