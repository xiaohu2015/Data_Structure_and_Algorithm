/*
Algorithm.h

*/

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <algorithm>

namespace ha
{
	// ������Ԫ�ؽ�����������
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

	// ���ֲ��������е�һ����val���Ԫ������Ӧ�ĵ�����
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

	// ���ַ����������е�һ�����ڵ���valֵ��Ԫ������Ӧ��ָ��
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

	// ��������
	template<typename ForwardIterator, typename T>
	bool binary_search(ForwardIterator first, ForwardIterator last,
		const T& val)
	{
		first = std::lower_bound(first, last, val);
		return	(first != last && *first == val);
	}
}
#endif