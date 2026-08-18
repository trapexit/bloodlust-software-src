        .386
        .model flat
        LOCALS

        PUBLIC initcdasm_,cdrequest_,cdsetdrive_
        .data
        
       PUBLIC _cdrealmemseg 

        REG                   STRUC
          REDI         dd     ?    ; offset 0
          RESI         dd     ?    ; offset 4
          REBP         dd     ?    ; offset 8
          XXXX         dd     ?    ; offset 12
          REBX         dd     ?    ; offset 16
          REDX         dd     ?    ; offset 20
          RECX         dd     ?    ; offset 24
          REAX         dd     ?    ; offset 28
          RFLAGS       dw     ?    ; offset 32
          RES          dw     ?    ; offset 30
          RDS          dw     ?    ; offset 32
          RFS          dw     ?    ; offset 34
          RGS          dw     ?    ; offset 36
          RIP          dw     ?    ; offset 38
          RCS          dw     ?    ; offset 40
          RSP          dw     ?    ; offset 42
          RSS          dw     ?    ; offset 44
        REG                   ENDS
rm     REG ? ; real mode regs        


_cdrealmemseg dw 0 ;real stack segments

cdletter dw ?

        .code
        
        
initcdasm_ PROC 
        push edi
        push ds 
        pop es
        
        
        ;allocate realmode xfer area
        mov eax,100h ;allocate
        mov ebx,512/16
        int 31h  ;get memory
        mov [_cdrealmemseg],ax
        jnc GOTMEMORY
        mov eax,0
        jmp @@DONE  ;;abort
GOTMEMORY:
                
        
        ;set up real mode int call struct
        mov rm.XXXX,0
        mov rm.RCS,0        
        mov rm.RIP,0        
        mov rm.RFS,ax       
        mov rm.RGS,ax       
        mov rm.RES,ax
        mov rm.RDS,ax
        mov rm.REBX,0  ;es:bx buffer
      
        mov rm.RSP,0
        mov rm.RSS,0
        pushfd
        pop eax
        mov rm.RFLAGS,ax

        ;get drive letters
        mov rm.REAX,150Dh ;function num
        call cdrequest_
        

        mov rm.REAX,1510h ;function num

        mov eax,1 ;;success
@@DONE:
        pop edi
        ret
initcdasm_ ENDP


cdrequest_ PROC
        push edi
        mov eax,0300h
        mov ebx,2Fh
        xor ecx,ecx
        mov edi,OFFSET rm
        push ds
        pop es
        int 31h        
        pop edi
        ret
cdrequest_ ENDP


cdsetdrive_ PROC
        mov rm.RECX,eax
        ret
cdsetdrive_ ENDP                

        END


