xcopy /y/s	.\Engine\public\*.*		.\Reference\Headers\

xcopy /y	.\Engine\bin\*.lib		.\Reference\Library\Debug\

xcopy /y/s	.\Engine\bin\*.dll		.\MaterialTool\Bin\
xcopy /y/s  .\Engine\bin\*.dll		.\Tool_YM\Bin\Debug_x64\
xcopy /y/s  .\Engine\bin\*.dll		.\UITool\Bin\
xcopy /y/s  .\Engine\bin\*.dll		.\Client\bin\



xcopy /y/s /e .\Engine\bin\ShaderFiles\*.*   .\ShaderExamples\bin\ShaderFiles\
xcopy /y/s /e .\Engine\bin\ShaderFiles\*.*   .\Reference\ShaderFile\
xcopy /y/s /e .\Engine\bin\*.dll			 .\ShaderExamples\bin\Debug\