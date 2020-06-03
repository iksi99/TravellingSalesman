REG QUERY HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x64

if %ERRORLEVEL% NEQ 0 vcredist_x64.exe else GOTO CHECK_VERSION

:INSTALL
rmdir /s /q "C:\TravellingSalesmanSolver"

mkdir "C:\TravellingSalesmanSolver"
mkdir "C:\TravellingSalesmanSolver\input"
mkdir "C:\TravellingSalesmanSolver\output"
mkdir "C:\TravellingSalesmanSolver\output\plots"
copy GeneticAlgorithm.exe "C:\TravellingSalesmanSolver"
copy icon.ico "C:\TravellingSalesmanSolver"
copy wi29.tsp "C:\TravellingSalesmanSolver\input"
copy dj38.tsp "C:\TravellingSalesmanSolver\input"
copy qa194.tsp "C:\TravellingSalesmanSolver\input"
copy zi929.tsp "C:\TravellingSalesmanSolver\input"

set SCRIPT="%TEMP%\%RANDOM%-%RANDOM%-%RANDOM%-%RANDOM%.vbs"

echo Set oWS = WScript.CreateObject("WScript.Shell") >> %SCRIPT%
echo sLinkFile = "%USERPROFILE%\Desktop\Travelling Salesman Solver.lnk" >> %SCRIPT%
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SCRIPT%
echo oLink.TargetPath = "C:\TravellingSalesmanSolver\GeneticAlgorithm.exe" >> %SCRIPT%
echo oLink.WorkingDirectory = "C:\TravellingSalesmanSolver" >> %SCRIPT%
echo oLink.IconLocation = "C:\TravellingSalesmanSolver\icon.ico" >> %SCRIPT%
echo oLink.Save >> %SCRIPT%

cscript /nologo %SCRIPT%
del %SCRIPT%
GOTO END

:CHECK_VERSION

for /f "tokens=3" %%a in ('REG QUERY HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x64 /v Bld ^|findstr /ri "REG_DWORD"') do set VCBLD=%%a
if %VCBLD% LSS 0x7030 vcredist_x64.exe
GOTO INSTALL

:END