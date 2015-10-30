#!/bin/sh
# @ job_name           = julia_256
# @ job_type           = bluegene
# @ comment            = "BGQ Julia Assgn 3"
# @ error              = $(job_name).$(jobid).err
# @ output             = $(job_name).$(jobid).out
# @ bg_size            = 64
# @ wall_clock_limit   = 1:00
# @ bg_connectivity    = Torus
# @ queue

# Launch all BGQ jobs using runjob
runjob --np 256 --ranks-per-node=16 --cwd=$PWD/ : $PWD/julia params.dat
