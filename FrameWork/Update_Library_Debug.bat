xcopy /y/s	.\Engine\public\*.*		.\Reference\Headers\

xcopy /y	.\Engine\bin\*.lib		.\Reference\Library\Debug\

xcopy /y/s	.\Engine\bin\*.dll		.\Tool_YASIC\Bin\
xcopy /y/s  .\Engine\bin\*.dll		.\Tool_YM\Bin\










xcopy /y/s /e .\Engine\bin\ShaderFiles\*.hlsl   .\ShaderExamples\bin\ShaderFiles\
xcopy /y/s /e .\Engine\bin\*.dll				.\ShaderExamples\bin\Debug\
xcopy /y	.\Engine\bin\*.lib					.\Reference\Library\Debug\