xcopy /y/s	.\Engine\public\*.*		.\Reference\Headers\

xcopy /y	.\Engine\bin\*.lib			.\Reference\Library\Release\


xcopy /y/s  .\Engine\bin\*.dll		.\Tool_YM\Bin\Release_x64\
xcopy /y/s  .\Engine\bin\*.dll		.\UITool\Bin\
xcopy /y/s  .\Engine\bin\*.dll		.\Client\bin\

xcopy /y/s /e .\Engine\bin\ShaderFiles\*.*   .\Reference\ShaderFile\
xcopy /y/s /e .\Engine\bin\ShaderFiles\*.hpp .\Client\bin\ShaderFile\