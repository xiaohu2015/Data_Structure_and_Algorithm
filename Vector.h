/*
Vector.h
STL中的vector的实现：使用数组实现列表
author: Ye Hu
2016/10/08
*/

#ifndef VECTOR_H_
#define VECTOR_H_

#include <algorithm>

template<typename Object>
class Vector
{
public:
	// 禁止隐式转换
	explicit Vector(int initSize = 0) : theSize{ initSize },
		theCapacity {initSize + SPARE_CAPACITY}
	{
		objects = new Object[theCapacity];
	}

	// copy构造函数
	Vector(const Vector& rhs) : theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity },
		objects{nullptr}
	{
		objects = new Object[theCapacity];
		// 浅copy
		for (int k = 0; k < theSize; k++)
		{
			objects[k] = rhs.objects[k];
		}
	}
	// 析构函数
	~Vector()
	{
		delete[] objects;
	}

	// move构造函数
	Vector(Vector && rhs) :theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity },
		objects{rhs.objects}
	{
		rhs.objects = nullptr;
		rhs.theCapacity = 0;
		rhs.theSize = 0;
	}

	// 重载赋值操作符
	Vector& operator=(const Vector& rhs)
	{
		Vector copy = rhs; // 调用copy构造函数
		std::swap(*this, copy);	 // 内部使用move构造函数和move赋值操作符
		return *this;
	}
	 // move赋值操作符
	Vector& operator=(Vector&& rhs)
	{
		// 利用移动语义
		std::swap(theSize, rhs.theSize);
		std::swap(theCapacity, rhs.theCapacity);
		std::swap(objects, rhs.objects);
		return *this;
	}

	// 重置大小
	void resize(int newSize)
	{
		if (newSize > theCapacity)
		{
			reserve(newSize * 2);
		}
		theSize = newSize;
	}

	void reserve(int newCapacity)
	{
		if (newCapacity < theSize)
		{
			return;   // 无作用
		}
		Object* newArray = new Object[newCapacity];
		for (int k = 0; k < theSize; ++k)
		{
			newArray[k] = std::move(objects[k]);  // 获取右值,将会使用move赋值函数,避免重复赋值对象
		}
		theCapacity = newCapacity;
		std::swap(objects, newArray);	 // 交换两个指针的指向
		delete[] newArray;	   // 实际删除的旧的
	}

	// []运算符
	Object& operator[](int index)
	{
		return objects[index];
	}
	const Object& operator[](int index)	const	 // const Vector对象使用
	{
		return objects[index];
	}

	// 是否为空
	bool empty() const { return size() == 0; }
	// 大小
	int size()	const { return theSize; }
	// 当前容量
	int capacity() const { return theCapacity; }

	// 末尾插入元素
	void push_back(const Object& x)
	{
		if (theSize == theCapacity)
		{
			reserve(2 * theCapacity + 1);  // 扩张容量
		}
		objects[theSize++] = x;
	}

	// 末尾插入元素（使用右值）
	void push_back(Object && x)
	{
		if (theSize == theCapacity)
		{
			reserve(2 * theCapacity + 1);
		}
		objects[theSize++] = std::move(x);
	}

	// 删除末尾元素
	void pop_back()
	{
		--theSize;
	}

	// 返回最后一个元素
	const Object& back() const
	{
		return objects[theSize - 1];
	}

	// 返回第一个元素
	const Object& front() const
	{
		return objects[0];
	}

	// 定义迭代器
	typedef Object* iterator;
	typedef const Object* const_iterator;

	// 初始元素位置
	iterator begin()
	{
		return &objects[0];
	}
	const_iterator begin() const
	{
		return &objects[0];
	}

	// 末尾元素位置
	iterator end()
	{
		return &objects[size()];
	}

	const_iterator end()  const
	{
		return &objects[size()];
	}


	static const int SPARE_CAPACITY = 16;  // 备用容量
private:
	int theSize;     // 实际存储元素大小
	int theCapacity;  // 当前容量
	Object* objects;  // 存储数组
};
#endif