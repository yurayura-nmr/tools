;ks_hncogp3d
;avance-version (12/01/11)
;HNCO
;3D sequence with
;   inverse correlation for triple resonance using multiple
;      inept transfer steps
;
;      F1(H) -> F3(N) -> F2(C=O,t1) -> F3(N,t2) -> F1(H,t3)
;
;on/off resonance Ca and C=O pulses using shaped pulse
;phase sensitive (t1)
;phase sensitive using Echo/Antiecho gradient selection (t2)
;using constant time in t2
;(use parameterset HNCOGP3D)
;
;S. Grzesiek & A. Bax, J. Magn. Reson. 96, 432 - 440 (1992)
;J. Schleucher, M. Sattler & C. Griesinger, Angew. Chem. Int. Ed. 32,
;   1489-1491 (1993)
;L.E. Kay, G.Y. Xu & T. Yamazaki, J. Magn. Reson. A109, 129-133 (1994)
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
"d13=4u"

"d21=5.5m"
"d23=12m"
"d26=2.3m"


"d0=3u"
"d10=d23/2-p14/2"
"d29=d23/2-p14/2-p26-d21-4u"
"d30=d23/2-p14/2"

"p26=pcpd1"
"plw0=0"
"plw16=plw3*pow((p21/pcpd3),2)"			; 15N decoupling power
"plw19=plw1*pow((p1/pcpd1),2)"			; 1H decoupling power
"spw1=plw1*pow((p1/(p11*0.5889)),2)"		; Water-flip-back pulse power
"spw2=plw2*pow((p3/(p13*0.0535693)),2)"		; 13C 90 G4
"spw3=plw2*pow((p3/((p14/2)*0.1514799)),2)"	; 13C 180 Q3
"spw5=spw3"					; 13C 180 Q3
"spw8=spw2"					; 13C 90 G4tr

"in0=inf1/2"
"in10=inf2/4"

"in29=in10"
"in30=in10"

"DELTA=d0*2+larger(p14,p22)-p14"
"DELTA1=p16+d16+d13+4u" 
"DELTA2=d23-d21-p26"
"DELTA3=d21-p16-d16-4u"
"DELTA4=d26-p19-d16"
"DELTA5=d26-p16-d16"

"cnst21=o2/bf2"

"spoff2=0"
"spoff3=0"
"spoff5=bf2*(cnst22/1000000)-o2"
"spoff8=0"


aqseq 321


1 d11 ze
  d11 pl16:f3 
2 d11 do:f3
3 d1 pl1:f1
  p1 ph1
  d26 pl3:f3
  (center (p2 ph1) (p22 ph1):f3 )
  d26 UNBLKGRAD
  (p1 ph2):f1 

  4u pl0:f1
  (p11:sp1 ph1:r):f1
  4u
  p16:gp1
  d16

  (p21 ph3):f3
  d21 pl19:f1
  (p26 ph2):f1
  DELTA2 cpds1:f1 ph1
  (center (p14:sp3 ph1):f2 (p22 ph1):f3 )
  d23
  (p21 ph1):f3

  d13 do:f1
  (p26 ph7):f1
  4u
  p16:gp2
  d16

  (p13:sp2 ph4):f2
  d0
  (center (p14:sp5 ph1):f2 (p22 ph1):f3 )
  d0
  4u
  (p14:sp3 ph1):f2
  DELTA
  (p14:sp5 ph1):f2
  4u
  (p13:sp8 ph1):f2

  4u
  p16:gp3
  d16
  (p26 ph2):f1
  20u cpds1:f1 ph1

  (p21 ph1):f3
  d30
  (p14:sp5 ph1):f2
  d30
  (center (p14:sp3 ph1):f2 (p22 ph8):f3 )
  d10
  (p14:sp5 ph1):f2
  d29
  4u do:f1
  (p26 ph7):f1
  4u
  p16:gp4*EA
  d16 
  DELTA3 pl1:f1

  (center (p1 ph1) (p21 ph5):f3 )
  p19:gp6
  d16
  DELTA4
  (center (p2 ph1) (p22 ph1):f3 )
  DELTA4
  p19:gp6
  d16
  (center (p1 ph2) (p21 ph6):f3 )
  p16:gp7
  d16
  DELTA5
  (center (p2 ph1) (p22 ph1):f3 )
  DELTA5
  p16:gp7
  d16
  (p1 ph1)
  DELTA1
  (p2 ph1)
  d13
  p16:gp5
  d16 pl16:f3
  4u BLKGRAD
  go=2 ph31 cpd3:f3 
  d11 do:f3 mc #0 to 2 
     F1PH(calph(ph4, +90), caldel(d0, +in0)) 
     F2EA(calgrad(EA) & calph(ph6, +180), caldel(d10, +in10) & caldel(d29, +in29) & caldel(d30, -in30))
