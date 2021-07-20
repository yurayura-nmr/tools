#!/bin/tcsh

setenv FILE $argv[1]

nmrPipe -in ${FILE}.fid					\
| nmrPipe -fn SOL					\
| nmrPipe -fn SP -c 0.5 -off 0.45 -end 0.98 -pow 1	\
| nmrPipe -fn ZF -auto					\
| nmrPipe -fn FT					\
| nmrPipe -fn PS -p0 95 -p1 13 -di			\
| nmrPipe -fn TP					\
| nmrPipe -fn SP -c 0.5 -off 0.5 -end 1 -pow 1		\
| nmrPipe -fn ZF -auto					\
| nmrPipe -fn FT -alt					\
| nmrPipe -fn PS -p0 0 -p1 0 -di			\
| nmrPipe -fn POLY -auto -ord 1				\
| nmrPipe -fn TP					\
| nmrPipe -fn POLY -auto -ord 1				\
| nmrPipe -fn EXT -x1 5.9ppm -xn 9.8ppm -sw		\
| nmrPipe -out ${FILE}.ft2 -ov

#pipe2xyz -nv -in ${FILE}.ft2 -out ${FILE}.nv -ov
#pipe2ucsf ${FILE}.ft2 ${FILE}.ucsf
