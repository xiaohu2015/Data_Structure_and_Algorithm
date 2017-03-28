/*
sort.h
�����㷨ʵ��
2016/10/18
*/

#ifndef SORT_H_
#define SORT_H_


/*
*�����������㷨��Ĭ�ϴ�������鶼�洢��vector�У�
*/
#include <vector>
#include <string>
#include <functional>

using std::vector;
using std::string;

/************************��������*************************/
// �򵥵Ĳ��������㷨
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

// ���������STLʵ�� (3��������
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

// ����������STLʵ��
template<typename Iterator>
void insertSort(const Iterator & begin, const Iterator & end)
{
	auto lessThan = std::less<decltype(*begin)>{};
	insertSort(begin, end, lessThan);
}

/*****************************************************************/

/*******************ϣ������(Shell sorting)***********************/
template<typename Comparable>
void shellSort(vector<Comparable> & a)
{
	for (int gap = a.size() / 2; gap > 0; gap /= 2)		// �ݼ�gap
	{
		for (int i = gap; i < a.size(); ++i)
		{
			// ʹ�ò�������
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

/*******************������(HeapSort)*******************************/
// ������
template<typename Comparable>
void heapSort(vector<Comparable> & a)
{
	// �����ɶ�
	for (int i = a.size() / 2 - 1; i >= 0; --i)
	{
		percDown(a, i, a.size());
	}
	// ���Ͻ��������Ԫ�������һ��Ԫ�ؽ���������ȥ1
	for (int i = a.size() - 1; i > 0; --i)
	{
		std::swap(a[i], a[0]);
		percDown(a, 0, i);
	}
}
// ������ڵ�λ��
inline int leftChild(int n)
{
	return 2 * n + 1;
}

// �������룬�Ӷ�ά�ֶ�����
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

/**********************�鲢����MergeSort��***********************/

// ������
template<typename Comparable>
void mergeSort(vector<Comparable> & a)
{
	vector <Comparable> tmp(a.size());
	mergeSort(a, tmp, 0, a.size() - 1);
}

// ����������
template<typename Comparable>
void mergeSort(vector<Comparable> & a, vector<Comparable> & tmp, int left, int right)
{
	if (left < right)
	{
		int center = (left + right) / 2;
		mergeSort(a, tmp, left, center);       // ����벿�ֽ��еݹ�����
		mergeSort(a, tmp, center + 1, right);  // ���Ұ벿�ֽ��еݹ�����
		merge(a, tmp, left, center + 1, right);	  // �鲢�����Ѿ������������
	}
}
// �ϲ������Ѿ����������
template<typename Comparable>
void merge(vector<Comparable> & a, vector<Comparable> & tmp, 
	int leftPos, int rightPos, int rightEnd)
{
	int leftEnd = rightPos - 1;	 
	int tmpPos = leftPos;
	int numElements = rightEnd - leftPos + 1;

	// ����������
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
	// ����ʣ�ಿ��
	while (leftPos <= leftEnd)
	{
		tmp[tmpPos++] = std::move(a[leftPos++]);
	}
	while (rightPos <= rightEnd)
	{
		tmp[tmpPos++] = std::move(a[rightPos++]);
	}

	// ��tmp�����ݸ��ƻ�ԭ����
	for (int i = 0; i < numElements; ++i)
	{
		a[rightEnd] = std::move(tmp[rightEnd]);
		--rightEnd;
	}

}

/*****************************************************************/

/********************��������quickSort)**************************/
// ��ʵ�֣�û���Ż�
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
		// �ݹ����
		simpleQuickSort(smaller);
		simpleQuickSort(larger);
		// ���Ƶ�ԭ��λ��
		std::move(smaller.begin(), smaller.end(), a.begin());
		std::move(same.begin(), same.end(), a.begin() + smaller.size());
		std::move(larger.begin(), larger.end(), a.end() - larger.size());
	}
}

// �Ż��汾
// �����ú���
template<typename Comparable>
void quickSort(vector<Comparable> & a)
{
	quickSort(a, 0, a.size() - 1);
}
// ��ȡ��λ��
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
	std::swap(a[center], a[right - 1]);    // ��pivot����right-1λ�ô� 
	return a[right-1];
}

// ��quickSort�ڲ����õĲ�������
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

// ��������������
template<typename Comparable>
void quickSort(vector<Comparable> & a, int left, int right)
{
	if (left + 10 <= right)	 // �ݹ���ÿ�������
	{
		auto pivot = median3(a, left, right);
		int i = left;
		int j = right - 1;
		// ��������
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
		// �ݹ����
		quickSort(a, left, i - 1);
		quickSort(a, i + 1, right);
	}
	// ���ò�������
	else
	{
		insertSort(a, left, right);
	}
	
}

/*****************************************************************/

/*************************��������*********************************/
// һ������ʱ�临�Ӷȵ����򷽷�

// �򵥵Ļ������򣬴���̶����ȵ��ַ���
void radixSort(vector<string> & arr, int len)
{
	const int BUCKETS = 256;
	vector<vector<string>> buckets(BUCKETS);  // Ͱ

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

// ���ڼ�������Ļ�������
void countingRadixSort(vector<string> & arr, int len)
{
	const int BUCKETS = 256;  // Ͱ����
	int N = arr.size();      // �ַ�������

	vector<string> buffers(N);
	vector<string> *in = &arr;  // ����
	vector<string> *out = &buffers;  // ���

	for (int pos = len - 1; pos >= 0; --pos)
	{
		vector<int> counts(BUCKETS + 1);

		for (int i = 0; i < N; ++i)
		{
			++counts[(*in)[i][pos] + 1];
		}
		// �ۼ�
		for (int i = 1; i <= BUCKETS; ++i)
		{
			counts[i] += counts[i - 1];
		}

		// ����out
		for (int i = 0; i < N; ++i)
		{
			(*out)[counts[(*in)[i][pos]]++] = std::move((*in)[i]);
		}
		// ����in��out
		std::swap(in, out);
	}

	// ��ʱinָ�����buffer��outָ��arr
	if (len % 2 == 1)
	{
		for (int i = 0; i < N; ++i)
		{
			(*out)[i] = std::move((*in)[i]);
		}
	}
}

/*****************************************************************/

/*************************�����㷨*********************************/

// ���ֲ��ң�������յ������Ѿ�����
template<typename Comparable>
void binarySearch(const vector<Comparable> & arr, const Comparable & item, 
	int & position)
{
	int low = 0;
	int high = arr.size();
	position = -1;   // Ĭ����-1
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

// �Ͻ纯����upper_bound
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