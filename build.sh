export CC="`realpath ./util/g++-m32`" CXX="`realpath ./util/`"

if [[ -d build ]]; then
    mkdir build
fi

cd build
cmake ..
make
make install
