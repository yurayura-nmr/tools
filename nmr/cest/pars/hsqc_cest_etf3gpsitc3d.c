;hsqc_cest_etf3gpsitc3d
;avance-version (15/09/22)
;3D H-1/X correlation via double inept transfer
;   using sensitivity improvement
;for measuring N-15 chemical exchange by saturation transfer
;phase sensitive using Echo/Antiecho-TPPI gradient selection
;with decoupling during acquisition
;using f3 - channel
;using flip-back pulse
;as pseudo3D
;first experiment is used as reference
;(use parameterset )
;
;P. Vallurupalli, G. Bouvignies & L.E. Kay, 
;   J. Am. Chem. Soc. 134, 8148-8161 (2012)
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


define list<frequency> f3_list=<$FQ3LIST>
define list<frequency> f3_zero={sfo hz, 0.0}


"p2=p1*2"
"p22=p21*2"
"d11=30m"
"d24=1s/(cnst4*cnst11)"
"d25=1s/(cnst4*cnst12)"
"d26=1s/(cnst4*4)"


"d10=3u"

"in10=inf2/2"


"l11=0"

"DELTA2=p16+d16-p1*2/3.1416+de+8u"
"DELTA3=d25-p16-d16-4u"
"DELTA6=d24-p19-d16"
"DELTA7=d26-p16-d16"
"DELTA8=d25-larger(p2,p22)/2"

#   ifdef LABEL_CN
"DELTA1=d25-p16-d16-larger(p8,p44)-d10*2-4u"
#   else
"DELTA1=d25-p16-d16-p44-d10*2-4u"
#   endif /*LABEL_CN*/

"TAU=d31-d21"

"plw33= plw3*pow(cnst9*4*p21*0.000001,2)"	


"spoff1=0"
"spoff13=bf2*((cnst21+cnst22)/2000000)-o2"
"spoff30=0"


"acqt0=0"
baseopt_echo


aqseq 312


1 ze
  d11 pl16:f3
2 d11 do:f3
  
  20u f3_zero:f3
  20u
  if "l11 == 0"
     { 
     5u 
     "TAU=d31" 
     }
  else 
     { 
     5u
     "f3_list.idx=(l11-1)"
     5u
     "TAU=d31-d21" 
     }

  20u pl19:f1
  4u cpds8:f1
  TAU
  4u do:f1

  d1 pl1:f1 pl3:f3
  50u UNBLKGRAD
  (p21 ph1):f3
  p16:gp7
  d16

  (p1 ph1)
  d26
  (center (p2 ph1) (p22 ph1):f3 )
  d26
  (p1 ph2)

  p16:gp1
  d16

  (p21 ph3):f3
  DELTA8 
  (center (p2 ph1) (p22 ph1):f3 )
  DELTA8
  (ralign (p1 ph1) (p21 ph2):f3 )

  p16:gp8
  d16

  4u fq=cnst19(bf ppm):f1
  4u f3_list:f3

  4u pl19:f1 pl33:f3

  if "l11 > 0"
     {
     d21 cpds8:f1 cpd7:f3 
     }

  4u do:f1 do:f3
  4u pl1:f1 pl3:f3

  4u fq=cnst18(bf ppm):f1
  4u f3_zero:f3

  p16:gp4
  d16

  (p21 ph8):f3
  4u
  p16:gp2*-1*EA
  d16
  DELTA3
  (p22 ph6):f3
  d10 gron0*-1
  2u groff
#   ifdef LABEL_CN
  (center (p44:sp30 ph9) (p8:sp13 ph1):f2  )
#   else
  (p44:sp30 ph9)
#   endif /*LABEL_CN*/

  d10 gron0
  2u groff
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
  go=2 ph31 cpd3:f3

  d11 do:f3 mc #0 to 2
     F1QF(calclc(l11, 1))
     F2EA(calgrad(EA) & calph(ph5, +180), caldel(d10, +in10) & calph(ph6, +180) & calph(ph8, +180) & calph(ph31, +180))
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
;pl19: f1 channel - power level for CPD/BB decoupling
;pl33: f3 channel - power level for CW irradiation
;sp13: f2 channel - shaped pulse 180 degree  (Ca and C=O, adiabatic)
;sp30: f1 channel - shaped pulse 180 degree (Bip720,50,20.1)
;p1 : f1 channel -  90 degree high power pulse
;p2 : f1 channel - 180 degree high power pulse
;p8 : f2 channel - 180 degree shaped pulse for inversion (adiabatic)
;p16: homospoil/gradient pulse
;p19: gradient pulse 2                               [500 usec]
;p21: f3 channel -  90 degree high power pulse
;p22: f3 channel - 180 degree high power pulse
;p44: f1 channel - 180 degree shaped pulse
;d1 : relaxation delay; 1-5 * T1
;d10: incremented delay                              [3 usec]
;d11: delay for disk I/O                             [30 msec]
;d16: delay for homospoil/gradient recovery
;d21: T_Ex
;d24: 1/(4J)YH for YH
;     1/(8J)YH for all multiplicities
;d25: 1/(4J)YH for YH
;     1/(8J)YH for all multiplicities
;d26: 1/(4J(YH))
;d31: T_max
;cnst4: = J(YH)
;cnst9: Saturation B1 field strength [Hz]
;cnst11: for multiplicity selection = 4 for NH, 8 for all multiplicities
;cnst12: for multiplicity selection = 4 for NH, 8 for all multiplicities
;cnst18: H2O chemical shift (offset, in ppm)
;cnst19: H(N) chemical shift (offset, in ppm)
;cnst21: CO chemical shift (offset, in ppm)
;cnst22: Calpha chemical shift (offset, in ppm)
;inf2: 1/SW(N) = 2 * DW(N)
;in10: 1/(2 * SW(N)) = DW(N)
;nd10: 2
;ns: 8 * n
;ds: >= 16
;td1: number of delays in fq-list
;td2: number of experiments in F2
;FnMODE: QF in F1
;FnMODE: echo-antiecho in F2
;cpd1: decoupling according to sequence defined by cpdprg8: 90x_240y_90x
;cpd3: decoupling according to sequence defined by cpdprg3
;cpd7: decoupling according to sequence defined by cpdprg7: cwp
;pcpd3: f3 channel - 90 degree pulse for decoupling sequence
;pcpd8: f1 channel - 90 degree pulse for decoupling sequence


;for z-only gradients:
;gpz0: 0.5%
;gpz1: 50%
;gpz2: 80%
;gpz3: 16.2%
;gpz4: 31%
;gpz5: 5%
;gpz6: -2%
;gpz7: 23%
;gpz8: 13%

;use gradient files:   
;gpnam1: SMSQ10.100
;gpnam2: SMSQ10.100
;gpnam3: SMSQ10.100
;gpnam4: SMSQ10.100
;gpnam5: SMSQ10.50
;gpnam6: SMSQ10.100
;gpnam7: SMSQ10.100
;gpnam8: SMSQ10.100


;define FQ3LIST (irradiation frequencies)


                                          ;preprocessor-flags-start
;LABEL_CN: for C-13 and N-15 labeled samples start experiment with 
;             option -DLABEL_CN (eda: ZGOPTNS)
                                          ;preprocessor-flags-end



;$Id: hsqc_cest_etf3gpsitc3d,v 1.1.2.3 2015/10/23 15:32:24 ber Exp $
