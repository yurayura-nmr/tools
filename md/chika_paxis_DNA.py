"""
2018/04/14
Erik Walinda

chika_paxis.py

Get signed principal axis (1)

For Protein - calculate ourselves
For DNA - get paxis from gromacs and just check orientation

"""
import sys
import numpy
import math

def getGromacsPaxis():
    xyz = []
    with open("major_principal_axis.xvg", 'r') as pdb_file:
         for line in pdb_file:
             if "#" in line:
                 pass
             elif "@" in line:
                 pass
             else:
                 xyz.append(line.split()[1:])
    #print xyz
    return numpy.array([float(xyz[0][0]), 
                        float(xyz[0][1]), 
                        float(xyz[0][2])])

xyz = []
with open("test.pdb", 'r') as pdb_file:
    for line in pdb_file:
        if line.startswith("ATOM"):
            # extract x, y, z coordinates for carbon alpha atoms
            if "DG5" in line:
                x = float(line[30:38].strip())
                y = float(line[38:46].strip())
                z = float(line[46:54].strip())
                #if line[12:16].strip() == "CA":
                xyz.append([x, y, z])
            elif "DC3" in line:
                x = float(line[30:38].strip())
                y = float(line[38:46].strip())
                z = float(line[46:54].strip())
                #if line[12:16].strip() == "CA":
                xyz.append([x, y, z])
            else:
                pass

FivePrime = numpy.array(xyz[0])
ThreePrime = numpy.array(xyz[-1])

#print "5' ", FivePrime
#print "3' ", ThreePrime

major_axis_1 = getGromacsPaxis()

CN = ThreePrime - FivePrime
CROSS = numpy.dot(CN, major_axis_1)

#print "Vector from center of gravity to the N-terminal Calpha atom: ", CN
#print "Cross-product of first principal axis to symmetry-break vector: ", CROSS

if CROSS < 0:
    major_axis_1 = - major_axis_1
elif CROSS > 0:
    major_axis_1 = major_axis_1


print major_axis_1[0], " ", major_axis_1[1], " ", major_axis_1[2]
