	.386p
	model flat
	ifndef	??version
	?debug	macro
	endm
	endif
	?debug	S "clip.cpp"
	?debug	T "clip.cpp"
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
_DATA	segment dword public use32 'DATA'
_DATA	ends
_BSS	segment dword public use32 'BSS'
_BSS	ends
DGROUP	group	_BSS,_DATA
_BSS	segment dword public use32 'BSS'
_clipboard	label	byte
	db	4	dup(?)
_BSS	ends
_DATA	segment dword public use32 'DATA'
_$C	label	dword
	dd	@@$xt$p8clipdata
	dd	16399
	dd	-4
	dd	0
_$D	label	dword
	dd	0
	dd	-36
	dw	0
	dw	5
	dd	0
	dd	0
	dw	8
	dw	5
	dd	0
	dd	_$C
_DATA	ends
_TEXT	segment dword public use32 'CODE'
	align	4
@clipdata@1$bctr$qqrv	proc	near
?live1@0:
   ;	
   ;	clipdata::clipdata()
   ;	
@1:
	push      ebp
	mov       ebp,esp
	add       esp,-36
	push      ebx
	push      esi
	push      edi
	mov       ebx,eax
	mov       eax,offset _$D
	call      @__InitExceptBlock
	mov       dword ptr [ebx],offset @@clipdata@5+12
   ;	
   ;	{
   ;	 clipboard.adddata(this);
   ;	
?live1@16: ; EBX = this
	mov       esi,ebx
	mov       edi,dword ptr [_clipboard]
	test      edi,edi
	je        short @2
	mov       dword ptr [ebp-4],edi
	mov       eax,dword ptr [ebp-4]
	test      eax,eax
	je        short @3
	push      0
	mov       edx,dword ptr [ebp-4]
	push      edx
	call      @__GetPolymorphicDTC$qpvui
	add       esp,8
	mov       ecx,dword ptr fs:[4]
	add       dword ptr [ecx-4],eax
	mov       word ptr [ebp-20],20
	mov       edx,3
	mov       eax,dword ptr [ebp-4]
	mov       ecx,dword ptr [eax]
	call      dword ptr [ecx]
	mov       word ptr [ebp-20],8
@2:
@3:
	mov       dword ptr [_clipboard],esi
   ;	
   ;	}
   ;	
	mov       eax,dword ptr fs:[4]
	inc       dword ptr [eax-4]
	mov       edx,dword ptr [ebp-36]
	mov       dword ptr fs:[0],edx
	mov       eax,ebx
?live1@48: ; 
@7:
@6:
	pop       edi
	pop       esi
	pop       ebx
	mov       esp,ebp
	pop       ebp
	ret 
@clipdata@1$bctr$qqrv	endp
	align	4
@clip_images@1$bctr$qqrp5framep9objectdef	proc	near
?live1@64:
   ;	
   ;	clip_images::clip_images(struct frame *fptr,class objectdef *tod)
   ;	
@8:
	push      ebx
	push      esi
	push      edi
	push      ebp
	mov       edi,ecx
	mov       esi,edx
	mov       ebx,eax
   ;	
   ;	 :clipdata()
   ;	
