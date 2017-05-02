/************************************************************************/
/* chapter 5: 二叉树  Ye Hu 2017/03/11                                  */
/************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <utility>

using std::vector;
using std::stack;
using std::pair;

// 树节点
class TreeNode
{
public:
	TreeNode(int value, TreeNode* lp=nullptr, TreeNode* rp=nullptr)	:
		val{ value }, left{ lp }, right{rp}
	{}
	int val;
	TreeNode* left;
	TreeNode* right;
};

// 单链表节点
class ListNode
{
public:
	ListNode(int value, ListNode* np) :
		val{ value }, next{ np }
	{}
	int val;
	ListNode* next;
};

class Solution_6
{
public:
	// 迭代版本：前序遍历
	vector<int> preorderTraversal(TreeNode* root)
	{
		vector<int> result;
		stack<TreeNode*> s;
		TreeNode* p = nullptr;

		if (root != nullptr)
		{
			s.push(root);
		}

		while (!s.empty())
		{
			p = s.top();
			s.pop();
			result.push_back(p->val);
			if (p->right != nullptr)
			{
				s.push(p->right);
			}
			if (p->left != nullptr)
			{
				s.push(p->left);
			}
		}
		return result;
	}

	// 迭代版本：中序遍历
	vector<int> inorderTraversal(TreeNode* root)
	{
		vector<int> result;
		stack<TreeNode*> s;
		TreeNode* p = root;

		while (!s.empty() || p != nullptr)
		{
			if (p != nullptr)
			{
				s.push(p);
				p = p->left;
			}
			else
			{
				p = s.top();
				s.pop();
				result.push_back(p->val);
				p = p->right;
			}
		}
		return result;
	}

	// 迭代版本：后序遍历
	vector<int> postorderTraversal(TreeNode* root)
	{
		vector<int> result;
		stack<TreeNode*> s;
		TreeNode* p = root;
		TreeNode* prev = nullptr;


		do
		{
			while (p != nullptr)
			{
				s.push(p);
				p = p->left;
			}

			prev = nullptr;
			while (!s.empty())
			{
				p = s.top();
				if (p->right == prev)
				{
					result.push_back(p->val);
					s.pop();
					prev = p;
				}
				else
				{
					p = p->right;
					break;
				}
			}
		} while (!s.empty());
		return result;
	}

	// Level级遍历
	vector<vector<int>> levelOrder(TreeNode* root)
	{
		vector<vector<int>> result;
		_traversa(root, 1, result);
		return result;
	}

	void _traversa(TreeNode* root, int level, vector<vector<int>>& result)
	{
		if (root == nullptr)
		{
			return;
		}
		if (level > result.size())
		{
			result.push_back(vector<int>());
		}
		result[level - 1].push_back(root->val);
		_traversa(root->left, level + 1, result);
		_traversa(root->right, level + 1, result);
	}

	// 判断是否是平衡树
	bool isBalancedTree(TreeNode* root)
	{
		return balancedHeight(root) > 0;
	}

	// 如果是平衡树，那么就返回最大树高，否则返回-1
	int balancedHeight(TreeNode* root)
	{
		if (root == nullptr)
		{
			return 0;
		}
		int left = balancedHeight(root->left);
		int right = balancedHeight(root->right);

		if (left < 0 || right < 0 || std::abs(left - right)	> 1)
		{
			return -1;
		}
		return std::max(left, right) + 1;
	}

	// 将二叉树转换为链表结构	 拉平
	void flatten(TreeNode* root)
	{
		if (root == nullptr )
		{
			return;
		}

		flatten(root->right);

		if (root->left != nullptr)
		{
			flatten(root->left);
			TreeNode* p = root->left;
			while (p->right != nullptr)
			{
				p = p->right;
			}
			p->right = root->right;
			root->right = root->left;
		}
		
		root->left = nullptr;

	}

	// 将二叉树转换为链表结构 拉平 ， 迭代版本
	void flattenItr(TreeNode* root)
	{
		stack<TreeNode*> s;
		if (root != nullptr)
		{
			s.push(root);
		}

		TreeNode* p = nullptr;
		while (!s.empty())
		{
			p = s.top();
			s.pop();

			if (p->right != nullptr)
			{
				s.push(p->right);
			}

			if (p->left != nullptr)
			{
				s.push(p->left);
			}
			
			p->left = nullptr;
			if (!s.empty())
			{
				p->right = s.top();
			}
		}
	}

	// 合法的二叉搜索树
	bool isValidBST(TreeNode* root)
	{
		return isValidBST(root, INT_MIN, INT_MAX);
	}
	bool isValidBST(TreeNode* root, int lower, int upper)
	{
		if (root == nullptr)
		{
			return true;
		}
		return root->val > lower && root->val < upper &&
			isValidBST(root->left, lower, root->val) &&
			isValidBST(root->right, root->val, upper);
	}

	// 将排序数组转化为平衡二叉搜索树
	TreeNode* sortedArrayToBST(vector<int>& arr)
	{
		return sortedArrayToBST(std::begin(arr), std::end(arr));
	}
	template<typename RandomAccessIterator>
	TreeNode* sortedArrayToBST(RandomAccessIterator first, RandomAccessIterator last)
	{
		auto length = std::distance(first, last);
		if (length <= 0)
		{
			return nullptr;
		}
		auto mid = std::next(first, length / 2);
		TreeNode* root = new TreeNode{ *mid };
		root->left = sortedArrayToBST(first, mid);
		root->right = sortedArrayToBST(mid + 1, last);
		return root;
	}

	// 将单链表（有序）转化为二叉搜索树
	TreeNode* sortedListToBST(ListNode* head)
	{
		int len = 0;
		ListNode* p = head;
		while (p != nullptr)
		{
			++len;
			p = p->next;
		}
		return sortedListToBST(head, 0, len);
	}

	TreeNode* sortedListToBST(ListNode*& node, int first, int last)
	{
		if (first >= last)
		{
			return nullptr;
		}
		int mid = first + (last - first) / 2;
		// 先处理左子树
		TreeNode* leftTree = sortedListToBST(node, first, mid);
		TreeNode* parent = new TreeNode{ node->val, leftTree, nullptr };
		node = node->next;
		parent->right = sortedListToBST(node, mid + 1, last);
		return parent;
	}

	// 寻找二叉树的最小深度
	int minDepth(TreeNode* root)
	{
		return minDepth(root, false);
	}

	int minDepth(TreeNode* root, bool hasBrother)
	{
		if (root == nullptr)
		{
			return hasBrother ?  INT_MAX : 0;
		}
		return 1 + std::min(minDepth(root->left, root->right != nullptr),
			minDepth(root->right, root->left != nullptr));
	}
	// 迭代版本
	int minDepthItr(TreeNode* root)
	{
		if (root == nullptr)
		{
			return 0;
		}

		int result = INT_MAX;
		stack<pair<TreeNode*, int>> s;

		s.push(pair<TreeNode*, int>{ root, 1 });

		while (!s.empty())
		{
			TreeNode* p = s.top().first;
			int depth = s.top().second;
			s.pop();

			if (p->left == nullptr && p->right == nullptr)
			{
				result = std::min(result, depth);
			}

			if (p->left != nullptr && result > depth)
			{
				s.push(pair<TreeNode*, int>{p->left, depth+1});
			}

			if (p->right != nullptr && result > depth)
			{
				s.push(pair<TreeNode*, int>{p->right, depth+1});
			}
		}
		return result;
	}

	// 最大树深
	int maxDepth(TreeNode* root)
	{
		if (root == nullptr)
		{
			return 0;
		}
		return 1 + std::max(maxDepth(root->left), maxDepth(root->right));
	}

	// 路径和是否存在
	bool hasPathSum(TreeNode* root, int sum)
	{
		if (root == nullptr)
		{
			return false;
		}
		if (root->left == nullptr && root->right == nullptr)
		{
			return (sum - root->val) == 0;
		}
		return (hasPathSum(root->right, sum - root->val) || 
			hasPathSum(root->left, sum - root->val));
	}

	// 路径和是否存在，并返回路径
	vector<vector<int>> pathSum(TreeNode* root, int sum)
	{
		vector<vector<int>> result;
		vector<int> cur;
		pathSum(root, sum, cur, result);
		return result;
	}
	
	void pathSum(TreeNode* root, int gap, vector<int>& cur,
		vector<vector<int>>& result)
	{
		if (root == nullptr)
		{
			return;
		}
		cur.push_back(root->val);

		if (root->left == nullptr && root->right == nullptr)
		{
			if (gap == root->val)
			{
				result.push_back(cur);
			}
		}

		pathSum(root->left, gap - root->val, cur, result);
		pathSum(root->right, gap - root->val, cur, result);

		cur.pop_back(); // 重要
	}

	// 最大路径和：可以从任意节点开始，到任意节点结束
	int maxPathSum(TreeNode* root)
	{
		int max_sum = INT_MIN;
		dfs(root, max_sum);
		return max_sum;
	}
	int dfs(TreeNode* root, int& max_sum)
	{
		if (root == nullptr)
		{
			return 0;
		}
		int l = dfs(root->left, max_sum);
		int r = dfs(root->right, max_sum);

		int sum = root->val;

		if (l > 0)
		{
			sum += l;
		}
		if (r > 0)
		{
			sum += r;
		}
		max_sum = std::max(max_sum, sum);

		return std::max(r, l) > 0 ? std::max(r, l) + root->val : root->val;
	}

};
