.386p
locals

_DATA segment dword public 'DATA' use32
	assume cs:_DATA

PUBLIC  __I32TchainLabel
PUBLIC  __I32TcallLabel
PUBLIC  __interrupt32Thunk
PUBLIC  __I32TdsLoadLabel
PUBLIC  __I32TchainSel
PUBLIC  __I32TchainOffset
PUBLIC  __interrupt32ThunkSize

dummy   proc near

@@startThunk            label byte

__interrupt32Thunk      label byte
	
	pushad
	push    ds
	push    es
	push    fs
	push    gs

	mov     ax, word ptr cs:[0FFFFFFFFh]
__I32TdsLoadLabel       EQU $ - @@startThunk - 4

	mov     ds, ax
	mov     es, ax

	db      0E8h
	dd      0FFFFFFFFh
__I32TcallLabel         EQU $ - @@startThunk - 4

;Routine called by thunk should return 1 to chain, 0 to not chain
        add     eax, 0FFFFFFFFh              ; set carry flag if chaining

	pop     gs
	pop     fs
	pop     es
	pop     ds
	popad

        jc      @@chain                      ; chain if carry flag is set
@@iret:                                      ; if carry flag is cleared
	iret                                 ;  interrupt return wo/chain

@@chain:
	jmp     [cs:fword ptr chainPointer]
__I32TchainLabel        EQU $ - @@startThunk - 4

chainPointer    label   dword

__I32TchainOffset       EQU $ - @@startThunk
	dd      0FFFFFFFFh
__I32TchainSel          EQU $ - @@startThunk
	dw      0FFFFh

__interrupt32ThunkSize  EQU $ - @@startThunk

dummy   endp

_DATA ends

end

