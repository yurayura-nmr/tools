#!/usr/bin/env python3

"""
Erik Walinda
Kyoto University
Graduate School of Medicine

Axes3D requires a weird version of matplotlib for the
current script to work.
The problem is in the coloring of the dots, i.e. the c=i statement.
"""

import matplotlib

# Force matplotlib to not use any Xwindows backend.
matplotlib.use('Agg')

import numpy
import matplotlib.pyplot as plt

from mpl_toolkits.mplot3d import Axes3D

def readFile(filename):
    array = []

    with open(filename, "rt") as infile:
        for line in infile:
            columns = line.split()
            array.append(columns)

    return numpy.array(array, dtype=float)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

vectors = readFile("output.txt")

"""
Start and end FRAME

2000 = 2000 * 10 ps = 20 ns
"""
start = 0
end   = 1000
vectors=vectors[start:end]

nColors = len(vectors)

#cmap = plt.get_cmap('jet', len(vectors)*10)
cmap = plt.get_cmap('jet', nColors)
cmap.set_under('gray')

print (vectors)

for i in range(nColors):
    vec = vectors[i]
    print(i)
    #cax = ax.scatter(vec[0], vec[1], vec[2], '-o', cmap=cmap, c=i, vmin=0, vmax=nColors, edgecolors='none')
    cax = ax.scatter(vec[0], vec[1], vec[2], '-o', cmap=cmap, c=i,  vmin=0, vmax=nColors)

ax.set_xlim([-1, 1])
ax.set_ylim([-1, 1])
ax.set_zlim([-1, 1])

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

#fig.colorbar(cax)

#plt.show()

ax.view_init(elev=10, azim=7.5)
plt.savefig("test_" + str(start) + "_" + str(end) + ".pdf")

plt.close()
