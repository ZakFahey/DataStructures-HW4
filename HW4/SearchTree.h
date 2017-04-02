#pragma once
#include "Node.h"
#include "Exceptions.h"
#include <vector>
#include <tuple>
using namespace std;

template <class T>
class SearchTree
{
private:
	Node<T> *firstNode;
	int size;
	tuple<Node<T>*, Node<T>*> FindWithParent(T *item);
	vector<T*> GetAllOrderedInner(Node<T> *node, int index);

public:
	SearchTree();
	void Insert(T *item);
	T * Find(T *item);
	vector<T*> GetAllAscending();
	vector<T*> GetAllDescending();
	void Empty();
	T * Remove(T *item);
	~SearchTree();
};

//Returns a tuple with the first index being the parent of the item and the second being the found item.
template<class T>
inline tuple<Node<T>*, Node<T>*> SearchTree<T>::FindWithParent(T * item)
{
	Node<T> *node = firstNode;
	Node<T> *prevNode = nullptr;
	while (true)
	{
		int side;
		if (node == nullptr || *item == *node->value)
		{
			return make_tuple(prevNode, node);
		}
		else if (*item < *node->value)
		{
			side = 0;
		}
		else if (*item > *node->value)
		{
			side = 1;
		}

		prevNode = node;
		node = node->children[side];
	}
}

//Internal method to return the items in the tree. childIndex: 0 = ascending; 1 = descending
template<class T>
inline vector<T*> SearchTree<T>::GetAllOrderedInner(Node<T>* node, int childIndex)
{
	//Array concatenation with standard arrays is just too much, especially since you don't know the length of the array. Vectors will do.
	if (node == nullptr) return vector<T*>{};

	//Concatenate these 3 vectors together
	vector<T*> items = GetAllOrderedInner(node->children[childIndex], childIndex);
	vector<T*> items2 { node->value };
	vector<T*> items3 = GetAllOrderedInner(node->children[-childIndex + 1], childIndex); //Opposite of first child
	items.insert(items.end(), items2.begin(), items2.end());
	items.insert(items.end(), items3.begin(), items3.end());

	return items;
}

template<class T>
inline SearchTree<T>::SearchTree()
{
	size = 0;
	firstNode = nullptr;
}

//Inserts an item into the tree
template<class T>
inline void SearchTree<T>::Insert(T * item)
{
	if (firstNode == nullptr)
	{
		size++;
		firstNode = new Node(item);
		return;
	}

	Node<T> *node = firstNode;
	while (true)
	{
		int side;
		if (*item < *node->value)
		{
			side = 0;
		}
		else if (*item > *node->value)
		{
			side = 1;
		}
		else //Duplicate item
		{
			throw ItemAlreadyExistsException();
		}

		if (node->children[side] == nullptr)
		{
			//Successful insert
			size++;
			node->children[side] = new Node(item);
			//TODO: balance tree
			return;
		}
		else
		{
			node = node->children[side];
		}
	}
}

template<class T>
inline T * SearchTree<T>::Find(T * item)
{
	//Return the item from FindWithParent
	return get<1>(FindWithParent(item))->value;
}

//Returns a vector of all the items in the tree in ascending order
template<class T>
inline vector<T*> SearchTree<T>::GetAllAscending()
{
	return GetAllOrderedInner(firstNode, 0);
}

//Returns a vector of all the items in the tree in descending order
template<class T>
inline vector<T*> SearchTree<T>::GetAllDescending()
{
	return GetAllOrderedInner(firstNode, 1);
}

//Safely removes all items from the tree
template<class T>
inline void SearchTree<T>::Empty()
{
	size = 0;
	if(firstNode != nullptr)
	{
		firstNode->Empty();
		firstNode = nullptr;
	}
}

template<class T>
inline T * SearchTree<T>::Remove(T * item)
{
	//Items are already found using inner method
	tuple<Node<T>*, Node<T>*> itemParent = FindWithParent(item);
	Node<T> *parent = get<0>(itemParent);
	Node<T> *item = get<1>(itemParent);

	//TODO: remove and balance
	size--;
}

template<class T>
inline SearchTree<T>::~SearchTree()
{
	Empty();
}