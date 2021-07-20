#!/bin/csh

bruk2pipe -in ./ser \
  -bad 0.0 -ext -aswap -AMX -decim 2376 -dspfvs 20 -grpdly 67.9872589111328  \
  -xN              1024  -zN                30  -yN                 4  \
  -xT               512  -zT                30  -yT                 2  \
  -xMODE            DQD  -zMODE           Real  -yMODE  Echo-AntiEcho  \
  -xSW         8417.508  -zSW               30  -ySW         2341.920  \
  -xOBS         700.183  -zOBS           1.000  -yOBS          70.957  \
  -xCAR           4.700  -zCAR           0.000  -yCAR         118.000  \
  -xLAB              HN  -zLAB              ID  -yLAB             15N  \
  -ndim               3  -aq2D         Complex                         \
  -out ./fid/%d.fid -verb -ov
