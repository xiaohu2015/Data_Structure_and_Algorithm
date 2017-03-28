/*
GenBST.h
二叉树
*/

#ifndef GENBST_H_
#define GENBST_H_

#include <queue>
#include <stack>
#include <iostream>

using std::stack;
using std::queue;
using std::cout;

// 使用STL实现的stack类
template<typename T>
class Stack : public stack<T>
{
public:
	// 出栈最后一个元素
	T pop()
	{
		T temp = top();
		stack<T>::pop();
		return temp;
	}
};

// 使用STL实现的队列类
template<typename T>
class Queue : public queue<T>
{
public:
	// 出队列第一个元素
	T dequeue()
	{
		T temp = front();
		queue<T>::pop();
		return temp;
	}
	// 入列
	void enqueue(const T& el)
	{
		push(el);
	}
};

// BST的节点类
template<typename T>
class BSTNode
{
public:
	BSTNode()
	{
		left = NULL;
		right = NULL;
	}
	BSTNode(const T& e, BSTNode<T>* l=NULL, BSTNode<T>* r=NULL)
	{
		el = e;
		left = l;
		right = r;
	}
	T el;	// 元素值
	BSTNode<T>* left;	  // 左节点
	BSTNode<T>* right;	 // 右节点
};

// 二叉树
template<typename T>
class BST
{
public:
	BST()
	{
		root = NULL;
	}
	~BST()
	{
		clear();
	}

	void clear()
	{ 
		clear(root);
		root = NULL;
	}

	bool isEmpty()	const
	{
		return root == NULL;
	}

	void preorder()
	{
		preorder(root);
	}

	void inorder()
	{
		inorder(root);
	}

	void postorder()
	{
		postorder(root);
	}

	void insert(const T& el);	 // 非迭代插入
	void recursiveInsert(const T& el)
	{
		recursiveInsert(root, el);
	}

	T* search(const T& el) const
	{
		return search(root, el);
	}

	T* recursiveSearch(const T& el) const
	{
		return recursiveSearch(root, el);
	}

	void deleteByCopying(BSTNode<T>* & p);
	void findAndDeleteByCopying(const T& el);
	void deleteByMerging(BSTNode<T>* & p);
	void findAndDeleteByMerging(const T& el);
	void iterativePreorder();  // 迭代前序遍历
	void iterativeInorder();   // 迭代中序遍历
	void iterativePostorder();  // 迭代后序遍历
	void MorrisPreorder();
	void MorrisInorder();
	void MorrisPostorder();
	void balance(T*, int, int);

	void breadthFirst();  // 宽度优先遍历
protected:
	BSTNode<T>* root;  // 根节点
	void clear(BSTNode<T>* p);  // 清除子树（root为p)
	void recursiveInsert(BSTNode<T>*& p, const T& el); // 递归方式插入节点
	T* search(BSTNode<T>* p, const T& el) const;  // 查找
	T* recursiveSearch(BSTNode<T>* p, const T& el) const; //递归查找
	void preorder(BSTNode<T>* p);    // 递归前序遍历
	void inorder(BSTNode<T>* p);    // 递归中序遍历
	void postorder(BSTNode<T>* p);  // 递归后序遍历

	virtual void visit(BSTNode<T>* p)  // 遍历动作
	{
		cout << p->el << " ";
	}

};

// 平衡树结构
template<typename T>
void BST<T>::balance(T* data, int first, int last)
{
	if (first <= last)
	{
		int middle = (first + last) / 2;
		insert(data[middle]);
		balance(data, first, middle - 1);
		balance(data, middle + 1, last);
	}
}

// 删除（替换）
template<typename T>
void BST<T>::deleteByCopying(BSTNode<T>* & p)
{
	BSTNode<T>* tmp = p;
	BSTNode<T>* prev;
	if (p->right == NULL)
	{
		p = p->left;
	}
	else if (p->left == NULL)
	{
		p = p->right;
	}
	else  // 有两个子节点
	{
		tmp = p->left;
		prev = p;
		while (tmp->right != NULL)
		{
			prev = tmp;
			tmp = tmp->right;
		}
		p->el = tmp->el;
		if (prev == p)
		{
			prev->left = tmp->left;
		}
		else
		{
			prev->right = tmp->left;
		}

	}
	delete tmp;
}

