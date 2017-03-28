/*
GenBST.h
������
*/

#ifndef GENBST_H_
#define GENBST_H_

#include <queue>
#include <stack>
#include <iostream>

using std::stack;
using std::queue;
using std::cout;

// ʹ��STLʵ�ֵ�stack��
template<typename T>
class Stack : public stack<T>
{
public:
	// ��ջ���һ��Ԫ��
	T pop()
	{
		T temp = top();
		stack<T>::pop();
		return temp;
	}
};

// ʹ��STLʵ�ֵĶ�����
template<typename T>
class Queue : public queue<T>
{
public:
	// �����е�һ��Ԫ��
	T dequeue()
	{
		T temp = front();
		queue<T>::pop();
		return temp;
	}
	// ����
	void enqueue(const T& el)
	{
		push(el);
	}
};

// BST�Ľڵ���
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
	T el;	// Ԫ��ֵ
	BSTNode<T>* left;	  // ��ڵ�
	BSTNode<T>* right;	 // �ҽڵ�
};

// ������
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

	void insert(const T& el);	 // �ǵ�������
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
	void iterativePreorder();  // ����ǰ�����
	void iterativeInorder();   // �����������
	void iterativePostorder();  // �����������
	void MorrisPreorder();
	void MorrisInorder();
	void MorrisPostorder();
	void balance(T*, int, int);

	void breadthFirst();  // ������ȱ���
protected:
	BSTNode<T>* root;  // ���ڵ�
	void clear(BSTNode<T>* p);  // ���������rootΪp)
	void recursiveInsert(BSTNode<T>*& p, const T& el); // �ݹ鷽ʽ����ڵ�
	T* search(BSTNode<T>* p, const T& el) const;  // ����
	T* recursiveSearch(BSTNode<T>* p, const T& el) const; //�ݹ����
	void preorder(BSTNode<T>* p);    // �ݹ�ǰ�����
	void inorder(BSTNode<T>* p);    // �ݹ��������
	void postorder(BSTNode<T>* p);  // �ݹ�������

	virtual void visit(BSTNode<T>* p)  // ��������
	{
		cout << p->el << " ";
	}

};

// ƽ�����ṹ
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

// ɾ�����滻��
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
	else  // �������ӽڵ�
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

// ���ң�ɾ����ͨ���ϲ���
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

// ɾ�����ϲ�
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

// �ǵݹ����
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

// ��ʹ��ջ�ĵ����������
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
				 // ����p->left �� p֮��
				 for (q = p->left, r = q->right, s = r->right;
					 r != p; q = r, r = s, s = s->right)
				 {
					 r->right = q;
				 }
				 // ���
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

// ��ʹ��ջ�ĵ���ǰ�����
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

// ��ʹ��ջ�ĵ����������
template<typename T>
void BST<T>::MorrisInorder()
{
	BSTNode<T>* p = root;
	BSTNode<T>* temp;
	while (p != NULL)
	{
		if (p->left == NULL)  // ������Ϊ�գ�ֱ�ӱ�����ǰ�ڵ㣬��һ��
		{
			visit(p);
			p = p->right;
		}
		else
		{
			tmp = p->left; // ��ڵ�
			// �ҵ���ǰ�ڵ�����Ҳ�ڵ�
			while (temp->right != NULL && temp->right != p)
			{
				temp = temp->right;
			}

			// �ҵ����Ҳ�ڵ�
			if (temp->right == NULL)
			{
				temp->right = p;
				p = p->left;
			}
			else  // ��ԭ���ṹ
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
	// ʹ�ú������
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
	// ʹ���������ͣ�����ֱ���޸�ָ��
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
		// �ҵ��ڵ�
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
	// ���ڵ�->������->������
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
	// ��ڵ�->���ڵ�->�ҽڵ�
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
		// ������->������->���ڵ�
		postorder(p->left);
		postorder(p->right);
		visit(p);
	}
}

template<typename T>
void BST<T>::breadthFirst()
{
	// ��ȱ���
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
		travStack.push(p);  // ��ջroot
		while (!travStack.empty())
		{
			p = travStack.pop();  // ����ջ��Ԫ��
			visit(p);  // ��ǰ�ڵ���Ϊ
			if (p->right != NULL)  // �Ƚ��ҽڵ�ѹ��ջ
			{
				travStack.push(p->right);
			}
			if (p->left != NULL)  // �����ڵ�ѹ��ջ����һ�����ȳ�ջ
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
			if (p->right != NULL)	 // �ҽڵ�������ջ
			{
				travStack.push(p->right);
			}
			travStack.push(p);	// ������ջ
				p = p->left;  // ����������������
		}
		p = travStack.pop();  // ���һ��Ԫ�س�ջ
		// ����һ���ڵ�������������������ջ
		while (!travStack.empty() && p->right == NULL)
		{
			visit(p);
			p = travStack.pop();
		}
		visit(p);
		if (!travStack.empty())
		{
			p = travStack.pop(); // �ýڵ㺬��������
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
	BSTNode<T>* q = root;   // ��¼�ϴα����Ľڵ�
	while (p != NULL)
	{
		for (; p->left != NULL; p = p->left)
		{
			travStack.push(p);    // ����ڵ��𽥼���ջ��ֱ�����һ���������룩
		}

		while (p->right == NULL || p->right = q)
		{
			 // ���ҽڵ�����Ѿ��������ҽڵ�ʱ�ű�����ǰ�ڵ�
			visit(p);
			q = p;
			if (travStack.empty())
			{
				return;
			}
			p = travStack.pop();
		}
		travStack.push(p); // ������������ô��Ҫ�Ƚ��ýڵ���ջ
		p = p->right;	  // Ȼ�����������
	}
}
#endif