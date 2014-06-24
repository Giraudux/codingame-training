// Alexis Giraudet
// june 2014
// gcc -std=c99

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

struct node;

void start(void);
void end(void);
void add_link(int n0, int n1);
void add_node_link(int index, int id);
void remove_link(int n0, int n1);
void remove_node_link(int index, int id);
void set_gateway(int n);
int next_gateway(int n);
int linked_gateway(void);
void print_graph(void);
int main(void);

struct node * graph;
int n_nodes;
int n_links;
int n_gateways;

struct node {
	bool _is_gateway;// = false;
	int _n_nodes;// = 0;
	//struct node ** _ap_nodes;// = NULL;
	int * _id_nodes;// = NULL;
};

const struct node NODE_INIT = {false, 0, NULL};

void start(void)
{
	char * line;
	size_t s_line;
	ssize_t s_getline;
	int i, n0, n1;
	
	//#init destructor
	if(atexit(end) != 0)
	{
		fprintf(stderr, "atexit: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	//#init n_nodes, n_links, n_gateways and read line
	line = NULL;
	s_line = 0;
	s_getline = getline(&line, &s_line, stdin);
	if(s_getline == -1)
	{
		fprintf(stderr, "getline: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	n_nodes = 0;
	n_links = 0;
	n_gateways = 0;
	if(sscanf(line ,"%d %d %d", &n_nodes, &n_links, &n_gateways) != 3)
	{
		fprintf(stderr, "sscanf: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	free(line);
	line = NULL;
	s_line = 0;
	
	//#init graph
	graph = NULL;
	if((graph = calloc(n_nodes, sizeof(struct node))) == NULL)
	{
		fprintf(stderr, "malloc: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	//#threading
	for(i=0; i<n_nodes; i++)
	{
		//graph[i] = NODE_INIT;
		graph[i]._is_gateway = false;
		graph[i]._n_nodes = 0;
		//graph[i]._ap_nodes = NULL;
		graph[i]._id_nodes = NULL;
	}
	
	//#init links #threading
	for(i=0; i<n_nodes; i++)
	{
		s_getline = getline(&line, &s_line, stdin);
		if(s_getline == -1)
		{
			fprintf(stderr, "getline: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		if(sscanf(line ,"%d %d", &n0, &n1) != 2)
		{
			fprintf(stderr, "sscanf: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		free(line);
		line = NULL;
		s_line = 0;
		
		add_link(n0, n1);
	}
	
	//#init gateways #threading
	for(i=0; i<n_gateways; i++)
	{
		s_getline = getline(&line, &s_line, stdin);
		if(s_getline == -1)
		{
			fprintf(stderr, "getline: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		if(sscanf(line ,"%d", &n0) != 1)
		{
			fprintf(stderr, "sscanf: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		free(line);
		line = NULL;
		s_line = 0;
		
		set_gateway(n0);
	}
}

void end(void)
{
	int i;
	
	if(graph != NULL)
	{
		for(i=0; i<n_nodes; i++)
		{
			//if(graph[i]._ap_nodes != NULL)
			if(graph[i]._id_nodes != NULL)
			{
				//free(graph[i]._ap_nodes);
				free(graph[i]._id_nodes);
				//graph[i]._ap_nodes = NULL;
				graph[i]._id_nodes = NULL;
			}
		}
		free(graph);
		graph = NULL;
	}
}

void add_link(int n0, int n1)
{
	add_node_link(n0, n1);
	add_node_link(n1, n0);
}

void add_node_link(int index, int id)
{
	int * new_id_nodes;
	//#malloc
	if(graph[index]._id_nodes == NULL)
	{
		if((graph[index]._id_nodes = calloc(graph[index]._n_nodes+1, sizeof(int))) == NULL)
		{
			fprintf(stderr, "malloc: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	//#realloc
	else
	{
		new_id_nodes = realloc(graph[index]._id_nodes, (graph[index]._n_nodes+1)*sizeof(int));
		if(new_id_nodes == NULL)
		{
			fprintf(stderr, "realloc: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		else
		{
			graph[index]._id_nodes = new_id_nodes;
		}
	}
	graph[index]._id_nodes[graph[index]._n_nodes] = id;
	graph[index]._n_nodes++;
}

void remove_link(int n0, int n1)
{
	remove_node_link(n0, n1);
	remove_node_link(n1, n0);
}

void remove_node_link(int index, int id)
{
	int * new_id_nodes;
	int i;
	if(graph[index]._id_nodes != NULL)
	{
		for(i=0; i<graph[index]._n_nodes; i++)
		{
			if(graph[index]._id_nodes[i] == id)
			{
				//#free
				if(graph[index]._n_nodes == 1)
				{
					free(graph[index]._id_nodes);
					graph[index]._id_nodes = NULL;
				}
				//#realloc
				else
				{
					if(i != (graph[index]._n_nodes-1))
					{
						graph[index]._id_nodes[i] = graph[index]._id_nodes[graph[index]._n_nodes-1];
					}
					new_id_nodes = realloc(graph[index]._id_nodes, (graph[index]._n_nodes-1)*sizeof(int));
					if(new_id_nodes == NULL)
					{
						fprintf(stderr, "realloc: %s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}
					else
					{
						graph[index]._id_nodes = new_id_nodes;
					}
				}
				break;
			}
		}
	}
	graph[index]._n_nodes--;
}

void set_gateway(int n)
{
	graph[n]._is_gateway = true;
}

int next_gateway(int n)
{
	int i;
	if(graph[n]._id_nodes != NULL)
	{
		for(i=0; i<graph[n]._n_nodes; i++)
		{
			if(graph[graph[n]._id_nodes[i]]._is_gateway)
			{
				return graph[n]._id_nodes[i];
			}
		}
	}
	return -1;
}

int linked_gateway(void)
{
	int i;
	for(i=0; i<n_nodes; i++)
	{
		if((graph[i]._is_gateway) && (graph[i]._id_nodes != NULL))
		{
			return i;
		}
	}
	return -1;
}

void print_graph(void)
{
	int i, j;
	if(graph != NULL)
	{
		for(i=0; i<n_nodes; i++)
		{
			fprintf(stdout, "%d %d ->", i, graph[i]._is_gateway);
			if(graph[i]._id_nodes != NULL)
			{
				for(j=0; j<graph[i]._n_nodes; j++)
				{
					fprintf(stdout, " %d", graph[i]._id_nodes[j]);
				}
			}
			fprintf(stdout, "\n");
		}
	}
}

int main(void)
{
	int n_pos, n_del;
	char * line;
	size_t s_line;
	ssize_t s_getline;
	
	start();
	//print_graph();
	
	line = NULL;
	s_line = 0;
	while(true)
	{
		s_getline = getline(&line, &s_line, stdin);
		if(s_getline == -1)
		{
			fprintf(stderr, "getline: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		if(sscanf(line ,"%d", &n_pos) != 1)
		{
			fprintf(stderr, "sscanf: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		free(line);
		line = NULL;
		s_line = 0;
		
		n_del = next_gateway(n_pos);
		if(n_del == -1)
		{
			n_del = linked_gateway();
			if(n_del == -1)
			{
				fprintf(stderr, "linked_gateway: no linked gateway available\n");
				exit(EXIT_FAILURE);
			}
			n_pos = graph[n_del]._id_nodes[0];
		}
		remove_link(n_pos, n_del);
		fprintf(stdout, "%d %d\n", n_pos, n_del);
	}
	return EXIT_SUCCESS;
}
