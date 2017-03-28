/*
RedBlackTree.h
�����ʵ�֣�û�и���ָ�룩
2016/10/20
*/
#ifndef REDBLACKTREE_H_
#define REDBLACKTREE_H_

#include <iostream>
#include <algorithm>
#include "dsexception.h"

template<typename Comparable>
class RedBlackTree
{
public:
	explicit RedBlackTree(const Comparable & negInf)
	{
		/*negInf����һ�����õ�ֵ������header�ڵ�*/
		// �սڵ�
		nullNode = new RedBlackNode{};
		nullNode->left = nullNode;
		nullNode->right = nullNode;
		// ����ڵ�
		header = new RedBlackNode{ negInf };
		header->left = nullNode;
		header->right = nullNode;
	}
	// copy���캯��
	RedBlackTree(const RedBlackTree & rhs)
	{
		nullNode = new RedBlackNode{};
		nullNode->left = nullNode;
		nullNode->right = nullNode;

		header = new RedBlackNode{ rhs.header->element };
		header->left = nullNode;
		header->right = clone(rhs.header->right);
	}
	// move���캯��
	RedBlackTree(RedBlackTree && rhs)
		:nullNode{ rhs.nullNode }, header{rhs.header}
	{
		rhs.nullNode = nullptr;
		rhs.header = nullptr;
	}
	// copy��ֵ����������
	RedBlackTree & operator=(const RedBlackTree & rhs)
	{
		RedBlackTree copy = rhs;
		std::swap(copy, *this);
		return *this;
	}
	// move��ֵ����������
	RedBlackTree & operator=(RedBlackTree && rhs)
	{
		std::swap(header, rhs.header);
		std::swap(nullNode, rhs.nullNode);
		return *this;
	}
	~RedBlackTree()
	{
		makeEmpty();
		delete header;
		delete nullNode;
	}

