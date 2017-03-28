/*
AvlTree.h
AVL树 平衡树
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

	// 利用后序遍历计算树高
	int height()  const
	{
		return _height(root);
	}

private:
	struct 	AvlNode	   // 节点定义
	{
		Comparable element;
		AvlNode *left;
		AvlNode *right;
		int height;		  // 保存节点高度，空节点的高度为-1
		AvlNode(const Comparable & x, AvlNode *l, AvlNode *r, int h = 0)   // 新节点高度默认为0
			:element{ x }, left{ l }, right{ r }, height{ h }
		{}
		AvlNode( Comparable && x, AvlNode *l, AvlNode *r, int h = 0)
			:element{ std::move(x) }, left{ l }, right{ r }, height{ h }
		{}
	};
	AvlNode *root;	 // 根节点

	int _height(AvlNode * t)	 const
	{
		if (t == nullptr)
		{
			return -1;
		}
		return 1 + max(_height(t->left), _height(t->right));
	}
	/*
	 *insert函数属于递归函数，每次递归时，都要平衡当前节点
	 *所以实际上从被插入的节点一直到根节点之间都会进行平衡调用
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
		balance(t);	   // 调用平衡函数
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
	
	// 删除函数
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
		 else	// 找到元素
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

	// 包含函数
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

	// 置空函数
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

	// 克隆
	AvlNode * clone(AvlNode *t)	const
	{
		if (t == nullptr)
		{
			return nullptr;
		}
		return new AvlNode{ t->element, clone(t->left), clone(t->right), t->height };
	}


	static const int ALLOWED_IMBALANCE = 1;	 // 所允许的树的最大不平衡度
	// 用于平衡树结构
	void balance(AvlNode * & t)
	{
	   if (t == nullptr)    // 叶节点不用处理
	   {
		   return;
	   }
	   if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)	  // 左子树比右子树高出最大不平衡度
	   {
		   if (height(t->left->left) >= height(t->left->right))	   // case1:右旋
		   {
			   rotateWithLeftChild(t);
		   }
		   else		 // case2: 双旋
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
	   t->height = max(height(t->left), height(t->right)) + 1;   // 更新t的高度
	}

	int height(AvlNode *t) const { return (t == nullptr) ? -1 : t->height; }  // 空树高度是-1
	int max(int lhs, int rhs) const { return (lhs > rhs) ? lhs : rhs; }

	// 	右旋拥有左子树的节点
	void rotateWithLeftChild(AvlNode * & t)
	{
		AvlNode* l = t->left;   // 左节点
		t->left = l->right;    // 左节点的右子树变为当前节点的左子树
		l->right = t;		   // 当前节点为其左节点的右子树
		// 更新节点的高度
		t->height = max(height(t->left), height(t->right)) + 1;
		l->height = max(height(l->left), height(l->right)) + 1;
		t = l;  // 引用会改变当前节点
	}

	// 左旋拥有右子树的节点
	// 与右旋对称
	void rotateWithRightChild(AvlNode * & t)
	{
		AvlNode *r = t->right;
		t->right = r->left;
		r->left = t;
		t->height = max(height(t->left), height(t->right)) + 1;
		r->height = max(height(r->left), height(r->right)) + 1;
		t = r;
	}

	// 双旋：先左旋，后右旋
	void  doubleWithLeftChid(AvlNode * & t)
	{
		rotateWithRightChild(t->left);  // 左旋左节点
		rotateWithLeftChild(t);    // 右旋当前节点
	}

	// 左旋：先右旋后左旋
	void doubleWithRightChild(AvlNode * & t)
	{
		rotateWithLeftChild(t->right);  // 右旋右节点
		rotateWithRightChild(t);      // 左旋当前节点
	}
};


#endif