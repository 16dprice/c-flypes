#include <stdint.h>
#include <math.h>

#include "print_structures.h"
#include "utilities.h"
#include "queue.h"

// the first three numbers are 0 so that the array can just be accessed by using the number of crossings
const int four_edge_subsets_count[] = {
        0, 0, 0, // 0, 1, and 2 crossings have no meaning
        15, 70, 210, 495, 1001, 1820, 3060, 4845, 7315, 10626, 14950, 20475, 27405, 35960
};

bool in_array(int val, int n, int arr[n]) {
    for(int i = 0; i < n; i++) {
        if(arr[i] == val) return true;
    }
    return false;
}

int nelts_of_pd_stor(pd_stor_t* pdstor) {

    pd_code_t *next_pd_code;
    int count = 0;

    for(next_pd_code = pd_stor_firstelt(pdstor); next_pd_code != NULL; next_pd_code = pd_stor_nextelt(pdstor)) {
        count++;
        free(next_pd_code);
    }

    return count;
}

bool is_crossing_positive(int crossing[4]) {
    return (crossing[1] - crossing[3] == 1 || crossing[1] - crossing[3] < -1);
}

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
            pd_code->cross->edge[j] = (pd_idx_t) pd_code_arr[i][j] - 1;
        }
        pd_code->cross->sign = is_crossing_positive(pd_code_arr[i]) ? PD_POS_ORIENTATION : PD_NEG_ORIENTATION;
        pd_code->cross++;
    }
    pd_code->cross = crossing;

//    pd_regenerate_crossings(pd_code);
//    pd_regenerate_faces(pd_code);
    pd_regenerate_edges(pd_code);
//    pd_regenerate_comps(pd_code);
//    pd_regenerate_hash(pd_code);
    pd_regenerate(pd_code);

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
struct pd_tangle_list get_tangles_from_four_edge_subset(int cr_num, int pd_code[cr_num][4], int four_edge_subset[4][2]) {

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

    struct pd_tangle_list tangles;
    tangles.num_tangles = 2;

    tangles.tangles = malloc(sizeof(tangle1) + sizeof(tangle2));

    tangles.tangles[0] = tangle1;
    tangles.tangles[1] = tangle2;

    return tangles;

}

// each index of tangle list will be a list of numbers indicating which indices of pd_code make up the tangle
// all other values should be -1
struct pd_tangle_list get_all_tangles_from_pd_code(int cr_num, int pd_code[cr_num][4]) {

    int adjacency_matrix[cr_num][cr_num];
    memset(adjacency_matrix, 0, sizeof(adjacency_matrix));
    generate_adjacency_matrix_from_pd(cr_num, pd_code, adjacency_matrix);

    int edge_list[2 * cr_num][2];
    get_edge_list_from_adjacency_matrix(cr_num, edge_list, adjacency_matrix);

    int four_edge_subset_list[four_edge_subsets_count[cr_num]][4][2];
    get_four_edge_subsets_from_edge_list(cr_num, edge_list, four_edge_subset_list);

    // the max number of tangles that are possible is 2 * max number of four edge subsets
    // because each four edge subset could potentially split the knot into two tangles
    // in reality, there are much fewer tangles than that, but this is an upper limit
    struct pd_tangle_list tangles;
    tangles.tangles = malloc(2 * four_edge_subsets_count[cr_num] * sizeof(struct pd_tangle));

    int tangle_count = -1;
    for(int i = 0; i < four_edge_subsets_count[cr_num]; i++) {
        if(!is_connected_after_removal_of_edges(cr_num, four_edge_subset_list[i], adjacency_matrix)) {
            struct pd_tangle_list new_tangles = get_tangles_from_four_edge_subset(cr_num, pd_code, four_edge_subset_list[i]);
            tangles.tangles[++tangle_count] = new_tangles.tangles[0];
            tangles.tangles[++tangle_count] = new_tangles.tangles[1];
        }
    }
    tangles.num_tangles = tangle_count + 1;

    return tangles;

}

bool is_tangle_trivial(int cr_num, struct pd_tangle tangle) {
    return tangle.cr_num == cr_num - 1 || tangle.cr_num == 1;
}

