		.386
		.MODEL USE32 LARGE

		LOCALS

;		PUBLIC _CopyBackground,_PutBackground,_PutBackgroundOverlap
		PUBLIC _PutBackgroundTriOverlap,_PutBackground


		.CODE

comment *
_CopyBackground	PROC  NEAR
	ARG	@@DEST:DWORD,@@SRC:DWORD,@@X:WORD,@@LENGTH:WORD
		push	bp
		mov	bp,sp

		push	ds
		push	es
		push	si
		push	di

		lds	si,@@SRC
		les	di,@@DEST

		add	si,@@X

		mov	dx,@@LENGTH

@@YLOOP:	mov	cx,320d/4
		rep	movsd

		add	si,320

		dec	dl
		jnz	@@YLOOP

		pop	di
		pop	si
		pop	es
		pop	ds

		pop	bp

		ret

_CopyBackground	ENDP



_PutBackgroundOverlap  PROC
	ARG	@@SRC:DWORD,@@SRC2:DWORD,@@DEST:DWORD,@@XST:WORD,@@YST:WORD,@@XST2:WORD,@@YST2:WORD,@@DX:WORD,@@DY:WORD,@@YLIST:WORD
	LOCAL	@@XT:WORD,@@YT:WORD,@@XT2 =LLEN

		push	bp
		mov	bp,sp
		sub	sp,LLEN

		push	ds
		push	es
		push	si
		push	di

		lds	si,@@SRC

		les	di,@@DEST
		imul	ax,@@DY,320  ;;Position es:di pointer
		add	ax,@@DX
		add	di,ax

		lfs	dx,@@SRC2
		imul	ax,@@YST2,320  ;;Position fs:dx pointer
		add	ax,@@XST2
		add	dx,ax

		mov	ax,320
		sub	ax,@@XST2
		mov	@@XT2,ax

		lodsw		;Get xlength
		sub	ax,@@XST ;number of x pixels remaining in bg
		mov	bx,320d
		sub	bx,@@DX ;number of x pixels left on screen
		cmp	bx,ax
		jge 	@@TOOMANYX
		mov	ax,bx
@@TOOMANYX:       mov	@@XT,ax  ;Total number of pixels to copy



		lodsw		;Get ylength
		sub	ax,@@YST ;number of y pixels remaining in bg
		mov	bx,195;200d
		sub	bx,@@DY ;number of y pixels left on screen
		cmp	bx,ax
		jge 	@@TOOMANYY
		mov	ax,bx
@@TOOMANYY:       mov	@@YT,ax  ;Total number of pixels to copy


		xor	cx,cx      ;loop

@@YLOOP:	mov     bx,@@YLIST
		add	bx,@@YST
		add	bx,@@YST
		mov	si,[ss:bx] ;Get address of line
		add	si,[WORD PTR @@SRC]

		lea	ax,[di+320]  ;SRC1
		push	ax
		mov	ax,dx  ;SRC2
		add	ax,320
		push	ax
;--------------------------------------------------------
		;Left Clip
		push	[@@XT2]
		mov	bx,@@XST
		xor	ax,ax

@@PHASE1:	lodsb
		test	al,80h
		jnz	@@TCOLOR1


@@DCOLOR1:      add	si,ax
		sub	bx,ax
		jg	@@PHASE1
		jz	@@DONE1
		mov	ax,bx
		neg	ax
		sub	si,ax
		mov	bx,@@XT
		jmp	@@DCOLOR2

@@TCOLOR1:      and	al,7fh
		sub	bx,ax
		jg	@@PHASE1
		jz	@@DONE1
		mov	ax,bx
		neg	ax
		mov	bx,@@XT
		jmp	@@TCOLOR2

@@DONE1:        mov	bx,@@XT

;--------------------------------------------------------------
		;Actual copying
@@PHASE2:	lodsb
		test	al,80h
		jnz	@@TCOLOR2

