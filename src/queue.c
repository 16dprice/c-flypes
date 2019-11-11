#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"

int intArray[MAX] = {-1};
int front = 0;
int rear = -1;
int itemCount = 0;

int peek() {
    return intArray[front];
}

bool isEmpty() {
    return itemCount == 0;
}

bool isFull() {
    return itemCount == MAX;
}

int size() {
    return itemCount;
}

void insert(int data) {

    if(!isFull()) {

        if(rear == MAX-1) {
            rear = -1;
        }

        intArray[++rear] = data;
        itemCount++;
    }
}

int removeData() {
    int data = intArray[front++];

    if(front == MAX) {
        front = 0;
    }

    itemCount--;
    return data;
}

void reset() {
    memset(intArray, -1, sizeof(intArray));
    front = 0;
    rear = -1;
    itemCount = 0;
}

bool is_in_same_component(int cr_num, int adjacency_matrix[cr_num][cr_num], int given_vertex, int desired_vertex) {

    reset();

    bool mark[cr_num];
    for(int i = 0; i < cr_num; i++) {
        mark[i] = false;
    }

    insert(given_vertex);
    mark[given_vertex] = true;

    while(!isEmpty()) {
        int current = removeData();

        for(int i = 0; i < cr_num; i++) {
            if(adjacency_matrix[current][i] > 0 && !mark[i]) {
                mark[i] = true;
                insert(i);
            }
        }
    }

    return mark[desired_vertex];

}

bool is_connected(int cr_num, int adjacency_matrix[cr_num][cr_num]) {

    for(int i = 0; i < cr_num; i++) {
        if(!is_in_same_component(cr_num, adjacency_matrix, 0, i)) {
            return false;
        }
    }
    return true;

}

//int main() {
//
//
//
//}