int num_non_trivial_tangles_from_tangle_list(int cr_num, struct pd_tangle_list tangles) {
    int trivial_count = 0;

    for(int i = 0; i < tangles.num_tangles; i++) {
        if(is_tangle_trivial(cr_num, tangles.tangles[i])) trivial_count++;
    }

    return tangles.num_tangles - trivial_count;
}

int num_non_trivial_tangles(int cr_num, int pd_code[cr_num][4]) {
    struct pd_tangle_list tangles = get_all_tangles_from_pd_code(cr_num, pd_code);
    return num_non_trivial_tangles_from_tangle_list(cr_num, tangles);
}

struct pd_tangle_list get_non_trivial_tangles_from_pd_code(int cr_num, int pd_code[cr_num][4]) {

    struct pd_tangle_list all_tangles = get_all_tangles_from_pd_code(cr_num, pd_code);
    struct pd_tangle_list non_trivial_tangles;

    non_trivial_tangles.num_tangles = num_non_trivial_tangles_from_tangle_list(cr_num, all_tangles);
    non_trivial_tangles.tangles = malloc(non_trivial_tangles.num_tangles * sizeof(struct pd_tangle));

    int non_trivial_tangle_count = -1;
    for(int i = 0; i < all_tangles.num_tangles; i++) {
        if(!is_tangle_trivial(cr_num, all_tangles.tangles[i])) {
            non_trivial_tangles.tangles[++non_trivial_tangle_count] = all_tangles.tangles[i];
        }
    }

    return non_trivial_tangles;

}

// returns pointer to 4 ints representing the 4 strands on the outside of the tangle
int* get_strands(int cr_num, int pd_code[cr_num][4], struct pd_tangle tangle) {

    int *strands = malloc(4 * sizeof(int));

    // basically flattening out the tangle
    int all_tangle_strands[4 * tangle.cr_num];
    for(int i = 0; i < tangle.cr_num; i++) {
        all_tangle_strands[4 * i + 0] = pd_code[*tangle.crossings][0];
        all_tangle_strands[4 * i + 1] = pd_code[*tangle.crossings][1];
        all_tangle_strands[4 * i + 2] = pd_code[*tangle.crossings][2];
        all_tangle_strands[4 * i + 3] = pd_code[*tangle.crossings][3];

        tangle.crossings++;
    }

    // if a strand is unique, add it to the strands list
    // there should be exactly 4
    int strand, same_strand_count;
    int strands_found = -1;
    for(int i = 0; i < 4 * tangle.cr_num; i++) {
        strand = all_tangle_strands[i];
        same_strand_count = 0;

        for(int j = 0; j < 4 * tangle.cr_num; j++) {
            if(strand == all_tangle_strands[j] && i != j) {
                same_strand_count++;
            }
        }

        if(same_strand_count == 0) {
            strands[++strands_found] = strand;
        }
    }

    return strands;

}

// returns NULL if the crossing is not connected to the tangle by 2 strands
// otherwise, returns pointer to 2 ints representing the 2 strands connected to the crossing
int* get_instrands(int cr_num, int pd_code[cr_num][4], int crossing_index, struct pd_tangle tangle) {

    // short circuit if crossing is in tangle
    if(in_array(crossing_index, tangle.cr_num, tangle.crossings)) {
        return NULL;
    }

    int* strands = get_strands(cr_num, pd_code, tangle);
    int* instrands = malloc(2 * sizeof(int));

    int instrand_count = 0;
    for(int i = 0; i < 4; i++) {
        if(in_array(strands[i], 4, (int*) pd_code[crossing_index])) {
            if(instrand_count == 2) {
                return NULL;
            }
            instrands[instrand_count++] = strands[i];
        }
    }

    if(instrand_count != 2) {
        return NULL;
    } else {
        return instrands;
    }

}

// returns NULL if the crossing is not connected to the tangle by 2 strands
// otherwise, returns pointer to 2 ints representing the 2 strands NOT connected to the crossing
int* get_outstrands(int cr_num, int pd_code[cr_num][4], int crossing_index, struct pd_tangle tangle) {

    // short circuit if crossing is in tangle
    if(in_array(crossing_index, tangle.cr_num, tangle.crossings)) {
        return NULL;
    }

    int* strands = get_strands(cr_num, pd_code, tangle);
    int* outstrands = malloc(2 * sizeof(int));

    int outstrand_count = 0;
    for(int i = 0; i < 4; i++) {
        if(!in_array(strands[i], 4, (int*) pd_code[crossing_index])) {
            if(outstrand_count == 2) {
                return NULL;
            }
            outstrands[outstrand_count++] = strands[i];
        }
    }

    if(outstrand_count != 2) {
        return NULL;
    } else {
        return outstrands;
    }

}

