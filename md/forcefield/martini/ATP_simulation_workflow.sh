# Some ATP molecules in a box
gmx insert-molecules -ci ./ATP.gro -box 7.5 7.5 7.5 -nmol 128 -radius 0.21 -try 500 -o 128_ATP_noW.gro

# Run energy minimization
# Requires martini files and ATP.itp
gmx grompp -f minimization.mdp -c 128_ATP_noW.gro -p ATP.top -o ATP-min-init.tpr 
gmx mdrun -deffnm ATP-min-init -v -c 128_minimized.gro

# Solvate
gmx solvate -cp 128_minimized.gro -cs water.gro -o waterbox.gro -radius 0.21 

# Now minimize again
gmx grompp -f minimization.mdp -c waterbox.gro -p ATP.top -o ATP-min-solvent.tpr
gmx mdrun -deffnm ATP-min-solvent -v -c minimized.gro

# Ions
gmx grompp -f ./ions.mdp -c minimized.gro -p ./ATP.top -o ./ions.tpr # -maxwarn 1 
printf "W" | gmx genion -s ions.tpr -o ATP_solv_ions.gro -p ./ATP.top -neutral

# Now minimize again
gmx grompp -f minimization.mdp -c ATP_solv_ions.gro -p ATP.top -o ATP-min-solvent-ions.tpr
gmx mdrun -deffnm ATP-min-solvent-ions -v -c ATP-min-solvent-ions-minimized.gro

# Run MD
gmx grompp -f martini_md.mdp -c ATP-min-solvent-ions-minimized.gro -p ATP.top -o ATP-md.tpr
gmx mdrun -deffnm ATP-md -v

# Visualize with PYMOL
gmx trjconv -s ATP-md.tpr -f ATP-md.xtc -o ATP-md.pdb -pbc whole -conect -dt 100