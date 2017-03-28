/*
BinarySearchTree.h
二叉搜索树
author: Ye Hu
2016/10/11
*/

#ifndef BINARYSEARCHTREE_H_
#define BINARYSEARCHTREE_H_

#include <algorithm>
#include <iostream>

template<typename Comparable>
class BinarySearchTree
{
public:
	BinarySearchTree() :root{ nullptr } {}
	// copy构造函数
	BinarySearchTree(const BinarySearchTree& rhs) 
		:root{ nullptr } 
	{ 
		root = clone(rhs.root);
	}
	BinarySearchTree& operator=(const BinarySearchTree& rhs)
	{
		BinarySearchTree copy = rhs;
		std::swap(*this, copy);
		return *this;
	}
	// move构造函数
	BinarySearchTree(BinarySearchTree&& rhs)
		:root{ rhs.root }
	{
		rhs.root = nullptr;
	}
	BinarySearchTree& operator=(BinarySearchTree&& rhs)
	{
		std::swap(root, rhs.root);
		return *this;
	}
	~BinarySearchTree()
	{
		makeEmpty();
	}

	// const操作
	const Comparable& findMin()	 const
	{
		return findMin(root)->element;
	}
	const Comparable& findMax()	 const
	{
		return findMax(root)->element;
	}
	bool contains(const Comparable& x)	const { return contains(x, root); }
	bool isEmpty()	const { return root == nullptr; }
	void printTree(std::ostream & out = std::cout) const 
	{
		if (isEmpty())	out << "Empty tree" << endl;
		else printTree(root, out);
	}
	
	// 操作
	void makeEmpty() { makeEmpty(root); }
	void insert(const Comparable& x) { insert(x, root); }
	void insert(Comparable&& x)   { insert(std::move(x), root); }
	void remove(const Comparable & x) {	remove(x, root); }

	


private:
	// 节点定义
	struct BinaryNode
	{
		Comparable element;
		BinaryNode *left;
		BinaryNode *right;
		BinaryNode(const Comparable& theElement, BinaryNode* theLeft, BinaryNode* theRight)
			:element{ theElement }, left{ theLeft }, right{ theRight }
		{}
		BinaryNode( Comparable&& theElement, BinaryNode* theLeft, BinaryNode* theRight)
			:element{ std::move(theElement) }, left{ theLeft }, right{ theRight }
		{}
	};
	// 根节点
	BinaryNode* root;

	// 内部使用
	void insert(const Comparable & x, BinaryNode* & t)
	{
		if (t == nullptr)
		{
			t = new BinaryNode{ x, nullptr, nullptr };
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
			return;    // 已经存在该元素，不作为
		}
	}
	void insert(Comparable && x, BinaryNode* & t)
	{
		if (t == nullptr)
		{
			t = new BinaryNode{ std::move(x), nullptr, nullptr };
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
	}
	void remove(const Comparable& x, BinaryNode* & t)
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
		else  // 找到元素处理
		{
			if (t->left != nullptr && t->right != nullptr)	  // 两个子树都不为空
			{
				t->element = findMin(t->right)->element;
				remove(t->element, t->right);
			}
			else
			{
				BinaryNode* p = t;
				t = t->left == nullptr ? t->right : t->left;
				delete p;
			}
		}
	}
	BinaryNode* findMin(BinaryNode* t) const
	{
		if (t == nullptr)
		{
			return 	nullptr;
		}
		if (t->left == nullptr)
		{
			return t;
		}
		return findMin(t->left);
	}
	BinaryNode* findMax(BinaryNode* t) const
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
	bool contains(const Comparable & x, BinaryNode* t)	const;
	void makeEmpty(BinaryNode* & t)
	{
		// 采用后序遍历删除
		if (t != nullptr)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}
	void printTree(BinaryNode* t, std::ostream & out) const
	{
		if (t != nullptr)
		{
			printTree(t->left, out);
			out << t->element << std::endl;
			printTree(t->right, out);
		}
	}
	BinaryNode* clone(BinaryNode* t) const
	{
		if (t == nullptr)
		{
			return nullptr;
		}
		return new BinaryNode{ t->element, clone(t->left), clone(t->right)};
	}


};

template<typename Comparable>
bool BinarySearchTree<Comparable>::contains(const Comparable & x, BinaryNode* t)  const
{
	//	 递归版本
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
	 
	// 循环版本
	/*while (t != nullptr)
	{
	   if (x < t->element)
	   {
		   t = t->left;
	   }
	   else if (x > t->element)
	   {
		   t = t->right;
	   }
	   else
	   {
		   return true;
	   }
	}
	return false;*/
}


#endif