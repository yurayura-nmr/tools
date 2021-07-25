;f1f2Nonr1rho4d
;
;avance-version (07/15/2017)
;pseudo-3D F1F2(1H-15N)-selective N15 on-resonance R1rho relaxation dispersion
;   using sensitivity improvement
;phase sensitive using Echo/Antiecho-TPPI gradient selection (t1)
;with decoupling during acquisition
;using f3 - channel
;acquisition as pseudo-4D
;
;      F1(H) -(CP)-> F2(N) -> spin-lock(N) -> F2(N,t1) -> F1(H,t2)
;
;E. Walinda, D. Morimoto, M. Shirakawa, and K. Sugase, 
;  J. Biomol. NMR, 2017, 68 (1), 41-52
;
;$CLASS=HighRes
;$DIM=3D
;$TYPE=
;$SUBTYPE=
;$COMMENT=
;
;ignore the warning regarding va15N



#include <Avance.incl>
#include <Grad.incl>
#include <Delay.incl>

define list<power> va15N=<$VALIST>
define list<frequency> fq1H=<$FQ1LIST>
define list<frequency> fq15N=<$FQ2LIST>
define list<pulse> vp1H=<$VPLIST>

define pulse Pcp
define delay Teq
"Pcp=10.8ms"
"Teq=5m"

"cnst2=3900"					; 1H decoupling power [Hz]

"p2=p1*2"
;"p5=(PI/2-atan(abs(cnst19-o1/bf1)*bf1/cnst2))*p1/(PI/2)"
"p22=p21*2"

"plw0=0"
"plw10=plw1*pow(p1/(250000/cnst9),2)"		; 1H CP power
"plw22=plw3*pow(p21/(250000/cnst9),2)"		; 15N CP power
"plw19=plw1*pow(p1/(250000/cnst2),2)"		; 1H decoupling power
"plw16=plw3*pow((p21/pcpd3),2)"			; 15N decoupling power
;"cnst10=250000/p21*sqrt(plw3/plw25)"
;"plw26=plw3*pow(p21/(250000/(cnst11-cnst10)),2)"; 15N heating compensation
"spw1=plw1*pow((p1/(p11*0.5889)),2)"		; Water-flip-back pulse power

;"cnst5=bf1*cnst19-o1*1000000"

"d10=3u"
"d11=30m"
"d26=1s/(cnst4*4)"

"in10=inf3/2"

"l11=0"
"l12=0"

"DELTA=d26-p16-d16"

#   ifdef LABEL_CN
"DELTA1=d26-p16-d16-larger(p2,p8)-d10*2-8u"
"spw13=plw2*pow((p3/((p8/2)*0.1023327)),2)"	; 13C 180 adiabatic pulse power
#   else
"DELTA1=d26-p16-d16-p2-d10*2-8u"
#   endif /*LABEL_CN*/

"DELTA2=d26-p19-d16"
"DELTA3=d26-p16-d16"
"DELTA4=p16+d16+4u"
"DELTA5=Teq-p16-d16-vp1H-8u"

"acqt0=0"
baseopt_echo

;aqseq 312


1 ze
  d11 pl16:f3
2 d11 do:f3

  "fq1H.idx=(l12)"
  "fq15N.idx=(l12)"
  "vp1H.idx=(l12)"

  "va15N.idx=(l11*2)"

  d1 pl19:f1 fq=300 (bf ppm):f1 pl3:f3 fq15N:f3 va15N.inc
;*******************************************************  1H & 15N heating compensation
  if "va15N==1000"
  {
	4u cw:f1 va15N.dec
  }
  else
  {
	4u va15N.dec
  }
				(p21 ph3):f3
  4u va15N:f3 va15N.inc
				(p25 ph0):f3
  4u do:f1
;*******************************************************  end heating compensation
  50u UNBLKGRAD
  p16:gp6
  d16 pl0:f1 fq=0 (sfo hz):f1
  (p11:sp1 ph2):f1
  4u
  4u pl1:f1 fq1H:f1
  (p1 ph0):f1
  4u pl10:f1 pl22:f3
  (Pcp ph7):f1			(Pcp ph8):f3		; cross polarization
  4u pl1:f1 pl3:f3
				(p21 ph1):f3
  p16:gp1
  d16
  (vp1H ph1):f1
  4u pl19:f1
  4u cw:f1 ph0
  DELTA5
  				(p21 ph1):f3
;*******************************************************  Begin spinlock
  4u va15N:f3
  if "va15N==1000" goto 5
				(p25 ph0):f3
;*******************************************************  End spinlock
5 4u pl3:f3
				(p21 ph3):f3
  DELTA5
  4u do:f1
  4u pl1:f1
  (vp1H ph3):f1
  p16:gp7
  d16 fq=0 (sfo hz):f1
				(p21 ph5):f3
;*******************************************************  15N chemical shift evolution + INEPT
  p16:gp2*-1*EA
  d16 pl1:f1
  DELTA
				(p22 ph4):f3
  d10 gron0
  4u groff

