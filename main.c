#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#include <time.h>

#include "plcTopology.h"
#include "src/utilities.h"
#include "src/queue.h"
#include "src/print_structures.h"

int main() {

    pd_stor_t *flype_circuit;
    FILE *infile, *outfile;
    pd_code_t *next_pd_code;
    pd_crossing_t *crossing;

    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }

    // NOTE: the code starts not working for knots of 16 crossings
    infile = fopen("/home/dj/CLionProjects/c-flypes/knot_txt_files/knot_6_3.txt", "r");
    next_pd_code = pd_read_KnotTheory(infile);

    int cr_num = next_pd_code->ncross;

    int pd_code[cr_num][4];
    pd_code_t_to_int_array(cr_num, next_pd_code, pd_code);

    // get the adjacency matrix for the graph corresponding to the knot given by the pd code
    int adjacency_matrix[next_pd_code->ncross][next_pd_code->ncross];
    memset(adjacency_matrix, 0, sizeof(adjacency_matrix)); // init all values to 0
    generate_adjacency_matrix_from_pd(cr_num, pd_code, adjacency_matrix);

//    print_adjacency_matrix(cr_num, adjacency_matrix);

    // get the edge list of the graph from the adjacency matrix
    int edge_list[2 * cr_num][2];
    get_edge_list_from_adjacency_matrix(cr_num, edge_list, adjacency_matrix);

//    print_edge_list(cr_num, edge_list);

    int four_edge_subset_list[four_edge_subsets_count[cr_num]][4][2];
    get_four_edge_subsets_from_edge_list(cr_num, edge_list, four_edge_subset_list);

//    print_four_edge_subset_list(cr_num, four_edge_subset_list);

    struct pd_tangle_list tangles = get_all_tangles_from_pd_code(cr_num, pd_code);

//    for(int i = 0; i < tangles.num_tangles; i++) {
//        print_pd_tangle(tangles.tangles[i]);
//    }

    struct pd_tangle_list non_trivial_tangles = get_non_trivial_tangles_from_pd_code(cr_num, pd_code);

//    int* instrands = get_instrands(cr_num, pd_code, 1, non_trivial_tangles.tangles[0]);
//    if(instrands != NULL) {
//        printf("Instrands: %d, %d\n", instrands[0], instrands[1]);
//    }

    int* outstrands = get_outstrands(cr_num, pd_code, 1, non_trivial_tangles.tangles[0]);
//    if(outstrands != NULL) {
//        printf("Outstrands: %d, %d\n", outstrands[0], outstrands[1]);
//    }

    struct pd_flype_list all_flypes = get_all_flypes_from_pd_code(cr_num, pd_code);

//    printf("Flype Count: %d\n", all_flypes.num_flypes);

//    for(int i = 0; i < all_flypes.num_flypes; i++) {
//        printf("%s ", is_flype_parallel(cr_num, pd_code, all_flypes.flypes[i]) ? "T" : "F");
//    }

    pd_stor_t *flyped_codes;
    pd_code_t *new_pd_code_t;

    flyped_codes = pd_new_pdstor();
//    new_pd_code_t = pd_code_new((pd_idx_t) cr_num + 2);
    new_pd_code_t = pd_copy(next_pd_code);

    int new_pd_code[cr_num][4];

    pd_addto_pdstor(flyped_codes, next_pd_code, DIAGRAM_ISOTOPY);
    for(int i = 0; i < all_flypes.num_flypes; i++) {
        if(!is_flype_parallel(cr_num, pd_code, all_flypes.flypes[i])) {
            anti_parallel_flype(cr_num, pd_code, all_flypes.flypes[i], new_pd_code);
            int_array_to_pd_code_t(cr_num, new_pd_code, new_pd_code_t);
            pd_addto_pdstor(flyped_codes, new_pd_code_t, DIAGRAM_ISOTOPY);
//            print_pd_code(cr_num, new_pd_code);
        } else {
            parallel_flype(cr_num, pd_code, all_flypes.flypes[i], new_pd_code);
            int_array_to_pd_code_t(cr_num, new_pd_code, new_pd_code_t);
            pd_addto_pdstor(flyped_codes, new_pd_code_t, DIAGRAM_ISOTOPY);
//            print_pd_code(cr_num, new_pd_code);
        }
    }

    outfile = fopen("/home/dj/CLionProjects/c-flypes/flype_output/knot_6_3.txt", "w");
    for(next_pd_code = pd_stor_firstelt(flyped_codes); next_pd_code != NULL; next_pd_code = pd_stor_nextelt(flyped_codes)) {
        pd_write_KnotTheory(outfile, next_pd_code);
        free(next_pd_code);
    }
//    fclose(infile);
//    fclose(outfile);
//
//    flyped_codes = pd_new_pdstor();
//    infile = fopen("/home/dj/CLionProjects/c-flypes/flype_output/knot_6_1.txt", "r");
//    for(int i = 0; i < 7; i++) {
//        next_pd_code = pd_read_KnotTheory(infile);
//        pd_addto_pdstor(flyped_codes, next_pd_code, DIAGRAM_ISOTOPY);
//        free(next_pd_code);
//    }
//
//    outfile = fopen("/home/dj/CLionProjects/c-flypes/flype_output/knot_6_1.txt", "w");
//    for(next_pd_code = pd_stor_firstelt(flyped_codes); next_pd_code != NULL; next_pd_code = pd_stor_nextelt(flyped_codes)) {
//        pd_write_KnotTheory(outfile, next_pd_code);
//        free(next_pd_code);
//    }


//    anti_parallel_flype(cr_num, pd_code, all_flypes.flypes[all_flypes.num_flypes - 1], new_pd_code);
//    print_pd_flype(cr_num, pd_code, all_flypes.flypes[all_flypes.num_flypes - 1]);
//    print_pd_code(cr_num, new_pd_code);

    fclose(infile);
    fclose(outfile);

    pd_free_pdstor(&flyped_codes);

    return 0;

}
