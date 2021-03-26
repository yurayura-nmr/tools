"""
Erik Walinda
Kyoto University
Graduate School of Medicine

https://github.com/yurayura-nmr/tools

Setup of GROMACS on a new PC or a new Linux installation
A list of commands. Not to be run as a single script.
"""

# 1. NVIDIA driver 
"""
Hardest part is to properly install the NVIDIA driver.
So it is best to get this out of the way first.

BEST WAY IS TO USE A DISTRIBUTION THAT HAS NVIDIA DRIVER AND CUDA PREINSTALLED
e.g. Pop! OS (use etcher to burn to USB. UNetboot thingy caused errors during install)

Next time, let's try Pop! OS or another distro with NVIDIA drivers preinstalled.

Easiest using the Mint GUI called Driver Manager.
GUI > Driver Manager > Set to nvidia and hope it works. 
If not, choose a different version of the driver or try manual install.
390 works for most of my PCs, whereas 460 works only for some of them
"""
sudo apt purge *nvidia*                 # Need to go back first otherwise installs will fail.
sudo apt-get install nvidia-driver-390  # 390 works best. Newer drivers cause problems on some cards.
sudo reboot                             # Necessary
nvidia-smi                              # Driver reccognizes GPU correctly? Even after reboot?

# Or... (this sometimes but not often worked for me)
sudo add-apt-repository ppa:graphics-drivers/ppa
sudo apt-get update
sudo apt-get install nvidia-driver-...



# 2. Set static IP
sudo apt-get install ssh
"""
Also easiest via the Mint/Ubuntu GUI.
Go to network connection > properties > IPv4 > set IP
IP:      choose
Netmask: 255.255.255.0
Gateway: 
Reboot and make sure it worked. Sometimes it can disconnect everything.
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
wget https://github.com/Reference-LAPACK/lapack/archive/v3.9.0.tar.gz
tar xvf v3.9.0.tar.gz
mkdir testbuild
cd testbuild
sudo apt-get install gfortran
cmake .. -DBUILD_SHARED_LIBS=ON # required to get libblas.so and liblapack.so
make -j4

sudo make install # note where the libraries are installed; this will be important later when GROMACS is compiled.

# 6. CUDA
"""
Get CUDA drivers from NVIDIA.

* This also would be easier with Pop! OS, I guess...

* Make sure that the CUDA version is compatible with whatever driver we get to work.
  e.g. nvidia-390 supports CUDA 9.1 but not CUDA 11, etc.
  
* Moreover, the CUDA version must be compatible with the gcc version.
  https://stackoverflow.com/questions/6622454/cuda-incompatible-with-my-gcc-version
  
* CUDA will sometimes complain that the driver is too old for current CUDA.
  Newer drivers are great but sometimes they cause installation trouble.
  As stated here, we may want to take the older driver and ignore the CUDA installer's
  claim that CUDA won't work. In fact, it does work with older drivers.
  https://www.pyimagesearch.com/2019/12/09/how-to-install-tensorflow-2-0-on-ubuntu/
"""
wget ...
chmod +x run ...
./run... -override # need to override otherwise, the installer always complains about the compiler

# Pop! OS is easier!
# https://support.system76.com/articles/cuda/
sudo apt install system76-cuda-latest

# 7. GROMACS part 1
"""
Gromacs wants somewhat new, but CUDA wants somewhat old C and C++ compilers.
Then we need to rewrite the host_config header so that the compile works.
"""
sudo apt-get install build-essential
sudo apt-get install libhwloc-dev
sudo apt-get install libomp-dev
#sudo apt-get install openmpi-bin libopenmpi-dev
sudo apt-get install gcc-7 g++-7
sudo apt-get install vim

vi /usr/local/cuda-9.1/include/crt/host_config.h # change this file to work around it. GNU 7 compilers will just do fine.

# Comment out like this, i.e. leave everything as it is and just skip the error.
#if __GNUC__ > 6
//#error -- unsupported GNU version! gcc versions later than 6 are not supported!
#endif /* __GNUC__ > 6 */

# 8. GROMACS part 2
"""
Check output of cmake so we do not miss anything.
was CUDA found? If not, specify with: -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-7.5
was fftw3 found?
were blas and lapack found?
"""
cmake .. -DGMX_GPU=CUDA -DCMAKE_C_COMPILER=gcc-7 -DCMAKE_CXX_COMPILER=g++-7 -DREGRESSIONTEST_DOWNLOAD=ON -DGMX_BLAS_USER=/usr/local/lib/libblas.so -DGMX_LAPACK_USER=/usr/local/lib/liblapack.so -DGMX_OPENMP=ON -DGMX_MPI=OFF -DGMX_THREAD_MPI=ON

make -j4
make check -j4      # all tests must pass. Otherwise trouble with install or hardware. Can fall back on older gromacs.
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
