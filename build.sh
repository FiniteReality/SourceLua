# Convenience script for people trying to build SourceLua

# Force 32-bit builds (Source SDK only has 32-bit binaries currently)
export CC="`realpath ./util/gcc-m32.sh`" CXX="`realpath ./util/g++-m32.sh`"

if [[ ! -d build ]]; then
    mkdir build
fi

cd build
cmake ..
make
make install
