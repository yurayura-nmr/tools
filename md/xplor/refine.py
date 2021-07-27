import sys

"""
Command line to run this script:
xplor -py refine.py

Erik Walinda
2021/07/27

Script was originally written by CDS in 2005 for version 2.34, 
but in my case some errors are thrown for me when working with Zn2+ ions and RDC pseudoatoms. 
Therefore, it works better in version 3.3.

Current workdir: ~/.../~/aru_rikai_20210727
"""
xplor.requireVersion("3.3")

# Slow cooling protocol in torsion angle space for protein G. 
# Uses NOE, RDC, J-coupling restraints.
# This version refines from a reasonable model structure.
# CDS 2005/05/10

(opts, args) = xplor.parseArguments(["quick"]) # check for command-line typos

quick = False
for opt in opts:
    if opt[0]=="quick":  #specify -quick to just test that the script runs
        quick=True
        pass
    pass

numberOfStructures = 100

if quick:
    numberOfStructures = 3
    pass

# Protocol module has many high-level helper functions.
import protocol
protocol.initRandomSeed(3421)   #explicitly set random seed

# Annealing settings
command = xplor.command

protocol.initParams("protein")
#sys.exit()

# Read an existing model
protocol.loadPDB("xplor.pdb")
xplor.simulation.deleteAtoms("not known")

protocol.fixupCovalentGeom(maxIters=100, useVDW=1)
#sys.exit()

# A PotList contains a list of potential terms. This is used to specify which
# terms are active during refinement.
from potList import PotList
potList = PotList()

# Parameters to ramp up during the simulated annealing protocol
from simulationTools import MultRamp, StaticRamp, InitialParams

rampedParams=[]
highTempParams=[]

# Compare atomic Cartesian rmsd with a reference structure
# Backbone and heavy atom RMSDs will be printed in the output
# structure files
from posDiffPotTools import create_PosDiffPot
refRMSD = create_PosDiffPot("refRMSD","name CA or name C or name N",
                            pdbFile='xplor.pdb',
                            cmpSel="not name H*")

# Orientation Tensor - used with the dipolar coupling term
# One for each medium
# For each medium, specify a name, and initial values of Da, Rh.
from varTensorTools import create_VarTensor

media={}

# medium  Da   rhombicity

# using pales
# DATA Da_HN       6.8491193 
# DATA Rhombicity  0.14428608

for (medium, Da, Rh) in [ ('t',   6.8491193, 0.14428608)]:
    oTensor = create_VarTensor(medium)
    oTensor.setDa(Da)
    oTensor.setRh(Rh)
    media[medium] = oTensor
    pass
 
#sys.exit()   

# dipolar coupling restraints for protein amide NH.  
#
# collect all RDCs in the rdcs PotList
#
# RDC scaling. Three possible contributions.
#   1) gamma_A * gamma_B / r_AB^3 prefactor. So that the same Da can be used
#      for different expts. in the same medium. Sometimes the data is
#      prescaled so that this is not needed. scale_toNH() is used for this.
#      Note that if the expt. data has been prescaled, the values for rdc rmsd
#      reported in the output will relative to the scaled values- not the expt.
#      values.
#   2) expt. error scaling. Used here. A scale factor equal to 1/err^2
#      (relative to that for NH) is used.
#   3) sometimes the reciprocal of the Da^2 is used if there is a large
#      spread in Da values. Not used here.
from rdcPotTools import create_RDCPot, scale_toNH
rdcs = PotList('rdc') 
for (medium,expt,file,                 scale) in \
    [('t','NH' ,'nzf.rdc'       ,1)]:
    rdc = create_RDCPot("%s_%s"%(medium,expt),file,media[medium])

    #1) scale prefactor relative to NH
    #   see python/rdcPotTools.py for exact calculation
    # scale_toNH(rdc) - not needed for these datasets -
    #                        but non-NH reported rmsd values will be wrong.

    #3) Da rescaling factor (separate multiplicative factor)
    # scale *= ( 1. / rdc.oTensor.Da(0) )**2
    rdc.setScale(scale)
    rdc.setShowAllRestraints(1) #all restraints are printed during analysis
    rdc.setThreshold(1.5)       # in Hz
    rdcs.append(rdc)
    pass

potList.append(rdcs)
rampedParams.append( MultRamp(0.05,5.0, "rdcs.setScale( VALUE )") )

# Calculate initial tensor orientation
# and setup tensor calculation during simulated annealing
from varTensorTools import calcTensorOrientation, calcTensor
for medium in media.keys():
    calcTensorOrientation(media[medium])
    rampedParams.append( StaticRamp("calcTensor(media['%s'])" % medium) )
    pass

# set up NOE potential
noe = PotList('noe')
potList.append(noe)
from noePotTools import create_NOEPot
for (name,scale,file) in [('all', 1, "final.tbl"),
                          #add entries for additional tables
                          ]:
    pot = create_NOEPot(name,file)
    pot.setPotType("soft") # if you think there may be bad NOEs
    pot.setScale(scale)
    noe.append(pot)
