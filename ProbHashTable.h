#pragma once

// 开放寻址法 Open addressing

#include <vector>
#include <string>
#include <functional>
#include <algorithm>

using std::vector;
using std::hash;
using std::string;

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


template <typename HashObj>
class ProbHashTable
{
public:
	explicit ProbHashTable(int size = 101):
		array(size)
	{
		makeEmpty();
	}

	bool contains(const HashObj& x) const
	{
		return isActive(findPos(x));
	}

	bool insert(const HashObj& x)
	{
		int pos = findPos(x);
		if (isActive(pos)) return false;

		array[pos].element = x;
		array[pos].type = ACTIVE;

		if (++currentSize > array.size() / 2)
		{
			rehash();
		}
		return true;
	}

	bool insert(HashObj&& x)
	{
		int pos = findPos(x);
		if (isActive(pos)) return false;

		array[pos].element = std::move(x);
		array[pos].type = ACTIVE;

		if (++currentSize > array.size() / 2)
		{
			rehash();
		}
		return true;
	}

	bool remove(const HashObj& x)
	{
		int pos = findPos(x);
		if (!isActive(pos)) return false;

		array[pos].type = DELETED;
		--currentSize;
		return true;
	}

	void makeEmpty()
	{
		currentSize = 0;
		for (HashEntry& entry : array)
		{
			entry.type = EMPTY;
		}
	}

	enum  EntryType { ACTIVE, EMPTY, DELETED};

private:

	struct HashEntry
	{
		HashObj element;
		EntryType type;

		HashEntry(const HashObj& e = HashObj{}, EntryType t = EMPTY) :
			element{e}, type{t}
		{}

		HashEntry(HashObj&& e, EntryType t = EMPTY) :
			element{ std::move(e) }, type{ t }
		{}
	};

	vector<HashEntry> array;
	int currentSize;

	bool isActive(int pos) const { return array[pos].type == ACTIVE; }

	int findPos(const HashObj& x) const
	{
		int offset = 1;
		int currentPos = myHash(x);

		while (array[currentPos].type != EMPTY &&
			array[currentPos].element != x)
		{
			currentPos += offset;
			offset += 2;
			if (currentPos >= array.size())
			{
				currentPos -= array.size();
			}
		}
		return currentPos;
	}

	void rehash() 
	{
		vector<HashEntry> oldArray = array;
		array.resize(nextPrime(2 * array.size()));
		for (auto& entry : array)
		{
			entry.type = EMPTY;
		}

		currentSize = 0;
		for (auto& entry : oldArray)
		{
			if (entry.type == ACTIVE)
			{
				insert(std::move(entry.element));
			}
		}
		
	};

	size_t myHash(const HashObj& x) const
	{
		static hash<HashObj> hf;
		return hf(x) % array.size();
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
