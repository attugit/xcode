'distclean' finished successfully (0.000s)
Setting top to                           : /root/xcode 
Setting out to                           : /root/xcode/build 
Checking for 'g++' (C++ compiler)        : g++-6 
Checking boost includes                  : 1.60.0 
'configure' finished successfully (0.027s)
Waf: Entering directory `/root/xcode/build/release'
[ 1/14] Compiling src/lib/node.cpp
[ 2/14] Compiling src/bin/xcode.cpp
[ 3/14] Compiling gtest/src/gtest-all.cc
[ 4/14] Compiling gtest/src/gtest_main.cc
[ 5/14] Compiling test/tree/node_test.cpp
[ 6/14] Compiling test/tree/siblings_iterator_test.cpp
[ 7/14] Compiling test/tree/siblings_test.cpp
[ 8/14] Compiling test/lazy/lazy_test.cpp
[ 9/14] Linking build/release/libxc.so
[10/14] Linking build/release/xCode
[11/14] Linking build/release/ut_tree
[12/14] Processing build/release/ut_tree
[13/14] Linking build/release/ut_lazy
[14/14] Processing build/release/ut_lazy
Waf: Leaving directory `/root/xcode/build/release'
execution summary 
  tests that pass 2/2 
    /root/xcode/build/release/ut_tree 
    /root/xcode/build/release/ut_lazy 
  tests that fail 0/2 
'build_release' finished successfully (13.375s)
Waf: Entering directory `/root/xcode/build/release'
+ install /root/xcode/build/usr/include/xcode/lazy.hpp (from inc/xcode/lazy.hpp)
+ install /root/xcode/build/usr/include/xcode/node.hpp (from inc/xcode/node.hpp)
+ install /root/xcode/build/usr/include/xcode/siblings.hpp (from inc/xcode/siblings.hpp)
+ install /root/xcode/build/usr/lib/libxc.so (from build/release/libxc.so)
+ install /root/xcode/build/usr/bin/xCode (from build/release/xCode)
Waf: Leaving directory `/root/xcode/build/release'
'install_release' finished successfully (0.022s)
