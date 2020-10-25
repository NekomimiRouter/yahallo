The series of patch provide build foundation services. The first oneis a workaround for EDK2 build
system in order to generate valid Windows Boot Application.

The second one uses hard float for Clang ARM builds. Tested on Clang 11.

To apply this:

    git apply YahalloPkg/Edk2Patches/0001-BaseTools-support-generation-of-Windows-Boot-Applica.patch
    git apply 0002-BaseTools-use-hard-float-for-Clang-LLVM-ARM-build.patch
    make -C BaseTools
