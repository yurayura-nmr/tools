"""
Erik Walinda
Kyoto University
Graduate School of Medicine

https://github.com/yurayura-nmr/tools

Setup of GROMACS on a new PC using Pop! OS
A list of commands. Not to be run as a single script.
"""

# 1. NVIDIA driver 
"""
Hardest part is to properly install the NVIDIA driver.
Luckily, this is easy in Pop! OS.
Download image and use etcher to burn to USB. 
(UNetboot thingy caused errors during install).
"""
nvidia-smi   # Driver reccognizes GPU correctly?


# 2. Set static IP
sudo apt-get install ssh
"""
Easiest via the 
Go to network connection > properties > IPv4 > set IP
IP:      10.249.63.XXX [e.g. fubuki: 127]
Netmask: 255.255.255.0
Gateway: 10.249.63.254 [router]

Disconnect and reconnect, check if it works.
e.g. if apt-get update still works.
"""

# 3. CMAKE
# Get newest version and build from source: https://cmake.org/download/
sudo apt-get install libssl-dev

wget https://github.com/Kitware/CMake/releases/download/v3.20.0/cmake-3.20.0.tar.gz
tar xvf cmake-3.20.0.tar.gz
./configure

make -j4
sudo make install

# 4. FFTW
# Get newest version and build from source: http://www.fftw.org/download.html
wget http://www.fftw.org/fftw-3.3.9.tar.gz
tar xvf fftw-3.3.9.tar.gz
./configure --enable-static --enable-shared --enable-sse2 --enable-avx --enable-avx2  --enable-float

make -j4
sudo make install

# 5. LAPACK (includes BLAS)
# Get source
sudo apt-get install gfortran

wget https://github.com/Reference-LAPACK/lapack/archive/v3.9.0.tar.gz
tar xvf v3.9.0.tar.gz
mkdir testbuild
cd testbuild
cmake .. -DBUILD_SHARED_LIBS=ON # required to get libblas.so and liblapack.so

make -j4
sudo make install # note where the libraries are installed; this will be important later when GROMACS is compiled.

# 6. CUDA
"""
Get CUDA drivers from NVIDIA.
Again, Pop! OS comes to the rescue.
https://support.system76.com/articles/cuda/
"""
sudo apt install system76-cuda-latest
sudo apt-get install mlocate
locate cuda # check where they went, we have to tell GROMACS this

# 7. GROMACS part 1
"""
Gromacs wants somewhat new, but CUDA wants somewhat old C and C++ compilers.
Sometimes we need to rewrite the host_config header so that the compile works.
Not necessary for newer CUDA.
"""
sudo apt-get install build-essential
sudo apt-get install libhwloc-dev
sudo apt-get install libomp-dev
sudo apt-get install gcc-7 g++-7
sudo apt-get install vim
#sudo apt-get install openmpi-bin libopenmpi-dev

#vi /usr/local/cuda-9.1/include/crt/host_config.h # change this file to work around it. GNU 7 compilers will just do fine.
# Comment out like this, i.e. leave everything as it is and just skip the error.
#if __GNUC__ > 6
//#error -- unsupported GNU version! gcc versions later than 6 are not supported!
#endif /* __GNUC__ > 6 */

# 8. GROMACS part 2
"""
Check output of cmake so we do not miss anything.
was CUDA found? was fftw3 found? were blas and lapack found?
"""
# get GROMACS source
tar xvf ...
mkdir testbuild
cd testbuild
cmake .. -DGMX_GPU=CUDA -DCMAKE_C_COMPILER=gcc-7 -DCMAKE_CXX_COMPILER=g++-7 -DREGRESSIONTEST_DOWNLOAD=ON -DGMX_BLAS_USER=/usr/local/lib/libblas.so -DGMX_LAPACK_USER=/usr/local/lib/liblapack.so -DGMX_OPENMP=ON -DGMX_MPI=OFF -DGMX_THREAD_MPI=ON -DCUDA_TOOLKIT_ROOT_DIR=/usr/lib/cuda-11.2

make -j4
make check -j4      # all tests must pass. Otherwise trouble with install or hardware. Might be better to fall back on older gromacs if they work.

./bin/gmx --version # verify that CUDA was recognized during the build

"""
GROMACS version:    2021.1
Verified release checksum is 8c24bff5d3f78b0a9afb16e880b5667e5affe9a686d462482bac20ce975492c6  <-- recognized?
Precision:          mixed
Memory model:       64 bit
MPI library:        thread_mpi
OpenMP support:     enabled (GMX_OPENMP_MAX_THREADS = 64) <-- recognized?
GPU support:        CUDA                                  <-- recognized?
SIMD instructions:  AVX2_256
FFT library:        fftw-3.3.9-sse2-avx-avx2-avx2_128
RDTSCP usage:       enabled
TNG support:        enabled
Hwloc support:      disabled
Tracing support:    disabled
C compiler:         /usr/bin/gcc-8 GNU 8.4.0
C compiler flags:   -mavx2 -mfma -Wno-missing-field-initializers -fexcess-precision=fast -funroll-all-loops -pthread -O3 -DNDEBUG
C++ compiler:       /usr/bin/g++-8 GNU 8.4.0
C++ compiler flags: -mavx2 -mfma -Wno-missing-field-initializers -fexcess-precision=fast -funroll-all-loops -pthread -fopenmp -O3 -DNDEBUG
CUDA compiler:      /usr/local/cuda/bin/nvcc nvcc: NVIDIA (R) Cuda compiler driver;Copyright (c) 2005-2019 NVIDIA Corporation;Built on Wed_Oct_23_19:24:38_PDT_2019;Cuda compilation tools, release 10.2, V10.2.89
CUDA compiler flags:-std=c++14;-gencode;arch=compute_30,code=sm_30;-gencode;arch=compute_35,code=sm_35;-gencode;arch=compute_37,code=sm_37;-gencode;arch=compute_50,code=sm_50;-gencode;arch=compute_52,code=sm_52;-gencode;arch=compute_60,code=sm_60;-gencode;arch=compute_61,code=sm_61;-gencode;arch=compute_70,code=sm_70;-gencode;arch=compute_75,code=sm_75;-gencode;arch=compute_35,code=compute_35;-gencode;arch=compute_32,code=compute_32;-use_fast_math;-D_FORCE_INLINES;-mavx2 -mfma -Wno-missing-field-initializers -fexcess-precision=fast -funroll-all-loops -pthread -fopenmp -O3 -DNDEBUG
CUDA driver:        11.0              <-- recognized?
CUDA runtime:       10.20             <-- recognized?
"""

sudo make install

# Add to .bashrc etc.
source /usr/local/gromacs/bin/GMXRC.bash

# Install additional force fields, bonds, etc.
# e.g. isopeptide: specbond.dat, residuetypes.dat