// 查找，删除（通过合并）
template<typename T>
void BST<T>::findAndDeleteByMerging(const T& el)
{
	BSTNode<T>* node = root;
	BSTNode<T>* prev = NULL;
	while (node != NULL)
	{
		if (node->el == el)
		{
			break;
		}
		prev = node;
		if (node->el > el)
		{
			node = node->left;
		}
		else
		{
			node = node->right;
		}
	}
	if (node != NULL && node->el == el)
	{
		if (node == root)
		{
			deleteByMerging(root);
		}
		else if (prev->left == node)
		{
			deleteByMerging(prev->left)
		}
		else
		{
			deleteByMerging(prev->right)
		}
	}
	else if (root != NULL)
	{
		cout << "element " << el << " is not in the tree\n ";
	}
	else
	{
		cout << "the tree is empty.\n";
	}
}

// 删除并合并
template<typename T>
void BST<T>::deleteByMerging(BSTNode<T>* & p)
{
	BSTNode<T>* tmp = p;
	if (p != NULL)
	{
		if (p->right == NULL)
		{
			p = p->left;
		}
		else if (p->left == NULL)
		{
			p = p->right;
		}
		else
		{
			tmp = p->left;
			while (tmp->right != NULL)
			{
				tmp = tmp->right;
			}
			tmp->right = p->right;
			tmp = p;
			p = p->left;
		}
		delete tmp;
	}

}

// 非递归插入
template<typename T>
void BST<T>::insert(const T& el)
{
	BSTNode<T>* p = root;
	BSTNode<T>* prev = NULL;
	while (p != NULL)
	{
		prev = p;
		if (el < p->el)
		{
			p = p->left;
		}
		else  if (el == p->el)
		{
			return;
		}
		else
		{
			p = p->right;
		}
	}

	if (root == NULL)
	{
		root = new BSTNode<T>(el);
	}
	else if (el < prev->el)
	{
		prev->left = new BSTNode<T>(el);
	}
	else
	{
		prev->right = new BSTNode<T>(el);
	}
}

// 不使用栈的迭代后序遍历
template<typename T>
void BST<T>::MorrisPostorder()
{
	BSTNode<T>* p = new BSTNode<T>();
	BSTNode<T>* tmp, *q, *r, *s;
	p->left = root;
	while (p != NULL)
	{
		 if (p->left == NULL)
		 {
			 p = p->right;
		 }
		 else
		 {
			 tmp = p->left;
			 while (tmp->right != NULL && tmp->right != p)
			 {
				 tmp = tmp->right;
			 }
			 if (tmp->right == NULL)
			 {
				 tmp->right = p;
				 p = p->left;
			 }
			 else
			 {
				 // 逆序p->left 到 p之间
				 for (q = p->left, r = q->right, s = r->right;
					 r != p; q = r, r = s, s = s->right)
				 {
					 r->right = q;
				 }
				 // 输出
				 for (s = q->right; q != p->left; q->right = r, r = q, s = s->right)
				 {
					 visit(q);
				 }
				 visit(p->left);
				 tmp->right = NULL;
				 p = p->right;
			 }
		 }
	}
}

// 不使用栈的迭代前序遍历
template<typename T>
void BST<T>::MorrisPreorder()
{
	BSTNode<T>* p = root;
	BSTNode<T>* temp;
	while (p != NULL)
	{
		if (p->left == NULL)
		{
			visit(p);
			p = p->right;
		}
		else
		{
			temp = p->left;
			while (temp->right != NULL && temp->right == p)
			{
				temp = temp->right;
			}
			if (temp->right == NULL)
			{
				visit(p);
				temp->right = p;
				p = p->left;
			}
			else
			{
				temp->right = NULL;
				p = p->right;
			}
		}
	}
}

// 不使用栈的迭代中序遍历
template<typename T>
void BST<T>::MorrisInorder()
{
	BSTNode<T>* p = root;
	BSTNode<T>* temp;
	while (p != NULL)
	{
		if (p->left == NULL)  // 左子树为空，直接遍历当前节点，下一步
		{
			visit(p);
			p = p->right;
		}
		else
		{
			tmp = p->left; // 左节点
			// 找到当前节点的最右侧节点
			while (temp->right != NULL && temp->right != p)
			{
				temp = temp->right;
			}

			// 找到最右侧节点
			if (temp->right == NULL)
			{
				temp->right = p;
				p = p->left;
			}
			else  // 还原树结构
			{
				visit(p);
				temp->right = NULL;
				p = p->right;
			}
		}
	}
}

template<typename T>
void BST<T>::clear(BSTNode<T>* p)
{
	// 使用后序遍历
	if (p != NULL)
	{
		clear(p->left);
		clear(p->right);
		delete p;
	}
}

