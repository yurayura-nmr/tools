"""
2021/07/14

Just personal notes to remember what GROMACS h2order does:

Slice 3D box into 2D planes. 
Take the normal vector (e.g. box Z-axis vector on the xy plane).
Then compute the angle between this vector and the dipole axis of each water molecule.
Report average for each slice.
"""

gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d X -o orderX.xvg
gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d Y -o orderY.xvg
gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d Z -o orderZ.xvg

# Test if any periodic behaviour occurs

gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d X -o orderX_00.xvg -b 0 -e 100
gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d Y -o orderY_00.xvg -b 0 -e 100
gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d Z -o orderZ_00.xvg -b 0 -e 100

gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d X -o orderX_01.xvg -b 100 -e 200
gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d Y -o orderY_01.xvg -b 100 -e 200
gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d Z -o orderZ_01.xvg -b 100 -e 200