struct pd_flype_list get_all_flypes_from_pd_code(int cr_num, int pd_code[cr_num][4]) {

    struct pd_tangle_list non_trivial_tangles = get_non_trivial_tangles_from_pd_code(cr_num, pd_code);

    struct pd_flype_list all_flypes;
    all_flypes.flypes = malloc(2 * non_trivial_tangles.num_tangles * sizeof(struct pd_flype));

    // for each tangle, look at each crossing
    // if the instrands for that crossing and tangle is not NULL, that means the tangle can be flyped over it
    int flypes_found = 0;
    for(int i = 0; i < non_trivial_tangles.num_tangles; i++) {
        for(int j = 0; j < cr_num; j++) {
            if(get_instrands(cr_num, pd_code, j, non_trivial_tangles.tangles[i]) != NULL) {

                struct pd_flype new_flype;

                new_flype.crossing = j;
                new_flype.tangle = non_trivial_tangles.tangles[i];

                all_flypes.flypes[flypes_found++] = new_flype;

            }
        }
    }
    all_flypes.num_flypes = flypes_found;

    return all_flypes;

}

int get_next_strand(int strand, int crossing[4]) {
    if(crossing[0] == strand) return crossing[2];
    if(crossing[1] == strand) return crossing[3];
    if(crossing[2] == strand) return crossing[0];
    if(crossing[3] == strand) return crossing[1];
}

bool is_flype_parallel(int cr_num, int pd_code[cr_num][4], struct pd_flype flype) {

    int* instrands = get_instrands(cr_num, pd_code, flype.crossing, flype.tangle);

    int diff1 = instrands[0] - get_next_strand(instrands[0], pd_code[flype.crossing]);
    if(diff1 > 1) diff1 = -1;
    if(diff1 < -1) diff1 = 1;

    int diff2 = instrands[1] - get_next_strand(instrands[1], pd_code[flype.crossing]);
    if(diff2 > 1) diff2 = -1;
    if(diff2 < -1) diff2 = 1;

    return diff1 == diff2;

}

bool is_boundary_in_code(int cr_num, int pd_code[cr_num][4], struct pd_flype flype) {

    if(in_array(1, 4, pd_code[flype.crossing]) && in_array(2 * cr_num, 4, pd_code[flype.crossing])) return true;

    int* crossings = flype.tangle.crossings;
    for(int i = 0; i < flype.tangle.cr_num; i++) {
        if(in_array(1, 4, pd_code[*crossings]) && in_array(2 * cr_num, 4, pd_code[*crossings])) return true;
        crossings++;
    }

    return false;

}

