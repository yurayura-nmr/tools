;hsqcf3gpiaphwg.2
;avance-version (12/01/11)
;HSQC - IPAP
;2D H-1/X correlation via double inept transfer
;phase sensitive
;with decoupling during acquisition
;using f3 - channel
;water suppression using watergate sequence
;using similar delays for IP and AP
;
;M.Ottiger, F. Delaglio & A. Bax, J. Magn. Reson. 131, 373-378 (1998)
;
;$CLASS=HighRes
;$DIM=2D
;$TYPE=
;$SUBTYPE=
;$COMMENT=


prosol relations=<triple>


#include <Avance.incl>
#include <Delay.incl>
#include <Grad.incl>


"p2=p1*2"
"p22=p21*2"
"d11=30m"
"d26=1s/(cnst4*4)"


"d0=3u"

"in0=inf1/2"


"DELTA=p8+d0*2-p21*4/3.1416"
"DELTA1=d26-p16-d16"
"DELTA2=d21/2-p16-d16"
"DELTA3=d21/2-p16-d16-p1"
"DELTA4=d26-p16-p11-8u"

"TAU=p2"

"l0=1"


1 ze 
  d11 pl16:f3
2 d11 do:f3
3 d1 pl1:f1 pl3:f3
  50u UNBLKGRAD
  (p21 ph1):f3
  p16:gp1
  d16

  (p1 ph1)
  p16:gp2
  d16
  DELTA1
  (center (p2 ph1) (p22 ph1):f3 )
  DELTA1
  p16:gp2
  d16
  (p1 ph3)

  p16:gp3
  d16

  if "l0 %2 == 1"
     {
     (p21 ph4):f3
     p16:gp4
     d16
     DELTA2
     (center (TAU) (p22 ph6):f3 )
     DELTA2
     p16:gp4
     d16
     }
  else
     {
     (p21 ph5):f3
     p16:gp4
     d16
     DELTA2
     (center (p2 ph1) (p22 ph6):f3 )
     DELTA3
     p16:gp4
     d16
     (p1 ph7)
     }

  d0
  (p8:sp13 ph1):f2
  d0
  (p22 ph1):f3
  DELTA
  (p21 ph1):f3

  p16:gp5
  d16

  (p1 ph1) 
  p16:gp6
  DELTA4 pl0:f1
  (p11:sp1 ph8:r):f1
  4u
  4u pl1:f1
  (center (p2 ph1) (p22 ph1):f3 )
  4u pl0:f1
  (p11:sp1 ph8:r):f1
  p16:gp6
  DELTA4 pl16:f3
  4u BLKGRAD

  go=2 ph31 cpd3:f3

  d11 do:f3 mc #0 to 2 
     F1I(iu0, 2)
     F1PH(calph(ph4, +90) & calph(ph5, +90) & calph(ph6, +90), caldel(d0, +in0))
exit 
  

ph1=0
ph2=1
ph3=3 1
ph4=0 0 2 2
ph5=3 3 1 1
ph6=0 0 0 0 1 1 1 1 2 2 2 2 3 3 3 3
ph7=0 0 0 0 0 0 0 0 2 2 2 2 2 2 2 2
ph8=2
ph31=0 2 2 0 2 0 0 2


;pl0 : 0W
;pl1 : f1 channel - power level for pulse (default)
;pl3 : f3 channel - power level for pulse (default)
;pl16: f3 channel - power level for CPD/BB decoupling
;sp1: f1 channel - shaped pulse  90 degree
;sp13: f2 channel - shaped pulse 180 degree (adiabatic)
;p1 : f1 channel -  90 degree high power pulse
;p2 : f1 channel - 180 degree high power pulse
;p8 : f2 channel - 180 degree shaped pulse for inversion (adiabatic)
;p11: f1 channel -  90 degree shaped pulse
;p16: homospoil/gradient pulse
;p21: f3 channel -  90 degree high power pulse
;p22: f3 channel - 180 degree high power pulse
;d0 : incremented delay (2D)                         [3 usec]
;d1 : relaxation delay; 1-5 * T1
;d11: delay for disk I/O                             [30 msec]
;d16: delay for homospoil/gradient recovery
;d21: 1/(2J)NH                                       [5.3 msec]
;d26: 1/(4J)NH                                       [2.5 msec]
;cnst4: = J(NH)
;l0: flag to switch between inphase and antiphase 
;inf1: 1/SW(X) = 2 * DW(X)
;in0: 1/(2 * SW(X)) = DW(X)
;nd0: 2
;ns: 4 * n
;ds: 16
;td1: number of experiments
;FnMODE: States-TPPI, TPPI, States or QSEQ
;cpd3: decoupling according to sequence defined by cpdprg3
;pcpd3: f3 channel - 90 degree pulse for decoupling sequence


;use gradient ratio:    gp 1 : gp 2 : gp 3 : gp 4 : gp 5 : gp 6
;                         30 :    7 :   80 :    5 :   11 :   23

;for z-only gradients:
;gpz1: 30%
;gpz2: 7%
;gpz3: 80%
;gpz4: 5%
;gpz5: 11%
;gpz6: 23%

;use gradient files:   
;gpnam1: SMSQ10.100
;gpnam2: SMSQ10.100
;gpnam3: SMSQ10.100
;gpnam4: SMSQ10.100
;gpnam5: SMSQ10.100
;gpnam6: SMSQ10.100


;use AU-program split [ipap 2] to split data



;$Id: hsqcf3gpiaphwg.2,v 1.5.6.1 2012/01/31 17:56:32 ber Exp $
