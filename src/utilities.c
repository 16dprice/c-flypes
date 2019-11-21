#include <stdint.h>

#include "utilities.h"
#include "queue.h"
//#include "plcTopology.h"

// the first three numbers are 0 so that the array can just be accessed by using the number of crossings
const int four_edge_subsets_count[] = {
        0, 0, 0, // 0, 1, and 2 crossings have no meaning
        15, 70, 210, 495, 1001, 1820, 3060, 4845, 7315, 10626, 14950, 20475, 27405, 35960
};

void pd_code_t_to_int_array(int cr_num, pd_code_t *pd_code, int pd_code_arr[][4]) {

    pd_crossing_t *crossing = pd_code->cross;
    pd_idx_t incoming_under_edgepos, outgoing_under_edgepos;
    pd_idx_t incoming_over_edgepos, outgoing_over_edgepos;

    for(int i = 0; i < pd_code->ncross; i++) {
        pd_understrand_pos(pd_code, (pd_idx_t) i, &incoming_under_edgepos, &outgoing_under_edgepos);

        pd_code_arr[i][0] = crossing->edge[incoming_under_edgepos] + 1;
        pd_code_arr[i][2] = crossing->edge[outgoing_under_edgepos] + 1;

        pd_overstrand_pos(pd_code, (pd_idx_t) i, &incoming_over_edgepos, &outgoing_over_edgepos);

        if(crossing->sign == PD_POS_ORIENTATION) {
            pd_code_arr[i][1] = crossing->edge[outgoing_over_edgepos] + 1;
            pd_code_arr[i][3] = crossing->edge[incoming_over_edgepos] + 1;
        } else {
            pd_code_arr[i][3] = crossing->edge[outgoing_over_edgepos] + 1;
            pd_code_arr[i][1] = crossing->edge[incoming_over_edgepos] + 1;
        }

        crossing++;
    }

}

void int_array_to_pd_code_t(int cr_num, int pd_code_arr[][4], pd_code_t *pd_code) {
    // gonna want to use pd_regenerate_crossings(pd_code_t *pd)

    pd_crossing_t *crossing = pd_code->cross;

    for(int i = 0; i < cr_num; i++) {
        for(int j = 0; j < 4; j++) {
            crossing->edge[j] = (pd_idx_t) pd_code_arr[i][j] - 1;
        }
        crossing++;
    }

    pd_regenerate_crossings(pd_code);

}

void generate_adjacency_matrix_from_pd(int cr_num, int pd_code[cr_num][4], int adjacency_matrix[cr_num][cr_num]) {

    int i, j;

    for(int k = 1; k <= 2 * cr_num; k++) {

        i = 0;

        while (pd_code[i][0] != k && pd_code[i][1] != k && pd_code[i][2] != k && pd_code[i][3] != k) {
            i++;
        }

        j = i + 1;
        while (pd_code[j][0] != k && pd_code[j][1] != k && pd_code[j][2] != k && pd_code[j][3] != k) {
            j++;
        }

        adjacency_matrix[i][j] = adjacency_matrix[i][j] + 1;
        adjacency_matrix[j][i] = adjacency_matrix[j][i] + 1;

//        printf("for k = %d : i is %d : j is %d\n", k, i, j);

    }

}

void get_edge_list_from_adjacency_matrix(int cr_num, int edge_list[2 * cr_num][2], int adjacency_matrix[cr_num][cr_num]) {

    int edge_count = -1;

    for(int i = 0; i < cr_num; i++) {
        for(int j = i; j < cr_num; j++) {

            for(int k = 0; k < adjacency_matrix[i][j]; k++) {
                edge_count++;
                edge_list[edge_count][0] = i;
                edge_list[edge_count][1] = j;
            }

        }
    }

}

void get_four_edge_subsets_from_edge_list(int cr_num, int edge_list[2 * cr_num][2], int four_edge_subset_list[][4][2]) {

    int subset_count = -1;
    int i, j, k, l;

    for(i = 0; i < 2 * cr_num - 3; i++) {
        for(j = i + 1; j < 2 * cr_num - 2; j++) {
            for(k = j + 1; k < 2 * cr_num - 1; k++) {
                for(l = k + 1; l < 2 * cr_num; l++) {
                    subset_count++;

                    four_edge_subset_list[subset_count][0][0] = edge_list[i][0];
                    four_edge_subset_list[subset_count][0][1] = edge_list[i][1];

                    four_edge_subset_list[subset_count][1][0] = edge_list[j][0];
                    four_edge_subset_list[subset_count][1][1] = edge_list[j][1];

                    four_edge_subset_list[subset_count][2][0] = edge_list[k][0];
                    four_edge_subset_list[subset_count][2][1] = edge_list[k][1];

                    four_edge_subset_list[subset_count][3][0] = edge_list[l][0];
                    four_edge_subset_list[subset_count][3][1] = edge_list[l][1];
                }
            }
        }
    }

}

