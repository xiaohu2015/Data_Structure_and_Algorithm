/*
RBTree.h
红黑树实现
author： Ye Hu
2016/10/07
*/
#ifndef RBTREE_H_
#define RBTREE_H_

#include <iostream>	

template<typename KEY, typename U>	// 键值对模板
class RBTree
{
public:
	RBTree();
	~RBTree(){}

	int size() const { return n; }	// 大小
	bool isEmpty() const { return n == 0; }	// 是否为空
	void insert(KEY key, U u);    // 插入
	U remove(KEY key);       // 删除

	U get(KEY key) const	// 获取元素
	{
		RBNode* p = find(key);
		if (p == nil)
		{
			return U(NULL);
		}
		return p->data;
	}

	// 设置元素
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

	enum COLOR {RED, BLACK};  // 枚举类：节点的颜色
	class RBNode	 // 红黑树节点类
	{
	public:
		RBNode()
		{
			right = nullptr;
			left = nullptr;
			parent = nullptr;
			color = BLACK;  // 默认黑色
		}
		COLOR color;  // 颜色
		RBNode* right;  // 右节点指针
		RBNode* left;   // 左节点指针
		RBNode* parent;  // 父亲节点指针
		KEY key;   // 键值
		U data;   // 值
	};

private:
	RBNode* nil;   // 空节点
	RBNode* root;  // 根节点
	int n;        // 元素个数

	// 左旋
	void rotateLeft(RBNode* node);
	// 右旋
	void rotateRight(RBNode* node);
	// 插入后的修复函数
	void insertFixUp(RBNode* node);
	// 删除之后的修复函数
	void removeFixUp(RBNode* node);

	// 中序遍历
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

	// 前序遍历
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

	// 中序打印
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

	// 查找Key所在节点的指针
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
		return nil;  // 未找到，返回nil
	}

	// 节点的后驱节点
	RBNode* treeSuccessor(RBNode* node)
	{
		
		RBNode* result = node->right;
		// 右节点不为空，那么寻找右节点的最左端
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
	nil = new RBNode();   // 初始化空节点
	root = nil;
	root->right = nil;
	root->left = nil;
	root->parent = nil;
	n = 0;
}

// 插入，如果键值已经存在，则无作用
template<typename KEY, typename U>
void RBTree<KEY, U>::insert(KEY key, U u)
{
	RBNode* p = root;
	RBNode* prev = nil; // 保存前节点
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
			return;   // 已经存在该键
		}
	}

	RBNode* newNode = new RBNode();   // 新节点
	newNode->key = key;	 // 设置新节点的属性
	newNode->data = u;
	newNode->color = RED;
	newNode->right = nil;
	newNode->left = nil;
	newNode->parent = prev;
	if (prev == nil)   // 说明原来是空树
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
	insertFixUp(newNode);  // 修复红黑树性质
	n++;
}

template<typename KEY, typename U>
void RBTree<KEY, U>::insertFixUp(RBNode* node)
{
	while (node->parent->color == RED)   // 父亲节点的颜色是红色
	{
		
	   if (node->parent == node->parent->parent->left)		// 父节点是祖父节点的左节点
	   {
		   RBNode* uncle = node->parent->parent->right;  // 叔节点
		   if (uncle->color == RED)   // 情况1：叔节点为红色
		   {
			   node->parent->color = BLACK;
			   uncle->color = BLACK;
			   node->parent->parent->color = RED;
			   node = node->parent->parent;	  // 需要继续向上检查
		   }
		   else  
		   {
			   if (node == node->parent->right)	  // 情况2： 叔节点为黑色且当前节点是右孩子
			   {
				   node = node->parent;
				   rotateLeft(node);    // 左旋父亲节点变成情况3
			   }
			   // 情况3：叔节点为黑色且当前节点是左孩子
			   node->parent->color = BLACK;
			   node->parent->parent->color = RED;
			   rotateRight(node->parent->parent);   // 此时下次循环会自动结束
			  
			   
		   }
	   }
	   else	 // 父节点是祖父节点的右节点（对称）
	   {
		   RBNode* uncle = node->parent->parent->left;
		   if (uncle->color == RED)
		   {
			   node->parent->color = BLACK;
			   uncle->color = BLACK;
			   node->parent->parent->color = RED;
			   node = node->parent->parent;	  // 需要继续向上检查
			  
		   }
		   else
		   {
			   if (node == node->parent->left)	 // 与上面恰好相反
			   {
				   node = node->parent;
				   rotateRight(node);
			   }
			   node->parent->color = BLACK;
			   node->parent->parent->color = RED;
			   rotateLeft(node->parent->parent);   // 此时下次循环会自动结束
			 
		   }
	   }
	}
	root->color = BLACK;   //最后直接修改根节点为黑色（防止出现）
}