@@DCOLOR2:	sub	bx,ax       ;Test X Total
		jge	@@NORIGHTCLIP   ;
		add	ax,bx       ;(right clip
@@NORIGHTCLIP:

		add	dx,ax		;;Skip @@SRC2
		sub     [@@XT2],ax
		jg	@@NOOVERSRC2
;		add	dx,ax  ;;End of line
		sub	dx,320d ;;Go back to beginning
		mov	[@@XT2],320d
@@NOOVERSRC2:

		mov	cx,ax           ;copy data
		shr	cx,1
		jnc	@@NOEXTRA1
		movsb
@@NOEXTRA1:	shr	cx,1
		jnc	@@NOEXTRA2
		movsw
@@NOEXTRA2:     rep     movsd

		and	bx,bx
		jg	@@PHASE2
		jmp	@@DONE2



@@TCOLOR2:      and	al,7fh
		sub	bx,ax
		jge	@@NORIGHTCLIP2   ;
		add	ax,bx       ;(right clip
@@NORIGHTCLIP2:

		xchg	dx,si

		sub     [@@XT2],ax
		jg	@@NOOVERSRC3
		add	ax,[@@XT2]  ;;bytes to end of line
		mov	cx,ax

		shr	cx,1
		jnc	@@aNOEXTRA1
		db	64h
		movsb
@@aNOEXTRA1:	shr	cx,1
		jnc	@@aNOEXTRA2
		db	64h
		movsw
@@aNOEXTRA2:	db	64h
		rep     movsd

		sub	si,320d ;;Go back to beginning
		mov	ax,320
		xchg	ax,[@@XT2] ;@@XT2=320  ax=@@XT2
		neg	ax  ;Bytes on next line
@@NOOVERSRC3:

		mov	cx,ax
		shr	cx,1
		jnc	@@bNOEXTRA1
		db	64h
		movsb
@@bNOEXTRA1:	shr	cx,1
		jnc	@@bNOEXTRA2
		db	64h
		movsw
@@bNOEXTRA2:	db	64h
		rep     movsd

		xchg	dx,si


		and	bx,bx
		jg	@@PHASE2


@@DONE2:        pop	[@@XT2]
		pop	dx
		pop	di

		inc	@@YST

		dec	[@@YT]		;Decrease Y total
		jz	@@DONE		;
		jmp	@@YLOOP

@@DONE:         pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp,bp
		pop	bp
		ret
_PutBackgroundOverlap	ENDP

	 *









_PutBackgroundTriOverlap  PROC NEAR
	ARG	@@SRC:DWORD,@@SRC2:DWORD,@@SRC3:DWORD,@@DEST:DWORD,@@XST:DWORD,@@YST:DWORD,@@XST2:DWORD,@@YST2:DWORD,@@XST3:DWORD,@@YST3:DWORD,@@DX:DWORD,@@DY:DWORD,@@YLIST:DWORD,@@YLIST2:DWORD
	LOCAL	@@XT:DWORD,@@YT:DWORD, @@XT2:DWORD,@@RUN2:DWORD, @@XT3:DWORD,@@PTR3:DWORD,@@YL:DWORD =LLEN

		push	ebp
		mov	ebp,esp
		sub	esp,LLEN

		push	ebx
		push	esi
		push	edi

		;Position 1st layer pointer
		mov	esi,@@SRC

		;Position 2nd layer pointer
		mov	ebx,@@SRC2
		movzx	eax,word ptr [ebx+2]
		mov	[@@YL],eax

		;Position 3rd layer pointer
		xor   eax,eax
		imul	eax,@@YST3,320  ;;Position [PTR3] pointer
		add	eax,@@XST3
		add	eax,@@SRC3
		mov	[@@PTR3],eax


		;Position dest pointer
		mov	edi,@@DEST
		imul	eax,@@DY,320  ;;Position es:di pointer
		add	eax,@@DX
		add	edi,eax

		;Find out how many total pixels needed to draw on 1st layer
		lodsw		;Get xlength
		sub	eax,@@XST ;number of x pixels remaining in bg
		mov	ebx,320d
		sub	ebx,@@DX ;number of x pixels left on screen
		cmp	ebx,eax
		jge 	@@TOOMANYX
		mov	eax,ebx
@@TOOMANYX:     mov	@@XT,eax  ;Total number of pixels to copy

		lodsw		;Get ylength
		sub	eax,@@YST ;number of y pixels remaining in bg
		mov	ebx,[@@YL];195;200d
		sub	ebx,@@DY ;number of y pixels left on screen
		cmp	ebx,eax
		jge 	@@TOOMANYY
		mov	eax,ebx
@@TOOMANYY:     mov	@@YT,eax  ;Total number of pixels to copy

		;Find out how many pixels left on 2nd layer
		mov   ebx,@@SRC2
		mov	ax,[ebx] ;Get width
		sub	eax,@@XST2
		mov	@@XT2,eax

		;Find out how many pixels are left on the 3rd layer scan line
		mov	eax,320
		sub	eax,@@XST3
		mov	@@XT3,eax

		xor	ecx,ecx      ;loop

@@YLOOP:        ;Go to beginning of 1st layer scan line
		mov	ebx,@@YST
		shl	ebx,1
		add   ebx,@@YLIST
		movzx	esi,word ptr [ebx] ;Get address of line
		add	esi,[@@SRC]

		;Go to beginning of 2nd layer scan line
		mov	ebx,@@YST2
		shl	ebx,1
		add   ebx,@@YLIST2
		movzx	edx,word ptr [ebx] ;Get address of line
		add	edx,[@@SRC2]



		lea	eax,[edi+320]  ;Find beginning of next dest line
		push	eax
		mov	eax,[@@PTR3]  ;Find begin of next SRC3 line
		add	eax,320
		push	eax
;--------------------------------------------------------
		;Left Clip
		push	[@@XT3]
		push	[@@XT2]

	 ;----Skip necessary bytes in SRC1
		mov	ebx,@@XST
		xor	eax,eax
@@PHASE1A:	lodsb
		test	al,80h
		jnz	@@TCOLOR1A

@@DCOLOR1A:
		add	esi,eax
		sub	ebx,eax
		jg	@@PHASE1A
		neg	ebx
		sub	esi,ebx
		jmp	@@DONE1A

@@TCOLOR1A:     and	al,7fh
		sub	ebx,eax
		jg	@@PHASE1A
		neg	ebx
		or	 bl,80h 	;Set Transp bit
@@DONE1A:
		push	ebx     ;SAVE*************** runcount SRC1


	 ;----Skip necessary bytes in SRC2

		mov	ebx,@@XST2
		xchg	esi,edx  ;Position si
		xor	eax,eax
@@PHASE1B:
		lodsb
		test	al,80h
		jnz	@@TCOLOR1B

@@DCOLOR1B:     add	esi,eax
		sub	ebx,eax
		jg	@@PHASE1B
		neg	ebx
		sub	esi,ebx
		jmp	@@DONE1B

@@TCOLOR1B:     and	al,7fh
		sub	ebx,eax
		jg	@@PHASE1B
		neg	ebx
		or	bl,80h 	;Set Transp bit
@@DONE1B:
		xchg	esi,edx  ;Switch back
		mov	[@@RUN2],ebx  ;SAVE ******** runcount SRC2


@@DONENL1:

		mov	ebx,@@XT
		pop	eax
		jmp	@@RESUME2

;--------------------------------------------------------------
;***************************************************************
		;Actual copying
@@PHASE2:	lodsb
@@RESUME2:	test	al,80h
		jnz	@@TCOLOR2

	  ;****************************OPAQUE COLOR
@@DCOLOR2:	sub	ebx,eax       ;Test X Total
		jge	@@NORIGHTCLIP   ;
		add	eax,ebx       ;(right clip
@@NORIGHTCLIP:

	;--------------Advance SRC2
		push	eax
		push	ebx
		mov	ebx,eax                   ;bytes to advance
		xchg	esi,edx  ;Position si
		mov	eax,[@@RUN2]
		jmp	@@A1RESUME

@@A1PHASE:
		lodsb
@@A1RESUME:	test	al,80h
		jnz	@@A1TCOLOR

@@A1DCOLOR:     add	esi,eax
		sub	ebx,eax
		jg	@@A1PHASE
		neg	ebx
		sub	esi,ebx
		jmp	@@A1DONE

@@A1TCOLOR:    	and	eax,7fh
		sub	ebx,eax
		jg	@@A1PHASE
		neg	ebx
		or	bl,80h 	;Set Transp bit
@@A1DONE:
		xchg	esi,edx  ;Switch back
		mov	[@@RUN2],ebx  ;SAVE ******** runcount SRC2
		pop	ebx
		pop	eax

	;--------------Advance SRC3
		add	[@@PTR3],eax		;;Skip @@SRC2
		sub     [@@XT3],eax
		jg	@@NOOVERSRC3
		sub	[@@PTR3],320d ;;Go back to beginning
		mov	[@@XT3],320d
@@NOOVERSRC3:


@@DONENL2:
	;--------------Copy    SRC1
		mov	ecx,eax           ;copy data
		shr	ecx,1
		jnc	@@NOEXTRA1
		movsb
@@NOEXTRA1:	shr	ecx,1
		jnc	@@NOEXTRA2
		movsw
@@NOEXTRA2:     rep     movsd

		and	ebx,ebx
		jg	@@PHASE2
		jmp	@@DONE2






	  ;****************************TRANSPARENT COLOR
@@TCOLOR2:      and	al,7fh
		sub	ebx,eax
		jge	@@NORIGHTCLIP2   ;
		add	eax,ebx       ;(right clip
@@NORIGHTCLIP2:

		xchg	edx,esi

	;-------------------------------------------------Copy SRC2
		push	ebx

		mov	ebx,eax                   ;bytes to Copy
		mov	eax,[@@RUN2]
		jmp	@@A2RESUME

@@A2PHASE:      	lodsb
@@A2RESUME:	test	al,80h
		jnz	@@A2TCOLOR

			;--------OPAQUE COLOR
@@A2DCOLOR:     sub	ebx,eax       ;Test X Total
		jge	@@A2NORIGHTCLIP   ;
		add	eax,ebx       ;(right clip)
@@A2NORIGHTCLIP:
	;--------------Advance SRC3
		add	[@@PTR3],eax		;;Skip @@SRC2
		sub     [@@XT3],eax
		jg	@@A2NOOVERSRC
		sub	[@@PTR3],320d ;;Go back to beginning
		mov	[@@XT3],320d
@@A2NOOVERSRC:
	;--------------Copy  SRC2
		mov     ecx,eax		;COPY stuff from SRC2 to dest
;		db	64h
;		rep	movsb
		shr	ecx,1
		jnc	@@bNOEXTRA1
		movsb
@@bNOEXTRA1:	shr	ecx,1
		jnc	@@bNOEXTRA2
		movsw
@@bNOEXTRA2:  		rep     movsd


		and	ebx,ebx
		jg	@@A2PHASE
		neg	ebx
		jmp	@@A2DONE



			;--------TRANSP COLOR
@@A2TCOLOR:    	and	al,7fh
		sub	ebx,eax
		jge	@@A2NORIGHTCLIP2   ;
		add	eax,ebx       ;(right clip
@@A2NORIGHTCLIP2:
		 ;----------------Copy SRC3
		xchg	esi,[@@PTR3]

		sub    [@@XT3],eax
		jg	@@A2NOOVERSRC2
		add	eax,[@@XT3]  ;;bytes to end of line
		mov	ecx,eax
		shr	ecx,1
		jnc	@@cNOEXTRA1
		movsb
@@cNOEXTRA1:	shr	ecx,1
		jnc	@@cNOEXTRA2
		movsw
@@cNOEXTRA2:  	rep     movsd

		sub	esi,320d ;;Go back to beginning
		mov	eax,320
		xchg	eax,[@@XT3] ;@@XT3=320  ax=@@XT3
		neg	eax  ;Bytes on next line
@@A2NOOVERSRC2:
		mov	ecx,eax
		shr	ecx,1
		jnc	@@dNOEXTRA1
		movsb
@@dNOEXTRA1:	shr	ecx,1
		jnc	@@dNOEXTRA2
		movsw
@@dNOEXTRA2:  	rep     movsd
		xchg	esi,[@@PTR3]


		and	ebx,ebx
		jg	@@A2PHASE
		neg	ebx
		or	bl,80h 	;Set Transp bit
@@A2DONE:
		mov	[@@RUN2],ebx  ;SAVE ******** runcount SRC2
		pop	ebx


		xchg	edx,esi

@@DONENL3:

		and	ebx,ebx
		jg	@@PHASE2


@@DONE2:        pop	[@@XT2]
		pop	[@@XT3]
		pop	[@@PTR3]
		pop	edi

		inc	@@YST
		inc	@@YST2

		dec	[@@YT]		;Decrease Y total
		jz	@@DONE		;
		jmp	@@YLOOP

@@DONE:         pop	edi
		pop	esi
		pop	ebx
		mov	esp,ebp
		pop	ebp
		ret
_PutBackgroundTriOverlap	ENDP



_PutBackground  PROC near
	ARG	@@SRC:DWORD,@@DEST:DWORD,@@XST:DWORD,@@YST:DWORD,@@DX:DWORD,@@DY:DWORD,@@YLIST:DWORD
	LOCAL	@@XT:DWORD,@@YT:DWORD =LLEN

		push	ebp
		mov	ebp,esp
		sub	esp,LLEN

		push	ebx
		push	esi
		push	edi

		mov	esi,@@SRC
		mov	edi,@@DEST

		imul	eax,@@DY,320  ;;Position es:di pointer
		add	eax,@@DX
;		sub	ax,@@XST
		add	edi,eax

		lodsw		;Get xlength
		sub	eax,@@XST ;number of x pixels remaining in bg
		mov	ebx,320d
		sub	ebx,@@DX ;number of x pixels left on screen
		cmp	ebx,eax
		jge 	TOOMANYX
		mov	eax,ebx
TOOMANYX:       mov	@@XT,eax  ;Total number of pixels to copy

;		mov	@@XT,320


		lodsw		;Get ylength
		sub	eax,@@YST ;number of y pixels remaining in bg
		mov	ebx,195;200d
		sub	ebx,@@DY ;number of y pixels left on screen
		cmp	ebx,eax
		jge 	TOOMANYY
		mov	eax,ebx
TOOMANYY:       mov	@@YT,eax  ;Total number of pixels to copy

;		mov	@@YT,100


		xor	ecx,ecx      ;loop

@@YLOOP:	mov     ebx,@@YLIST
		add	ebx,@@YST
		add	ebx,@@YST
		movzx	esi,word ptr [ebx] ;Get address of line
		add	esi,[@@SRC]

		lea	eax,[edi+320]
		push	eax
;--------------------------------------------------------
		;Left Clip
		mov	ebx,@@XST
		xor	eax,eax

@@PHASE1:	lodsb
		test	al,80h
		jnz	@@TCOLOR1


@@DCOLOR1:      add	esi,eax
		sub	ebx,eax
		jg	@@PHASE1
		jz	@@DONE1
		mov	eax,ebx
		neg	eax
		sub	esi,eax
		mov	ebx,@@XT
		jmp	@@DCOLOR2

@@TCOLOR1:      and	al,7fh
		sub	ebx,eax
		jg	@@PHASE1
		jz	@@DONE1
		mov	eax,ebx
		neg	eax
		mov	ebx,@@XT
		jmp	@@TCOLOR2

@@DONE1:        mov	ebx,@@XT

;--------------------------------------------------------------
		;Actual copying
@@PHASE2:	lodsb
		test	al,80h
		jnz	@@TCOLOR2

@@DCOLOR2:	sub	ebx,eax       ;Test X Total
		jge	@@NORIGHTCLIP   ;
		add	eax,ebx       ;(right clip
@@NORIGHTCLIP:

		mov	ecx,eax           ;copy data
;		rep	movsb
		shr	ecx,1
		jnc	@@NOEXTRA1
		movsb
@@NOEXTRA1:	shr	ecx,1
		jnc	@@NOEXTRA2
		movsw
@@NOEXTRA2:     rep     movsd

		and	ebx,ebx
		jg	@@PHASE2
		jmp	@@DONE2

@@TCOLOR2:      and	al,7fh
		add	edi,eax
		sub	ebx,eax
		jg	@@PHASE2


@@DONE2:	pop	edi

		inc	@@YST

		dec	[@@YT]		;Decrease Y total
		jz	@@DONE		;
		jmp	@@YLOOP

@@DONE:         pop	edi
		pop	esi
		pop	ebx
		mov	esp,ebp
		pop	ebp
		ret
_PutBackground	ENDP




		END