template<typename T>
void BST<T>::recursiveInsert(BSTNode<T>*& p, const T& el)
{
	// 使用引用类型，可以直接修改指针
	if (p == NULL)
	{
		p = new BSTNode<T>(el, NULL, NULL);
	}
	else if (el < p->el)
	{
		recursiveInsert(p->left, el);
	}
	else
	{
		recursiveInsert(p->right, el);
	}
}

template<typename T>
T* BST<T>::search(BSTNode<T>* p, const T& el)  const
{
	while (p != NULL)
	{
		// 找到节点
		if (el == p->el)
		{
			return &(p->el);
		}
		else if (el < p->el)
		{
			p = p->left;
		}
		else
		{
			p = p->right;
		}
	}
	return NULL;
}

template<typename T>
T* BST<T>::recursiveSearch(BSTNode<T>* p, const T& el) const
{
	if (p == NULL)
	{
		return NULL;
	}
	if (el == p->el)
	{
		return &(p->el);
	}
	else if (el < p->el)
	{
		return recursiveSearch(p->left, el);
	}
	else
	{
		return recursiveSearch(p->right, el);
	}
}

template<typename T>
void BST<T>::preorder(BSTNode<T>* p)
{
	// 根节点->左子树->右子树
	if (p != NULL)
	{
		visit(p);
		preorder(p->left);
		preorder(p->right);
	}
}

template<typename T>
void BST<T>::inorder(BSTNode<T>* p)
{
	// 左节点->根节点->右节点
	if (p != NULL)
	{
		inorder(p->left);
		visit(p);
		inorder(p->right);
	}
}

template<typename T>
void BST<T>::postorder(BSTNode<T>* p)
{
	if (p != NULL)
	{
		// 左子树->右子树->根节点
		postorder(p->left);
		postorder(p->right);
		visit(p);
	}
}

template<typename T>
void BST<T>::breadthFirst()
{
	// 宽度遍历
	Queue<BSTNode<T>*> q;
	BSTNode<T>* p = root;
	if (p != NULL)
	{
		q.enqueue(p);
		while (!q.empty())
		{
			p = q.dequeue();
			visit(p);
			if (p->left != NULL)
			{
				q.enqueue(p->left);
			}
			if (p->right != NULL)
			{
				q.enqueue(p->right);
			}
		}
	}
}

template<typename T>
void BST<T>::iterativePreorder()
{
	Stack<BSTNode<T>*> travStack;
	BSTNode<T>* p = root;
	if (p != NULL)
	{
		travStack.push(p);  // 进栈root
		while (!travStack.empty())
		{
			p = travStack.pop();  // 弹出栈顶元素
			visit(p);  // 当前节点行为
			if (p->right != NULL)  // 先将右节点压入栈
			{
				travStack.push(p->right);
			}
			if (p->left != NULL)  // 最后将左节点压入栈，下一次优先出栈
			{
				travStack.push(p->left);  
			}

		}
	}
}

template<typename T>
void BST<T>::iterativeInorder()
{
	Stack<BSTNode<T>*> travStack;
	BSTNode<T>* p = root;
	while (p != NULL)
	{
		while (p != NULL)
		{
			if (p->right != NULL)	 // 右节点优先入栈
			{
				travStack.push(p->right);
			}
			travStack.push(p);	// 自身入栈
				p = p->left;  // 继续向左子树搜索
		}
		p = travStack.pop();  // 最后一个元素出栈
		// 当上一个节点无右子树可以连续出栈
		while (!travStack.empty() && p->right == NULL)
		{
			visit(p);
			p = travStack.pop();
		}
		visit(p);
		if (!travStack.empty())
		{
			p = travStack.pop(); // 该节点含有右子树
		}
		else
		{
			p = NULL;
		}
	}
}

template<typename T>
void BST<T>::iterativePostorder()
{
	Stack<BSTNode<T>*> travStack;
	BSTNode<T>* p = root;
	BSTNode<T>* q = root;   // 记录上次遍历的节点
	while (p != NULL)
	{
		for (; p->left != NULL; p = p->left)
		{
			travStack.push(p);    // 将左节点逐渐加入栈，直到最后一个（不加入）
		}

		while (p->right == NULL || p->right = q)
		{
			 // 无右节点或者已经遍历了右节点时才遍历当前节点
			visit(p);
			q = p;
			if (travStack.empty())
			{
				return;
			}
			p = travStack.pop();
		}
		travStack.push(p); // 有右子树，那么需要先将该节点入栈
		p = p->right;	  // 然后访问右子树
	}
}
#endif