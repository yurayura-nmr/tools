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
