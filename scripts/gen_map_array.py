# Uitlity script to generate array form .track file in syntax of specified
# language

# Usage : python gen_map_array.py <.track file> <language>
# supported languages: c, python

import sys
import os

map_file = sys.argv[1]
language = sys.argv[2]

with open(map_file, 'r') as f:
    l1 = f.readline()
    if language == "python":
        track = [[a for a in b] for b in f.read().split('\n') if b]
        t = list(map(list, zip(*track)))
        print('[')
        for x in t:
            print("{},".format(x))

        print(']')

    elif language == "c":
        dims=l1.split()[1:]
        dim_x = int(dims[0])
        dim_y = int(dims[1])
        track = []
        for y in range (0, dim_y):
            track.append([])

        for i in range(0, dim_x):
            l = f.readline()
            l = l.rstrip()
            for j in range(0, dim_y):
                track[j].append(l[j])

        print('{')
        for i in range(0, dim_y):
            print('{', end='')
            for j in range(0, dim_x-1):
                print("'{}', ".format(track[i][j]), end='')
            print("'{}'".format(track[i][dim_x-1]), end='')
            print("},")

        print('};')
    else:
        print("Language not supported")
