/*
RedBlackTree.h
红黑树实现（没有父亲指针）
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
		/*negInf代表一个不用的值，传给header节点*/
		// 空节点
		nullNode = new RedBlackNode{};
		nullNode->left = nullNode;
		nullNode->right = nullNode;
		// 标兵节点
		header = new RedBlackNode{ negInf };
		header->left = nullNode;
		header->right = nullNode;
	}
	// copy构造函数
	RedBlackTree(const RedBlackTree & rhs)
	{
		nullNode = new RedBlackNode{};
		nullNode->left = nullNode;
		nullNode->right = nullNode;

		header = new RedBlackNode{ rhs.header->element };
		header->left = nullNode;
		header->right = clone(rhs.header->right);
	}
	// move构造函数
	RedBlackTree(RedBlackTree && rhs)
		:nullNode{ rhs.nullNode }, header{rhs.header}
	{
		rhs.nullNode = nullptr;
		rhs.header = nullptr;
	}
	// copy赋值操作符重载
	RedBlackTree & operator=(const RedBlackTree & rhs)
	{
		RedBlackTree copy = rhs;
		std::swap(copy, *this);
		return *this;
	}
	// move赋值操作符重载
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
	// 插入函数
	void insert(const Comparable & x)
	{
		current = parent = grand = great = header;
		nullNode->element = x;   // 保证到达叶子节点

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
			return;    // 已经存在包含相同值的节点
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
	// 颜色枚举类型
	enum COLOR
	{
		RED,
		BLACK
	};

	// 节点
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

	RedBlackNode *header;   // 哨兵节点，不存任何实质元素，右节点指向树的实际根节点
	RedBlackNode *nullNode;  // 空节点，与实例绑定（切记）
	
	// 插入操作的辅助指针（可认为是静态的）
	RedBlackNode *current;
	RedBlackNode *parent;
	RedBlackNode *grand;
	RedBlackNode *great;

	// 递归函数
	// 回收内存
	void reclaimMemory(RedBlackNode *t)
	{
		if (t != t->left)
		{
			reclaimMemory(t->left);
			reclaimMemory(t->right);
			delete t;
		}
	}

	// 中序遍历打印树
	void printTree(RedBlackNode *t)	const
	{
		if (t != t->left)
		{
			printTree(t->left);
			std::cout << t->element << std::endl;
			printTree(t->right);
		}
	}
	RedBlackNode * clone(RedBlackNode *t) const  // 用于复制
	{
		if (t == t->left) // 空节点	不能使用nullNode因为不同实例的nullNode不同
		{
			return nullNode;
		}
		return new RedBlackNode{ t->element, clone(t->left), clone(t->right), t->color };
	}

	// 红黑树主要操作

	/**
	* 供insert调用(当一个节点具有两个红色子节点时）
	* 进行flip和rotate
	*/
	void handleReorient(const Comparable & item)
	{
		// 颜色翻转
		current->color = RED;
		current->left->color = BLACK;
		current->right->color = BLACK;

		// 如果parent是红色，那么要进行rotate
		if (parent->color == RED)
		{
			grand->color = RED;  // 祖父节点涂红
			if ((item < parent->element) != (item < grand->element))  // R-L 或者 L-R
			{
				parent = rotate(item, grand);   // 转化成L-L或者R-R
			}
			current = rotate(item, great);  // 处理L-L或者R-R
			current->color = BLACK;   // 当前颜色涂黑
		}
		// 根节点涂黑色
		header->color = BLACK;

	}

	/**
	* handleReorient调用的旋转函数，包含四种情况
	* item是handleReorient函数中的，用于区分四种情况
	* theParent是要旋转子树根节点的父节点
	* 最后返回旋转子树的根节点
	*/
	RedBlackNode * rotate(const Comparable & item, RedBlackNode *theParent)
	{
		if (item < theParent->element)	 // 子树是左子树
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
		else    // 子树是右子树
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

	// 具有左节点的节点右旋
	void rotateWithLeftChild(RedBlackNode * & t)
	{
		RedBlackNode * lt = t->left;
		t->left = lt->right;
		lt->right = t;
		t = lt;
	}

	// 具有右节点的节点左旋
	void rotateWithRightChild(RedBlackNode * & t)
	{
		RedBlackNode *rt = t->right;
		t->right = rt->left;
		rt->left = t;
		t = rt;
	}

};
#endif