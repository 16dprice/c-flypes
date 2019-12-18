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

    FILE *infile, *outfile;

    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }

    // NOTE: the code starts not working for knots of 16 crossings
    infile = fopen("/home/dj/CLionProjects/c-flypes/knot_txt_files/knot_12_499.txt", "r");
    pd_code_t* first_code = pd_read_KnotTheory(infile);

    pd_stor_t *flyped_codes;
    flyped_codes = run_get_all_pd_codes_dfs(first_code);

    pd_code_t* next_pd_code;
    outfile = fopen("/home/dj/CLionProjects/c-flypes/flype_output/knot_12_499.txt", "w");
    for(next_pd_code = pd_stor_firstelt(flyped_codes); next_pd_code != NULL; next_pd_code = pd_stor_nextelt(flyped_codes)) {
        pd_write_KnotTheory(outfile, next_pd_code);
        free(next_pd_code);
    }

    fclose(infile);
    fclose(outfile);

    pd_free_pdstor(&flyped_codes);

    return 0;

}
