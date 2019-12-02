# You need clang, cmake
sudo apt-get -y install cmake g++ clang libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libgtest-dev
mkdir testBin
mkdir out

cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib
sudo mkdir /usr/local/lib/gtest
sudo ln -s /usr/lib/libgtest.a /usr/local/lib/gtest/libgtest.a
sudo ln -s /usr/lib/libgtest_main.a /usr/local/lib/gtest/libgtest_main.a