#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fstream>
using namespace std;
struct AdjListNode
{
	int dest;
	int weight;
	struct AdjListNode* next;
};

struct AdjList
{
struct AdjListNode *head;
};
struct Graph
{
	int V;
	struct AdjList* array;
};
struct AdjListNode* newAdjListNode(
				int dest, int weight)
{
	struct AdjListNode* newNode =
			(struct AdjListNode*)
	malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}
struct Graph* createGraph(int V)
{
	struct Graph* graph = (struct Graph*)
			malloc(sizeof(struct Graph));
	graph->V = V;
	graph->array = (struct AdjList*)
	malloc(V * sizeof(struct AdjList));
	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

void addEdge(struct Graph* graph, int src,
				int dest, int weight)
{
	struct AdjListNode* newNode =
			newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	newNode = newAdjListNode(src, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

struct MinHeapNode
{
	int v;
	int dist;
};
struct MinHeap
{
	int size;	
	int capacity;
	int *pos;
	struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v,
								int dist)
{
	struct MinHeapNode* minHeapNode =
		(struct MinHeapNode*)
	malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap =
		(struct MinHeap*)
	malloc(sizeof(struct MinHeap));
	minHeap->pos = (int *)malloc(
			capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array =
		(struct MinHeapNode**)
				malloc(capacity *
	sizeof(struct MinHeapNode*));
	return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a,
					struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}
void minHeapify(struct MinHeap* minHeap,
								int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size &&
		minHeap->array[left]->dist <
		minHeap->array[smallest]->dist )
	smallest = left;

	if (right < minHeap->size &&
		minHeap->array[right]->dist <
		minHeap->array[smallest]->dist )
	smallest = right;

	if (smallest != idx)
	{
		MinHeapNode *smallestNode =
			minHeap->array[smallest];
		MinHeapNode *idxNode =
				minHeap->array[idx];
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;
		swapMinHeapNode(&minHeap->array[smallest],
						&minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}
int isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap*
								minHeap)
{
	if (isEmpty(minHeap))
		return NULL;
	struct MinHeapNode* root =
				minHeap->array[0];
	struct MinHeapNode* lastNode =
		minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	minHeap->pos[root->v] = minHeap->size-1;
	minHeap->pos[lastNode->v] = 0;
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}
void decreaseKey(struct MinHeap* minHeap,
						int v, int dist)
{
	int i = minHeap->pos[v];

	minHeap->array[i]->dist = dist;

	while (i && minHeap->array[i]->dist <
		minHeap->array[(i - 1) / 2]->dist)
	{
		minHeap->pos[minHeap->array[i]->v] =
									(i-1)/2;
		minHeap->pos[minHeap->array[
							(i-1)/2]->v] = i;
		swapMinHeapNode(&minHeap->array[i],
				&minHeap->array[(i - 1) / 2]);

		i = (i - 1) / 2;
	}
}

bool isInMinHeap(struct MinHeap *minHeap, int v)
{
if (minHeap->pos[v] < minHeap->size)
	return true;
return false;
}

void printArr(int dist[], int n)
{
	cout<<"\n\t\t==========================================================================";
    cout<<"\n\t\t\t        ***************Shortest path finder*************";
    cout<<"\n\t\t==========================================================================\n\n";
	printf("TouristPlaces  Distance from Resort\n");
	for (int i = 0; i < n; ++i)
		cout<<i<<"\t\t\t"<<dist[i]<<"km\n";	
}
void dijkstra(struct Graph* graph, int src)
{
	int V = graph->V;

	int dist[V];	

	struct MinHeap* minHeap = createMinHeap(V);

	for (int v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v,
									dist[v]);
		minHeap->pos[v] = v;
	}

	minHeap->array[src] =
		newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);
	minHeap->size = V;

	while (!isEmpty(minHeap))
	{
		struct MinHeapNode* minHeapNode =
					extractMin(minHeap);
		int u = minHeapNode->v;

		struct AdjListNode* pCrawl =
					graph->array[u].head;
		while (pCrawl != NULL)
		{
			int v = pCrawl->dest;

			if (isInMinHeap(minHeap, v) &&
					dist[u] != INT_MAX &&
			pCrawl->weight + dist[u] < dist[v])
			{
				dist[v] = dist[u] + pCrawl->weight;

				decreaseKey(minHeap, v, dist[v]);
			}
			pCrawl = pCrawl->next;
		}
	}
	printArr(dist, V);
}
void readfile(char* filename)
{
	ifstream f(filename , ios::in);
	if(!f)
	{
		cout<<"File is not found";
	}
	else{
		char line[100];
		while(true)
		{
			f.getline(line , 100);
			if(!f)
			{
				break;
			}

			cout<<line;
		}
		
	}
	f.close();

}
int hashcode(int n)
{
	return n%10;
}

int main()
{   
	int V = 9;
	int choice;
	struct Graph* graph = createGraph(V);
	addEdge(graph, 0, 1, 4);
	addEdge(graph, 0, 7, 8);
	addEdge(graph, 1, 2, 8);
	addEdge(graph, 1, 7, 11);
	addEdge(graph, 2, 3, 7);
	addEdge(graph, 2, 8, 2);
	addEdge(graph, 2, 5, 4);
	addEdge(graph, 3, 4, 9);
	addEdge(graph, 4, 5, 10);
	addEdge(graph, 5, 6, 2);
	addEdge(graph, 6, 7, 1);
	addEdge(graph, 6, 8, 6);
	addEdge(graph, 7, 8, 7);
	system("cls");
	  while(1)
	  {
	  
      cout<<"\n\t\t==========================================================================";
      cout<<"\n\t\t\t        ***************Shortest path finder*************";
      cout<<"\n\t\t==========================================================================";
      cout<<"\n\n\t\tPress '1' to view Roadmap "<<endl;
	  cout<<"\n\t\tPress '2' to Show the Shortest Path from Resorts to Tourist places"<<endl;
	  cout<<"\n\t\tPress '3' to Description of the places   "<<endl;
	  cout<<"\n\t\tPress '4' to exit   "<<endl;
	  cout<<"\n\n\t\tEnter your choice:";
	  cin>>choice;
	  switch(choice)
	  {
      case 1:
	     system("cls");
      cout<<"\n\t\t==========================================================================";
      cout<<"\n\t\t\t        ***************Shortest path finder*************";
      cout<<"\n\t\t==========================================================================";
      cout<<"\n\nThe RoadMap:\n\n\n\n";
	cout<<"\t\t                        (8km)         (7km)                  "<<endl;
	cout<<"\t\t      Elk Hill Murugan ------ Pine  --------  Sterling       "<<endl;
	cout<<"\t\t         Temple              Forest            Resort        "<<endl;
	cout<<"\t\t        /   |                 |    \\              \\        "<<endl;
	cout<<"\t\t       /    |                 |     \\              \\       "<<endl;
	cout<<"\t\t (4km)/     |            (2km)|      \\              \\(9km) "<<endl;
	cout<<"\t\t     /      |                 |       \\              \\     "<<endl;
	cout<<"\t\t    /       |                 |        \\              \\    "<<endl;
	cout<<"\t\t   /        |                 |         \\              \\   "<<endl;
	cout<<"\t\t Avalanche  |(11km)      Western valley  \\ (4km)      Thunder"<<endl;
	cout<<"\t\t   Lake     |          /     Resort       \\            World"<<endl;
	cout<<"\t\t    \\       |         /       |            \\           /   "<<endl;
	cout<<"\t\t     \\      |   (7km)/   (6km)|             \\         /    "<<endl; 
	cout<<"\t\t (8km)\\     |       /         |              \\       /(10km)"<<endl;
	cout<<"\t\t       \\    |      /          |               \\     /      "<<endl;
	cout<<"\t\t        \\   |     /           |                \\   /       "<<endl;
	cout<<"\t\t          Rose    (1km)                (2km)   Green Nest     "<<endl;
	cout<<"\t\t         Garden ---------- Doodabetta --------  Resort       "<<endl;
	cout<<"\t\t                              peak                                \n\n\n  "<<endl;
    	 system("pause");
	     system("cls");
		  break;
      case 2:
		  system("cls");
		  int x;
	cout<<"\n\t\t==========================================================================";
    cout<<"\n\t\t\t        ***************Shortest path finder*************";
    cout<<"\n\t\t==========================================================================\n\n";
    cout<<"\t\t                        (8km)         (7km)                  "<<endl;
	cout<<"\t\t      Elk Hill Murugan ------ Pine  --------  Sterling       "<<endl;
	cout<<"\t\t         Temple              Forest            Resort        "<<endl;
	cout<<"\t\t        /   |                 |    \\              \\        \t\tTop Resorts in OOTY"<<endl;
	cout<<"\t\t       /    |                 |     \\              \\       \t\t********************"<<endl;
	cout<<"\t\t (4km)/     |            (2km)|      \\              \\(9km) \t\t1)Sterling Resort"<<endl;
	cout<<"\t\t     /      |                 |       \\              \\     \t\t2)Green Nest Resort"<<endl;
	cout<<"\t\t    /       |                 |        \\              \\    \t\t3)Western Valley Resort"<<endl;
	cout<<"\t\t   /        |                 |         \\              \\   "<<endl;
	cout<<"\t\t Avalanche  |(11km)      Western valley  \\ (4km)      Thunder"<<endl;
	cout<<"\t\t   Lake     |          /     Resort       \\            World"<<endl;
	cout<<"\t\t    \\       |         /       |            \\           /   "<<endl;
	cout<<"\t\t     \\      |   (7km)/   (6km)|             \\         /    "<<endl; 
	cout<<"\t\t (8km)\\     |       /         |              \\       /(10km)"<<endl;
	cout<<"\t\t       \\    |      /          |               \\     /      "<<endl;
	cout<<"\t\t        \\   |     /           |                \\   /       "<<endl;
	cout<<"\t\t          Rose    (1km)                (2km)   Green Nest     "<<endl;
	cout<<"\t\t         Garden ---------- Doodabetta --------  Resort       "<<endl;
	cout<<"\t\t                              peak                                \n\n\n  "<<endl;
		   
	cout<<"\n\n\t\tEnter your choice :";
	cin>>x;
	cout<<"\n"; 
	switch(x)
	  {
      case 1:
	     system("cls");
         dijkstra(graph, 3);
         cout<<"\n------------------------------------------------";
		 cout<<"\n|0---------------> Avalanche lake               |";
		 cout<<"\n|1 --------------> Elk hill murugan Temple      |";
		 cout<<"\n|2---------------> pine forest                  |";
		 cout<<"\n|3---------------> sterling hotel               |";
		 cout<<"\n|4---------------> Thunder world                |";
		 cout<<"\n|5--------------->Green Nest resort             |";
		 cout<<"\n|6--------------->Doddabetta peak               |";
		 cout<<"\n|7--------------->Rose gardern                  |";
		 cout<<"\n|8--------------->Western valley resort         |";
    	 cout<<"\n------------------------------------------------\n";
    	 cout<<"\nSterling Ooty Resort :";
    	 cout<<"\nContact:+91 9003302727";
    	 cout<<"\nMail us:crs@sterlingholidays.com\n";
		 system("pause");
	     system("cls");
		  break;
      case 2:
		  system("cls");
		  dijkstra(graph, 5);
		 cout<<"\n------------------------------------------------";
		 cout<<"\n|0---------------> Avalanche lake               |";
		 cout<<"\n|1 --------------> Elk hill murugan Temple      |";
		 cout<<"\n|2---------------> pine forest                  |";
		 cout<<"\n|3---------------> sterling hotel               |";
		 cout<<"\n|4---------------> Thunder world                |";
		 cout<<"\n|5--------------->Green Nest resort             |";
		 cout<<"\n|6--------------->Doddabetta peak               |";
		 cout<<"\n|7--------------->Rose gardern                  |";
		 cout<<"\n|8--------------->Western valley resort         |";
    	 cout<<"\n------------------------------------------------\n";
		 cout<<"\nGreen Nest Resort :";
    	 cout<<"\nContact:+91 9600445111";
    	 cout<<"\nMail us:reservations@greennest.in\n";
		  system("pause");
		  system("cls");
		  break;
	  case 3:
		  system("cls");
		 dijkstra(graph, 8);
		  cout<<"\n------------------------------------------------";
		 cout<<"\n|0---------------> Avalanche lake               |";
		 cout<<"\n|1 --------------> Elk hill murugan Temple      |";
		 cout<<"\n|2---------------> pine forest                  |";
		 cout<<"\n|3---------------> sterling hotel               |";
		 cout<<"\n|4---------------> Thunder world                |";
		 cout<<"\n|5---------------> Green Nest resort            |";
		 cout<<"\n|6---------------> Doddabetta peak              |";
		 cout<<"\n|7---------------> Rose gardern                 |";
		 cout<<"\n|8---------------> Western valley resort        |";
    	 cout<<"\n------------------------------------------------\n";
    	 cout<<"\nGreen Nest Resort :";
    	 cout<<"\nContact:+91 90039 30530";
    	 cout<<"\nMail us:westernvalleyresorts@gmail.com\n";
		  system("pause"); 
		  system("cls");
		  break;
        }

case 3:
	    if(choice==3)
	    {
	     system("cls");
	     cout<<"\n\t\t==========================================================================";
    cout<<"\n\t\t\t        ***************TOURISM*************";
    cout<<"\n\t\t==========================================================================\n\n";
		 cout<<"\n\t\t------------------------------------------------";
		 cout<<"\n\t\t|0---------------> Avalanche lake               |";
		 cout<<"\n\t\t|1 --------------> Elk hill murugan Temple      |";
		 cout<<"\n\t\t|2---------------> pine forest                  |";
		 cout<<"\n\t\t|3---------------> sterling hotel               |";
		 cout<<"\n\t\t|4---------------> Thunder world                |";
		 cout<<"\n\t\t|5---------------> Green Nest resort            |";
		 cout<<"\n\t\t|6---------------> Doddabetta peak              |";
		 cout<<"\n\t\t|7---------------> Rose gardern                 |";
		 cout<<"\n\t\t|8---------------> Western valley resort        |";
    	 cout<<"\n\t\t------------------------------------------------\n";
		 cout<<"\n Enter the Description choices : ";
		 {
		 int choices;

		 char arr[10][40] = {"Avalanche_lake.txt" , "Elk hill murugan Temple.txt" , "pine forest .txt" , "sterling hotel.txt" , "Thunder world.txt" ,
		  "Green Nest resort.txt" , "Doddabetta peak.txt" , "Rose garden.txt" , "Western valley resort.txt" };
		 
		 cin>>choices;
		 cout<<arr[hashcode(choices)]<<endl;
		 readfile(arr[hashcode(choices)]);
		 cout<<endl;
		 system("pause");
		 }
	}
	system("cls");
	break;
	 
	}
	if (choice==4)
      {
	   return 0;
      }
}

}

