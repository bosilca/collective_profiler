/*************************************************************************
 * Copyright (c) 2020, NVIDIA CORPORATION. All rights reserved.
 *
 * See LICENSE.txt for license information
 ************************************************************************/

#include <stdio.h>
#include "grouping.h"

#define MAX_SUBGROUPS (2)
#define MAX_ELTS      (10)

typedef struct gp_result {
    int size;
    int elts[MAX_ELTS];
} gp_result_t;

typedef struct gp_test {
    int num_points;
    int points[MAX_ELTS];
    int num_gps;
    gp_result_t groups_result[MAX_SUBGROUPS];
} gp_test_t;

int grouping_test(void)
{
    gp_test_t tests[] = {
        {
            num_points: 5,
            points: {1, 2, 3, 3, 3},
            num_gps: 2,
            groups_result: {
                {
                    size: 2,
                    elts: {1, 2},
                },
                {
                    size: 3, 
                    elts: {3, 3, 3},
                },
            },
        },
        {
            num_points: 3,
            points: {1, 2, 3},
            num_gps: 1,
            groups_result: {
                {
                    size: 3,
                    elts: {1, 2, 3},
                },
            },
        },
        {
            num_points: 4,
            points: {1, 2, 3, 5},
            num_gps: 1, 
            groups_result: {
                {
                    size: 4,
                    elts: {1, 2, 3, 5},
                },
            },
        },
        {
            num_points: 6,
            points: {1, 2, 3, 10, 11, 12},
            num_gps: 2,
            groups_result: {
                {
                    size: 3,
                    elts: {1, 2, 3},
                },
                {
                    size: 3,
                    elts: {10, 11, 12},
                },
            },
        },
    };

    for (int i = 0; i < 4; i++) {
        fprintf(stdout, "*** Running test %d\n", i);
        for (int j = 0; j < tests[i].num_points; j++)
        {
            fprintf(stdout, "-> Adding %d\n", tests[i].points[j]);
            add_datapoint(j, tests[i].points);
        }

        // Compare the resulting groups with what we expect
        group_t *gps = NULL;
        int num_gps = 0;
        get_groups(&gps, &num_gps);
        if (num_gps != tests[i].num_gps) {
            fprintf(stderr, "*** [ERROR] Test %d reports %d groups instead of %d\n", i, num_gps, tests[i].num_gps);
            return 1;
        }
        group_t *ptr = gps;
        for (int k = 0; k < num_gps; k++)
        {
            for (int l = 0; l < ptr->size; l++)
            {
                if (ptr->size != tests[i].groups_result[k].size) {
                    fprintf(stderr, "*** [ERROR] Returned group #%d has %d elements while expecting %d\n", k, ptr->size, tests[i].groups_result[k].size);
                    return 1;
                }
                if (tests[i].points[ptr->elts[l]] != tests[i].groups_result[k].elts[l]) {
                    fprintf(stderr, "*** [ERROR] element %d of group %d is %d instead of %d\n", l, k, ptr->elts[l], tests[i].groups_result[k].elts[l]);
                    return 1;
                }
            }
            ptr = ptr->next;
        }
        grouping_fini();
        fprintf(stdout, "*** Test %d successful\n", i);
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (grouping_test()) {
        fprintf(stderr, "[ERROR] grouping test failed\n");
    } else {
        fprintf(stdout, "grouping test succeeded\n");
    }

    return 0;
}