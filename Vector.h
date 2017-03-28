/*
Vector.h
STL�е�vector��ʵ�֣�ʹ������ʵ���б�
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
	// ��ֹ��ʽת��
	explicit Vector(int initSize = 0) : theSize{ initSize },
		theCapacity {initSize + SPARE_CAPACITY}
	{
		objects = new Object[theCapacity];
	}

	// copy���캯��
	Vector(const Vector& rhs) : theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity },
		objects{nullptr}
	{
		objects = new Object[theCapacity];
		// ǳcopy
		for (int k = 0; k < theSize; k++)
		{
			objects[k] = rhs.objects[k];
		}
	}
	// ��������
	~Vector()
	{
		delete[] objects;
	}

	// move���캯��
	Vector(Vector && rhs) :theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity },
		objects{rhs.objects}
	{
		rhs.objects = nullptr;
		rhs.theCapacity = 0;
		rhs.theSize = 0;
	}

	// ���ظ�ֵ������
	Vector& operator=(const Vector& rhs)
	{
		Vector copy = rhs; // ����copy���캯��
		std::swap(*this, copy);	 // �ڲ�ʹ��move���캯����move��ֵ������
		return *this;
	}
	 // move��ֵ������
	Vector& operator=(Vector&& rhs)
	{
		// �����ƶ�����
		std::swap(theSize, rhs.theSize);
		std::swap(theCapacity, rhs.theCapacity);
		std::swap(objects, rhs.objects);
		return *this;
	}

	// ���ô�С
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
			return;   // ������
		}
		Object* newArray = new Object[newCapacity];
		for (int k = 0; k < theSize; ++k)
		{
			newArray[k] = std::move(objects[k]);  // ��ȡ��ֵ,����ʹ��move��ֵ����,�����ظ���ֵ����
		}
		theCapacity = newCapacity;
		std::swap(objects, newArray);	 // ��������ָ���ָ��
		delete[] newArray;	   // ʵ��ɾ���ľɵ�
	}

	// []�����
	Object& operator[](int index)
	{
		return objects[index];
	}
	const Object& operator[](int index)	const	 // const Vector����ʹ��
	{
		return objects[index];
	}

	// �Ƿ�Ϊ��
	bool empty() const { return size() == 0; }
	// ��С
	int size()	const { return theSize; }
	// ��ǰ����
	int capacity() const { return theCapacity; }

	// ĩβ����Ԫ��
	void push_back(const Object& x)
	{
		if (theSize == theCapacity)
		{
			reserve(2 * theCapacity + 1);  // ��������
		}
		objects[theSize++] = x;
	}

	// ĩβ����Ԫ�أ�ʹ����ֵ��
	void push_back(Object && x)
	{
		if (theSize == theCapacity)
		{
			reserve(2 * theCapacity + 1);
		}
		objects[theSize++] = std::move(x);
	}

	// ɾ��ĩβԪ��
	void pop_back()
	{
		--theSize;
	}

	// �������һ��Ԫ��
	const Object& back() const
	{
		return objects[theSize - 1];
	}

	// ���ص�һ��Ԫ��
	const Object& front() const
	{
		return objects[0];
	}

	// ���������
	typedef Object* iterator;
	typedef const Object* const_iterator;

	// ��ʼԪ��λ��
	iterator begin()
	{
		return &objects[0];
	}
	const_iterator begin() const
	{
		return &objects[0];
	}

	// ĩβԪ��λ��
	iterator end()
	{
		return &objects[size()];
	}

	const_iterator end()  const
	{
		return &objects[size()];
	}


	static const int SPARE_CAPACITY = 16;  // ��������
private:
	int theSize;     // ʵ�ʴ洢Ԫ�ش�С
	int theCapacity;  // ��ǰ����
	Object* objects;  // �洢����
};
#endif