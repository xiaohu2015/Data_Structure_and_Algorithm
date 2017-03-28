/*
sort.h
排序算法实现
2016/10/18
*/

#ifndef SORT_H_
#define SORT_H_


/*
*常见的排序算法（默认处理的数组都存储在vector中）
*/
#include <vector>
#include <string>
#include <functional>

using std::vector;
using std::string;

/************************插入排序*************************/
// 简单的插入排序算法
template<typename Comparable>
void insertSort(vector<Comparable> & a)
{
	for (int i = 1; i < a.size(); ++i)
   {
		Comparable tmp = std::move(a[i]);
		int j;
		for (j = i; j > 0 && tmp < a[j-1]; --j)
		{
			a[j] = std::move(a[j - 1]);
		}
		a[j] = std::move(tmp);
   }
}

// 插入排序的STL实现 (3个参数）
template<typename Iterator, typename Comparator>
void insertSort(const Iterator & begin, const Iterator & end, Comparator lessThan)
{
	if (begin == end)
	{
		return;
	}
	Iterator itr;
	for (Iterator p = begin + 1; p != end; ++p)
	{
		auto tmp = *p;
		for (itr = p; itr != begin && lessThan(tmp, *(itr-1)); --itr)
		{
			*itr = std::move(*(itr - 1));
		}
		*itr = std::move(tmp);
	}
}

// 两个参数的STL实现
template<typename Iterator>
void insertSort(const Iterator & begin, const Iterator & end)
{
	auto lessThan = std::less<decltype(*begin)>{};
	insertSort(begin, end, lessThan);
}

/*****************************************************************/

/*******************希尔排序(Shell sorting)***********************/
template<typename Comparable>
void shellSort(vector<Comparable> & a)
{
	for (int gap = a.size() / 2; gap > 0; gap /= 2)		// 递减gap
	{
		for (int i = gap; i < a.size(); ++i)
		{
			// 使用插入排序
			int j = i;
			Comparable tmp = std::move(a[i]);
			for (; j >= gap && tmp < a[j - gap]; j -= gap)
			{
				a[j] = std::move(a[j - gap]);
			}
			a[j] = std::move(tmp);
		}
	}
}

/*******************堆排序(HeapSort)*******************************/
// 堆排序
template<typename Comparable>
void heapSort(vector<Comparable> & a)
{
	// 先生成堆
	for (int i = a.size() / 2 - 1; i >= 0; --i)
	{
		percDown(a, i, a.size());
	}
	// 不断将数组的首元素与最后一个元素交换，并减去1
	for (int i = a.size() - 1; i > 0; --i)
	{
		std::swap(a[i], a[0]);
		percDown(a, 0, i);
	}
}
// 计算左节点位置
inline int leftChild(int n)
{
	return 2 * n + 1;
}

// 向下渗入，从而维持堆性质
template<typename Comparable>
void percDown(vector<Comparable> & a, int i, int n)
{
	int child;
	Comparable tmp = std::move(a[i]);
	for (; leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		if (child != n - 1 && a[child+1] > a[child] )
		{
			child++;
		}
		if (a[child] > tmp)
		{
			a[i] = std::move(a[child]);
		}
		else
		{
			break;
		}
	}
	a[i] = std::move(tmp);
}
/*****************************************************************/

/**********************归并排序（MergeSort）***********************/

// 主函数
template<typename Comparable>
void mergeSort(vector<Comparable> & a)
{
	vector <Comparable> tmp(a.size());
	mergeSort(a, tmp, 0, a.size() - 1);
}

// 辅助主函数
template<typename Comparable>
void mergeSort(vector<Comparable> & a, vector<Comparable> & tmp, int left, int right)
{
	if (left < right)
	{
		int center = (left + right) / 2;
		mergeSort(a, tmp, left, center);       // 对左半部分进行递归排序
		mergeSort(a, tmp, center + 1, right);  // 对右半部分进行递归排序
		merge(a, tmp, left, center + 1, right);	  // 归并两个已经排序的子数组
	}
}
// 合并两个已经排序的数组
template<typename Comparable>
void merge(vector<Comparable> & a, vector<Comparable> & tmp, 
	int leftPos, int rightPos, int rightEnd)
{
	int leftEnd = rightPos - 1;	 
	int tmpPos = leftPos;
	int numElements = rightEnd - leftPos + 1;

	// 处理公共部分
	while (leftPos <= leftEnd && rightPos <= rightEnd)
	{
		if (a[leftPos] < a[rightPos])
		{
			tmp[tmpPos++] = std::move(a[leftPos++]);
		}
		else
		{
			tmp[tmpPos++] = std::move(a[rightPos++]);
		}
	}
	// 处理剩余部分
	while (leftPos <= leftEnd)
	{
		tmp[tmpPos++] = std::move(a[leftPos++]);
	}
	while (rightPos <= rightEnd)
	{
		tmp[tmpPos++] = std::move(a[rightPos++]);
	}

	// 将tmp中数据复制回原数组
	for (int i = 0; i < numElements; ++i)
	{
		a[rightEnd] = std::move(tmp[rightEnd]);
		--rightEnd;
	}

}

/*****************************************************************/

