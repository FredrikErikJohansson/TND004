#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "dsexceptions.h"
#include <algorithm>
#include <string>
#include <cmath>
using namespace std;

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinarySearchTree
{
	struct BinaryNode
	{
		Comparable element;
		BinaryNode *left;
		BinaryNode *right;
		BinaryNode *parent;

		BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt, BinaryNode *pt)
			: element{ theElement }, left{ lt }, right{ rt }, parent{ pt } { }

		BinaryNode(Comparable && theElement, BinaryNode *lt, BinaryNode *rt, BinaryNode *pt)
			: element{ std::move(theElement) }, left{ lt }, right{ rt }, parent{ pt } { }
	};

public:

	class BiIterator
	{
	public:
		BiIterator(BinaryNode *p = nullptr) : current{ p } {  };
		Comparable& operator*() const
		{
			if (current != nullptr) return current->element;
		}

		Comparable* operator->() const
		{
			return current;
		}

		bool operator==(const BiIterator &it) const
		{
			return current == it.current;
		}
		bool operator!=(const BiIterator &it) const
		{
			return current != it.current;
		}

		BiIterator& operator++()
		{
			BinaryNode *prev = current;
			current = find_successor(current);
			if (prev == current) *this = BiIterator();
			return *this;
		}

		BiIterator operator++(int)
		{
			BinaryNode *prev = current;
			current = find_successor(current);
			if (prev == current) *this = BiIterator();
			return *this;
		}

		BiIterator& operator--()
		{
			BinaryNode *prev = current;
			current = find_predecessor(current);
			if (prev == current) *this = BiIterator();
			return *this;
		}

		BiIterator operator--(int)
		{
			BinaryNode *prev = current;
			current = find_predecessor(current);
			if (prev == current) *this = BiIterator();
			return *this;
		}
	private:
		BinaryNode *current;
	};

	BiIterator begin() const
	{
		if (isEmpty()) return end();
		return BiIterator(findMin(root));
	}
	BiIterator end() const
	{
		return BiIterator();
	}

	BinarySearchTree() : root{ nullptr }
	{
	}

	/**
	 * Copy constructor
	 */
	BinarySearchTree(const BinarySearchTree & rhs) : root{ nullptr }
	{
		root = clone(rhs.root);
		clone_parents(root, nullptr);
	}

	/**
	 * Move constructor
	 */
	BinarySearchTree(BinarySearchTree && rhs) : root{ rhs.root }
	{
		rhs.root = nullptr;
	}

	/**
	 * Destructor for the tree
	 */
	~BinarySearchTree()
	{
		makeEmpty();
	}


	/**
	 * Copy assignment: copy and swap idiom
	 */
	BinarySearchTree & operator=(BinarySearchTree _copy)
	{
		std::swap(root, _copy.root);
		return *this;
	}


	/**
	 * Find the smallest item in the tree.
	 * Throw UnderflowException if empty.
	 */
	const Comparable & findMin() const
	{
		if (isEmpty())
		{
			throw UnderflowException{ };
		}

		return findMin(root)->element;
	}

	/**
	 * Find the largest item in the tree.
	 * Throw UnderflowException if empty.
	 */
	const Comparable & findMax() const
	{
		if (isEmpty())
		{
			throw UnderflowException{ };
		}

		return findMax(root)->element;
	}

	/**
	 * Returns true if x is found in the tree.
	 */
	BiIterator contains(const Comparable & x) const
	{
		return BiIterator(contains(x, root));
	}

	Comparable get_parent(const Comparable & x) const
	{
		BinaryNode* t = (contains(x, root));
		if (t != nullptr && t->parent != nullptr)
			return t->parent->element;
		return Comparable{};
	}

	void find_pred_succ(const Comparable& x, Comparable& pred, Comparable& suc)
	{
		BinaryNode* t = contains(x, root);
		BinaryNode* temp;

		if (t == nullptr)
		{
			insert(x);
			t = contains(x, root);

			temp = find_successor(t);
			if (findMax(root)->element < x) temp = nullptr;
			suc = (temp != nullptr) ? temp->element : Comparable{};

			temp = find_predecessor(t);
			if (findMin(root)->element > x) temp = nullptr;
			pred = (temp != nullptr) ? temp->element : Comparable{};

			remove(x);
		}
		else
		{
			suc = find_successor(t)->element;
			pred = find_predecessor(t)->element;
		}

	}

	/**
	 * Test if the tree is logically empty.
	 * Return true if empty, false otherwise.
	 */
	bool isEmpty() const
	{
		return root == nullptr;
	}

	/**
	 * Print the tree contents in sorted order.
	 */
	void printTree(ostream & out = cout) const
	{
		if (isEmpty())
		{
			out << "Empty tree" << endl;
		}
		else
		{
			printTree(root, out, "");
		}
	}

	/**
	 * Make the tree logically empty.
	 */
	void makeEmpty()
	{
		root = makeEmpty(root);
	}

	/**
	 * Insert x into the tree; duplicates are ignored.
	 */
	void insert(const Comparable & x)
	{
		root = insert(x, root, nullptr);
	}

	/**
	 * Insert x into the tree; duplicates are ignored.
	 */
	void insert(Comparable && x)
	{
		root = insert(std::move(x), root, nullptr);
	}

	/**
	 * Remove x from the tree. Nothing is done if x is not found.
	 */
	void remove(const Comparable & x)
	{
		root = remove(x, root);
	}

