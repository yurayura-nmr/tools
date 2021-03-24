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

Easiest using the Mint GUI called Driver Manager.
GUI > Driver Manager > Set to nvidia and hope it works. 
If not, choose a different version of the driver or try manual install.
390 works for most of my PCs, whereas 460 works only for some of them
"""
sudo apt purge *nvidia*                 # Need to go back first otherwise installs will fail.
sudo apt-get install nvidia-driver-390  # 390 works best. Newer drivers cause problems on some cards.
sudo reboot                             # Necessary
nvidia-smi                              # Driver reccognizes GPU correctly? Even after reboot?

# 2. Set static IP
sudo apt-get install ssh
"""
Also easiest via the Mint/Ubuntu GUI.
Go to network connection > properties > IPv4 > set IP
IP:      choose
Netmask: 255.255.255.0
Gateway: use whatever was there before
"""

# 3. CMAKE
# Get newest version and build from source: https://cmake.org/download/
sudo apt-get install libssl-dev

wget ...
tar xvf ...
./configure

make -j4
sudo make install

# 4. FFTW
# Get newest version and build from source: http://www.fftw.org/download.html
sudo apt-get install gfortran

wget ...
tar xvf ...
./configure --enable-static --enable-shared --enable-sse2 --enable-avx --enable-avx2  --enable-float

make -j4
sudo make install

# 5. LAPACK (includes BLAS)
# Get source
wget ..
tar xvf ...
mkdir testbuild
cd testbuild
cmake ..
make -j4

sudo make install # note where the libraries are installed; this will be important later when GROMACS is compiled.

# 6. CUDA
"""
Get CUDA drivers from NVIDIA.
Make sure that the CUDA version is compatible with whatever driver we get to work.
e.g. nvidia-390 supports CUDA 9.1 but not CUDA 11, etc.
"""
wget ...
chmod +x run ...
./run... -override # need to override otherwise, the installer always complains about the compiler

# 7. GROMACS
"""
Gromacs wants somewhat new, but CUDA wants somewhat old C and C++ compilers.
Then we need to rewrite the host_config header so that the compile works.
"""
sudo apt-get install gcc-7 g++-7
sudo apt-get install vim

vi /usr/local/cuda-9.1/include/crt/host_config.h # change this file to work around it. GNU 7 compilers will just do fine.

# Comment out like this, i.e. leave everything as it is and just skip the error.
#if __GNUC__ > 6
//#error -- unsupported GNU version! gcc versions later than 6 are not supported!
#endif /* __GNUC__ > 6 */

cmake .. -DGMX_GPU=CUDA -DCMAKE_C_COMPILER=gcc-7 -DCMAKE_CXX_COMPILER=g++-7 -DREGRESSIONTEST_DOWNLOAD=ON -DGMX_BLAS_USER=/usr/local/lib/libblas.a



sudo apt-get install Hwloc


# BLAS
# build simply with cmake and make install

git clone https://github.com/Reference-LAPACK/lapack.git
...

# CUDA

http://developer.download.nvidia.com/compute/cuda/7.5/Prod/local_installers/cuda_7.5.18_linux.run

# GROMACS

# CUDA toolkit and driver mismatch can occur
# check gmx --version after installation if mismatch
# Manually set compilers if cmake looks for old version like gcc5 or 6
# Compiler upgrade summary: https://gist.github.com/youmych/122e32cfa1c228c84684fec13b7d52e1
cmake .. -DGMX_GPU=CUDA -DGMX_BUILD_OWN_FFTW=ON -DGMX_BLAS_USER=/usr/local/lib/libblas.so -DREGRESSIONTEST_DOWNLOAD=ON -DGMX_LAPACK_USER=/usr/local/lib/liblapack.so -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-7.5 -DCMAKE_C_COMPILER=gcc-7 -DCMAKE_CXX_COMPILER=g++-7

# CUDA needs manual install from NVIDIA. Apt-get just costs time. apt purge old.
# It seems after installing drivers reboot necessary?
# Sometimes apt purge/remove driver>> apt-get install nvidia-driver-460 etc works if lucky. 
# If message is: Failed to initialize NVML: Driver/library version mismatch --> reboot

# chihiro: cmake .. -DGMX_BUILD_OWN_FFTW=ON -DGMX_BLAS_USER=/usr/lib/x86_64-linux-gnu/libblas.so -DREGRESSIONTEST_DOWNLOAD=ON -DGMX_LAPACK_USER=/usr/lib/x86_64-linux-gnu/liblapack.so -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-10.2/ -DCMAKE_C_COMPILER=gcc-7 -DCMAKE_CXX_COMPILER=g++-7 -DGMX_GPU=CUDA
# mayuta : cmake .. -DGMX_BUILD_OWN_FFTW=ON -DGMX_BLAS_USER=/usr/lib/libblas.so -DREGRESSIONTEST_DOWNLOAD=ON -DGMX_LAPACK_USER=/usr/lib/lapack/liblapack.so -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-11.2/ -DCMAKE_C_COMPILER=gcc-7 -DCMAKE_CXX_COMPILER=g++-7 -DGMX_GPU=CUDA
# fubuki : cmake .. -DGMX_BUILD_OWN_FFTW=ON -DGMX_BLAS_USER=/usr/lib/libblas.so -DREGRESSIONTEST_DOWNLOAD=ON -DGMX_LAPACK_USER=/usr/lib/liblapack.so -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-11.2/ -DCMAKE_C_COMPILER=gcc-7 -DCMAKE_CXX_COMPILER=g++-7 -DGMX_GPU=CUDA

make check # not make test!!!

# Isopeptide: specbond.dat, residuetypes.dat

"""
Manual install
* Download CUDA including the driver
* Boot to terminal mode CTRL ALT F1
"""
sudo apt purge *nvidia*
sudo apt-get install gcc-10 g++-10
chmod +x ./cuda...
./cuda...run # under options choose that the system config is updated to use NVIDIA driver