rampedParams.append( MultRamp(2,30, "noe.setScale( VALUE )") )

#sys.exit()

# Set up dihedral angles
from xplorPot import XplorPot
protocol.initDihedrals("talos.tbl",
                       #useDefaults=False  # by default, symmetric sidechain
                                           # restraints are included
                       )
potList.append( XplorPot('CDIH') )
highTempParams.append( StaticRamp("potList['CDIH'].setScale(10)") )
rampedParams.append( StaticRamp("potList['CDIH'].setScale(200)") )
# set custom values of threshold values for violation calculation
#
potList['CDIH'].setThreshold( 5 ) #5 degrees is the default value, though

# Set up Zinc coodinating restraints
import protocol
protocol.initTopology("ion.top")
protocol.initParams("ion.par")

"""
Adding Zn2+ atom to the 4 coordination cysteine residues. 
Follow directions in the following patch:
1, 2, 3, 4 are cysteine residues.
5 is the zinc atom.

Specify which residue numbers are these cysteine and zinc residues at the end in the !do patch part.
"""

xplor.command("""
topology
AUTO ANGLe=False DIHEdral=False END
 
! patch to create Zn (Cys)4
! 1 and 2 should be CYS
! 3 and 4 should be CYS
 presidue ZC4
! new for zinc-2 

!  delete atom 1HG end
!  delete atom 2HG end
!  delete atom 3HG end
!  delete atom 4HG end

!  add bond 5ZN+2 1SG
!  add bond 5ZN+2 2SG
!  add bond 5ZN+2 3SG
!  add bond 5ZN+2 4SG

!  add angle 1SG  5ZN+2 2SG
!  add angle 1SG  5ZN+2 3SG
!  add angle 1SG  5ZN+2 4SG
!  add angle 2SG  5ZN+2 3SG
!  add angle 2SG  5ZN+2 4SG
!  add angle 3SG  5ZN+2 4SG

!  add angle 1CB  1SG  5ZN+2 
!  add angle 2CB  2SG  5ZN+2 
!  add angle 3CB  3SG  5ZN+2
!  add angle 4CB  4SG  5ZN+2
!  end zinc-2
!  add improper +HA1 +HA2 -SG +C  !stereo CA

end
end

!add the PHO groups 
segment
 setup=true
number=300
 chain
   sequence ZN2 end
 end
end

 
!param
!  bond  1SG 5ZN+2 500.0 2.40 !FIX!
!  bond  2SG 5ZN+2 500.0 2.30 !FIX!
!  bond  3SG 5ZN+2 500.0 2.40 !FIX!
!  bond  4SG 5ZN+2 500.0 2.40 !FIX!
!end
 
 
!do patch
 patch ZC4  reference=1=(resid 13) 
            reference=2=(resid 16) 
            reference=3=(resid 27) 
            reference=4=(resid 30)
            reference=5=(resid 300) 
end
 
""")


xplor.command("write psf output=z4c.psf end")

#sys.exit()

# Gyration volume term 
from gyrPotTools import create_GyrPot
gyr = create_GyrPot("Vgyr",
                    "resid 6:65") # selection should exclude disordered tails
potList.append(gyr)
rampedParams.append( MultRamp(.002, 1, "gyr.setScale(VALUE)") )

# hbda - distance/angle bb hbond term
#protocol.initHBDA('hbda.tbl')
#potList.append( XplorPot('HBDA') )

# hbdb - knowledge-based backbone hydrogen bond term
#protocol.initHBDB()
protocol.initHBDB("resid 6:65") # specify residues to avoid error with pseudo-atoms later.
potList.append( XplorPot('HBDB') )

# New torsion angle database potential
from torsionDBPotTools import create_TorsionDBPot
torsionDB = create_TorsionDBPot('torsionDB')
potList.append( torsionDB )
rampedParams.append( MultRamp(.002, 2, "torsionDB.setScale(VALUE)") )

# Setup parameters for atom-atom repulsive term. (van der Waals-like term)
potList.append( XplorPot('VDW') )
rampedParams.append( StaticRamp("protocol.initNBond(nbxmod=4)") )
rampedParams.append( MultRamp(0.9, 0.8,
                              "command('param nbonds repel VALUE end end')") )
rampedParams.append( MultRamp(.004, 4,
                              "command('param nbonds rcon VALUE end end')") )

# Nonbonded interaction only between CA atoms
highTempParams.append( StaticRamp("""protocol.initNBond(cutnb=100,
                                                        rcon=0.004,
                                                        tolerance=45,
                                                        repel=1.2,
                                                        onlyCA=1)""") )

