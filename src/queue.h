#ifndef C_FLYPES_QUEUE_H
#define C_FLYPES_QUEUE_H

#define MAX 16

int peek();
bool isEmpty();
bool isFull();
int size();
void insert(int data);
int removeData();

// true if given_vertex and desired_vertex are in the same component
// false otherwise
bool is_in_same_component(int cr_num, int adjacency_matrix[cr_num][cr_num], int given_vertex, int desired_vertex);

// true if graph given by adjacency_matrix is connected
// false otherwise
bool is_connected(int cr_num, int adjacency_matrix[cr_num][cr_num]);


#endif //C_FLYPES_QUEUE_H
