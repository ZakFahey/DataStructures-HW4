#pragma once

template <class T>
class Node
{
public:
	Node(T *value);
	T *value;
	Node *children[2];
	void Empty();
};

template<class T>
inline Node<T>::Node(T *value)
{
	this->value = value;
	children = new Node[2]{ nullptr, nullptr };
}

template<class T>
inline void Node<T>::Empty()
{
	delete value;
	for(int i = 0; i < 2; i++)
	{
		if (children[0] != nullptr)
			delete children[0];
	}
	delete this;
}
