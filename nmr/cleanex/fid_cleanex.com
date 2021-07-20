#!/bin/tcsh

bruk2pipe -in ./ser \
  -bad 0.0 -aswap -DMX -decim 2040 -dspfvs 20 -grpdly 67.9862060546875  \
  -xN              1024  -zN               256  -yN                 5  \
  -xT               512  -zT               128  -yT                 5  \
  -xMODE            DQD  -zMODE        Complex  -yMODE           Real  \
  -xSW         9803.922  -zSW         1986.492  -ySW                5  \
  -xOBS         700.183  -zOBS          70.957  -yOBS           1.000  \
  -xCAR           4.700  -zCAR         119.000  -yCAR           0.000  \
  -xLAB              HN  -zLAB             15N  -yLAB              ID  \
  -ndim               3  -aq2D          States                         \
| nmrPipe -fn TP \
| nmrPipe -fn ZTP \
| nmrPipe -fn TP -hyper \
| pipe2xyz -out ./fid/%d.fid -verb -ov
