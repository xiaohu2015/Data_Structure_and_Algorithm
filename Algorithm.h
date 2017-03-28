/*
Algorithm.h

*/

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <algorithm>

namespace ha
{
	// 对所有元素进行条件测试
	template<typename InputIterator, typename UnaryPredicate>
	bool all_of(InputIterator first, InputIterator last,
		UnaryPredicate pred)
	{
		while (first != end)
		{
			if (!(pred(*first)))
			{
				return false;
			}
			++first;
		}
		return true;
	}

	// 二分查找序列中第一个比val大的元素所对应的迭代器
	template<typename ForwardIterator, typename T>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
		const T& val)
	{
		ForwardIterator it;
		std::iterator_traits<ForwardIterator>::difference_type count, step;
		count = std::distance(first, last);
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

	// 二分法查找序列中第一个大于等于val值的元素所对应的指针
	template<typename ForwardIterator, typename T>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
		const T& val)
	{
		ForwardIterator it;
		std::iterator_traits<ForwardIterator>::difference_type count, step;
		count = std::distance(first, last);
		while (count > 0)
		{
			it = first;
			step = count / 2;
			std::advance(it, step);
			if (val > *it)
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

	// 二分搜索
	template<typename ForwardIterator, typename T>
	bool binary_search(ForwardIterator first, ForwardIterator last,
		const T& val)
	{
		first = std::lower_bound(first, last, val);
		return	(first != last && *first == val);
	}
}
#endif