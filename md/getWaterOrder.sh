# 2021/07/14

gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d X -o orderX_noE.xvg
gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d Y -o orderY_noE.xvg
gmx h2order -f md_0_1_E10000M.xtc -s md_0_1_E10000M.tpr -n index.ndx -d Z -o orderZ_noE.xvg
