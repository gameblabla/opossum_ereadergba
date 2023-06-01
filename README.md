**Opossum Massage Simulator**

By gameblabla (PC, Ti Nspire, N64, GBA ports) and DJ Omnimaga (original idea and game for TI83CE)

GAME SYSNOPSIS
===============
In the year 2024, Donald Trump ascended to the presidency of the United States once more. 
As stated in his party's platform, he implemented a bold and controversial policy: the prohibition of aliens within American borders.

This action was not received lightly by the extraterrestrial community. 
Deeply affronted, they engaged in extensive deliberations. 
The alien council, after much debate and consideration, reached a decision. 
By a substantial majority of 220 to 80 votes, they resolved to launch an invasion upon our Earth.

Our planet's only defense now rests on an unlikely savior: the humble opossum. We urgently implore your aid in safeguarding our world. Together, we can uphold justice and ensure a secure future for our Earth!

Compatibility and emulators
============================
Opposum Massage Simulator works on the US e-Reader as well as the original Japanese e-Reader and the e-Reader Plus.

For emulation, you'll need mGBA (https://mgba.io/) and an additional GBA bios as well otherwise scanning the dotcodes
and playing the game will not work.

Additionally, the japanese version will only work on No$GBA due to an emulation issue with mGBA (even with BIOS).

Compiling the game
=====================
For compiling the game, you'll need devkitpro.
It's recommended to use linux (as that's the main setup i have). 
For Windows users: you'll need to modify the makefiles to run nedcmake tools.

Makefile.er is for the US e-Reader
```
make -f Makefile.er
```

Makefile.erj is for the Japanese e-Reader. 
Additionally, you need to use NO$GBA built-in A22 assembler (Assemble File option in the debugger version) to make the final binary by selecting the OPOSSUM.A22 file.
```
make -f Makefile.erj
```

This is for the cartridge release (mostly done for testing) :
```
make
```

