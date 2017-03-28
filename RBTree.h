/*
RBTree.h
�����ʵ��
author�� Ye Hu
2016/10/07
*/
#ifndef RBTREE_H_
#define RBTREE_H_

#include <iostream>	

template<typename KEY, typename U>	// ��ֵ��ģ��
class RBTree
{
public:
	RBTree();
	~RBTree(){}

	int size() const { return n; }	// ��С
	bool isEmpty() const { return n == 0; }	// �Ƿ�Ϊ��
	void insert(KEY key, U u);    // ����
	U remove(KEY key);       // ɾ��

	U get(KEY key) const	// ��ȡԪ��
	{
		RBNode* p = find(key);
		if (p == nil)
		{
			return U(NULL);
		}
		return p->data;
	}

	// ����Ԫ��
	void set(KEY key, U data)
	{
		RBNode* p = find(key);
		if (p == nil)
		{
			insert(key, data);
		}
		else
		{
			p->data = data;
		}
	}

	void inOrderTraverse()
	{
		inOrderTraverse(root);
	}
	void preOrderTraverse()	 { preOrderTraverse(root); }
	void  print()  { printHelp(root, 0); }

	enum COLOR {RED, BLACK};  // ö���ࣺ�ڵ����ɫ
	class RBNode	 // ������ڵ���
	{
	public:
		RBNode()
		{
			right = nullptr;
			left = nullptr;
			parent = nullptr;
			color = BLACK;  // Ĭ�Ϻ�ɫ
		}
		COLOR color;  // ��ɫ
		RBNode* right;  // �ҽڵ�ָ��
		RBNode* left;   // ��ڵ�ָ��
		RBNode* parent;  // ���׽ڵ�ָ��
		KEY key;   // ��ֵ
		U data;   // ֵ
	};

private:
	RBNode* nil;   // �սڵ�
	RBNode* root;  // ���ڵ�
	int n;        // Ԫ�ظ���

	// ����
	void rotateLeft(RBNode* node);
	// ����
	void rotateRight(RBNode* node);
	// �������޸�����
	void insertFixUp(RBNode* node);
	// ɾ��֮����޸�����
	void removeFixUp(RBNode* node);

	// �������
	void inOrderTraverse(RBNode* node)
	{
		if (node == nil)
		{
			return;
		}
		inOrderTraverse(node->left);
		std::cout << node->key << ": " << node->data << std::endl;;
		inOrderTraverse(node->right);

	}

	// ǰ�����
	void preOrderTraverse(RBNode* node)
	{
		if (node == nil)
		{
			return;
		}
		std::cout << node->key << ": " << node->data << std::endl;
		preOrderTraverse(node->left);
		preOrderTraverse(node->right);
	}

	// �����ӡ
	void printHelp(RBNode* node, int n)
	{
		if (node == nil)  return;
		printHelp(node->left, n + 1);
		for (int i = 0; i < n; i++)
		{
			std::cout << " ";
		}
		std::cout << node->key <<  std::endl;
		printHelp(node->right, n + 1);
	}

	// ����Key���ڽڵ��ָ��
	RBNode* find(KEY key)  const
	{
		RBNode* p = root;
		while (p != nil)
		{
			if (key < p->key)
			{
				p = p->left;
			}
			else if (key > p->key)
			{
				p = p->right;
			}
			else
			{
				return p;
			}
		}
		return nil;  // δ�ҵ�������nil
	}

	// �ڵ�ĺ����ڵ�
	RBNode* treeSuccessor(RBNode* node)
	{
		
		RBNode* result = node->right;
		// �ҽڵ㲻Ϊ�գ���ôѰ���ҽڵ�������
		while (result->left != nil)
		{
			result = result->left;
		}
		return result;
	}

};

template<typename KEY, typename U>
RBTree<KEY, U>::RBTree()
{
	nil = new RBNode();   // ��ʼ���սڵ�
	root = nil;
	root->right = nil;
	root->left = nil;
	root->parent = nil;
	n = 0;
}

// ���룬�����ֵ�Ѿ����ڣ���������
template<typename KEY, typename U>
void RBTree<KEY, U>::insert(KEY key, U u)
{
	RBNode* p = root;
	RBNode* prev = nil; // ����ǰ�ڵ�
	while (p != nil)
	{
		prev = p;
		if (key < p->key)
		{
			p = p->left;
		}
		else if (key > p->key)
		{
			p = p->right;
		}
		else
		{
			return;   // �Ѿ����ڸü�
		}
	}

	RBNode* newNode = new RBNode();   // �½ڵ�
	newNode->key = key;	 // �����½ڵ������
	newNode->data = u;
	newNode->color = RED;
	newNode->right = nil;
	newNode->left = nil;
	newNode->parent = prev;
	if (prev == nil)   // ˵��ԭ���ǿ���
	{
		root = newNode;
		nil->left = root;
		nil->right = root;
	}
	else if (key < prev->key)
	{
		prev->left = newNode;
	}
	else
	{
		prev->right = newNode;
	}
	insertFixUp(newNode);  // �޸����������
	n++;
}

