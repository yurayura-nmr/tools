#!/bin/csh

setenv NAME hncoca

bruk2pipe -in ser \
  -bad 0.0 -ext -aswap -AMX -decim 2040 -dspfvs 20 -grpdly 67.9862060546875  \
  -xN              1024  -yN                64  -zN                64  \
  -xT               512  -yT                32  -zT                32  \
  -xMODE            DQD  -yMODE  Echo-AntiEcho  -zMODE        Complex  \
  -xSW         9803.922  -ySW         2554.931  -zSW         4930.966  \
  -xOBS         700.183  -yOBS          70.957  -zOBS         176.070  \
  -xCAR           4.773  -yCAR         117.075  -zCAR          56.234  \
  -xLAB              HN  -yLAB             15N  -zLAB             13C  \
  -ndim               3  -aq2D          States                         \
  -out fid/${NAME}%03d.fid -verb -ov
