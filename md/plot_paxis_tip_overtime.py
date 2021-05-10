#!/usr/bin/env python3

"""
Erik Walinda

Kyoto University
Graduate School of Medicine

Last update: 2021/5/10
* Rewrote c=object part of the ax.scatter
* Thus, finally compatible with Python 3.8.5
* Tested with matplotlib version 3.3.4, 3.3.2

"""
import matplotlib
matplotlib.use('Agg')  # Force matplotlib to not use any Xwindows backend.

print ("[ Matplotlib version ]", matplotlib.__version__)  # 3.3.4 works well

import numpy
import matplotlib.pyplot as plt

from matplotlib import cm
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

vectors = readFile("paxis_output.txt")

"""
Specify start and end FRAMEs
For example, a MD trajectory saved every 10 ps has 2000 frames to reach 20 ns in duration.

2000 = 2000 * 10 ps = 20 ns
"""
start = 0
end   = 1000

vectors = vectors[start:end]
nColors = len(vectors)

cmap = plt.get_cmap('jet', nColors)  # Can specify other colors by exchanging jet to any other matplotlib colormap
cmap.set_under('gray')

#print (vectors)

"""
Loop over the array (time boundaries specified by start and end) to draw 3D plot
"""
for i in range(nColors):
    print("[\033[1;32m  Frame  \033[0m] ", i)

    vec = vectors[i]

    # Get color as integer value
    color = [cmap(float(i) / nColors)]  # The use of [] here is a workaround since ax.scatter expects an array, not an integer.
    cax = ax.scatter(vec[0], vec[1], vec[2], '-o', cmap=cmap, c=color, vmin=0, vmax=nColors, edgecolors='none')
    #cax = ax.scatter(vec[0], vec[1], vec[2], '-o', cmap=cmap, c=color, vmin=0, vmax=nColors) # matplotlib 3.4.2 can create an error when using edgecolors

ax.set_xlim([-1, 1])
ax.set_ylim([-1, 1])
ax.set_zlim([-1, 1])

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

fig.colorbar(cm.ScalarMappable(norm=None, cmap=cmap), ax=ax)
#plt.show()  # Commented out for remote use via ssh.

ax.view_init(elev=10, azim=7.5)
plt.savefig("test_" + str(start) + "_" + str(end) + ".pdf")

plt.close()
