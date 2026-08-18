.before
 del ados\build.obj
.after
 copy ados.exe c:\a32\ados.exe
project : c:\source\a32\ados\ados.exe .SYMBOLIC

!include c:\source\a32\ados\ados.mk1