/********************快速排序（quickSort)**************************/
// 简单实现，没有优化
template<typename Comparable>
void simpleQuickSort(vector<Comparable> & a)
{
	if (a.size() > 1)
	{
		vector<Comparable> smaller;
		vector<Comparable> same;
		vector<Comparable> larger;
		Comparable pivot = a[a.size() / 2];
		for (auto & item: a)
		{
			if (item == pivot)
			{
				same.push_back(std::move(item));
			}
			else if (item < pivot)
			{
				smaller.push_back(std::move(item));
			}
			else
			{
				larger.push_back(std::move(item));
			}
		}
		// 递归调用
		simpleQuickSort(smaller);
		simpleQuickSort(larger);
		// 复制到原来位置
		std::move(smaller.begin(), smaller.end(), a.begin());
		std::move(same.begin(), same.end(), a.begin() + smaller.size());
		std::move(larger.begin(), larger.end(), a.end() - larger.size());
	}
}

// 优化版本
// 主调用函数
template<typename Comparable>
void quickSort(vector<Comparable> & a)
{
	quickSort(a, 0, a.size() - 1);
}
// 获取中位数
template<typename Comparable>
const Comparable & median3(vector<Comparable> & a, int left, int right)
{
	int center = (left + right) / 2;
	if (a[left] > a[center])
	{
		std::swap(a[left], a[center]);
	}
	if (a[left] > a[right])
	{
		std::swap(a[left], a[right]);
	}
	if (a[center] > a[right])
	{
		std::swap(a[center], a[right]);
	}
	std::swap(a[center], a[right - 1]);    // 将pivot置于right-1位置处 
	return a[right-1];
}

// 共quickSort内部调用的插入排序
template<typename Comparable>
void insertSort(vector<Comparable> & a, int left, int right)
{
	for (int i = left + 1; i <= right; ++i)
	{
		Comparable tmp = std::move(a[i]);
		int j;
		for (j = i; j > left && a[j-1] > tmp; --j)
		{
			a[j] = std::move(a[j - 1]);
		}
		a[j] = std::move(tmp);
	}
}

// 主函数辅助函数
template<typename Comparable>
void quickSort(vector<Comparable> & a, int left, int right)
{
	if (left + 10 <= right)	 // 递归调用快速排序
	{
		auto pivot = median3(a, left, right);
		int i = left;
		int j = right - 1;
		// 开启分区
		for (;;)
		{
			while (a[++i] < pivot)  {}
			while (a[--j] > pivot)	{}
			if (i < j)
			{
				std::swap(a[i], a[j]);
			}
			else
			{
				break;
			}
			
		}
		std::swap(a[i], a[right - 1]);
		// 递归调用
		quickSort(a, left, i - 1);
		quickSort(a, i + 1, right);
	}
	// 调用插入排序
	else
	{
		insertSort(a, left, right);
	}
	
}

/*****************************************************************/

/*************************基数排序*********************************/
// 一种线性时间复杂度的排序方法

// 简单的基数排序，处理固定长度的字符串
void radixSort(vector<string> & arr, int len)
{
	const int BUCKETS = 256;
	vector<vector<string>> buckets(BUCKETS);  // 桶

	for (int i = len - 1; i >= 0; --i)
	{
		for (auto & str: arr)
		{
			buckets[str[i]].push_back(std::move(str));
		}
		int index = 0;
		for (auto & thisBucket: buckets)
		{
			for (auto & str: thisBucket)
			{
				arr[index++] = std::move(str);
			}
			thisBucket.clear();
		}
	}
}

// 基于计数排序的基数排序
void countingRadixSort(vector<string> & arr, int len)
{
	const int BUCKETS = 256;  // 桶数量
	int N = arr.size();      // 字符串数量

	vector<string> buffers(N);
	vector<string> *in = &arr;  // 输入
	vector<string> *out = &buffers;  // 输出

	for (int pos = len - 1; pos >= 0; --pos)
	{
		vector<int> counts(BUCKETS + 1);

		for (int i = 0; i < N; ++i)
		{
			++counts[(*in)[i][pos] + 1];
		}
		// 累加
		for (int i = 1; i <= BUCKETS; ++i)
		{
			counts[i] += counts[i - 1];
		}

		// 存入out
		for (int i = 0; i < N; ++i)
		{
			(*out)[counts[(*in)[i][pos]]++] = std::move((*in)[i]);
		}
		// 交换in与out
		std::swap(in, out);
	}

	// 此时in指向的是buffer而out指向arr
	if (len % 2 == 1)
	{
		for (int i = 0; i < N; ++i)
		{
			(*out)[i] = std::move((*in)[i]);
		}
	}
}

/*****************************************************************/

/*************************查找算法*********************************/

// 二分查找，假设接收的数组已经排序
template<typename Comparable>
void binarySearch(const vector<Comparable> & arr, const Comparable & item, 
	int & position)
{
	int low = 0;
	int high = arr.size();
	position = -1;   // 默认是-1
	int mid = 0;
	while (low < high && position == -1)
	{
		int mid = (low + high) / 2;
		if (arr[mid] == item)
		{
			position = mid;
		}
		else if (arr[mid] < item)
		{
			low = mid + 1;
		}
		else
		{
			high = mid;
		}
	}

	if (position == -1)
	{
		/*high = high == arr.size() ? high - 1 : high;
		position = high >= 0 && arr[high] > item ? high : high + 1;*/
		position = low;
	}
}

// 上界函数；upper_bound
template<typename ForwardIterator, typename T>
ForwardIterator upperBound(ForwardIterator first, ForwardIterator end,
	const T& val)
{
	ForwardIterator it;
	std::iterator_traits<ForwardIterator>::difference_type count, step;
	count = std::distance(first, end);
	while (count > 0)
	{
		it = first;
		step = count / 2;
		std::advance(it, step);
		if (!(val < *it))
		{
			first = ++it;
			count -= step + 1;
		}
		else
		{
			count = step;
		}
	}
	return first;
}
#endif