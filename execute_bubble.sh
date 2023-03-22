#!/bin/bash
gcc -pthread -o bubble bubble.c
source ./default.sh
# Remove output.txt file if it already exists
if [ -f bubble.txt ]; then
  rm bubble.txt
fi

for ((i=1; i <= $NUM_ITERATIONS; i++)) do
  echo "Running iteration $i"

  # Run a command that takes some time to execute
  ./bubble $i $ARRAY_SIZE >> bubble.txt &
  
  # Wait for the command to finish before starting the next iteration
  wait
done