void remove_four_edges_from_adjacency_matrix(int cr_num, int four_edge_subset[4][2], int adjacency_matrix[cr_num][cr_num], int new_adj_matrix[cr_num][cr_num]) {

    for(int i = 0; i < cr_num; i++) {
        for(int j = 0; j < cr_num; j++) {
            new_adj_matrix[i][j] = adjacency_matrix[i][j];
        }
    }

    int vertex1, vertex2;
    for(int i = 0; i < 4; i++) {
        vertex1 = four_edge_subset[i][0];
        vertex2 = four_edge_subset[i][1];

        new_adj_matrix[vertex1][vertex2] = new_adj_matrix[vertex1][vertex2] - 1;
        new_adj_matrix[vertex2][vertex1] = new_adj_matrix[vertex2][vertex1] - 1;
    }

}

bool is_connected_after_removal_of_edges(int cr_num, int four_edge_subset[4][2], int adjacency_matrix[cr_num][cr_num]) {

    int adjacency_matrix_copy[cr_num][cr_num];
    remove_four_edges_from_adjacency_matrix(cr_num, four_edge_subset, adjacency_matrix, adjacency_matrix_copy);

    return is_connected(cr_num, adjacency_matrix_copy);

}

// this function assumes that the four edge subset will disconnect the graph
struct pd_tangle* get_tangles_from_four_edge_subset(int cr_num, int pd_code[cr_num][4], int four_edge_subset[4][2]) {

    int adjacency_matrix[cr_num][cr_num];
    memset(adjacency_matrix, 0, sizeof(adjacency_matrix));
    generate_adjacency_matrix_from_pd(cr_num, pd_code, adjacency_matrix);

    int new_adjacency_matrix[cr_num][cr_num];
    remove_four_edges_from_adjacency_matrix(cr_num, four_edge_subset, adjacency_matrix, new_adjacency_matrix);

    struct pd_tangle tangle1;
    struct pd_tangle tangle2;

    tangle1.cr_num = 0;
    tangle2.cr_num = 0;

    int component_designation[cr_num];

    for(int i = 0; i < cr_num; i++) {
        if(is_in_same_component(cr_num, new_adjacency_matrix, 0, i)) {
            component_designation[i] = IS_CONNECTED_TO_0;
            tangle1.cr_num++;
        } else {
            component_designation[i] = NOT_CONNECTED_TO_0;
            tangle2.cr_num++;
        }
    }

    int tangle_1_crossing_count = -1;
    int tangle_2_crossing_count = -1;

    tangle1.crossings = malloc(tangle1.cr_num * sizeof(int));
    tangle2.crossings = malloc(tangle2.cr_num * sizeof(int));

    for(int i = 0; i < cr_num; i++) {
        if(component_designation[i] == IS_CONNECTED_TO_0) {
            tangle1.crossings[++tangle_1_crossing_count] = i;
        } else {
            tangle2.crossings[++tangle_2_crossing_count] = i;
        }
    }

    struct pd_tangle* tangles;
    tangles = malloc(sizeof(tangle1) + sizeof(tangle2));

    tangles[0] = tangle1;
    tangles[1] = tangle2;

    return tangles;

}

// each index of tangle list will be a list of numbers indicating which indices of pd_code make up the tangle
// all other values should be -1
void get_all_tangles_from_pd_code(int cr_num, int pd_code[cr_num][4], int tangle_list[2 * four_edge_subsets_count[cr_num]][cr_num]) {

    // the max number of tangles that are possible is 2 * max number of four edge subsets
    // because each four edge subset could potentially split the knot into two tangles

    // in reality, there are much fewer tangles than that, but this is an upper limit

    int adjacency_matrix[cr_num][cr_num];
    memset(adjacency_matrix, 0, sizeof(adjacency_matrix));
    generate_adjacency_matrix_from_pd(cr_num, pd_code, adjacency_matrix);

    int edge_list[2 * cr_num][2];
    get_edge_list_from_adjacency_matrix(cr_num, edge_list, adjacency_matrix);

    int four_edge_subset_list[four_edge_subsets_count[cr_num]][4][2];
    get_four_edge_subsets_from_edge_list(cr_num, edge_list, four_edge_subset_list);

    int tangle_count = -1;
    for(int i = 0; i < four_edge_subsets_count[cr_num]; i++) {

        if(!is_connected_after_removal_of_edges(cr_num, four_edge_subset_list[i], adjacency_matrix)) {
            int two_tangle_list[2][cr_num];
            memset(two_tangle_list, -1, sizeof(two_tangle_list));
            get_tangle_from_four_edge_subset(cr_num, pd_code, four_edge_subset_list[i], two_tangle_list);

            tangle_count++;

            for(int j = 0; j < cr_num; j++) {
                tangle_list[tangle_count][j] = two_tangle_list[0][j];
            }

            tangle_count++;

            for(int j = 0; j < cr_num; j++) {
                tangle_list[tangle_count][j] = two_tangle_list[1][j];
            }
        }

    }

}

