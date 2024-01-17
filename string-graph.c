/*
 * Project Description: In this project we created a program that can
 * create multiple data structures that we could manipulate to hold
 * information about graphs such as its vertices and edges. Each of these
 * structures will represent adjacency lists that will hold the information
 * of each edge and vertices such as the edges weight, the vertices names
 * while also checking if there name weights and edge connections are valid.
 * In summary I will be using singly linked linked list to code this program
 * to store graphs. Program will also free the memory used from allocating for
 * the vertices and edges and removes then from the graph as well as deleting
 * the entire graph if the user so desires.
 *
 * Name: Nicolas Aparicio-Felix
 * UID: 119365847
 * Project: 8
 * Directory ID: naparic1
 * Class/Section: CMSC 216/0202
 * Honor Pledge: I pledge on my honor that
 * I have not given or received
 * any unauthorized assistance on this assignment
 */

#include "string-graph.h"
#include <stdlib.h>
#include <string.h>

/* Helper function that will sort alphabetically
and add the vertices into my graph and indicating
if it did so correctly by returning 1. If it found
a vertex with the same name as my node->name then
it will return 0 to indicate a duplicate */
static short helper_insertion_sort(String_graph *const graph, Vertex *node) {
    Vertex *curr = NULL, *prev = NULL, *new_item = NULL;
    short result = 0;

    if (graph->head != NULL)
        curr = graph->head;

    if (node->name != NULL)
        result = strcmp(curr->name, node->name);

    if (result == 0)
        return 0;

    /* Inserting in the middle when this condition is met */
    while (curr != NULL && result < 0) {
        prev = curr;
        curr = curr->next;
        if (curr != NULL) {
            /* Comparing the vertice names to indicate a duplicate */
            if (node->name != NULL)
                result = strcmp(curr->name, node->name);

            if (result == 0)
                return 0;
        }
    }

    /* Inserting at the end of the list */
    new_item = node;
    new_item->next = curr;

    if (prev == NULL)
        graph->head = node;
    else
        prev->next = node;

    return 1;
}

/* Initializes graph parameter if it is not null.
If so it should not have an effect */
void graph_init(String_graph *const graph) {
    if (graph != NULL) {
        graph->head = NULL;
        graph->vertex_count = 0;
    }
}

/* Adds vertex to our graph in sorted order with it returning 1
on success and 0 if it found a duplicate vertex when trying to add
it to the list */
short add_vertex_to_graph(String_graph *const graph, const char new_vertex[]) {
    Vertex *new_node = NULL;

    if (graph == NULL)
        return 0;

    new_node = malloc(sizeof(struct vertex));

    /* Initialize all struct fields to avoid memory issues */
    new_node->next = NULL;
    new_node->edge_list = NULL;
    new_node->edge_count = 0;

    if (new_vertex == NULL)
        new_node->name = NULL;
    else {
        /* Allocate strings to allow any size array to be passed in */
        new_node->name = malloc(strlen(new_vertex) + 1);
        strcpy(new_node->name, new_vertex);
    }

    /* Insertion at head */
    if (graph->head == NULL)
        graph->head = new_node;
    else {
        /* Insertion in the middle/end */
        if (helper_insertion_sort(graph, new_node) == 0)
            return 0;
    }

    graph->vertex_count++;

    return 1;
}

/* Finds the number of vertices in our graph and
returns that value */
short vertex_count(const String_graph *const graph) {
    if (graph == NULL)
        return 0;

    return graph->vertex_count;
}

/* Compares vertices to find if that vertice exists already
and returns 1 if this is true and 0 if not */
short isa_vertex(const String_graph *const graph, const char name[]) {
    Vertex *curr = NULL;
    int i = 0;

    if (graph != NULL)
        curr = graph->head;

    for (i = 0; i < graph->vertex_count; i++) {

        if (strcmp(curr->name, name) == 0)
            return 1;

        curr = curr->next;
    }

    return 0;
}

/* Returns our list of vertices list in sorted order after
copying it over from our list of vertices */
char **get_vertex_list(const String_graph *const graph) {
    int size = graph->vertex_count, i = 0;
    Vertex *curr = graph->head;

    /* Allocates memory for the array of strings. First allocate
   memory for the rows and then for each string
   and then copy those strings into our name_cpy variable */
    char **name_cpy = malloc(sizeof(char *) * (size + 1));

    if (graph == NULL)
        return NULL;

    /* Loop to go through each vertice and
     copy it over to an allocated string */
    for (i = 0; i < size; i++) {
        *(name_cpy + i) = malloc(strlen(curr->name) + 1);
        strcpy(*(name_cpy + i), curr->name);
        curr = curr->next;
    }

    *(name_cpy + size) = NULL;

    return name_cpy;
}

