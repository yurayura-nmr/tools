"""
Still unsigned axis.
Can easily break symmetry by defining N->C vector and taking cross-product
of 1st principal axis with this vector, then requiring the vector directed
so that this cross product be positive.
"""

import MDAnalysis
from MDAnalysis.tests.datafiles import PSF, DCD

import numpy as np

# Your file must be read here
ubiquitin = MDAnalysis.Universe("md_0_1_Esmall310.gro", "md_0_1_Esmall310.xtc")

for timeStep in ubiquitin.trajectory:
    print("Frame: {0:5d}, Time: {1:8.3f} ps".format(timeStep.frame, ubiquitin.trajectory.time))

    CA = ubiquitin.select_atoms("protein and name CA")
    I = CA.moment_of_inertia()
    UT = CA.principal_axes()

    print UT[0]

# transpose the row-vector layout UT = [p1, p2, p3]
#U = UT.T

# test that U diagonalizes I
#Lambda = U.T.dot(I.dot(U))
#print(Lambda)

# check that it is diagonal (to machine precision)
#print(np.allclose(Lambda - np.diag(np.diagonal(Lambda)), 0))