bool is_tangle_trivial(int cr_num, const int *tangle) {
    int minus_1_count = 0;
    for(int i = 0; i < cr_num; i++) {
        if(tangle[i] == -1) {
            minus_1_count++;
        }
    }
    if(minus_1_count == 0 || minus_1_count == 1 || minus_1_count == (cr_num - 1) || minus_1_count == cr_num) {
        return true;
    }
    return false;
}

int num_non_trivial_tangles(int cr_num, int pd_code[cr_num][4]) {

    int tangle_list[2 * four_edge_subsets_count[cr_num]][cr_num];
    memset(tangle_list, -1, sizeof(tangle_list));
    get_all_tangles_from_pd_code(cr_num, pd_code, tangle_list);

    int tangle_count = 0;
    for(int i = 0; i < 2 * four_edge_subsets_count[cr_num]; i++) {
        if(!is_tangle_trivial(cr_num, tangle_list[i])) {
            tangle_count++;
        }
    }

    return tangle_count;

}

void get_non_trivial_tangles_from_pd_code(int cr_num, int pd_code[cr_num][4], int non_trivial_tangle_list[][cr_num]) {

    // tangle list should have first dimension length of num_tangles(cr_num, pd_code) and be initialized to all -1's
    int non_trivial_tangle_count = -1;
    int tangle_list[2 * four_edge_subsets_count[cr_num]][cr_num];
    memset(tangle_list, -1, sizeof(tangle_list));

    get_all_tangles_from_pd_code(cr_num, pd_code, tangle_list);

    for(int i = 0; i < 2 * four_edge_subsets_count[cr_num]; i++) {
        if(!is_tangle_trivial(cr_num, tangle_list[i])) {
            non_trivial_tangle_count++;
            for(int j = 0; j < cr_num; j++) {
                non_trivial_tangle_list[non_trivial_tangle_count][j] = tangle_list[i][j];
            }
        }
    }

}

int get_tangle_size(int cr_num, int tangle[cr_num]) {
    int size = 0;
    for(int i = 0; i < cr_num; i++) {
        if(tangle[i] == -1) return size;
        size++;
    }
    return size; // should technically never get here
}

bool in_array(int val, int n, int arr[n]) {
    for(int i = 0; i < n; i++) {
        if(arr[i] == val) return true;
    }
    return false;
}

bool is_crossing_in_tangle(int cr_num, int crossing[4], int pd_code[cr_num][4], int tangle[cr_num]) {

    for(int i = 0; i < cr_num; i++) {
        if(tangle[i] == -1) return false;

        if(pd_code[i][0] == crossing[0] && pd_code[i][1] == crossing[1] && pd_code[i][2] == crossing[2] && pd_code[i][3] == crossing[3]) {
            return true;
        }
    }
    return false;

}

bool is_crossing_flypable(int cr_num, int crossing[4], int pd_code[cr_num][4], int tangle[cr_num]) {
    int tangle_size = get_tangle_size(cr_num, tangle);

    // load up all the edge values that are in the tangle
    int tangle_edges[4 * tangle_size];
    for(int i = 0; i < tangle_size; i++) {
        tangle_edges[4 * i    ] = pd_code[tangle[i]][0];
        tangle_edges[4 * i + 1] = pd_code[tangle[i]][1];
        tangle_edges[4 * i + 2] = pd_code[tangle[i]][2];
        tangle_edges[4 * i + 3] = pd_code[tangle[i]][3];
    }

    int edges_connected_to_tangle = 0;
    for(int i = 0; i < 4; i++) {
        if(in_array(crossing[i], 4 * tangle_size, tangle_edges)) {
            edges_connected_to_tangle++;
        }
    }

    return edges_connected_to_tangle == 2;

}

void get_flypes_from_tangle(int cr_num, int pd_code[cr_num][4], int tangle[cr_num], int flypable_crossings[2]) {

    int flypes_found = -1;

    // for each crossing, if it's not in the tangle, check to see if it shares
    for(int i = 0; i < cr_num; i++) {
        if(!in_array(i, cr_num, tangle)) {
            if(is_crossing_flypable(cr_num, pd_code[i], pd_code, tangle)) {
                flypable_crossings[++flypes_found] = i;
            }
        }
    }

    if(flypes_found > 1) {
        printf("Error in get_flypes_from_tangle. Too many flypes found!\n");
    }

}

