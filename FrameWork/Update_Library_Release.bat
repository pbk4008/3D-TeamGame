xcopy /y/s	.\Engine\public\*.*		.\Reference\Headers\

xcopy /y	.\Engine\bin\*.lib			.\Reference\Library\Release\


xcopy /y/s	.\Engine\bin\*.dll		.\Tool_YASIC\Bin\
xcopy /y/s  .\Engine\bin\*.dll		.\Tool_YM\Bin\Release_x64\
xcopy /y/s  .\Engine\bin\*.dll		.\UITool\Bin\












xcopy /y/s /e .\Engine\bin\ShaderFiles\*.hlsl   .\ShaderExamples\bin\ShaderFiles\
xcopy /y/s /e .\Engine\bin\*.dll				.\ShaderExamples\bin\Release\