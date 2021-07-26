;hsqcnoef3gpsi3d
;avance-version (13/12/11)
;3D H-1/X correlation via double inept transfer
;   using sensitivity improvement
;for measuring H1-N15 NOEs
;phase sensitive using Echo/Antiecho-TPPI gradient selection
;with decoupling during acquisition
;using f3 - channel
;recording NONOE and NOE as pseudo3D
;(use parameterset )
;
;$CLASS=HighRes
;$DIM=3D
;$TYPE=
;$SUBTYPE=
;$COMMENT=


prosol relations=<triple>


#include <Avance.incl>
#include <Grad.incl>
#include <Delay.incl>



"p2=p1*2"
"p22=p21*2"
"d11=30m"
"d12=20u"
"d24=1s/(cnst4*cnst11)"
"d25=1s/(cnst4*cnst12)"
"d26=1s/(cnst4*4)"

"p0=p1*4/3"


"d10=3u"

"in10=inf2/2"


"DELTA2=p16+d16-p1*2/PI+de+8u"
"DELTA3=d25-p16-d16-4u"
"DELTA4=d24-p19-d16"
"DELTA5=d26-p16-d16"

#   ifdef LABEL_CN
"DELTA1=d25-p16-d16-larger(p2,p8)-d10*2"
#   else
"DELTA1=d25-p16-d16-p2-d10*2"
#   endif /*LABEL_CN*/


"l4=d1/(p0+5m)"
"l11=0"


"spoff1=0"
"spoff13=bf2*((cnst21+cnst22)/2000000)-o2"


"acqt0=0"
baseopt_echo


aqseq 312


1 ze
  d11 pl1:f1 pl16:f3
2 d11 do:f3 
3 d12

  if "l11 %2 == 0"
     {
     d1
     50u UNBLKGRAD
     4u
     (p11:sp1 ph1:r):f1
     4u
     4u pl1:f1
     }
  else
     {
4    (p0 ph1)
     5m
     lo to 4 times l4
     4u
     50u UNBLKGRAD
     }

  (p1 ph1)
  p16:gp1
  d16 pl3:f3

  (p21 ph8):f3
  4u
  p16:gp2*-1*EA
  d16
  DELTA3
  (p22 ph6):f3
  d10 

#   ifdef LABEL_CN
  (center (p2 ph9) (p8:sp13 ph1):f2 )
#   else
  (p2 ph9)
#   endif /*LABEL_CN*/

  d10
  p16:gp2*EA
  d16
  DELTA1

  (center (p1 ph1) (p21 ph4):f3 )
  p19:gp4
  d16
  DELTA4
  (center (p2 ph1) (p22 ph1):f3 )
  DELTA4
  p19:gp4
  d16
  (center (p1 ph2) (p21 ph5):f3 )
  p16:gp5
  d16
  DELTA5
  (center (p2 ph1) (p22 ph1):f3 )
  DELTA5
  p16:gp5
  d16
  (p1 ph1)
  DELTA2
  (p2 ph1)
  4u
  p16:gp3
  d16 pl16:f3
  4u  BLKGRAD
  go=2 ph31 cpd3:f3
  d11 do:f3 mc #0 to 2
     F1QF(calclc(l11, 1))
     F2EA(calgrad(EA) & calph(ph5, +180), caldel(d10, +in10) & calph(ph6, +180) & calph(ph8, +180) & calph(ph31, +180))
exit
   

ph1=0 
ph2=1
ph4=0 0 2 2
ph5=3 3 1 1
ph6=0
ph7=3
ph8=1 3 
ph9=0 0 2 2
ph31=0 2 2 0 
  

;pl1 : f1 channel - power level for pulse (default)
;pl3 : f3 channel - power level for pulse (default)
;pl16: f3 channel - power level for CPD/BB decoupling
;sp1 : f1 channel - shaped pulse  90 degree  (H2O on resonance)
;sp13: f2 channel - shaped pulse 180 degree  (Ca and C=O, adiabatic)
;p0 : f1 channel - 120 degree high power pulse
;p1 : f1 channel -  90 degree high power pulse
;p2 : f1 channel - 180 degree high power pulse
;p8 : f2 channel - 180 degree shaped pulse for inversion (adiabatic)
;p11: f1 channel -  90 degree shaped pulse           [1 msec]
;p16: homospoil/gradient pulse
;p19: gradient pulse 2                               [500 usec]
;p21: f3 channel -  90 degree high power pulse
;p22: f3 channel - 180 degree high power pulse
;d1 : relaxation delay; 1-5 * T1
;d10: incremented delay (F2 in 3D)                   [3 usec]
;d11: delay for disk I/O                             [30 msec]
;d12: delay for power switching                      [20 usec]
;d16: delay for homospoil/gradient recovery
;d24: 1/(4J)YH for YH
;     1/(8J)YH for all multiplicities
;d25: 1/(4J)YH for YH
;     1/(8J)YH for all multiplicities
;d26: 1/(4J(YH))
;cnst4: = J(YH)
;cnst11: for multiplicity selection = 4 for NH, 8 for all multiplicities
;cnst12: for multiplicity selection = 4 for NH, 8 for all multiplicities
;cnst21: CO chemical shift (offset, in ppm)
;cnst22: Calpha chemical shift (offset, in ppm)
;l4: loop for saturation sequence: = d1 / (p0 + 5m)
;inf2: 1/SW(X) = 2 * DW(X)
;in10: 1/(2 * SW(X)) = DW(X)
;nd10: 2
;ns: 4 * n
;ds: >= 16
;td1: = 2
;td2: number of experiments in F2
;FnMODE: QF in F1
;FnMODE: echo-antiecho in F2
;cpd3: decoupling according to sequence defined by cpdprg3
;pcpd3: f3 channel - 90 degree pulse for decoupling sequence


;for z-only gradients:
;gpz1: 50%
;gpz2: 80%
;gpz3: 16.2%
;gpz4: 5%
;gpz5: -2%


;use gradient files:
;gpnam1: SMSQ10.100
;gpnam2: SMSQ10.100
;gpnam3: SMSQ10.100
;gpnam4: SMSQ10.100
;gpnam5: SMSQ10.100


                                          ;preprocessor-flags-start
;LABEL_CN: for C-13 and N-15 labeled samples start experiment with 
;             option -DLABEL_CN (eda: ZGOPTNS)
                                          ;preprocessor-flags-end



;$Id: hsqcnoef3gpsi3d,v 1.1.2.4 2014/01/17 15:19:37 ber Exp $
