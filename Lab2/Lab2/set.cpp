#include "set.h"

/*****************************************************
* Implementation of the member functions             *
******************************************************/

//Default constructor
Set::Set()
	: counter{ 0 }
{
	head = new Node();
	tail = new Node();
	head->next = tail;
	tail->prev = head;
	//IMPLEMENT before HA session on week 15
}


//Conversion constructor
Set::Set(int n)
	: Set()
{
	tail->prev = tail->prev->next = new Node(n, tail, tail->prev);
	counter++;
}


//Constructor to create a Set from a SORTED array
Set::Set(int a[], int n) // a is sorted
	: Set()
{
	for (int i = 0; i < n; i++) {
		tail->prev = tail->prev->next = new Node(a[i], tail, tail->prev);
		counter++;
	}
	//IMPLEMENT before HA session on week 15
}


//Make the set empty
void Set::make_empty()
{
	Node* current = head->next;
	while (current != tail) {
		Node* next = current->next;
		delete current;
		current = next;
	}
	head->next = tail;
	tail->prev = head;
	counter = 0;
	//IMPLEMENT before HA session on week 15
}


Set::~Set()
{
	make_empty();
	delete head;
	delete tail;
	//Member function make_empty() can be used to implement the desctructor
	//IMPLEMENT before HA session on week 15

}


//Copy constructor
Set::Set(const Set& source)
	: Set()
{
	Node* currentS = source.head->next;

	while (currentS != source.tail) {

		//Node *newNode = new Node{ currentS->value, tail, tail->next };
		//tail->prev = tail->prev->next = newNode;
		tail->prev->prev->next = tail->prev = new Node{ currentS->value, tail, tail->prev };
		currentS = currentS->next;
		counter++;
	}
	//IMPLEMENT before HA session on week 15
}

Set::Set(Set&& source)
{
	head = source.head;
	std::cout << head->next->value << endl;
	tail = source.tail;
	counter = source.counter;

	source.head = new Node();
	source.tail = new Node();
	source.head->next = source.tail;
	source.tail->prev = source.head;
	source.counter = 0;
}


//Copy-and-swap assignment operator
//Note that call-by-value is used for source parameter
Set& Set::operator=(Set _copy)
{
	std::swap(_copy.head, head);
	std::swap(_copy.tail, tail);
	std::swap(_copy.counter, counter);

	return *this;
}

//Test whether a set is empty
bool Set::_empty() const
{
	return (!counter);
}


//Test set membership
bool Set::is_member(int val) const
{
	//IMPLEMENT before HA session on week 15
	Node* current = head;
	while (current->next != tail) {
		if (current->next->value == val) {
			return true;
		}
		current = current->next;
	}

	return false; //remove this line
}



//Return number of elements in the set
unsigned Set::cardinality() const
{
	return counter;
}



//Modify Set *this such that it becomes the union of *this with Set S
//Add to Set *this all elements in Set S (repeated elements are not allowed)
//Algorithm used in exercise 5, of lesson 1 in TNG033 is useful to implement this function
Set& Set::operator+=(const Set& S)
{
	//IMPLEMENT before HA session on week 15
	Node* currentThis = head->next;
	Node* currentS = S.head->next;

	//Merge S1 with S2
	while (currentThis != tail && currentS != S.tail)
	{
		if (currentThis->value < currentS->value)
		{
			currentThis = currentThis->next;
		}
		else if (currentS->value < currentThis->value)
		{
			currentThis->prev = currentThis->prev->next = new Node(currentS->value, currentThis, currentThis->prev);
			currentS = currentS->next;
		}
		else //S1[count1] == S2[count2]
		{
			currentS = currentS->next;
			currentThis = currentThis->next;
		}
	}

	//copy any remaining values from S1 to S3
	while (currentS != S.tail)
	{
		tail->prev = tail->prev->next = new Node(currentS->value, tail, tail->prev);
		currentS = currentS->next;
	}

	return *this;
}


//Modify Set *this such that it becomes the intersection of *this with Set S
Set& Set::operator*=(const Set& S)
{
	Node* currentThis = head->next;
	Node* currentS = S.head->next;

	while (currentThis != tail && currentS != S.tail)
	{
		if (currentThis->value < currentS->value)
		{
			Node* tempNode = currentThis->next;
			tempNode->prev = currentThis->prev;
			tempNode->prev->next = tempNode;
			delete currentThis;
			currentThis = tempNode;
		}
		else if (currentS->value < currentThis->value)
		{
			currentS = currentS->next;
		}
		else //S1[count1] == S2[count2]
		{
			currentS = currentS->next;
			currentThis = currentThis->next;
		}
	}

	//copy any remaining values from S1 to S3
	while (currentThis != tail)
	{
		Node* tempNode = currentThis->next;
		tempNode->prev = currentThis->prev;
		tempNode->prev->next = tempNode;
		delete currentThis;
		currentThis = tempNode;
	}

	return *this;
}


//Modify Set *this such that it becomes the Set difference between *this and Set S
Set& Set::operator-=(const Set& S)
{
	Node* currentThis = head->next;
	Node* currentS = S.head->next;

	while (currentThis != tail && currentS != S.tail)
	{
		if (currentThis->value < currentS->value)
		{
			currentThis = currentThis->next;
		}
		else if (currentS->value < currentThis->value)
		{
			currentS = currentS->next;
		}
		else //S1[count1] == S2[count2]
		{
			Node* tempNode = currentThis->next;
			tempNode->prev = currentThis->prev;
			tempNode->prev->next = tempNode;
			delete currentThis;
			currentThis = tempNode;
		}
	}

	return *this;
}

//Return true, if the set is a subset of b, otherwise false
//a <= b iff every member of a is a member of b
bool Set::operator<=(const Set& b) const
{
	Node* currentThis = head->next;
	Node* currentS = b.head->next;

	while (currentThis != tail && currentS != b.tail)
	{
		if (currentThis->value < currentS->value)
		{
			return false;
		}
		else if (currentS->value < currentThis->value)
		{
			currentThis = currentThis->next;
		}
		else //S1[count1] == S2[count2]
		{
			currentThis = currentThis->next;
			currentS = currentS->next;
		}
	}

	if (currentThis != tail) return false;

	return true;
}


//Return true, if the set is equal to set b
//a == b, iff a <= b and b <= a
bool Set::operator==(const Set& b) const
{
	//IMPLEMENT

	return (*this <= b && b <= *this);
}


//Return true, if the set is different from set b
//a == b, iff a <= b and b <= a
bool Set::operator!=(const Set& b) const
{
	//IMPLEMENT

	return !(*this == b);
}


//Return true, if the set is a strict subset of S, otherwise false
//a == b, iff a <= b but not b <= a
bool Set::operator<(const Set& b) const
{
	//IMPLEMENT

	return (*this <= b && *this != b); //remove this line
}


// Overloaded operator<<
ostream& operator<<(ostream& os, const Set& b)
{
	if (b._empty())
	{
		os << "Set is empty!" << endl;
	}
	else
	{
		auto temp = b.head->next;

		os << "{ ";
		while (temp != b.tail)
		{
			os << temp->value << " ";
			temp = temp->next;
		}

		os << "}" << endl;
	}

	return os;
}