template<typename KEY, typename U>
void RBTree<KEY, U>::rotateLeft(RBNode* node)
{
	// 左旋必须有不为空的右子节点
	if (node == nil || node->right == nil)
	{
		return;
	}

	RBNode* r = node->right;   // 右节点
	node->right = r->left;    // node右节点的左节点设置为node的右节点
	if (r->left != nil)	 r->left->parent = node;  //  不为nil时要设置parent指针
	r->parent = node->parent;    // node的parent为r的parent
	if (node->parent == nil)     // 此时node为root节点
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
	// 右旋必须有不为空的左子树
	if (node == nil || node->left == nil)
	{
		return;
	}
	RBNode* l = node->left;   // 左节点
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

// 删除函数，如果存在，返回要删除的值
template<typename KEY, typename U>
U RBTree<KEY, U>::remove(KEY key)
{
	RBNode* removePtr = find(key);
	if (removePtr == nil)
	{
		return U(NULL);   // 返回空值
	}
	U item = removePtr->data;    // 要删除的值
	// 要删除的节点的子节点均不为空
	if (removePtr->left != nil && removePtr->right != nil)
	{
		RBNode* s = treeSuccessor(removePtr);   // 找到后驱节点
		// 用后驱节点填充要删除的节点
		
		removePtr->data = s->data;
		removePtr->key = s->key;
		// 然后要处理的节点就是s
		removePtr = s;
	}
	// 下面要处理被删除的节点
	RBNode* child;    // 找到要删除的节点的一个孩子
	if (removePtr->right != nil)   // 为右子节点
	{
		child = removePtr->right;
	}
	else  // 为左子节点（可能为空）
	{
		child = removePtr->left;
	}
	child->parent = removePtr->parent;  // 连接父节点与子节点
	if (removePtr->parent == nil)   // 删除的是根节点
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
	
	// 修复红黑树  (删除的节点是黑色，并且删除之后不是空数）
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
	// node不是根节点或者node是红黑节点
	while (node != root && node->color == BLACK)
	{
		if (node == node->parent->left)		 // 节点是左节点
		{
			RBNode* brother = node->parent->right; // 兄弟节点
			if (brother->color == RED)	     // 情况1： 兄弟节点是红色节点
			{
				brother->color = BLACK;
				node->parent->color = RED;
				rotateLeft(node->parent);
				brother = node->parent->right;   // 转入其他情况
			}
			/* 此时兄弟节点的颜色是黑色	*/
			 // 情况2：兄弟节点的两个子节点都是黑色的
			if (brother->left->color == BLACK && brother->right->color == BLACK)
			{
				brother->color = RED;
				node = node->parent;
			}
			else  
			{
				if (brother->right->color == BLACK)		// 情况3：兄弟节点的左子节点是红色，右子节点是黑色
				{
					brother->left->color = BLACK;
					brother->color = RED;
					rotateRight(brother);
					brother = node->parent->right;   // 转入情况4
				}
				// 情况4：  兄弟节点的左子节点是红色，右子节点是红色
				brother->color = node->parent->color;
				node->parent->color = BLACK;
				brother->right->color = BLACK;
				rotateLeft(node->parent);
				node = root;   // 主动停止循环
			}

		}
		else	// 节点是右节点 与前面对称
		{
			RBNode* brother = node->parent->left; // 兄弟节点
			if (brother->color == RED)	     // 情况1： 兄弟节点是红色节点
			{
				brother->color = BLACK;
				node->parent->color = RED;
				rotateRight(node->parent);
				brother = node->parent->left;   // 转入其他情况
			}
			/* 此时兄弟节点的颜色是黑色	*/
			// 情况2：兄弟节点的两个子节点都是黑色的
			if (brother->left->color == BLACK && brother->right->color == BLACK)
			{
				brother->color = RED;
				node = node->parent;
			}
			else
			{
				if (brother->left->color == BLACK)		// 情况3：兄弟节点的左子节点是红色，右子节点是黑色
				{
					brother->right->color = BLACK;
					brother->color = RED;
					rotateLeft(brother);
					brother = node->parent->left;   // 转入情况4
				}
				// 情况4：  兄弟节点的左子节点是红色，右子节点是红色
				brother->color = node->parent->color;
				node->parent->color = BLACK;
				brother->left->color = BLACK;
				rotateRight(node->parent);
				node = root;   // 主动停止循环
			}
		}
	}
	node->color = BLACK;   // 最后修改一下node的颜色
}
#endif