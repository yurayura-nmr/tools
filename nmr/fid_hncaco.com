#!/bin/csh

setenv NAME hncaco

bruk2pipe -in ser \
  -bad 0.0 -ext -aswap -AMX -decim 2040 -dspfvs 20 -grpdly 67.9862060546875  \
  -xN              1024  -yN                64  -zN                62  \
  -xT               512  -yT                32  -zT                31  \
  -xMODE            DQD  -yMODE  Echo-AntiEcho  -zMODE        Complex  \
  -xSW         9803.922  -ySW         2554.931  -zSW         1761.184  \
  -xOBS         700.183  -yOBS          70.957  -zOBS         176.091  \
  -xCAR           4.773  -yCAR         117.575  -zCAR         177.206  \
  -xLAB              HN  -yLAB             15N  -zLAB             13C  \
  -ndim               3  -aq2D          States                         \
  -out fid/${NAME}%03d.fid -verb -ov
