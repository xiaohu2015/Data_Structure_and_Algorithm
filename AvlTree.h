/*
AvlTree.h
AVL�� ƽ����
2016/10/12
*/

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <algorithm>
#include <iostream>

#include "dsexception.h"

template<typename Comparable>
class AvlTree
{
public:
	AvlTree()
		:root{ nullptr }  
	{}

	AvlTree(const AvlTree & rhs)
		:root{ nullptr }
	{
		root = clone(rhs.root);
	}

	AvlTree(AvlTree && rhs)
		:root{ rhs.root }
	{
		rhs.root = nullptr;
	}

	AvlTree & operator=(const AvlTree & rhs)
	{
		AvlTree copy = rhs;
		std::swap(*this, copy);
		return *this;
	}
	AvlTree & operator=(AvlTree && rhs)
	{
		std::swap(root, rhs.root);
		return *this;
	}
	~AvlTree()
	{
		makeEmpty();
	}

	const Comparable & findMin() const
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		return findMin(root)->element;
	}

	const Comparable & findMax() const
	{
		if (isEmpty())
		{
			throw UnderflowException{};
		}
		return findMax(root)->element;
	}

	bool isEmpty()	const
	{
		return root == nullptr;
	}

	void makeEmpty()
	{
		makeEmpty(root);
	}

	bool contains(const Comparable & x)	const
	{
		return contains(x, root);
	}

	void printTree() const
	{
		if (isEmpty())
		{
			std::cout << "Empty Tree." << std::endl;
		}
		printTree(root);
	}

	void insert(const Comparable & x)
	{
		insert(x, root);
	}

	void insert(Comparable && x)
	{
		insert(std::move(x), root);
	}

	void remove(const Comparable & x)
	{
		remove(x, root);
	}

	// ���ú��������������
	int height()  const
	{
		return _height(root);
	}

