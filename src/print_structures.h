#ifndef C_FLYPES_PRINT_STRUCTURES_H
#define C_FLYPES_PRINT_STRUCTURES_H

#include <stdio.h>

#include "utilities.h"

void print_adjacency_matrix(int cr_num, int adjacency_matrix[cr_num][cr_num]);

void print_edge_list(int cr_num, int edge_list[2 * cr_num][2]);

void print_four_edge_subset_list(int cr_num, int four_edge_subset_list[four_edge_subsets_count[cr_num]][4][2]);

void print_pd_tangle(struct pd_tangle tangle);

void print_pd_flype(int cr_num, int pd_code[cr_num][4], struct pd_flype flype);

#endif //C_FLYPES_PRINT_STRUCTURES_H