template<typename KEY, typename U>
void RBTree<KEY, U>::insertFixUp(RBNode* node)
{
	while (node->parent->color == RED)   // ���׽ڵ����ɫ�Ǻ�ɫ
	{
		
	   if (node->parent == node->parent->parent->left)		// ���ڵ����游�ڵ����ڵ�
	   {
		   RBNode* uncle = node->parent->parent->right;  // ��ڵ�
		   if (uncle->color == RED)   // ���1����ڵ�Ϊ��ɫ
		   {
			   node->parent->color = BLACK;
			   uncle->color = BLACK;
			   node->parent->parent->color = RED;
			   node = node->parent->parent;	  // ��Ҫ�������ϼ��
		   }
		   else  
		   {
			   if (node == node->parent->right)	  // ���2�� ��ڵ�Ϊ��ɫ�ҵ�ǰ�ڵ����Һ���
			   {
				   node = node->parent;
				   rotateLeft(node);    // �������׽ڵ������3
			   }
			   // ���3����ڵ�Ϊ��ɫ�ҵ�ǰ�ڵ�������
			   node->parent->color = BLACK;
			   node->parent->parent->color = RED;
			   rotateRight(node->parent->parent);   // ��ʱ�´�ѭ�����Զ�����
			  
			   
		   }
	   }
	   else	 // ���ڵ����游�ڵ���ҽڵ㣨�Գƣ�
	   {
		   RBNode* uncle = node->parent->parent->left;
		   if (uncle->color == RED)
		   {
			   node->parent->color = BLACK;
			   uncle->color = BLACK;
			   node->parent->parent->color = RED;
			   node = node->parent->parent;	  // ��Ҫ�������ϼ��
			  
		   }
		   else
		   {
			   if (node == node->parent->left)	 // ������ǡ���෴
			   {
				   node = node->parent;
				   rotateRight(node);
			   }
			   node->parent->color = BLACK;
			   node->parent->parent->color = RED;
			   rotateLeft(node->parent->parent);   // ��ʱ�´�ѭ�����Զ�����
			 
		   }
	   }
	}
	root->color = BLACK;   //���ֱ���޸ĸ��ڵ�Ϊ��ɫ����ֹ���֣�
}

template<typename KEY, typename U>
void RBTree<KEY, U>::rotateLeft(RBNode* node)
{
	// ���������в�Ϊ�յ����ӽڵ�
	if (node == nil || node->right == nil)
	{
		return;
	}

	RBNode* r = node->right;   // �ҽڵ�
	node->right = r->left;    // node�ҽڵ����ڵ�����Ϊnode���ҽڵ�
	if (r->left != nil)	 r->left->parent = node;  //  ��ΪnilʱҪ����parentָ��
	r->parent = node->parent;    // node��parentΪr��parent
	if (node->parent == nil)     // ��ʱnodeΪroot�ڵ�
	{
		root = r;
		nil->left = root;
		nil->right = root;
	}
	else if (node == node->parent->left)
	{
		node->parent->left = r;
	}
	else
	{
		node->parent->right = r;
	}
	r->left = node;
	node->parent = r;
}

template<typename KEY, typename U>
void RBTree<KEY, U>::rotateRight(RBNode* node)
{
	// ���������в�Ϊ�յ�������
	if (node == nil || node->left == nil)
	{
		return;
	}
	RBNode* l = node->left;   // ��ڵ�
	node->left = l->right;
	if (l->right != nil)
	{
		l->right->parent = node;
	}
	l->parent = node->parent;
	if (node->parent == nil)
	{
		root = l;
		nil->left = root;
		nil->right = root;
	}
	else if (node == node->parent->left)
	{
		 node->parent->left = l;
	}
	else
	{
		node->parent->right = l;
	}
	node->parent = l;
	l->right = node;
}