#   ifdef LABEL_CN
  (center (p2 ph0):f1				(p8:sp13 ph0):f2 )
#   else
  (p2 ph0):f1
#   endif /*LABEL_CN*/

  d10 gron0*-1
  4u groff
  p16:gp2*EA
  d16
  DELTA1
;*******************************************************  PEP
  (center (p1 ph2):f1		(p21 ph0):f3 )
  p19:gp4
  d16
  DELTA2
  (center (p2 ph3):f1		(p22 ph0):f3 )
  DELTA2
  p19:gp4
  d16
  (center (p1 ph3):f1		(p21 ph6):f3 )
  p16:gp5
  d16
  DELTA3
  (center (p2 ph3):f1		(p22 ph3):f3 )
  DELTA3
  p16:gp5
  d16
  (p1 ph0):f1
  DELTA4
  (p2 ph0):f1
  p16:gp3
  d16 pl16:f3
  4u  BLKGRAD

  go=2 ph31 cpds3:f3
  d11 mc #0 to 2
     F1QF(calclc(l12, 1))
     F2QF(calclc(l11, 1))
     F3EA(calgrad(EA) & calph(ph6, +180), caldel(d10, +in10) & calph(ph5, +180) & calph(ph31, +180))

exit

ph0 = 0
ph1 = 1
ph2 = 2
ph3 = 3
ph4 = 0 0 0 0 2 2 2 2
ph5 = 1
ph6 = 3
ph7 = 1 3
ph8 = 0 0 2 2
ph31= 0 2 2 0

;pl1 : f1 channel - power level for pulse (default)
;pl2 : f2 channel - power level for pulse (default)
;pl3 : f3 channel - power level for pulse (default)
;pl10: f1 channel - power level for CP
;pl16: f3 channel - power level for CPD/BB decoupling
;pl19: f1 channel - power level for CPD/BB decoupling
;pl22: f3 channel - power level for CP
;pl25: f3 channel - power level for R1rho spinlock (valist)
;pl26: f3 channel - power level for heat compensation (valist)
;sp1:  f1 channel - shaped pulse  90 degree
;spnam1: Sinc1.1000
;sp13: f2 channel - shaped pulse 180 degree (adiabatic)
;spnam13: Crp60,0.5,20.1
;p1 : f1 channel -  90 degree high power pulse
;p2 : f1 channel - 180 degree high power pulse
;p3 : f2 channel -  90 degree high power pulse
;p8 : f2 channel - 180 degree shaped pulse for inversion (adiabatic)
;p11: f1 channel -  90 degree selective pulse
;p16: homospoil/gradient pulse                         [1 msec]
;p19: gradient pulse 2                                 [500 usec]
;p21: f3 channel -  90 degree high power pulse
;p22: f3 channel - 180 degree high power pulse
;p25: f3 channel - R1rho spinlock length
;d1 : relaxation delay; 1-5 * T1
;d11: delay for disk I/O                               [30 msec]
;d16: delay for homospoil/gradient recovery
;d26: 1/(4J(YH))
;cnst4: = J(YH)
;cnst9: CP power in Hz
;cnst10: spin-lock power in Hz
;cnst11: highest spin-lock power in Hz
;cnst19: amide 1H in ppm
;inf3: 1/SW(X) = 2 * DW(X)
;in10: 1/(2 * SW(X)) = DW(X)
;nd10: 2
;o3p: amide 15N in ppm
;nbl: number of powers in valist
;ns: 4 * n
;ds: >= 16
;td1: number of frequencies in fq-list
;td2: (number of powers)/2 in valist
;td3: number of experiments in F3
;FnMODE: QF in F1
;FnMODE: QF in F2
;FnMODE: Echo-Antiecho in F3
;cpd3: decoupling according to sequence defined by cpdprg3
;pcpd3: f3 channel - 90 degree pulse for decoupling sequence


;use gradient ratio:  gp 0 :gp 1 : gp 2 : gp 3 : gp 4 : gp 5 : gp 6 : gp 7
;                      0.2 :  30 :   80 : 16.2 :    5 :   -2 :    6 :  -60

;for z-only gradients:
;gpz0: 0.2%
;gpz1:  30%
;gpz2:  80%
;gpz3:16.2%
;gpz4:   5%
;gpz5:  -2%
;gpz6:   6%
;gpz7: -60%

;use gradient files:   
;gpnam1: SMSQ10.100
;gpnam2: SMSQ10.100
;gpnam3: SMSQ10.100
;gpnam4: SMSQ10.50
;gpnam5: SMSQ10.100
;gpnam6: SMSQ10.100
;gpnam7: SMSQ10.100

                                          ;preprocessor-flags-start
;LABEL_CN: for C-13 and N-15 labeled samples start experiment with 
;             option -DLABEL_CN (eda: ZGOPTNS)
                                          ;preprocessor-flags-end