?live1@80: ; EBX = this, ESI = fptr, EDI = tod
	mov       eax,ebx
	call      @clipdata@1$bctr$qqrv
	mov       dword ptr [ebx],offset @@clip_images@5+12
   ;	
   ;	{
   ;	 od=tod;
   ;	
	mov       dword ptr [ebx+4],edi
   ;	
   ;	 numimages=fptr->numimages;
   ;	
?live1@112: ; EBX = this, ESI = fptr
	xor       eax,eax
	mov       al,byte ptr [esi+13]
	mov       dword ptr [ebx+8],eax
   ;	
   ;	 size=numimages*sizeof(image);
   ;	
?live1@128: ; EBX = this, ESI = fptr, EAX = @temp2
	mov       edi,eax
	add       edi,edi
	lea       edi,dword ptr [edi+2*edi]
	mov       dword ptr [ebx+12],edi
   ;	
   ;	 i=(image *)malloc(size);
   ;	
?live1@144: ; EBX = this, ESI = fptr, EDI = @temp3
	push      edi
	call      _malloc
	pop       ecx
	mov       ebp,eax
	mov       dword ptr [ebx+16],ebp
   ;	
   ;	
   ;	 memcpy(i,fptr->getimgptr(),size);
   ;	
?live1@160: ; EBX = this, ESI = fptr, EBP = @temp4
	test      esi,esi
	mov       eax,dword ptr [ebx+12]
	push      eax
	jne       short @9
	xor       edx,edx
	jmp       short @10
@9:
	lea       edx,dword ptr [esi+14]
@10:
	push      edx
	push      ebp
	call      _memcpy
	add       esp,12
   ;	
   ;	 msg.printf(3,"%d images copied",numimages);
   ;	
?live1@176: ; EBX = this
	mov       eax,dword ptr [ebx+8]
	push      eax
	push      offset s@
	push      3
	push      offset _msg
	call      @msgbuffer@1printf$qipce
	add       esp,16
   ;	
   ;	}
   ;	
	mov       ecx,dword ptr fs:[4]
	inc       dword ptr [ecx-4]
	mov       eax,ebx
?live1@208: ; 
@12:
@11:
	pop       ebp
	pop       edi
	pop       esi
	pop       ebx
	ret 
@clip_images@1$bctr$qqrp5framep9objectdef	endp
	align	4
@clip_images@1$bdtr$qqrv	proc	near
?live1@224:
   ;	
   ;	clip_images::~clip_images()
   ;	
@13:
	push      ebx
	push      esi
	mov       esi,edx
	mov       ebx,eax
   ;	
   ;	{
   ;	
?live1@240: ; EBX = this, ESI = $delflag
	mov       eax,dword ptr fs:[4]
	dec       dword ptr [eax-4]
	test      ebx,ebx
	je        short @14
	mov       dword ptr [ebx],offset @@clip_images@5+12
   ;	
   ;	 if (i) free(i);
   ;	
	mov       eax,dword ptr [ebx+16]
	test      eax,eax
	je        short @15
	push      eax
	call      _free
	pop       ecx
@15:
	mov       edx,dword ptr fs:[4]
	dec       dword ptr [edx-4]
	test      esi,1
	je        short @16
	push      ebx
	call      @$bdele$qpv
	pop       ecx
   ;	
   ;	}
   ;	
?live1@272: ; 
@16:
@14:
@17:
	pop       esi
	pop       ebx
	ret 
@clip_images@1$bdtr$qqrv	endp
	align	4
@clip_images@1paste$qqrv	proc	near
?live1@288:
   ;	
   ;	void clip_images::paste()
   ;	