private:
	struct 	AvlNode	   // �ڵ㶨��
	{
		Comparable element;
		AvlNode *left;
		AvlNode *right;
		int height;		  // ����ڵ�߶ȣ��սڵ�ĸ߶�Ϊ-1
		AvlNode(const Comparable & x, AvlNode *l, AvlNode *r, int h = 0)   // �½ڵ�߶�Ĭ��Ϊ0
			:element{ x }, left{ l }, right{ r }, height{ h }
		{}
		AvlNode( Comparable && x, AvlNode *l, AvlNode *r, int h = 0)
			:element{ std::move(x) }, left{ l }, right{ r }, height{ h }
		{}
	};
	AvlNode *root;	 // ���ڵ�

	int _height(AvlNode * t)	 const
	{
		if (t == nullptr)
		{
			return -1;
		}
		return 1 + max(_height(t->left), _height(t->right));
	}
	/*
	 *insert�������ڵݹ麯����ÿ�εݹ�ʱ����Ҫƽ�⵱ǰ�ڵ�
	 *����ʵ���ϴӱ�����Ľڵ�һֱ�����ڵ�֮�䶼�����ƽ�����
	*/
	void insert(const Comparable & x, AvlNode * & t)
	{
		if (t == nullptr)
		{
			t = new AvlNode{ x, nullptr, nullptr };
		}
		else if (x < t->element)
		{
			insert(x, t->left);
		}
		else if (x > t->element)
		{
			insert(x, t->right);
		}
		else
		{
			return;
		}
		balance(t);	   // ����ƽ�⺯��
	}

	void insert(Comparable && x, AvlNode * & t)
	{
		if (t == nullptr)
		{
			t = new AvlNode{ std::move(x), nullptr, nullptr };
		}
		else if (x < t->element)
		{
			insert(std::move(x), t->left);
		}
		else if (x > t->element)
		{
			insert(std::move(x), t->right);
		}
		else
		{
			return;
		}
		balance(t);
	}
	
	// ɾ������
	void remove(const Comparable & x, AvlNode * & t)
	{
		 if (t == nullptr)
		 {
			 return;
		 }
		 if (x < t->element)
		 {
			 remove(x, t->left);
		 }
		 else if (x > t->element)
		 {
			 remove(x, t->right);
		 }
		 else	// �ҵ�Ԫ��
		 {
			  if (t->left != nullptr && t->right != nullptr)
			  {
				  t->element = findMin(t->right)->element;
				  remove(t->element, t->right);
			  }
			  else
			  {
				  AvlNode *old = t;
				  t = (t->left == nullptr) ? t->right : t->left;
				  delete old;
			  }
		 }
		 balance(t);
	}

	AvlNode * findMin(AvlNode *t)  const
	{
		if (t != nullptr)
		{
			while (t->left != nullptr)
			{
				t = t->left;
			}
		}
		return t;
	}

	AvlNode * findMax(AvlNode *t) const
	{
		if (t != nullptr)
		{
			while (t->right != nullptr)
			{
				t = t->right;
			}
		}
		return t;
	}

	// ��������
	bool contains(const Comparable & x, AvlNode *t)	const
	{
		if (t == nullptr)
		{
			return false;
		}
		else if (x < t->element)
		{
			return contains(x, t->left);
		}
		else if (x > t->element)
		{
			return contains(x, t->right);
		}
		else
		{
			return true;
		}
	}

	// �ÿպ���
	void makeEmpty(AvlNode * & t)
	{
		if (t != nullptr)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}

	void printTree(AvlNode *t) const
	{
		if (t != nullptr)
		{
			printTree(t->left);
			std::cout << t->element << std::endl;
			printTree(t->right);
		}
	}

	// ��¡
	AvlNode * clone(AvlNode *t)	const
	{
		if (t == nullptr)
		{
			return nullptr;
		}
		return new AvlNode{ t->element, clone(t->left), clone(t->right), t->height };
	}


	static const int ALLOWED_IMBALANCE = 1;	 // ��������������ƽ���
	// ����ƽ�����ṹ
	void balance(AvlNode * & t)
	{
	   if (t == nullptr)    // Ҷ�ڵ㲻�ô���
	   {
		   return;
	   }
	   if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)	  // ���������������߳����ƽ���
	   {
		   if (height(t->left->left) >= height(t->left->right))	   // case1:����
		   {
			   rotateWithLeftChild(t);
		   }
		   else		 // case2: ˫��
		   {
			   doubleWithLeftChid(t);
		   }
	   }
	   else	if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
	   {
		   if (height(t->right->right) >= height(t->right->left))
		   {
			   rotateWithRightChild(t);
		   }
		   else
		   {
			   doubleWithRightChild(t);
		   }
	   }
	   t->height = max(height(t->left), height(t->right)) + 1;   // ����t�ĸ߶�
	}

	int height(AvlNode *t) const { return (t == nullptr) ? -1 : t->height; }  // �����߶���-1
	int max(int lhs, int rhs) const { return (lhs > rhs) ? lhs : rhs; }

	// 	����ӵ���������Ľڵ�
	void rotateWithLeftChild(AvlNode * & t)
	{
		AvlNode* l = t->left;   // ��ڵ�
		t->left = l->right;    // ��ڵ����������Ϊ��ǰ�ڵ��������
		l->right = t;		   // ��ǰ�ڵ�Ϊ����ڵ��������
		// ���½ڵ�ĸ߶�
		t->height = max(height(t->left), height(t->right)) + 1;
		l->height = max(height(l->left), height(l->right)) + 1;
		t = l;  // ���û�ı䵱ǰ�ڵ�
	}

	// ����ӵ���������Ľڵ�
	// �������Գ�
	void rotateWithRightChild(AvlNode * & t)
	{
		AvlNode *r = t->right;
		t->right = r->left;
		r->left = t;
		t->height = max(height(t->left), height(t->right)) + 1;
		r->height = max(height(r->left), height(r->right)) + 1;
		t = r;
	}

	// ˫������������������
	void  doubleWithLeftChid(AvlNode * & t)
	{
		rotateWithRightChild(t->left);  // ������ڵ�
		rotateWithLeftChild(t);    // ������ǰ�ڵ�
	}

	// ������������������
	void doubleWithRightChild(AvlNode * & t)
	{
		rotateWithLeftChild(t->right);  // �����ҽڵ�
		rotateWithRightChild(t);      // ������ǰ�ڵ�
	}
};


#endif