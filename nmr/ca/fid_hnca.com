#!/bin/csh

setenv NAME hnca

bruk2pipe -in ser \
  -bad 0.0 -ext -aswap -AMX -decim 2040 -dspfvs 20 -grpdly 67.9862060546875  \
  -xN              1024  -yN                48  -zN                56  \
  -xT               512  -yT                24  -zT                28  \
  -xMODE            DQD  -yMODE  Echo-AntiEcho  -zMODE        Complex  \
  -xSW         9803.922  -ySW         2554.931  -zSW         4930.966  \
  -xOBS         700.183  -yOBS          70.957  -zOBS         176.070  \
  -xCAR           4.773  -yCAR         117.575  -zCAR          53.500  \
  -xLAB              HN  -yLAB             15N  -zLAB             13C  \
  -ndim               3  -aq2D          States                         \
  -out fid/${NAME}%03d.fid -verb -ov