@18:
	push      ebx
	push      esi
	push      edi
	push      ecx
	mov       ebx,eax
   ;	
   ;	{
   ;	 if (!objspace) return;
   ;	
?live1@304: ; EBX = this
	mov       eax,dword ptr [_objspace]
	test      eax,eax
	je        @20
   ;	
   ;	 object *o=objspace->p;
   ;	
?live1@320: ; EBX = this, EAX = @temp5
	mov       esi,dword ptr [eax+89]
   ;	
   ;	 if (!o) return;
   ;	
?live1@336: ; EBX = this, ESI = o
	test      esi,esi
	je        @20
   ;	
   ;	 frame *fptr=o->fptr;
   ;	
	mov       eax,dword ptr [esi+34]
   ;	
   ;	 series *s=o->cs;
   ;	
?live1@368: ; EAX = fptr, EBX = this, ESI = o
	mov       edi,dword ptr [esi+26]
   ;	
   ;	 if (!fptr || !s || o->od!=od) return;
   ;	
?live1@384: ; EAX = fptr, EBX = this, ESI = o, EDI = s
	test      eax,eax
	je        @20
	test      edi,edi
	je        @20
	mov       edx,dword ptr [esi+21]
	cmp       edx,dword ptr [ebx+4]
	jne       @20
   ;	
   ;	
   ;	 //remove old images
   ;	 int oldimgsize=fptr->numimages*sizeof(image);
   ;	
	xor       ecx,ecx
	mov       cl,byte ptr [eax+13]
	mov       edx,ecx
	add       edx,edx
   ;	
   ;	 s->deletebytes(fptr->getimgptr(),oldimgsize); //delete old images
   ;	
	test      eax,eax
	lea       edx,dword ptr [edx+2*edx]
	mov       dword ptr [esp],edx
	jne       short @24
	xor       edx,edx
	jmp       short @25
@24:
	lea       edx,dword ptr [eax+14]
@25:
	mov       ecx,dword ptr [esp]
	mov       eax,edi
	call      @series@1deletebytes$qqrpvi
   ;	
   ;	 fptr=o->resetfptr();
   ;	
?live1@464: ; EBX = this, ESI = o, EDI = s
	mov       eax,esi
	call      @object@1resetfptr$qqrv
   ;	
   ;	 fptr->size-=oldimgsize;
   ;	
?live1@480: ; EAX = fptr, EBX = this, ESI = o, EDI = s
	mov       dx,word ptr [esp]
	sub       word ptr [eax],dx
   ;	
   ;	 fptr->numimages=0;
   ;	
	mov       byte ptr [eax+13],0
   ;	
   ;	
   ;	 //add new images
   ;	 s->insertbytes(fptr->getimgptr(),size);
   ;	
	test      eax,eax
	jne       short @26
	xor       edx,edx
	jmp       short @27
@26:
	lea       edx,dword ptr [eax+14]
@27:
	mov       ecx,dword ptr [ebx+12]
	mov       eax,edi
	call      @series@1insertbytes$qqrpvi
   ;	
   ;	 fptr=o->resetfptr();
   ;	
?live1@528: ; EBX = this, ESI = o
	mov       eax,esi
	call      @object@1resetfptr$qqrv
   ;	
   ;	 fptr->size+=size;
   ;	
?live1@544: ; EAX = fptr, EBX = this
	mov       dx,word ptr [ebx+12]
	add       word ptr [eax],dx
   ;	
   ;	 fptr->numimages=numimages;
   ;	 memcpy(fptr->getimgptr(),i,size);
   ;	
	test      eax,eax
	mov       cl,byte ptr [ebx+8]
	mov       byte ptr [eax+13],cl
	mov       edx,dword ptr [ebx+12]
	push      edx
	mov       ecx,dword ptr [ebx+16]
	push      ecx
	jne       short @28
	xor       edx,edx
	jmp       short @29
@28:
	lea       edx,dword ptr [eax+14]
@29:
	push      edx
	call      _memcpy
	add       esp,12
   ;	
   ;	 msg.printf(3,"pasted %d image(s), %d bytes",numimages,size);
   ;	
?live1@608: ; EBX = this
	mov       eax,dword ptr [ebx+12]
	push      eax
	mov       ecx,dword ptr [ebx+8]
	push      ecx
	push      offset s@+17
	push      3
	push      offset _msg
	call      @msgbuffer@1printf$qipce
	add       esp,20
   ;	
   ;	}
   ;	
?live1@624: ; 
@30:
@20:
	pop       edx
	pop       edi
	pop       esi
	pop       ebx
	ret 
@clip_images@1paste$qqrv	endp
	align	4
@clip_frame@1$bctr$qqrp5frameip9objectdef	proc	near
?live1@640:
   ;	
   ;	clip_frame::clip_frame(struct frame *fptr,int tnumf,class objectdef *tod)
   ;	
