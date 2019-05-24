/*********************************************
* file:	~\tnd004\lab\lab4a\digraph.cpp       *
* remark: mplementation of directed graphs   *
* file for students                          *
**********************************************/


#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

#include "digraph.h"
#include "queue.h"

const int INF = 9999;
//const int INF = 9999; //Visual Studio seems to dislike the constant name "INFINITY"

// -- CONSTRUCTORS

Digraph::Digraph(int n)
{
	assert(n >= 1);
	array = new List[n + 1];
	dist = new int[n + 1];
	path = new int[n + 1];
	done = new bool[n + 1];
	size = n;
}

// -- DESTRUCTOR

Digraph::~Digraph()
{
	delete[] array;
	delete[] dist;
	delete[] path;
	delete[] done;
}

// -- MEMBER FUNCTIONS

// insert directed edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Digraph::insertEdge(int u, int v, int w)
{
	assert(u >= 1 && u <= size);
	assert(v >= 1 && v <= size);
	array[u].insert(v, w);
}

// remove directed edge (u, v)
void Digraph::removeEdge(int u, int v)
{
	assert(u >= 1 && u <= size);
	assert(v >= 1 && v <= size);
	array[u].remove(v);
}

// unweighted single source shortest paths
void Digraph::uwsssp(int s)
{
	if (s < 1 || s > size)
	{
		cout << "\nERROR: expected source s in range 1.." << size << " !" << endl;
		return;
	}

	Queue<int> Q;

	for (int i = 1; i <= size; i++)
	{
		dist[i] = INF;
		path[i] = 0;
	}

	dist[s] = 0;
	Q.enqueue(s);

	while (!Q.isEmpty())
	{
		int v = Q.getFront();
		Q.dequeue();

		Node *p = array[v].getFirst();

		while (p != nullptr)
		{
			if (dist[p->vertex] == INF)
			{
				dist[p->vertex] = dist[v] + 1;
				path[p->vertex] = v;
				Q.enqueue(p->vertex);
			}
			p = array[v].getNext();
		}
	}
}

// positive weighted single source shortest paths
//Dijktra’s algorithm
void Digraph::pwsssp(int s)
{
	if (s < 1 || s > size)
	{
		cout << "\nERROR: expected source s in range 1.." << size << " !" << endl;
		return;
	}

	for (int i = 1; i <= size; i++)
	{
		dist[i] = INF;
		path[i] = 0;
		done[i] = false;
	}

	dist[s] = 0;
	done[s] = true;

	while (true)
	{
		Node *p = array[s].getFirst();

		while (p != nullptr)
		{
			if (done[p->vertex] == false && dist[p->vertex] > dist[s] + p->weight)
			{
				dist[p->vertex] = dist[s] + p->weight;
				path[p->vertex] = s;
			}

			p = array[s].getNext();
		}

		int smallest = INF;
		for (int i = 1; i <= size; i++)
		{
			if (dist[i] < smallest && done[i] == false)
			{
				smallest = dist[i];
				s = i;

			}
		}
		if (smallest == INF) break;
		done[s] = true;
	}
}

// print graph
void Digraph::printGraph() const
{
	cout << "------------------------------------------------------------------" << endl;
	cout << "vertex  adjacency list                  " << endl;
	cout << "------------------------------------------------------------------" << endl;

	for (int v = 1; v <= size; v++)
	{
		cout << setw(4) << v << " : ";
		array[v].print();
	}

	cout << "------------------------------------------------------------------" << endl;
}

// print shortest path tree for s
void Digraph::printTree() const
{
	cout << "----------------------" << endl;
	cout << "vertex    dist    path" << endl;
	cout << "----------------------" << endl;

	for (int v = 1; v <= size; v++)
	{
		cout << setw(4) << v << " :" << setw(8)
			<< dist[v] << setw(8) << path[v] << endl;
	}

	cout << "----------------------" << endl;
}

// print shortest path from s to t
void Digraph::printPath(int t, int depth) const
{
	if (t < 1 || t > size)
	{
		cout << "\nERROR: expected target t in range 1.." << size << " !" << endl;
		return;
	}

	if (path[t] == 0)
	{
		cout << " " << t;
		if (depth == 0) cout << " (0)" << endl;
		return;
	}
	printPath(path[t], depth + 1);
	cout << " " << t;

	if (depth == 0) cout << " (" << dist[t] << ")" << endl;
}
