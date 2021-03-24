import MDAnalysis
from MDAnalysis.tests.datafiles import PSF, DCD
import numpy as np


# Your file must be read here
#u = MDAnalysis.Universe("traj.pdb")
ubiquitin = MDAnalysis.Universe("md_0_1_Esmall310.gro", "md_0_1_Esmall310.xtc")
#ubiquitin = MDAnalysis.Universe("md_0_1_Esmall310.gro")

for timeStep in ubiquitin.trajectory:
    print("Frame: {0:5d}, Time: {1:8.3f} ps".format(timeStep.frame, ubiquitin.trajectory.time))
    #print("Rgyr: {0:g} A".format(ubiquitin.atoms.radius_of_gyration()))

    CA = ubiquitin.select_atoms("protein and name CA")
    #print CA

    I = CA.moment_of_inertia()

    #print I

    UT = CA.principal_axes()

    print UT[0]

# transpose the row-vector layout UT = [p1, p2, p3]
#U = UT.T

# test that U diagonalizes I
#Lambda = U.T.dot(I.dot(U))
#print(Lambda)

# check that it is diagonal (to machine precision)
#print(np.allclose(Lambda - np.diag(np.diagonal(Lambda)), 0))