@31:
	push      ebp
	mov       ebp,esp
	push      ebx
	push      esi
	push      edi
	mov       esi,ecx
	mov       edi,edx
	mov       ebx,eax
   ;	
   ;	 :clipdata(),od(tod)
   ;	
?live1@656: ; EBX = this, ESI = tnumf, EDI = fptr
	mov       eax,ebx
	call      @clipdata@1$bctr$qqrv
	mov       dword ptr [ebx],offset @@clip_frame@5+12
   ;	
   ;	{
   ;	 for (f=fptr,size=0,numf=0; numf<tnumf; numf++,size+=f->size,f=f->next());
   ;	
	xor       ecx,ecx
	mov       edx,dword ptr [ebp+8]
	xor       eax,eax
	mov       dword ptr [ebx+4],edx
	mov       dword ptr [ebx+16],edi
	mov       dword ptr [ebx+12],ecx
	mov       dword ptr [ebx+8],eax
	jmp       short @33
@32:
	inc       dword ptr [ebx+8]
	mov       edx,dword ptr [ebx+16]
	movzx     ecx,word ptr [edx]
	add       dword ptr [ebx+12],ecx
	mov       eax,dword ptr [ebx+16]
	movzx     edx,word ptr [eax]
	add       eax,edx
	mov       dword ptr [ebx+16],eax
@33:
	mov       eax,dword ptr [ebx+8]
	cmp       esi,eax
	jg        short @32
   ;	
   ;	 //get memory for frames
   ;	 f=(frame *)malloc(size);
   ;	
?live1@752: ; EBX = this, EDI = fptr
	mov       ecx,dword ptr [ebx+12]
	push      ecx
	call      _malloc
	pop       ecx
	mov       esi,eax
	mov       dword ptr [ebx+16],esi
   ;	
   ;	 memcpy(f,fptr,size); //copy frames
   ;	
?live1@768: ; EBX = this, EDI = fptr, ESI = @temp4
	mov       eax,dword ptr [ebx+12]
	push      eax
	push      edi
	push      esi
	call      _memcpy
	add       esp,12
   ;	
   ;	 msg.printf(3,"%d frames copied",numf);
   ;	
?live1@784: ; EBX = this
	mov       edx,dword ptr [ebx+8]
	push      edx
	push      offset s@+46
	push      3
	push      offset _msg
	call      @msgbuffer@1printf$qipce
	add       esp,16
   ;	
   ;	
   ;	}
   ;	
	mov       ecx,dword ptr fs:[4]
	inc       dword ptr [ecx-4]
	mov       eax,ebx
?live1@816: ; 
@36:
@35:
	pop       edi
	pop       esi
	pop       ebx
	pop       ebp
	ret       4
@clip_frame@1$bctr$qqrp5frameip9objectdef	endp
	align	4
@clip_frame@1$bdtr$qqrv	proc	near
?live1@832:
   ;	
   ;	clip_frame::~clip_frame() {if (f) free(f);}
   ;	
@37:
	push      ebx
	push      esi
	mov       esi,edx
	mov       ebx,eax
	mov       eax,dword ptr fs:[4]
	dec       dword ptr [eax-4]
	test      ebx,ebx
	je        short @38
	mov       dword ptr [ebx],offset @@clip_frame@5+12
	mov       eax,dword ptr [ebx+16]
	test      eax,eax
	je        short @39
	push      eax
	call      _free
	pop       ecx
@39:
	mov       edx,dword ptr fs:[4]
	dec       dword ptr [edx-4]
	test      esi,1
	je        short @40
	push      ebx
	call      @$bdele$qpv
	pop       ecx
@40:
@38:
@41:
	pop       esi
	pop       ebx
	ret 
@clip_frame@1$bdtr$qqrv	endp
	align	4
@clip_frame@1paste$qqrv	proc	near
?live1@864:
   ;	
   ;	void clip_frame::paste()
   ;	
