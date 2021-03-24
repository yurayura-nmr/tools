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

DNA = True


def read_pdb_xyz(pdb_name):
    """
    Reads atomic coordinates of C-alpha atoms from a .pdb file.

    Parameters
    ----------
    pdb_name : str
        Name of pdb file.

    Returns
    -------
    array of atomic coordinates
        [[x1 y1 z1]
         [x2 y2 z2]
         [.. .. ..]
         [xn yn zn]]
    """
    xyz = []
    with open(pdb_name, 'r') as pdb_file:
        for line in pdb_file:
            if line.startswith("ATOM"):
                # extract x, y, z coordinates for carbon alpha atoms
                x = float(line[30:38].strip())
                y = float(line[38:46].strip())
                z = float(line[46:54].strip())
                if line[12:16].strip() == "CA":
                    xyz.append([x, y, z])
    return xyz


def principalAxes(filename):

    # check if argument file exists
    pdb_name = filename

    #--------------------------------------------------------------------------
    # compute principal axes
    #--------------------------------------------------------------------------
    # read pdb
    xyz = read_pdb_xyz(pdb_name)
    #print("%d CA atomes found in %s" %(len(xyz), pdb_name))

    #create coordinates array
    coord = numpy.array(xyz, float)

    # compute geometric center
    center = numpy.mean(coord, 0)
    #print("Coordinates of the geometric center:\n", center)

    # center with geometric center
    coord = coord - center

    # compute principal axis matrix
    inertia = numpy.dot(coord.transpose(), coord)
    e_values, e_vectors = numpy.linalg.eig(inertia)
    # warning eigen values are not necessary ordered!
    # http://docs.scipy.org/doc/numpy/reference/generated/numpy.linalg.eig.html
    #print("(Unordered) eigen values:")

    #print "E_vectors", e_vectors

    #--------------------------------------------------------------------------
    # order eigen values (and eigen vectors)
    #
    # axis1 is the principal axis with the biggest eigen value (eval1)
    # axis2 is the principal axis with the second biggest eigen value (eval2)
    # axis3 is the principal axis with the smallest eigen value (eval3)
    #--------------------------------------------------------------------------
    order = numpy.argsort(e_values)
    eval3, eval2, eval1 = e_values[order]
    axis3, axis2, axis1 = e_vectors[:, order].transpose()
    #print("Inertia axis are now ordered !")

    #print "Major ", axis1
    #print "Middle ", axis2
    #print "Minor ", axis3

    #--------------------------------------------------------------------------

    CS = numpy.array([axis1,
                      axis2,
                      axis3])

    Nter = xyz[0]
                   
    return CS, numpy.array(center), numpy.array(Nter)


def calculateSignedAxis():

    domain1 = "test.pdb"

    # Obtain the axes using the PyMOL script
    CS1, center, Nter = principalAxes(domain1)

    major_axis_1 = numpy.array(CS1[0])

    #print "Major axis: ", major_axis_1
    #print "Center of gravity: ", center
    #print "Nterminus: ", Nter

    CN = Nter - center
    CROSS = numpy.dot(CN, major_axis_1)

    #print "Vector from center of gravity to the N-terminal Calpha atom: ", CN
    #print "Cross-product of first principal axis to symmetry-break vector: ", CROSS

    if CROSS < 0:
        return (- major_axis_1)
    elif CROSS > 0:
        return major_axis_1

signedAxis = calculateSignedAxis()
print signedAxis[0], " ", signedAxis[1], " ", signedAxis[2]
