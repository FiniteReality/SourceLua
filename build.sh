export CC="`realpath ./util/gcc-m32`" CXX="`realpath ./util/g++-m32`"

if [[ -d build ]]; then
    mkdir build
fi

cd build
cmake ..
make
make install
