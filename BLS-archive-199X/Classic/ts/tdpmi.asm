        .386
        .model flat
        
        .data
    
        ALIGN DWORD        
stack     dd   4000h DUP (?)
stackend  label DWORD        

oldss   dw ?
oldsp   dd ?

       
        
        
        .code
        
        
        PUBLIC _SetDPMIVector,_GetDPMIVector,_CheckDS,_SwitchStack,_SwitchBack
        
       
        
_SwitchStack PROC NEAR
;        cli
        pop ebx  ;get calling funk
        mov [oldsp],esp
        mov [oldss],ss
        
        mov ax,ds
        mov ss,ax
        mov esp,OFFSET stackend-4
;        sti
        
        jmp ebx ;return
_SwitchStack ENDP
        
        
_SwitchBack PROC NEAR
;        cli
        pop ebx
        mov ss,[oldss]
        mov esp,[oldsp]
 ;       sti
        
        jmp ebx ;return
_SwitchBack ENDP                
                       
        
_CheckDS  PROC NEAR

        mov eax,0 
        mov bx,ds
        mov cx,ss
        cmp bx,cx
        je  @@OK
        inc eax
@@OK:   ret
_CheckDS ENDP        
        
        
        
        
        
_SetDPMIVector PROC NEAR
        ARG @@V:QWORD,@@NUM:DWORD
        
        push ebp
        mov ebp,esp
     
        mov  ax,205h
        mov ebx,@@NUM
        mov cx,[WORD PTR @@V+4]
        mov edx,[DWORD PTR @@V]
        int  31h
        
        pop ebp         
        ret        

_SetDPMIVector ENDP


_GetDPMIVector PROC NEAR
        ARG @@NUM:DWORD
        push ebp
        mov ebp,esp

        mov  ax,204h
        mov  ebx,@@NUM
        int  31h
        mov    eax,edx
        movzx  edx,cx
        
        pop ebp
        ret
_GetDPMIVector ENDP


        END        
   
