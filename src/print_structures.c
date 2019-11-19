#include "print_structures.h"

// print comment length >
// ------------------------------------------------
//

void print_adjacency_matrix(int cr_num, int adjacency_matrix[cr_num][cr_num]) {

    printf("\n----------------Adjacency Matrix----------------\n");
    for(int i = 0; i < cr_num; i++) {
        for(int j = 0; j < cr_num; j++) {
            printf("%d ", adjacency_matrix[i][j]);
        }
        printf("\n");
    }
    printf("------------------------------------------------\n");

}

void print_edge_list(int cr_num, int edge_list[2 * cr_num][2]) {

    printf("\n------------------- Edge List ------------------\n");
    for(int i = 0; i < 2 * cr_num; i++) {
        printf("{%d, %d}\n", edge_list[i][0], edge_list[i][1]);
    }
    printf("------------------------------------------------\n");

}