/* Insert edge from our source parameter
to dest parameter which includes the data of the weight.
If the edge was added successfully it'll return 1. If
not it could return 0 if the weight of the edge is negative */
short create_graph_edge(String_graph *const graph, const char source[],
                        const char dest[], int weight) {
    Vertex *curr = NULL;
    Edge *new_edge = NULL, *list = NULL, *prev = NULL;
    int result = 0;

    if (weight < 0 || graph == NULL || source == NULL || dest == NULL)
        return 0;

    curr = graph->head;

    /* Checking to see if our vertice name matches with a src or dest
    If our vertice name does match with either, we create a new vertice */
    if (isa_vertex(graph, source) == 0)
        add_vertex_to_graph(graph, source);

    if (isa_vertex(graph, dest) == 0)
        add_vertex_to_graph(graph, dest);

    curr = graph->head;

    while (curr != NULL) {
        result = strcmp(source, curr->name);
        if (result == 0) {
            /* all three memebers of edge are initialized,
            now ready to insert edge */
            new_edge = malloc(sizeof(Edge));
            new_edge->weight = weight;
            new_edge->dest_name = malloc(strlen(dest) + 1);
            strcpy(new_edge->dest_name, dest);
            new_edge->next_edge = NULL;

            list = curr->edge_list;

            /* Insert our graphs edges in sorted order while handling
            the cases of it being inserted at the head, tail, or the middle */
            while (list != NULL && strcmp(list->dest_name, dest) <= 0) {

                /* If middle node edge is a
                duplicate than we update weight */
                if (strcmp(list->dest_name, dest) == 0) {
                    list->weight = weight;
                    new_edge = NULL;

                    return 1;
                }
                prev = list;
                list = list->next_edge;
            }

            if (prev == NULL) {
                curr->edge_list = new_edge;
                new_edge->next_edge = list;
            } else {
                prev->next_edge = new_edge;
                new_edge->next_edge = list;
            }

            curr->edge_count++;
            return 1;
        }
        curr = curr->next;
    }

    return 0;
}

/* Traverses through the edge list to then find and return our edge weights
It will return -1 if it could not find the desired dest or source or if the
edge from source to dest does not exist */
short get_graph_edge_weight(const String_graph *const graph,
                            const char source[], const char dest[]) {
    Vertex *curr = NULL;
    Edge *list;
    int result = 0;

    if (graph == NULL || source == NULL || dest == NULL)
        return -1;

    if (graph != NULL)
        curr = graph->head;

    while (curr != NULL) {
        result = strcmp(source, curr->name);

        if (result == 0) {
            list = curr->edge_list;

            /* If list is null, nothing
            should be compared since it's nothing to compare */
            while (list != NULL) {
                if (strcmp(list->dest_name, dest) == 0)
                    return list->weight;

                list = list->next_edge;
            }
            return -1;
        }
        curr = curr->next;
    }

    return -1;
}

/* Finds and returns the number of
outgoing edges for our graphs vertices
If it could not find the a vertex
with the specified name then it'll return -1 */
short neighbor_count(const String_graph *const graph, const char vertex[]) {
    Vertex *name_v = NULL;
    int neighbor_count = 0;

    if (graph == NULL || vertex == NULL)
        return -1;

    if (graph != NULL)
        name_v = graph->head;

    while (name_v != NULL) {
        if (strcmp(vertex, name_v->name) == 0) {
            if (name_v->edge_list != NULL)
                neighbor_count = name_v->edge_count;
            else
                neighbor_count = 0;

            return neighbor_count;
        }

        name_v = name_v->next;
    }

    return -1;
}

/* Frees each vertex name given the array of vertices name
If the list is NULL than nothing happens in this function */
void free_vertex_name_list(char **const names) {
    int i = 0;

    if (names != NULL) {
        /* Iterate through names and free each one */
        while (*(names + i) != NULL) {
            free(*(names + i));
            i++;
        }

        free(names);
    }
}

/* Helper function to find the edge list of a specific vertex
Once it was able to find that edge_list it will return that list
and if not it will return */
static Vertex *get_edges(const String_graph *const graph, const char vertex[]) {
    Vertex *curr = NULL;

    if (graph == NULL)
        return NULL;
    else
        curr = graph->head;

    /* Loop through vertex list to find matching
    vertex name and return that vertex edge list */
    while (curr != NULL) {
        if (strcmp(vertex, curr->name) == 0)
            return curr;

        curr = curr->next;
    }

    return NULL;
}