@42:
	push      ebx
	push      esi
	push      edi
	mov       ebx,eax
   ;	
   ;	{
   ;	 if (!objspace) return;
   ;	
?live1@880: ; EBX = this
	mov       eax,dword ptr [_objspace]
	test      eax,eax
	je        short @44
   ;	
   ;	 object *o=objspace->p;
   ;	
?live1@896: ; EBX = this, EAX = @temp4
	mov       esi,dword ptr [eax+89]
   ;	
   ;	 if (!o) return;
   ;	
?live1@912: ; EBX = this, ESI = o
	test      esi,esi
	je        short @44
   ;	
   ;	 frame *fptr=o->fptr;
   ;	
	mov       eax,dword ptr [esi+34]
   ;	
   ;	 series *s=o->cs;
   ;	
?live1@944: ; EBX = this, ESI = o, EAX = fptr
	mov       edi,dword ptr [esi+26]
   ;	
   ;	 if (!s || o->od!=od) return;
   ;	
?live1@960: ; EBX = this, ESI = o, EDI = s, EAX = fptr
	test      edi,edi
	je        short @44
	mov       edx,dword ptr [esi+21]
	cmp       edx,dword ptr [ebx+4]
	jne       short @44
   ;	
   ;	
   ;	 fptr=(frame *)s->insertbytes(fptr,size);
   ;	
	mov       ecx,dword ptr [ebx+12]
	mov       edx,eax
	mov       eax,edi
	call      @series@1insertbytes$qqrpvi
   ;	
   ;	 memcpy(fptr,f,size);
   ;	
	mov       ecx,dword ptr [ebx+12]
	push      ecx
	mov       edx,dword ptr [ebx+16]
	push      edx
	push      eax
	call      _memcpy
	add       esp,12
   ;	
   ;	 s->nf+=numf; //insert frame in there
   ;	
?live1@1008: ; EBX = this, ESI = o, EDI = s
	mov       al,byte ptr [ebx+8]
	add       byte ptr [edi+9],al
   ;	
   ;	 o->nf=s->nf;
   ;	
	mov       cl,byte ptr [edi+9]
	mov       byte ptr [esi+43],cl
   ;	
   ;	 msg.printf(3,"pasted %d frame(s), %d bytes",numf,size);
   ;	
?live1@1040: ; EBX = this
	mov       eax,dword ptr [ebx+12]
	push      eax
	mov       edx,dword ptr [ebx+8]
	push      edx
	push      offset s@+63
	push      3
	push      offset _msg
	call      @msgbuffer@1printf$qipce
	add       esp,20
   ;	
   ;	}
   ;	
?live1@1056: ; 
@48:
@44:
	pop       edi
	pop       esi
	pop       ebx
	ret 
_INIT_	segment word public use32 'INITDATA'
_INIT_	ends
@clip_frame@1paste$qqrv	endp
_TEXT	ends
_INIT_	segment word public use32 'INITDATA'
	db	0
	db	32
	dd	@_STCON0_$qqrv
_INIT_	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
_DATA	segment dword public use32 'DATA'
_$E	label	dword
	dd	@@$xt$9CLIPBOARD
	dd	0
	dd	_clipboard
	dd	0
_$F	label	dword
	dd	0
	dd	-32
	dw	0
	dw	5
	dd	0
	dd	_$E
_DATA	ends
_TEXT	segment dword public use32 'CODE'
	align	4
@_STCON0_$qqrv	proc	near
?live1@1072:
@49:
	push      ebp
	mov       ebp,esp
	add       esp,-32
	mov       eax,offset _$F
	call      @__InitExceptBlock
	mov       word ptr [ebp-16],8
	xor       edx,edx
	mov       dword ptr [_clipboard],edx
	mov       ecx,dword ptr fs:[4]
	inc       dword ptr [ecx-4]
	mov       eax,dword ptr [ebp-32]
	mov       dword ptr fs:[0],eax
@50:
	mov       esp,ebp
	pop       ebp
	ret 