potList.append( XplorPot("BOND") )
potList.append( XplorPot("ANGL") )
potList['ANGL'].setThreshold( 5 )
rampedParams.append( MultRamp(0.4,1,"potList['ANGL'].setScale(VALUE)") )
potList.append( XplorPot("IMPR") )
potList['IMPR'].setThreshold( 5 )
rampedParams.append( MultRamp(0.1,1,"potList['IMPR'].setScale(VALUE)") )
      
# Give atoms uniform weights, except for the anisotropy axis
protocol.massSetup()

# IVM setup
#   the IVM is used for performing dynamics and minimization in torsion-angle
#   space, and in Cartesian space.
from ivm import IVM
dyn = IVM()

# Initially minimize in Cartesian space with only the covalent constraints.
#   Note that bonds, angles and many impropers can't change with the 
#   internal torsion-angle dynamics
#   breaks bonds topologically - doesn't change force field
dyn.potList().add( XplorPot("BOND") )
dyn.potList().add( XplorPot("ANGL") )
dyn.potList().add( XplorPot("IMPR") )

dyn.breakAllBondsIn("not resname ANI")
import varTensorTools
for m in media.values():
    m.setFreedom("fix")                  # Fix tensor parameters
    varTensorTools.topologySetup(dyn, m) # Setup tensor topology

protocol.initMinimize(dyn, numSteps=1000)
dyn.run()

# Reset ivm topology for torsion-angle dynamics
dyn.reset()

for m in media.values():
#    m.setFreedom("fixDa, fixRh")       # Fix tensor Rh, Da, vary orientation
    m.setFreedom("varyDa, varyRh")      # Vary tensor Rh, Da, vary orientation
protocol.torsionTopology(dyn)

# Minc used for final cartesian minimization
minc = IVM()
protocol.initMinimize(minc)

for m in media.values():
    m.setFreedom("varyDa, varyRh")    #allow all tensor parameters float here
    pass
protocol.cartesianTopology(minc)

# Object which performs simulated annealing
from simulationTools import AnnealIVM
init_t  = 3000.     # Need high temp and slow annealing to converge
cool = AnnealIVM(initTemp = init_t,
                 finalTemp = 25,
                 tempStep = 12.5,
                 ivm = dyn,
                 rampedParams = rampedParams)

def accept(potList):
    """
    return True if current structure meets acceptance criteria
    """
    if potList['noe'].violations() > 0:
        return False
    if potList['rdc'].rms() > 1.2: # This might be tightened some
        return False
    if potList['CDIH'].violations() > 0:
        return False
    if potList['BOND'].violations() > 0:
        return False
    if potList['ANGL'].violations() > 0:
        return False
    if potList['IMPR'].violations()> 1:
        return False
    
    return True

def calcOneStructure(loopInfo):
    """ 
    This function calculates a single structure, performs analysis on the
    structure, and then writes out a pdb file, with remarks.
    """

    # Initialize parameters for high temp dynamics.
    InitialParams( rampedParams )
    
    # High temp dynamics setup - only need to specify parameters which
    # differ from initial values in rampedParams
    InitialParams( highTempParams )

    # High temp dynamics
    protocol.initDynamics(dyn,
                          potList = potList, # potential terms to use
                          bathTemp = init_t,
                          initVelocities = 1,
                          finalTime = 10,    # stops at 10ps or 5000 steps
                          numSteps = 5000,   # whichever comes first
                          printInterval = 100)

    dyn.setETolerance( init_t/100 )  #used to det. stepsize. default: t/1000 
    dyn.run()

    # Initialize parameters for cooling loop
    InitialParams( rampedParams )

    # Initialize integrator for simulated annealing
    protocol.initDynamics(dyn,
                          potList = potList,
                          numSteps = 100,       # At each temp: 100 steps or
                          finalTime = .2,      # .2ps, whichever is less
                          printInterval = 100)

    # Perform simulated annealing
    cool.run()
    
    # Final torsion angle minimization
    protocol.initMinimize(dyn,
                          printInterval = 50)
    dyn.run()

    # Final all- atom minimization
    protocol.initMinimize(minc,
                          potList = potList,
                          dEPred = 10)
    minc.run()

    # Do analysis and write structure when this function returns True (?)
    pass

from simulationTools import StructureLoop, FinalParams

StructureLoop(numStructures = numberOfStructures,
              structLoopAction = calcOneStructure,
              calcMissingStructs = True, #calculate only missing structures
              doWriteStructures = True,  #analyze and write coords after calc
              genViolationStats = True,
              averagePotList = potList,
              averageSortPots = [potList['BOND'],potList['ANGL'],potList['IMPR'],
                                 noe,rdcs,potList['CDIH']],
              averageCrossTerms = refRMSD,
              averageTopFraction = 0.5, #report only on best 50% of structs
              averageAccept = accept,   #only use structures which pass accept()
              averageContext = FinalParams(rampedParams),
              averageFilename = "SCRIPT_ave.pdb",    #generate regularized ave structure
              averageFitSel = "name CA",
              averageCompSel = "not resname ANI and not name H*"     ).run()
