#ifndef C_FLYPES_PRINT_STRUCTURES_H
#define C_FLYPES_PRINT_STRUCTURES_H

#include <stdio.h>

#include "utilities.h"

void print_adjacency_matrix(int cr_num, int adjacency_matrix[cr_num][cr_num]);

void print_edge_list(int cr_num, int edge_list[2 * cr_num][2]);

#endif //C_FLYPES_PRINT_STRUCTURES_H
