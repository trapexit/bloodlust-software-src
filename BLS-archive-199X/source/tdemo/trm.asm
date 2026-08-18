        .386
        .model flat
        
        .data


_TEXT16 SEGMENT BYTE PUBLIC USE16 'CODE'
         ASSUME  cs:_TEXT16        
         
      PUBLIC _rmhandler,_rmackport

_rmhandler PROC
push ax
push dx
mov dx,_rmackport
in al, dx
mov al, 020h
out 0A0h,al
out  20h,al
pop dx
pop ax
iret
_rmhandler ENDP        

_rmackport dw ?
        
_TEXT16 ENDS        


END
