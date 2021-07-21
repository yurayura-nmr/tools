#!/bin/csh

bruk2pipe -verb -in ./ser \
  -bad 0.0 -ext -aswap -AMX -decim 2376 -dspfvs 20 -grpdly 67.9872589111328  \
  -xN              1024  -yN                30  -zN                 4  \
  -xT               512  -yT                30  -zT                 2  \
  -xMODE            DQD  -yMODE           Real  -zMODE  Echo-AntiEcho  \
  -xSW         8417.508  -ySW         7002.801  -zSW         2341.920  \
  -xOBS         700.183  -yOBS         700.183  -zOBS          70.957  \
  -xCAR           4.700  -yCAR           4.700  -zCAR         118.000  \
  -xLAB              HN  -yLAB              1H  -zLAB             15N  \
  -ndim               3  -aq2D         Complex                         \
| nmrPipe -fn TP \
| nmrPipe -fn ZTP \
| nmrPipe -fn TP \
| pipe2xyz -x -out fid/test%03d.fid

basicFT2.com -pseudo3D -in fid/test%03d.fid -out ft/test%03d.ft2 -xP0 88 -yP0 -90

chmod +x nmr.com
./nmr.com

# From: https://groups.io/g/nmrpipe/topic/71773980?p=Created,,,20,2,0,0
# Currently ultralow resolution for setup only.

# CCPN has trouble reading these data cause it thinks the ft2 files are 3D.
# Can trick it into reading by making 2D projections of the 2D data... 
# i.e.: proj3D.tcl -in test001.ft2 test_corrected_2D.ft2
# or  : sethdr test001.ft2 -ndim 2
#       showhdr test001.ft2
# Dimensions should have decreased from 3 to 2 and CCPN should be able to open it now. 
