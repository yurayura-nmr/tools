;ks_hsqct2etf3gpsi3d
;
;avance-version (01/24/2018)
;3D H-1/X correlation via double inept transfer
;   using sensitivity improvement
;for measuring N-15 T2 relaxation times
;phase sensitive using Echo/Antiecho-TPPI gradient selection
;with decoupling during acquisition
;using f3 - channel
;using flip-back pulse
;as pseudo3D
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


define list<loopcounter> cpmglist=<$VCLIST>
define delay Heat


"p2=p1*2"
"p22=p21*2"

"plw0=0"
"plw16=plw3*pow((p21/pcpd3),2)"			; 15N decoupling power
"plw23=plw3*pow((p21/(p30/2)),2)"		; 15N CPMG power
"spw1=plw1*pow((p1/(p11*0.5889)),2)"		; Water-flip-back pulse power

"d10=3u"
"d11=30m"
"d21=500u-p30/2"
"d26=1s/(cnst4*4)"

"in10=inf2/2"

#   ifdef LABEL_CN
"DELTA1=d26-p16-d16-larger(p2,p8)-d10*2-p21*4/PI"
#   else
"DELTA1=d26-p16-d16-p2-d10*2-p21*4/PI"
#   endif /*LABEL_CN*/

"DELTA2=p16+d16+4u"
"DELTA3=d21-p1"
"DELTA4=d26-p16-d16"
"DELTA5=d26-p19-d16"





"nbl=td1"
"acqt0=0"
baseopt_echo
aqseq 312


1 ze
  d11 pl16:f3 st0
2 6m do:f3
3 6m
4 d11 pl23:f3
;******************************************** heat compensation
  "COUNTER=(l5-cpmglist)*4"
  "Heat=300ms-(p30*2+d21*4)*COUNTER"

5 d21
				(p30 ph0 d21*2 p30 ph0):f3
  d21
  lo to 5 times COUNTER

  Heat
  d1 UNBLKGRAD
;********************************************
				(p21 ph0):f3
  p16:gp6
  d16 pl1:f1
;******************************************** refocused INEPT
  (p1 ph0):f1
  d26 pl3:f3
  (center (p2 ph0):f1		(p22 ph0):f3 )
  d26
  (p1 ph1):f1
  4u pl0:f1
  (p11:sp1 ph8:r):f1
  4u
  p16:gp1
  d16 pl1:f1
				(p21 ph4):f3
  d26
  (center (p2 ph0):f1		(p22 ph0):f3 )
  d26 pl23:f3
;******************************************** CPMG
6 d21
				(p30 ph0 d21*2 p30 ph0):f3
  DELTA3
  (p2 ph0):f1
  DELTA3
				(p30 ph1 d21*2 p30 ph3):f3
  d21*2
				(p30 ph2 d21*2 p30 ph2):f3
  DELTA3
  (p2 ph0):f1
  DELTA3
				(p30 ph3 d21*2 p30 ph1):f3
  d21
  lo to 6 times cpmglist
;******************************************** End CPMG
;******************************************** 15N chemical shift evolution + INEPT
  p16:gp2*-1*EA
  d16
  DELTA4 pl3:f3
				(p22 ph7):f3
  d10 

#   ifdef LABEL_CN
  (center (p2 ph5):f1				(p8:sp13 ph0):f2 )
#   else
  (p2 ph5):f1
#   endif /*LABEL_CN*/

  d10
  p16:gp2*EA
  d16
  DELTA1
;******************************************** PEP
  (center (p1 ph0):f1		(p21 ph5):f3 )
  p19:gp4
  d16
  DELTA5
  (center (p2 ph0):f1		(p22 ph0):f3 )
  DELTA5
  p19:gp4
  d16
  (center (p1 ph1):f1		(p21 ph6):f3 )
  p16:gp5
  d16
  DELTA4
  (center (p2 ph0):f1		(p22 ph0):f3 )
  DELTA4
  p16:gp5
  d16
  (p1 ph0):f1
  DELTA2
  (p2 ph0):f1
  p16:gp3
  d16 pl16:f3
  4u  BLKGRAD

  goscnp ph31 cpd3:f3

  3m do:f3
  3m st cpmglist.inc
  lo to 3 times nbl

  3m cpmglist.res
  3m ipp4 ipp5 ipp6 ipp7 ipp31
  lo to 4 times ns

  d11 mc #0 to 4
     F1QF()
     F2EA(calgrad(EA) & calph(ph6, +180) & exec(rppall), caldel(d10, +in10) & calph(ph4, +180) & calph(ph31, +180))

exit


ph0=0
ph1=1
ph2=2
ph3=3
ph4=0 2 
ph5=0 0 2 2
ph6=3 3 1 1
ph7=0 0 0 0 2 2 2 2
ph8=0
ph31=0 2 2 0
  

;pl0 : 0W
;pl1 : f1 channel - power level for pulse (default)
;pl3 : f3 channel - power level for pulse (default)
;pl16: f3 channel - power level for CPD/BB decoupling
;pl23: f3 channel - power level for CPMG
;sp1 : f1 channel - shaped pulse  90 degree
;spnam1: Sinc1.1000
;sp13: f2 channel - shaped pulse 180 degree  (Ca and C=O, adiabatic)
;spnam13: Crp60,0.5,20.1
;p1 : f1 channel -  90 degree high power pulse
;p2 : f1 channel - 180 degree high power pulse
;p8 : f2 channel - 180 degree shaped pulse for inversion (adiabatic)
;p11: f1 channel -  90 degree shaped pulse
;p16: homospoil/gradient pulse
;p19: gradient pulse 2                               [500 usec]
;p21: f3 channel -  90 degree high power pulse
;p22: f3 channel - 180 degree high power pulse
;p30: f3 channel - 180 degree pulse at pl23
;d1 : relaxation delay; 1-5 * T1
;d10: incremented delay                              [3 usec]
;d11: delay for disk I/O                             [30 msec]
;d16: delay for homospoil/gradient recovery
;d20: wanted T2 delay
;d21: echo delay, d21 + p30/2 = 500u
;d26: 1/(4J(YH))
;d31: T2 delay as executed
;cnst4: = J(YH)
;vc : variable loop-coounter for T2 delay, taken from vc-list
;l5: max(vc)
;inf2: 1/SW(X) = 2 * DW(X)
;in10: 1/(2 * SW(X)) = DW(X)
;nd10: 2
;NS: 4 * n
;DS: >= 8
;td1: number of delays in vc-list
;td2: number of experiments in F2
;NBL: = td1
;FnMODE: QF in F1
;FnMODE: echo-antiecho in F2
;cpd3: decoupling according to sequence defined by cpdprg3
;pcpd3: f3 channel - 90 degree pulse for decoupling sequence


;use gradient ratio:	gp 1 : gp 2 : gp 3 : gp 4 : gp 5 : gp 6
;			  30 :   80 : 16.2 :    5 :   -2 :    6

;for z-only gradients:
;gpz1: 30%
;gpz2: 80%
;gpz3: 16.2%
;gpz4: 5%
;gpz5: -2%
;gpz6: 6%
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
