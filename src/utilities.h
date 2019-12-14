#ifndef C_FLYPES_UTILITIES_H
#define C_FLYPES_UTILITIES_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "../libplcurve_src/src/plcTopology.h"

#define IS_CONNECTED_TO_0 1
#define NOT_CONNECTED_TO_0 0

const int four_edge_subsets_count[17];

struct pd_crossing {
    int edge[4];
};

struct pd_tangle {
    int cr_num;
    int* crossings; // a list of indices in the pd code array
};

struct pd_tangle_list {
    int num_tangles;
    struct pd_tangle* tangles;
};

struct pd_flype {
    int crossing;
    struct pd_tangle tangle;
};

struct pd_flype_list {
    int num_flypes;
    struct pd_flype* flypes;
};

struct PD_code {
    int cr_num;
    struct pd_crossing* crossings;
};

void matrixPower(const int size, const int* matrix, int n, int* res);

void readPDCodeFromTxtFile(const char * filename);

//void print_flype_info(flype_t flype);

/**
 * Converts the pd_code_t struct pd_code into a familiar Mathematica-esque array for the pd code and stores it in
 * pd_code_arr.
 * @param cr_num
 * @param pd_code
 * @param pd_code_arr
 */
void pd_code_t_to_int_array(int cr_num, pd_code_t *pd_code, int pd_code_arr[][4]);

struct PD_code* pd_code_t_to_pd_code(pd_code_t* pd);

void print_PD_code(struct PD_code* pd_code);

/**
 * Basically just undoes pd_code_t_to_int_array
 * @param cr_num
 * @param pd_code_arr
 * @param pd_code
 */
void int_array_to_pd_code_t(int cr_num, int pd_code_arr[][4], pd_code_t *pd_code);

/**
 * Generates an adjacency matrix for the 4-regular graph representing the knot given
 * by the parameter pd_code.
 * @param cr_num
 * @param pd_code
 * @param adjacency_matrix
 */
void generate_adjacency_matrix_from_pd(int cr_num, int pd_code[cr_num][4], int adjacency_matrix[cr_num][cr_num]);

void get_edge_list_from_adjacency_matrix(int cr_num, int edge_list[2 * cr_num][2], int adjacency_matrix[cr_num][cr_num]);

void get_four_edge_subsets_from_edge_list(int cr_num, int edge_list[2 * cr_num][2], int four_edge_subset_list[][4][2]);

void remove_four_edges_from_adjacency_matrix(int cr_num, int four_edge_subset[4][2], int adjacency_matrix[cr_num][cr_num], int new_adj_matrix[cr_num][cr_num]);

bool is_connected_after_removal_of_edges(int cr_num, int four_edge_subset[4][2], int adjacency_matrix[cr_num][cr_num]);

struct pd_tangle_list get_tangles_from_four_edge_subset(int cr_num, int pd_code[cr_num][4], int four_edge_subset[4][2]);

struct pd_tangle_list get_all_tangles_from_pd_code(int cr_num, int pd_code[cr_num][4]);

bool is_tangle_trivial(int cr_num, struct pd_tangle tangle);

int num_non_trivial_tangles_from_tangle_list(int cr_num, struct pd_tangle_list tangles);

int num_non_trivial_tangles(int cr_num, int pd_code[cr_num][4]);

struct pd_tangle_list get_non_trivial_tangles_from_pd_code(int cr_num, int pd_code[cr_num][4]);

int* get_strands(int cr_num, int pd_code[cr_num][4], struct pd_tangle tangle);

int* get_instrands(int cr_num, int pd_code[cr_num][4], int crossing_index, struct pd_tangle tangle);

int* get_outstrands(int cr_num, int pd_code[cr_num][4], int crossing_index, struct pd_tangle tangle);

struct pd_flype_list get_all_flypes_from_pd_code(int cr_num, int pd_code[cr_num][4]);

bool is_flype_parallel(int cr_num, int pd_code[cr_num][4], struct pd_flype flype);

void anti_parallel_flype(int cr_num, int pd_code[cr_num][4], struct pd_flype flype, int new_pd_code[cr_num][4]);

#endif //C_FLYPES_UTILITIES_H
