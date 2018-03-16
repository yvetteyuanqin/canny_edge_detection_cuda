#!/bin/bash
#$ -N CANNYOMP
#$ -q class8i


# Module load gcc compiler version 4.8.2
module load  gcc/4.9.2


# Runs a bunch of standard command-line
# utilities, just as an example:

echo "Script began:" `date`
echo "Node:" `hostname`
echo "Current directory: ${PWD}"

echo ""

./canny_seq


echo ""
echo "=== Done! ==="

# eof
