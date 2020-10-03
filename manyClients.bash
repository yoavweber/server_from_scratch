#!/bin/bash

for N in {1..150}
do
    python3 client.py &
done
wait