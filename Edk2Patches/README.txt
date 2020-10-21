The patch provided in this folder is a workaround for EDK2 build system in order
to generate valid Windows Boot Application.

To apply this:

    git apply YahalloPkg/Edk2Patches/0001-BaseTools-support-generation-of-Windows-Boot-Applica.patch
    make -C BaseTools
