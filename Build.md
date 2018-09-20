# Building OBSInfoWriter

1. Setup a directory where you can clone all the projects in

2. Clone the latest OBS-Studio
- Follow the Build directions for obs-studio https://github.com/jp9000/obs-studio/wiki/Install-Instructions
- You don't need to build the project

3. On Mac:
- Extract the .dylib binaries from the latest OBS for MAC build and place them in a folder called obs-binaries (right-click on OBS in Applications and choose "Show package contents")

4. On Windows:
- Make .lib files related to libobs.dll and obs-frontend-api.dll
  - Open a Visual Studio Command Prompt and go to a new folder
  - Run dumpbin /EXPORTS "C:\Program Files (x86)\obs-studio\bin\64bit\obs.dll" > obs.exports
  - Run dumpbin /EXPORTS "C:\Program Files (x86)\obs-studio\bin\64bit\obs-frontend-api.dll" > obsfrontend.exports
  - In a text editor, open the exports files and remove everything but the function names
  - Put the word EXPORTS on top of the files
  - Run lib /def:obs.exports /out:obs64.lib /machine:x64
  - Run lib /def:obsfrontend.exports /out:obs-frontend-api64.lib /machine:x64
  - Run lib /def:obs.exports /out:obs32.lib /machine:x86
  - Run lib /def:obsfrontend.exports /out:obs-frontend-api32.lib /machine:x86
- Copy the lib files to ..\obs-studio-bin\lib\x64 and ..\obs-studio-bin\lib\x86 from the OBSInfoWriter folder

5. On Linux:
- No extra action required

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
