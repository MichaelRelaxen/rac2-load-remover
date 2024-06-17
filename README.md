# Black Label Timer Mod
[This mod](<https://github.com/MichaelRelaxen/gcpt>) times RaC2 black label runs in a hardware-independent way. It removes black (loading) screens, automatically does long load removal and normalisation, and provides per-planet segment splits. 

To use it, load the mod (details below) and the mod should automatically reset your timer when you load a fresh save file on aranos 1. After killing the protopet on yeedil, the timer should freeze; time warp to before killing the protopet to see the final time of your run with extra loading times removed. 

# Usage 

The mod should be configured for runs by default, but you can use the following button combos:
* `L2+R2+L1+R1`: toggle display
* `L2+R2+up`: show current segment 
* `L2+R2+right`: show next segment 
* `L2+R2+left`: show previous segment
* `L2+R2+down`: toggle segment mode

## Installation
To run the mod on emulator (PCSX2) you can put the `.pnach` file into the cheats directory, then enable cheats.

To run the mod on console with a disc, use the supplied .ELF file which bundles the mod and automatically applies it.

## Known Issues
- Some graphical bugs with the timer are present during autosaves, pauses, cutscenes etc.
- If you use progressive scan you'll need to play a cinematic from the menu to get the timer to display.
We're looking for people to test this!! Please report any issues you find!

## Build Instructions
To build the load remover, install [docker](https://docker.com/)
```
docker pull ps2dev/ps2dev:v1.2.0
docker run -it --rm -v "$PWD\:/src" ps2dev/ps2dev:v1.2.0
cd src
cd gcpt
./docker-init.sh
```

Then build:
```
make clean && make
```

NOTE: The docker-init script is known to fail due to inconsistent line endings when your git is configured to convert line endings (windows). If this is the case you can either replace all line endings in the file with LF or just run each command in the script manually.

To build the `gctimer.elf`, just run the `docker-make.bat` file inside the ps2cheat folder.

## Credits
- [deadlocked-cheats](https://github.com/Dnawrkshp/deadlocked-cheats) by Dnawrkshp and Metroynome
- [NetCheat](https://github.com/Dnawrkshp/NetCheat) by Dnawrkshp