;f1f2hncanh
;
;avance-version (05/23/2017)
;2D F1F2-selective HN(CA)NH
;2D sequence with
;   inverse correlation for triple resonance using multiple
;      inept transfer steps
;on/off resonance Ca and C=O pulses using shaped pulse
;phase sensitive (t1)
;phase sensitive using Echo/Antiecho-TPPI gradient selection (t1)
;using semi-constant time in t1
;
;      F1(H) -(CP)-> F2(N) -> F3(Ca) -> F2(N,t1) -> F1(H,t2)
;
;E. Walinda, D. Morimoto, M. Shirakawa, and K. Sugase, 
;  J. Biomol. NMR, 2017, 68 (1), 41-52
;
;$CLASS=HighRes
;$DIM=2D
;$TYPE=
;$SUBTYPE=
;$COMMENT=



#include <Avance.incl>
#include <Grad.incl>
#include <Delay.incl>


define pulse Pcp
"Pcp=10.8ms"
"p2=p1*2"
"p22=p21*2"
"p26=pcpd1"

"plw0=0"
"plw10=plw1*pow(p1/(250000/cnst9),2)"	; 1H CP power
"plw22=plw3*pow(p21/(250000/cnst9),2)"	; 15N CP power
"plw16=plw3*pow((p21/pcpd3),2)"		; 15N decoupling power
"plw19=plw1*pow((p1/pcpd1),2)"		; 1H decoupling power
"spw1=plw1*pow((p1/(p11*0.5889)),2)"	; Water-flip-back pulse power

"cnst5=sfo1*cnst19-o1*1000000"
"cnst6=sfo3*cnst0-o3*1000000"

"d11=30m"
"d21=5.5m"
"d23=12.4m"
"d26=2.3m"
"d27=12.5m"

"d0=d23/2-p14/2"
"d17=3u"
"d29=d23/2-p14/2-p26-d21-4u"
"d30=d23/2+p14/2+d17"

"in0=inf1/4"

"FACTOR2=d30*10000000*2/td1"
"in30=FACTOR2/10000000"

"if ( in30 > in0 ) { in17 = 0; } else { in17=in0-in30; }"
"if ( in30 > in0 ) { in30 = in0; }"

"in29=in0"

"DELTA=d21-p16-d16"
"DELTA1=p16+d16+4u"

"spoff2=0"
"spoff3=0"
"spoff5=bf2*(cnst21/1000000)-o2"
"spoff8=0"
"spoff9=0"

"acqt0=0"
baseopt_echo


1 ze
  d11 pl16:f3
2 d11 do:f3
3 d1 pl0:f1
  50u UNBLKGRAD
  (p11:sp1 ph2):f1
  4u
  4u pl1:f1 fq=cnst5 (sfo hz):f1
  (p1 ph0):f1
  4u pl10:f1 pl22:f3 fq=cnst6 (sfo hz):f3
  (Pcp ph1):f1				(Pcp ph0):f3	; cross polarization
  4u pl19:f1 fq=0 (sfo hz):f1 fq=0 (sfo hz):f3 pl3:f3
;*******************************************************
					(p21 ph1):f3
  p16:gp1
  d16
  (ralign (p26 ph3):f1			(p21 ph4):f3 )
  d23 cpds1:f1 ph0
  (center	(p14:sp3 ph0):f2	(p22 ph0):f3 )
  d23
					(p21 ph0):f3
  4u do:f1
  (p26 ph1):f1
  p16:gp4
  d16
  (p26 ph3):f1
  4u cpds1:f1 ph0
;******************************************************* N -> Ca
		(p13:sp2 ph0):f2
  d27
		(p14:sp5 ph0):f2
  d27
	(center (p24:sp9 ph0):f2	(p22 ph0):f3 )
  d27
  		(p14:sp5 ph0):f2
  d27
  		(p13:sp8 ph0):f2
;******************************************************* Ca -> N
  4u do:f1
  (p26 ph1):f1
  p16:gp5
  d16
  (p26 ph3):f1
  4u cpds1:f1 ph0
;******************************************************* 15N chemical shift evolution
 					(p21 ph5):f3	
  d30
  		(p14:sp5 ph0):f2
  d30
					(p22 ph8):f3
  d17
  		(p14:sp5 ph0):f2
  d17
		(p14:sp3 ph0):f2
  d0
  		(p14:sp5 ph0):f2
  d29
;*******************************************************
  4u do:f1
  (p26 ph1):f1
  p16:gp2*EA
  d16 
  DELTA pl1:f1
