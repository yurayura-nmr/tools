Get started to cook
-------------------

Preparation
"""""""""""

First, let's obtain our PDB file.
As an example, let's take our favourite molecule: 1UBQ.pdb

Next, we have to generate a PQR file.
That's easy because there is a webserver to do that for us:

https://server.poissonboltzmann.org/ 
http://nbcr-222.ucsd.edu/pdb2pqr_2.0.0/

According to this server: 
*PQR files are PDB files where the occupancy and B-factor columns have been replaced by per-atom charge and radius.*

So nothing too complicated is done in this step.

This may be interesting for Ulrich's research too:
*Use PROPKA to assign protonation states at provided pH.*

Of course, we could calculate that by hand, but why do that if it is so easy to use a tool.

Please enter either > Upload file > 1UBQ.pdb
Pick a forcefield to use > AMBER
Pick an output naming scheme to use: Internal naming scheme
pKa Options> Use pH = desired pH > Use PROPKA to assign protonation states at provided pH

APBS calculation
""""""""""""""""

Just use webserver.

In PyMOL:

cd to directory where result *.dx file is stored.

dir
open 1UBQ.pdb
load 1.dx, map
show surface, 1UBQ
ramp_new elvl, map,[-14,0,14]
set surface_color, elvl, 1UBQ

APBS writes out the electrostatic potential in dimensionless units of kb*T*ec-1 where:

#. kb is Boltzmann's constant
#. T is the temperature of your calculation in K
#. ec is the charge of an electron

If we want a unit, it is [V] (potential).
Pymol visualizes the electrostatic potential map.

 