/* Function that will find the specified vertex given from its parameter
and sort the given vertice edge list in alphabetical order and return
that sorted list on success and return NULL on failure */
char **get_neighbor_names(const String_graph *const graph,
                          const char vertex[]) {
    int size = 0, i = 0;
    char **new_arr = NULL;
    Vertex *specified_vertex = NULL;
    Edge *desired_edge_list = NULL;

    if (graph == NULL)
        return NULL;
    else {
        /* Initializes our variables if our graph isn't NULL */
        size = neighbor_count(graph, vertex);
        new_arr = malloc(sizeof(char *) * (size + 1));
        specified_vertex = get_edges(graph, vertex);

        if (specified_vertex == NULL)
            return NULL;
        else
            desired_edge_list = specified_vertex->edge_list;

        /* Copies over edge names into allocated string array */
        for (i = 0; i < size; i++) {
            *(new_arr + i) = malloc(strlen(desired_edge_list->dest_name) + 1);
            strcpy(*(new_arr + i), desired_edge_list->dest_name);
            desired_edge_list = desired_edge_list->next_edge;
        }

        /* Sets our last element to NULL */
        *(new_arr + size) = NULL;

        return new_arr;
    }
}

/* Remove an edge with the specified source vertex and edge dest,
    decrement edge count for vertex */
int remove_graph_edge(String_graph *const graph, const char source[],
                      const char dest[]) {
    Vertex *curr = NULL;
    Edge *prev_edge = NULL, *curr_edge;

    if (graph->head != NULL) {
        curr = graph->head;

        /* Loop to find the specific source vertex we want to remove the edge*/
        while (curr != NULL && strcmp(curr->name, source) != 0)
            curr = curr->next;

        if (curr != NULL) {
            if (curr->edge_list == NULL)
                return 0;

            curr_edge = curr->edge_list;
            /* loop through edges to find edge to remove */
            while (curr_edge != NULL &&
                   strcmp(curr_edge->dest_name, dest) != 0) {
                prev_edge = curr_edge;
                curr_edge = curr_edge->next_edge;
            }

            /* Deletes specified vertex using our edge list parameter */
            if (curr_edge != NULL) {
                if (prev_edge == NULL)
                    curr->edge_list = curr_edge->next_edge;
                else
                    prev_edge->next_edge = curr_edge->next_edge;

                free(curr_edge->dest_name);
                curr_edge->next_edge = NULL;
                curr_edge->dest_name = NULL;
                free(curr_edge);
                curr_edge = NULL;
                curr->edge_count--;
            }
        }
    }

    return 1;
}

/* Removes the specified vertex by first removing the vertices incoming and
outgoing edges and then removing the specified */
int remove_vertex_from_graph(String_graph *const graph, const char vertex[]) {
    Vertex *curr = NULL, *prev = NULL;
    Edge *curr_edge = NULL;

    if (vertex != NULL) {

        if (graph == NULL)
            return 0;

        if (graph->head != NULL)
            curr = graph->head;
        else if (graph->head == NULL)
            return 0;

        curr_edge = curr->edge_list;

        /* Loops thru all vertices to find edges to remove*/
        while (curr != NULL) {
            while (curr_edge != NULL) {

                if (remove_graph_edge(graph, curr->name, vertex) == 1) {
                    break;
                }
                curr_edge = curr_edge->next_edge;
            }

            prev = curr;
            curr = curr->next;
            if (curr != NULL)
                curr_edge = curr->edge_list;
        }

        /* Removes the vertex: this reloads the graph since all vertices
        looped thru above when removing edges */
        curr = graph->head;

        /* Prev set back to NULL for end of a vertex when there is only two
        vertices left */
        prev = NULL;
        while (curr != NULL && strcmp(curr->name, vertex) != 0) {
            prev = curr;
            curr = curr->next;
        }

        /* If vertex not found, return 0 */
        if (curr == NULL)
            return 0;

        /* Removes the outgoing edges of the current vertex */
        while (curr->edge_list != NULL && curr != NULL)
            remove_graph_edge(graph, vertex, curr->edge_list->dest_name);

        if (prev == NULL)
            graph->head = curr->next;
        else
            prev->next = curr->next;

        free(curr->name);
        free(curr);
        graph->vertex_count--;

        return 1;
    }
    
    return 0;
}

/* Deletes graph by freeing all of the memory that was allocated
by creating the vertices and edges. If our graph is NULL
then there should be no effect */
void graph_delete(String_graph *const graph) {
    Vertex *curr_v = NULL, *next_v = NULL;
    Edge *curr_e = NULL, *next_e = NULL;

    if (graph != NULL) {
        curr_v = graph->head;
        curr_e = graph->head->edge_list;
        /* Loop to free all vertices */
        while (curr_v != NULL) {
            /* Loop to free all edges of specified vertex
            first before freeing the vertex itself */
            while (curr_e != NULL) {
                next_e = curr_e->next_edge;
                free(curr_e->dest_name);
                free(curr_e);
                curr_e = next_e;
            }
            next_v = curr_v->next;
            free(curr_v->name);
            free(curr_v);
            curr_v = next_v;
            if (curr_v != NULL)
                if (curr_v->edge_list != NULL)
                    curr_e = curr_v->edge_list;
        }

        graph->vertex_count = 0;
    }
}