;******************************************************* PEP
  (center (p1 ph0):f1 			(p21 ph6):f3 )
  d26
  (center (p2 ph0):f1 			(p22 ph0):f3 )
  d26
  (center (p1 ph1):f1 			(p21 ph7):f3 )
  d26
  (center (p2 ph0):f1 			(p22 ph0):f3 )
  d26
  (p1 ph0):f1
  DELTA1
  (p2 ph0):f1
  p16:gp3
  d16 pl16:f3
  4u BLKGRAD

  go=2 ph31 cpd3:f3
  d11 do:f3 mc #0 to 2
     F1EA(calgrad(EA) & calph(ph7, +180), caldel(d0, +in0) & caldel(d17, +in17) & caldel(d29, +in29) & caldel(d30, -in30) & calph(ph5, +180) & calph(ph31, +180))
exit

ph0 = 0
ph1 = 1
ph2 = 2
ph3 = 3
ph4 = 1 3
ph5 = 0 
ph6 = 0 0 2 2
ph7 = 3 3 1 1
ph8 = 0 0 0 0 2 2 2 2
ph31= 0 2 2 0

;pl1 : f1 channel - power level for pulse (default)
;pl2 : f2 channel - power level for pulse (default)
;pl3 : f3 channel - power level for pulse (default)
;pl10: f1 channel - power level for CP
;pl16: f3 channel - power level for CPD/BB decoupling
;pl19: f1 channel - power level for CPD/BB decoupling
;pl22: f3 channel - power level for CP
;sp1: f1 channel - shaped pulse  90 degree  (H2O on resonance)
;sp2: f2 channel - shaped pulse  90 degree  (Ca on resonance)
;sp3: f2 channel - shaped pulse 180 degree  (Ca on resonance)
;sp5: f2 channel - shaped pulse 180 degree  (C=O off resonance)
;sp8: f2 channel - shaped pulse  90 degree  (Ca on resonance)
;                  for time reversed pulse
;sp9: f2 channel - shaped pulse 180 degree  (Ca on resonance)
;     sp9 requires higher selectivity - refocussing Calpha only
;p1 : f1 channel -  90 degree high power pulse
;p2 : f1 channel - 180 degree high power pulse
;p11: f1 channel -  90 degree shaped pulse
;p13: f2 channel -  90 degree shaped pulse
;p14: f2 channel - 180 degree shaped pulse
;p16: homospoil/gradient pulse                         [1 msec]
;p21: f3 channel -  90 degree high power pulse
;p22: f3 channel - 180 degree high power pulse
;p24: f2 channel - 180 degree shaped pulse (Ca, selective)
;p26: f1 channel -  90 degree pulse at pl19
;d0: incremented delay (F1 in 2D) = d23/2-p14/2
;d1 : relaxation delay; 1-5 * T1
;d11: delay for disk I/O                               [30 msec]
;d16: delay for homospoil/gradient recovery
;d17: incremented delay (F1 in 2D)                     [3 usec]
;d21: 1/(2J(NH))                                       [5.5 msec]
;d23: 1/(4J(NCa))                                      [12.4 msec]
;d26: 1/(4J'(NH))                                      [2.3 msec]
;d27: 1/(4J`(NCa))                                     [12.5 msec]
;d29: incremented delay (F1 in 2D) = d23/2-p14/2-p26-d21-4u
;d30: decremented delay (F1 in 2D) = d23/2+p14/2+d17
;cnst0: amide 15N in ppm
;cnst9: CP power in Hz
;cnst19: amide 1H in ppm
;cnst21: CO chemical shift (offset, in ppm)
;o2p: Calpha chemical shift
;inf1: 1/SW(N) = 2 * DW(N)
;in0: 1/(4 * SW(N)) = (1/2) DW(N)
;nd0: 4
;in17: = (1 - k2) * in0
;in29: = in0
;in30: = k2 * in0
;ns: 8 * n
;ds: >= 8
;td1: number of experiments in F1
;FnMODE: Echo-Antiecho in F1
;cpds1: decoupling according to sequence defined by cpdprg1
;cpds3: decoupling according to sequence defined by cpdprg3
;pcpd1: f1 channel - 90 degree pulse for decoupling sequence
;pcpd3: f3 channel - 90 degree pulse for decoupling sequence


;use gradient ratio:  gp 1 :gp 2 : gp 3 : gp 4 : gp 5
;                       30 :  80 :  8.1 :   50 :   23

;for z-only gradients:
;gpz1: 30%
;gpz2: 80%
;gpz3: 8.1%
;gpz4: 50%
;gpz5: 23%

;use gradient files:   
;gpnam1: SMSQ10.100
;gpnam2: SMSQ10.100
;gpnam3: SMSQ10.100
;gpnam4: SMSQ10.100
;gpnam5: SMSQ10.100