bool is_sorted(int arr[4]) {
    for(int i = 0; i < 3; i++) {
        if(arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

void sort_in_and_outstrands(int arr[4]) {

    int temp;
    while(!is_sorted(arr)) {
        for(int i = 0; i < 3; i++) {
            if(arr[i] > arr[i + 1]) {
                temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
            }
        }
    }

}

void flip_crossing(int crossing[4]) {
    int crossing_copy[] = {
            crossing[0], crossing[1], crossing[2], crossing[3]
    };

    bool is_positive = is_crossing_positive(crossing);

    if(is_positive) {
        crossing[0] = crossing_copy[3];
        crossing[1] = crossing_copy[2];
        crossing[2] = crossing_copy[1];
        crossing[3] = crossing_copy[0];
    } else {
        crossing[0] = crossing_copy[1];
        crossing[1] = crossing_copy[0];
        crossing[2] = crossing_copy[3];
        crossing[3] = crossing_copy[2];
    }

}

int ensure_strand_is_in_bounds(int strand, int cr_num) {
    int return_strand = (strand + (2 * cr_num)) % (2 * cr_num);
    if(return_strand == 0) return 2 * cr_num;
    return return_strand;
}

void copy_crossing(int crossing[4], int new_crossing[4]) {
    for(int j = 0; j < 4; j++) {
        crossing[j] = new_crossing[j];
    }
}

void anti_parallel_flype(int cr_num, int pd_code[cr_num][4], struct pd_flype flype, int new_pd_code[cr_num][4]) {

    for(int i = 0; i < cr_num; i++) {
        for(int j = 0; j < 4; j++) {
            new_pd_code[i][j] = pd_code[i][j];
        }
    }

    int count = 0; // extra safeguard
    while(is_boundary_in_code(cr_num, new_pd_code, flype) && count <= 2 * cr_num) {
        for(int i = 0; i < cr_num; i++) {
            for(int j = 0; j < 4; j++) {
                new_pd_code[i][j] = ensure_strand_is_in_bounds(new_pd_code[i][j] + 1, cr_num);
            }
        }
        count++;
    }

    int* instrands =  get_instrands (cr_num, new_pd_code, flype.crossing, flype.tangle);
    int* outstrands = get_outstrands(cr_num, new_pd_code, flype.crossing, flype.tangle);

    int a, b;
    if(instrands[0] - get_next_strand(instrands[0], new_pd_code[flype.crossing]) == 1) {
        a = instrands[0];
        b = instrands[1];
    } else {
        a = instrands[1];
        b = instrands[0];
    }

    int in_and_outstrands[] = {
            instrands[0], instrands[1], outstrands[0], outstrands[1]
    };

    sort_in_and_outstrands(in_and_outstrands);

    bool is_parity_inf;
    if((in_and_outstrands[0] == a && in_and_outstrands[1] == b) || (in_and_outstrands[2] == a && in_and_outstrands[3] == b)) {
        is_parity_inf = true;
    } else {
        is_parity_inf = false;
    }

    int c, d;
    if(is_parity_inf) {
        // is parity infinity
        if(outstrands[0] < outstrands[1]) {
            c = outstrands[0];
            d = outstrands[1];
        } else {
            c = outstrands[1];
            d = outstrands[0];
        }
    } else {
        // is NOT parity infinity
        if(abs(a - outstrands[0]) + abs(b - outstrands[1]) == 2 * flype.tangle.cr_num) {
            c = outstrands[0];
            d = outstrands[1];
        } else {
            c = outstrands[1];
            d = outstrands[0];
        }
    }

    // TODO: this might be an issue...
    bool c_is_overpass = false;
    int* tangle_crossing = flype.tangle.crossings;
    for(int i = 0; i < flype.tangle.cr_num; i++) {
        if(new_pd_code[*tangle_crossing][1] == c || new_pd_code[*tangle_crossing][3] == c) {
            c_is_overpass = true;
        }
        tangle_crossing++;
    }

    bool is_positive = is_crossing_positive(new_pd_code[flype.crossing]);


    // flip the tangle
    tangle_crossing = flype.tangle.crossings;
    for(int i = 0; i < flype.tangle.cr_num; i++) {
        flip_crossing(new_pd_code[*tangle_crossing]);
        tangle_crossing++;
    }

    int tangle_crossing_index_list[flype.tangle.cr_num + 1];
    tangle_crossing_index_list[0] = flype.crossing;

    tangle_crossing = flype.tangle.crossings;
    for(int i = 1; i <= flype.tangle.cr_num; i++) {
        tangle_crossing_index_list[i] = *tangle_crossing;
        tangle_crossing++;
    }

    if(is_parity_inf) {
        // TRUE: tangle is parity infinity
        if(a < c) {
            // TRUE: a < c
            tangle_crossing = flype.tangle.crossings;
            for(int i = 0; i < flype.tangle.cr_num; i++) {
                for(int j = 0; j < 4; j++) {
                    new_pd_code[*tangle_crossing][j] = ensure_strand_is_in_bounds(new_pd_code[*tangle_crossing][j] - 1, cr_num);
                }
                tangle_crossing++;
            }
            for(int i = 0; i < cr_num; i++) {
                // if the crossing is not in the tangle and is not the flype crossing
                if(!in_array(i, flype.tangle.cr_num + 1, tangle_crossing_index_list)) {

                    for(int j = 0; j < 4; j++) {
                        if(b < new_pd_code[i][j] && new_pd_code[i][j] <= c) {
                            new_pd_code[i][j] = ensure_strand_is_in_bounds(new_pd_code[i][j] - 2, cr_num);
                        }
                    }

                }
            }
        } else {
            // FALSE: a > c
            tangle_crossing = flype.tangle.crossings;
            for(int i = 0; i < flype.tangle.cr_num; i++) {
                for(int j = 0; j < 4; j++) {
                    new_pd_code[*tangle_crossing][j] = ensure_strand_is_in_bounds(new_pd_code[*tangle_crossing][j] + 1, cr_num);
                }
                tangle_crossing++;
            }
            for(int i = 0; i < cr_num; i++) {
                // if the crossing is not in the tangle and is not the flype crossing
                if(!in_array(i, flype.tangle.cr_num + 1, tangle_crossing_index_list)) {

                    for(int j = 0; j < 4; j++) {
                        if(d <= new_pd_code[i][j] && new_pd_code[i][j] <= a) {
                            new_pd_code[i][j] = ensure_strand_is_in_bounds(new_pd_code[i][j] + 2, cr_num);
                        }
                    }

                }
            }
        }
    } else {
        // FALSE: tangle is NOT parity infinity
        tangle_crossing = flype.tangle.crossings;
        for(int i = 0; i < flype.tangle.cr_num; i++) {
            for(int j = 0; j < 4; j++) {
                if(a <= new_pd_code[*tangle_crossing][j] && new_pd_code[*tangle_crossing][j] <= c) {
                    new_pd_code[*tangle_crossing][j] = ensure_strand_is_in_bounds(new_pd_code[*tangle_crossing][j] - 1, cr_num);
                }
                if(d <= new_pd_code[*tangle_crossing][j] && new_pd_code[*tangle_crossing][j] <= b) {
                    new_pd_code[*tangle_crossing][j] = ensure_strand_is_in_bounds(new_pd_code[*tangle_crossing][j] + 1, cr_num);
                }
            }
            tangle_crossing++;
        }
    }

    if(is_parity_inf) {
        // TRUE: tangle is parity infinity
        if(a < c) {
            // a < c
            if(is_positive) {
                // crossing was POSITIVE
                if(c_is_overpass) {
                    int new_crossing[4] = {d - 1, c - 1, d, c - 2};
                    copy_crossing(new_pd_code[flype.crossing], new_crossing);
                } else {
                    int new_crossing[4] = {c - 2, d, c - 1, d - 1};
                    copy_crossing(new_pd_code[flype.crossing], new_crossing);
                }
            } else {
                // crossing was NEGATIVE
                if(c_is_overpass) {
                    int new_crossing[4] = {d - 1, c - 2, d, c - 1};
                    copy_crossing(new_pd_code[flype.crossing], new_crossing);
                } else {
                    int new_crossing[4] = {c - 2, d - 1, c - 1, d};
                    copy_crossing(new_pd_code[flype.crossing], new_crossing);
                }
            }
        } else {
            // a > c
            if(is_positive) {
                // crossing was POSITIVE
                if(c_is_overpass) {
                    int new_crossing[4] = {d + 1, c + 1, d + 2, c};
                    copy_crossing(new_pd_code[flype.crossing], new_crossing);
                } else {
                    int new_crossing[4] = {c, d + 2, c + 1, d + 1};
                    copy_crossing(new_pd_code[flype.crossing], new_crossing);
                }
            } else {
                // crossing was NEGATIVE
                if(c_is_overpass) {
                    int new_crossing[4] = {d + 1, c, d + 2, c + 1};
                    copy_crossing(new_pd_code[flype.crossing], new_crossing);
                } else {
                    int new_crossing[4] = {c, d + 1, c + 1, d + 2};
                    copy_crossing(new_pd_code[flype.crossing], new_crossing);
                }
            }
        }
    } else {
        // FALSE: tangle is NOT parity infinity
        if(is_positive) {
            // crossing was POSITIVE
            if(c_is_overpass) {
                int new_crossing[4] = {d, c, d + 1, c - 1};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            } else {
                int new_crossing[4] = {c - 1, d + 1, c, d};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            }
        } else {
            // crossing was NEGATIVE
            if(c_is_overpass) {
                int new_crossing[4] = {d, c - 1, d + 1, c};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            } else {
                int new_crossing[4] = {c - 1, d, c, d + 1};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            }
        }
    }

    for(int j = 0; j < 4; j++) {
        new_pd_code[flype.crossing][j] = ensure_strand_is_in_bounds(new_pd_code[flype.crossing][j], cr_num);
    }

}

void parallel_flype(int cr_num, int pd_code[cr_num][4], struct pd_flype flype, int new_pd_code[cr_num][4]) {

    for(int i = 0; i < cr_num; i++) {
        for(int j = 0; j < 4; j++) {
            new_pd_code[i][j] = pd_code[i][j];
        }
    }

    int* instrands =  get_instrands (cr_num, new_pd_code, flype.crossing, flype.tangle);
    int* outstrands = get_outstrands(cr_num, new_pd_code, flype.crossing, flype.tangle);

    int a, b;
    bool reverse;
    if(instrands[0] == new_pd_code[flype.crossing][0] || instrands[1] == new_pd_code[flype.crossing][0]) {
        a = pd_code[flype.crossing][0];
        reverse = true;
    } else {
        a = pd_code[flype.crossing][2];
        reverse = false;
    }

    if(instrands[0] == new_pd_code[flype.crossing][1] || instrands[1] == new_pd_code[flype.crossing][1]) {
        b = pd_code[flype.crossing][1];
    } else {
        b = pd_code[flype.crossing][3];
    }

    bool negative = !is_crossing_positive(new_pd_code[flype.crossing]);

    // flip the tangle
    int* tangle_crossing;
    tangle_crossing = flype.tangle.crossings;
    for(int i = 0; i < flype.tangle.cr_num; i++) {
        flip_crossing(new_pd_code[*tangle_crossing]);

        for(int j = 0; j < 4; j++) {
            if(reverse) {
                new_pd_code[*tangle_crossing][j] = ensure_strand_is_in_bounds(new_pd_code[*tangle_crossing][j] + 1, cr_num);
            } else {
                new_pd_code[*tangle_crossing][j] = ensure_strand_is_in_bounds(new_pd_code[*tangle_crossing][j] - 1, cr_num);
            }
        }

        tangle_crossing++;

    }

    int c, d;
    if( abs(a - outstrands[0]) + abs(b - outstrands[1]) == 2 * flype.tangle.cr_num ) {
        c = outstrands[0];
        d = outstrands[1];
    } else {
        c = outstrands[1];
        d = outstrands[0];
    }

    if( abs(a - c) % 2 == 1 ) {
        // |a - c| is ODD => parity 1
        if(reverse) {
            // REVERSED
            if(negative) {
                int new_crossing[4] = {d, c, d + 1, c + 1};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            } else {
                int new_crossing[4] = {d, c + 1, d + 1, c};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            }
        } else {
            // NOT REVERSED
            if(negative) {
                int new_crossing[4] = {d - 1, c - 1, d, c};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            } else {
                int new_crossing[4] = {d - 1, c, d, c - 1};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            }
        }
    } else {
        // |a - c| is EVEN => parity 0
        if(reverse) {
            // REVERSED
            if(negative) {
                int new_crossing[4] = {c, d, c + 1, d + 1};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            } else {
                int new_crossing[4] = {c, d + 1, c + 1, d};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            }
        } else {
            // NOT REVERSED
            if(negative) {
                int new_crossing[4] = {c - 1, d - 1, c, d};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            } else {
                int new_crossing[4] = {c - 1, d, c, d - 1};
                copy_crossing(new_pd_code[flype.crossing], new_crossing);
            }
        }
    }

    for(int j = 0; j < 4; j++) {
        new_pd_code[flype.crossing][j] = ensure_strand_is_in_bounds(new_pd_code[flype.crossing][j], cr_num);
    }

}

void perform_flype(int cr_num, int pd_code[cr_num][4], struct pd_flype flype, int new_pd_code[cr_num][4]) {
    if(is_flype_parallel(cr_num, pd_code, flype)) {
        parallel_flype(cr_num, pd_code, flype, new_pd_code);
    } else {
        anti_parallel_flype(cr_num, pd_code, flype, new_pd_code);
    }
}
