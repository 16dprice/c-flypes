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
    struct pd_crossing* crossings;
};

struct pd_flype {
    struct pd_crossing cross;
    struct pd_tangle tangle;
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
void generate_adjacency_matrix_from_pd(int cr_num, int pd_code[][4], int adjacency_matrix[cr_num][cr_num]);

void get_edge_list_from_adjacency_matrix(int cr_num, int edge_list[2 * cr_num][2], int adjacency_matrix[cr_num][cr_num]);

void get_four_edge_subsets_from_edge_list(int cr_num, int edge_list[2 * cr_num][2], int four_edge_subset_list[][4][2]);

void remove_four_edges_from_adjacency_matrix(int cr_num, int four_edge_subset[4][2], int adjacency_matrix[cr_num][cr_num], int new_adj_matrix[cr_num][cr_num]);

bool is_connected_after_removal_of_edges(int cr_num, int four_edge_subset[4][2], int adjacency_matrix[cr_num][cr_num]);

void get_tangle_from_four_edge_subset(int cr_num, int pd_code[cr_num][4], int four_edge_subset[4][2], int tangle[2][cr_num]);

void get_all_tangles_from_pd_code(int cr_num, int pd_code[cr_num][4], int tangle_list[2 * four_edge_subsets_count[cr_num]][cr_num]);

bool is_tangle_trivial(int cr_num, const int *tangle);

int num_non_trivial_tangles(int cr_num, int pd_code[cr_num][4]);

void get_non_trivial_tangles_from_pd_code(int cr_num, int pd_code[cr_num][4], int non_trivial_tangle_list[][cr_num]);

int get_tangle_size(int cr_num, int tangle[cr_num]);

bool in_array(int val, int n, int arr[n]);

bool is_crossing_in_tangle(int cr_num, int crossing[4], int pd_code[cr_num][4], int tangle[cr_num]);

void get_flypes_from_tangle(int cr_num, int pd_code[cr_num][4], int tangle[cr_num], int flypable_crossings[2]);

int get_num_flypes(int cr_num, int pd_code[cr_num][4]);

//void get_all_flypes_from_pd_code(int cr_num, int pd_code[cr_num][4], flype_t flypes[]);
//
//void get_instrands(flype_t flype, int instrands[2]);
//
//int get_next_strand(int crossing[4], int strand);
//
//bool is_flype_parallel(flype_t flype);

#endif //C_FLYPES_UTILITIES_H
