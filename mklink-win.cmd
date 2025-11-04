
rmdir qwadro
mklink /J "qwadro" "..\qwadro\qwadro"

chdir ".\src\impl\"
rmdir "xss"
mklink /J "xss" "..\..\..\qwadro_xss\src"

pause
