#!/bin/bash
echo "Promoting ..."
# profiling environment variables
export JOB_NOW=$( date +%Y%m%d-%H%M%S )
export PROJECT_ROOT=/global/home/users/cyrusl/placement/expt0070/alltoall_profiling

EXAMPLE_PROGS=(alltoall_simple_c alltoall_bigcounts_c alltoall_multicomms_c alltoall_dt_c)  

for EXAMPLE_PROG in ${EXAMPLE_PROGS[@]}
do
        mv -v $PROJECT_ROOT/tests/$EXAMPLE_PROG/unchecked/* \
           $PROJECT_ROOT/tests/$EXAMPLE_PROG/expectedOutput/
done

echo "Moved all results from unchecked to their positions at test/alltoall_*,"
echo "which indicate that they are indeed the expected results files."