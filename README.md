# Yahallo: Tegra 3 and Tegra 4 TrustZone UEFI variable services handler exploit and Secure Boot unlock tool

This tool exploits NVIDIA Tegra 3/Tegra 4 UEFI variable services and implements TrustZone takeover. In this way, users can permanently turn off Secure Boot on Tegra-based Windows RT devices without external devices' assistance (e.g. RCM Mode.)

This documentation is intentionally drafted in a professional way to discourage average device owners from messing up the system firmware.

**Disclaimer**: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. By using this tool, you acknowledge that you are intentionally turning off the device's ~security features~. The author is not liable for any consequence, for instance, confidential data loss due to fTPM lockout, or warranty void.

# Issue Disclosure
- 2020/08: Discovery, initial prototype
- 2020/09/22: Reported to MSRC (MSRC 61209)
- 2020/10/07: MSRC confirmed wontfix since Surface RT and Surface 2 hardware are EOL

> Unfortunately, you are correct - support for these versions of the Surface has ended, and no additional security updates will be offered. We appreciate the opportunity to review your research... - MSRC

- 2020/10/19: Reported to NVIDIA PSIRT (3156921)
- 2020/10/23: NVIDIA confirmed new Tegra SoC UEFI implementations don't reuse the old TZ code, old SoC are EOL and they think MS16-100 and MS16-140 fully addressed the prerequisite (but you can always install a BMR image and reset it...), wontfix

> The development team has evaluated this report. The UEFI variable store for current versions of Tegra has changed - the UEFI variable store for Orin/Hopper is not what was used in TZ in previous targets and they do not believe it is affected by this issue.
>
> Also, MS16-100 and MS16-140 appear to be both changes in MS code not system firmware, biggest potential piece would be for the bad images to be rejected from the UEFI secure boot. Likely, MS updated the main dbx file hosted here: https://uefi.org/revocationlistfile as that is the normal way for security issues to be handled in UEFI. - NVIDIA PSIRT

# Usage
- Install Secure Boot Golden Key Exploit first. If the device installed WU updates after Nov 2016, install the BMR to reset Secure Boot Key Storage.
- Run this tool as Windows Boot Manager Boot Application.

# Buildout
I've migrated the build system from Visual Studio (uefi-simple) to EDK2. To build it:

- Place this repo under EDK2 tree, such as `YahalloPkg`
- Apply the EDK2 build system patch. See `Edk2Patches` folder for details.
- `build -a ARM -p YahalloPkg/YahalloPkg.dsc -t GCC5`

Launch this image as a Windows Boot Manager OS entry, with `nointegritychecks` on and `testsigning` on.

# About Project Naming
"Yahallo" by Yui Yuigahama From [Oregairu](https://www.youtube.com/watch?v=Nhr5vrjHcIM). _No objections will be acknowledged._

# License
Copyright (c) 2019 - 2020, Bingxing Wang and other project authors. All rights reserved. 

This tool is released under GPLv2.
