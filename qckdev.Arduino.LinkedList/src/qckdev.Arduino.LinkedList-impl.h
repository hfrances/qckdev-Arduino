#pragma once
#include "qckdev.Arduino.LinkedList.h"

template<typename T>
LinkedList<T>::LinkedList()
{
	_size = 0;
	_firstNode = nullptr;
	_lastNode = nullptr;
	setIterator(NAN, nullptr);
}

template<typename T>
LinkedList<T>::~LinkedList()
{
	clear();
}

template<typename T>
int LinkedList<T>::getSize() const
{
	return _size;
}

template<typename T>
T LinkedList<T>::getAt(int index, const T defaultValue)
{
	Node<T>* node = getNode(index);

	return (node == nullptr ? defaultValue : node->value);
}

template<typename T>
bool LinkedList<T>::tryGetAt(int index, T& value)
{
	Node<T>* node = getNode(index);

	if (node == nullptr) {
		return false;
	}
	else {
		value = node->value;
		return true;
	}
}

template<typename T>
int LinkedList<T>::add(T value)
{
	insert(_size, value);
	return _size;
}

template<typename T>
void LinkedList<T>::insert(int index, T value)
{
	Node<T>* node = new Node<T>();

	node->value = value;

	if (_firstNode == nullptr) {
		// First item.
		_firstNode = node;
		_lastNode = node;
		node->previous = nullptr;
		node->next = nullptr;
		setIterator(_size, node);
	}
	else if (index >= _size) {
		// Append item.
		_lastNode->next = node;
		node->previous = _lastNode;
		node->next = nullptr;
		_lastNode = node;
		setIterator(_size, node);
	}
	else {
		Node<T>* iteratorNode = getNode(index);

		// Is first node?
		if (iteratorNode->previous == nullptr) {
			_firstNode = node;
		}
		else {
			iteratorNode->previous->next = node;
		}

		node->previous = iteratorNode->previous;
		node->next = iteratorNode;
		iteratorNode->previous = node;
		setIterator(index, node);
	}
	_size++;
}

template<typename T>
void LinkedList<T>::replaceAt(int index, T value)
{
	Node<T>* iteratorNode = getNode(index);

	if (iteratorNode == nullptr) {
		add(value);
	}
	else {
		iteratorNode->value = value;
	}
}

template<typename T>
T LinkedList<T>::removeAt(int index) {
	Node<T>* iteratorNode = getNode(index);

	if (iteratorNode != nullptr) { // no items.

		// Update previous (if this is the first, the next one become the new one).
		if (iteratorNode->previous == nullptr) {
			_firstNode = iteratorNode->next;
		}
		else {
			iteratorNode->previous->next = iteratorNode->next;
		}

		// Update previous (if this is the last, the previous one become the new one).
		if (iteratorNode->next == nullptr) {
			_lastNode = iteratorNode->previous;
			setIterator(index - 1, _lastNode);
		}
		else {
			iteratorNode->next->previous = iteratorNode->previous;
			setIterator(index, iteratorNode->next);
		}
		delete iteratorNode;
		_size--;
	}
}


template<typename T>
void LinkedList<T>::clear() {
	Node<T>* node;

	setIterator(NAN, nullptr);
	while (_lastNode != nullptr)
	{
		// remove last replacing for the previous one until there is no more items.
		node = _lastNode;
		_lastNode = node->previous;
		if (_lastNode == nullptr) {
			_firstNode = nullptr;
		}
		else {
			_lastNode->next = nullptr;
		}
		delete node;
		_size--;
	}
}

template<typename T>
void LinkedList<T>::fromArray(T* array, int arrayLength)
{
	if (_size > 0)
		clear();

	for (int i = 0; i < arrayLength; i++)
	{
		add(array[i]);
	}
}

template<typename T>
T* LinkedList<T>::toArray()
{
	T* rdo = new T[_size];

	if (_size > 0) {
		Node<T>* node = _firstNode;
		for (size_t i = 0; i < _size; i++)
		{
			rdo[i] = node->value;
			node = node->next;
		}
	}
	return rdo;
}

template<typename T>
T* LinkedList<T>::toArrayReverse()
{
	T* rdo = new T[_size];

	if (_size > 0) {
		Node<T>* node = _lastNode;

		for (size_t i = 0; i < _size; i++)
		{
			rdo[i] = node->value;
			node = node->previous;
		}
	}
	return rdo;
}

template<typename T>
void LinkedList<T>::setIterator(int index, Node<T>* node) {
	_iteratorNode = node;
	_iteratorIndex = index;
}

