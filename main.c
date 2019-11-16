#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#include "src/utilities.h"
#include "src/queue.h"

int main() {

    pd_stor_t *flype_circuit;
    FILE *infile, *outfile;
    pd_code_t *next_pd_code;
    pd_crossing_t *crossing;

    // NOTE: the code starts not working for knots of 16 crossings
    infile = fopen("knot_txt_files/knot_3_1.txt", "r");
    next_pd_code = pd_read_KnotTheory(infile);

    struct PD_code* pd_code = pd_code_t_to_pd_code(next_pd_code);
    print_PD_code(pd_code);

//    // get the adjacency matrix for the graph corresponding to the knot given by the pd code
//    int adjacency_matrix[next_pd_code->ncross][next_pd_code->ncross];
//    memset(adjacency_matrix, 0, sizeof(adjacency_matrix)); // init all values to 0
//    generate_adjacency_matrix_from_pd(cr_num, pd_code, adjacency_matrix);
//
//    // get the edge list of the graph from the adjacency matrix
//    int edge_list[2 * cr_num][2];
//    get_edge_list_from_adjacency_matrix(cr_num, edge_list, adjacency_matrix);
//
//    pd_code_t *new_pd_code = pd_copy(next_pd_code);
//    int_array_to_pd_code_t(cr_num, pd_code, new_pd_code);
//    crossing = new_pd_code->cross;
//
//    int four_edge_subset_list[four_edge_subsets_count[cr_num]][4][2];
//    get_four_edge_subsets_from_edge_list(cr_num, edge_list, four_edge_subset_list);
//
//    int tangle_list[2 * four_edge_subsets_count[cr_num]][cr_num];
//    memset(tangle_list, -1, sizeof(tangle_list)); // prepare the list
//    get_all_tangles_from_pd_code(cr_num, pd_code, tangle_list);
//
//    int num_tangles = num_non_trivial_tangles(cr_num, pd_code);
//    int non_trivial_tangle_list[num_tangles][cr_num];
//    memset(non_trivial_tangle_list, -1, sizeof(non_trivial_tangle_list));
//
//    get_non_trivial_tangles_from_pd_code(cr_num, pd_code, non_trivial_tangle_list);

//    printf("Flype Count: %d\n", get_num_flypes(cr_num, pd_code));

//    flype_t flypes[2 * num_tangles];
//    get_all_flypes_from_pd_code(cr_num, pd_code, flypes);

//    for(int i = 0; i < get_num_flypes(cr_num, pd_code); i++) {
//        if(i == 2) print_flype_info(flypes[i]);
//        printf("Is parallel: %s\n", is_flype_parallel(flypes[i]) ? "true" : "false");
//    }

    fclose(infile);

    return 0;

}