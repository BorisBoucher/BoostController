
REM Cleanup
IF EXIST target (
	rmdir /Q /S target
)

SET BUILD_TYPE=%1

REM Create output directory
MKDIR target

REM Deploy dependencies, resources and binary
windeployqt --dir target %BUILD_TYPE%\boost_configurator.exe
copy %BUILD_TYPE%\boost_configurator.exe target
copy *.png target

REM run installer generator
"c:\Program Files (x86)\Inno Setup 6\iscc.exe" installer_script.iss
