import sys
import os

map_file = sys.argv[1]

with open(map_file, 'r') as f:
    l1 = f.readline()
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

    for i in range(0, dim_y):
        for j in range(0, dim_x-1):
            print("{},".format(track[i][j]), end='')
        print("{}".format(track[i][dim_x-1]))