_EXIT_	segment word public use32 'EXITDATA'
_EXIT_	ends
@_STCON0_$qqrv	endp
_TEXT	ends
_EXIT_	segment word public use32 'EXITDATA'
	db	0
	db	32
	dd	@_STDES0_$qqrv
_EXIT_	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
_DATA	segment dword public use32 'DATA'
_$G	label	dword
	dd	@@$xt$p8clipdata
	dd	16399
	dd	-4
	dd	0
_$H	label	dword
	dd	0
	dd	-36
	dw	0
	dw	5
	dd	0
	dd	0
	dw	8
	dw	5
	dd	0
	dd	_$G
_DATA	ends
_TEXT	segment dword public use32 'CODE'
	align	4
@_STDES0_$qqrv	proc	near
?live1@1104:
@51:
	push      ebp
	mov       ebp,esp
	add       esp,-36
	mov       eax,offset _$H
	push      ebx
	call      @__InitExceptBlock
	mov       edx,dword ptr fs:[4]
	dec       dword ptr [edx-4]
	mov       ebx,dword ptr [_clipboard]
	test      ebx,ebx
	je        short @52
	mov       dword ptr [ebp-4],ebx
	mov       eax,dword ptr [ebp-4]
	test      eax,eax
	je        short @53
	push      0
	mov       edx,dword ptr [ebp-4]
	push      edx
	call      @__GetPolymorphicDTC$qpvui
	add       esp,8
	mov       ecx,dword ptr fs:[4]
	add       dword ptr [ecx-4],eax
	mov       word ptr [ebp-20],20
	mov       edx,3
	mov       eax,dword ptr [ebp-4]
	mov       ecx,dword ptr [eax]
	call      dword ptr [ecx]
	mov       word ptr [ebp-20],8
@52:
@53:
	mov       eax,dword ptr [ebp-36]
	mov       dword ptr fs:[0],eax
@56:
	pop       ebx
	mov       esp,ebp
	pop       ebp
	ret 
@_STDES0_$qqrv	endp
_TEXT	ends
_TEXT	segment dword public use32 'CODE'
@$xt$9CLIPBOARD	segment virtual
@@$xt$9CLIPBOARD	label	byte
	dd	4
	dw	3
	dw	48
	dd	-1
	dd	3
	dw	60
	dw	64
	dd	0
	dw	0
	dw	0
	dd	0
	dd	1
	dd	1
	dd	@@CLIPBOARD@1$bdtr$qqrv
	dw	131
	dw	68
	db	67
	db	76
	db	73
	db	80
	db	66
	db	79
	db	65
	db	82
	db	68
	db	0
	db	0
	db	0
	dd	0
	dd	0
	dd	0
@$xt$9CLIPBOARD	ends
_TEXT	ends
_TEXT	segment dword public use32 'CODE'
@$xt$p8clipdata	segment virtual
@@$xt$p8clipdata	label	dword
	dd	4
	dw	16
	dw	12
	dd	@@$xt$8clipdata
	db	99
	db	108
	db	105
	db	112
	db	100
	db	97
	db	116
	db	97
	db	32
	db	42
	db	0
@$xt$p8clipdata	ends
_TEXT	ends
_DATA	segment dword public use32 'DATA'
_$I	label	dword
	dd	@@$xt$p8clipdata
	dd	16399
	dd	-4
	dd	0
_$J	label	dword
	dd	0
	dd	-36
	dw	0
	dw	5
	dd	0
	dd	0
	dw	8
	dw	5
	dd	-1
	dd	_$I
_DATA	ends
_TEXT	segment dword public use32 'CODE'
@CLIPBOARD@1$bdtr$qqrv	segment virtual
	align	2