private:
	BinaryNode *root;



	/**
	 * Private member function to insert into a subtree.
	 * x is the item to insert.
	 * t is the node that roots the subtree.
	 * Return a pointer to the node storing x.
	 */
	BinaryNode* insert(const Comparable & x, BinaryNode* t, BinaryNode* currentPoint)
	{

		if (t == nullptr)
		{
			t = new BinaryNode{ x, nullptr, nullptr, currentPoint };
		}
		else if (x < t->element)
		{
			t->left = insert(x, t->left, t);
		}
		else if (t->element < x)
		{
			t->right = insert(x, t->right, t);
		}
		else
		{
			;  // Duplicate; do nothing
		}

		return t;
	}

	/**
	 * Private member function to insert into a subtree.
	 * x is the item to insert.
	 * t is the node that roots the subtree.
	 * Return a pointer to the node storing x.
	 */
	BinaryNode* insert(Comparable && x, BinaryNode* t)
	{
		if (t == nullptr)
		{
			t = new BinaryNode{ std::move(x), nullptr, nullptr };
		}
		else if (x < t->element)
		{
			t->left = insert(std::move(x), t->left);
		}
		else if (t->element < x)
		{
			t->right = insert(std::move(x), t->right);
		}
		else
		{
			;  // Duplicate; do nothing
		}

		return t;
	}


	/**
	 * Private member function to remove from a subtree.
	 * x is the item to remove.
	 * t is the node that roots the subtree.
	 * Return a pointer to the new root of the subtree that had root x
	 */
	BinaryNode* remove(const Comparable & x, BinaryNode *t)
	{
		if (t == nullptr)
		{
			return t;   // Item not found
		}
		if (x < t->element)
		{
			t->left = remove(x, t->left);
		}
		else if (t->element < x)
		{
			t->right = remove(x, t->right);
		}

		else if (t->left != nullptr && t->right != nullptr) // Two children
		{
			t->element = findMin(t->right)->element;
			t->right = remove(t->element, t->right);
		}
		else
		{
			BinaryNode *oldNode = t;
			t = (t->left != nullptr) ? t->left : t->right;
			//We changed here
			if (t != nullptr) t->parent = oldNode->parent;
			delete oldNode;
		}

		return t;
	}


	/**
	 * Private member function to find the smallest item in a subtree t.
	 * Return node containing the smallest item.
	 */
	static BinaryNode * findMin(BinaryNode *t)
	{
		if (t == nullptr)
		{
			return nullptr;
		}
		if (t->left == nullptr)
		{
			return t;
		}

		//Tail recursion can be easily replaced by a loop
		return findMin(t->left); //recursive call
	}

	/**
	 * Private member function to find the largest item in a subtree t.
	 * Return node containing the largest item.
	 */
	static BinaryNode * findMax(BinaryNode *t)
	{
		if (t != nullptr)
		{
			while (t->right != nullptr)
			{
				t = t->right;
			}
		}

		return t;
	}


	/**
	 * Private member function to test if an item is in a subtree.
	 * x is item to search for.
	 * t is the node that roots the subtree.
	 * Return a pointer to the node storing x, if x is found
	 * Otherwise, return nullptr
	 */
	BinaryNode* contains(const Comparable & x, BinaryNode *t) const
	{
		if (t == nullptr)
		{
			return t;
		}
		else if (x < t->element)
		{
			return contains(x, t->left);
		}
		else if (t->element < x)
		{
			return contains(x, t->right);
		}
		else
		{
			return t;    // Match
		}
	}

	static BinaryNode* climb_tree_succ(BinaryNode* t)
	{
		if (t->parent == nullptr) return t;
		if (t == t->parent->left) return t->parent;
		climb_tree_succ(t->parent);
	}

	static BinaryNode* climb_tree_pred(BinaryNode* t)
	{
		if (t->parent == nullptr) return t;
		if (t == t->parent->right) return t->parent;
		climb_tree_pred(t->parent);
	}

	static BinaryNode* climb_tree_top(BinaryNode* t)
	{
		if (t->parent == nullptr) return t;
		climb_tree_top(t->parent);
	}

	static BinaryNode* find_successor(BinaryNode* t)
	{
		BinaryNode* root = nullptr;
		if (t != nullptr) root = climb_tree_top(t);

		if (findMax(root) == t) return t; //t is largest value in the tree

		if (findMax(root)->element < t->element) return nullptr; //t is largest value in the tree

		if (t != nullptr && t->right != nullptr) //t has a right sub-tree
		{
			return findMin(t->right);
		}
		else //successor is one of the ancestors
		{
			return climb_tree_succ(t);
		}
	}

	static BinaryNode* find_predecessor(BinaryNode* t)
	{
		BinaryNode* root = nullptr;
		if (t != nullptr) root = climb_tree_top(t);

		if (findMin(root) == t) return t; //t is largest value in the tree

		if (findMin(root)->element > t->element) return nullptr; //t is largest value in the tree

		if (t != nullptr && t->left != nullptr) //t has a right sub-tree
		{
			return findMax(t->left);
		}
		else //successor is one of the ancestors
		{
			return climb_tree_pred(t);
		}
	}

	/****** NONRECURSIVE VERSION*************************
		Node* contains( const Comparable & x, BinaryNode *t ) const
		{
			while( t != nullptr )
			{
				if( x < t->element )
				{
					t = t->left;
				}
				else if( t->element < x )
				{
					t = t->right;
				}
				else
				{
					return t;    // Match
				}
			}

			return t;   // No match
		}
	*****************************************************/

	/**
	 * Private member function to make subtree empty.
	 */
	BinaryNode* makeEmpty(BinaryNode *t)
	{
		if (t != nullptr)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}

		return nullptr;
	}

	/**
	 * Private member function to print a subtree rooted at t in sorted order.
	 * In-order traversal is used
	 */
	void printTree(BinaryNode *t, ostream & out, string indent) const
	{
		if (t != nullptr)
		{
			out << indent;
			out << t->element << endl;
			indent += "   ";
			printTree(t->left, out, indent);
			printTree(t->right, out, indent);
		}
	}

	void clone_parents(BinaryNode * t, BinaryNode * lastNode)
	{
		if (t == nullptr) return;
		t->parent = lastNode;
		clone_parents(t->left, t);
		clone_parents(t->right, t);
	}

	/**
	 * Private member function to clone subtree.
	 */
	BinaryNode * clone(BinaryNode *t) const
	{
		if (t == nullptr)
		{
			return nullptr;
		}
		else
		{
			/*
			BinaryNode* newNode;

			if(t->parent != nullptr)
				newNode = new BinaryNode(t->parent->element, t->parent->left, t->parent->right, t->parent->parent);
			else
				newNode = nullptr;
				*/

			return new BinaryNode{ t->element, clone(t->left), clone(t->right), nullptr };
		}
	}
};

#endif
