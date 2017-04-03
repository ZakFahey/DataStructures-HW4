#pragma once

template<class T>
class Node
{
public:
	Node<T>();
	Node<T>(T *value);
	void Empty();
	bool red;
	T *value;
	Node<T> *parent;
	Node<T> *children[2];
};

template<class T>
inline Node<T>::Node()
{
	red = true;
	value = nullptr;
	parent = nullptr;
	children[0] = nullptr;
	children[1] = nullptr;
}

template<class T>
inline Node<T>::Node(T *value_in)
{
	red = true;
	value = value_in;
	parent = nullptr;
	children[0] = nullptr;
	children[1] = nullptr;
}

template<class T>
inline void Node<T>::Empty()
{
	if (value != nullptr)
	{
		delete value;
		value = nullptr;
	}
	if (children[0] != nullptr)
	{
		children[0]->Empty();
		children[0] = nullptr;
	}
	if (children[1] != nullptr)
	{
		children[1]->Empty();
		children[1] = nullptr;
	}
}