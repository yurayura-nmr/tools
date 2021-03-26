#!/usr/bin/env python

# Prepare di-ubiquitin trajectory for pymol viewing
# by adding chain labels to the pdb file

################################################################

def inBoundary(boundary, atomNumber):
    if atomNumber >= boundary[0]:
        if atomNumber <= boundary[1]:
            return True
    else:
        return False

################################################################

def attachLabel(line, label):
    labeledLine = line[:21] + label + line[22:]
    return labeledLine

################################################################

def parsePDB(boundaryDistal, boundaryProximal, filename):
    filenameLabeled = filename + '_labeled.pdb'

    with open(filenameLabeled, "wt") as fout:
        with open(filename, "rt") as fin:
            for line in fin:
                if 'ATOM' in line:
                    atomNumber = int(line.split()[1])
                    # Distal ubiquitin subunit?
                    if inBoundary(boundaryDistal, atomNumber) is True:
                        outline = attachLabel(line, 'A')
                        fout.write(outline)
                    elif inBoundary(boundaryProximal, atomNumber) is True:
                        outline = attachLabel(line, 'B')
                        fout.write(outline)
                else:
                    fout.write(line)

################################################################

# Setup

distal = [1, 1229]
proximal = [1230, 2458]

filename = './in.pdb'

parsePDB(distal, proximal, filename)
