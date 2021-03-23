#!/usr/bin/env python3

"""
Chemical shift referencing using DSS according to:
Markley, J. L. et al. J. Biomol. NMR 12 (1), 1−23. (1998)

Erik Walinda
Kyoto University
Graduate School of Medicine

https://github.com/yurayura-nmr/tools
For Bruker TopSpin

Last change: 2021/03/23

    1. Record 1D spectrum of DSS in the same buffer (external)
       or the actual protein sample (internal) standard
       using 1D pulse with water suppression (e.g., p3919fpgp).
    2. Process the 1D in NMR draw and note the frequency of
       the DSS methyl resonance line (sharp peak around 0 ppm)
    3. Run this script in the folder containing the Bruker NMR 
       data, i.e., the acqu, acqus files etc. 
       When asked, plug in the DSS-methyl shift in ppm.
    4. The script will give you the new, referenced spectral
       centers in ppm. Use these in your NMRPipe fid.com file 
       and you will obtain properly referenced spectra.

    Note: This script assumes the 
          15N channel is on O3 and that the 
          13C channel is on O2.
"""

import os.path
import sys

from decimal import *

def main():
    fileCheck() # Check if we are in the right folder and get dimensionality
    
    SFO1, O1, SFO2, O2, SFO3, O3, DSS_ppm = getPars()
    calc(SFO1, O1, SFO2, O2, SFO3, O3, DSS_ppm)

def fileCheck():
    # Dimensionality and file check
    dimensions = 0

    if os.path.exists("./acqu") is True:
        dimensions += 1
    else:
        print ("NMR file [./acqu] not found!")
        print ("Please make sure you are in the correct directory.")
        sys.exit()

    if os.path.exists("./acqu2") is True:
        dimensions += 1

    if os.path.exists("./acqu3") is True:
        dimensions += 1

    print (str(dimensions) + "-dimensional experiment detected!")

def getPars():
    # Extract parameters from the Bruker files

    acqu = open('acqus')

    for line in acqu:
        if "SFO1" in line:
            a = line.split()
            SFO1 = float(a[1])
            print("Found SFO1 in acqus", SFO1)
        elif "$O1" in line:
            b = line.split()
            O1 = float(b[1])
            print("Found O1 in acqus", O1)
        elif "SFO3" in line:
            c = line.split()
            SFO3 = float(c[1])
            print("Found SFO3 in acqus", SFO3)
        elif "$O3" in line:
            d = line.split()
            O3 = float(d[1])
            print("Found O3 in acqus", O3)
        elif "SFO2" in line:
            e = line.split()
            SFO2 = float(e[1])
            print("Found SFO2 in acqus", SFO2)
        elif "$O2" in line:
            f = line.split()
            O2 = float(f[1])
            print("Found O2 in acqus", O2)

    print()
    DSS_ppm = input('Please enter the measured chemical shift of DSS in [ppm] ....  ')

    return SFO1, O1, SFO2, O2, SFO3, O3, DSS_ppm

def calc(SFO1, O1, SFO2, O2, SFO3, O3, DSS_ppm):
    getcontext().prec = 12

    # Using decimal for precision handling
    DSS_ppm = Decimal(DSS_ppm)
    DSS_Hz  = DSS_ppm * Decimal(1E-6)

    hydrogen_sf = Decimal(SFO1)
    carbon_sf   = Decimal(SFO2)
    nitrogen_sf = Decimal(SFO3)
    hydrogen_o1 = Decimal(O1)

    conversion_factor_hydrogen = Decimal(1)
    conversion_factor_carbon   = Decimal(0.251449530)
    conversion_factor_nitrogen = Decimal(0.101329118)  

    # Calculate: Carrier frequency (spectral center, proton)
    hydrogen_sf_Hz = hydrogen_sf * Decimal(1E6)             # Convert to Hz since O1 is read in as Hz but SFO1 is read in as MHz
    carrier = (hydrogen_sf_Hz - hydrogen_o1) / Decimal(1E6) # back to [MHz]

    # Calculate: How much is the measured DSS drifted from 0 ppm currently (proton)?
    carrier_Hz = carrier * Decimal(1E6)
    DSS_currentDrift_Hz = carrier_Hz * DSS_Hz               # in [Hz]
    DSS_shift_MHz = carrier + (DSS_currentDrift_Hz * Decimal(1E-6)) # in [MHz]

    # Proton, Carbon, Nitrogen - what are their true zero frequencies (to make the DSS signal come to 0.000 ppm in all dimensions)?
    zero_frequency_hydrogen = DSS_shift_MHz * conversion_factor_hydrogen
    zero_frequency_carbon   = DSS_shift_MHz * conversion_factor_carbon
    zero_frequency_nitrogen = DSS_shift_MHz * conversion_factor_nitrogen

    center_hydrogen = ((hydrogen_sf - zero_frequency_hydrogen) / zero_frequency_hydrogen) * Decimal(1E6) # in ppm
    center_carbon   = ((carbon_sf   - zero_frequency_carbon)   / zero_frequency_carbon)   * Decimal(1E6) # in ppm
    center_nitrogen = ((nitrogen_sf - zero_frequency_nitrogen) / zero_frequency_nitrogen) * Decimal(1E6) # in ppm
    
    # Done!
    print()
    print("Parameters")
    print()
    print("SFO1 [MHz] ...................... ", SFO1)
    print("O1 [Hz] ......................... ", O1)
    print("Carrier [MHz] ................... ", carrier)
    print("DSS drift from 0.00 ppm [Hz] .... ", DSS_currentDrift_Hz)
    print("Frequency of DSS at 0 ppm [MHz] . ", DSS_shift_MHz)

    print()
    print()
    print("DSS-referenced parameters")
    print()
    print(" 1H new center [ppm] ............ ", center_hydrogen)
    print("15N new center [ppm] ............ ", center_nitrogen)
    print("13C new center [ppm] ............ ", center_carbon)
    print()

main()
