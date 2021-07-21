#!/bin/csh

setenv NAME nnoesy

bruk2pipe -in ./ser \
  -bad 0.0 -ext -aswap -AMX -decim 2040 -dspfvs 20 -grpdly 67.9862060546875  \
  -xN              1024  -yN                48  -zN               128  \
  -xT               512  -yT                24  -zT                64  \
  -xMODE            DQD  -yMODE  Echo-AntiEcho  -zMODE        Complex  \
  -xSW         9803.922  -ySW         2483.855  -zSW         8417.508  \
  -xOBS         700.183  -yOBS          70.957  -zOBS         700.183  \
  -xCAR           4.773  -yCAR         117.575  -zCAR           4.773  \
  -xLAB              HN  -yLAB             15N  -zLAB              1H  \
  -ndim               3  -aq2D         Complex                         \
  -out fid/${NAME}%03d.fid -verb -ov
