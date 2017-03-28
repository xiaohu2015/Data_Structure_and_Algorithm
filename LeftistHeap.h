/*
LestistHeap.h
�����
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

	void merge(LeftistHeap & rhs)   // �ϲ�������
	{
		if (this == &rhs)	// �����Լ����Լ��ϲ�
		{
			return;
		}
		merge(root, rhs.root);
		rhs.root = nullptr;
	}

private:
	// �ڵ�
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

	LeftistNode *root;   // ���ڵ�

	void swapChildNode(LeftistNode *p)	  // �����ڵ�����Һ��ӵ�λ��
	{
		LeftistNode *tmp = p->left;
		p->left = p->right;
		p->right = tmp;
	}

	LeftistNode * merge(LeftistNode *h1, LeftistNode *h2 )
	{
		// ���ȴ���һ����Ϊ�յ����
		if (h1 == nullptr)   
		{
			return h2;
		}
		if (h2 == nullptr)
		{
			return h1;
		}
		// ȷ��h1�ĸ��ڵ�Ԫ�ؽ�С
		if (h1->element > h2->element)
		{
			return merge(h2, h1);
		}

		if (h1->left == nullptr)	  // �����ڵ�
		{
			h1->left = h2;   // ��h2����Ϊh1����ڵ㼴��
		}
		else  // �ݹ�
		{
			h1->right = merge(h1->right, h2);  // �ϲ�h1����������h2,������Ϊh1��������
			// ��ʱ���ܻ����root�ڵ�Υ������ѵ�����
			if (h1->left->npl < h1->right->npl)
			{
				swapChildNode(h1);
			}
			// ����h1��npl
			h1->npl = h1->right->npl + 1;
		}
		return h1;
	}

	// �����ڴ�
	void reclaimMemory(LeftistNode *p)
	{
		// ʹ�ú�����������Ƕ��ڲ�ƽ������˵�����ܵ���ջ�ռ䲻��
		if (p != nullptr)
		{
			reclaimMemory(p->left);
			reclaimMemory(p->right);
			delete p;
		}
	}

	LeftistNode * clone(LeftistNode *p)	const  // ���ƽڵ�
	{
		if (p == nullptr)
		{
			return nullptr;
		}
		return new LeftistNode{ p->element, clone(p->left), clone(p->right), p->npl };
	}
};
#endif