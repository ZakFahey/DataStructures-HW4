#pragma once
#include "Node.h"
#include "Exceptions.h"
#include <vector>
#include <tuple>
using namespace std;

template<class T>
class SearchTree
{
private:
	Node<T> *root;
	int size;
	tuple<Node<T>*, Node<T>*> FindWithParent(T *item);
	vector<T*> GetAllOrderedInner(Node<T> *node, int index);
	void Balance(Node<T> *node);
	void RotateLeft(Node<T> *pivot);
	void RotateRight(Node<T> *pivot);

public:
	SearchTree();
	void Insert(T *item);
	T * Find(T *item);
	vector<T*> GetAllAscending();
	vector<T*> GetAllDescending();
	void EmptyTree();
	T * Remove(T *item);
	~SearchTree();
};

//Returns a tuple with the first index being the parent of the item and the second being the found item.
template<class T>
inline tuple<Node<T>*, Node<T>*> SearchTree<T>::FindWithParent(T *item)
{
	Node<T> *node = root;
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
inline vector<T*> SearchTree<T>::GetAllOrderedInner(Node<T> *node, int childIndex)
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
inline void SearchTree<T>::Balance(Node<T> *node)
{
	// if node is root or node is black
	if (node == root || node->parent->red == false)
		return;

	// assign grandparent, parent, and siblings
	Node<T> *parent = node->parent;
	Node<T> *grand = parent->parent;
	Node<T> *sibling = nullptr;
	Node<T> *parentSibling = nullptr;
	if (parent->children[0] == node)
		sibling = parent->children[1];
	else
		sibling = parent->children[0];
	if (grand->children[0] == parent)
		parentSibling = grand->children[1];
	else
		parentSibling = grand->children[0];

	// if parent's sibling is red, recolor
	// else rotate and recolor
	if (parentSibling != nullptr && parentSibling->red == true)
	{
		parent->red = false;
		parentSibling->red = false;
		grand->red = true;

		// if grand's parent is red, rotate
		if (grand->parent != nullptr && grand->parent->red == true)
		{
			if (grand == grand->parent->children[0])
				RotateRight(grand->parent);
			else
				RotateLeft(grand->parent);
		}
	}
	else
	{
		if (parent == grand->children[0])
			RotateRight(grand);
		else
			RotateLeft(grand);
		parent->red = false;
		if (parentSibling != nullptr)
			parentSibling->red = true;
	}
}

template<class T>
inline void SearchTree<T>::RotateLeft(Node<T> *pivot)
{
	if (pivot->children[1] == nullptr)
		return;

	// declare right and set equal pivot's right
	Node<T> *right = pivot->children[1];

	// set pivot's right equal to right's left
	pivot->children[1] = right->children[0];

	// if right's left is a leaf, set its parent equal to pivot
	if (right->children[0] == nullptr)
	{
		right->children[0] = new Node<T>;
		right->children[0]->parent = pivot;
	}

	// set right's parent equal to pivot's parent
	right->parent = pivot->parent;

	// if pivot's parent equals nullptr, set root equal to right
	// else if pivot equals its parent's left, set parent's left equal to right
	// else set parent's right equal to right
	if (pivot->parent == nullptr)
		root = right;
	else if (pivot == pivot->parent->children[0])
		pivot->parent->children[0] = right;
	else
		pivot->parent->children[1] = right;

	// set right's left equal to pivot and pivot's parent equal to right
	right->children[0] = pivot;
	pivot->parent = right;
}

template<class T>
inline void SearchTree<T>::RotateRight(Node<T> *pivot)
{
	// declare right and set equal pivot's right
	Node<T> *left = pivot->children[0];

	// set pivot's left equal to left's right
	pivot->children[0] = left->children[1];

	// if left's right is a leaf, set its parent equal to pivot
	if (left->children[1] == nullptr)
	{
		left->children[1] = new Node<T>;
		left->children[1]->parent = pivot;
	}

	// set left's parent equal to pivot's parent
	left->parent = pivot->parent;

	// if pivot's parent equals nullptr, set root equal to left
	// else if pivot equals its parent's left, set parent's left equal to left
	// else set parent's right equal to left
	if (pivot->parent == nullptr)
		root = left;
	else if (pivot == pivot->parent->children[0])
		pivot->parent->children[0] = left;
	else
		pivot->parent->children[1] = left;

	// set left's right equal to pivot and pivot's parent equal to left
	left->children[1] = pivot;
	pivot->parent = left;
}

template<class T>
inline SearchTree<T>::SearchTree()
{
	size = 0;
	root = nullptr;
}

//Inserts an item into the tree
template<class T>
inline void SearchTree<T>::Insert(T *item)
{
	if (root == nullptr)
	{
		size++;
		root = new Node<T>(item);
		root->red = false;
		root->parent = nullptr;
		return;
	}

	// parent and grandparent nodes needed for rebalancing
	Node<T> *node = root;

	while (true)
	{
		// determines which side to move to
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
			return;
		}

		// sets leaf node or moves on
		if (node->children[side] == nullptr)
		{
			//Successful insert
			size++;
			node->children[side] = new Node<T>(item);
			node->children[side]->parent = node;
			Balance(node->children[side]);
			root->red = false;
			return;
		}
		else
		{
			node = node->children[side];
		}
	}
}

template<class T>
inline T * SearchTree<T>::Find(T *item)
{
	//Return the item from FindWithParent
	Node<T> *node = get<1>(FindWithParent(item));
	if (node != nullptr)
		return node->value;
	return nullptr;
}

//Returns a vector of all the items in the tree in ascending order
template<class T>
inline vector<T*> SearchTree<T>::GetAllAscending()
{
	return GetAllOrderedInner(root, 0);
}

//Returns a vector of all the items in the tree in descending order
template<class T>
inline vector<T*> SearchTree<T>::GetAllDescending()
{
	return GetAllOrderedInner(root, 1);
}

//Safely removes all items from the tree
template<class T>
inline void SearchTree<T>::EmptyTree()
{
	size = 0;
	if(root != nullptr)
	{
		root->Empty();
		root = nullptr;
	}
}

template<class T>
inline T * SearchTree<T>::Remove(T *item)
{
	//Items are already found using inner method
	tuple<Node<T>*, Node<T>*, Node<T>*> itemParent = FindWithParent(item);
	Node<T> *parent = get<0>(itemParent);
	Node<T> *node = get<1>(itemParent);
	T *ret = nullptr;
	if (node != nullptr)
	{
		ret = node->value;

		// assign child of parent
		if (node->children[0] == nullptr && node->children[1] == nullptr)
		{
			if (node == parent->children[0])
				parent->children[0] = nullptr;
			else
				parent->children[1] = nullptr;
		}

		// balance tree and decrement size
		Balance(node);
		size--;
	}

	// return item
	return ret;
}

template<class T>
inline SearchTree<T>::~SearchTree()
{
	EmptyTree();
}