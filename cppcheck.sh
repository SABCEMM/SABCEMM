#!/bin/bash
cppcheck --enable=all --std=c++11 --quiet -Ilibs/boost/ -Ilibs/progressbar/include -Isrc -ITests --inconclusive src Tests --suppress="*:*Algorithms*"