int get_num_flypes(int cr_num, int pd_code[cr_num][4]) {

    int num_tangles = num_non_trivial_tangles(cr_num, pd_code);
    int non_trivial_tangle_list[num_tangles][cr_num];
    memset(non_trivial_tangle_list, -1, sizeof(non_trivial_tangle_list));

    get_non_trivial_tangles_from_pd_code(cr_num, pd_code, non_trivial_tangle_list);

    int num_flypes_found = 0;
    for(int i = 0; i < num_tangles; i++) {

        int flypable_crossings[2];
        memset(flypable_crossings, -1, sizeof(flypable_crossings));

        get_flypes_from_tangle(cr_num, pd_code, non_trivial_tangle_list[i], flypable_crossings);

        if(flypable_crossings[0] != -1) {
            num_flypes_found++;
        }

        if(flypable_crossings[1] != -1) {
            num_flypes_found++;
        }

    }

    return num_flypes_found;
}

//void get_all_flypes_from_pd_code(int cr_num, int pd_code[cr_num][4], flype_t flypes[]) {
//
//    int num_tangles = num_non_trivial_tangles(cr_num, pd_code);
//    int non_trivial_tangle_list[num_tangles][cr_num];
//    memset(non_trivial_tangle_list, -1, sizeof(non_trivial_tangle_list));
//
//    get_non_trivial_tangles_from_pd_code(cr_num, pd_code, non_trivial_tangle_list);
//
//    int num_flypes_found = -1;
//    for(int i = 0; i < num_tangles; i++) {
//
//        int flypable_crossings[2];
//        memset(flypable_crossings, -1, sizeof(flypable_crossings));
//
//        get_flypes_from_tangle(cr_num, pd_code, non_trivial_tangle_list[i], flypable_crossings);
//
//        if(flypable_crossings[0] != -1) {
//            flype_t flype;
//
//            flype.cr_num = cr_num;
//            flype.pd_code = *pd_code;
//            flype.crossing = flypable_crossings[0];
//            flype.tangle_length = get_tangle_size(cr_num, non_trivial_tangle_list[i]);
//            flype.tangle = non_trivial_tangle_list[i];
//
//            flypes[++num_flypes_found] = flype;
//        }
//
//        if(flypable_crossings[1] != -1) {
//            flype_t flype;
//
//            flype.cr_num = cr_num;
//            flype.pd_code = *pd_code;
//            flype.crossing = flypable_crossings[1];
//            flype.tangle_length = get_tangle_size(cr_num, non_trivial_tangle_list[i]);
//            flype.tangle = non_trivial_tangle_list[i];
//
//            flypes[++num_flypes_found] = flype;
//        }
//
//    }
//
//}
//
//void get_instrands(flype_t flype, int instrands[2]) {
//
//    // load up all the edge values that are in the tangle
//    int tangle_edges[4 * flype.tangle_length];
//    for(int i = 0; i < flype.tangle_length; i++) {
//        tangle_edges[4 * i    ] = flype.pd_code[4 * flype.tangle[i]    ];
//        tangle_edges[4 * i + 1] = flype.pd_code[4 * flype.tangle[i] + 1];
//        tangle_edges[4 * i + 2] = flype.pd_code[4 * flype.tangle[i] + 2];
//        tangle_edges[4 * i + 3] = flype.pd_code[4 * flype.tangle[i] + 3];
//    }
//
//    int instrands_found = -1;
//    for(int i = 0; i < 4; i++) {
//        if(in_array(flype.pd_code[4 * flype.crossing + i], 4 * flype.tangle_length, tangle_edges)) {
//            instrands[++instrands_found] = flype.pd_code[4 * flype.crossing + i];
//        }
//    }
//
//}
//
//int get_next_strand(int crossing[4], int strand) {
//
//    if(crossing[0] == strand) return crossing[2];
//    if(crossing[2] == strand) return crossing[0];
//
//    if(crossing[1] == strand) return crossing[3];
//    if(crossing[3] == strand) return crossing[1];
//
//}
//
//bool is_flype_parallel(flype_t flype) {
//
//    int instrands[2];
//    get_instrands(flype, instrands);
//
//    int crossing_array[4];
//    for(int i = 0; i < 4; i++) {
//        crossing_array[i] = flype.pd_code[4 * flype.crossing + i];
//    }
//
//    int diff1 = instrands[0] - get_next_strand(crossing_array, instrands[0]);
//    if(diff1 >  1) diff1 = -1;
//    if(diff1 < -1) diff1 =  1;
//
//    int diff2 = instrands[1] - get_next_strand(crossing_array, instrands[1]);
//    if(diff2 >  1) diff2 = -1;
//    if(diff2 < -1) diff2 =  1;
//
//    return diff1 == diff2;
//
//}
