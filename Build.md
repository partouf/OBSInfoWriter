# Building OBSInfoWriter

1. Setup a directory where you can clone all the projects in

2. Follow the Build directions for obs-studio
- https://github.com/jp9000/obs-studio/wiki/Install-Instructions

3. Clone Crosscables
- git clone https://github.com/partouf/Crosscables.git
- cd Crosscables
- On linux/mac, run sh buildall.sh
- On Windows, open the .sln in Visual Studio (should be te same VS as OBS-Studio was built with)
- If building stops with Jumpknots because of openssl, that's ok, we only need Groundfloor to build

4. Clone OBSInfoWriter
- git clone https://github.com/partouf/OBSInfoWriter.git

5. Build
- Linux: make -f Makefile.linux
- Mac: make -f Makefile.mac
