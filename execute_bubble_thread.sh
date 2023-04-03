#!/bin/bash
gcc -pthread -o bubble_thread bubble_thread.c
source ./default.sh
# Remove output.txt file if it already exists
if [ -f bubble_thread.txt ]; then
  rm bubble_thread.txt
fi

for ((i=1; i <= $NUM_ARRAYS; i++)) do
  echo "Running iteration $i"

  # Run a command that takes some time to execute
  ./bubble_thread $i $ARRAY_SIZE $THREADS >> bubble_thread1000x1000x3.txt &
  
  # Wait for the command to finish before starting the next iteration
  wait
done