exit


ph1=0
ph2=1 
ph3=0 0 0 0 0 0 0 0 2 2 2 2 2 2 2 2
ph4=0 2
ph5=0 0 2 2
ph6=3 3 1 1
ph7=3
ph8=0 0 0 0 2 2 2 2
ph31=0 2 2 0 0 2 2 0 2 0 0 2 2 0 0 2


;pl0 : 0W
;pl1 : f1 channel - power level for pulse (default)
;pl3 : f3 channel - power level for pulse (default)
;pl16: f3 channel - power level for CPD/BB decoupling
;pl19: f1 channel - power level for CPD/BB decoupling
;sp1: f1 channel - shaped pulse  90 degree  (H2O on resonance)
;sp2: f2 channel - shaped pulse  90 degree  (C=O on resonance)
;sp3: f2 channel - shaped pulse 180 degree  (C=O on resonance)
;sp5: f2 channel - shaped pulse 180 degree  (Ca off resonance)
;sp8: f2 channel - shaped pulse  90 degree  (C=O on resonance)
;                  for time reversed pulse
;p1 : f1 channel -  90 degree high power pulse
;p2 : f1 channel - 180 degree high power pulse
;p11: f1 channel -  90 degree shaped pulse             [2 msec]
;p13: f2 channel -  90 degree shaped pulse
;p14: f2 channel - 180 degree shaped pulse
;p16: homospoil/gradient pulse                         [1 msec]
;p19: gradient pulse 2                                 [500 usec]
;p21: f3 channel -  90 degree high power pulse
;p22: f3 channel - 180 degree high power pulse
;p26: f1 channel -  90 degree pulse at pl19
;d0 : incremented delay (F1 in 3D)                     [3 usec]
;d1 : relaxation delay; 1-5 * T1
;d10: incremented delay (F2 in 3D) =  d23/2-p14/2
;d11: delay for disk I/O                               [30 msec]
;d13: short delay                                      [4 usec]
;d16: delay for homospoil/gradient recovery
;d21: 1/(2J(NH)                                        [5.5 msec]
;d23: 1/(4J(NCO)                                       [12 msec]
;d26: 1/(4J'(NH)                                       [2.3 msec]
;d29: incremented delay (F2 in 3D) = d23/2-p14/2-p26-d21-4u
;d30: decremented delay (F2 in 3D) = d23/2-p14/2
;cnst21: CO chemical shift (offset, in ppm)
;cnst22: Calpha chemical shift (offset, in ppm)
;o2p: CO chemical shift (cnst21)
;inf1: 1/SW(CO) = 2 * DW(CO)
;inf2: 1/SW(N) = 2 * DW(N)
;in0: 1/(2 * SW(CO)) =  DW(CO)
;nd0: 2
;in10: 1/(4 * SW(N)) = (1/2) DW(N)
;nd10: 4
;in29: = in10
;in30: = in10
;ns: 8 * n
;ds: >= 16
;td1: number of experiments in F1
;td2: number of experiments in F2       td2 max = 2 * d30 / in30
;FnMODE: States-TPPI (or TPPI) in F1
;FnMODE: echo-antiecho in F2
;cpds1: decoupling according to sequence defined by cpdprg1
;cpd3: decoupling according to sequence defined by cpdprg3
;pcpd1: f1 channel - 90 degree pulse for decoupling sequence
;pcpd3: f3 channel - 90 degree pulse for decoupling sequence


;use gradient ratio:    gp 1 : gp 2 : gp 3 : gp 4 : gp 5 : gp 6 : gp 7
;                         60 :  -40 :   10 :   80 :  8.1 :    5 :   -2

;for z-only gradients:
;gpz1: 60%
;gpz2: -40%
;gpz3: 10%
;gpz4: 80%
;gpz5: 8.1%
;gpz6: 5%
;gpz7: -2%

;use gradient files:   
;gpnam1: SMSQ10.100
;gpnam2: SMSQ10.100
;gpnam3: SMSQ10.100
;gpnam4: SMSQ10.100
;gpnam5: SMSQ10.100
;gpnam6: SMSQ10.100
;gpnam7: SMSQ10.100
