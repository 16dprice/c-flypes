#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#include <time.h>

#include "plcTopology.h"
#include "src/utilities.h"

int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("Two arguments required: 'Crossing Number' and 'Ordering'. %d given.\n", argc);
        return 0;
    }

    char knot_source_path[PATH_MAX], knot_output_path[PATH_MAX];
    FILE *infile, *outfile;
    pd_code_t *first_code;
    pd_stor_t *flyped_codes;
    pd_code_t *next_pd_code;

    clock_t start, end;
    double time_spent;

    int cr_num = atoi(argv[1]);
    int ordering = atoi(argv[2]);

//    start = clock();

    snprintf(knot_source_path, PATH_MAX, "/home/dj/CLionProjects/c-flypes/knot_txt_files/%d_crossings/knot_%d_%d.txt", cr_num, cr_num, ordering);
    snprintf(knot_output_path, PATH_MAX, "/home/dj/CLionProjects/c-flypes/flype_output/bfs/%d_crossings/knot_%d_%d.txt", cr_num, cr_num, ordering);

    infile = fopen(knot_source_path, "r");
    first_code = pd_read_KnotTheory(infile);

//    flyped_codes = run_get_all_pd_codes_dfs(first_code);

    flyped_codes = get_all_pd_codes_bfs(first_code);

    outfile = fopen(knot_output_path, "w");
    for(next_pd_code = pd_stor_firstelt(flyped_codes); next_pd_code != NULL; next_pd_code = pd_stor_nextelt(flyped_codes)) {
        pd_write_KnotTheory(outfile, next_pd_code);
        free(next_pd_code);
    }

    fclose(infile);
    fclose(outfile);

    pd_free_pdstor(&flyped_codes);

//    end = clock();
//    time_spent = (double) (end - start) / CLOCKS_PER_SEC;
//    printf("Time spent on knot %d - %d: %f\n", cr_num, ordering, time_spent);

    return 0;

}
