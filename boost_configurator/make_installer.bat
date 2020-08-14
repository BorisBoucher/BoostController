
REM Cleanup
IF EXIST target (
	rmdir /Q /S target
)

REM Create output directory
MKDIR target

REM Deploy dependencies, resources and binary
windeployqt --dir target debug\boost_configurator.exe
copy debug\boost_configurator.exe target
copy *.png target

REM run instaleur generator
"c:\Program Files (x86)\Inno Setup 6\iscc.exe" installer_script.iss
