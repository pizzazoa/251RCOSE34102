#!/bin/bash


for j in {1..100}; do
    for i in {1..100}; do
        ./producer 0 1 &
    done
done

wait