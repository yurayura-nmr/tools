;hsqct1etf3gpsi3d.2
;avance-version (11/08/16)
;3D H-1/X correlation via double inept transfer
;   using sensitivity improvement
;for measuring N-15 T1 relaxation times
;phase sensitive using Echo/Antiecho-TPPI gradient selection
;with decoupling during acquisition
;using f3 - channel
;using flip-back pulse
;using H-1 180o pulses rather than cpd decoupling
;as pseudo3D
;(use parameterset HSQCT1ETF3GPSI3D.2)
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


define list<delay> vd_list=<$VDLIST>


"p2=p1*2"
"p22=p21*2"
"d11=30m"
"d24=1s/(cnst4*cnst11)"
"d25=1s/(cnst4*cnst12)"
"d26=1s/(cnst4*4)"


"d10=3u"

"plw16=plw3*pow((p21/pcpd3),2)"			; 15N decoupling power

"in10=inf2/2"

"DELTA2=p16+d16-p1*2/PI+de+8u"
"DELTA3=d25-p16-d16-4u"
"DELTA4=2.5m-p44/2"
"DELTA5=2.5m-p44/2-p16-d16-4u"
"DELTA6=d24-p19-d16"
"DELTA7=d26-p16-d16"
"DELTA8=d25-larger(p22,p44)/2"

#   ifdef LABEL_CN
"DELTA1=d25-p16-d16-larger(p8,p44)-d10*2"
#   else
"DELTA1=d25-p16-d16-p44-d10*2"
#   endif /*LABEL_CN*/



"spoff1=0"
"spoff13=bf2*((cnst21+cnst22)/2000000)-o2"
"spoff30=0"


"acqt0=0"
baseopt_echo


aqseq 312


1 ze
  d11 pl16:f3 st0
2 6m do:f3
3 6m
4 d1 pl1:f1
 
  "COUNTER=( (vd_list-10m) / 10m )"

  (p1 ph1)
  d26 pl3:f3
  (center (p2 ph1) (p22 ph1):f3 )
  d26 UNBLKGRAD
  (p1 ph2)

  4u
  (p11:sp1 ph1:r):f1
  4u
  p16:gp1
  d16

  (p21 ph3):f3
  DELTA8 
  (center (p44:sp30 ph1) (p22 ph1):f3  )
  DELTA8
  (p21 ph2):f3

  DELTA4
  (p44:sp30 ph10)
  DELTA4

6 DELTA4
  (p44:sp30 ph1)
  DELTA4
  DELTA4
  (p44:sp30 ph10)
  DELTA4
  lo to 6 times COUNTER

  DELTA4
  (p44:sp30 ph1)
  4u
  p16:gp4
  d16
  DELTA5

  (p21 ph8):f3
  4u
  p16:gp2*-1*EA
  d16
  DELTA3
  (p22 ph6):f3
  d10

#   ifdef LABEL_CN
  (center (p44:sp30 ph9) (p8:sp13 ph1):f2  )
#   else
  (p44:sp30 ph9)
#   endif /*LABEL_CN*/

  d10
  p16:gp2*EA
  d16
  DELTA1 pl1:f1

  (center (p1 ph1) (p21 ph4):f3 )
  p19:gp5
  d16
  DELTA6
  (center (p2 ph1) (p22 ph1):f3 )
  DELTA6
  p19:gp5
  d16
  (center (p1 ph2) (p21 ph5):f3 )
  p16:gp6
  d16
  DELTA7
  (center (p2 ph1) (p22 ph1):f3 )
  DELTA7
  p16:gp6
  d16
  (p1 ph1)
  DELTA2
  (p2 ph1)
  4u
  p16:gp3
  d16 pl16:f3
  4u  BLKGRAD
  goscnp ph31 cpd3:f3

  3m do:f3
  3m st vd_list.inc
  lo to 3 times nbl

  3m vd_list.res
  3m ipp3 ipp4 ipp5 ipp8 ipp9 ipp31
  lo to 4 times ns

  d1 mc #0 to 4
     F1QF()
     F2EA(calgrad(EA) & calph(ph5, +180) & exec(rppall), caldel(d10, +in10) & calph(ph6, +180) & calph(ph8, +180) & calph(ph31, +180))
exit
   

ph1=0 
ph2=1
ph3=0 0 0 0 2 2 2 2 
ph4=0 0 2 2
ph5=3 3 1 1
ph6=0
ph7=3
ph8=1 3
ph9=0 0 2 2
ph10=2
ph31=0 2 2 0 2 0 0 2
  

;pl1 : f1 channel - power level for pulse (default)
;pl3 : f3 channel - power level for pulse (default)
;pl16: f3 channel - power level for CPD/BB decoupling
;sp1 : f1 channel - shaped pulse  90 degree
;sp13: f2 channel - shaped pulse 180 degree  (Ca and C=O, adiabatic)
;sp30: f1 channel - shaped pulse 180 degree (Bip720,50,20.1)
;p1 : f1 channel -  90 degree high power pulse
;p2 : f1 channel - 180 degree high power pulse
;p8 : f2 channel - 180 degree shaped pulse for inversion (adiabatic)
;p11: f1 channel -  90 degree shaped pulse
;p16: homospoil/gradient pulse
;p19: gradient pulse 2                               [500 usec]
;p21: f3 channel -  90 degree high power pulse
;p22: f3 channel - 180 degree high power pulse
;p44: f1 channel - 180 degree shaped pulse
;d1 : relaxation delay; 1-5 * T1
;d10: incremented delay                              [3 usec]
;d11: delay for disk I/O                             [30 msec]
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
;inf2: 1/SW(X) = 2 * DW(X)
;in10: 1/(2 * SW(X)) = DW(X)
;nd10: 2
;NS: 1 * n
;DS: >= 16
;td1: number of delays in vd-list
;td2: number of experiments in F2
;NBL: = td1
;FnMODE: QF in F1
;FnMODE: echo-antiecho in F2
;cpd3: decoupling according to sequence defined by cpdprg3
;pcpd3: f3 channel - 90 degree pulse for decoupling sequence


;for z-only gradients:
;gpz1: 50%
;gpz2: 80%
;gpz3: 16.2%
;gpz4: 30%
;gpz5: 5%
;gpz6: -2%

;use gradient files:   
;gpnam1: SMSQ10.100
;gpnam2: SMSQ10.100
;gpnam3: SMSQ10.100
;gpnam4: SMSQ10.100
;gpnam5: SMSQ10.100
;gpnam6: SMSQ10.100


                                          ;preprocessor-flags-start
;LABEL_CN: for C-13 and N-15 labeled samples start experiment with 
;             option -DLABEL_CN (eda: ZGOPTNS)
                                          ;preprocessor-flags-end



;$Id: hsqct1etf3gpsi3d.2,v 1.14.2.1 2011/09/09 10:47:16 ber Exp $
