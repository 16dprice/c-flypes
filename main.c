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

    struct pd_flype_list all_flypes = get_all_flypes_from_pd_code(cr_num, pd_code);

    pd_stor_t *flyped_codes;
    pd_code_t *new_pd_code_t;

    flyped_codes = pd_new_pdstor();
//    new_pd_code_t = pd_code_new((pd_idx_t) cr_num + 2);
    new_pd_code_t = pd_copy(next_pd_code);

    int new_pd_code[cr_num][4];

    pd_addto_pdstor(flyped_codes, next_pd_code, DIAGRAM_ISOTOPY);

    int old_num_codes, new_num_codes;
    old_num_codes = nelts_of_pd_stor(flyped_codes);

    for(int i = 0; i < all_flypes.num_flypes; i++) {

        perform_flype(cr_num, pd_code, all_flypes.flypes[i], new_pd_code);
        int_array_to_pd_code_t(cr_num, new_pd_code, new_pd_code_t);
        pd_addto_pdstor(flyped_codes, new_pd_code_t, DIAGRAM_ISOTOPY);
//        print_pd_code(cr_num, new_pd_code);

    }

    outfile = fopen("/home/dj/CLionProjects/c-flypes/flype_output/knot_6_3.txt", "w");
    for(next_pd_code = pd_stor_firstelt(flyped_codes); next_pd_code != NULL; next_pd_code = pd_stor_nextelt(flyped_codes)) {
        pd_write_KnotTheory(outfile, next_pd_code);
        free(next_pd_code);
    }

    fclose(infile);
    fclose(outfile);

    pd_free_pdstor(&flyped_codes);

    return 0;

}
