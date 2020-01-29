cd build    
CC=clang CXX=clang cmake .
nof_processors=$(grep -c ^processor /proc/cpuinfo)
make -j$nof_processors