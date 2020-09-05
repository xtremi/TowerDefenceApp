echo on
set TDinputGen=D:\AGK\AGK_libs2\apps\Test_app_tower_defence\TDInputGen\TDInputGen\x64\Release\TDInputGen.exe

set asset_path=%cd%\..\assets

set towersCSV=%cd%\towers.csv
set mobsCSV=%cd%\mobs.csv
set wavesCSV=%cd%\waves.csv
set bulletsCSV=%cd%\bullets.csv
set animationsCSV=%cd%\animations.csv
set shadersCSV=%cd%\shaders.csv

set towersJSON=%cd%\..\assets\json\tower_defs.json
set mobsJSON=%cd%\..\assets\json\mob_defs.json
set wavesJSON=%cd%\..\assets\json\wave_defs.json
set bulletsJSON=%cd%\..\assets\json\bullet_defs.json
set animationsJSON=%cd%\..\assets\json\animation_defs.json
set shadersJSON=%cd%\..\assets\json\shader_defs.json


%TDinputGen% %asset_path% ^
%towersCSV%     %towersJSON%    %mobsCSV%       %mobsJSON% ^
%wavesCSV%      %wavesJSON%     %bulletsCSV%    %bulletsJSON% ^
%animationsCSV% %animationsJSON% %shadersCSV%   %shadersJSON% dump
 
pause