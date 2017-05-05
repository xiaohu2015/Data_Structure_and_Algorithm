#pragma once

// 分离链接法 Separate Chaining

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <algorithm>

using std::vector;
using std::list;
using std::hash;
using std::string;
using std::cout;

int nextPrime(int value)
{
	int currentValue = value + 1;
	while (true)
	{
		bool flag = true;
		for (int i = 2; i * i <= currentValue; ++i)
		{
			if (currentValue % i == 0)
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			break;
		}
		++currentValue;
	}
	return currentValue;
}

template<typename HashedObj>
class HashTable
{
public:
	explicit HashTable(int size = 101):
		theLists(size, list<HashedObj>())
	{}

	bool contains(const HashedObj& x) const
	{
		auto& lt = theLists[myHash(x)];
		return std::find(lt.begin(), lt.end(), x) != lt.end();
	}

	void makeEmpty()
	{
		for (list<HashedObj>& lt : theLists)
		{
			lt.clear();
		}
	}

	bool insert(const HashedObj& x)
	{
		list<HashedObj>& lt = theLists[myHash(x)];
		auto itr = std::find(lt.begin(), lt.end(), x);

		if (itr != lt.end())  return false;   // 已经存在

		lt.push_back(x);

		// 重新hash
		if (++currentSize > theLists.size())
		{
			rehash();
		}

		return true;
	}

	bool insert(HashedObj&& x)
	{
		list<HashedObj>& lt = theLists[myHash(x)];
		auto itr = std::find(lt.begin(), lt.end(), x);

		if (itr != lt.end())  return false;   // 已经存在

		lt.push_back(std::move(x));

		// 重新hash
		if (++currentSize > theLists.size())
		{
			rehash();
		}

		return true;
	}

	bool remove(const HashedObj& x)
	{
		list<HashedObj>& lt = theLists[myHash(x)];
		auto itr = std::find(lt.begin(), lt.end(), x);

		if (itr == lt.end())  return false;  // 不存在

		lt.erase(itr);
		--currentSize;
		return true;
	}

private:
	vector<list<HashedObj>> theLists;
	int currentSize;

	void rehash() 
	{
		vector<list<HashedObj>> oldLists = theLists;
		theLists.resize(nextPrime(2 * theLists.size()));
		for (auto& lt : theLists)
		{
			lt.clear();
		}

		currentSize = 0;
		for (auto& lt : oldLists)
		{
			for (auto& x : lt)
			{
				insert(std::move(x));
			}
		}

	};

	size_t myHash(const HashedObj& x) const
	{
		static hash<HashedObj> hf;
		return hf(x) % theLists.size();
	}
};

template <>
class hash<string>
{
public:
	size_t operator()(const string& key)
	{
		size_t hashVal = 0;
		for (char c : key)
		{
			hashVal = 37 * hashVal + c;
		}
		return hashVal;
	}
};
