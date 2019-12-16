#include "utilities.h"
#include "print_structures.h"

// print comment length >
// ------------------------------------------------
//

void print_end_sequence() {
    printf("------------------------------------------------\n");
}

void print_pd_code(int cr_num, int pd_code[cr_num][4]) {

    printf("\n------------------- PD Code --------------------\n");

    for(int i = 0; i < cr_num; i++) {
        printf("{%d, %d, %d, %d} ",
               pd_code[i][0],
               pd_code[i][1],
               pd_code[i][2],
               pd_code[i][3]
        );
    }
    printf("\n");
    print_end_sequence();

}

void print_crossing(int pd_code[][4], int crossing_index) {

    printf("\n------------------- Crossing -------------------\n");

    printf("Crossing: {%d, %d, %d, %d}\n",
           pd_code[crossing_index][0],
           pd_code[crossing_index][1],
           pd_code[crossing_index][2],
           pd_code[crossing_index][3]
    );

    print_end_sequence();
}

void print_crossing_array(int crossing[4]) {
    printf("\n------------------- Crossing -------------------\n");

    printf("Crossing: {%d, %d, %d, %d}\n",
           crossing[0],
           crossing[1],
           crossing[2],
           crossing[3]
    );

    print_end_sequence();
}

void print_array(int arr[], int n) {

    printf("\n--------------------- Array --------------------\n");
    for(int i = 0; i < n; i++) {
        printf("%d : ", arr[i]);
    }
    printf("\n");
    print_end_sequence();

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

void print_pd_flype(int cr_num, int pd_code[cr_num][4], struct pd_flype flype) {

    printf("\n------------------ Flype Info ------------------\n");

    printf("Crossing: No: %d -- {%d, %d, %d, %d}\n",
           flype.crossing,
           pd_code[flype.crossing][0],
           pd_code[flype.crossing][1],
           pd_code[flype.crossing][2],
           pd_code[flype.crossing][3]
    );

    int* crossings = (int*) flype.tangle.crossings;
    for(int i = 0; i < flype.tangle.cr_num; i++) {
        print_crossing(pd_code, *crossings);
        crossings++;
    }

    print_end_sequence();

}