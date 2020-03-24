
#ifndef _qckdev_Arduino_LinkedListLib_h
#define _qckdev_Arduino_LinkedListLib_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

template<class T>
struct Node
{
	T value;
	Node<T> *next;
	Node<T> *previous;
};

template <typename T>
class LinkedList {

protected:
	int _size;
	Node<T>* _firstNode;
	Node<T>* _lastNode;
	Node<T>* _iteratorNode;
	int _iteratorIndex;

public:
	LinkedList();
	~LinkedList();

	int getSize() const;
	T getAt(int index, const T defaultValue = nullptr);
	bool tryGetAt(int index, T& value);

	int add(T);
	void insert(int index, T);
	void replaceAt(int index, T value);
	T removeAt(int index);

	void clear();
	void fromArray(T* array, int arrayLength);
	T* toArray();
	T* toArrayReverse();

private:
	void setIterator(int index, Node<T>* node);
	Node<T>* getNode(int index);

public:
	class Iterator
	{
	public:
		bool moveFirst();
		bool moveLast();
		bool moveNext();
		bool movePrevious();
		T getCurrent();
		Iterator(LinkedList<T>* source);
		Iterator(const LinkedList<T>* source);

	protected:


	private:
		bool _isNew;
		Node<T>* _current;
		LinkedList<T>* _source;

		Iterator() {}
	};

	Iterator getIterator();
};

#include "qckdev.Arduino.LinkedList-impl.h"

#endif