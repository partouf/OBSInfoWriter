# Building OBSInfoWriter

1. Setup a directory where you can clone all the projects in

2. Clone the latest OBS-Studio
- Follow the Build directions for obs-studio https://github.com/jp9000/obs-studio/wiki/Install-Instructions
- You don't need to build the project

3. On Mac:
- Extract the .dylib binaries from the latest OBS for MAC build and place them in a folder called obs-binaries (right-click on OBS in Applications and choose "Show package contents")

4. On Windows:
- Run .... to get static .lib files to link to for libobs.dll and obs-frontend

5. On Linux:
- ... Copy .so files?

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