	const Comparable & findMax()  const
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		RedBlackNode *p = header->right;
		while (p->right != nullNode)
		{
			p = p->right;
		}
		return p->element;
	}

	const Comparable & findMin() const
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		RedBlackNode *p = header->right;
		while (p->left != nullNode)
		{
			p = p->left;
		}
		return p->element;
	}

	bool contains(const Comparable & x) const
	{
		RedBlackNode *p = header->right;
		while (p != nullNode)
		{
			if (x < p->element)
			{
				p = p->left;
			}
			else if (x > p->element)
			{
				p = p->right;
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	bool isEmpty()	const { return header->right == nullNode; }

	void printTree() const
	{
		if (isEmpty())
		{
			std::cout << "Empty tree." << std::endl;
		}
		printTree(header->right);
	}
	// ���뺯��
	void insert(const Comparable & x)
	{
		current = parent = grand = great = header;
		nullNode->element = x;   // ��֤����Ҷ�ӽڵ�

		while (current->element != x)
		{
			great = grand;
			grand = parent;
			parent = current;
			current = x < current->element ? current->left : current->right;
			if (current->left->color == RED && current->right->color == RED)
			{
				handleReorient(x);
			}

		}

		if (current != nullNode)
		{
			return;    // �Ѿ����ڰ�����ֵͬ�Ľڵ�
		}

		current = new RedBlackNode{ x, nullNode, nullNode };
		if (x < parent->element)
		{
			parent->left = current;
		}
		else
		{
			parent->right = current;
		}
		handleReorient(x);
	}
	/*void insert(Comparable && x);
	void remove(const Comparable & x);*/
	void makeEmpty()
	{
		if (header == nullptr)
		{
			return;
		}
		reclaimMemory(header->right);
		header->right = nullNode;
	}

	
private:
	// ��ɫö������
	enum COLOR
	{
		RED,
		BLACK
	};

	// �ڵ�
	struct RedBlackNode
	{
		Comparable element;
		RedBlackNode *left;
		RedBlackNode *right;
		int color;
		RedBlackNode(const Comparable & el = Comparable{}, RedBlackNode *lt = nullptr,
			RedBlackNode *rt = nullptr, int theColor = BLACK)
			:element{ el }, left{ lt }, right{ rt }, color{ theColor }
		{}
		RedBlackNode(Comparable && el, RedBlackNode *lt = nullptr, RedBlackNode *rt = nullptr,
			int theColor = BLACK)
			:element{ std::move(el) }, left{ lt }, right{ rt }, color{ theColor }
		{}
	};

	RedBlackNode *header;   // �ڱ��ڵ㣬�����κ�ʵ��Ԫ�أ��ҽڵ�ָ������ʵ�ʸ��ڵ�
	RedBlackNode *nullNode;  // �սڵ㣬��ʵ���󶨣��мǣ�
	
	// ��������ĸ���ָ�루����Ϊ�Ǿ�̬�ģ�
	RedBlackNode *current;
	RedBlackNode *parent;
	RedBlackNode *grand;
	RedBlackNode *great;

	// �ݹ麯��
	// �����ڴ�
	void reclaimMemory(RedBlackNode *t)
	{
		if (t != t->left)
		{
			reclaimMemory(t->left);
			reclaimMemory(t->right);
			delete t;
		}
	}

	// ���������ӡ��
	void printTree(RedBlackNode *t)	const
	{
		if (t != t->left)
		{
			printTree(t->left);
			std::cout << t->element << std::endl;
			printTree(t->right);
		}
	}
	RedBlackNode * clone(RedBlackNode *t) const  // ���ڸ���
	{
		if (t == t->left) // �սڵ�	����ʹ��nullNode��Ϊ��ͬʵ����nullNode��ͬ
		{
			return nullNode;
		}
		return new RedBlackNode{ t->element, clone(t->left), clone(t->right), t->color };
	}

	// �������Ҫ����

	/**
	* ��insert����(��һ���ڵ����������ɫ�ӽڵ�ʱ��
	* ����flip��rotate
	*/
	void handleReorient(const Comparable & item)
	{
		// ��ɫ��ת
		current->color = RED;
		current->left->color = BLACK;
		current->right->color = BLACK;

		// ���parent�Ǻ�ɫ����ôҪ����rotate
		if (parent->color == RED)
		{
			grand->color = RED;  // �游�ڵ�Ϳ��
			if ((item < parent->element) != (item < grand->element))  // R-L ���� L-R
			{
				parent = rotate(item, grand);   // ת����L-L����R-R
			}
			current = rotate(item, great);  // ����L-L����R-R
			current->color = BLACK;   // ��ǰ��ɫͿ��
		}
		// ���ڵ�Ϳ��ɫ
		header->color = BLACK;

	}

	/**
	* handleReorient���õ���ת�����������������
	* item��handleReorient�����еģ����������������
	* theParent��Ҫ��ת�������ڵ�ĸ��ڵ�
	* ��󷵻���ת�����ĸ��ڵ�
	*/
	RedBlackNode * rotate(const Comparable & item, RedBlackNode *theParent)
	{
		if (item < theParent->element)	 // ������������
		{
			if (item < theParent->left->element)  // L-L
			{
				rotateWithLeftChild(theParent->left);
			}
			else   // L-R
			{
				rotateWithRightChild(theParent->left);
			}
			return theParent->left;
		}
		else    // ������������
		{
			if (item < theParent->right->element)  // R-L
			{
				rotateWithLeftChild(theParent->right);
			}
			else  // R-R
			{
				rotateWithRightChild(theParent->right);
			}
			return theParent->right;
		}
	}

	// ������ڵ�Ľڵ�����
	void rotateWithLeftChild(RedBlackNode * & t)
	{
		RedBlackNode * lt = t->left;
		t->left = lt->right;
		lt->right = t;
		t = lt;
	}

	// �����ҽڵ�Ľڵ�����
	void rotateWithRightChild(RedBlackNode * & t)
	{
		RedBlackNode *rt = t->right;
		t->right = rt->left;
		rt->left = t;
		t = rt;
	}

};
#endif