@@CLIPBOARD@1$bdtr$qqrv	proc	near
?live16387@0:
@57:
	push      ebp
	mov       ebp,esp
	add       esp,-36
	push      ebx
	push      esi
	mov       esi,edx
	mov       ebx,eax
	mov       eax,offset _$J
	call      @__InitExceptBlock
	mov       edx,dword ptr fs:[4]
	dec       dword ptr [edx-4]
	test      ebx,ebx
	je        short @58
	mov       eax,dword ptr [ebx]
	test      eax,eax
	je        short @59
	mov       dword ptr [ebp-4],eax
	mov       edx,dword ptr [ebp-4]
	test      edx,edx
	je        short @60
	push      0
	mov       ecx,dword ptr [ebp-4]
	push      ecx
	call      @__GetPolymorphicDTC$qpvui
	add       esp,8
	mov       edx,dword ptr fs:[4]
	add       dword ptr [edx-4],eax
	mov       word ptr [ebp-20],20
	mov       edx,3
	mov       eax,dword ptr [ebp-4]
	mov       ecx,dword ptr [eax]
	call      dword ptr [ecx]
	mov       word ptr [ebp-20],8
@60:
@61:
@59:
	test      esi,1
	je        short @62
	push      ebx
	call      @$bdele$qpv
	pop       ecx
@62:
@58:
	mov       eax,dword ptr [ebp-36]
	mov       dword ptr fs:[0],eax
@63:
	pop       esi
	pop       ebx
	mov       esp,ebp
	pop       ebp
	ret 
@@CLIPBOARD@1$bdtr$qqrv	endp
@CLIPBOARD@1$bdtr$qqrv	ends
_TEXT	ends
_DATA	segment dword public use32 'DATA'
@clip_frame@5	segment virtual
	align	2
@@clip_frame@5	label	byte
	dd	@@$xt$10clip_frame
	dd	0
	dd	0
	dd	@clip_frame@1$bdtr$qqrv
	dd	@clip_frame@1paste$qqrv
@clip_frame@5	ends
_DATA	ends
_DATA	segment dword public use32 'DATA'
@clip_images@5	segment virtual
	align	2
@@clip_images@5	label	byte
	dd	@@$xt$11clip_images
	dd	0
	dd	0
	dd	@clip_images@1$bdtr$qqrv
	dd	@clip_images@1paste$qqrv
@clip_images@5	ends
_DATA	ends
_DATA	segment dword public use32 'DATA'
@clipdata@5	segment virtual
	align	2
@@clipdata@5	label	byte
	dd	@@$xt$8clipdata
	dd	0
	dd	0
	dd	@@clipdata@1$bdtr$qqrv
	dd	__pure_error_
@clipdata@5	ends
_DATA	ends
_TEXT	segment dword public use32 'CODE'
@$xt$11clip_images	segment virtual
@@$xt$11clip_images	label	byte
	dd	20
	dw	3
	dw	48
	dd	0
	dd	119
	dw	60
	dw	76
	dd	0
	dw	0
	dw	0
	dd	0
	dd	2
	dd	2
	dd	@clip_images@1$bdtr$qqrv
	dw	131
	dw	80
	db	99
	db	108
	db	105
	db	112
	db	95
	db	105
	db	109
	db	97
	db	103
	db	101
	db	115
	db	0
	dd	@@$xt$8clipdata
	dd	0
	dd	3
	dd	0
	dd	0
	dd	0
@$xt$11clip_images	ends
_TEXT	ends
_TEXT	segment dword public use32 'CODE'
@$xt$10clip_frame	segment virtual
@@$xt$10clip_frame	label	byte
	dd	20
	dw	3
	dw	48
	dd	0
	dd	119
	dw	60
	dw	76
	dd	0
	dw	0
	dw	0
	dd	0
	dd	2
	dd	2
	dd	@clip_frame@1$bdtr$qqrv
	dw	131
	dw	80
	db	99
	db	108
	db	105
	db	112
	db	95
	db	102
	db	114
	db	97
	db	109
	db	101
	db	0
	db	0
	dd	@@$xt$8clipdata
	dd	0
	dd	3
	dd	0
	dd	0
	dd	0
