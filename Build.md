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

4.ALT. Another way to do step 4 above for Windows
- Make .lib files for obs.dll and obs-frontend-api.dll
  - Open visual studio command prompt and copy obs.dll and obs-frontend-api.dll to a folder (no spaces in path)
  - Create a .bat file (make sure to execute in the visual studio command prompt) as follows

```
@echo off
dumpbin /exports obs.dll > exports.txt
echo EXPORTS > obs.def
for /f "skip=19 tokens=4" %%A in (exports.txt) do echo %%A >> obs.def
lib /def:obs.def /out:obs32.lib /machine:x86
lib /def:obs.def /out:obs64.lib /machine:x64
```
```
@echo off
dumpbin /exports obs-frontend-api.dll > exports.txt
echo EXPORTS > obs-frontend-api.def
for /f "skip=19 tokens=4" %%A in (exports.txt) do echo %%A >> obs-frontend-api.def
lib /def:obs-frontend-api.def /out:obs-frontend-api32.lib /machine:x86
lib /def:obs-frontend-api.def /out:obs-frontend-api64.lib /machine:x64
```

  - Run the .bat files and you should have the 4 lib files made
  - Copy the lib files to ..\obs-studio-bin\lib\x64 and ..\obs-studio-bin\lib\x86 from the OBSInfoWriter folder
  - I believe the files need to be renamed obs-frontend-api.lib and obs.lib under the \x86 and \x64 folders

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
