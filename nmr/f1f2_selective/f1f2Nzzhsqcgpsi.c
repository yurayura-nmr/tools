;f1f2Nzzhsqcgpsi
;
;avance-version (02/04/2020)
;2D H-1/N-15 cross polarization
;   using sensitivity improvement
;phase sensitive using Echo/Antiecho-TPPI gradient selection (t1)
;with decoupling during acquisition
;ZZ-exchange
;
;      F2(H) -(CP)-> F1(N) -(ZZ-exchange)-> F1(N,t1) -> F2(H,t2)
;
;M. Nishizawa, E. Walinda, D. Morimoto, and K. Sugase, 
;
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

"plw0=0"
"plw10=plw1*pow(p1/(250000/cnst9),2)"		; 1H CP power
"plw22=plw3*pow(p21/(250000/cnst9),2)"		; 15N CP power
"plw16=plw3*pow((p21/pcpd3),2)"			; 15N decoupling power
"spw1=plw1*pow((p1/(p11*0.5889)),2)"		; Water-flip-back pulse power
"spw12=plw1*pow((p1/(p12*0.07981016/2)),2)"     ; amide proton selective 180 pulse

"cnst5=sfo1*cnst19-o1*1000000"

"d0=3u"
"d11=30m"
"d26=1s/(cnst4*4)"
"d31=10ms*l2+p16+d16"

"in0=inf1/2"

"DELTA=d26-p16-d16"

#   ifdef LABEL_CN
"DELTA1=d26-p16-d16-larger(p2,p8)-d0*2-8u"
"spw13=plw2*pow((p3/((p8/2)*0.1023327)),2)"	; 13C 180 adiabatic pulse power
#   else
"DELTA1=d26-p16-d16-p2-d0*2-8u"
#   endif /*LABEL_CN*/

"DELTA2=d26-p19-d16"
"DELTA3=d26-p16-d16"
"DELTA4=p16+d16+4u"
"TAU=2.5m-p12/2"


"acqt0=0"
baseopt_echo


1 ze
  d11 pl16:f3
2 d11 do:f3
  d1 pl0:f1
  50u UNBLKGRAD
  (p11:sp1 ph2):f1
  4u
  4u pl1:f1 fq=cnst5 (sfo hz):f1
  (p1 ph0):f1
  4u pl10:f1 pl22:f3
  (Pcp ph7):f1			(Pcp ph8):f3		; cross polarization
  4u pl3:f3
				(p21 ph1):f3

;******************************************** ZZ-exchange
  p16:gp1
  d16 pl0:f1 fq=0 (sfo hz):f1
  if "l2==0" goto 4
3 TAU
  (p12:sp12 ph0):f1
  TAU
  TAU
  (p12:sp12 ph0):f1
  TAU
  lo to 3 times l2
4				(p21 ph5):f3
;*******************************************************  15N chemical shift evolution + INEPT
  p16:gp2*-1*EA
  d16 pl1:f1
  DELTA
				(p22 ph4):f3
  d0 gron0*-1
  4u groff

#   ifdef LABEL_CN
  (center (p2 ph0):f1				(p8:sp13 ph0):f2 )
#   else
  (p2 ph0):f1
#   endif /*LABEL_CN*/

  d0 gron0
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
  d11 do:f3 mc #0 to 2
     F1EA(calgrad(EA) & calph(ph6, +180), caldel(d0, +in0) & calph(ph5, +180) & calph(ph31, +180))
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
;pl22: f3 channel - power level for CP
;sp1:  f1 channel - shaped pulse  90 degree
;spnam1: Sinc1.1000
;sp12: f1 channel - shaped pulse 180 degree
;spnam12: Reburp.1000
;sp13: f2 channel - shaped pulse 180 degree (adiabatic)
;spnam13: Crp60,0.5,20.1
;p1 : f1 channel -  90 degree high power pulse
;p2 : f1 channel - 180 degree high power pulse
;p3 : f2 channel -  90 degree high power pulse
;p8 : f2 channel - 180 degree shaped pulse for inversion (adiabatic)
;p11: f1 channel -  90 degree selective pulse
;p12: f1 channel - 180 degree shaped pulse             [3 msec]
;p21: f3 channel -  90 degree high power pulse
;p22: f3 channel -  90 degree high power pulse
;p16: homospoil/gradient pulse                         [1 msec]
;p19: gradient pulse 2                                 [500 usec]
;d1 : relaxation delay; 1-5 * T1
;d8 : mixing time
;d11: delay for disk I/O                               [30 msec]
;d16: delay for homospoil/gradient recovery
;d26: 1/(4J(NH))
;d31: actual mixingi time
;cnst4: = J(NH)
;cnst9: CP power in Hz
;cnst19: amide 1H in ppm
;l2: ZZ-exchange cycles, Mixing time = 10ms x l2
;o3p: amide 15N in ppm
;inf1: 1/SW(N) = 2 * DW(N)
;in0: 1/(2 * SW(N)) = DW(N)
;nd0: 2
;ns: 4 * n
;ds: >= 8
;td1: number of experiments in F1
;FnMODE: Echo-Antiecho in F1
;cpd3: decoupling according to sequence defined by cpdprg3
;pcpd3: f3 channel - 90 degree pulse for decoupling sequence


;use gradient ratio:  gp 0 : gp 1 : gp 2 : gp 3 : gp 4 : gp 5
;                      0.2 :   30 :   80 : 16.2 :    5 :   -2

;for z-only gradients:
;gpz0: 0.2%
;gpz1:  30%
;gpz2:  80%
;gpz3:16.2%
;gpz4:   5%
;gpz5:  -2%

;use gradient files:   
;gpnam1: SMSQ10.100
;gpnam2: SMSQ10.100
;gpnam3: SMSQ10.100
;gpnam4: SMSQ10.50
;gpnam5: SMSQ10.100

                                          ;preprocessor-flags-start
;LABEL_CN: for C-13 and N-15 labeled samples start experiment with 
;             option -DLABEL_CN (eda: ZGOPTNS)
                                          ;preprocessor-flags-end