// ɾ��������������ڣ�����Ҫɾ����ֵ
template<typename KEY, typename U>
U RBTree<KEY, U>::remove(KEY key)
{
	RBNode* removePtr = find(key);
	if (removePtr == nil)
	{
		return U(NULL);   // ���ؿ�ֵ
	}
	U item = removePtr->data;    // Ҫɾ����ֵ
	// Ҫɾ���Ľڵ���ӽڵ����Ϊ��
	if (removePtr->left != nil && removePtr->right != nil)
	{
		RBNode* s = treeSuccessor(removePtr);   // �ҵ������ڵ�
		// �ú����ڵ����Ҫɾ���Ľڵ�
		
		removePtr->data = s->data;
		removePtr->key = s->key;
		// Ȼ��Ҫ����Ľڵ����s
		removePtr = s;
	}
	// ����Ҫ����ɾ���Ľڵ�
	RBNode* child;    // �ҵ�Ҫɾ���Ľڵ��һ������
	if (removePtr->right != nil)   // Ϊ���ӽڵ�
	{
		child = removePtr->right;
	}
	else  // Ϊ���ӽڵ㣨����Ϊ�գ�
	{
		child = removePtr->left;
	}
	child->parent = removePtr->parent;  // ���Ӹ��ڵ����ӽڵ�
	if (removePtr->parent == nil)   // ɾ�����Ǹ��ڵ�
	{
		root = child;
		nil->left = root;
		nil->right = root;
	}
	else if (removePtr == removePtr->parent->right)
	{
		removePtr->parent->right = child;
	}
	else
	{
		removePtr->parent->left = child;
	}
	
	// �޸������  (ɾ���Ľڵ��Ǻ�ɫ������ɾ��֮���ǿ�����
	if (removePtr->color == BLACK && !(child == nil && removePtr->parent == nil))
	{
		removeFixUp(child);
	}
	delete removePtr;
	n--;
	return item;
}

template<typename KEY, typename U>
void RBTree<KEY, U>::removeFixUp(RBNode* node)
{
	// node���Ǹ��ڵ����node�Ǻ�ڽڵ�
	while (node != root && node->color == BLACK)
	{
		if (node == node->parent->left)		 // �ڵ�����ڵ�
		{
			RBNode* brother = node->parent->right; // �ֵܽڵ�
			if (brother->color == RED)	     // ���1�� �ֵܽڵ��Ǻ�ɫ�ڵ�
			{
				brother->color = BLACK;
				node->parent->color = RED;
				rotateLeft(node->parent);
				brother = node->parent->right;   // ת���������
			}
			/* ��ʱ�ֵܽڵ����ɫ�Ǻ�ɫ	*/
			 // ���2���ֵܽڵ�������ӽڵ㶼�Ǻ�ɫ��
			if (brother->left->color == BLACK && brother->right->color == BLACK)
			{
				brother->color = RED;
				node = node->parent;
			}
			else  
			{
				if (brother->right->color == BLACK)		// ���3���ֵܽڵ�����ӽڵ��Ǻ�ɫ�����ӽڵ��Ǻ�ɫ
				{
					brother->left->color = BLACK;
					brother->color = RED;
					rotateRight(brother);
					brother = node->parent->right;   // ת�����4
				}
				// ���4��  �ֵܽڵ�����ӽڵ��Ǻ�ɫ�����ӽڵ��Ǻ�ɫ
				brother->color = node->parent->color;
				node->parent->color = BLACK;
				brother->right->color = BLACK;
				rotateLeft(node->parent);
				node = root;   // ����ֹͣѭ��
			}

		}
		else	// �ڵ����ҽڵ� ��ǰ��Գ�
		{
			RBNode* brother = node->parent->left; // �ֵܽڵ�
			if (brother->color == RED)	     // ���1�� �ֵܽڵ��Ǻ�ɫ�ڵ�
			{
				brother->color = BLACK;
				node->parent->color = RED;
				rotateRight(node->parent);
				brother = node->parent->left;   // ת���������
			}
			/* ��ʱ�ֵܽڵ����ɫ�Ǻ�ɫ	*/
			// ���2���ֵܽڵ�������ӽڵ㶼�Ǻ�ɫ��
			if (brother->left->color == BLACK && brother->right->color == BLACK)
			{
				brother->color = RED;
				node = node->parent;
			}
			else
			{
				if (brother->left->color == BLACK)		// ���3���ֵܽڵ�����ӽڵ��Ǻ�ɫ�����ӽڵ��Ǻ�ɫ
				{
					brother->right->color = BLACK;
					brother->color = RED;
					rotateLeft(brother);
					brother = node->parent->left;   // ת�����4
				}
				// ���4��  �ֵܽڵ�����ӽڵ��Ǻ�ɫ�����ӽڵ��Ǻ�ɫ
				brother->color = node->parent->color;
				node->parent->color = BLACK;
				brother->left->color = BLACK;
				rotateRight(node->parent);
				node = root;   // ����ֹͣѭ��
			}
		}
	}
	node->color = BLACK;   // ����޸�һ��node����ɫ
}
#endif