@$xt$10clip_frame	ends
_TEXT	ends
_TEXT	segment dword public use32 'CODE'
@$xt$8clipdata	segment virtual
@@$xt$8clipdata	label	byte
	dd	4
	dw	3
	dw	48
	dd	0
	dd	115
	dw	60
	dw	64
	dd	0
	dw	0
	dw	0
	dd	0
	dd	1
	dd	1
	dd	@@clipdata@1$bdtr$qqrv
	dw	131
	dw	68
	db	99
	db	108
	db	105
	db	112
	db	100
	db	97
	db	116
	db	97
	db	0
	db	0
	db	0
	db	0
	dd	0
	dd	0
	dd	0
@$xt$8clipdata	ends
_TEXT	ends
_TEXT	segment dword public use32 'CODE'
@clipdata@1$bdtr$qqrv	segment virtual
	align	2
@@clipdata@1$bdtr$qqrv	proc	near
?live16394@0:
@64:
	mov       ecx,dword ptr fs:[4]
	dec       dword ptr [ecx-4]
	test      eax,eax
	je        short @65
	test      dl,1
	mov       dword ptr [eax],offset @@clipdata@5+12
	je        short @66
	push      eax
	call      @$bdele$qpv
	pop       ecx
@66:
@65:
@67:
	ret 
@@clipdata@1$bdtr$qqrv	endp
@clipdata@1$bdtr$qqrv	ends
_TEXT	ends
_DATA	segment dword public use32 'DATA'
s@	label	byte
	;	s@+0:
	db	"%d images copied",0
	;	s@+17:
	db	"pasted %d image(s), %d bytes",0
	;	s@+46:
	db	"%d frames copied",0
	;	s@+63:
	db	"pasted %d frame(s), %d bytes",0
	align	4
_DATA	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
	extrn	@$bdele$qpv:near
	extrn	__pure_error_:near
	extrn	@__GetPolymorphicDTC$qpvui:near
	extrn	__Exception_list:dword
	extrn	___StackBase:dword
	extrn	@__InitExceptBlock:near
	extrn	_free:near
	extrn	_malloc:near
	extrn	_memcpy:near
	public	@clipdata@1$bctr$qqrv
	public	@clip_images@1$bctr$qqrp5framep9objectdef
	public	@clip_images@1$bdtr$qqrv
	public	@clip_images@1paste$qqrv
	public	@clip_frame@1$bctr$qqrp5frameip9objectdef
	public	@clip_frame@1$bdtr$qqrv
	public	@clip_frame@1paste$qqrv
	public	_clipboard
	extrn	@series@1insertbytes$qqrpvi:near
	extrn	@series@1deletebytes$qqrpvi:near
	extrn	@object@1resetfptr$qqrv:near
	extrn	_objspace:dword
	extrn	@msgbuffer@1printf$qipce:near
	extrn	_msg:byte
	?debug	D "MESSAGE.H" 8583 3511
	?debug	D "gui.h" 8604 39747
	?debug	D "OBJSPACE.H" 8605 6386
	?debug	D "\a32\cfunc.h" 8477 48348
	?debug	D "input.h" 8596 14389
	?debug	D "dlgpos.h" 8593 46869
	?debug	D "uutimer.h" 8587 42352
	?debug	D "gui.h" 8604 39747
	?debug	D "types.h" 8601 19027
	?debug	D "effect.h" 8604 9896
	?debug	D "types.h" 8601 19027
	?debug	D "objdef.h" 8604 33466
	?debug	D "OBJECT.H" 8604 34263
	?debug	D "CLIP.H" 8604 33328
	?debug	D "C:\BC5\INCLUDE\MEM.H" 8308 10240
	?debug	D "C:\BC5\INCLUDE\_null.h" 8308 10240
	?debug	D "C:\BC5\INCLUDE\_defs.h" 8308 10240
	?debug	D "C:\BC5\INCLUDE\STDLIB.H" 8308 10240
	?debug	D "clip.cpp" 8604 33824
	end