template<typename T>
inline Node<T>* LinkedList<T>::getNode(int index)
{
	Node<T>* iteratorNode = _iteratorNode;
	Node<T>* rdo = _iteratorNode;
	size_t i = _iteratorIndex;
	size_t iRdo = _iteratorIndex;
	bool subtract;

	subtract = (i > index ? true : false);
	while (iRdo != index && iteratorNode != nullptr) {
		rdo = iteratorNode;
		iRdo = i;

		if (subtract) {
			iteratorNode = iteratorNode->previous;
			i--;
		}
		else {
			iteratorNode = iteratorNode->next;
			i++;
		}
	}
	setIterator(iRdo, rdo);
	return rdo;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::getIterator() {
	return typename LinkedList<T>::Iterator(this);
}

template<typename T>
LinkedList<T>::Iterator::Iterator(const LinkedList<T>* source) {
	_source = source;
	_isNew = true;
	_current = nullptr;
}

template<typename T>
LinkedList<T>::Iterator::Iterator(LinkedList<T>* source) {
	_source = source;
	_isNew = true;
	_current = nullptr;
}

template<typename T>
bool LinkedList<T>::Iterator::moveFirst() {
	_current = _source->_firstNode;

	return (_current != nullptr);
}

template<typename T>
bool LinkedList<T>::Iterator::moveLast() {
	_current = _source->_lastNode;

	return (_current != nullptr);
}

template<typename T>
bool LinkedList<T>::Iterator::moveNext() {
	if (_current == nullptr) {
		if (_isNew) {
			moveFirst();
			_isNew = false;
		}
	}
	else {
		_current = _current->next;
	}

	return (_current != nullptr);
}

template<typename T>
bool LinkedList<T>::Iterator::movePrevious() {
	if (_current == nullptr) {
		if (_isNew) {
			moveLast();
			_isNew = false;
		}
	}
	else {
		_current = _current->previous;
	}

	return (_current != nullptr);
}

template<typename T>
T LinkedList<T>::Iterator::getCurrent() {
	return _current->value;
}



/*

void TESTS() {
	LinkedList<float> list;
	Serial.println("a0");
	Serial.println(toString(list.getAt(0, NAN)));
	Serial.println("a1");
	list.insert(0, 0);
	Serial.println("a2");
	list.add(2);
	Serial.println("a3");
	list.insert(1, 1);
	Serial.println("a4");
	list.insert(99, 3);
	Serial.println("a5");
	list.insert(0, -1);
	Serial.println("a6");
	list.insert(99, 4);
	Serial.println("a7");
	list.insert(3, 1.5);
	Serial.println("a8");
	list.insert(3, 1.2);
	Serial.println("a9");
	list.insert(5, 1.7);
	Serial.println("a10");
	list.add(5);
	Serial.println("b1");
	printArray(list.toArray(), list.getSize());
	Serial.println("b2");
	printArray(list.toArrayReverse(), list.getSize());
	Serial.println("b3");
	Serial.println("Value[0] = " + toString(list.getAt(0, NAN)));
	Serial.println("b4");
	Serial.println("Value[4] = " + toString(list.getAt(4, NAN)));
	Serial.println("b5");
	Serial.println("Value[99] = " + toString(list.getAt(99, NAN)));
	Serial.println("c1");
	list.removeAt(3);
	list.removeAt(0);
	list.removeAt(99);
	printArray(list.toArray(), list.getSize());
	printArray(list.toArrayReverse(), list.getSize());
	Serial.println("c2");
	{
		LinkedList<float>::Iterator itor = list.getIterator();
		Serial.print("Values: ");
		while (itor.moveNext()) {
			Serial.print(toString(itor.getCurrent()) + ", ");
		}
		Serial.println();
	}
	{
		LinkedList<float>::Iterator itor = list.getIterator();
		Serial.print("Values: ");
		while (itor.movePrevious()) {
			Serial.print(toString(itor.getCurrent()) + ", ");
		}
		Serial.println();
	}
	Serial.println("c3");
	list.clear();
	Serial.println("d1");
	printArray(list.toArray(), list.getSize());
	Serial.println("d2");
	Serial.println("Value[0] = " + toString(list.getAt(0, NAN)));
	Serial.println("d3");
	Serial.println("Value[99] = " + toString(list.getAt(99, NAN)));
	Serial.println("e");
	float floatArr[] = { 1, 10, 100, 1000 };
	list.fromArray(floatArr, sizeof(floatArr) / sizeof(float));
	printArray(list.toArray(), list.getSize());

	Serial.println("f");
	LinkedList<WiFiConfigInfo> list2;
	Serial.println("f1");
	//list2.add(WiFiConfigInfo("ssid2", "pwd"));
	WiFiConfigInfo item2 = list2.getAt(0, emptyWiFiConfigInfo);
	Serial.println("Value: " + String(item2 != emptyWiFiConfigInfo ? "true" : "false"));
	Serial.println("f2");
	Serial.println("Value: " + String(list2.tryGetAt(0, item2) ? "true" : "false"));

	Serial.println("g");
	LinkedList<WiFiConfigInfo*> list3;
	//WiFiConfigInfo value3("ssid3", "pwd");
	Serial.println("g1");
	//list3.add(&value3);
	WiFiConfigInfo* item3 = list3.getAt(0);
	Serial.println("Value: " + String(item3 != nullptr ? "true" : "false"));
	Serial.println("g2");
	Serial.println("Value: " + String(list3.tryGetAt(0, item3) ? "true" : "false"));

	Serial.println("h");
}



void printArray(float* arr, int size) {
	Serial.print("Values: ");
	for (size_t i = 0; i < size; i++)
	{
		if (i > 0) Serial.print(", ");
		Serial.print(toString(arr[i]));
	}
	Serial.println();
}

String toString(float number) {
	String value = String(number);
	int dot = value.indexOf('.');

	if (dot > -1) {
		size_t i = value.length() - 1;

		while (i >= dot && (value[i] == '0' || value[i] == '.')) {
			i--;
		}
		value = value.substring(0, i + 1);
	}
	return value;
}


*/