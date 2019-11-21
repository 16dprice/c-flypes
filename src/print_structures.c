#include "print_structures.h"

// print comment length >
// ------------------------------------------------
//

void print_end_sequence() {
    printf("------------------------------------------------\n");
}

void print_adjacency_matrix(int cr_num, int adjacency_matrix[cr_num][cr_num]) {

    printf("\n----------------Adjacency Matrix----------------\n");
    for(int i = 0; i < cr_num; i++) {
        for(int j = 0; j < cr_num; j++) {
            printf("%d ", adjacency_matrix[i][j]);
        }
        printf("\n");
    }
    print_end_sequence();

}

void print_edge_list(int cr_num, int edge_list[2 * cr_num][2]) {

    printf("\n------------------- Edge List ------------------\n");
    for(int i = 0; i < 2 * cr_num; i++) {
        printf("{%d, %d}\n", edge_list[i][0], edge_list[i][1]);
    }
    print_end_sequence();

}

void print_four_edge_subset_list(int cr_num, int four_edge_subset_list[four_edge_subsets_count[cr_num]][4][2]) {

    printf("\n------------- Four Edge Subset List ------------\n");
    for(int i = 0; i < four_edge_subsets_count[cr_num]; i++) {
        printf("Four Edges: ");
        printf("{%d, %d}, ", four_edge_subset_list[i][0][0], four_edge_subset_list[i][0][1]);
        printf("{%d, %d}, ", four_edge_subset_list[i][1][0], four_edge_subset_list[i][1][1]);
        printf("{%d, %d}, ", four_edge_subset_list[i][2][0], four_edge_subset_list[i][2][1]);
        printf("{%d, %d}\n", four_edge_subset_list[i][3][0], four_edge_subset_list[i][3][1]);
    }
    print_end_sequence();

}

void print_pd_tangle(struct pd_tangle tangle) {

    printf("\n------------------ Tangle Info -----------------\n");

    printf("Number of Crossings: %d\n", tangle.cr_num);

    printf("{");
    int* crossings = (int*) tangle.crossings;
    for(int i = 0; i < tangle.cr_num; i++) {
        if(i == tangle.cr_num - 1) {
            printf("%d}\n", *crossings);
        } else {
            printf("%d, ", *crossings);
        }
        crossings++;
    }
    print_end_sequence();

}