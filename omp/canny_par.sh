#!/bin/bash
#$ -N CANNYOMP
#$ -q class8i
#$ -pe openmp 8

# Module load gcc compiler version 4.8.2
module load  gcc/4.9.2
# Module load Cuda Compilers and GCC
module load  cuda/5.0
module load  gcc/4.4.3

# Runs a bunch of standard command-line
# utilities, just as an example:

echo "Script began:" `date`
echo "Node:" `hostname`
echo "Current directory: ${PWD}"

echo ""
echo "=== Running 5 trials of naive ... ==="
./canny_par


echo ""
echo "=== Done! ==="

# eof
