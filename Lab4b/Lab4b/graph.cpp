/*********************************************
* file:	~\tnd004\lab\lab4b\graph.h           *
* remark:implementation of undirected graphs *
* file for students                          *
**********************************************/


#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

#include "graph.h"
#include "edge.h"
#include "heap.h"
#include "dsets.h"

const int INF = 9999;

// -- CONSTRUCTORS

Graph::Graph(int n)
{
	assert(n >= 1);
	array = new List[n + 1];
	dist = new int[n + 1];
	path = new int[n + 1];
	done = new bool[n + 1];
	size = n;
}

// -- DESTRUCTOR

Graph::~Graph()
{
	delete[] array;
}

// -- MEMBER FUNCTIONS

// insert undirected edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Graph::insertEdge(int u, int v, int w)
{
	assert(u >= 1 && u <= size);
	assert(v >= 1 && v <= size);
	array[u].insert(v, w);
	array[v].insert(u, w);
}

// remove undirected edge (u, v)
void Graph::removeEdge(int u, int v)
{
	assert(u >= 1 && u <= size);
	assert(v >= 1 && v <= size);
	array[u].remove(v);
	array[v].remove(u);
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const
{
	for (int i = 1; i <= size; ++i)
	{
		dist[i] = INF;
		path[i] = 0;
		done[i] = false;
	}

	dist[1] = 0;
	done[1] = true;
	int v = 1;
	int weightCount = 0;

	while (true)
	{
		Node *p = array[v].getFirst();

		while (p != nullptr)
		{
			if (done[p->vertex] == false && dist[p->vertex] > p->weight)
			{
				dist[p->vertex] = p->weight;
				path[p->vertex] = v;
			}

			p = array[v].getNext();
		}

		int smallest = INF;
		for (int i = 1; i <= size; i++)
		{
			if (dist[i] < smallest && done[i] == false)
			{
				smallest = dist[i];
				v = i;

			}
		}
		if (smallest == INF) break;
		done[v] = true;

		cout << "( " << path[v] << ", " << v << ", " << dist[v] << ")" << endl;
		weightCount += dist[v];
	}

	cout << "Total weight = " << weightCount << endl;
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const
{
	Heap<Edge> H;

	DSets D(size);

	for (int i = 1; i <= size; i++)
	{
		Node* N = array[i].getFirst();

		while (N != nullptr)
		{
			if (N->vertex > i) {
				Edge E(i, N->vertex, N->weight);
				H.insert(E);
			}
			N = N->next;
		}
	}

	int counter = 0;
	int weightCount = 0;

	while (counter < size - 1)
	{
		Edge u = H.deleteMin();
		//cout << "THIS IS U: " << u << endl;
		//

		if (D.find(u.tail) != D.find(u.head))
		{
			cout << u << endl;
			D.join(D.find(u.tail), D.find(u.head));
			weightCount += u.weight;
			counter++;
		}
	}
	cout << "Total weight = " << weightCount << endl;
}

// print graph
void Graph::printGraph() const
{
	cout << "------------------------------------------------------------------" << endl;
	cout << "vertex  adjacency list                                            " << endl;
	cout << "------------------------------------------------------------------" << endl;

	for (int v = 1; v <= size; v++)
	{
		cout << setw(4) << v << " : ";
		array[v].print();
	}

	cout << "------------------------------------------------------------------" << endl;
}
