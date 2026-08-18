 dc.l $00000000, $00000208, $00000200, $00000200, $00000200, $00000200
 dc.l $00000200, $00000200, $00000200, $00000200, $00000200, $00000200
 dc.l $00000200, $00000200, $00000200, $00000200, $00000200, $00000200
 dc.l $00000200, $00000200, $00000200, $00000200, $00000200, $00000200
 dc.l $00000200, $00000200, $00000200, $00000200, $00000200, $00000200
 dc.l $00002616, $00000200, $00000200, $00000200, $00000200, $00000200
 dc.l $00000200, $00000200, $00000200, $00000200, $00000200, $00000200
 dc.l $00000200, $00000200, $00000200, $00000200, $00000200, $00000200
 dc.l $00000200, $00000200, $00000200, $00000200, $00000200, $00000200
 dc.l $00000200, $00000200, $00000200, $00000200, $00000200, $00000200
 dc.l $00000200, $00000200, $00000200, $00000200
 dc.b 'SEGA MEGA DRIVE (C)SEGA 1990.SEP'
 dc.b '                                                '
 dc.b 'STRIDER HIRYU                                   '
 dc.b 'GM 00001112-00'
 dc.w $cd54
 dc.b 'J               '
 dc.l $00000000, $000fffff, $00ff0000, $00ffffff
 dc.b '  '
 dc.b $20,$20
 dc.l $20202020, $20202020
 dc.b '            '
 dc.b '                                        '
 dc.b 'UE              '
00000200                          MOVE.B    #$00,$00A1000D
00000208                          TST.L     $00A10008
0000020E                          BNE       $00000308(pc)
00000212                          TST.W     $00A1000C
00000218                          BNE       $0000020E
0000021A                          LEA       $000002A4(pc),A5
0000021E                          MOVEM.L   (A5)+,D5-D7/A0-A4
00000222                          MOVE.B    -$10FF(A1),D0
00000226                          AND.B     #$0F,D0
0000022A                          BEQ       $00000234
0000022C                          MOVE.L    #$436E4320,$2F00(A1)
00000234                          MOVE.W    (A4),D0
00000236                          MOVEQ     #$00,D0
00000238                          MOVE.L    D0,A6
0000023A                          MOVE      A6,USP
0000023C                          MOVEQ     #$17,D1
0000023E                          MOVE.B    (A5)+,D5
00000240                          MOVE.W    D5,(A4)
00000242                          ADD.W     D7,D5
00000244                          DBRA      D1,$0000023E
00000248                          MOVE.L    #$40000080,(A4)
0000024E                          MOVE.W    D0,(A3)
00000250                          MOVE.W    D7,(A1)
00000252                          MOVE.W    D7,(A2)
00000254                          BTST      D0,(A1)
00000256                          BNE       $00000254
00000258                          MOVEQ     #$27,D2
0000025A                          MOVE.B    (A5)+,(A0)+
0000025C                          DBRA      D2,$0000025A
00000260                          MOVE.W    D0,(A2)
00000262                          MOVE.W    D0,(A1)
00000264                          MOVE.W    D7,(A2)
00000266                          MOVE.L    D0,-(A6)
00000268                          DBRA      D6,$00000266
0000026C                          MOVE.L    #$81048F02,(A4)
00000272                          MOVE.L    #$C0000000,(A4)
00000278                          MOVEQ     #$1F,D3
0000027A                          MOVE.L    D0,(A3)
0000027C                          DBRA      D3,$0000027A
00000280                          MOVE.L    #$40000010,(A4)
00000286                          MOVEQ     #$13,D4
00000288                          MOVE.L    D0,(A3)
0000028A                          DBRA      D4,$00000288
0000028E                          MOVEQ     #$03,D5
00000290                          MOVE.B    (A5)+,$0011(A3)
00000294                          DBRA      D5,$00000290
00000298                          MOVE.W    D0,(A2)
0000029A                          MOVEM.L   (A6),D0-D7/A0-A6
0000029E                          MOVE      #$2700,SR
000002A2                          BRA       $00000308
000002A4                          OR.B      #$00,D0
000002A8                          OR.B      #$FF,D0
000002AC                          OR.B      #$00,D0
000002B0                          OR.L      #$000000A1,-(A0)
000002B6                          MOVE.B    D0,-(A0)
000002B8                          OR.L      #$120000C0,-(A1)
000002BE                          OR.B      #$C0,D0
000002C2                          OR.B      #$14,D4
000002C6                          MOVE.W    #$076C,D0
000002CA                          OR.B      #$00,D0
000002CE                          Invalid
000002D0                          SBCD      $00(A7,A0.W),D0
000002D4                          BTST      D0,D0
000002D6                          Invalid
000002D8                          Invalid
000002DA                          OR.L      #$AF01D71F,D0
000002E0                          MOVE.B    $0021(A1),-(A0)
000002E4                          MOVE.L    D0,D4
000002E6                          Invalid
000002E8                          ROXL.L    D6,D0
000002EA                          ADD.L     -(A1),A6
000002EC                          Invalid
000002EE                          ASL.W     #6,D7
000002F0                          LSL.W     #6,D7
000002F2                          BSET      #$C1,(A1)+
000002F6                          ADD.L     -(A1),A0
000002F8                          BSET      #$D1,(A1)+
000002FC                          ASL       $F3ED5636
00000302                          AND       -$6041(A1)
00000306                          ADD.L     ,A7
00000308                          MOVE.B    $00A10001,D0
0000030E                          MOVE.B    D0,D7
00000310                          AND.B     #$0F,D0
00000314                          BEQ       $00000320
00000316                          MOVE.L    #$436E4320,$00A14000
00000320                          BTST      #$06,$00A1000D
00000328                          BNE       $00000370
0000032A                          MOVE.L    #$00000200,A0
00000330                          MOVE.L    #$000001A4,A1
00000336                          MOVE.L    (A1),D0
00000338                          MOVEQ     #$00,D1
0000033A                          ADD.W     (A0)+,D1
0000033C                          CMP.L     A0,D0
0000033E                          BCC       $0000033A
00000340                          MOVE.L    #$0000018E,A1
00000346                          CMP.W     (A1),D1
00000348                          BNE       $000003C6(pc)
0000034C                          JSR       $000011AA
00000352                          MOVE.B    D7,$FD1B
00000356                          MOVE.W    #$0001,$FDE2
0000035C                          MOVE.B    #$03,$FD18
00000362                          MOVE.W    #$07D0,$FD0E
00000368                          MOVE.L    #$00FFA000,$FD10
00000370                          JSR       $000011BE
00000376                          JSR       $00020352
0000037C                          JSR       $00022EEC
00000382                          MOVE.W    $E100,D0
00000386                          ADD.W     D0,D0
00000388                          ADD.W     D0,D0
0000038A                          MOVE.L    +$00(D3.W),A0
0000038E                          JSR       (A0)
00000390                          BRA       $00000382
00000392                          OR.B      #$26,D0
00000396                          OR.B      #$9C,D0
0000039A                          OR.B      #$7E,D0
0000039E                          OR.B      #$2C,D0
000003A2                          OR.B      #$D6,D0
000003A6                          OR.B      #$F4,D0
000003AA                          OR.B      #$E0,D0
000003AE                          OR.B      #$58,D0
000003B2                          OR.B      #$D6,D0
000003B6                          OR.B      #$BC,D0
000003BA                          OR.B      #$90,D0
000003BE                          OR.B      #$C8,D0
000003C2                          OR.B      #$A2,D0
000003C6                          JSR       $00020352
000003CC                          MOVE.L    #$C0000000,$00C00004
000003D6                          MOVEQ     #$3F,D7
000003D8                          MOVE.W    #$000E,$00C00000
000003E0                          DBRA      D7,$000003D8
000003E4                          BRA       $000003E4
000003E6                          JSR       $0000DC16
000003EC                          BEQ       $000003F0
000003EE                          RTS
000003F0                          JSR       $000041F0
000003F6                          JSR       $00003886
000003FC                          JSR       $000097E2
00000402                          JSR       $00003C28
00000408                          JSR       $00009244
0000040E                          JSR       $000129D2
00000414                          JSR       $000126EA
0000041A                          JSR       $000127EA
00000420                          JSR       $0001710C
00000426                          JSR       $000264DC
0000042C                          TST.B     $E190
00000430                          BEQ       $0000043E
00000432                          MOVE.B    #$00,$E190
00000438                          JSR       $0000858C
0000043E                          TST.B     $E191
00000442                          BEQ       $00000450
00000444                          MOVE.B    #$00,$E191
0000044A                          JSR       $0000899E
00000450                          TST.B     $E197
00000454                          BEQ       $00000462
00000456                          MOVE.B    #$00,$E197
0000045C                          JSR       $000089DE
00000462                          JSR       $0000F254
00000468                          JSR       $0000F112
0000046E                          JSR       $0000EFFE
00000474                          JSR       $0000EDD0
0000047A                          JSR       $00008150
00000480                          JSR       $00020DDA
00000486                          JSR       $00013E80
0000048C                          JSR       $0000D94C
00000492                          BCLR      #$00,$E198
00000498                          BEQ       $000004A0
0000049A                          JSR       $00020198
000004A0                          MOVE.B    #$00,$E182
000004A6                          MOVE.L    #$00FFE280,$E000
000004AE                          JSR       $00000544(pc)
000004B2                          JSR       $00000B2A(pc)
000004B6                          JSR       $00000B0C(pc)
000004BA                          JSR       $0000086A(pc)
000004BE                          JSR       $00000978(pc)
000004C2                          JSR       $00000882(pc)
000004C6                          JSR       $0000089A(pc)
000004CA                          JSR       $00000746(pc)
000004CE                          JSR       $00000934(pc)
000004D2                          JSR       $00000AEC(pc)
000004D6                          JSR       $00000902(pc)
000004DA                          JSR       $0000092A(pc)
000004DE                          JSR       $0000093E(pc)
000004E2                          JSR       $0000088E(pc)
000004E6                          JSR       $000005BE(pc)
000004EA                          JSR       $00000752(pc)
000004EE                          JSR       $00000A88(pc)
000004F2                          JSR       $000008EE(pc)
000004F6                          JSR       $000008F8(pc)
000004FA                          JSR       $00000920(pc)
000004FE                          JSR       $00000948(pc)
00000502                          JSR       $00000ABA(pc)
00000506                          MOVE.L    $E000,A0
0000050A                          MOVE.L    #$00000000,(A0)
00000510                          RTS
00000512                          MOVE.L    $FD10,A0
00000516                          LEA       $0000053C(pc),A1
0000051A                          MOVE.W    $0008(A0),D4
0000051E                          MOVE.W    $000C(A0),D3
00000522                          SUB.W     $EA08,D4
00000526                          ADD.W     #$0080,D4
0000052A                          SUB.W     $EA0C,D3
0000052E                          ADD.W     #$0080,D3
00000532                          MOVE.W    #$8000,D5
00000536                          JMP       $00022F38
0000053C                          OR.B      #$00,D0
00000540                          ADD.L     #$FC000C78,D1
00000546                          OR.B      #$00,D4
0000054A                          BNE       $0000056A
0000054C                          BTST      #$05,$E11B
00000552                          BNE       $0000056A
00000554                          LEA       $0000056C(pc),A1
00000558                          MOVE.W    #$00F0,D4
0000055C                          MOVE.W    #$00F0,D3
00000560                          MOVE.W    #$8000,D5
00000564                          JMP       $00022F38
0000056A                          RTS
0000056C                          BTST      D7,D0
0000056E                          OR.B      #$90,D0
00000572                          ASR.B     #4,D0
00000574                          OR.B      #$F0,D6
00000578                          OR.B      #$8F,D0
0000057C                          Invalid
0000057E                          OR.B      #$00,D6
00000582                          OR.B      #$93,D0
00000586                          BTST      #$06,D0
0000058A                          (CCC     #$1,(A0)+
0000058C                          OR.B      #$8C,D0
00000590                          MOVE.L    D0,D0
00000592                          OR.B      #$28,D6
00000596                          OR.B      #$95,D0
0000059A                          MOVE.W    D0,D0
0000059C                          OR.B      #$38,D6
000005A0                          OR.B      #$96,D0
000005A4                          NBCD      D0
000005A6                          OR.B      #$50,D6
000005AA                          OR.B      #$8C,D0
000005AE                          ADDQ.B    #$4,D0
000005B0                          OR.B      #$60,D6
000005B4                          OR.B      #$80,D0
000005B8                          BVC       $000005C0(pc)
000005BC                          (CCC     #$3,$41(A0,D4.L)
000005C0                          Invalid
000005C2                          MOVE.B    $0001(A0),D0
000005C6                          BEQ       $000005E2
000005C8                          MOVE.B    $0000(A0),D0
000005CC                          BEQ       $000005E2
000005CE                          MOVE.W    $0012(A0),D0
000005D2                          MOVE.L    $001A(A0),A6
000005D6                          LEA       $00(A6,D0.W),A1
000005DA                          MOVE.L    A1,A3
000005DC                          BSR       $000005E4
000005DE                          BRA       $0000067A(pc)
000005E2                          RTS
000005E4                          MOVE.W    $0004(A3),D0
000005E8                          LEA       $00(A6,D0.W),A1
000005EC                          MOVE.W    $0008(A0),D4
000005F0                          MOVE.W    $000C(A0),D3
000005F4                          MOVE.B    $0008(A3),D1
000005F8                          EXT.W     D1
000005FA                          ADD.W     D1,D4
000005FC                          TST.B     $005B(A0)
00000600                          BNE       $0000060C
00000602                          MOVE.B    $0009(A3),D1
00000606                          EXT.W     D1
00000608                          ADD.W     D1,D3
0000060A                          BRA       $00000614
0000060C                          MOVE.B    $0009(A3),D1
00000610                          EXT.W     D1
00000612                          SUB.W     D1,D3
00000614                          MOVEQ     #$00,D6
00000616                          MOVE.B    $0001(A1),D6
0000061A                          CMP.B     #$78,D6
0000061E                          BCC       $0000065C
00000620                          CMP.B     $E1E2,D6
00000624                          BEQ       $0000065C
00000626                          MOVE.B    D6,$E1E2
0000062A                          CMP.B     #$FF,D6
0000062E                          BEQ       $0000066E
00000630                          MOVE.B    #$FE,$E1EA
00000636                          LEA       $000F0000,A2
0000063C                          LSL.W     #2,D6
0000063E                          MOVE.L    $60(A2,D0.W),D7
00000642                          MOVE.L    D7,$E1E4
00000646                          MOVE.L    $60(A2,D2.W),D6
0000064A                          SUB.L     D7,D6
0000064C                          LSR.W     #1,D6
0000064E                          MOVE.W    D6,$E1E0
00000652                          LSR.W     #4,D6
00000654                          ADD.W     $E122,D6
00000658                          MOVE.W    D6,$005E(A0)
0000065C                          MOVE.W    $E122,D5
00000660                          MOVE.W    D5,$005C(A0)
00000664                          MOVE.B    $0007(A3),D7
00000668                          LSR.B     #2,D7
0000066A                          BRA       $000006F4(pc)
0000066E                          MOVE.W    #$0000,$E1E0
00000674                          MOVE.W    $E122,$005E(A0)
0000067A                          MOVE.W    $000A(A3),D0
0000067E                          LEA       $00(A6,D0.W),A1
00000682                          MOVE.W    $0008(A0),D4
00000686                          MOVE.W    $000C(A0),D3
0000068A                          MOVE.B    $000C(A3),D1
0000068E                          EXT.W     D1
00000690                          ADD.W     D1,D4
00000692                          TST.B     $005B(A0)
00000696                          BNE       $000006A2
00000698                          MOVE.B    $000D(A3),D1
0000069C                          EXT.W     D1
0000069E                          ADD.W     D1,D3
000006A0                          BRA       $000006AA
000006A2                          MOVE.B    $000D(A3),D1
000006A6                          EXT.W     D1
000006A8                          SUB.W     D1,D3
000006AA                          MOVE.W    $005E(A0),D5
000006AE                          MOVEQ     #$00,D6
000006B0                          MOVE.B    $0001(A1),D6
000006B4                          CMP.B     #$78,D6
000006B8                          BCC       $000006E6
000006BA                          CMP.B     $E1EA,D6
000006BE                          BEQ       $000006E6
000006C0                          MOVE.B    D6,$E1EA
000006C4                          CMP.B     #$FF,D6
000006C8                          BEQ       $000006E6
000006CA                          LEA       $000F0000,A2
000006D0                          LSL.W     #2,D6
000006D2                          MOVE.L    $60(A2,D0.W),D7
000006D6                          MOVE.L    D7,$E1EC
000006DA                          MOVE.L    $60(A2,D2.W),D6
000006DE                          SUB.L     D7,D6
000006E0                          LSR.W     #1,D6
000006E2                          MOVE.W    D6,$E1E8
000006E6                          MOVE.B    $0007(A3),D7
000006EA                          BRA       $000006F4
000006EC                          MOVE.W    #$0000,$E1E8
000006F2                          RTS
000006F4                          SUB.W     $EA08,D4
000006F8                          ADD.W     #$0080,D4
000006FC                          SUB.W     $EA0C,D3
00000700                          ADD.W     #$00A0,D3
00000704                          TST.B     $005B(A0)
00000708                          BEQ       $0000070E
0000070A                          SUB.W     #$0040,D3
0000070E                          AND.W     #$0003,D7
00000712                          TST.B     $A05B
00000716                          BEQ       $0000071C
00000718                          EOR.W     #$0002,D7
0000071C                          CMP.W     #$0001,D7
00000720                          BEQ       $00000734
00000722                          CMP.W     #$0002,D7
00000726                          BEQ       $0000073A
00000728                          CMP.W     #$0003,D7
0000072C                          BEQ       $00000740
0000072E                          JMP       $00022F38
00000734                          JMP       $00022F82
0000073A                          JMP       $00022FDE
00000740                          JMP       $0002303A
00000746                          LEA       $E974,A0
0000074A                          LEA       $E900,A1
0000074E                          BRA       $0000075A(pc)
00000752                          LEA       $E92C,A0
00000756                          LEA       $E8C8,A1
0000075A                          MOVE.W    (A0),D7
0000075C                          BEQ       $00000868(pc)
00000760                          MOVE.L    (A1),A3
00000762                          MOVEM.L   A0-A1,-(A7)
00000766                          SUBQ.W    #$1,D7
00000768                          MOVE.L    (A3)+,A0
0000076A                          MOVE.L    D7,-(A7)
0000076C                          CMP.B     #$02,$0017(A0)
00000772                          BNE       $0000077A
00000774                          JSR       $000009C2(pc)
00000778                          BRA       $000007F4
0000077A                          MOVE.W    $0012(A0),D0
0000077E                          MOVE.L    $001A(A0),A6
00000782                          LEA       $00(A6,D0.W),A1
00000786                          MOVE.W    $0008(A0),D4
0000078A                          SUB.W     $EA08,D4
0000078E                          ADD.W     #$0080,D4
00000792                          MOVE.W    $000C(A0),D3
00000796                          SUB.W     $EA0C,D3
0000079A                          ADD.W     #$0080,D3
0000079E                          MOVE.W    $0008(A1),D0
000007A2                          MOVE.L    A1,A4
000007A4                          LEA       $00(A6,D0.W),A1
000007A8                          MOVE.L    $FC44,D6
000007AC                          CMP.L     A0,D6
000007AE                          BEQ       $0000080A
000007B0                          MOVE.L    $FC48,D6
000007B4                          CMP.L     A0,D6
000007B6                          BNE       $000007F4
000007B8                          LEA       $000487DE,A2
000007BE                          MOVEQ     #$00,D6
000007C0                          MOVE.B    $0001(A1),D6
000007C4                          CMP.B     $E1FA,D6
000007C8                          BEQ       $000007E4
000007CA                          MOVE.B    D6,$E1FA
000007CE                          LSL.W     #2,D6
000007D0                          MOVE.L    $60(A2,D0.W),D7
000007D4                          MOVE.L    D7,$E1FC
000007D8                          MOVE.L    $60(A2,D2.W),D6
000007DC                          SUB.L     D7,D6
000007DE                          LSR.W     #1,D6
000007E0                          MOVE.W    D6,$E1F8
000007E4                          MOVE.W    $E122,D5
000007E8                          SUB.W     #$0010,D5
000007EC                          MOVE.W    D5,$005C(A0)
000007F0                          JSR       $000009F4(pc)
000007F4                          MOVE.L    (A7)+,D7
000007F6                          DBRA      D7,$00000768
000007FA                          MOVEM.L   (A7)+,A0-A1
000007FE                          TST.B     $F0B8
00000802                          BNE       $00000868
00000804                          MOVE.L    A3,(A1)
00000806                          CLR.W     (A0)
00000808                          RTS
0000080A                          LEA       $000487DE,A2
00000810                          CMP.B     #$01,$0017(A0)
00000816                          BNE       $0000081E
00000818                          LEA       $0004958A,A2
0000081E                          MOVEQ     #$00,D6
00000820                          MOVE.B    $0001(A1),D6
00000824                          CMP.B     $E1F2,D6
00000828                          BEQ       $00000844
0000082A                          MOVE.B    D6,$E1F2
0000082E                          LSL.W     #2,D6
00000830                          MOVE.L    $60(A2,D0.W),D7
00000834                          MOVE.L    D7,$E1F4
00000838                          MOVE.L    $60(A2,D2.W),D6
0000083C                          SUB.L     D7,D6
0000083E                          LSR.W     #1,D6
00000840                          MOVE.W    D6,$E1F0
00000844                          MOVE.W    $E122,D5
00000848                          SUB.W     #$0030,D5
0000084C                          MOVE.W    D5,$005C(A0)
00000850                          JSR       $000009F4(pc)
00000854                          MOVE.L    (A7)+,D7
00000856                          DBRA      D7,$00000768
0000085A                          MOVEM.L   (A7)+,A0-A1
0000085E                          TST.B     $F0B8
00000862                          BNE       $00000868
00000864                          MOVE.L    A3,(A1)
00000866                          CLR.W     (A0)
00000868                          RTS
0000086A                          MOVE.W    $E946,D7
0000086E                          BEQ       $00000880
00000870                          MOVE.L    $E8B8,A3
00000874                          JSR       $00000B64(pc)
00000878                          MOVE.L    A3,$E8B8
0000087C                          CLR.W     $E946
00000880                          RTS
00000882                          LEA       $E96E,A0
00000886                          LEA       $E8F8,A1
0000088A                          BRA       $000008A2(pc)
0000088E                          LEA       $E970,A0
00000892                          LEA       $E8FC,A1
00000896                          BRA       $000008A2(pc)
0000089A                          LEA       $E976,A0
0000089E                          LEA       $E904,A1
000008A2                          MOVE.W    (A0),D7
000008A4                          BEQ       $000008EC
000008A6                          MOVE.L    (A1),A3
000008A8                          MOVEM.L   A0-A1,-(A7)
000008AC                          SUBQ.W    #$1,D7
000008AE                          MOVE.L    (A3)+,A0
000008B0                          MOVE.L    D7,-(A7)
000008B2                          MOVE.W    $0012(A0),D0
000008B6                          MOVE.L    $001A(A0),A6
000008BA                          MOVEQ     #$00,D7
000008BC                          MOVE.B    $FC0A,D7
000008C0                          ADD.W     D7,D7
000008C2                          ADD.W     D7,D7
000008C4                          MOVE.W    $70(A6,D1.W),D5
000008C8                          BTST      #$07,$E122
000008CE                          BEQ       $000008D4
000008D0                          OR.W      #$8000,D5
000008D4                          JSR       $000009CE(pc)
000008D8                          MOVE.L    (A7)+,D7
000008DA                          DBRA      D7,$000008AE
000008DE                          MOVEM.L   (A7)+,A0-A1
000008E2                          TST.B     $F0B8
000008E6                          BNE       $000008EC
000008E8                          MOVE.L    A3,(A1)
000008EA                          CLR.W     (A0)
000008EC                          RTS
000008EE                          LEA       $E926,A0
000008F2                          LEA       $E89C,A1
000008F6                          BRA       $00000950
000008F8                          LEA       $E978,A0
000008FC                          LEA       $E908,A1
00000900                          BRA       $00000950
00000902                          LEA       $E97C,A0
00000906                          LEA       $E910,A1
0000090A                          BRA       $00000950
0000090C                          LEA       $E94E,A0
00000910                          LEA       $E8C0,A1
00000914                          BRA       $00000950
00000916                          LEA       $E952,A0
0000091A                          LEA       $E8C4,A1
0000091E                          BRA       $00000950
00000920                          LEA       $E922,A0
00000924                          LEA       $E894,A1
00000928                          BRA       $00000950
0000092A                          LEA       $E956,A0
0000092E                          LEA       $E8B0,A1
00000932                          BRA       $00000950
00000934                          LEA       $E94A,A0
00000938                          LEA       $E8BC,A1
0000093C                          BRA       $00000950
0000093E                          LEA       $E958,A0
00000942                          LEA       $E8B4,A1
00000946                          BRA       $00000950
00000948                          LEA       $E942,A0
0000094C                          LEA       $E8A8,A1
00000950                          MOVE.W    (A0),D7
00000952                          BEQ       $00000976
00000954                          MOVE.L    (A1),A3
00000956                          MOVEM.L   A0-A1,-(A7)
0000095A                          SUBQ.W    #$1,D7
0000095C                          MOVE.L    (A3)+,A0
0000095E                          MOVE.L    D7,-(A7)
00000960                          BSR       $000009C2
00000962                          MOVE.L    (A7)+,D7
00000964                          DBRA      D7,$0000095C
00000968                          MOVEM.L   (A7)+,A0-A1
0000096C                          TST.B     $F0B8
00000970                          BNE       $00000976
00000972                          MOVE.L    A3,(A1)
00000974                          CLR.W     (A0)
00000976                          RTS
00000978                          LEA       $E920,A0
0000097C                          LEA       $E890,A1
00000980                          MOVE.W    (A0),D7
00000982                          BEQ       $000009A6
00000984                          MOVE.L    (A1),A3
00000986                          MOVEM.L   A0-A1,-(A7)
0000098A                          SUBQ.W    #$1,D7
0000098C                          MOVE.L    (A3)+,A0
0000098E                          MOVE.L    D7,-(A7)
00000990                          BSR       $000009A8
00000992                          MOVE.L    (A7)+,D7
00000994                          DBRA      D7,$0000098C
00000998                          MOVEM.L   (A7)+,A0-A1
0000099C                          TST.B     $F0B8
000009A0                          BNE       $000009A6
000009A2                          MOVE.L    A3,(A1)
000009A4                          CLR.W     (A0)
000009A6                          RTS
000009A8                          MOVE.W    $0012(A0),D0
000009AC                          MOVE.L    $001A(A0),A6
000009B0                          JSR       $00000A22(pc)
000009B4                          BTST      #$07,$E122
000009BA                          BEQ       $000009CE
000009BC                          OR.W      #$8000,D5
000009C0                          BRA       $000009CE
000009C2                          MOVE.W    $0012(A0),D0
000009C6                          MOVE.L    $001A(A0),A6
000009CA                          JSR       $00000A22(pc)
000009CE                          LEA       $00(A6,D0.W),A1
000009D2                          MOVE.W    $0008(A0),D4
000009D6                          SUB.W     $EA08,D4
000009DA                          ADD.W     #$0080,D4
000009DE                          MOVE.W    $000C(A0),D3
000009E2                          SUB.W     $EA0C,D3
000009E6                          ADD.W     #$0080,D3
000009EA                          MOVE.W    $0008(A1),D0
000009EE                          MOVE.L    A1,A4
000009F0                          LEA       $00(A6,D0.W),A1
000009F4                          MOVE.B    $0007(A4),D7
000009F8                          AND.W     #$0060,D7
000009FC                          BEQ       $00000A10
000009FE                          CMP.W     #$0020,D7
00000A02                          BEQ       $00000A16
00000A04                          CMP.W     #$0040,D7
00000A08                          BEQ       $00000A1C
00000A0A                          JMP       $00023212
00000A10                          JMP       $000230A4
00000A16                          JMP       $00023112
00000A1C                          JMP       $00023192
00000A22                          MOVEQ     #$00,D7
00000A24                          MOVE.B    $FC0A,D7
00000A28                          ADD.W     D7,D7
00000A2A                          ADD.W     D7,D7
00000A2C                          MOVE.W    $70(A6,D1.W),D5
00000A30                          MOVE.W    $70(A6,D0.W),D7
00000A34                          BEQ       $00000A66
00000A36                          BPL       $00000A4A
00000A38                          AND.W     #$9FFF,D5
00000A3C                          MOVEQ     #$00,D7
00000A3E                          MOVE.B    $0027(A0),D7
00000A42                          ADD.W     D7,D7
00000A44                          ADD.W     +$70(D1.W),D5
00000A48                          RTS
00000A4A                          BCLR      #$04,D7
00000A4E                          BNE       $00000A70
00000A50                          SUBQ.W    #$1,D7
00000A52                          MULU      #$0014,D7
00000A56                          MOVEQ     #$00,D5
00000A58                          MOVE.B    $FC26,D5
00000A5C                          SUBQ.W    #$1,D5
00000A5E                          ADD.W     D5,D5
00000A60                          ADD.W     D5,D7
00000A62                          MOVE.W    $70(A6,D2.L),D5
00000A66                          RTS
00000A68                          OR.B      #$00,D0
00000A6C                          NEGX.B    D0
00000A6E                          BRA       $00005498(pc)
00000A72                          OR.B      #$DA,D6
00000A76                          MOVE.W    #$06A0,D5
00000A7A                          BTST      #$07,$E122
00000A80                          BEQ       $00000A86
00000A82                          MOVE.W    #$86A0,D5
00000A86                          RTS
00000A88                          MOVE.W    $E944,D7
00000A8C                          BEQ       $00000AB8
00000A8E                          MOVE.L    $E8AC,A3
00000A92                          SUBQ.W    #$1,D7
00000A94                          MOVE.L    (A3)+,A0
00000A96                          MOVEM.L   D7/A3,-(A7)
00000A9A                          JSR       $000198C0
00000AA0                          MOVEM.L   (A7)+,D7/A3
00000AA4                          DBRA      D7,$00000A94
00000AA8                          TST.B     $F0B8
00000AAC                          BNE       $00000AB8(pc)
00000AB0                          MOVE.L    A3,$E8AC
00000AB4                          CLR.W     $E944
00000AB8                          RTS
00000ABA                          MOVE.W    $E97E,D7
00000ABE                          BEQ       $00000AEA
00000AC0                          MOVE.L    $E914,A3
00000AC4                          SUBQ.W    #$1,D7
00000AC6                          MOVE.L    (A3)+,A0
00000AC8                          MOVEM.L   D7/A3,-(A7)
00000ACC                          JSR       $0001CC62
00000AD2                          MOVEM.L   (A7)+,D7/A3
00000AD6                          DBRA      D7,$00000AC6
00000ADA                          TST.B     $F0B8
00000ADE                          BNE       $00000AEA(pc)
00000AE2                          MOVE.L    A3,$E914
00000AE6                          CLR.W     $E97E
00000AEA                          RTS
00000AEC                          MOVE.W    $E97A,D7
00000AF0                          BEQ       $00000B0A
00000AF2                          MOVE.L    $E90C,A3
00000AF6                          BSR       $00000B64(pc)
00000AFA                          TST.B     $F0B8
00000AFE                          BNE       $00000B0A(pc)
00000B02                          MOVE.L    A3,$E90C
00000B06                          CLR.W     $E97A
00000B0A                          RTS
00000B0C                          MOVE.W    $E928,D7
00000B10                          BEQ       $00000B28
00000B12                          MOVE.L    $E8A0,A3
00000B16                          SUBQ.W    #$1,D7
00000B18                          MOVE.L    (A3)+,A0
00000B1A                          BSR       $00000B48
00000B1C                          DBRA      D7,$00000B18
00000B20                          MOVE.L    A3,$E8A0
00000B24                          CLR.W     $E928
00000B28                          RTS
00000B2A                          MOVE.W    $E92A,D7
00000B2E                          BEQ       $00000B46
00000B30                          MOVE.L    $E8A4,A3
00000B34                          SUBQ.W    #$1,D7
00000B36                          MOVE.L    (A3)+,A0
00000B38                          BSR       $00000B48
00000B3A                          DBRA      D7,$00000B36
00000B3E                          MOVE.L    A3,$E8A4
00000B42                          CLR.W     $E92A
00000B46                          RTS
00000B48                          MOVE.L    D7,-(A7)
00000B4A                          MOVE.L    $001A(A0),A1
00000B4E                          MOVE.W    $0008(A0),D4
00000B52                          MOVE.W    $000C(A0),D3
00000B56                          MOVE.W    #$2500,D5
00000B5A                          JSR       $00022F38
00000B60                          MOVE.L    (A7)+,D7
00000B62                          RTS
00000B64                          SUBQ.W    #$1,D7
00000B66                          MOVE.L    (A3)+,A0
00000B68                          MOVE.L    A3,-(A7)
00000B6A                          MOVE.L    D7,D6
00000B6C                          BSR       $00000B7A(pc)
00000B70                          MOVE.L    (A7)+,A3
00000B72                          MOVE.L    D6,D7
00000B74                          DBRA      D7,$00000B66
00000B78                          RTS
00000B7A                          MOVE.W    $0012(A0),D0
00000B7E                          MOVE.L    $001A(A0),A6
00000B82                          JSR       $00000A22(pc)
00000B86                          LEA       $00(A6,D0.W),A1
00000B8A                          LEA       (A1),A3
00000B8C                          MOVE.W    $0004(A3),D0
00000B90                          LEA       $00(A6,D0.W),A1
00000B94                          MOVE.W    $0006(A3),D2
00000B98                          MOVE.W    $0008(A0),D4
00000B9C                          MOVE.W    $000C(A0),D3
00000BA0                          ADD.W     $0008(A3),D4
00000BA4                          ADD.W     $000A(A3),D3
00000BA8                          BSR       $00000BC0(pc)
00000BAC                          MOVE.W    $000C(A3),D0
00000BB0                          LEA       $00(A6,D0.W),A1
00000BB4                          MOVE.W    $000E(A3),D2
00000BB8                          MOVE.W    $0008(A0),D4
00000BBC                          MOVE.W    $000C(A0),D3
00000BC0                          SUB.W     $EA08,D4
00000BC4                          ADD.W     #$0080,D4
00000BC8                          SUB.W     $EA0C,D3
00000BCC                          ADD.W     #$0080,D3
00000BD0                          BTST      #$05,D2
00000BD4                          BNE       $00000BDC
00000BD6                          JMP       $000230A4
00000BDC                          JMP       $00023112
00000BE2                          TST.B     $E18E
00000BE6                          BNE       $00000C30
00000BE8                          LEA       $E080,A1
00000BEC                          BSR       $00000BFE
00000BEE                          LEA       $E084,A1
00000BF2                          BSR       $00000BFE
00000BF4                          LEA       $E088,A1
00000BF8                          BSR       $00000BFE
00000BFA                          LEA       $E08C,A1
00000BFE                          MOVEQ     #$00,D0
00000C00                          MOVE.B    (A1),D0
00000C02                          ADD.W     D0,D0
00000C04                          MOVE.W    +$00(D3.W),D0
00000C08                          JMP       +$00(D1.W)
00000C0C                          OR.B      #$26,-(A4)
00000C10                          OR.W      #$00A2,$00C8(A4)
00000C16                          BTST      D0,
00000C18                          BCLR      D0,(A4)+
00000C1A                          MOVEP.L   D0,$0204(A6)
00000C1E                          AND.B     #$8A,$02(A2,D0.W)
00000C24                          AND.L     #$028A028A,A2
00000C2A                          MOVE.L    #$00000000,(A1)
00000C30                          RTS
00000C32                          MOVE.W    $E106,D0
00000C36                          AND.W     #$0007,D0
00000C3A                          BNE       $00000C56
00000C3C                          MOVE.W    $0002(A1),D0
00000C40                          ADDQ.W    #$4,D0
00000C42                          AND.W     #$001F,D0
00000C46                          MOVE.W    D0,$0002(A1)
00000C4A                          MOVE.L    +$00(D6.W),$E788
00000C50                          MOVE.B    #$01,$E183
00000C56                          RTS
00000C58                          OR.B      #$06,$0028(A4)
00000C5E                          OR.B      #$04,D4
00000C62                          OR.B      #$00,D2
00000C66                          OR.B      #$00,D0
00000C6A                          OR.B      #$04,D0
00000C6E                          OR.B      #$28,D2
00000C72                          OR.B      #$2C,D4
00000C76                          OR.B      #$38,D6
00000C7A                          ASL.B     #8,D6
00000C7C                          AND.W     #$000F,D0
00000C80                          BNE       $00000C9C
00000C82                          MOVE.W    $0002(A1),D0
00000C86                          ADDQ.W    #$2,D0
00000C88                          AND.W     #$000F,D0
00000C8C                          MOVE.W    D0,$0002(A1)
00000C90                          MOVE.W    +$00(D6.W),$E78C
00000C96                          MOVE.B    #$01,$E183
00000C9C                          RTS
00000C9E                          AND.B     #$44,-(A2)
00000CA2                          ADD.W     #$0888,-(A6)
00000CA6                          EOR.L     #$00000000,$0000(A2)
00000CAE                          MOVE.W    $E106,D0
00000CB2                          AND.W     #$0007,D0
00000CB6                          BNE       $00000CD2
00000CB8                          MOVE.W    $0002(A1),D0
00000CBC                          ADDQ.W    #$2,D0
00000CBE                          AND.W     #$000F,D0
00000CC2                          MOVE.W    D0,$0002(A1)
00000CC6                          MOVE.W    +$00(D3.L),$E732
00000CCC                          MOVE.B    #$01,$E183
00000CD2                          RTS
00000CD4                          MOVE.W    $E106,D0
00000CD8                          AND.W     #$0003,D0
00000CDC                          BNE       $00000D12
00000CDE                          LEA       $0002(A1),A1
00000CE2                          MOVEQ     #$00,D6
00000CE4                          MOVEQ     #$02,D7
00000CE6                          JSR       $000012EA(pc)
00000CEA                          MULU      #$0012,D0
00000CEE                          MOVE.L    +$00(D2.W),$E772
00000CF4                          MOVE.L    +$00(D1.W),$E776
00000CFA                          MOVE.L    +$00(D0.W),$E77A
00000D00                          MOVE.L    +$00(D7.L),$E77E
00000D06                          MOVE.W    +$00(D6.L),$E782
00000D0C                          MOVE.B    #$01,$E183
00000D12                          RTS
00000D14                          EOR.W     #$0646,-(A6)
00000D18                          SUB.W     #$0664,D4
00000D1C                          ADD.W     #$0222,-(A4)
00000D20                          SUB.W     #$0444,D4
00000D24                          ADD.W     #$0866,D4
00000D28                          EOR.W     #$0644,$0646(A0)
00000D2E                          SUB.W     #$0664,D2
00000D32                          SUB.W     #$0222,D4
00000D36                          SUB.W     #$0A66,D4
00000D3A                          BCHG      #$44,-(A6)
00000D3E                          ADD.W     #$0664,D4
00000D42                          SUB.W     #$0444,D2
00000D46                          SUB.W     #$0222,D4
00000D4A                          MOVE.W    $E106,D0
00000D4E                          AND.W     #$0003,D0
00000D52                          BNE       $00000D7E
00000D54                          MOVE.W    $0002(A1),D0
00000D58                          ADDQ.W    #$1,D0
00000D5A                          AND.W     #$0003,D0
00000D5E                          MOVE.W    D0,$0002(A1)
00000D62                          MULU      #$000A,D0
00000D66                          MOVE.L    +$00(D4.L),$E764
00000D6C                          MOVE.L    +$00(D3.L),$E768
00000D72                          MOVE.W    +$00(D2.L),$E76C
00000D78                          MOVE.B    #$01,$E183
00000D7E                          RTS
00000D80                          CMP.L     #$08660644,$0866(A2)
00000D88                          CMP.L     #$08660CAA,$0866(A2)
00000D90                          ADD.W     #$0EEE,D4
00000D94                          ADD.W     #$0866,D4
00000D98                          CMP.L     #$08660CAA,$0866(A2)
00000DA0                          ADD.W     #$0866,D4
00000DA4                          CMP.L     #$0EEE3038,-$1EFA(A2)
00000DAC                          AND.W     #$0007,D0
00000DB0                          BNE       $00000DD0
00000DB2                          MOVE.W    $0002(A1),D0
00000DB6                          ADDQ.W    #$2,D0
00000DB8                          CMP.W     #$0008,D0
00000DBC                          BCC       $00000C2A(pc)
00000DC0                          MOVE.W    D0,$0002(A1)
00000DC4                          MOVE.L    +$00(D6.W),$E770
00000DCA                          MOVE.B    #$01,$E183
00000DD0                          RTS
00000DD2                          OR.B      #$00,D0
00000DD6                          SUB.B     #$22,D0
00000DDA                          MOVE.W    $E106,D0
00000DDE                          AND.W     #$0007,D0
00000DE2                          BNE       $00000DFE
00000DE4                          MOVE.W    $0002(A1),D0
00000DE8                          ADDQ.W    #$4,D0
00000DEA                          AND.W     #$000F,D0
00000DEE                          MOVE.W    D0,$0002(A1)
00000DF2                          MOVE.L    +$00(D6.W),$E78A
00000DF8                          MOVE.B    #$01,$E183
00000DFE                          RTS
00000E00                          ADD.L     #$00660462,D4
00000E06                          OR.W      #$0240,-(A6)
00000E0A                          SUB.L     #$02400288,$3038(A2)
00000E12                          ASL.B     #8,D6
00000E14                          AND.W     #$000F,D0
00000E18                          BNE       $00000E34
00000E1A                          MOVE.W    $0002(A1),D0
00000E1E                          ADDQ.W    #$2,D0
00000E20                          AND.W     #$0007,D0
00000E24                          MOVE.W    D0,$0002(A1)
00000E28                          MOVE.W    +$00(D6.W),$E76C
00000E2E                          MOVE.B    #$01,$E183
00000E34                          RTS
00000E36                          BSET      #$88,$0000(A6)
00000E3C                          OR.B      #$38,D0
00000E40                          ASL.B     #8,D6
00000E42                          AND.W     #$000F,D0
00000E46                          BNE       $00000E74
00000E48                          MOVE.W    $0002(A1),D0
00000E4C                          ADDQ.W    #$0,D0
00000E4E                          AND.W     #$001F,D0
00000E52                          MOVE.W    D0,$0002(A1)
00000E56                          MOVE.W    +$00(D7.L),$E762
00000E5C                          MOVE.W    +$00(D5.L),$E764
00000E62                          MOVE.W    +$00(D3.L),$E76C
00000E68                          MOVE.W    +$00(D1.L),$E76A
00000E6E                          MOVE.B    #$01,$E183
00000E74                          RTS
00000E76                          AND.B     #$4C,$088E(A0)
00000E7C                          .L        #$066C088E,-(A6)
00000E82                          Invalid
00000E84                          EOR.W     #$0AAE,-(A2)
00000E88                          Invalid
00000E8A                          Invalid
00000E8C                          ADD.B     #$28,-(A0)
00000E90                          SUB.W     #$088E,A4
00000E94                          .L        #$3038E106,-(A6)
00000E9A                          AND.W     #$0007,D0
00000E9E                          BNE       $00000EBA
00000EA0                          MOVE.W    $0002(A1),D0
00000EA4                          ADDQ.W    #$4,D0
00000EA6                          AND.W     #$000F,D0
00000EAA                          MOVE.W    D0,$0002(A1)
00000EAE                          MOVE.L    +$00(D6.W),$E78A
00000EB4                          MOVE.B    #$01,$E183
00000EBA                          RTS
00000EBC                          BCHG      #$00,A6
00000EC0                          SUB.B     #$84,A2
00000EC4                          OR.B      #$40,D6
00000EC8                          OR.B      #$20,D2
00000ECC                          LEA       $00000F06(pc),A2
00000ED0                          TST.B     $A05B
00000ED4                          BNE       $00000EDA
00000ED6                          LEA       $00000F16(pc),A2
00000EDA                          MOVE.W    $E106,D0
00000EDE                          AND.W     #$000F,D0
00000EE2                          BNE       $00000F04
00000EE4                          MOVE.W    $0002(A1),D0
00000EE8                          ADDQ.W    #$0,D0
00000EEA                          AND.W     #$000F,D0
00000EEE                          MOVE.W    D0,$0002(A1)
00000EF2                          MOVE.L    $00(A2,D0.W),$E784
00000EF8                          MOVE.W    $00(A2,D2.W),$E78A
00000EFE                          MOVE.B    #$01,$E183
00000F04                          RTS
00000F06                          OR.W      #$0004,$0484(A4)
00000F0C                          OR.B      #$28,D0
00000F10                          OR.B      #$40,D4
00000F14                          OR.B      #$04,D0
00000F18                          OR.W      #$0484,$0000(A4)
00000F1E                          OR.B      #$28,D4
00000F22                          OR.W      #$0000,D0
00000F26                          MOVEM.L   D0-D7/A0-A6,-(A7)
00000F2A                          LEA       $E0D0,A0
00000F2E                          LEA       $E710,A1
00000F32                          LEA       $E790,A2
00000F36                          BSR       $00000F86
00000F38                          MOVEM.L   (A7)+,D0-D7/A0-A6
00000F3C                          RTS
00000F3E                          MOVEM.L   D0-D7/A0-A6,-(A7)
00000F42                          LEA       $E0D6,A0
00000F46                          LEA       $E730,A1
00000F4A                          LEA       $E7B0,A2
00000F4E                          BSR       $00000F86
00000F50                          MOVEM.L   (A7)+,D0-D7/A0-A6
00000F54                          RTS
00000F56                          MOVEM.L   D0-D7/A0-A6,-(A7)
00000F5A                          LEA       $E0DC,A0
00000F5E                          LEA       $E750,A1
00000F62                          LEA       $E7D0,A2
00000F66                          BSR       $00000F86
00000F68                          MOVEM.L   (A7)+,D0-D7/A0-A6
00000F6C                          RTS
00000F6E                          MOVEM.L   D0-D7/A0-A6,-(A7)
00000F72                          LEA       $E0E2,A0
00000F76                          LEA       $E770,A1
00000F7A                          LEA       $E7F0,A2
00000F7E                          BSR       $00000F86
00000F80                          MOVEM.L   (A7)+,D0-D7/A0-A6
00000F84                          RTS
00000F86                          MOVE.L    A1,-(A7)
00000F88                          JSR       $00001428(pc)
00000F8C                          MOVE.L    (A7)+,A1
00000F8E                          MOVEQ     #$00,D1
00000F90                          JSR       $00001364(pc)
00000F94                          MOVE.W    #$0010,$0000(A0)
00000F9A                          MOVE.W    #$0108,$0002(A0)
00000FA0                          MOVE.W    #$FFFF,$0004(A0)
00000FA6                          RTS
00000FA8                          MOVEM.L   D0-D7/A0-A6,-(A7)
00000FAC                          LEA       $E0E8,A0
00000FB0                          LEA       $E710,A1
00000FB4                          LEA       $E790,A2
00000FB8                          BSR       $00001008
00000FBA                          MOVEM.L   (A7)+,D0-D7/A0-A6
00000FBE                          RTS
00000FC0                          MOVEM.L   D0-D7/A0-A6,-(A7)
00000FC4                          LEA       $E0EE,A0
00000FC8                          LEA       $E730,A1
00000FCC                          LEA       $E7B0,A2
00000FD0                          BSR       $00001008
00000FD2                          MOVEM.L   (A7)+,D0-D7/A0-A6
00000FD6                          RTS
00000FD8                          MOVEM.L   D0-D7/A0-A6,-(A7)
00000FDC                          LEA       $E0F4,A0
00000FE0                          LEA       $E750,A1
00000FE4                          LEA       $E7D0,A2
00000FE8                          BSR       $00001008
00000FEA                          MOVEM.L   (A7)+,D0-D7/A0-A6
00000FEE                          RTS
00000FF0                          MOVEM.L   D0-D7/A0-A6,-(A7)
00000FF4                          LEA       $E0FA,A0
00000FF8                          LEA       $E770,A1
00000FFC                          LEA       $E7F0,A2
00001000                          BSR       $00001008
00001002                          MOVEM.L   (A7)+,D0-D7/A0-A6
00001006                          RTS
00001008                          JSR       $00001428(pc)
0000100C                          MOVE.W    #$0010,$0000(A0)
00001012                          MOVE.W    #$0108,$0002(A0)
00001018                          MOVE.W    #$FFFF,$0004(A0)
0000101E                          RTS
00001020                          MOVE.B    #$01,$E18E
00001026                          BSR       $00000FA8
00001028                          MOVE.W    #$0101,$E0EA
0000102E                          BSR       $00000FC0
00001030                          MOVE.W    #$0101,$E0F0
00001036                          BSR       $00000FD8
00001038                          MOVE.W    #$0101,$E0F6
0000103E                          BSR       $00000FF0
00001040                          MOVE.W    #$0101,$E0FC
00001046                          BSR       $000010A0
00001048                          JSR       $000014BE
0000104E                          TST.W     $E0FA
00001052                          BNE       $00001046
00001054                          MOVE.B    #$00,$E18E
0000105A                          RTS
0000105C                          MOVE.B    #$01,$E18E
00001062                          BSR       $00000F26(pc)
00001066                          MOVE.W    #$0101,$E0D2
0000106C                          BSR       $00000F3E(pc)
00001070                          MOVE.W    #$0101,$E0D8
00001076                          BSR       $00000F56(pc)
0000107A                          MOVE.W    #$0101,$E0DE
00001080                          BSR       $00000F6E(pc)
00001084                          MOVE.W    #$0101,$E0E4
0000108A                          BSR       $000010A0
0000108C                          JSR       $000014BE
00001092                          TST.W     $E0E2
00001096                          BNE       $0000108A
00001098                          MOVE.B    #$00,$E18E
0000109E                          RTS
000010A0                          BSR       $000010B6
000010A2                          BSR       $000010BE
000010A4                          BSR       $000010C6
000010A6                          BSR       $000010F4
000010A8                          BSR       $000010FC
000010AA                          BSR       $00001104
000010AC                          BSR       $0000110C
000010AE                          LEA       $E0D0,A0
000010B2                          MOVEQ     #$00,D5
000010B4                          BRA       $000010CC
000010B6                          LEA       $E0D6,A0
000010BA                          MOVEQ     #$20,D5
000010BC                          BRA       $000010CC
000010BE                          LEA       $E0DC,A0
000010C2                          MOVEQ     #$40,D5
000010C4                          BRA       $000010CC
000010C6                          LEA       $E0E2,A0
000010CA                          MOVEQ     #$60,D5
000010CC                          TST.W     $0000(A0)
000010D0                          BEQ       $000010DC
000010D2                          TST.B     $0002(A0)
000010D6                          BEQ       $000010DE
000010D8                          SUBQ.B    #$1,$0002(A0)
000010DC                          RTS
000010DE                          MOVE.B    $0003(A0),$0002(A0)
000010E4                          SUBQ.W    #$1,$0000(A0)
000010E8                          MOVEQ     #$10,D4
000010EA                          SUB.W     $0000(A0),D4
000010EE                          MOVE.W    $0004(A0),D3
000010F2                          BRA       $00001136
000010F4                          LEA       $E0E8,A0
000010F8                          MOVEQ     #$00,D5
000010FA                          BRA       $00001112
000010FC                          LEA       $E0EE,A0
00001100                          MOVEQ     #$20,D5
00001102                          BRA       $00001112
00001104                          LEA       $E0F4,A0
00001108                          MOVEQ     #$40,D5
0000110A                          BRA       $00001112
0000110C                          LEA       $E0FA,A0
00001110                          MOVEQ     #$60,D5
00001112                          TST.W     $0000(A0)
00001116                          BEQ       $00001122
00001118                          TST.B     $0002(A0)
0000111C                          BEQ       $00001124
0000111E                          SUBQ.B    #$1,$0002(A0)
00001122                          RTS
00001124                          MOVE.B    $0003(A0),$0002(A0)
0000112A                          SUBQ.W    #$1,$0000(A0)
0000112E                          MOVE.W    $0000(A0),D4
00001132                          MOVE.W    $0004(A0),D3
00001136                          LEA       $E710,A1
0000113A                          LEA       $50(A1,D0.W),A1
0000113E                          LEA       $0080(A1),A2
00001142                          MOVEQ     #$0F,D7
00001144                          MOVE.W    (A2)+,D0
00001146                          ROL.W     #1,D3
00001148                          BCC       $00001176
0000114A                          MOVE.W    D0,D1
0000114C                          MOVE.W    D0,D2
0000114E                          AND.W     #$0E00,D0
00001152                          AND.W     #$00E0,D1
00001156                          AND.W     #$000E,D2
0000115A                          MULU      D4,D0
0000115C                          MULU      D4,D1
0000115E                          MULU      D4,D2
00001160                          LSR.W     #4,D0
00001162                          LSR.W     #4,D1
00001164                          LSR.W     #4,D2
00001166                          AND.W     #$0E00,D0
0000116A                          AND.W     #$00E0,D1
0000116E                          AND.W     #$000E,D2
00001172                          ADD.W     D1,D0
00001174                          ADD.W     D2,D0
00001176                          MOVE.W    D0,(A1)+
00001178                          DBRA      D7,$00001144
0000117C                          MOVE.B    #$01,$E183
00001182                          RTS
00001184                          MOVEQ     #$00,D0
00001186                          MOVE.B    D0,$ED6E
0000118A                          MOVE.B    D0,$ED8E
0000118E                          MOVE.B    D0,$EDAE
00001192                          MOVE.B    D0,$EDCE
00001196                          MOVE.B    #$02,$EDAF
0000119C                          MOVE.B    #$01,$EDCF
000011A2                          MOVE.B    #$01,$EDEF
000011A8                          RTS
000011AA                          LEA       $00FF0000,A1
000011B0                          MOVEQ     #$00,D1
000011B2                          MOVE.W    #$3F7F,D2
000011B6                          MOVE.L    D1,(A1)+
000011B8                          DBRA      D2,$000011B6
000011BC                          RTS
000011BE                          LEA       $00FF0000,A1
000011C4                          MOVEQ     #$00,D1
000011C6                          MOVE.W    #$3EFF,D7
000011CA                          MOVE.L    D1,(A1)+
000011CC                          DBRA      D7,$000011CA
000011D0                          RTS
000011D2                          MOVE.W    #$0000,$EA08
000011D8                          MOVE.W    #$0000,$EA0C
000011DE                          MOVE.W    #$0000,$EA88
000011E4                          MOVE.W    #$0000,$EA8C
000011EA                          RTS
000011EC                          LEA       $9C00,A1
000011F0                          MOVEQ     #$00,D1
000011F2                          BRA       $00001324(pc)
000011F6                          MOVE.B    #$03,$FC27
000011FC                          MOVE.B    #$81,$FC3E
00001202                          MOVE.B    #$81,$FC3F
00001208                          MOVE.B    #$81,$FC40
0000120E                          MOVE.B    #$00,$FC41
00001214                          MOVE.B    #$00,$FC42
0000121A                          RTS
0000121C                          MOVEM.L   D0-D3/A6,-(A7)
00001220                          MOVE.W    D0,D2
00001222                          BMI       $00001270
00001224                          MOVE.W    D1,D3
00001226                          BMI       $00001270
00001228                          LEA       $EB00,A6
0000122C                          LSR.W     #8,D0
0000122E                          LSR.W     #3,D1
00001230                          AND.W     #$FFE0,D1
00001234                          ADD.W     D1,D0
00001236                          MOVE.B    $00(A6,D0.W),D0
0000123A                          LEA       $00FF0000,A6
00001240                          BCLR      #$07,D0
00001244                          BEQ       $0000124C
00001246                          LEA       $00FF3000,A6
0000124C                          LSL.W     #8,D0
0000124E                          ADD.W     D0,D0
00001250                          MOVE.W    D2,D1
00001252                          LSR.W     #3,D1
00001254                          AND.W     #$001E,D1
00001258                          ADD.W     D1,D0
0000125A                          MOVE.W    D3,D1
0000125C                          ADD.W     D1,D1
0000125E                          AND.W     #$01E0,D1
00001262                          ADD.W     D1,D0
00001264                          MOVE.W    $00(A6,D0.W),$E116
0000126A                          MOVEM.L   (A7)+,D0-D3/A6
0000126E                          RTS
00001270                          MOVE.W    #$0000,$E116
00001276                          MOVEM.L   (A7)+,D0-D3/A6
0000127A                          RTS
0000127C                          MOVEM.L   D0-D3/A6,-(A7)
00001280                          MOVE.W    D0,D2
00001282                          BMI       $000012D0
00001284                          MOVE.W    D1,D3
00001286                          BMI       $000012D0
00001288                          LEA       $ED00,A6
0000128C                          LSR.W     #8,D0
0000128E                          LSR.W     #3,D1
00001290                          AND.W     #$FFE0,D1
00001294                          ADD.W     D1,D0
00001296                          MOVE.B    $00(A6,D0.W),D0
0000129A                          LEA       $00FF6000,A6
000012A0                          BCLR      #$07,D0
000012A4                          BEQ       $000012AC
000012A6                          LEA       $00FF7C00,A6
000012AC                          LSL.W     #8,D0
000012AE                          ADD.W     D0,D0
000012B0                          MOVE.W    D2,D1
000012B2                          LSR.W     #3,D1
000012B4                          AND.W     #$001E,D1
000012B8                          ADD.W     D1,D0
000012BA                          MOVE.W    D3,D1
000012BC                          ADD.W     D1,D1
000012BE                          AND.W     #$01E0,D1
000012C2                          ADD.W     D1,D0
000012C4                          MOVE.W    $00(A6,D0.W),$E116
000012CA                          MOVEM.L   (A7)+,D0-D3/A6
000012CE                          RTS
000012D0                          MOVE.W    #$0000,$E116
000012D6                          MOVEM.L   (A7)+,D0-D3/A6
000012DA                          RTS
000012DC                          MOVE.W    (A1),D0
000012DE                          SUBQ.W    #$1,D0
000012E0                          CMP.W     D6,D0
000012E2                          BGE       $000012E6
000012E4                          MOVE.W    D7,D0
000012E6                          MOVE.W    D0,(A1)
000012E8                          RTS
000012EA                          MOVE.W    (A1),D0
000012EC                          ADDQ.W    #$1,D0
000012EE                          CMP.W     D7,D0
000012F0                          BLS       $000012F4
000012F2                          MOVE.W    D6,D0
000012F4                          MOVE.W    D0,(A1)
000012F6                          RTS
000012F8                          MOVE.B    (A1),D0
000012FA                          SUBQ.B    #$1,D0
000012FC                          CMP.B     D6,D0
000012FE                          BGE       $00001302
00001300                          MOVE.B    D7,D0
00001302                          MOVE.B    D0,(A1)
00001304                          RTS
00001306                          MOVE.B    (A1),D0
00001308                          ADDQ.B    #$1,D0
0000130A                          CMP.B     D7,D0
0000130C                          BLS       $00001310
0000130E                          MOVE.B    D6,D0
00001310                          MOVE.B    D0,(A1)
00001312                          RTS
00001314                          BSR       $00001318
00001316                          BRA       $00001330
00001318                          BSR       $0000131C
0000131A                          BRA       $00001330
0000131C                          BSR       $00001320
0000131E                          BRA       $00001330
00001320                          BSR       $00001324
00001322                          BRA       $00001330
00001324                          BSR       $00001328
00001326                          BRA       $00001330
00001328                          BSR       $0000132C
0000132A                          BRA       $00001330
0000132C                          BSR       $00001330
0000132E                          NOP
00001330                          BSR       $00001334
00001332                          NOP
00001334                          MOVE.L    D1,(A1)+
00001336                          MOVE.L    D1,(A1)+
00001338                          MOVE.L    D1,(A1)+
0000133A                          MOVE.L    D1,(A1)+
0000133C                          MOVE.L    D1,(A1)+
0000133E                          MOVE.L    D1,(A1)+
00001340                          MOVE.L    D1,(A1)+
00001342                          MOVE.L    D1,(A1)+
00001344                          MOVE.L    D1,(A1)+
00001346                          MOVE.L    D1,(A1)+
00001348                          MOVE.L    D1,(A1)+
0000134A                          MOVE.L    D1,(A1)+
0000134C                          MOVE.L    D1,(A1)+
0000134E                          MOVE.L    D1,(A1)+
00001350                          MOVE.L    D1,(A1)+
00001352                          MOVE.L    D1,(A1)+
00001354                          MOVE.L    D1,(A1)+
00001356                          MOVE.L    D1,(A1)+
00001358                          MOVE.L    D1,(A1)+
0000135A                          MOVE.L    D1,(A1)+
0000135C                          MOVE.L    D1,(A1)+
0000135E                          MOVE.L    D1,(A1)+
00001360                          MOVE.L    D1,(A1)+
00001362                          MOVE.L    D1,(A1)+
00001364                          MOVE.L    D1,(A1)+
00001366                          MOVE.L    D1,(A1)+
00001368                          MOVE.L    D1,(A1)+
0000136A                          MOVE.L    D1,(A1)+
0000136C                          MOVE.L    D1,(A1)+
0000136E                          MOVE.L    D1,(A1)+
00001370                          MOVE.L    D1,(A1)+
00001372                          MOVE.L    D1,(A1)+
00001374                          RTS
00001376                          BSR       $0000137A
00001378                          BRA       $00001392
0000137A                          BSR       $0000137E
0000137C                          BRA       $00001392
0000137E                          BSR       $00001382
00001380                          BRA       $00001392
00001382                          BSR       $00001386
00001384                          BRA       $00001392
00001386                          BSR       $0000138A
00001388                          BRA       $00001392
0000138A                          BSR       $0000138E
0000138C                          BRA       $00001392
0000138E                          BSR       $00001392
00001390                          NOP
00001392                          BSR       $00001396
00001394                          NOP
00001396                          MOVE.L    D1,(A1)
00001398                          MOVE.L    D1,(A1)
0000139A                          MOVE.L    D1,(A1)
0000139C                          MOVE.L    D1,(A1)
0000139E                          MOVE.L    D1,(A1)
000013A0                          MOVE.L    D1,(A1)
000013A2                          MOVE.L    D1,(A1)
000013A4                          MOVE.L    D1,(A1)
000013A6                          MOVE.L    D1,(A1)
000013A8                          MOVE.L    D1,(A1)
000013AA                          MOVE.L    D1,(A1)
000013AC                          MOVE.L    D1,(A1)
000013AE                          MOVE.L    D1,(A1)
000013B0                          MOVE.L    D1,(A1)
000013B2                          MOVE.L    D1,(A1)
000013B4                          MOVE.L    D1,(A1)
000013B6                          MOVE.L    D1,(A1)
000013B8                          MOVE.L    D1,(A1)
000013BA                          MOVE.L    D1,(A1)
000013BC                          MOVE.L    D1,(A1)
000013BE                          MOVE.L    D1,(A1)
000013C0                          MOVE.L    D1,(A1)
000013C2                          MOVE.L    D1,(A1)
000013C4                          MOVE.L    D1,(A1)
000013C6                          MOVE.L    D1,(A1)
000013C8                          MOVE.L    D1,(A1)
000013CA                          MOVE.L    D1,(A1)
000013CC                          MOVE.L    D1,(A1)
000013CE                          MOVE.L    D1,(A1)
000013D0                          MOVE.L    D1,(A1)
000013D2                          MOVE.L    D1,(A1)
000013D4                          MOVE.L    D1,(A1)
000013D6                          RTS
000013D8                          BSR       $000013DC
000013DA                          BRA       $000013F4
000013DC                          BSR       $000013E0
000013DE                          BRA       $000013F4
000013E0                          BSR       $000013E4
000013E2                          BRA       $000013F4
000013E4                          BSR       $000013E8
000013E6                          BRA       $000013F4
000013E8                          BSR       $000013EC
000013EA                          BRA       $000013F4
000013EC                          BSR       $000013F0
000013EE                          BRA       $000013F4
000013F0                          BSR       $000013F4
000013F2                          NOP
000013F4                          BSR       $000013F8
000013F6                          NOP
000013F8                          MOVE.L    (A1)+,(A2)+
000013FA                          MOVE.L    (A1)+,(A2)+
000013FC                          MOVE.L    (A1)+,(A2)+
000013FE                          MOVE.L    (A1)+,(A2)+
00001400                          MOVE.L    (A1)+,(A2)+
00001402                          MOVE.L    (A1)+,(A2)+
00001404                          MOVE.L    (A1)+,(A2)+
00001406                          MOVE.L    (A1)+,(A2)+
00001408                          MOVE.L    (A1)+,(A2)+
0000140A                          MOVE.L    (A1)+,(A2)+
0000140C                          MOVE.L    (A1)+,(A2)+
0000140E                          MOVE.L    (A1)+,(A2)+
00001410                          MOVE.L    (A1)+,(A2)+
00001412                          MOVE.L    (A1)+,(A2)+
00001414                          MOVE.L    (A1)+,(A2)+
00001416                          MOVE.L    (A1)+,(A2)+
00001418                          MOVE.L    (A1)+,(A2)+
0000141A                          MOVE.L    (A1)+,(A2)+
0000141C                          MOVE.L    (A1)+,(A2)+
0000141E                          MOVE.L    (A1)+,(A2)+
00001420                          MOVE.L    (A1)+,(A2)+
00001422                          MOVE.L    (A1)+,(A2)+
00001424                          MOVE.L    (A1)+,(A2)+
00001426                          MOVE.L    (A1)+,(A2)+
00001428                          MOVE.L    (A1)+,(A2)+
0000142A                          MOVE.L    (A1)+,(A2)+
0000142C                          MOVE.L    (A1)+,(A2)+
0000142E                          MOVE.L    (A1)+,(A2)+
00001430                          MOVE.L    (A1)+,(A2)+
00001432                          MOVE.L    (A1)+,(A2)+
00001434                          MOVE.L    (A1)+,(A2)+
00001436                          MOVE.L    (A1)+,(A2)+
00001438                          RTS
0000143A                          BSR       $0000143E
0000143C                          BRA       $00001456
0000143E                          BSR       $00001442
00001440                          BRA       $00001456
00001442                          BSR       $00001446
00001444                          BRA       $00001456
00001446                          BSR       $0000144A
00001448                          BRA       $00001456
0000144A                          BSR       $0000144E
0000144C                          BRA       $00001456
0000144E                          BSR       $00001452
00001450                          BRA       $00001456
00001452                          BSR       $00001456
00001454                          NOP
00001456                          BSR       $0000145A
00001458                          NOP
0000145A                          MOVE.L    (A1)+,(A2)
0000145C                          MOVE.L    (A1)+,(A2)
0000145E                          MOVE.L    (A1)+,(A2)
00001460                          MOVE.L    (A1)+,(A2)
00001462                          MOVE.L    (A1)+,(A2)
00001464                          MOVE.L    (A1)+,(A2)
00001466                          MOVE.L    (A1)+,(A2)
00001468                          MOVE.L    (A1)+,(A2)
0000146A                          MOVE.L    (A1)+,(A2)
0000146C                          MOVE.L    (A1)+,(A2)
0000146E                          MOVE.L    (A1)+,(A2)
00001470                          MOVE.L    (A1)+,(A2)
00001472                          MOVE.L    (A1)+,(A2)
00001474                          MOVE.L    (A1)+,(A2)
00001476                          MOVE.L    (A1)+,(A2)
00001478                          MOVE.L    (A1)+,(A2)
0000147A                          MOVE.L    (A1)+,(A2)
0000147C                          MOVE.L    (A1)+,(A2)
0000147E                          MOVE.L    (A1)+,(A2)
00001480                          MOVE.L    (A1)+,(A2)
00001482                          MOVE.L    (A1)+,(A2)
00001484                          MOVE.L    (A1)+,(A2)
00001486                          MOVE.L    (A1)+,(A2)
00001488                          MOVE.L    (A1)+,(A2)
0000148A                          MOVE.L    (A1)+,(A2)
0000148C                          MOVE.L    (A1)+,(A2)
0000148E                          MOVE.L    (A1)+,(A2)
00001490                          MOVE.L    (A1)+,(A2)
00001492                          MOVE.L    (A1)+,(A2)
00001494                          MOVE.L    (A1)+,(A2)
00001496                          MOVE.L    (A1)+,(A2)
00001498                          MOVE.L    (A1)+,(A2)
0000149A                          RTS
0000149C                          MOVE.W    $E102,D0
000014A0                          MOVE.W    #$0005,$E102
000014A6                          BSR       $000014D4
000014A8                          MOVE.W    D0,$E102
000014AC                          RTS
000014AE                          MOVE.W    #$0006,$E102
000014B4                          BRA       $000014D4
000014B6                          MOVE.W    #$0003,$E102
000014BC                          BRA       $000014D4
000014BE                          MOVE.W    #$0002,$E102
000014C4                          BRA       $000014D4
000014C6                          MOVE.W    #$0001,$E102
000014CC                          BRA       $000014D4
000014CE                          MOVE.W    #$0000,$E102
000014D4                          MOVE.B    #$01,$E180
000014DA                          MOVE      #$2500,SR
000014DE                          TST.B     $E180
000014E2                          BNE       $000014DE
000014E4                          RTS
000014E6                          MOVEQ     #$00,D0
000014E8                          MOVE.B    $0022(A0),D0
000014EC                          TST.B     $005B(A0)
000014F0                          BEQ       $000014F6
000014F2                          MOVE.B    $0023(A0),D0
000014F6                          AND.W     #$0007,D0
000014FA                          RTS
000014FC                          MOVEQ     #$00,D1
000014FE                          MOVE.B    $0022(A0),D1
00001502                          TST.B     $005B(A0)
00001506                          BEQ       $0000150C
00001508                          MOVE.B    $0023(A0),D1
0000150C                          AND.W     #$0007,D1
00001510                          RTS
00001512                          MOVEQ     #$00,D0
00001514                          MOVE.B    $0023(A0),D0
00001518                          TST.B     $005B(A0)
0000151C                          BEQ       $00001522
0000151E                          MOVE.B    $0022(A0),D0
00001522                          AND.W     #$0007,D0
00001526                          RTS
00001528                          MOVEQ     #$00,D1
0000152A                          MOVE.B    $0023(A0),D1
0000152E                          TST.B     $005B(A0)
00001532                          BEQ       $00001538
00001534                          MOVE.B    $0022(A0),D1
00001538                          AND.W     #$0007,D1
0000153C                          RTS
0000153E                          MOVEQ     #$00,D0
00001540                          MOVEQ     #$00,D1
00001542                          MOVE.B    $0031(A0),D0
00001546                          MOVE.B    $0022(A0),D1
0000154A                          TST.B     $005B(A0)
0000154E                          BEQ       $00001558
00001550                          MOVE.B    $0032(A0),D0
00001554                          MOVE.B    $0023(A0),D1
00001558                          AND.W     #$0007,D0
0000155C                          AND.W     #$0007,D1
00001560                          RTS
00001562                          TST.B     $A000
00001566                          BEQ       $000015B4
00001568                          MOVE.L    #$70B80003,$00C00004
00001572                          LEA       $00C00000,A1
00001578                          LEA       $FC3E,A3
0000157C                          BSR       $00001586
0000157E                          BSR       $00001586
00001580                          BSR       $00001586
00001582                          BSR       $00001586
00001584                          NOP
00001586                          MOVE.B    (A3)+,D1
00001588                          BEQ       $000015B4
0000158A                          MOVE.W    #$8699,D0
0000158E                          CMP.B     #$80,D1
00001592                          BEQ       $000015AC
00001594                          MOVE.W    #$869C,D0
00001598                          CMP.B     #$81,D1
0000159C                          BEQ       $000015AC
0000159E                          MOVE.W    #$869B,D0
000015A2                          CMP.B     #$84,D1
000015A6                          BEQ       $000015AC
000015A8                          MOVE.W    #$869A,D0
000015AC                          MOVE.W    D0,(A1)
000015AE                          ADD.W     #$0800,D0
000015B2                          MOVE.W    D0,(A1)
000015B4                          RTS
000015B6                          TST.W     $FC50
000015BA                          BNE       $00001650(pc)
000015BE                          LEA       $FC3E,A1
000015C2                          MOVE.W    #$0004,D5
000015C6                          MOVE.B    (A1),D0
000015C8                          BEQ       $000015D8
000015CA                          CMP.B     #$80,D0
000015CE                          BEQ       $000015D8
000015D0                          LEA       $0001(A1),A1
000015D4                          DBRA      D5,$000015C6
000015D8                          LEA       -$0001(A1),A1
000015DC                          MOVE.B    (A1),D0
000015DE                          CMP.B     #$83,D0
000015E2                          BEQ       $000015EA
000015E4                          MOVE.B    #$80,(A1)
000015E8                          RTS
000015EA                          MOVE.L    $FC44,D0
000015EE                          BEQ       $0000161C
000015F0                          MOVE.L    D0,A2
000015F2                          MOVE.W    #$0600,$0002(A2)
000015F8                          CMP.B     #$02,$FC3C
000015FE                          BNE       $00001608
00001600                          MOVE.B    #$01,$FC3C
00001606                          BRA       $0000160E
00001608                          MOVE.B    #$00,$FC3C
0000160E                          MOVE.L    #$00000000,$FC44
00001616                          MOVE.B    #$80,(A1)
0000161A                          RTS
0000161C                          MOVE.L    $FC48,D0
00001620                          BEQ       $0000164E
00001622                          MOVE.L    $FC48,A2
00001626                          MOVE.W    #$0600,$0002(A2)
0000162C                          CMP.B     #$02,$FC3C
00001632                          BNE       $0000163C
00001634                          MOVE.B    #$01,$FC3C
0000163A                          BRA       $00001642
0000163C                          MOVE.B    #$00,$FC3C
00001642                          MOVE.L    #$00000000,$FC48
0000164A                          MOVE.B    #$80,(A1)
0000164E                          RTS
00001650                          MOVE.W    $FC44,D0
00001654                          BEQ       $00001666
00001656                          MOVE.B    #$01,$F0D8
0000165C                          MOVE.L    $FC44,A2
00001660                          MOVE.W    #$0600,$0002(A2)
00001666                          RTS
00001668                          MOVEM.L   D1/D5/A1,-(A7)
0000166C                          LEA       $FC3E,A1
00001670                          MOVE.W    #$0004,D5
00001674                          MOVE.B    (A1),D1
00001676                          BEQ       $00001686
00001678                          CMP.B     #$80,D1
0000167C                          BEQ       $00001686
0000167E                          LEA       $0001(A1),A1
00001682                          DBRA      D5,$00001674
00001686                          LEA       -$0001(A1),A1
0000168A                          MOVE.B    (A1),D1
0000168C                          CMP.B     #$83,D1
00001690                          BEQ       $00001686
00001692                          MOVE.B    #$83,(A1)
00001696                          MOVEM.L   (A7)+,D1/D5/A1
0000169A                          RTS
0000169C                          LEA       $FC3E,A1
000016A0                          MOVEQ     #$04,D5
000016A2                          MOVE.B    (A1),D1
000016A4                          CMP.B     #$80,D1
000016A8                          BEQ       $000016B2
000016AA                          LEA       $0001(A1),A1
000016AE                          DBRA      D5,$000016A2
000016B2                          MOVE.B    #$81,(A1)
000016B6                          SUBQ.B    #$1,$FC4E
000016BA                          BCC       $000016C2
000016BC                          MOVE.B    #$00,$FC4E
000016C2                          RTS
000016C4                          LEA       $FC3E,A1
000016C8                          MOVEQ     #$00,D5
000016CA                          MOVE.B    $FC27,D5
000016CE                          SUBQ.B    #$1,D5
000016D0                          MOVE.B    (A1),D1
000016D2                          BEQ       $000016EC
000016D4                          CMP.B     #$80,D1
000016D8                          BNE       $000016DE
000016DA                          MOVE.B    #$81,(A1)
000016DE                          LEA       $0001(A1),A1
000016E2                          DBRA      D5,$000016D0
000016E6                          MOVE.B    #$00,$FC4E
000016EC                          RTS
000016EE                          LEA       $FC3E,A1
000016F2                          MOVEQ     #$00,D5
000016F4                          MOVE.B    $FC27,D5
000016F8                          SUBQ.B    #$1,D5
000016FA                          MOVE.B    (A1),D1
000016FC                          BEQ       $00001706
000016FE                          MOVE.B    #$80,(A1)+
00001702                          DBRA      D5,$000016FA
00001706                          RTS
00001708                          MOVEQ     #$00,D0
0000170A                          MOVE.B    $FC27,D0
0000170E                          SUBQ.B    #$1,D0
00001710                          LEA       $FC3E,A1
00001714                          MOVE.B    (A1),D1
00001716                          CMP.B     #$80,D1
0000171A                          BHI       $00001726
0000171C                          CMP.B     #$80,D1
00001720                          BEQ       $0000173C
00001722                          MOVE.B    #$81,(A1)
00001726                          LEA       $0001(A1),A1
0000172A                          DBRA      D0,$00001714
0000172E                          SUBQ.B    #$1,$FC4E
00001732                          BCC       $0000173A
00001734                          MOVE.B    #$00,$FC4E
0000173A                          RTS
0000173C                          MOVE.B    #$81,(A1)+
00001740                          SUBQ.B    #$1,D0
00001742                          BCS       $00001758
00001744                          MOVE.B    (A1),D1
00001746                          CMP.B     #$80,D1
0000174A                          BHI       $00001754
0000174C                          MOVE.B    #$80,(A1)
00001750                          LEA       $0001(A1),A1
00001754                          DBRA      D0,$00001744
00001758                          RTS
0000175A                          MOVEQ     #$04,D0
0000175C                          LEA       $FC3E,A1
00001760                          MOVE.B    (A1),D1
00001762                          BEQ       $00001776
00001764                          CMP.B     #$83,D1
00001768                          BNE       $0000176E
0000176A                          MOVE.B    #$84,(A1)
0000176E                          LEA       $0001(A1),A1
00001772                          DBRA      D0,$00001760
00001776                          RTS
00001778                          MOVEQ     #$04,D0
0000177A                          LEA       $FC3E,A1
0000177E                          MOVE.B    (A1),D1
00001780                          BEQ       $00001794
00001782                          CMP.B     #$84,D1
00001786                          BNE       $0000178C
00001788                          MOVE.B    #$83,(A1)
0000178C                          LEA       $0001(A1),A1
00001790                          DBRA      D0,$0000177E
00001794                          RTS
00001796                          MOVEQ     #$04,D0
00001798                          LEA       $FC42,A1
0000179C                          MOVE.B    (A1),D1
0000179E                          BEQ       $000017B0
000017A0                          CMP.B     #$81,D1
000017A4                          BEQ       $000017BA(pc)
000017A8                          CMP.B     #$84,D1
000017AC                          BEQ       $000017EC(pc)
000017B0                          LEA       -$0001(A1),A1
000017B4                          DBRA      D0,$0000179C
000017B8                          RTS
000017BA                          MOVE.B    #$80,(A1)
000017BE                          LEA       -$0001(A1),A1
000017C2                          MOVE.B    (A1),D1
000017C4                          CMP.B     #$84,D1
000017C8                          BEQ       $000017D0
000017CA                          DBRA      D0,$000017BE
000017CE                          RTS
000017D0                          MOVE.B    #$81,(A1)
000017D4                          LEA       -$0001(A1),A1
000017D8                          MOVE.B    (A1),D1
000017DA                          CMP.B     #$84,D1
000017DE                          BEQ       $000017E6
000017E0                          DBRA      D0,$000017D4
000017E4                          RTS
000017E6                          MOVE.B    #$83,(A1)
000017EA                          RTS
000017EC                          MOVE.B    #$80,(A1)
000017F0                          BRA       $000017D4
000017F2                          MOVEM.L   D0-D7/A0-A6,-(A7)
000017F6                          MOVE.W    $FC50,D0
000017FA                          BNE       $00001856
000017FC                          MOVE.B    #$00,$FC3C
00001802                          JSR       $00028A66
00001808                          BCS       $00001826
0000180A                          MOVE.B    #$01,$0000(A1)
00001810                          MOVE.W    $A008,$0008(A1)
00001816                          MOVE.W    $A00C,$000C(A1)
0000181C                          MOVE.L    A1,$FC44
00001820                          MOVE.B    #$01,$FC3C
00001826                          JSR       $00028A66
0000182C                          BCS       $00001884
0000182E                          MOVE.B    #$01,$0000(A1)
00001834                          MOVE.B    #$01,$0050(A1)
0000183A                          MOVE.W    $A008,$0008(A1)
00001840                          MOVE.W    $A00C,$000C(A1)
00001846                          MOVE.L    A1,$FC48
0000184A                          MOVE.B    #$02,$FC3C
00001850                          JSR       $00001778(pc)
00001854                          BRA       $00001884
00001856                          MOVE.W    #$0000,$FC50
0000185C                          JSR       $00028A66
00001862                          BCS       $00001884
00001864                          MOVE.B    #$01,$0000(A1)
0000186A                          MOVE.W    $A008,$0008(A1)
00001870                          MOVE.W    $A00C,$000C(A1)
00001876                          MOVE.L    A1,$FC44
0000187A                          MOVE.B    #$01,$FC3C
00001880                          JSR       $00001796(pc)
00001884                          MOVEM.L   (A7)+,D0-D7/A0-A6
00001888                          RTS
0000188A                          SUBQ.W    #$5,$000C(A0)
0000188E                          JSR       $0000194A(pc)
00001892                          ADDQ.W    #$5,$000C(A0)
00001896                          TST.B     D1
00001898                          BNE       $000018A6
0000189A                          ADDQ.W    #$5,$000C(A0)
0000189E                          JSR       $0000194A(pc)
000018A2                          SUBQ.W    #$5,$000C(A0)
000018A6                          RTS
000018A8                          ADDQ.W    #$5,$000C(A0)
000018AC                          JSR       $000018C6(pc)
000018B0                          SUBQ.W    #$5,$000C(A0)
000018B4                          TST.B     D1
000018B6                          BNE       $000018C4
000018B8                          SUBQ.W    #$5,$000C(A0)
000018BC                          JSR       $000018C6(pc)
000018C0                          ADDQ.W    #$5,$000C(A0)
000018C4                          RTS
000018C6                          TST.B     $0052(A0)
000018CA                          BEQ       $000018D4
000018CC                          SUBQ.B    #$1,$0052(A0)
000018D0                          BRA       $00001946(pc)
000018D4                          LEA       $0004011C,A1
000018DA                          JSR       $00001F5C(pc)
000018DE                          MOVE.W    $E116,D1
000018E2                          ADD.W     D1,D1
000018E4                          MOVE.L    $E014,A3
000018E8                          JSR       $000019C2(pc)
000018EC                          JSR       $00001F46(pc)
000018F0                          BEQ       $00001928
000018F2                          MOVE.B    $10(A3,A0.W),D1
000018F6                          BEQ       $00001946
000018F8                          CMP.B     #$32,D1
000018FC                          BCC       $00001946
000018FE                          AND.W     #$00FF,D1
00001902                          CMP.B     #$10,D1
00001906                          BCS       $00001928
00001908                          LEA       $000D4B4A,A3
0000190E                          MOVE.W    D3,D0
00001910                          AND.W     #$000F,D0
00001914                          SUB.B     #$10,D1
00001918                          MOVE.W    D1,D2
0000191A                          LSL.W     #4,D1
0000191C                          ADD.W     D2,D1
0000191E                          ADD.W     D1,D0
00001920                          ADDQ.W    #$1,D0
00001922                          MOVE.B    $00(A3,D0.W),D1
00001926                          BEQ       $00001946
00001928                          MOVE.W    D4,D5
0000192A                          AND.W     #$000F,D5
0000192E                          MOVE.W    #$000F,D0
00001932                          SUB.W     D5,D0
00001934                          SUB.W     #$0010,D1
00001938                          NEG.W     D1
0000193A                          AND.W     #$000F,D1
0000193E                          SUB.W     D1,D0
00001940                          BMI       $00001946
00001942                          MOVEQ     #$01,D1
00001944                          RTS
00001946                          MOVEQ     #$00,D1
00001948                          RTS
0000194A                          TST.B     $0052(A0)
0000194E                          BEQ       $00001958
00001950                          SUBQ.B    #$1,$0052(A0)
00001954                          BRA       $000019BE(pc)
00001958                          LEA       $00040000,A1
0000195E                          JSR       $00001F5C(pc)
00001962                          MOVE.W    $E116,D1
00001966                          ADD.W     D1,D1
00001968                          MOVE.L    $E014,A3
0000196C                          BSR       $000019C2
0000196E                          JSR       $00001F46(pc)
00001972                          BEQ       $000019AA
00001974                          MOVE.B    $10(A3,A0.W),D1
00001978                          BEQ       $000019BE
0000197A                          CMP.B     #$20,D1
0000197E                          BCC       $000019BE
00001980                          AND.W     #$00FF,D1
00001984                          CMP.B     #$10,D1
00001988                          BCS       $000019AA
0000198A                          LEA       $000D4B4A,A3
00001990                          MOVE.W    D3,D0
00001992                          AND.W     #$000F,D0
00001996                          SUB.B     #$10,D1
0000199A                          MOVE.W    D1,D2
0000199C                          LSL.W     #4,D1
0000199E                          ADD.W     D2,D1
000019A0                          ADD.W     D1,D0
000019A2                          ADDQ.W    #$1,D0
000019A4                          MOVE.B    $00(A3,D0.W),D1
000019A8                          BEQ       $000019BE
000019AA                          MOVE.W    D4,D6
000019AC                          AND.W     #$000F,D6
000019B0                          AND.W     #$000F,D1
000019B4                          MOVE.W    D1,D0
000019B6                          SUB.W     D6,D0
000019B8                          BPL       $000019BE
000019BA                          MOVEQ     #$01,D1
000019BC                          RTS
000019BE                          MOVEQ     #$00,D1
000019C0                          RTS
000019C2                          TST.B     $F17F
000019C6                          BNE       $000019D2
000019C8                          TST.B     $F180
000019CC                          BEQ       $000019D2
000019CE                          SUB.W     #$01CC,A3
000019D2                          RTS
000019D4                          ADDQ.W    #$5,$000C(A0)
000019D8                          BSR       $000019F8
000019DA                          BCS       $000019EE
000019DC                          SUB.W     #$000A,$000C(A0)
000019E2                          BSR       $000019F8
000019E4                          BCS       $000019EE
000019E6                          ADDQ.W    #$5,$000C(A0)
000019EA                          MOVE      #$00,CCR
000019EE                          RTS
000019F0                          LEA       $00040000,A1
000019F6                          BRA       $000019FE
000019F8                          LEA       $0004011C,A1
000019FE                          JSR       $00001F5C(pc)
00001A02                          MOVE.W    $E116,D1
00001A06                          ADD.W     D1,D1
00001A08                          MOVE.L    $E014,A3
00001A0C                          BSR       $000019C2
00001A0E                          JSR       $00001F46(pc)
00001A12                          BEQ       $00001EDA(pc)
00001A16                          MOVE.B    $10(A3,D0.W),D7
00001A1A                          MOVE.B    $10(A3,A0.W),D1
00001A1E                          BEQ       $00001EDA(pc)
00001A22                          CMP.B     #$1B,D1
00001A26                          BEQ       $00001A86(pc)
00001A2A                          CMP.B     #$50,D1
00001A2E                          BEQ       $00001F00(pc)
00001A32                          CMP.B     #$30,D1
00001A36                          BCC       $00001EDA(pc)
00001A3A                          AND.W     #$00FF,D1
00001A3E                          LEA       $000D4D8C,A3
00001A44                          MOVE.W    D3,D0
00001A46                          AND.W     #$000F,D0
00001A4A                          SUB.B     #$10,D1
00001A4E                          MOVE.W    D1,D2
00001A50                          LSL.W     #4,D1
00001A52                          ADD.W     D2,D1
00001A54                          MOVE.B    $10(A3,D0.W),$0023(A0)
00001A5A                          ADD.W     D1,D0
00001A5C                          ADDQ.W    #$1,D0
00001A5E                          MOVE.B    $00(A3,D0.W),D1
00001A62                          BEQ       $00001EDA(pc)
00001A66                          MOVE.W    D4,D5
00001A68                          AND.W     #$000F,D5
00001A6C                          MOVE.W    #$000F,D0
00001A70                          SUB.W     D5,D0
00001A72                          AND.W     #$000F,D1
00001A76                          SUB.W     #$0010,D1
00001A7A                          NEG.W     D1
00001A7C                          AND.W     #$000F,D1
00001A80                          SUB.W     D1,D0
00001A82                          BRA       $00001EF8(pc)
00001A86                          MOVE.B    #$01,$0023(A0)
00001A8C                          MOVEQ     #$0F,D1
00001A8E                          MOVE.W    D4,D5
00001A90                          AND.W     #$000F,D5
00001A94                          MOVE.W    #$000F,D0
00001A98                          SUB.W     D5,D0
00001A9A                          SUB.W     #$0010,D1
00001A9E                          NEG.W     D1
00001AA0                          AND.W     #$000F,D1
00001AA4                          SUB.W     D1,D0
00001AA6                          SUB.W     #$0010,D0
00001AAA                          BRA       $00001EF8(pc)
00001AAE                          SUBQ.W    #$5,$000C(A0)
00001AB2                          BSR       $00001B74(pc)
00001AB6                          BCS       $00001AE0
00001AB8                          ADD.W     #$000A,$000C(A0)
00001ABE                          BSR       $00001B74(pc)
00001AC2                          BCS       $00001AE0
00001AC4                          SUB.W     #$000A,$000C(A0)
00001ACA                          BSR       $00001AE2
00001ACC                          BCS       $00001AE0
00001ACE                          ADD.W     #$000A,$000C(A0)
00001AD4                          BSR       $00001AE2
00001AD6                          BCS       $00001AE0
00001AD8                          SUBQ.W    #$5,$000C(A0)
00001ADC                          BRA       $00001EE2(pc)
00001AE0                          RTS
00001AE2                          LEA       $00040000,A1
00001AE8                          JSR       $00001F5C(pc)
00001AEC                          MOVE.W    $E116,D1
00001AF0                          ADD.W     D1,D1
00001AF2                          MOVE.L    $E014,A3
00001AF6                          JSR       $000019C2(pc)
00001AFA                          JSR       $00001F46(pc)
00001AFE                          BEQ       $00001EE2(pc)
00001B02                          MOVE.B    $10(A3,D0.W),D7
00001B06                          MOVE.B    $10(A3,A0.W),D1
00001B0A                          BEQ       $00001EE2(pc)
00001B0E                          CMP.B     #$50,D1
00001B12                          BEQ       $00001F00(pc)
00001B16                          CMP.B     #$30,D1
00001B1A                          BEQ       $00001B2A
00001B1C                          CMP.B     #$31,D1
00001B20                          BEQ       $00001B2A
00001B22                          CMP.B     #$20,D1
00001B26                          BCC       $00001EE2(pc)
00001B2A                          AND.W     #$00FF,D1
00001B2E                          MOVE.B    #$01,$0022(A0)
00001B34                          CMP.B     #$10,D1
00001B38                          BCS       $00001B62
00001B3A                          LEA       $000D4B4A,A3
00001B40                          MOVE.W    D3,D0
00001B42                          AND.W     #$000F,D0
00001B46                          SUB.B     #$10,D1
00001B4A                          MOVE.W    D1,D2
00001B4C                          LSL.W     #4,D1
00001B4E                          ADD.W     D2,D1
00001B50                          MOVE.B    $10(A3,D0.W),$0022(A0)
00001B56                          ADD.W     D1,D0
00001B58                          ADDQ.W    #$1,D0
00001B5A                          MOVE.B    $00(A3,D0.W),D1
00001B5E                          BEQ       $00001EE2(pc)
00001B62                          MOVE.W    D4,D5
00001B64                          AND.W     #$000F,D5
00001B68                          AND.W     #$000F,D1
00001B6C                          MOVE.W    D1,D0
00001B6E                          SUB.W     D5,D0
00001B70                          BRA       $00001EF8(pc)
00001B74                          CMP.B     #$03,$FC0A
00001B7A                          BNE       $00001EE2(pc)
00001B7E                          CMP.B     #$04,$FC26
00001B84                          BCS       $00001EE2(pc)
00001B88                          MOVE.B    #$00,$E196
00001B8E                          LEA       $00040000,A1
00001B94                          MOVEQ     #$00,D0
00001B96                          MOVE.B    $0010(A0),D0
00001B9A                          MOVE.B    $00(A1,D0.W),D0
00001B9E                          ADD.W     #$00E8,D0
00001BA2                          LEA       $00(A1,D0.W),A1
00001BA6                          MOVE.W    $EA88,D0
00001BAA                          ADD.W     (A1)+,D0
00001BAC                          MOVE.W    $A008,D3
00001BB0                          SUB.W     $EA08,D3
00001BB4                          ADD.W     D3,D0
00001BB6                          MOVE.W    D0,D3
00001BB8                          MOVE.W    $EA8C,D1
00001BBC                          ADD.W     (A1)+,D1
00001BBE                          MOVE.W    $A00C,D4
00001BC2                          SUB.W     $EA0C,D4
00001BC6                          ADD.W     D4,D1
00001BC8                          MOVE.W    D1,D4
00001BCA                          JSR       $0000127C(pc)
00001BCE                          MOVE.W    $E116,D1
00001BD2                          ADD.W     D1,D1
00001BD4                          MOVE.L    $E014,A3
00001BD8                          MOVE.B    $10(A3,D0.W),D7
00001BDC                          MOVE.B    $10(A3,A0.W),D1
00001BE0                          BEQ       $00001EE2(pc)
00001BE4                          CMP.B     #$20,D1
00001BE8                          BCC       $00001EE2(pc)
00001BEC                          AND.W     #$00FF,D1
00001BF0                          MOVE.B    #$01,$0022(A0)
00001BF6                          CMP.B     #$10,D1
00001BFA                          BCS       $00001C24
00001BFC                          LEA       $000D4B4A,A3
00001C02                          MOVE.W    D3,D0
00001C04                          AND.W     #$000F,D0
00001C08                          SUB.B     #$10,D1
00001C0C                          MOVE.W    D1,D2
00001C0E                          LSL.W     #4,D1
00001C10                          ADD.W     D2,D1
00001C12                          MOVE.B    $10(A3,D0.W),$0022(A0)
00001C18                          ADD.W     D1,D0
00001C1A                          ADDQ.W    #$1,D0
00001C1C                          MOVE.B    $00(A3,D0.W),D1
00001C20                          BEQ       $00001EE2(pc)
00001C24                          MOVE.W    D4,D5
00001C26                          AND.W     #$000F,D5
00001C2A                          AND.W     #$000F,D1
00001C2E                          MOVE.W    D1,D0
00001C30                          SUB.W     D5,D0
00001C32                          MOVE.B    #$01,$E196
00001C38                          BRA       $00001EF8(pc)
00001C3C                          LEA       $0004032C,A1
00001C42                          JSR       $00001F5C(pc)
00001C46                          MOVE.W    $E116,D1
00001C4A                          ADD.W     D1,D1
00001C4C                          MOVE.L    $E014,A3
00001C50                          JSR       $000019C2(pc)
00001C54                          JSR       $00001F46(pc)
00001C58                          BEQ       $00001EEA(pc)
00001C5C                          MOVE.B    $10(A3,D0.W),D7
00001C60                          CMP.B     #$50,$10(A3,A0.W)
00001C66                          BEQ       $00001C90
00001C68                          CMP.B     #$32,$10(A3,A0.W)
00001C6E                          BEQ       $00001C7A
00001C70                          CMP.B     #$30,$10(A3,A0.W)
00001C76                          BNE       $00001EEA(pc)
00001C7A                          MOVE.B    #$01,$0024(A0)
00001C80                          MOVE.W    D3,D5
00001C82                          AND.W     #$000F,D5
00001C86                          MOVEQ     #$0F,D0
00001C88                          SUB.W     D5,D0
00001C8A                          DBRA      D0,$00007C8C
00001C8E                          AND.W     #$4EFA,$0294(A2)
00001C94                          LEA       $00040220,A1
00001C9A                          JSR       $00001F5C(pc)
00001C9E                          MOVE.W    $E116,D1
00001CA2                          ADD.W     D1,D1
00001CA4                          MOVE.L    $E014,A3
00001CA8                          JSR       $000019C2(pc)
00001CAC                          JSR       $00001F46(pc)
00001CB0                          BEQ       $00001EEA(pc)
00001CB4                          MOVE.B    $10(A3,D0.W),D7
00001CB8                          CMP.B     #$50,$10(A3,A0.W)
00001CBE                          BEQ       $00001CE2
00001CC0                          CMP.B     #$31,$10(A3,A0.W)
00001CC6                          BNE       $00001EEA(pc)
00001CCA                          MOVE.B    #$02,$0024(A0)
00001CD0                          MOVE.W    D3,D5
00001CD2                          AND.W     #$000F,D5
00001CD6                          MOVE.W    #$000F,D0
00001CDA                          SUB.W     D5,D0
00001CDC                          SUBQ.W    #$7,D0
00001CDE                          BRA       $00001EF8(pc)
00001CE2                          JMP       $00001F26(pc)
00001CE6                          LEA       $00040000,A1
00001CEC                          JSR       $00001F5C(pc)
00001CF0                          MOVE.W    $E116,D1
00001CF4                          ADD.W     D1,D1
00001CF6                          MOVE.L    $E014,A3
00001CFA                          JSR       $000019C2(pc)
00001CFE                          JSR       $00001F46(pc)
00001D02                          BEQ       $00001EEA(pc)
00001D06                          CMP.B     #$50,$10(A3,A0.W)
00001D0C                          BEQ       $00001D18
00001D0E                          CMP.B     #$30,$10(A3,A0.W)
00001D14                          BNE       $00001EEA(pc)
00001D18                          MOVE.B    #$01,$0024(A0)
00001D1E                          MOVE.W    D3,D5
00001D20                          AND.W     #$000F,D5
00001D24                          MOVEQ     #$0F,D0
00001D26                          SUB.W     D5,D0
00001D28                          DBRA      D0,$00007D2A
00001D2C                          MOVEP.L   D0,$43F9(A4)
00001D30                          OR.B      #$00,D4
00001D34                          JSR       $00001F5C(pc)
00001D38                          MOVE.W    $E116,D1
00001D3C                          ADD.W     D1,D1
00001D3E                          MOVE.L    $E014,A3
00001D42                          JSR       $000019C2(pc)
00001D46                          JSR       $00001F46(pc)
00001D4A                          BEQ       $00001EEA(pc)
00001D4E                          CMP.B     #$50,$10(A3,A0.W)
00001D54                          BEQ       $00001D60
00001D56                          CMP.B     #$31,$10(A3,A0.W)
00001D5C                          BNE       $00001EEA(pc)
00001D60                          MOVE.B    #$02,$0024(A0)
00001D66                          MOVE.W    D3,D5
00001D68                          AND.W     #$000F,D5
00001D6C                          MOVE.W    #$000F,D0
00001D70                          SUB.W     D5,D0
00001D72                          SUBQ.W    #$7,D0
00001D74                          BRA       $00001EF8(pc)
00001D78                          ADDQ.W    #$5,$000C(A0)
00001D7C                          BSR       $00001D9A
00001D7E                          BCS       $00001D98
00001D80                          SUB.W     #$000A,$000C(A0)
00001D86                          BSR       $00001D9A
00001D88                          BCS       $00001D98
00001D8A                          ADDQ.W    #$5,$000C(A0)
00001D8E                          MOVE.B    #$00,$0022(A0)
00001D94                          MOVE      #$00,CCR
00001D98                          RTS
00001D9A                          LEA       $0004011C,A1
00001DA0                          JSR       $00001F5C(pc)
00001DA4                          MOVE.W    $E116,D1
00001DA8                          ADD.W     D1,D1
00001DAA                          MOVE.L    $E014,A3
00001DAE                          JSR       $000019C2(pc)
00001DB2                          JSR       $00001F46(pc)
00001DB6                          BEQ       $00001EE2(pc)
00001DBA                          MOVE.B    $10(A3,D0.W),D7
00001DBE                          MOVE.B    $10(A3,A0.W),D1
00001DC2                          BEQ       $00001EE2(pc)
00001DC6                          CMP.B     #$30,D1
00001DCA                          BCC       $00001EDA(pc)
00001DCE                          AND.W     #$00FF,D1
00001DD2                          MOVE.B    #$01,$0022(A0)
00001DD8                          CMP.B     #$10,D1
00001DDC                          BCS       $00001E06
00001DDE                          LEA       $000D4B4A,A3
00001DE4                          MOVE.W    D3,D0
00001DE6                          AND.W     #$000F,D0
00001DEA                          SUB.B     #$10,D1
00001DEE                          MOVE.W    D1,D2
00001DF0                          LSL.W     #4,D1
00001DF2                          ADD.W     D2,D1
00001DF4                          MOVE.B    $10(A3,D0.W),$0022(A0)
00001DFA                          ADD.W     D1,D0
00001DFC                          ADDQ.W    #$1,D0
00001DFE                          MOVE.B    $00(A3,D0.W),D1
00001E02                          BEQ       $00001EE2(pc)
00001E06                          MOVE.W    D4,D5
00001E08                          AND.W     #$000F,D5
00001E0C                          AND.W     #$000F,D1
00001E10                          MOVE.W    D1,D0
00001E12                          SUB.W     D5,D0
00001E14                          BRA       $00001EF8(pc)
00001E18                          ADDQ.W    #$5,$000C(A0)
00001E1C                          JSR       $00001E50(pc)
00001E20                          BCC       $00001E2C
00001E22                          SUBQ.W    #$5,$000C(A0)
00001E26                          ADDQ.W    #$5,D0
00001E28                          BRA       $00001EF8(pc)
00001E2C                          SUB.W     #$000A,$000C(A0)
00001E32                          JSR       $00001E50(pc)
00001E36                          BCS       $00001E46
00001E38                          ADDQ.W    #$5,$000C(A0)
00001E3C                          MOVE.B    #$00,$0022(A0)
00001E42                          BRA       $00001EF0(pc)
00001E46                          ADDQ.W    #$5,$000C(A0)
00001E4A                          SUBQ.W    #$5,D0
00001E4C                          BRA       $00001EF8(pc)
00001E50                          LEA       $00040000,A1
00001E56                          JSR       $00001F5C(pc)
00001E5A                          MOVE.W    $E116,D1
00001E5E                          ADD.W     D1,D1
00001E60                          MOVE.L    $E014,A3
00001E64                          JSR       $000019C2(pc)
00001E68                          JSR       $00001F46(pc)
00001E6C                          BEQ       $00001EE2(pc)
00001E70                          MOVE.B    $10(A3,D0.W),D7
00001E74                          MOVE.B    $10(A3,A0.W),D1
00001E78                          BEQ       $00001EE2(pc)
00001E7C                          CMP.B     #$30,D1
00001E80                          BEQ       $00001E90
00001E82                          CMP.B     #$31,D1
00001E86                          BEQ       $00001E90
00001E88                          CMP.B     #$20,D1
00001E8C                          BCC       $00001EE2(pc)
00001E90                          AND.W     #$00FF,D1
00001E94                          MOVE.B    #$01,$0022(A0)
00001E9A                          CMP.B     #$10,D1
00001E9E                          BCS       $00001EC8
00001EA0                          LEA       $000D4B4A,A3
00001EA6                          MOVE.W    D3,D0
00001EA8                          AND.W     #$000F,D0
00001EAC                          SUB.B     #$10,D1
00001EB0                          MOVE.W    D1,D2
00001EB2                          LSL.W     #4,D1
00001EB4                          ADD.W     D2,D1
00001EB6                          MOVE.B    $10(A3,D0.W),$0022(A0)
00001EBC                          ADD.W     D1,D0
00001EBE                          ADDQ.W    #$1,D0
00001EC0                          MOVE.B    $00(A3,D0.W),D1
00001EC4                          BEQ       $00001EE2(pc)
00001EC8                          MOVE.W    D4,D5
00001ECA                          AND.W     #$000F,D5
00001ECE                          AND.W     #$000F,D1
00001ED2                          MOVE.W    D1,D0
00001ED4                          SUB.W     D5,D0
00001ED6                          BRA       $00001EF8(pc)
00001EDA                          MOVE.B    #$00,$0023(A0)
00001EE0                          BRA       $00001EF0
00001EE2                          MOVE.B    #$00,$0022(A0)
00001EE8                          BRA       $00001EF0
00001EEA                          MOVE.B    #$00,$0024(A0)
00001EF0                          MOVE.W    D7,D2
00001EF2                          MOVE      #$00,CCR
00001EF6                          RTS
00001EF8                          MOVE.W    D7,D2
00001EFA                          MOVE      #$01,CCR
00001EFE                          RTS
00001F00                          MOVE.L    A0,D2
00001F02                          CMP.W     #$A000,D2
00001F06                          BNE       $00001F1E
00001F08                          TST.W     $FC4C
00001F0C                          BNE       $00001F1E
00001F0E                          TST.W     $FC38
00001F12                          BNE       $00001F1E
00001F14                          ADDQ.B    #$1,$001E(A0)
00001F18                          MOVE.B    #$80,$0007(A0)
00001F1E                          MOVE.W    D7,D2
00001F20                          MOVE      #$00,CCR
00001F24                          RTS
00001F26                          MOVE.L    A0,D2
00001F28                          CMP.W     #$A000,D2
00001F2C                          BNE       $00001F44
00001F2E                          TST.W     $FC4C
00001F32                          BNE       $00001F44
00001F34                          TST.W     $FC38
00001F38                          BNE       $00001F44
00001F3A                          ADDQ.B    #$1,$001E(A0)
00001F3E                          MOVE.B    #$80,$0007(A0)
00001F44                          RTS
00001F46                          TST.B     $E18F
00001F4A                          BNE       $00001F54
00001F4C                          BTST      #$00,$10(A3,D0.W)
00001F52                          RTS
00001F54                          BTST      #$01,$10(A3,D0.W)
00001F5A                          RTS
00001F5C                          MOVEQ     #$00,D0
00001F5E                          MOVE.B    $0010(A0),D0
00001F62                          MOVE.B    $00(A1,D0.W),D0
00001F66                          ADD.W     #$00E8,D0
00001F6A                          LEA       $00(A1,D0.W),A1
00001F6E                          MOVE.W    $0008(A0),D0
00001F72                          ADD.W     (A1)+,D0
00001F74                          MOVE.W    D0,D3
00001F76                          MOVE.W    $000C(A0),D1
00001F7A                          ADD.W     (A1)+,D1
00001F7C                          MOVE.W    D1,D4
00001F7E                          BRA       $0000121C(pc)
00001F82                          JSR       $000011EC(pc)
00001F86                          MOVEQ     #$00,D0
00001F88                          MOVE.B    D0,$E18F
00001F8C                          MOVE.L    D0,$E080
00001F90                          MOVE.L    D0,$E084
00001F94                          MOVE.L    D0,$E088
00001F98                          MOVE.L    D0,$E08C
00001F9C                          MOVE.B    D0,$E192
00001FA0                          MOVE.W    D0,$E12C
00001FA4                          MOVE.W    D0,$E146
00001FA8                          MOVE.W    D0,$E142
00001FAC                          MOVE.B    D0,$E193
00001FB0                          MOVE.B    D0,$E187
00001FB4                          MOVE.B    D0,$E195
00001FB8                          MOVE.W    D0,$FC3A
00001FBC                          MOVE.B    D0,$E186
00001FC0                          MOVE.B    D0,$F0A6
00001FC4                          MOVE.B    D0,$F0A7
00001FC8                          MOVE.B    D0,$F0CB
00001FCC                          MOVE.B    D0,$FC4F
00001FD0                          MOVE.W    #$FFFF,D0
00001FD4                          MOVE.B    D0,$E1E2
00001FD8                          MOVE.B    D0,$E1EA
00001FDC                          MOVE.B    D0,$E1F4
00001FE0                          MOVE.B    D0,$E1FC
00001FE4                          LEA       $E0D0,A1
00001FE8                          MOVEQ     #$00,D1
00001FEA                          JSR       $0000135C(pc)
00001FEE                          LEA       $E200,A1
00001FF2                          MOVEQ     #$00,D1
00001FF4                          JSR       $00001334(pc)
00001FF8                          LEA       $A000,A1
00001FFC                          MOVEQ     #$00,D1
00001FFE                          JSR       $00001344(pc)
00002002                          JSR       $000025F6(pc)
00002006                          JSR       $00002598(pc)
0000200A                          MOVEQ     #$00,D0
0000200C                          MOVEQ     #$00,D1
0000200E                          MOVE.B    $FC0A,D0
00002012                          ADD.W     D0,D0
00002014                          MOVE.W    +$00(D6.L),D0
00002018                          MOVE.B    $FC26,D1
0000201C                          ADD.W     D1,D1
0000201E                          ADD.W     D1,D0
00002020                          MOVE.W    +$00(D0.L),D0
00002024                          JSR       +$00(D6.W)
00002028                          JSR       $0000858C(pc)
0000202C                          JSR       $0000899E(pc)
00002030                          RTS
00002032                          OR.B      #$1C,A2
00002036                          OR.B      #$3E,$004C(A4)
0000203C                          OR.W      #$0084,-(A2)
00002040                          OR.L      #$00B200B2,-(A0)
00002046                          Invalid
00002048                          Invalid
0000204A                          Invalid
0000204C                          BTST      D0,D4
0000204E                          BTST      D0,(A4)+
00002050                          BTST      D0,(A4)+
00002052                          BTST      D0,(A4)+
00002054                          BTST      D0,$01(A0,D4.W)
00002058                          BCHG      D0,-(A6)
0000205A                          BCLR      D0,(A6)
0000205C                          BCLR      D0,(A6)+
0000205E                          MOVEP.L   D0,$01DA(A0)
00002062                          BSET      D0,$0204(A4)
00002066                          AND.B     #$4C,(A4)+
0000206A                          AND.W     #$02BC,$02CE(A2)
00002070                          Invalid
00002072                          Invalid
00002074                          Invalid
00002076                          MOVEP.W   $0316(A2),D1
0000207A                          BTST      D1,$037C(A0)
0000207E                          MOVEP.W   D1,$0394(A0)
00002082                          BCLR      D1,-(A0)
00002084                          BCLR      D1,$03B8(A4)
00002088                          MOVEP.L   D1,$03D6(A2)
0000208C                          BSET      D1,-(A2)
0000208E                          BSET      D1,$04(A4,D0.W)
00002092                          SUB.B     #$F9,(A2)
00002096                          OR.B      #$82,D2
0000209A                          JSR       $00002474(pc)
0000209E                          JSR       $00000F6E(pc)
000020A2                          MOVE.B    #$01,$FC12
000020A8                          MOVE.B    #$01,$E080
000020AE                          MOVE.B    #$02,$E084
000020B4                          RTS
000020B6                          LEA       $00023CBE,A2
000020BC                          JSR       $00002474(pc)
000020C0                          JSR       $000201FA
000020C6                          JSR       $0000149C(pc)
000020CA                          JSR       $00020008
000020D0                          BRA       $000020A8
000020D2                          LEA       $00023CFA,A2
000020D8                          JSR       $00002474(pc)
000020DC                          MOVE.B    #$80,$EDC8
000020E2                          BRA       $000020AE
000020E4                          LEA       $00023D36,A2
000020EA                          JSR       $00002474(pc)
000020EE                          MOVE.B    #$01,$F0A0
000020F4                          MOVE.B    #$81,$EBB2
000020FA                          JSR       $00001184(pc)
000020FE                          BRA       $000020AE
00002100                          LEA       $00023D72,A2
00002106                          JSR       $00002474(pc)
0000210A                          MOVE.B    #$03,$EB96
00002110                          JSR       $00001184(pc)
00002114                          BRA       $000020AE
00002116                          LEA       $00023DAE,A2
0000211C                          JSR       $00002474(pc)
00002120                          MOVE.B    #$89,$EB98
00002126                          MOVE.B    #$8A,$EBB8
0000212C                          MOVE.B    #$01,$E18F
00002132                          BRA       $000020AE(pc)
00002136                          LEA       $00023DEA,A2
0000213C                          JSR       $00002474(pc)
00002140                          MOVE.B    #$81,$EBB2
00002146                          JSR       $00001184(pc)
0000214A                          BRA       $000020AE(pc)
0000214E                          CLR.B     $F0D3
00002152                          CLR.B     $F0D4
00002156                          LEA       $00023E26,A2
0000215C                          JSR       $00002474(pc)
00002160                          RTS
00002162                          MOVE.B    #$04,$E080
00002168                          MOVE.B    #$01,$E193
0000216E                          LEA       $00023E62,A2
00002174                          JSR       $00002474(pc)
00002178                          RTS
0000217A                          LEA       $00023E9E,A2
00002180                          JSR       $00002474(pc)
00002184                          MOVE.B    #$0A,$EE04
0000218A                          MOVE.B    #$07,$EE24
00002190                          MOVE.B    #$01,$FC4F
00002196                          RTS
00002198                          LEA       $00023EDA,A2
0000219E                          JSR       $00002474(pc)
000021A2                          CLR.B     $F25F
000021A6                          CLR.B     $F0D3
000021AA                          CLR.B     $F0D4
000021AE                          MOVE.B    #$02,$EA96
000021B4                          MOVE.B    #$00,$EA86
000021BA                          MOVE.B    #$01,$E192
000021C0                          MOVEQ     #$21,D0
000021C2                          JMP       $00029062
000021C8                          LEA       $00023F16,A2
000021CE                          BRA       $0000219E
000021D0                          MOVE.B    #$05,$E080
000021D6                          MOVE.B    #$01,$E187
000021DC                          MOVE.B    #$07,$F0D3
000021E2                          MOVE.B    #$00,$EA86
000021E8                          MOVE.B    #$02,$EA96
000021EE                          LEA       $00023F52,A2
000021F4                          JSR       $00002474(pc)
000021F8                          RTS
000021FA                          LEA       $00023AA2,A2
00002200                          JSR       $00002474(pc)
00002204                          MOVE.B    #$06,$EA96
0000220A                          RTS
0000220C                          LEA       $00023AA2,A2
00002212                          JSR       $00002474(pc)
00002216                          MOVE.B    #$06,$EA96
0000221C                          RTS
0000221E                          LEA       $00023ADE,A2
00002224                          JSR       $00002474(pc)
00002228                          MOVE.B    #$06,$EA96
0000222E                          MOVE.W    #$0000,$EBA8
00002234                          RTS
00002236                          LEA       $00023B1A,A2
0000223C                          JSR       $00002474(pc)
00002240                          MOVE.B    #$06,$EA96
00002246                          MOVE.B    #$0B,$E08C
0000224C                          RTS
0000224E                          LEA       $00023B56,A2
00002254                          JSR       $00002474(pc)
00002258                          MOVE.B    #$85,$EB93
0000225E                          MOVE.B    #$85,$EB74
00002264                          MOVE.B    #$86,$EB75
0000226A                          MOVE.B    #$87,$EB76
00002270                          MOVE.B    #$01,$E192
00002276                          MOVE.B    #$07,$E080
0000227C                          RTS
0000227E                          LEA       $00023B92,A2
00002284                          JSR       $00002474(pc)
00002288                          MOVE.B    #$01,$DE92
0000228E                          MOVE.B    #$07,$E080
00002294                          MOVE.B    #$06,$EA96
0000229A                          RTS
0000229C                          LEA       $00023BCE,A2
000022A2                          JSR       $00002474(pc)
000022A6                          MOVE.L    #$07970797,D0
000022AC                          MOVE.L    D0,$00FF0CD6
000022B2                          MOVE.L    D0,$00FF0CDA
000022B8                          MOVE.W    D0,$00FF0CDE
000022BE                          MOVE.L    D0,$00FF0EC0
000022C4                          MOVE.L    D0,$00FF0EC4
000022CA                          MOVE.L    D0,$00FF0EC8
000022D0                          MOVE.L    D0,$00FF0ECC
000022D6                          MOVE.L    #$07980799,$00FF0ED0
000022E0                          MOVE.B    #$2C,$DE62
000022E6                          MOVE.B    #$06,$EA96
000022EC                          RTS
000022EE                          LEA       $00023C0A,A2
000022F4                          JSR       $00002474(pc)
000022F8                          MOVE.B    #$06,$EA96
000022FE                          RTS
00002300                          LEA       $00023C46,A2
00002306                          JSR       $00002474(pc)
0000230A                          MOVE.B    #$8B,$EB49
00002310                          MOVE.B    #$8B,$EB4A
00002316                          MOVE.B    #$01,$E192
0000231C                          MOVE.B    #$06,$EA96
00002322                          RTS
00002324                          LEA       $0002393A,A2
0000232A                          JSR       $00002474(pc)
0000232E                          RTS
00002330                          LEA       $00023976,A2
00002336                          JSR       $00002474(pc)
0000233A                          RTS
0000233C                          LEA       $000239B2,A2
00002342                          JSR       $00002474(pc)
00002346                          RTS
00002348                          LEA       $000239EE,A2
0000234E                          JSR       $00002474(pc)
00002352                          MOVE.B    #$01,$E195
00002358                          RTS
0000235A                          LEA       $00023A2A,A2
00002360                          JSR       $00002474(pc)
00002364                          MOVEQ     #$00,D0
00002366                          LEA       $00087C64,A0
0000236C                          LEA       $00FF4A00,A1
00002372                          JSR       $0002077E
00002378                          LEA       $00087D8A,A0
0000237E                          LEA       $00FF4400,A1
00002384                          JSR       $0002077E
0000238A                          LEA       $00087E3A,A0
00002390                          LEA       $00FF5000,A1
00002396                          JSR       $0002077E
0000239C                          LEA       $00087F88,A0
000023A2                          LEA       $00FF3A00,A1
000023A8                          JMP       $0002077E
000023AE                          LEA       $00023A66,A2
000023B4                          JSR       $00002474(pc)
000023B8                          RTS
000023BA                          LEA       $000236A6,A2
000023C0                          JSR       $00002474(pc)
000023C4                          RTS
000023C6                          LEA       $000236E2,A2
000023CC                          JSR       $00002474(pc)
000023D0                          RTS
000023D2                          LEA       $0002371E,A2
000023D8                          JSR       $00002474(pc)
000023DC                          RTS
000023DE                          LEA       $0002375A,A2
000023E4                          JSR       $00002474(pc)
000023E8                          RTS
000023EA                          LEA       $00023796,A2
000023F0                          JSR       $00002474(pc)
000023F4                          MOVE.B    #$0A,$E080
000023FA                          RTS
000023FC                          LEA       $000237D2,A2
00002402                          JSR       $00002474(pc)
00002406                          RTS
00002408                          LEA       $0002380E,A2
0000240E                          JSR       $00002474(pc)
00002412                          RTS
00002414                          MOVE.B    #$08,$E080
0000241A                          LEA       $0002384A,A2
00002420                          JSR       $00002474(pc)
00002424                          RTS
00002426                          LEA       $00023886,A2
0000242C                          JSR       $00002474(pc)
00002430                          RTS
00002432                          MOVE.B    #$08,$E080
00002438                          LEA       $000238C2,A2
0000243E                          JSR       $00002474(pc)
00002442                          RTS
00002444                          MOVE.B    #$09,$E080
0000244A                          MOVE.B    #$01,$F17D
00002450                          LEA       $000238FE,A2
00002456                          JSR       $00002474(pc)
0000245A                          MOVE.L    #$01030005,$EBD6
00002462                          MOVE.L    #$02040005,$EBF6
0000246A                          MOVE.L    #$080A0000,$EBB8
00002472                          RTS
00002474                          MOVEQ     #$00,D1
00002476                          LEA       $00FF0000,A6
0000247C                          MOVE.W    #$26FF,D0
00002480                          MOVE.L    D1,(A6)+
00002482                          DBRA      D0,$00002480
00002486                          LEA       $000CFC34,A6
0000248C                          MOVEQ     #$00,D0
0000248E                          MOVE.B    $FC0A,D0
00002492                          ADD.W     D0,D0
00002494                          MOVE.W    $00(A6,D0.W),D0
00002498                          LEA       $00(A6,D0.W),A6
0000249C                          MOVE.W    (A6)+,D7
0000249E                          LEA       $EB00,A5
000024A2                          MOVE.B    (A6)+,(A5)+
000024A4                          DBRA      D7,$000024A2
000024A8                          MOVE.W    (A6)+,D7
000024AA                          LEA       $ED00,A5
000024AE                          MOVE.B    (A6)+,(A5)+
000024B0                          DBRA      D7,$000024AE
000024B4                          LEA       $00002570(pc),A6
000024B8                          MOVEQ     #$00,D0
000024BA                          MOVE.B    $FC0A,D0
000024BE                          ADD.W     D0,D0
000024C0                          ADD.W     D0,D0
000024C2                          MOVE.L    $00(A6,D0.W),$E018
000024C8                          LEA       $00002584(pc),A6
000024CC                          MOVE.L    $00(A6,D0.W),$E014
000024D2                          MOVE.W    (A2)+,$A008
000024D6                          MOVE.W    (A2)+,$A00C
000024DA                          MOVE.W    (A2)+,$EA08
000024DE                          MOVE.W    (A2)+,$EA0C
000024E2                          MOVE.W    (A2)+,$EA88
000024E6                          MOVE.W    (A2)+,$EA8C
000024EA                          MOVE.L    (A2)+,$EA2A
000024EE                          MOVE.W    (A2)+,$EA2E
000024F2                          MOVE.W    (A2)+,$EA30
000024F6                          MOVE.L    (A2)+,$EA32
000024FA                          MOVE.W    (A2)+,$EA36
000024FE                          MOVE.W    (A2)+,$EA38
00002502                          MOVE.L    (A2)+,$DE62
00002506                          MOVE.B    (A2)+,$EA87
0000250A                          MOVE.B    (A2)+,$FC26
0000250E                          MOVE.B    (A2)+,$EA01
00002512                          MOVE.B    (A2)+,$EA81
00002516                          LEA       $00FF0000,A1
0000251C                          BSR       $0000254E
0000251E                          LEA       $00FF3000,A1
00002524                          BSR       $0000254E
00002526                          LEA       $00FF6000,A1
0000252C                          BSR       $0000254E
0000252E                          LEA       $00FF7C00,A1
00002534                          BSR       $0000254E
00002536                          MOVE.L    (A2)+,D0
00002538                          JSR       $00022D74
0000253E                          BSR       $0000255E
00002540                          BSR       $0000255E
00002542                          BSR       $0000255E
00002544                          MOVE.B    (A2)+,$9C0A
00002548                          JMP       $000286F6
0000254E                          MOVE.L    (A2)+,D0
00002550                          BMI       $0000255C
00002552                          MOVE.L    D0,A0
00002554                          MOVEQ     #$00,D0
00002556                          JMP       $0002077E
0000255C                          RTS
0000255E                          MOVEQ     #$00,D0
00002560                          MOVE.B    (A2)+,D0
00002562                          CMP.B     #$FF,D0
00002566                          BEQ       $0000256E
00002568                          JMP       $00029062
0000256E                          RTS
00002570                          OR.B      #$AA,D5
00002574                          OR.B      #$46,D6
00002578                          OR.B      #$D8,D7
0000257C                          OR.B      #$0A,D7
00002580                          OR.B      #$66,A0
00002584                          OR.B      #$9E,A5
00002588                          OR.B      #$8A,A5
0000258C                          OR.B      #$F4,A5
00002590                          OR.B      #$52,A5
00002594                          OR.B      #$72,A5
00002598                          JSR       $000233B6
0000259E                          JSR       $000232A0
000025A4                          JSR       $00023548
000025AA                          MOVEQ     #$00,D1
000025AC                          JSR       $000025DE(pc)
000025B0                          JSR       $000025E6(pc)
000025B4                          JSR       $000025FE(pc)
000025B8                          JSR       $00002606(pc)
000025BC                          MOVEQ     #$00,D0
000025BE                          MOVE.B    $FC27,D0
000025C2                          SUBQ.B    #$1,D0
000025C4                          LEA       $FC3E,A1
000025C8                          MOVE.B    (A1),D1
000025CA                          CMP.B     #$80,D1
000025CE                          BNE       $000025D4
000025D0                          MOVE.B    #$81,(A1)
000025D4                          LEA       $0001(A1),A1
000025D8                          DBRA      D0,$000025C8
000025DC                          RTS
000025DE                          LEA       $A000,A1
000025E2                          BRA       $00001344(pc)
000025E6                          LEA       $A060,A0
000025EA                          JSR       $00001344(pc)
000025EE                          LEA       $A0C0,A0
000025F2                          BRA       $00001344(pc)
000025F6                          LEA       $EA00,A1
000025FA                          BRA       $00001330(pc)
000025FE                          LEA       $DE60,A1
00002602                          BRA       $00001354(pc)
00002606                          LEA       $C4E0,A1
0000260A                          JSR       $00001314(pc)
0000260E                          JSR       $00001314(pc)
00002612                          BRA       $00001324(pc)
00002616                          MOVEM.L   D0-D7/A0-A6,-(A7)
0000261A                          TST.B     $E180
0000261E                          BEQ       $00002764(pc)
00002622                          MOVE.B    #$00,$E180
00002628                          LEA       $00C00004,A1
0000262E                          LEA       $00C00000,A2
00002634                          MOVE.W    (A1),D0
00002636                          TST.B     $E183
0000263A                          BEQ       $0000268A
0000263C                          MOVE.B    #$00,$E183
00002642                          MOVE.W    #$0100,$00A11100
0000264A                          BTST      #$00,$00A11100
00002652                          BNE       $0000264A
00002654                          LEA       $00C00004,A6
0000265A                          MOVE.L    #$93409400,(A6)
00002660                          MOVE.L    #$958896F3,(A6)
00002666                          MOVE.W    #$977F,(A6)
0000266A                          MOVE.W    #$C000,(A6)
0000266E                          MOVE.W    #$0080,-(A7)
00002672                          MOVE.W    (A7)+,(A6)
00002674                          MOVE.L    #$C0000000,(A6)
0000267A                          MOVE.W    $E710,$00C00000
00002682                          MOVE.W    #$0000,$00A11100
0000268A                          MOVE.W    $E102,D0
0000268E                          ADD.W     D0,D0
00002690                          ADD.W     D0,D0
00002692                          JMP       +$00(D1.W)
00002696                          BRA       $000026B6(pc)
0000269A                          BRA       $000026EC(pc)
0000269E                          BRA       $000026FC(pc)
000026A2                          BRA       $0000270E(pc)
000026A6                          BRA       $00002716(pc)
000026AA                          BRA       $00002728(pc)
000026AE                          BRA       $00002730(pc)
000026B2                          BRA       $0000273C(pc)
000026B6                          JSR       $00002792(pc)
000026BA                          JSR       $00008C3E(pc)
000026BE                          JSR       $00028E16
000026C4                          JSR       $00028E7C
000026CA                          JSR       $00028EC0
000026D0                          JSR       $00020958
000026D6                          JSR       $00020A64
000026DC                          JSR       $00001562
000026E2                          JSR       $000206EC
000026E8                          BRA       $0000274C(pc)
000026EC                          JSR       $00020402
000026F2                          JSR       $000206EC
000026F8                          BRA       $00002764(pc)
000026FC                          JSR       $00020402
00002702                          JSR       $00002792(pc)
00002706                          JSR       $00008C3E(pc)
0000270A                          BRA       $00002764(pc)
0000270E                          JSR       $00008C3E(pc)
00002712                          BRA       $0000274C(pc)
00002716                          JSR       $00002792(pc)
0000271A                          JSR       $00008C3E(pc)
0000271E                          JSR       $0000EB26
00002724                          BRA       $0000274C(pc)
00002728                          JSR       $00008C3E(pc)
0000272C                          BRA       $0000274C(pc)
00002730                          JSR       $00002792(pc)
00002734                          JSR       $00008C3E(pc)
00002738                          BRA       $0000274C(pc)
0000273C                          JSR       $000027DC(pc)
00002740                          JSR       $000970CA
00002746                          JSR       $00097108
0000274C                          JSR       $00020402
00002752                          TST.W     $E104
00002756                          BEQ       $0000275C
00002758                          SUBQ.W    #$1,$E104
0000275C                          ADDQ.W    #$1,$E106
00002760                          ADDQ.W    #$1,$F09E
00002764                          BTST      #$06,$FD1B
0000276A                          BEQ       $00002782
0000276C                          LEA       $FD1C,A1
00002770                          MOVEQ     #$00,D6
00002772                          MOVEQ     #$05,D7
00002774                          JSR       $00001306(pc)
00002778                          TST.B     D0
0000277A                          BNE       $00002782
0000277C                          JSR       $000B00F8
00002782                          JSR       $000B00F8
00002788                          ADDQ.W    #$1,$E11A
0000278C                          MOVEM.L   (A7)+,D0-D7/A0-A6
00002790                          RTE
00002792                          MOVE.W    #$0100,$00A11100
0000279A                          BTST      #$00,$00A11100
000027A2                          BNE       $0000279A
000027A4                          LEA       $00C00004,A6
000027AA                          MOVE.L    #$93409401,(A6)
000027B0                          MOVE.L    #$954096F1,(A6)
000027B6                          MOVE.W    #$977F,(A6)
000027BA                          MOVE.W    #$7800,(A6)
000027BE                          MOVE.W    #$0083,-(A7)
000027C2                          MOVE.W    (A7)+,(A6)
000027C4                          MOVE.L    #$78000003,(A6)
000027CA                          MOVE.W    $E280,$00C00000
000027D2                          MOVE.W    #$0000,$00A11100
000027DA                          RTS
000027DC                          MOVE.W    #$0100,$00A11100
000027E4                          BTST      #$00,$00A11100
000027EC                          BNE       $000027E4
000027EE                          LEA       $00C00004,A6
000027F4                          MOVE.L    #$93409401,(A6)
000027FA                          MOVE.L    #$954096F1,(A6)
00002800                          MOVE.W    #$977F,(A6)
00002804                          MOVE.W    #$7C00,(A6)
00002808                          MOVE.W    #$0082,-(A7)
0000280C                          MOVE.W    (A7)+,(A6)
0000280E                          MOVE.L    #$7C000002,(A6)
00002814                          MOVE.W    $E280,$00C00000
0000281C                          MOVE.W    #$0000,$00A11100
00002824                          RTS
00002826                          BCLR      #$06,$FD1F
0000282C                          MOVE.W    $FD1E,$00C00004
00002834                          JSR       $00020352
0000283A                          JSR       $00022DBC
00002840                          JSR       $000011BE
00002846                          MOVEQ     #$03,D0
00002848                          JSR       $00022D74
0000284E                          MOVEQ     #$04,D0
00002850                          JSR       $00028F50
00002856                          MOVEQ     #$02,D0
00002858                          JSR       $00022966
0000285E                          MOVE.W    #$00B4,$E104
00002864                          MOVE.W    #$0028,$E11C
0000286A                          JSR       $0000149C(pc)
0000286E                          BSET      #$06,$FD1F
00002874                          MOVE.W    $FD1E,$00C00004
0000287C                          JSR       $00022E7A
00002882                          JSR       $000014B6(pc)
00002886                          BTST      #$07,$E109
0000288C                          BNE       $00002894
0000288E                          TST.W     $E104
00002892                          BNE       $0000287C
00002894                          MOVE.W    #$0001,$E100
0000289A                          RTS
0000289C                          JSR       $0000E03E
000028A2                          JSR       $000234C6
000028A8                          JSR       $00020A0E
000028AE                          JSR       $00020A62
000028B4                          JSR       $000011F6
000028BA                          MOVE.W    #$0000,$FD06
000028C0                          MOVE.W    #$07D0,$FD0A
000028C6                          RTS
000028C8                          BCLR      #$06,$FD1F
000028CE                          MOVE.W    $FD1E,$00C00004
000028D6                          JSR       $00022DBC
000028DC                          JSR       $00020352
000028E2                          JSR       $000011D2
000028E8                          MOVE.L    #$08083534,D0
000028EE                          JSR       $00022D74
000028F4                          MOVEQ     #$0A,D0
000028F6                          JSR       $00028F50
000028FC                          MOVE.W    #$0000,$E124
00002902                          JSR       $0000149C(pc)
00002906                          BSET      #$06,$FD1F
0000290C                          MOVE.W    $FD1E,$00C00004
00002914                          MOVE.B    $E109,D0
00002918                          AND.B     #$0C,D0
0000291C                          BEQ       $00002924
0000291E                          EOR.W     #$0001,$E124
00002924                          LEA       $0002439A,A0
0000292A                          MOVE.W    #$8000,D0
0000292E                          JSR       $00023F8E
00002934                          LEA       $000243AC,A0
0000293A                          MOVE.W    $E124,D0
0000293E                          MULU      #$000A,D0
00002942                          LEA       $00(A0,D0.W),A0
00002946                          MOVE.W    #$A000,D0
0000294A                          JSR       $00023F8E
00002950                          JSR       $000014B6(pc)
00002954                          BTST      #$07,$E109
0000295A                          BEQ       $00002914
0000295C                          TST.W     $E124
00002960                          BEQ       $0000296A
00002962                          MOVE.W    #$0005,$E100
00002968                          RTS
0000296A                          MOVE.B    #$00,$FC0A
00002970                          MOVE.B    #$00,$FC26
00002976                          MOVE.W    #$0002,$E100
0000297C                          RTS
0000297E                          TST.B     $FC0A
00002982                          BNE       $00002A2A(pc)
00002986                          TST.B     $FC26
0000298A                          BNE       $00002A2A(pc)
0000298E                          BCLR      #$06,$FD1F
00002994                          MOVE.W    $FD1E,$00C00004
0000299C                          JSR       $00022DBC
000029A2                          JSR       $0002034C
000029A8                          JSR       $000011D2
000029AE                          MOVE.W    #$FF00,$EA0C
000029B4                          MOVE.B    #$03,$E080
000029BA                          MOVE.B    #$D6,$9C0A
000029C0                          MOVE.L    #$08080706,D0
000029C6                          JSR       $00022D74
000029CC                          MOVEQ     #$09,D0
000029CE                          JSR       $00028F50
000029D4                          JSR       $00002AAC(pc)
000029D8                          JSR       $00002ACA(pc)
000029DC                          MOVE.W    #$0100,$E104
000029E2                          MOVE.W    #$0000,$E126
000029E8                          MOVE.W    #$0000,$E12A
000029EE                          MOVE.B    #$03,$FD1A
000029F4                          JSR       $00003138(pc)
000029F8                          JSR       $0000149C(pc)
000029FC                          BSET      #$06,$FD1F
00002A02                          MOVE.W    $FD1E,$00C00004
00002A0A                          ADDQ.W    #$2,$EA0C
00002A0E                          SUBQ.W    #$1,$EA8C
00002A12                          JSR       $00000BE2(pc)
00002A16                          BSR       $00002A32
00002A18                          JSR       $000014B6(pc)
00002A1C                          BTST      #$07,$E109
00002A22                          BNE       $00002A2A
00002A24                          TST.W     $E104
00002A28                          BNE       $00002A0A
00002A2A                          MOVE.W    #$0003,$E100
00002A30                          RTS
00002A32                          MOVE.B    $E109,D7
00002A36                          MOVE.W    $E12A,D0
00002A3A                          MOVE.W    +$00(D3.W),D0
00002A3E                          JMP       +$00(D1.W)
00002A42                          OR.B      #$1C,A4
00002A46                          OR.B      #$3C,$004C(A4)
00002A4C                          OR.W      #$4A07,-(A0)
00002A50                          BEQ       $00002AA2
00002A52                          CMP.B     #$40,D7
00002A56                          BNE       $00002AA4
00002A58                          ADDQ.W    #$2,$E12A
00002A5C                          RTS
00002A5E                          TST.B     D7
00002A60                          BEQ       $00002AA2
00002A62                          CMP.B     #$20,D7
00002A66                          BNE       $00002AA4
00002A68                          ADDQ.W    #$2,$E12A
00002A6C                          RTS
00002A6E                          TST.B     D7
00002A70                          BEQ       $00002AA2
00002A72                          CMP.B     #$10,D7
00002A76                          BNE       $00002AA4
00002A78                          ADDQ.W    #$2,$E12A
00002A7C                          RTS
00002A7E                          TST.B     D7
00002A80                          BEQ       $00002AA2
00002A82                          CMP.B     #$20,D7
00002A86                          BNE       $00002AA4
00002A88                          ADDQ.W    #$2,$E12A
00002A8C                          RTS
00002A8E                          TST.B     D7
00002A90                          BEQ       $00002AA2
00002A92                          CMP.B     #$40,D7
00002A96                          BNE       $00002AA4
00002A98                          ADDQ.W    #$2,$E12A
00002A9C                          MOVE.B    #$01,$FD19
00002AA2                          RTS
00002AA4                          MOVE.W    #$0000,$E12A
00002AAA                          RTS
00002AAC                          MOVE.W    #$0000,D0
00002AB0                          LEA       $00053F04,A0
00002AB6                          LEA       $00FF0000,A1
00002ABC                          JSR       $0002077E
00002AC2                          MOVE.L    #$40080003,D0
00002AC8                          BRA       $00002AFE
00002ACA                          MOVE.W    #$2000,D0
00002ACE                          LEA       $0005402A,A0
00002AD4                          LEA       $00FF0000,A1
00002ADA                          JSR       $0002077E
00002AE0                          MOVE.L    #$60000003,D0
00002AE6                          BSR       $00002AFE
00002AE8                          MOVE.L    #$60400003,D0
00002AEE                          BSR       $00002AFE
00002AF0                          MOVE.L    #$70000003,D0
00002AF6                          BSR       $00002AFE
00002AF8                          MOVE.L    #$70400003,D0
00002AFE                          LEA       $00FF0000,A1
00002B04                          MOVEQ     #$1F,D2
00002B06                          MOVEQ     #$1F,D1
00002B08                          LEA       $00C00004,A2
00002B0E                          LEA       $00C00000,A3
00002B14                          MOVE.L    #$00800000,D4
00002B1A                          MOVE.L    D0,(A2)
00002B1C                          MOVE.W    D1,D3
00002B1E                          MOVE.W    (A1)+,(A3)
00002B20                          DBRA      D3,$00002B1E
00002B24                          ADD.L     D4,D0
00002B26                          DBRA      D2,$00002B1A
00002B2A                          RTS
00002B2C                          BCLR      #$06,$FD1F
00002B32                          MOVE.W    $FD1E,$00C00004
00002B3A                          JSR       $00020352
00002B40                          JSR       $00022DBC
00002B46                          MOVEQ     #$01,D0
00002B48                          JSR       $00022966
00002B4E                          MOVE.L    #$00030201,D0
00002B54                          JSR       $00028F50
00002B5A                          JSR       $00020A20
00002B60                          JSR       $000208FC
00002B66                          JSR       $00001F82
00002B6C                          MOVE.W    #$9202,$E114
00002B72                          MOVE.B    #$80,$E181
00002B78                          JSR       $0000149C(pc)
00002B7C                          BSET      #$06,$FD1F
00002B82                          MOVE.W    $FD1E,$00C00004
00002B8A                          JSR       $000031DE(pc)
00002B8E                          TST.B     $FC12
00002B92                          BEQ       $00002BAA
00002B94                          JSR       $0000DC7A
00002B9A                          JSR       $000010A0(pc)
00002B9E                          JSR       $00020AD2
00002BA4                          JSR       $000014BE(pc)
00002BA8                          BRA       $00002B8A
00002BAA                          JSR       $000031A0(pc)
00002BAE                          JSR       $000003E6(pc)
00002BB2                          JSR       $00020696
00002BB8                          JSR       $00000BE2(pc)
00002BBC                          JSR       $000010A0(pc)
00002BC0                          JSR       $00020AD2
00002BC6                          JSR       $000014CE(pc)
00002BCA                          TST.W     $E12C
00002BCE                          BEQ       $00002BDE
00002BD0                          SUBQ.W    #$1,$E12C
00002BD4                          BEQ       $00002C66(pc)
00002BD8                          MOVE.B    #$01,$F0CB
00002BDE                          TST.W     $E146
00002BE2                          BEQ       $00002BF2
00002BE4                          SUBQ.W    #$1,$E146
00002BE8                          BEQ       $00002C66(pc)
00002BEC                          MOVE.B    #$01,$F0CB
00002BF2                          TST.W     $E142
00002BF6                          BEQ       $00002C04
00002BF8                          SUBQ.W    #$1,$E142
00002BFC                          BEQ       $00002C32
00002BFE                          MOVE.B    #$01,$F0CB
00002C04                          TST.B     $F0A7
00002C08                          BEQ       $00002B8A
00002C0A                          SUBQ.B    #$1,$F0A7
00002C0E                          BNE       $00002B8A(pc)
00002C12                          MOVE.B    #$00,$E181
00002C18                          JSR       $00020A1C
00002C1E                          MOVE.W    #$0007,$E100
00002C24                          JSR       $00020130
00002C2A                          JSR       $00001020
00002C30                          RTS
00002C32                          MOVE.B    #$00,$E181
00002C38                          JSR       $000011F6
00002C3E                          JSR       $00020A1C
00002C44                          MOVE.W    #$0002,$E100
00002C4A                          TST.B     $FC00
00002C4E                          BNE       $00002C56
00002C50                          MOVE.W    #$0006,$E100
00002C56                          JSR       $00020130
00002C5C                          JSR       $00001020
00002C62                          BSR       $00002C7E
00002C64                          RTS
00002C66                          MOVE.B    #$00,$E181
00002C6C                          ADDQ.B    #$1,$FC0A
00002C70                          MOVE.B    #$00,$FC26
00002C76                          MOVE.W    #$0009,$E100
00002C7C                          RTS
00002C7E                          CMP.W     #$0002,$FDE2
00002C84                          BEQ       $00002C94
00002C86                          SUBQ.B    #$4,$FC09
00002C8A                          BPL       $00002CA0
00002C8C                          MOVE.B    #$00,$FC09
00002C92                          RTS
00002C94                          SUBQ.B    #$2,$FC09
00002C98                          BPL       $00002CA0
00002C9A                          MOVE.B    #$00,$FC09
00002CA0                          RTS
00002CA2                          JSR       $00095F0E
00002CA8                          MOVE.B    #$00,$FC0A
00002CAE                          MOVE.B    #$00,$FC26
00002CB4                          RTS
00002CB6                          OR.B      #$00,D1
00002CBA                          OR.B      #$2C,D2
00002CBE                          BTST      D0,D0
00002CC0                          OR.B      #$02,D0
00002CC4                          ADDQ.B    #$1,$0104(A4)
00002CC8                          OR.B      #$02,D0
00002CCC                          ððB       Internal check fail
00002CCE                          AND.B     #$00,D0
00002CD2                          OR.B      #$FC,D2
00002CD6                          BCLR      #$06,$FD1F
00002CDC                          MOVE.W    $FD1E,$00C00004
00002CE4                          JSR       $00020352
00002CEA                          JSR       $00022DBC
00002CF0                          MOVEQ     #$34,D0
00002CF2                          JSR       $00022966
00002CF8                          MOVE.L    #$00030201,D0
00002CFE                          JSR       $00028F50
00002D04                          JSR       $00020A20
00002D0A                          JSR       $000208FC
00002D10                          MOVE.W    $FD16,D0
00002D14                          ADDQ.W    #$0,$FD16
00002D18                          AND.W     #$001F,$FD16
00002D1E                          MOVE.B    +$00(D3.L),$FC0A
00002D24                          MOVE.B    +$00(A0.L),$FC26
00002D2A                          MOVE.L    +$00(D7.W),$E18A
00002D30                          JSR       $00001F82
00002D36                          JSR       $000011F6
00002D3C                          MOVE.B    #$01,$E189
00002D42                          MOVE.B    #$00,$E181
00002D48                          MOVE.W    #$1000,$E104
00002D4E                          JSR       $00003138(pc)
00002D52                          JSR       $0000149C(pc)
00002D56                          BSET      #$06,$FD1F
00002D5C                          MOVE.W    $FD1E,$00C00004
00002D64                          JSR       $000031A0(pc)
00002D68                          JSR       $000003E6(pc)
00002D6C                          JSR       $00020696
00002D72                          JSR       $00000BE2(pc)
00002D76                          JSR       $000010A0(pc)
00002D7A                          JSR       $00020AD2
00002D80                          JSR       $000014CE(pc)
00002D84                          TST.B     $E189
00002D88                          BEQ       $00002DA8
00002D8A                          TST.W     $E146
00002D8E                          BEQ       $00002D96
00002D90                          SUBQ.W    #$1,$E146
00002D94                          BEQ       $00002DA8
00002D96                          TST.W     $E142
00002D9A                          BEQ       $00002DA2
00002D9C                          SUBQ.W    #$1,$E142
00002DA0                          BEQ       $00002DA8
00002DA2                          TST.W     $E104
00002DA6                          BNE       $00002D64
00002DA8                          MOVE.W    #$0001,$E100
00002DAE                          JSR       $00020130
00002DB4                          JSR       $00001020
00002DBA                          RTS
00002DBC                          BCLR      #$06,$FD1F
00002DC2                          MOVE.W    $FD1E,$00C00004
00002DCA                          MOVE.W    #$9200,$00C00004
00002DD2                          JSR       $00022DFA
00002DD8                          MOVEQ     #$0A,D0
00002DDA                          JSR       $00028F50
00002DE0                          MOVE.L    #$08080801,D0
00002DE6                          JSR       $00022D74
00002DEC                          CMP.B     #$04,$A002
00002DF2                          BCS       $00002E0C
00002DF4                          MOVE.L    #$5A000003,$00C00004
00002DFE                          LEA       $00C00000,A1
00002E04                          MOVEQ     #$00,D1
00002E06                          JSR       $00001382
00002E0C                          LEA       $00023FBC,A0
00002E12                          MOVEQ     #$00,D0
00002E14                          MOVE.B    $FC0A,D0
00002E18                          SUBQ.W    #$1,D0
00002E1A                          ADD.W     D0,D0
00002E1C                          ADD.W     D0,D0
00002E1E                          MOVE.L    $00(A0,D0.W),A0
00002E22                          MOVE.W    #$8000,D0
00002E26                          JSR       $00023F8E
00002E2C                          MOVE.B    #$01,$A001
00002E32                          MOVE.W    #$0038,$A008
00002E38                          MOVE.W    #$0068,$A00C
00002E3E                          MOVE.B    #$00,$FC58
00002E44                          JSR       $000011D2
00002E4A                          MOVE.W    #$012C,$E104
00002E50                          JSR       $00020090
00002E56                          CMP.B     #$05,$FC0A
00002E5C                          BCS       $00002E6A
00002E5E                          MOVE.W    #$02D0,$E104
00002E64                          JSR       $000200F0
00002E6A                          JSR       $0000149C(pc)
00002E6E                          BSET      #$06,$FD1F
00002E74                          MOVE.W    $FD1E,$00C00004
00002E7C                          MOVE.B    #$00,$E182
00002E82                          MOVE.L    #$00FFE280,$E000
00002E8A                          JSR       $000005BE
00002E90                          MOVE.L    $E000,A0
00002E94                          MOVE.L    #$00000000,(A0)
00002E9A                          JSR       $000014CE(pc)
00002E9E                          BTST      #$07,$E109
00002EA4                          BNE       $00002EAC
00002EA6                          TST.W     $E104
00002EAA                          BNE       $00002E7C
00002EAC                          MOVEQ     #$00,D0
00002EAE                          MOVE.B    $FC0A,D0
00002EB2                          SUBQ.W    #$1,D0
00002EB4                          ADD.W     D0,D0
00002EB6                          MOVE.W    +$00(D7.L),D0
00002EBA                          JSR       $00020906
00002EC0                          MOVE.W    #$000A,$E100
00002EC6                          CMP.B     #$05,$FC0A
00002ECC                          BNE       $00002ED4
00002ECE                          MOVE.W    #$000C,$E100
00002ED4                          RTS
00002ED6                          BSET      D0,$03(A4,D0.W)
00002EDA                          BSET      D1,$07D0(A0)
00002EDE                          BCLR      D5,$08B8
00002EE2                          OR.B      #$1F,D6
00002EE6                          MOVE.W    $FD1E,$00C00004
00002EEE                          JSR       $00020352
00002EF4                          JSR       $00022DBC
00002EFA                          JSR       $000011EC(pc)
00002EFE                          MOVEQ     #$03,D0
00002F00                          JSR       $00022966
00002F06                          MOVEQ     #$08,D0
00002F08                          JSR       $00028F50
00002F0E                          MOVE.L    #$08080827,D0
00002F14                          JSR       $00022D74
00002F1A                          MOVE.W    #$0168,$E104
00002F20                          JSR       $000011D2
00002F26                          JSR       $00020088
00002F2C                          JSR       $0000149C(pc)
00002F30                          BSET      #$06,$FD1F
00002F36                          MOVE.W    $FD1E,$00C00004
00002F3E                          JSR       $000014B6(pc)
00002F42                          BTST      #$07,$E109
00002F48                          BNE       $00002F50
00002F4A                          TST.W     $E104
00002F4E                          BNE       $00002F3E
00002F50                          MOVE.W    #$0008,$E100
00002F56                          RTS
00002F58                          BCLR      #$06,$FD1F
00002F5E                          MOVE.W    $FD1E,$00C00004
00002F66                          JSR       $00020352
00002F6C                          JSR       $00022DBC
00002F72                          JSR       $000011EC(pc)
00002F76                          MOVEQ     #$04,D0
00002F78                          JSR       $00022966
00002F7E                          MOVEQ     #$08,D0
00002F80                          JSR       $00028F50
00002F86                          MOVE.L    #$08080827,D0
00002F8C                          JSR       $00022D74
00002F92                          MOVE.W    #$00B4,$E104
00002F98                          JSR       $000011D2
00002F9E                          JSR       $0000149C(pc)
00002FA2                          BSET      #$06,$FD1F
00002FA8                          MOVE.W    $FD1E,$00C00004
00002FB0                          JSR       $000014B6(pc)
00002FB4                          BTST      #$07,$E109
00002FBA                          BNE       $00002FC2
00002FBC                          TST.W     $E104
00002FC0                          BNE       $00002FB0
00002FC2                          MOVE.W    #$0002,$E100
00002FC8                          TST.B     $FC00
00002FCC                          BNE       $00002FD4
00002FCE                          MOVE.W    #$0006,$E100
00002FD4                          RTS
00002FD6                          TST.B     $FD1A
00002FDA                          BEQ       $00003050(pc)
00002FDE                          BCLR      #$06,$FD1F
00002FE4                          MOVE.W    $FD1E,$00C00004
00002FEC                          JSR       $00020352
00002FF2                          JSR       $00022DBC
00002FF8                          JSR       $000011EC(pc)
00002FFC                          MOVEQ     #$05,D0
00002FFE                          JSR       $00022966
00003004                          MOVEQ     #$08,D0
00003006                          JSR       $00028F50
0000300C                          MOVE.L    #$08080827,D0
00003012                          JSR       $00022D74
00003018                          MOVE.W    #$0258,$E104
0000301E                          JSR       $000011D2
00003024                          JSR       $00020098
0000302A                          JSR       $0000149C(pc)
0000302E                          BSET      #$06,$FD1F
00003034                          MOVE.W    $FD1E,$00C00004
0000303C                          BSR       $000030B6
0000303E                          JSR       $000014B6(pc)
00003042                          BTST      #$07,$E109
00003048                          BNE       $00003058
0000304A                          TST.W     $E104
0000304E                          BNE       $0000303C
00003050                          MOVE.W    #$0001,$E100
00003056                          RTS
00003058                          MOVE.B    $FC09,D0
0000305C                          MOVE.W    $FC52,D1
00003060                          MOVE.W    $FC56,D2
00003064                          MOVE.W    $FC54,D3
00003068                          MOVEM.L   D0-D3,-(A7)
0000306C                          JSR       $000234C6
00003072                          MOVEM.L   (A7)+,D0-D3
00003076                          MOVE.B    D0,$FC09
0000307A                          MOVE.W    D1,$FC52
0000307E                          MOVE.W    D2,$FC56
00003082                          MOVE.W    D3,$FC54
00003086                          JSR       $00020A0E
0000308C                          JSR       $00020A62
00003092                          JSR       $000011F6
00003098                          MOVE.W    #$0000,$E126
0000309E                          MOVE.W    #$0000,$FD06
000030A4                          MOVE.W    #$07D0,$FD0A
000030AA                          MOVE.W    #$0002,$E100
000030B0                          SUBQ.B    #$1,$FD1A
000030B4                          RTS
000030B6                          MOVEQ     #$00,D0
000030B8                          MOVE.W    $E104,D0
000030BC                          DIVU      #$003C,D0
000030C0                          LSL.W     #3,D0
000030C2                          MOVE.L    #$66260003,$00C00004
000030CC                          MOVE.L    +$00(D5.L),$00C00000
000030D4                          MOVE.L    #$66A60003,$00C00004
000030DE                          MOVE.L    +$00(D6.W),$00C00000
000030E6                          RTS
000030E8                          OR.B      (A3),D0
000030EA                          OR.B      (A4),D0
000030EC                          OR.B      $80(A1,D1.L),D0
000030F0                          OR.B      D1,D0
000030F2                          OR.B      D2,D0
000030F4                          OR.B      (A7)+,D0
000030F6                          OR.B      -(A0),D0
000030F8                          OR.B      D3,D0
000030FA                          OR.B      D4,D0
000030FC                          OR.B      -(A1),D0
000030FE                          OR.B      -(A2),D0
00003100                          OR.B      D5,D0
00003102                          OR.B      D6,D0
00003104                          OR.B      -(A3),D0
00003106                          OR.B      -(A4),D0
00003108                          OR.B      D7,D0
0000310A                          OR.B      A0,D0
0000310C                          OR.B      -(A5),D0
0000310E                          OR.B      -(A6),D0
00003110                          OR.B      A1,D0
00003112                          OR.B      A2,D0
00003114                          OR.B      -(A7),D0
00003116                          OR.B      -$7FF5(A0),D0
0000311A                          OR.B      A4,D0
0000311C                          OR.B      -$7FD6(A1),D0
00003120                          OR.B      A5,D0
00003122                          OR.B      A6,D0
00003124                          OR.B      -$7FD4(A3),D0
00003128                          OR.B      A7,D0
0000312A                          OR.B      (A0),D0
0000312C                          OR.B      -$7FD2(A5),D0
00003130                          OR.B      (A1),D0
00003132                          OR.B      (A2),D0
00003134                          OR.B      -$7FD0(A7),D0
00003138                          MOVE.B    $FDF8,D0
0000313C                          CMP.W     #$0004,$E100
00003142                          BNE       $00003146
00003144                          MOVEQ     #$0F,D0
00003146                          AND.W     #$0007,D0
0000314A                          MULU      #$0006,D0
0000314E                          LEA       $00003170(pc),A1
00003152                          LEA       $00(A1,D0.W),A1
00003156                          MOVE.B    $0001(A1),$FC09
0000315C                          MOVE.W    $0002(A1),D0
00003160                          MOVE.W    D0,$FC52
00003164                          MOVE.W    D0,$FC56
00003168                          MOVE.W    $0004(A1),$FC54
0000316E                          RTS
00003170                          OR.B      #$10,D6
00003174                          BCLR      D1,D4
00003176                          OR.B      #$18,D6
0000317A                          AND.W     #$0003,(A0)+
0000317E                          .B        #$84,(A0)
00003182                          OR.B      #$18,D0
00003186                          AND.W     #$0006,(A0)+
0000318A                          .B        #$58,(A0)
0000318E                          OR.B      #$8C,D6
00003192                          BCLR      D1,D4
00003194                          OR.B      #$10,A1
00003198                          AND.W     #$000F,(A0)+
0000319C                          EOR.L     #$03840C78,A4
000031A2                          OR.B      #$00,D4
000031A6                          BEQ       $000031DC
000031A8                          CMP.W     #$0000,$FDE2
000031AE                          BEQ       $000031D6
000031B0                          TST.W     $FC56
000031B4                          BNE       $000031D0
000031B6                          ADDQ.B    #$1,$FC09
000031BA                          CMP.B     #$0F,$FC09
000031C0                          BLE       $000031C8
000031C2                          MOVE.B    #$0F,$FC09
000031C8                          MOVE.W    $FC54,$FC56
000031CE                          RTS
000031D0                          SUBQ.W    #$1,$FC56
000031D4                          RTS
000031D6                          MOVE.B    #$00,$FC09
000031DC                          RTS
000031DE                          BTST      #$00,$E181
000031E4                          BEQ       $000031F2
000031E6                          JSR       $00020AD2
000031EC                          JSR       $000014C6(pc)
000031F0                          BRA       $000031DE
000031F2                          RTS
000031F4                          BCLR      #$06,$FD1F
000031FA                          MOVE.W    $FD1E,$00C00004
00003202                          JSR       $00022DBC
00003208                          JSR       $00020352
0000320E                          MOVE.L    #$35353534,D0
00003214                          JSR       $00022D74
0000321A                          MOVEQ     #$0A,D0
0000321C                          JSR       $00028F50
00003222                          JSR       $0000149C(pc)
00003226                          BSET      #$06,$FD1F
0000322C                          MOVE.W    $FD1E,$00C00004
00003234                          LEA       $00024062,A0
0000323A                          MOVE.W    #$8000,D0
0000323E                          JSR       $00023F8E
00003244                          JSR       $0000338C(pc)
00003248                          JSR       $0000334C(pc)
0000324C                          MOVE.W    $FDE2,D0
00003250                          MOVE.B    +$00(D1.W),$FDF8
00003256                          MOVE.W    $FDE4,D0
0000325A                          MOVE.B    +$00(D6.L),$FD18
00003260                          JSR       $000014B6(pc)
00003264                          BTST      #$07,$E109
0000326A                          BEQ       $00003234
0000326C                          MOVE.W    #$0001,$E100
00003272                          RTS
00003274                          BTST      D1,D0
00003276                          BTST      D3,D0
00003278                          BTST      D1,D4
0000327A                          BTST      D2,D0
0000327C                          OR.B      D2,D0
0000327E                          OR.B      D3,D0
00003280                          OR.B      D4,D0
00003282                          OR.B      D5,D0
00003284                          OR.B      D6,D0
00003286                          OR.B      D7,D0
00003288                          OR.B      A0,D0
0000328A                          OR.B      A1,D0
0000328C                          OR.B      A2,D0
0000328E                          OR.B      A3,D0
00003290                          LEA       $FD14,A1
00003294                          MOVEQ     #$00,D6
00003296                          MOVEQ     #$2B,D7
00003298                          BTST      #$05,$E109
0000329E                          BEQ       $000032A6
000032A0                          BSR       $000012EA(pc)
000032A4                          BRA       $000032B2
000032A6                          BTST      #$06,$E109
000032AC                          BEQ       $000032B2
000032AE                          BSR       $000012DC(pc)
000032B2                          MOVE.L    #$61380003,$00C00004
000032BC                          MOVE.W    $FD14,D0
000032C0                          ADD.W     D0,D0
000032C2                          MOVEQ     #$00,D1
000032C4                          MOVE.B    +$00(D7.W),D1
000032C8                          MOVE.B    D1,$FC0A
000032CC                          ADD.W     D1,D1
000032CE                          MOVE.W    +$10(D6.W),$00C00000
000032D6                          MOVE.W    #$8028,$00C00000
000032DE                          MOVEQ     #$00,D1
000032E0                          MOVE.B    +$00(A1.L),D1
000032E4                          MOVE.B    D1,$FC26
000032E8                          ADD.W     D1,D1
000032EA                          MOVE.W    +$10(D0.L),$00C00000
000032F2                          RTS
000032F4                          OR.B      #$01,D0
000032F8                          OR.B      #$03,D2
000032FC                          OR.B      #$05,D4
00003300                          OR.B      #$07,D6
00003304                          OR.B      #$00,A0
00003308                          BTST      D0,D1
0000330A                          BTST      D0,D2
0000330C                          BTST      D0,D3
0000330E                          BTST      D0,D4
00003310                          BTST      D0,D5
00003312                          BTST      D0,D6
00003314                          BTST      D0,D7
00003316                          AND.B     #$01,D0
0000331A                          AND.B     #$03,D2
0000331E                          AND.B     #$05,D4
00003322                          AND.B     #$07,D6
00003326                          AND.B     #$00,A0
0000332A                          BTST      D1,D1
0000332C                          BTST      D1,D2
0000332E                          BTST      D1,D3
00003330                          BTST      D1,D4
00003332                          BTST      D1,D5
00003334                          BTST      D1,D6
00003336                          SUB.B     #$01,D0
0000333A                          SUB.B     #$03,D2
0000333E                          SUB.B     #$05,D4
00003342                          SUB.B     #$07,D6
00003346                          SUB.B     #$09,A0
0000334A                          SUB.B     #$F9,A2
0000334E                          OR.B      #$AC,D2
00003352                          MOVE.W    $FDE0,D0
00003356                          BSR       $0000337A
00003358                          LEA       $000240CA,A0
0000335E                          MOVE.W    $FDE2,D0
00003362                          BSR       $0000337A
00003364                          LEA       $000240E8,A0
0000336A                          MOVE.W    $FDE4,D0
0000336E                          BSR       $0000337A
00003370                          LEA       $00024106,A0
00003376                          MOVE.W    $FDE6,D0
0000337A                          MULU      #$000A,D0
0000337E                          LEA       $00(A0,D0.W),A0
00003382                          MOVE.W    #$A000,D0
00003386                          JMP       $00023F8E
0000338C                          LEA       $FDE0,A1
00003390                          MOVEQ     #$00,D6
00003392                          MOVEQ     #$02,D7
00003394                          BTST      #$02,$E109
0000339A                          BEQ       $000033A2
0000339C                          JSR       $000012EA(pc)
000033A0                          BRA       $000033AE
000033A2                          BTST      #$03,$E109
000033A8                          BEQ       $000033AE
000033AA                          JSR       $000012DC(pc)
000033AE                          MOVE.W    $FDE0,D0
000033B2                          ADD.W     D0,D0
000033B4                          MOVE.W    +$00(D3.W),D0
000033B8                          JMP       +$00(D1.W)
000033BC                          OR.B      #$2A,D6
000033C0                          OR.W      #$43F8,A6
000033C4                          Invalid
000033C6                          MOVEQ     #$00,D6
000033C8                          MOVEQ     #$02,D7
000033CA                          BTST      #$00,$E109
000033D0                          BEQ       $000033D8
000033D2                          JSR       $000012EA(pc)
000033D6                          BRA       $000033E4
000033D8                          BTST      #$01,$E109
000033DE                          BEQ       $000033E4
000033E0                          JSR       $000012DC(pc)
000033E4                          RTS
000033E6                          LEA       $FDE4,A1
000033EA                          MOVEQ     #$00,D6
000033EC                          MOVEQ     #$02,D7
000033EE                          BTST      #$00,$E109
000033F4                          BEQ       $000033FC
000033F6                          JSR       $000012EA(pc)
000033FA                          BRA       $00003408
000033FC                          BTST      #$01,$E109
00003402                          BEQ       $00003408
00003404                          JSR       $000012DC(pc)
00003408                          RTS
0000340A                          LEA       $FDE6,A1
0000340E                          MOVEQ     #$00,D6
00003410                          MOVEQ     #$41,D7
00003412                          BTST      #$00,$E109
00003418                          BEQ       $00003426
0000341A                          JSR       $000012EA(pc)
0000341E                          JSR       $00020138
00003424                          BRA       $00003438
00003426                          BTST      #$01,$E109
0000342C                          BEQ       $00003438
0000342E                          JSR       $000012DC(pc)
00003432                          JSR       $00020138
00003438                          MOVE.B    $E109,D0
0000343C                          AND.B     #$70,D0
00003440                          BEQ       $0000344C
00003442                          MOVE.W    $FDE6,D0
00003446                          MOVE.B    +$00(D3.W),$9C0A
0000344C                          RTS
0000344E                          (BBBF   $000033D2
00003450                          (BBLS   $000033D6
00003452                          (BBCS   $000033DA
00003454                          DIVU      #$88,D3
00003456                          DIVU      #$8A,D4
00003458                          SBCD      Invalid
0000345A                          SUB.L     (A7),D3
0000345C                          OR.L      A5,D6
0000345E                          OR.L      A7,D7
00003460                          SUB.L     (A1),D0
00003462                          SUB.L     (A4)+,D5
00003464                          SUB.L     (A5)+,D4
00003466                          (CCC     #$4,(A6)+
00003468                          (CCC     #$1,(A7)+
0000346A                          (CCC     #$5,(A2)
0000346C                          SUB.L     -(A0),D2
0000346E                          Invalid
00003470                          Invalid
00003472                          Invalid
00003474                          Invalid
00003476                          CMP.L     $B4(A3,A2.L),D1
0000347A                          CMP.L     $BABBBDBE,D3
00003480                          MULU      D1,D0
00003482                          MULS      D4,D1
00003484                          MULU      D7,D3
00003486                          MULU      A5,D5
00003488                          MULU      A7,D7
0000348A                          ADD.L     (A2),A0
0000348C                          ADD.L     (A4),A1
0000348E                          ADD.L     (A6),A2
00003490                          BCLR      #$06,$FD1F
00003496                          MOVE.W    $FD1E,$00C00004
0000349E                          JSR       $00020352
000034A4                          JSR       $00022DBC
000034AA                          MOVE.L    #$00000E0B,D0
000034B0                          JSR       $00028F50
000034B6                          MOVE.B    #$00,$E194
000034BC                          JSR       $000011D2
000034C2                          JSR       $0000149C(pc)
000034C6                          LEA       $0000361E(pc),A1
000034CA                          MOVEQ     #$00,D0
000034CC                          MOVE.B    $FC0A,D0
000034D0                          MOVE.B    $00(A1,D0.W),$9C0A
000034D6                          BCLR      #$06,$FD1F
000034DC                          MOVE.W    $FD1E,$00C00004
000034E4                          JSR       $00022E30
000034EA                          LEA       $8000,A1
000034EE                          MOVEQ     #$00,D1
000034F0                          JSR       $00001354(pc)
000034F4                          MOVE.W    #$012C,$E104
000034FA                          MOVEQ     #$00,D1
000034FC                          MOVE.B    $FC0A,D1
00003500                          MULU      #$05,D1
00003504                          ADD.B     $E194,D1
00003508                          ADDQ.B    #$1,$E194
0000350C                          MOVE.B    +$10(D7.W),D1
00003510                          BMI       $00003548
00003512                          BSR       $00003576
00003514                          JSR       $00003706(pc)
00003518                          JSR       $000014AE(pc)
0000351C                          BSET      #$06,$FD1F
00003522                          MOVE.W    $FD1E,$00C00004
0000352A                          JSR       $00003706(pc)
0000352E                          JSR       $000014AE(pc)
00003532                          BTST      #$07,$E109
00003538                          BNE       $00003548
0000353A                          TST.W     $E104
0000353E                          BNE       $0000352A
00003540                          TST.B     $E194
00003544                          BPL       $000034D6(pc)
00003548                          MOVE.W    #$0003,$E100
0000354E                          JSR       $00020130
00003554                          JSR       $00001020
0000355A                          RTS
0000355C                          OR.B      #$FF,D1
00003560                          Invalid
00003562                          BTST      D0,D2
00003564                          Invalid
00003566                          BTST      D1,D4
00003568                          BSET      D2,
0000356A                          Invalid
0000356C                          BSET      D3,
0000356E                          Invalid
00003570                          BTST      #$0B,A1
00003574                          Invalid
00003576                          MULU      #$08,D1
0000357A                          LEA       $000035C2(pc),A1
0000357E                          ADD.W     D1,A1
00003580                          JSR       $00003624(pc)
00003584                          MOVEQ     #$00,D0
00003586                          MOVE.W    +$10(D3.L),D0
0000358A                          JSR       $00022D74
00003590                          MOVE.L    D1,-(A7)
00003592                          MOVEQ     #$00,D0
00003594                          MOVE.B    +$10(D5.W),D0
00003598                          JSR       $00022966
0000359E                          MOVE.L    (A7)+,D1
000035A0                          MOVEQ     #$00,D0
000035A2                          MOVE.B    +$10(A6.L),D0
000035A6                          ADD.W     D0,D0
000035A8                          ADD.W     D0,D0
000035AA                          LEA       $000243E4,A1
000035B0                          MOVE.L    $00(A1,D0.W),A0
000035B4                          MOVE.W    #$8200,D0
000035B8                          JMP       $00023F8E
000035BE                          NEGX.B    $06(A4,D0.W)
000035C2                          BCLR      D0,D2
000035C4                          OR.B      #$34,D0
000035C8                          BTST      D6,D1
000035CA                          EXG       #$1,D4
000035CC                          OR.B      #$34,D0
000035D0                          BTST      D3,D2
000035D2                          SBCD      Invalid
000035D4                          OR.B      #$34,D0
000035D8                          BTST      #$00,D3
000035DC                          OR.B      #$34,D0
000035E0                          BTST      D4,D4
000035E2                          MOVEP.W   D3,$49CA(A0)
000035E6                          NEG.B     $0A(A4,A2.W)
000035EA                          MOVEP.L   $0000(A4),D5
000035EE                          Invalid
000035F0                          BTST      D6,D6
000035F2                          SBCD      Invalid
000035F4                          OR.B      #$34,D0
000035F8                          .B        #$8F,D7
000035FC                          MOVE.B    D0,D0
000035FE                          Invalid
00003600                          MOVEP.W   -$2E6E(A0),D5
00003604                          OR.B      #$34,D0
00003608                          CMP.B     #$54,A1
0000360C                          OR.B      #$34,D0
00003610                          MOVE.W    A2,-(A1)
00003612                          MOVE.B    D0,-(A2)
00003614                          OR.B      #$34,D0
00003618                          MOVEP.W   -$29E9(A3),D6
0000361C                          SUB.B     D0,D4
0000361E                          SUB.L     (A0)+,D4
00003620                          (CCC     #$4,(A5)+
00003622                          SUB.B     D0,D7
00003624                          LEA       $8000,A0
00003628                          BSR       $0000363A
0000362A                          LEA       $8010,A0
0000362E                          BSR       $0000363A
00003630                          LEA       $8020,A0
00003634                          BSR       $0000363A
00003636                          LEA       $8030,A0
0000363A                          MOVE.B    (A1)+,D0
0000363C                          BEQ       $00003664
0000363E                          MOVE.B    D0,D2
00003640                          AND.W     #$003F,D0
00003644                          ADD.W     D0,D0
00003646                          ADD.W     D0,D0
00003648                          MOVE.W    +$00(D4.L),$0004(A0)
0000364E                          MOVE.W    +$00(D2.L),$0008(A0)
00003654                          ROL.B     #2,D2
00003656                          AND.W     #$0003,D2
0000365A                          ADD.W     D2,D2
0000365C                          MOVE.W    +$20(D4.W),D2
00003660                          JMP       +$20(D2.W)
00003664                          RTS
00003666                          OR.B      #$16,A0
0000366A                          OR.B      #$32,-(A4)
0000366E                          MOVE.W    #$0001,$0000(A0)
00003674                          MOVE.W    #$0050,$0002(A0)
0000367A                          RTS
0000367C                          MOVE.W    #$0002,$0000(A0)
00003682                          MOVE.W    #$0050,$0002(A0)
00003688                          RTS
0000368A                          MOVE.W    #$0003,$0000(A0)
00003690                          MOVE.W    #$0040,$0002(A0)
00003696                          RTS
00003698                          MOVE.W    #$0004,$0000(A0)
0000369E                          MOVE.W    #$0040,$0002(A0)
000036A4                          RTS
000036A6                          OR.L      #$01180098,(A0)+
000036AC                          BTST      D0,$00(A0,D4.L)
000036B0                          MOVEP.W   $0098(A0),D0
000036B4                          BTST      D0,-(A0)
000036B6                          Invalid
000036B8                          MOVEP.W   $00D0(A0),D0
000036BC                          BTST      D0,(A0)
000036BE                          OR.L      #$009800A0,-(A0)
000036C4                          OR.L      #$004000C8,$00(A0,D0.W)
000036CC                          Invalid
000036CE                          OR.L      #$01180068,(A0)+
000036D4                          BTST      D0,$00(A0,D0.L)
000036D8                          MOVEP.W   $0098(A0),D0
000036DC                          BTST      D0,(A0)+
000036DE                          OR.L      #$01300098,(A0)+
000036E4                          MOVEP.L   $00A8(A0),D0
000036E8                          BTST      D0,(A0)+
000036EA                          OR.L      #$013000A8,$0108(A0)
000036F2                          OR.W      #$0120,(A0)
000036F6                          OR.L      #$00B80098,$01(A0,D0.L)
000036FE                          OR.L      #$01280098,(A0)+
00003704                          BCHG      D0,D0
00003706                          MOVE.B    #$00,$E182
0000370C                          MOVE.L    #$00FFE280,$E000
00003714                          LEA       $8000,A0
00003718                          BSR       $00003738
0000371A                          LEA       $8010,A0
0000371E                          BSR       $00003738
00003720                          LEA       $8020,A0
00003724                          BSR       $00003738
00003726                          LEA       $8030,A0
0000372A                          BSR       $00003738
0000372C                          MOVE.L    $E000,A0
00003730                          MOVE.L    #$00000000,(A0)
00003736                          RTS
00003738                          MOVE.W    $0000(A0),D0
0000373C                          BEQ       $00003748
0000373E                          ADD.W     D0,D0
00003740                          MOVE.W    +$00(D3.W),D0
00003744                          JMP       +$00(D2.W)
00003748                          RTS
0000374A                          OR.B      #$14,A0
0000374E                          OR.B      #$30,-(A0)
00003752                          ADDQ.W    #$4,$0004(A0)
00003756                          SUBQ.W    #$1,$0002(A0)
0000375A                          BNE       $00003790
0000375C                          BRA       $00003788
0000375E                          SUBQ.W    #$4,$0004(A0)
00003762                          SUBQ.W    #$1,$0002(A0)
00003766                          BNE       $00003790
00003768                          BRA       $00003788
0000376A                          ADD.L     #$00002000,$0008(A0)
00003772                          SUBQ.W    #$1,$0002(A0)
00003776                          BNE       $00003790
00003778                          BRA       $00003788
0000377A                          SUB.L     #$00002000,$0008(A0)
00003782                          SUBQ.W    #$1,$0002(A0)
00003786                          BNE       $00003790
00003788                          MOVEQ     #$00,D1
0000378A                          MOVE.L    A0,A1
0000378C                          JMP       $0000136C(pc)
00003790                          LEA       $0000380C(pc),A1
00003794                          MOVE.W    $0004(A0),D4
00003798                          MOVE.W    $0008(A0),D3
0000379C                          MOVE.W    #$8000,D5
000037A0                          MOVEQ     #$00,D7
000037A2                          MOVE.L    $E000,A2
000037A6                          MOVE.B    (A1)+,D7
000037A8                          BMI       $000037FA
000037AA                          TST.B     (A1)+
000037AC                          CMP.B     #$50,$E182
000037B2                          BCC       $000037FA
000037B4                          MOVE.B    (A1)+,D0
000037B6                          EXT.W     D0
000037B8                          ADD.W     D3,D0
000037BA                          CMP.W     #$0060,D0
000037BE                          BLE       $00003800
000037C0                          CMP.W     #$0160,D0
000037C4                          BGE       $00003800
000037C6                          MOVE.W    D0,(A2)+
000037C8                          MOVE.B    (A1)+,(A2)+
000037CA                          ADDQ.B    #$1,$E182
000037CE                          MOVE.B    $E182,(A2)+
000037D2                          MOVE.B    (A1)+,D0
000037D4                          LSL.W     #8,D0
000037D6                          MOVE.B    (A1)+,D0
000037D8                          ADD.W     D5,D0
000037DA                          MOVE.W    D0,(A2)+
000037DC                          MOVE.W    (A1)+,D0
000037DE                          ADD.W     D4,D0
000037E0                          CMP.W     #$0060,D0
000037E4                          BLE       $00003804
000037E6                          CMP.W     #$01C0,D0
000037EA                          BGE       $00003804
000037EC                          AND.W     #$01FF,D0
000037F0                          BNE       $000037F4
000037F2                          ADDQ.W    #$1,D0
000037F4                          MOVE.W    D0,(A2)+
000037F6                          DBRA      D7,$000037AC
000037FA                          MOVE.L    A2,$E000
000037FE                          RTS
00003800                          ADDQ.W    #$4,A1
00003802                          BRA       $000037F6
00003804                          SUBQ.W    #$6,A2
00003806                          SUBQ.B    #$1,$E182
0000380A                          BRA       $000037F6
0000380C                          MOVE.B    D0,-(A1)
0000380E                          OR.B      #$40,D4
00003812                          OR.B      #$04,D0
00003816                          AND.W     #$0010,D0
0000381A                          OR.B      #$40,D4
0000381E                          OR.B      #$04,-(A0)
00003822                          AND.W     #$0030,D0
00003826                          OR.B      #$40,D4
0000382A                          OR.W      #$0004,D0
0000382E                          AND.W     #$0050,D0
00003832                          OR.B      #$40,D4
00003836                          OR.W      #$0004,-(A0)
0000383A                          AND.W     #$0070,D0
0000383E                          OR.B      #$40,D4
00003842                          OR.L      #$00040240,D0
00003848                          OR.L      #$00040240,(A0)
0000384E                          OR.L      #$00040240,-(A0)
00003854                          OR.L      #$00040240,$00(A0,D0.W)
0000385C                          OR.B      #$40,D4
00003860                          Invalid
00003862                          OR.B      #$40,D4
00003866                          Invalid
00003868                          OR.B      #$40,D4
0000386C                          Invalid
0000386E                          OR.B      #$40,D4
00003872                          BTST      D0,D0
00003874                          OR.B      #$40,D4
00003878                          BTST      D0,(A0)
0000387A                          OR.B      #$40,D4
0000387E                          BTST      D0,-(A0)
00003880                          OR.B      #$40,D4
00003884                          BTST      D0,$41(A0,D4.L)
00003888                          Invalid
0000388A                          MOVE.W    #$0003,D7
0000388E                          SWAP      D7
00003890                          TST.B     $0000(A0)
00003894                          BEQ       $0000389C
00003896                          MOVE.L    $001A(A0),A6
0000389A                          BSR       $000038A8
0000389C                          LEA       $0050(A0),A0
000038A0                          SWAP      D7
000038A2                          DBRA      D7,$0000388E
000038A6                          RTS
000038A8                          TST.B     $0002(A0)
000038AC                          BNE       $000038F2(pc)
000038B0                          MOVEQ     #$08,D1
000038B2                          TST.B     $A05B
000038B6                          BNE       $000038BA
000038B8                          NEG.W     D1
000038BA                          ADD.W     Invalid
000038BC                          OR.B      #$FA,A4
000038C0                          Invalid
000038C2                          MOVE.L    A6,$001A(A0)
000038C6                          MOVE.B    #$02,$0002(A0)
000038CC                          JSR       $00003ADE(pc)
000038D0                          JSR       $00003914(pc)
000038D4                          JSR       $00020DB0
000038DA                          AND.W     #$0001,D0
000038DE                          CMP.B     #$14,$0041(A0)
000038E4                          BGT       $000038E8
000038E6                          ADDQ.B    #$2,D0
000038E8                          LEA       $00003BB8(pc),A4
000038EC                          JMP       $000287D6
000038F2                          SUBQ.B    #$1,$0041(A0)
000038F6                          BNE       $000038FE
000038F8                          JMP       $00028B28
000038FE                          LEA       $00003B12(pc),A1
00003902                          TST.B     $0003(A0)
00003906                          BEQ       $0000390C
00003908                          LEA       $00003B66(pc),A1
0000390C                          JSR       (A1)
0000390E                          JMP       $00028C3A
00003914                          MOVEQ     #$00,D0
00003916                          MOVE.B    $0022(A0),D0
0000391A                          TST.B     $A05B
0000391E                          BEQ       $00003924
00003920                          MOVE.B    $0023(A0),D0
00003924                          LSL.W     #3,D0
00003926                          MOVE.W    D0,D1
00003928                          ADD.W     D0,D0
0000392A                          ADD.W     D1,D0
0000392C                          LEA       +$00(D0.L),A1
00003930                          CMP.B     #$14,$0041(A0)
00003936                          BGT       $0000393E
00003938                          ADD.L     #$0000000C,A1
0000393E                          TST.B     $0030(A0)
00003942                          BEQ       $0000394A
00003944                          ADD.L     #$000000C0,A1
0000394A                          MOVE.L    (A1)+,$0044(A0)
0000394E                          MOVE.L    (A1)+,$0048(A0)
00003952                          MOVE.W    (A1)+,$003C(A0)
00003956                          MOVE.B    $0001(A1),$0003(A0)
0000395C                          RTS
0000395E                          Invalid
00003960                          MOVE.B    (A4),-(A0)
00003962                          Invalid
00003964                          Invalid
00003966                          OR.B      #$00,-(A1)
0000396A                          Invalid
0000396C                          MOVE.L    -(A7),D1
0000396E                          Invalid
00003970                          Invalid
00003972                          OR.W      #$0000,D2
00003976                          Invalid
00003978                          MOVE.B    (A4),-(A0)
0000397A                          Invalid
0000397C                          Invalid
0000397E                          OR.B      #$00,-(A1)
00003982                          Invalid
00003984                          MOVE.L    -(A7),D1
00003986                          Invalid
00003988                          Invalid
0000398A                          OR.W      #$0000,D2
0000398E                          Invalid
00003990                          CMP.L     D1,A5
00003992                          Invalid
00003994                          Invalid
00003996                          Invalid
00003998                          OR.B      #$FF,D2
0000399C                          MOVEQ     #$7A,D3
0000399E                          Invalid
000039A0                          Invalid
000039A2                          Invalid
000039A4                          OR.B      #$FF,D2
000039A8                          OR.B      D0,D0
000039AA                          Invalid
000039AC                          Invalid
000039AE                          Invalid
000039B0                          OR.B      #$FF,D2
000039B4                          OR.B      #$FF,D0
000039B8                          Invalid
000039BA                          Invalid
000039BC                          OR.B      #$FF,D2
000039C0                          SUBQ.W    #$2,(A2)+
000039C2                          Invalid
000039C4                          Invalid
000039C6                          Invalid
000039C8                          OR.B      #$FE,D2
000039CC                          Invalid
000039CE                          Invalid
000039D0                          Invalid
000039D2                          Invalid
000039D4                          OR.B      #$FF,D2
000039D8                          ADD.L     -(A1),A6
000039DA                          Invalid
000039DC                          Invalid
000039DE                          OR.W      #$0000,-(A6)
000039E2                          Invalid
000039E4                          CMP.L     D1,A5
000039E6                          Invalid
000039E8                          Invalid
000039EA                          Invalid
000039EC                          OR.B      #$FF,D0
000039F0                          (CCC     #$4,(A2)+
000039F2                          Invalid
000039F4                          Invalid
000039F6                          OR.W      #$0000,(A4)+
000039FA                          Invalid
000039FC                          MOVE.W    $FF(A4,A7.L),-(A1)
00003A00                          Invalid
00003A02                          OR.L      #$0000FFFF,$555A
00003A0A                          Invalid
00003A0C                          Invalid
00003A0E                          OR.W      #$0000,A4
00003A12                          Invalid
00003A14                          Invalid
00003A16                          Invalid
00003A18                          Invalid
00003A1A                          OR.L      #$00000000,(A1)+
00003A20                          CMP       $0000(A4)
00003A24                          BSET      D1,$00(A7,A0.W)
00003A28                          OR.B      #$01,D0
00003A2C                          ADD.L     (A1)+,A6
00003A2E                          OR.B      #$EC,D0
00003A32                          OR.W      #$0000,D2
00003A36                          OR.B      #$EC,D0
00003A3A                          OR.B      #$F7,D0
00003A3E                          OR.B      #$00,-(A1)
00003A42                          OR.B      #$D9,D1
00003A46                          OR.B      #$EC,D0
00003A4A                          OR.W      #$0000,D2
00003A4E                          OR.B      #$A6,D0
00003A52                          OR.B      #$D7,D0
00003A56                          OR.W      #$0000,A4
00003A5A                          OR.B      #$53,D1
00003A5E                          OR.B      #$5D,D0
00003A62                          OR.L      #$00000000,(A1)+
00003A68                          BNE       $00003AD0
00003A6A                          OR.B      #$B0,D0
00003A6E                          OR.W      #$0000,(A4)+
00003A72                          OR.B      #$CC,D0
00003A76                          OR.B      #$CF,D0
00003A7A                          OR.L      #$00000000,$221F
00003A82                          OR.B      #$90,D0
00003A86                          OR.W      #$0000,-(A6)
00003A8A                          OR.B      #$3F,D0
00003A8E                          OR.B      #$40,D0
00003A92                          Invalid
00003A94                          OR.B      #$FF,D0
00003A98                          SUBQ.W    #$2,(A2)+
00003A9A                          Invalid
00003A9C                          Invalid
00003A9E                          OR.W      #$0002,D2
00003AA2                          Invalid
00003AA4                          Invalid
00003AA6                          Invalid
00003AA8                          Invalid
00003AAA                          OR.L      #$0002FFFF,D5
00003AB0                          OR.B      D0,D0
00003AB2                          Invalid
00003AB4                          Invalid
00003AB6                          OR.W      #$0002,-(A1)
00003ABA                          Invalid
00003ABC                          OR.B      #$FF,D0
00003AC0                          Invalid
00003AC2                          Invalid
00003AC4                          OR.B      #$FF,D2
00003AC8                          CMP.L     D1,A5
00003ACA                          Invalid
00003ACC                          Invalid
00003ACE                          OR.W      #$0002,-$0001(A6)
00003AD4                          MOVEQ     #$7A,D3
00003AD6                          Invalid
00003AD8                          Invalid
00003ADA                          Invalid
00003ADC                          OR.B      #$7C,D2
00003AE0                          OR.B      #$41,A7
00003AE4                          MOVEQ     #$00,D0
00003AE6                          MOVE.B    $0006(A0),D0
00003AEA                          MOVE.B    +$00(D0.W),D0
00003AEE                          MOVE.B    D0,$0006(A0)
00003AF2                          BPL       $00003B0A
00003AF4                          MOVE.B    #$1E,$0041(A0)
00003AFA                          MOVE.B    $F09F,D0
00003AFE                          AND.W     #$0003,D0
00003B02                          BNE       $00003B0A
00003B04                          MOVE.B    #$00,$0006(A0)
00003B0A                          RTS
00003B0C                          Invalid
00003B0E                          BTST      D0,D0
00003B10                          Invalid
00003B12                          BSR       $00003B2E
00003B14                          MOVE.W    $003C(A0),D0
00003B18                          SWAP      D0
00003B1A                          ASR.L     #8,D0
00003B1C                          TST.B     $A05B
00003B20                          BEQ       $00003B24
00003B22                          NEG.L     D0
00003B24                          (CCC     #$0,$000C(A0)
00003B28                          JMP       $00028804
00003B2E                          MOVE.L    $0044(A0),D0
00003B32                          ADD.L     Invalid
00003B34                          OR.B      #$68,A0
00003B38                          OR.W      #$6B14,A0
00003B3C                          MOVE.L    $0048(A0),D0
00003B40                          (CCC     #$0,$0044(A0)
00003B44                          BPL       $00003B4E
00003B46                          MOVE.L    #$00000000,$0044(A0)
00003B4E                          RTS
00003B50                          MOVE.L    $0048(A0),D0
00003B54                          (CCC     #$0,$0044(A0)
00003B58                          BMI       $00003B62
00003B5A                          MOVE.L    #$00000000,$0044(A0)
00003B62                          RTS
00003B64                          RTS
00003B66                          BSR       $00003B7A
00003B68                          MOVE.W    $003C(A0),D0
00003B6C                          SWAP      D0
00003B6E                          ASR.L     #8,D0
00003B70                          ADD.L     Invalid
00003B72                          OR.B      #$F9,A0
00003B76                          OR.B      #$04,D2
00003B7A                          MOVE.L    $0044(A0),D0
00003B7E                          TST.B     $A05B
00003B82                          BEQ       $00003B86
00003B84                          NEG.L     D0
00003B86                          (CCC     #$0,$000C(A0)
00003B8A                          TST.W     $0048(A0)
00003B8E                          BMI       $00003B50
00003B90                          MOVE.L    $0048(A0),D0
00003B94                          (CCC     #$0,$0044(A0)
00003B98                          BPL       $00003BA2
00003B9A                          MOVE.L    #$00000000,$0044(A0)
00003BA2                          RTS
00003BA4                          OR.B      #$01,D0
00003BA8                          OR.B      #$01,D0
00003BAC                          OR.B      #$01,D0
00003BB0                          OR.B      #$01,D0
00003BB4                          OR.B      #$01,D0
00003BB8                          OR.B      #$32,(A4)+
00003BBC                          OR.W      #$005E,A0
00003BC0                          OR.B      #$01,D0
00003BC4                          OR.B      A7,D0
00003BC6                          OR.B      #$74,D0
00003BCA                          OR.B      #$01,D0
00003BCE                          OR.L      D0,D0
00003BD0                          OR.B      #$7C,D0
00003BD4                          OR.B      #$00,-(A6)
00003BD8                          OR.B      #$14,D1
00003BDC                          OR.B      #$74,D0
00003BE0                          OR.B      #$01,D0
00003BE4                          OR.L      D0,D0
00003BE6                          OR.B      #$7C,D0
00003BEA                          OR.B      #$00,CCR
00003BEE                          OR.B      #$08,D1
00003BF2                          OR.B      #$74,D0
00003BF6                          OR.B      #$01,D0
00003BFA                          OR.L      D0,D0
00003BFC                          OR.B      #$7C,D0
00003C00                          OR.W      #$0000,(A2)
00003C04                          OR.B      #$0A,D1
00003C08                          OR.B      #$74,D0
00003C0C                          OR.B      #$01,D0
00003C10                          OR.L      D0,D0
00003C12                          OR.B      #$7C,D0
00003C16                          OR.W      #$0000,-$07FB(A0)
00003C1C                          OR.B      #$00,$0000(A0)
00003C22                          Invalid
00003C24                          OR.B      #$00,$41F8(A4)
00003C2A                          Invalid
00003C2C                          BSR       $00003C38
00003C2E                          LEA       $0040(A0),A0
00003C32                          BSR       $00003C38
00003C34                          LEA       $0040(A0),A0
00003C38                          TST.B     $0000(A0)
00003C3C                          BEQ       $00003C4C
00003C3E                          MOVE.L    $001A(A0),A6
00003C42                          MOVE.B    $0017(A0),D0
00003C46                          BNE       $00003E14(pc)
00003C4A                          BRA       $00003C4E
00003C4C                          RTS
00003C4E                          MOVEQ     #$00,D0
00003C50                          MOVE.B    $0002(A0),D0
00003C54                          MOVE.W    +$00(D3.W),D0
00003C58                          JMP       +$00(D1.W)
00003C5C                          OR.B      #$50,A0
00003C60                          OR.W      #$0084,SR
00003C64                          MOVE.B    #$01,$E198
00003C6A                          MOVE.B    #$02,$0002(A0)
00003C70                          LEA       $00003D3C(pc),A6
00003C74                          MOVE.L    A6,$001A(A0)
00003C78                          MOVE.W    #$0014,$0028(A0)
00003C7E                          MOVE.B    #$05,$0032(A0)
00003C84                          MOVE.B    #$00,$0033(A0)
00003C8A                          MOVE.B    $0030(A0),D0
00003C8E                          LEA       $00003D60(pc),A4
00003C92                          TST.W     $FC3A
00003C96                          BEQ       $00003CA6
00003C98                          TST.B     $0006(A0)
00003C9C                          BNE       $00003CA2
00003C9E                          SUBQ.W    #$1,$FC3A
00003CA2                          LEA       $00003D7C(pc),A4
00003CA6                          JSR       $000287D6
00003CAC                          BSR       $00003CE6
00003CAE                          SUBQ.B    #$1,$0032(A0)
00003CB2                          BNE       $00003CBA
00003CB4                          MOVE.B    #$04,$0002(A0)
00003CBA                          JSR       $00028804
00003CC0                          JSR       $0002897E
00003CC6                          BEQ       $00003CD6
00003CC8                          MOVE.L    $E8F8,A4
00003CCC                          MOVE.L    A0,-(A4)
00003CCE                          MOVE.L    A4,$E8F8
00003CD2                          ADDQ.W    #$1,$E96E
00003CD6                          RTS
00003CD8                          MOVE.B    #$06,$0002(A0)
00003CDE                          RTS
00003CE0                          JMP       $00028B18
00003CE6                          LEA       $A000,A1
00003CEA                          MOVE.W    $0008(A1),$0008(A0)
00003CF0                          MOVE.W    $000C(A1),$000C(A0)
00003CF6                          MOVEQ     #$00,D1
00003CF8                          TST.W     $FC3A
00003CFC                          BEQ       $00003D00
00003CFE                          ADDQ.W    #$0,D1
00003D00                          BTST      #$01,$0043(A1)
00003D06                          BEQ       $00003D0A
00003D08                          ADDQ.W    #$4,D1
00003D0A                          MOVE.W    +$10(D0.W),D0
00003D0E                          MOVE.W    +$10(D7.L),D1
00003D12                          TST.W     $0030(A0)
00003D16                          BEQ       $00003D1A
00003D18                          NEG.W     D0
00003D1A                          TST.B     $005B(A1)
00003D1E                          BEQ       $00003D22
00003D20                          NEG.W     D1
00003D22                          ADD.W     Invalid
00003D24                          OR.B      #$68,A0
00003D28                          OR.B      #$75,A4
00003D2C                          Invalid
00003D2E                          Invalid
00003D30                          Invalid
00003D32                          OR.B      #$40,D2
00003D36                          Invalid
00003D38                          OR.W      #$0002,D0
00003D3C                          OR.B      #$01,D0
00003D40                          OR.B      #$01,D0
00003D44                          OR.B      #$01,D0
00003D48                          OR.B      #$01,D0
00003D4C                          OR.B      #$01,D0
00003D50                          AND.B     D0,D0
00003D52                          MOVE.W    (A0),D0
00003D54                          NEGX.B    D0
00003D56                          MOVE.W    (A0),D0
00003D58                          ASR.B     #8,D0
00003D5A                          ADDQ.B    #$0,(A0)
00003D5C                          MOVE.L    D0,D0
00003D5E                          ADDQ.B    #$0,(A0)
00003D60                          OR.B      #$34,$0409(A0)
00003D66                          OR.B      #$82,D0
00003D6A                          OR.B      #$5C,-(A0)
00003D6E                          OR.B      #$08,$0000(A0)
00003D74                          OR.L      #$0000005C,D2
00003D7A                          OR.B      #$44,$00(A4,D0.L)
00003D80                          SUB.B     #$00,A1
00003D84                          BCLR      D1,D2
00003D86                          OR.B      #$90,-(A0)
00003D8A                          OR.W      #$0408,D4
00003D8E                          OR.B      #$82,D0
00003D92                          OR.B      #$90,D0
00003D96                          OR.W      #$0900,(A0)
00003D9A                          Invalid
00003D9C                          MOVE.B    D0,D0
00003D9E                          DIVU      $0D(A0,D0.L),D0
00003DA2                          AND.L     #$F00D100A,(A0)
00003DA8                          CMP.W     $0C(A0,D0.L),A0
00003DAC                          MOVE.B    (A0),(A1)+
00003DAE                          Invalid
00003DB0                          MOVE.B    (A6),D0
00003DB2                          Invalid
00003DB4                          SUB.B     #$80,D0
00003DB8                          OR.B      #$02,A5
00003DBC                          SUB.B     D0,D0
00003DBE                          BTST      D6,D0
00003DC0                          EOR.L     #$080C0012,$D0(A0,D4.W)
00003DC8                          SUB.B     #$F0,D0
00003DCC                          BTST      D6,D0
00003DCE                          Invalid
00003DD0                          MOVE.B    D0,D0
00003DD2                          SUB.W     $0D(A0,D0.L),A0
00003DD6                          AND.L     #$F00D1018,-(A0)
00003DDC                          MULU      $0D(A0,D0.W),D0
00003DE0                          MOVE.B    -(A0),(A6)+
00003DE2                          Invalid
00003DE4                          MOVE.B    -(A0),D0
00003DE6                          Invalid
00003DE8                          CMP.B     #$20,(A0)
00003DEC                          Invalid
00003DEE                          MOVE.B    (A6),D0
00003DF0                          NEGX.B    D0
00003DF2                          SUB.B     #$90,D0
00003DF6                          OR.B      #$02,A5
00003DFA                          Invalid
00003DFC                          BTST      D6,D0
00003DFE                          MOVE.B    D0,(A4)+
00003E00                          OR.B      #$1C,A5
00003E04                          ASR.B     #8,D0
00003E06                          BTST      D6,D0
00003E08                          MOVE.L    D0,D0
00003E0A                          BTST      #$12,A4
00003E0E                          MOVE.L    A0,D0
00003E10                          SUB.B     #$40,D0
00003E14                          MOVEQ     #$00,D0
00003E16                          MOVE.B    $0002(A0),D0
00003E1A                          MOVE.W    +$00(D3.W),D0
00003E1E                          JMP       +$00(D1.W)
00003E22                          OR.B      #$54,A0
00003E26                          BCHG      D0,$0174(A4)
00003E2A                          MOVE.B    #$02,$0002(A0)
00003E30                          LEA       $0000405E(pc),A6
00003E34                          MOVE.L    A6,$001A(A0)
00003E38                          MOVEQ     #$00,D0
00003E3A                          MOVE.B    $0006(A0),D0
00003E3E                          ADD.W     D0,D0
00003E40                          MOVE.W    +$00(D3.W),D0
00003E44                          JMP       +$00(D1.W)
00003E48                          OR.B      #$1A,D6
00003E4C                          OR.B      #$2B,-(A4)
00003E50                          JSR       $00029062
00003E56                          MOVEQ     #$00,D0
00003E58                          LEA       $0000407C(pc),A4
00003E5C                          JMP       $000287D6
00003E62                          LEA       $000040B2(pc),A4
00003E66                          JMP       $000287D2
00003E6C                          LEA       $000040E2(pc),A4
00003E70                          JMP       $000287D2
00003E76                          MOVEQ     #$00,D0
00003E78                          MOVE.B    $0006(A0),D0
00003E7C                          ADD.W     D0,D0
00003E7E                          MOVE.W    +$00(D3.W),D0
00003E82                          JMP       +$00(D1.W)
00003E86                          OR.B      #$70,D6
00003E8A                          Invalid
00003E8C                          BSR       $00003E9A
00003E8E                          JSR       $00028804
00003E94                          JMP       $00003CC0
00003E9A                          TST.B     $0003(A0)
00003E9E                          BNE       $00003EE2
00003EA0                          CMP.B     #$01,$0026(A0)
00003EA6                          BNE       $00003EE0
00003EA8                          JSR       $00028A1C
00003EAE                          BCS       $00003EE0
00003EB0                          MOVE.W    #$0100,$0000(A1)
00003EB6                          MOVE.B    #$01,$0017(A1)
00003EBC                          MOVE.B    #$01,$0006(A1)
00003EC2                          MOVE.B    $0016(A0),$0016(A1)
00003EC8                          MOVE.W    $0008(A0),$0008(A1)
00003ECE                          MOVE.W    $000C(A0),$000C(A1)
00003ED4                          MOVE.B    #$02,$0003(A0)
00003EDA                          MOVE.B    #$09,$0030(A0)
00003EE0                          RTS
00003EE2                          TST.B     $0030(A0)
00003EE6                          BEQ       $00003EEE
00003EE8                          SUBQ.B    #$1,$0030(A0)
00003EEC                          RTS
00003EEE                          MOVE.B    #$04,$0002(A0)
00003EF4                          RTS
00003EF6                          JSR       $00003FBE(pc)
00003EFA                          CMP.B     #$01,$0026(A0)
00003F00                          BNE       $00003F2E
00003F02                          JSR       $00028A1C
00003F08                          BCS       $00003F2E
00003F0A                          MOVE.W    #$0100,$0000(A1)
00003F10                          MOVE.B    #$01,$0017(A1)
00003F16                          MOVE.B    #$02,$0006(A1)
00003F1C                          MOVE.B    $0016(A0),$0016(A1)
00003F22                          MOVE.W    $0008(A0),$0008(A1)
00003F28                          MOVE.W    $000C(A0),$000C(A1)
00003F2E                          ADDQ.W    #$4,$0008(A0)
00003F32                          TST.B     $0016(A0)
00003F36                          BEQ       $00003F3C
00003F38                          DBRA      D0,$00003F42
00003F3C                          JSR       $00028804
00003F42                          JSR       $00003CC0
00003F48                          TST.B     $0001(A0)
00003F4C                          BNE       $00003F5A
00003F4E                          MOVE.B    #$04,$A003
00003F54                          MOVE.B    #$04,$0002(A0)
00003F5A                          RTS
00003F5C                          CMP.B     #$01,$0026(A0)
00003F62                          BEQ       $00003F86
00003F64                          ADD.L     #$00038000,$0008(A0)
00003F6C                          TST.B     $0016(A0)
00003F70                          BEQ       $00003F7A
00003F72                          SUB.L     #$00070000,$0008(A0)
00003F7A                          JSR       $00028804
00003F80                          JMP       $00003CC0
00003F86                          MOVE.B    #$04,$0002(A0)
00003F8C                          RTS
00003F8E                          MOVE.B    #$06,$0002(A0)
00003F94                          RTS
00003F96                          TST.W     $E142
00003F9A                          BNE       $00003FB8
00003F9C                          MOVE.W    #$001E,$E142
00003FA2                          CMP.B     #$04,$FC0A
00003FA8                          BCS       $00003FB8
00003FAA                          CMP.B     #$0A,$FC26
00003FB0                          BNE       $00003FB8
00003FB2                          MOVE.W    #$0258,$E142
00003FB8                          JMP       $00028B18
00003FBE                          TST.B     $0030(A0)
00003FC2                          BNE       $00004032(pc)
00003FC6                          CMP.W     #$0004,$E964
00003FCC                          BLT       $00004036(pc)
00003FD0                          JSR       $00028A06
00003FD6                          BCS       $0000402C(pc)
00003FDA                          BSR       $00004038
00003FDC                          MOVE.B    #$01,$0021(A1)
00003FE2                          ADD.W     #$0010,$000C(A1)
00003FE8                          JSR       $00028A06
00003FEE                          BCS       $00004036(pc)
00003FF2                          BSR       $00004038
00003FF4                          MOVE.B    #$01,$0021(A1)
00003FFA                          SUB.W     #$0010,$000C(A1)
00004000                          JSR       $00028A06
00004006                          BCS       $00004036
00004008                          BSR       $00004038
0000400A                          MOVE.B    #$00,$0021(A1)
00004010                          ADD.W     #$0018,$000C(A1)
00004016                          JSR       $00028A06
0000401C                          BCS       $00004036
0000401E                          BSR       $00004038
00004020                          MOVE.B    #$00,$0021(A1)
00004026                          SUB.W     #$0018,$000C(A1)
0000402C                          MOVE.B    #$1E,$0030(A0)
00004032                          SUBQ.B    #$1,$0030(A0)
00004036                          RTS
00004038                          MOVE.W    #$0100,$0000(A1)
0000403E                          MOVE.B    #$08,$0017(A1)
00004044                          MOVE.B    #$01,$0006(A1)
0000404A                          MOVE.B    $0016(A0),$0016(A1)
00004050                          MOVE.W    $0008(A0),$0008(A1)
00004056                          MOVE.W    $000C(A0),$000C(A1)
0000405C                          RTS
0000405E                          OR.B      #$A0,D0
00004062                          OR.B      #$A0,D0
00004066                          OR.B      #$A0,D0
0000406A                          OR.B      #$A0,D0
0000406E                          OR.B      #$A0,D0
00004072                          OR.B      #$80,(A6)
00004076                          OR.B      #$80,D0
0000407A                          OR.B      #$20,D0
0000407E                          OR.B      #$10,D2
00004082                          OR.B      D3,D0
00004084                          OR.B      #$B4,D0
00004088                          OR.W      #$0018,A5
0000408C                          OR.B      D4,D0
0000408E                          OR.B      #$B4,D0
00004092                          OR.L      #$002C8004,D3
00004098                          OR.B      #$CA,D0
0000409C                          OR.L      #$00488004,D3
000040A2                          OR.B      #$CA,D0
000040A6                          OR.L      #$01488080,D3
000040AC                          OR.B      #$CA,D0
000040B0                          OR.W      #$006E,A0
000040B4                          OR.W      #$002E,(A0)+
000040B8                          OR.B      #$0A,A6
000040BC                          OR.B      #$12,D0
000040C0                          OR.B      #$0E,-$7F7F(A6)
000040C6                          OR.B      #$12,D0
000040CA                          OR.W      #$002E,(A0)+
000040CE                          OR.B      #$0A,A6
000040D2                          OR.B      #$12,-(A0)
000040D6                          OR.B      #$0E,-$7F7F(A6)
000040DC                          OR.B      #$12,-(A0)
000040E0                          OR.W      #$009E,$0088(A6)
000040E6                          OR.B      #$06,(A6)+
000040EA                          OR.B      A2,D0
000040EC                          OR.B      #$32,D0
000040F0                          OR.B      #$06,(A6)+
000040F4                          OR.L      D0,D0
000040F6                          OR.B      #$32,D0
000040FA                          OR.L      #$001E0006,(A2)
00004100                          OR.B      A2,D0
00004102                          OR.B      #$32,-(A0)
00004106                          OR.B      #$06,(A6)+
0000410A                          OR.L      D0,D0
0000410C                          OR.B      #$32,-(A0)
00004110                          OR.L      #$0300E00F,$0000(A0)
00004118                          ASR       -(A0)
0000411A                          MOVEP.W   $0000(A0),D7
0000411E                          OR.B      #$00,A7
00004122                          ASR.B     #8,D0
00004124                          BTST      D7,(A0)+
00004126                          OR.B      #$00,D0
0000412A                          Invalid
0000412C                          OR.B      #$F0,(A0)
00004130                          MOVEP.W   $1010(A0),D7
00004134                          ADD.B     A3,D0
00004136                          OR.B      #$D0,-(A0)
0000413A                          MOVEP.W   $2000(A0),D5
0000413E                          LSR.B     #4,D0
00004140                          OR.B      #$E8,$0808(A4)
00004146                          MOVE.L    (A0)+,D6
00004148                          ADD.B     D5,D4
0000414A                          OR.B      #$D8,$0508(A7)
00004150                          MOVE.L    (A0)+,-(A7)
00004152                          MOVE.B    A0,D0
00004154                          MOVE.B    -$2FF0(A4),D0
00004158                          BTST      #$18,(A0)+
0000415C                          MOVE.B    A3,D0
0000415E                          MOVE.B    -(A0),D0
00004160                          ROXR.B    #4,D0
00004162                          BTST      D5,(A0)+
00004164                          MOVE.L    D0,D0
00004166                          MOVE.B    D5,D4
00004168                          MOVE.B    -$27E8(A7),D0
0000416C                          BTST      D2,(A0)+
0000416E                          MOVE.L    (A0)+,-(A7)
00004170                          BTST      D2,D0
00004172                          Invalid
00004174                          OR.B      #$F0,$0D(A3,D0.W)
0000417A                          MOVE.W    $F004,
0000417E                          OR.W      #$1800,D3
00004182                          BTST      D6,(A0)
00004184                          MOVE.W    (A0)+,$000D103B
0000418A                          Invalid
0000418C                          SUB.B     #$18,(A0)
00004190                          BTST      D0,D0
00004192                          Invalid
00004194                          OR.W      #$F000,D5
00004198                          BTST      D6,(A0)
0000419A                          LEA       $70(A0,D0.W),A2
0000419E                          MOVE.B    $FC0A,D0
000041A2                          MULU      #$0B,D0
000041A6                          ADD.B     $FC26,D0
000041AA                          MOVE.B    +$00(D6.W),$E122
000041B0                          MOVE.B    #$D0,$E123
000041B6                          RTS
000041B8                          ADD.B     #$06,D6
000041BC                          ADD.B     #$06,D6
000041C0                          ADD.B     #$06,D6
000041C4                          ADD.B     #$06,D6
000041C8                          ADD.B     #$06,D6
000041CC                          ADD.B     #$06,D6
000041D0                          OR.B      D6,D3
000041D2                          OR.B      D6,D3
000041D4                          ADD.B     #$06,D6
000041D8                          ADD.B     #$06,D6
000041DC                          ADD.L     #$86060606,D6
000041E2                          ADD.B     #$06,D6
000041E6                          OR.L      D6,D3
000041E8                          OR.L      D6,D3
000041EA                          OR.B      D6,D3
000041EC                          ADD.B     #$00,D6
000041F0                          JSR       $0000419C(pc)
000041F4                          LEA       $00040530,A6
000041FA                          LEA       $A000,A0
000041FE                          AND.B     #$AA,$0053(A0)
00004204                          CLR.B     $0037(A0)
00004208                          CMP.B     #$04,$FC0A
0000420E                          BNE       $0000421A
00004210                          CMP.W     #$0006,$F05A
00004216                          BEQ       $000080F0(pc)
0000421A                          MOVEQ     #$00,D0
0000421C                          MOVE.B    $0002(A0),D0
00004220                          MOVE.W    +$00(D1.W),D0
00004224                          JSR       +$00(D7.L)
00004228                          JSR       $00004640(pc)
0000422C                          CLR.B     $0055(A0)
00004230                          AND.B     #$55,$0053(A0)
00004236                          MOVE.B    #$00,$E199
0000423C                          MOVE.B    #$00,$E19A
00004242                          RTS
00004244                          OR.B      #$AE,A0
00004248                          Invalid
0000424A                          BCLR      D1,$214E(A0)
0000424E                          OR.B      #$F9,(A2)+
00004252                          OR.B      #$30,D4
00004256                          SUB.L     A6,A1
00004258                          MOVE.W    A1,$0028(A0)
0000425C                          MOVE.B    #$01,$0000(A0)
00004262                          MOVE.B    #$01,$0001(A0)
00004268                          MOVE.B    #$02,$0002(A0)
0000426E                          MOVEQ     #$00,D0
00004270                          MOVE.W    D0,$F0B6
00004274                          MOVE.B    D0,$0030(A0)
00004278                          AND.B     #$01,D0
0000427C                          LEA       $00040582,A4
00004282                          JSR       $00007E40(pc)
00004286                          TST.W     $FC50
0000428A                          BNE       $000042CC
0000428C                          TST.B     $FC3C
00004290                          BEQ       $000042CA
00004292                          JSR       $00028A66
00004298                          BCS       $000042CA
0000429A                          MOVE.W    #$0100,$0000(A1)
000042A0                          MOVE.B    #$00,$0050(A1)
000042A6                          MOVE.L    A1,$FC44
000042AA                          CMP.B     #$02,$FC3C
000042B0                          BNE       $000042CA
000042B2                          JSR       $00028A66
000042B8                          BCS       $000042CA
000042BA                          MOVE.W    #$0100,$0000(A1)
000042C0                          MOVE.B    #$01,$0050(A1)
000042C6                          MOVE.L    A1,$FC48
000042CA                          RTS
000042CC                          JSR       $00028A66
000042D2                          BCS       $000042CA
000042D4                          MOVE.W    #$0100,$0000(A1)
000042DA                          MOVE.B    #$01,$0017(A1)
000042E0                          MOVE.W    $0008(A0),$0008(A1)
000042E6                          MOVE.W    $000C(A0),$000C(A1)
000042EC                          MOVE.L    A1,$FC44
000042F0                          RTS
000042F2                          BTST      #$00,$E10A
000042F8                          BEQ       $00004308
000042FA                          BTST      #$06,$E10B
00004300                          BEQ       $00004308
00004302                          EOR.B     #$01,$E144
00004308                          TST.B     $E144
0000430C                          BEQ       $00004340
0000430E                          BTST      #$03,$E108
00004314                          BEQ       $0000431A
00004316                          SUBQ.W    #$2,$000C(A0)
0000431A                          BTST      #$02,$E108
00004320                          BEQ       $00004326
00004322                          ADDQ.W    #$2,$000C(A0)
00004326                          BTST      #$00,$E108
0000432C                          BEQ       $00004332
0000432E                          ADDQ.W    #$2,$0008(A0)
00004332                          BTST      #$01,$E108
00004338                          BEQ       $0000433E
0000433A                          SUBQ.W    #$2,$0008(A0)
0000433E                          RTS
00004340                          MOVE.B    $F09F,D0
00004344                          AND.W     #$000F,D0
00004348                          BNE       $00004380
0000434A                          MOVE.W    $0008(A0),D4
0000434E                          SUB.W     $EA08,D4
00004352                          MOVE.W    $000C(A0),D3
00004356                          SUB.W     $EA0C,D3
0000435A                          CMP.W     #$0170,D4
0000435E                          BGE       $000045EE(pc)
00004362                          CMP.W     #$FFE0,D4
00004366                          BLT       $000045EE(pc)
0000436A                          TST.B     $005B(A0)
0000436E                          BEQ       $00004378
00004370                          CMP.W     #$FFE0,D3
00004374                          BLT       $000045EE(pc)
00004378                          CMP.W     #$0100,D3
0000437C                          BGE       $000045EE(pc)
00004380                          MOVEQ     #$00,D0
00004382                          MOVE.B    $0003(A0),D0
00004386                          JSR       +$00(D1.L)
0000438A                          CMP.B     #$2C,$0003(A0)
00004390                          BEQ       $000043B8
00004392                          MOVE.W    $0008(A0),D0
00004396                          SUB.W     $EA08,D0
0000439A                          CMP.W     #$0120,D0
0000439E                          BGT       $000043B0
000043A0                          CMP.W     #$0020,D0
000043A4                          BGT       $000043B8
000043A6                          SUB.W     #$0020,D0
000043AA                          (CCC     #$0,$A008
000043AE                          RTS
000043B0                          SUB.W     #$0120,D0
000043B4                          (CCC     #$0,$A008
000043B8                          RTS
000043BA                          BRA       $000043EC(pc)
000043BE                          BRA       $0000527C(pc)
000043C2                          BRA       $000057F4(pc)
000043C6                          BRA       $000063A2(pc)
000043CA                          BRA       $000043EA(pc)
000043CE                          BRA       $00006E2E(pc)
000043D2                          BRA       $0000720A(pc)
000043D6                          BRA       $0000760E(pc)
000043DA                          BRA       $000043EA(pc)
000043DE                          BRA       $000079DE(pc)
000043E2                          BRA       $00007A70(pc)
000043E6                          BRA       $00007CB4(pc)
000043EA                          RTS
000043EC                          TST.B     $F0AE
000043F0                          BNE       $000043FE
000043F2                          MOVE.B    $001E(A0),D0
000043F6                          CMP.B     $0056(A0),D0
000043FA                          BNE       $00007D5C(pc)
000043FE                          BTST      #$04,$E109
00004404                          BEQ       $0000440A
00004406                          JSR       $000050C6(pc)
0000440A                          BTST      #$06,$E109
00004410                          BNE       $00005EAA(pc)
00004414                          BTST      #$05,$E109
0000441A                          BNE       $00005EAA(pc)
0000441E                          BSET      #$00,$0053(A0)
00004424                          MOVE.B    $0022(A0),$0031(A0)
0000442A                          MOVE.B    $0023(A0),$0032(A0)
00004430                          MOVE.B    $0016(A0),$0042(A0)
00004436                          JSR       $00007F1E(pc)
0000443A                          CMP.B     $0042(A0),D0
0000443E                          BEQ       $0000445A
00004440                          JSR       $000046C4(pc)
00004444                          CMP.B     $0030(A0),D1
00004448                          BEQ       $0000444E
0000444A                          JSR       $00006DD2(pc)
0000444E                          JSR       $0000471E(pc)
00004452                          CMP.B     $0033(A0),D0
00004456                          BNE       $0000477A(pc)
0000445A                          CMP.B     #$08,$0016(A0)
00004460                          BLT       $00004466
00004462                          MOVEQ     #$00,D0
00004464                          BRA       $00004474
00004466                          JSR       $000014E6(pc)
0000446A                          LSL.W     #3,D0
0000446C                          ADD.B     $0016(A0),D0
00004470                          MOVE.B    +$00(D2.L),D0
00004474                          ADD.W     D0,D0
00004476                          MOVE.W    +$00(D3.W),D0
0000447A                          JMP       +$00(D1.W)
0000447E                          OR.W      #$0086,
00004482                          OR.W      #$006A,A0
00004486                          OR.B      #$00,D0
0000448A                          OR.B      #$00,D0
0000448E                          OR.B      #$01,D2
00004492                          BTST      D0,D1
00004494                          BTST      D1,D3
00004496                          AND.B     #$01,D2
0000449A                          BTST      D0,D1
0000449C                          BTST      D1,D3
0000449E                          AND.B     #$00,D2
000044A2                          BTST      D1,D3
000044A4                          BTST      D1,D0
000044A6                          AND.B     #$00,D2
000044AA                          BTST      D1,D3
000044AC                          BTST      D1,D0
000044AE                          BTST      D1,D0
000044B0                          AND.B     #$00,D2
000044B4                          BTST      D1,D3
000044B6                          BTST      D1,D0
000044B8                          AND.B     #$00,D2
000044BC                          BTST      D1,D3
000044BE                          BTST      D1,D2
000044C0                          AND.B     #$01,D1
000044C4                          BTST      D1,D3
000044C6                          MOVE.B    #$10,$0043(A0)
000044CC                          JSR       $000048D0(pc)
000044D0                          JSR       $0000504C(pc)
000044D4                          JSR       $00004FDA(pc)
000044D8                          JSR       $00007E6C(pc)
000044DC                          JSR       $00005100(pc)
000044E0                          JSR       $00005086(pc)
000044E4                          BRA       $0000469E(pc)
000044E8                          MOVE.B    #$10,$0043(A0)
000044EE                          JSR       $000048D0(pc)
000044F2                          JSR       $0000504C(pc)
000044F6                          JSR       $0000500E(pc)
000044FA                          BRA       $000044D8
000044FC                          MOVE.B    #$00,$0043(A0)
00004502                          BRA       $0000450A
00004504                          MOVE.B    #$02,$0043(A0)
0000450A                          MOVEQ     #$00,D7
0000450C                          MOVE.W    D7,$0038(A0)
00004510                          MOVE.B    D7,$005A(A0)
00004514                          MOVE.B    D7,$0036(A0)
00004518                          JSR       $0000504C(pc)
0000451C                          JSR       $00004FAE(pc)
00004520                          JSR       $00007E6C(pc)
00004524                          JSR       $00005100(pc)
00004528                          JSR       $00005086(pc)
0000452C                          JSR       $000014E6(pc)
00004530                          TST.B     D0
00004532                          BNE       $0000469E(pc)
00004536                          BRA       $00007DF0(pc)
0000453A                          MOVE.B    $F09F,D0
0000453E                          AND.B     #$03,D0
00004542                          BNE       $0000454E
00004544                          JSR       $0002897E
0000454A                          BEQ       $000045EE(pc)
0000454E                          MOVEQ     #$00,D0
00004550                          MOVE.B    $0003(A0),D0
00004554                          MOVE.W    +$00(D3.W),D0
00004558                          JMP       +$00(D1.W)
0000455C                          OR.B      #$90,D6
00004560                          OR.L      #$48E70180,(A2)
00004566                          LEA       $A120,A0
0000456A                          LEA       $F850,A6
0000456E                          MOVE.W    #$0040,D2
00004572                          MOVE.W    #$0002,D7
00004576                          MOVE.W    #$000F,D5
0000457A                          MOVEQ     #$00,D0
0000457C                          MOVEQ     #$00,D1
0000457E                          MOVEQ     #$00,D3
00004580                          LEA       (A0),A1
00004582                          MOVE.W    D5,D4
00004584                          MOVE.L    D3,(A1)+
00004586                          DBRA      D4,$00004584
0000458A                          MOVE.L    A0,-(A6)
0000458C                          ADDQ.W    #$1,D1
0000458E                          LEA       $20(A0,D0.W),A0
00004592                          DBRA      D7,$00004580
00004596                          MOVE.W    D1,$E95A
0000459A                          MOVE.L    A6,$E8CC
0000459E                          MOVEM.L   (A7)+,D7/A0
000045A2                          MOVE.B    #$01,$EA01
000045A8                          JSR       $00028A1C
000045AE                          BCS       $000045EC
000045B0                          MOVE.W    #$0100,$0000(A1)
000045B6                          MOVE.B    #$01,$0017(A1)
000045BC                          MOVE.B    #$00,$0006(A1)
000045C2                          MOVE.B    #$00,$0016(A1)
000045C8                          TST.B     $0030(A0)
000045CC                          BNE       $000045D4
000045CE                          MOVE.B    #$01,$0016(A1)
000045D4                          MOVE.W    $0008(A0),$0008(A1)
000045DA                          MOVE.W    $000C(A0),$000C(A1)
000045E0                          MOVE.B    #$00,$0000(A0)
000045E6                          MOVE.B    #$02,$0003(A0)
000045EC                          RTS
000045EE                          TST.W     $E142
000045F2                          BNE       $00004610
000045F4                          MOVE.W    #$0001,$E142
000045FA                          CMP.B     #$04,$FC0A
00004600                          BCS       $00004610
00004602                          CMP.B     #$0A,$FC26
00004608                          BNE       $00004610
0000460A                          MOVE.W    #$0168,$E142
00004610                          MOVE.B    #$01,$F0A6
00004616                          MOVE.B    #$06,$0002(A0)
0000461C                          MOVEQ     #$00,D7
0000461E                          MOVE.B    D7,$FC41
00004622                          MOVE.B    D7,$FC42
00004626                          MOVE.W    D7,$FC38
0000462A                          MOVE.W    D7,$FC3A
0000462E                          MOVE.B    D7,$F0CF
00004632                          MOVE.B    D7,$0000(A0)
00004636                          MOVE.B    D7,$FC3C
0000463A                          MOVE.W    D7,$FC50
0000463E                          RTS
00004640                          TST.W     $FC4C
00004644                          BNE       $00004656
00004646                          MOVE.B    #$01,$0001(A0)
0000464C                          TST.W     $FC38
00004650                          BNE       $00004656(pc)
00004654                          RTS
00004656                          SUBQ.W    #$1,$FC4C
0000465A                          TST.B     $0058(A0)
0000465E                          BNE       $0000467E
00004660                          TST.B     $0059(A0)
00004664                          BEQ       $00004672
00004666                          SUBQ.B    #$1,$0059(A0)
0000466A                          MOVE.B    #$01,$0001(A0)
00004670                          RTS
00004672                          MOVE.B    #$01,$0058(A0)
00004678                          MOVE.B    #$02,$0059(A0)
0000467E                          TST.B     $0059(A0)
00004682                          BEQ       $00004690
00004684                          SUBQ.B    #$1,$0059(A0)
00004688                          MOVE.B    #$00,$0001(A0)
0000468E                          RTS
00004690                          MOVE.B    #$00,$0058(A0)
00004696                          MOVE.B    #$03,$0059(A0)
0000469C                          BRA       $00004660
0000469E                          TST.B     $005B(A0)
000046A2                          BNE       $000046B0
000046A4                          MOVE.B    $0022(A0),D0
000046A8                          CMP.B     $0031(A0),D0
000046AC                          BNE       $000046BC
000046AE                          RTS
000046B0                          MOVE.B    $0023(A0),D0
000046B4                          CMP.B     $0032(A0),D0
000046B8                          BNE       $000046BC
000046BA                          RTS
000046BC                          JSR       $00007DDC(pc)
000046C0                          BRA       $00004B1C(pc)
000046C4                          CMP.B     #$08,D0
000046C8                          BLT       $000046D0
000046CA                          MOVE.B    #$FF,D1
000046CE                          RTS
000046D0                          JSR       $000014FC(pc)
000046D4                          LSL.W     #3,D1
000046D6                          ADD.B     D0,D1
000046D8                          MOVE.B    +$10(D2.W),D1
000046DC                          RTS
000046DE                          Invalid
000046E0                          Invalid
000046E2                          Invalid
000046E4                          Invalid
000046E6                          Invalid
000046E8                          OR.B      #$01,D0
000046EC                          BTST      D0,D1
000046EE                          OR.B      #$00,D0
000046F2                          Invalid
000046F4                          BTST      D0,D1
000046F6                          OR.B      #$00,D0
000046FA                          BTST      D0,D1
000046FC                          BTST      D0,D1
000046FE                          OR.B      #$00,D0
00004702                          BTST      D0,D1
00004704                          BTST      D0,D1
00004706                          BTST      D0,D0
00004708                          OR.B      #$01,D0
0000470C                          BTST      D0,D1
0000470E                          BTST      D0,D0
00004710                          OR.B      #$01,D0
00004714                          BTST      D0,D1
00004716                          BTST      D0,D0
00004718                          OR.B      #$01,D0
0000471C                          BTST      D0,D1
0000471E                          CMP.B     #$08,$0016(A0)
00004724                          BLT       $0000472A
00004726                          MOVEQ     #$00,D0
00004728                          RTS
0000472A                          JSR       $000014FC(pc)
0000472E                          LSL.W     #3,D1
00004730                          ADD.B     $0016(A0),D1
00004734                          MOVE.B    +$10(D2.W),D0
00004738                          RTS
0000473A                          OR.B      #$00,D0
0000473E                          OR.B      #$00,D0
00004742                          OR.B      #$04,D2
00004746                          BTST      D0,D5
00004748                          BTST      D1,D3
0000474A                          AND.B     #$04,D2
0000474E                          BTST      D0,D5
00004750                          BTST      D1,D3
00004752                          AND.B     #$04,D2
00004756                          BTST      D1,D3
00004758                          BTST      D1,D5
0000475A                          AND.B     #$04,D2
0000475E                          BTST      D1,D3
00004760                          BTST      D1,D5
00004762                          BTST      D1,D4
00004764                          AND.B     #$05,D2
00004768                          BTST      D1,D3
0000476A                          BTST      D1,D4
0000476C                          AND.B     #$05,D2
00004770                          BTST      D1,D3
00004772                          BTST      D1,D2
00004774                          AND.B     #$05,D4
00004778                          BTST      D1,D3
0000477A                          MOVE.B    D0,$0033(A0)
0000477E                          BSET      #$00,$0053(A0)
00004784                          TST.B     $005B(A0)
00004788                          BNE       $000047AA
0000478A                          BCLR      #$01,$0053(A0)
00004790                          BEQ       $0000479A
00004792                          MOVE.B    $0054(A0),$0022(A0)
00004798                          BRA       $000047A4
0000479A                          JSR       $00001AAE(pc)
0000479E                          BCC       $000047A4
000047A0                          ADD.W     Invalid
000047A2                          OR.B      #$28,A4
000047A6                          OR.B      #$1E,-(A2)
000047AA                          BCLR      #$01,$0053(A0)
000047B0                          BEQ       $000047BA
000047B2                          MOVE.B    $0054(A0),$0023(A0)
000047B8                          BRA       $000047C4
000047BA                          JSR       $000019D4(pc)
000047BE                          BCC       $000047C4
000047C0                          ADD.W     Invalid
000047C2                          OR.B      #$28,A4
000047C6                          OR.B      #$41,-(A3)
000047CA                          OR.B      #$41,D7
000047CE                          ADD.W     D1,D1
000047D0                          MOVE.L    +$10(D7.L),A4
000047D4                          MOVE.B    $0033(A0),D0
000047D8                          AND.W     #$0007,D0
000047DC                          ADD.W     D0,D0
000047DE                          ADD.W     D0,D0
000047E0                          MOVE.L    $00(A4,D0.W),A4
000047E4                          MOVE.B    $0030(A0),D0
000047E8                          AND.B     #$01,D0
000047EC                          BRA       $00007E40(pc)
000047F0                          OR.B      #$10,D0
000047F4                          OR.B      #$20,D0
000047F8                          OR.B      #$38,D0
000047FC                          OR.B      #$50,D0
00004800                          OR.B      #$68,D0
00004804                          OR.B      #$80,D0
00004808                          OR.B      #$98,D0
0000480C                          OR.B      #$B0,D0
00004810                          OR.B      #$82,D4
00004814                          OR.B      #$AA,D4
00004818                          OR.B      #$D2,D4
0000481C                          OR.B      #$D2,D4
00004820                          OR.B      #$82,D4
00004824                          OR.B      #$AA,D4
00004828                          OR.B      #$D2,D4
0000482C                          OR.B      #$D2,D4
00004830                          OR.B      #$AA,D4
00004834                          OR.B      #$AA,D4
00004838                          OR.B      #$0A,D4
0000483C                          OR.B      #$5A,D4
00004840                          OR.B      #$AA,D4
00004844                          OR.B      #$22,D4
00004848                          OR.B      #$5A,D4
0000484C                          OR.B      #$5A,D4
00004850                          OR.B      #$1A,D4
00004854                          OR.B      #$1A,D4
00004858                          OR.B      #$6A,D4
0000485C                          OR.B      #$D2,D4
00004860                          OR.B      #$1A,D4
00004864                          OR.B      #$1A,D4
00004868                          OR.B      #$5A,D4
0000486C                          OR.B      #$5A,D4
00004870                          OR.B      #$AA,D4
00004874                          OR.B      #$12,D4
00004878                          OR.B      #$5A,D4
0000487C                          OR.B      #$5A,D4
00004880                          OR.B      #$6A,D4
00004884                          OR.B      #$6A,D4
00004888                          OR.B      #$34,D4
0000488C                          OR.B      #$DE,D4
00004890                          OR.B      #$6A,D4
00004894                          OR.B      #$6A,D4
00004898                          OR.B      #$2A,D4
0000489C                          OR.B      #$2A,D4
000048A0                          OR.B      #$F4,D4
000048A4                          OR.B      #$9E,D4
000048A8                          OR.B      #$2A,D4
000048AC                          OR.B      #$2A,D4
000048B0                          OR.B      #$1A,D4
000048B4                          OR.B      #$6A,D4
000048B8                          OR.B      #$66,D4
000048BC                          OR.B      #$DE,D4
000048C0                          OR.B      #$6A,D4
000048C4                          OR.B      #$6A,D4
000048C8                          OR.B      #$1A,D4
000048CC                          OR.B      #$6A,D4
000048D0                          TST.B     $0036(A0)
000048D4                          BEQ       $000048E0
000048D6                          JSR       $00004AB0(pc)
000048DA                          JSR       $00004AEE(pc)
000048DE                          BRA       $000048EA
000048E0                          TST.W     $0038(A0)
000048E4                          BNE       $000048EA
000048E6                          JSR       $00004A30(pc)
000048EA                          BSR       $0000491A
000048EC                          MOVEQ     #$00,D0
000048EE                          MOVE.B    $0038(A0),D0
000048F2                          AND.B     #$0F,D0
000048F6                          LSL.W     #4,D0
000048F8                          JSR       $000014FC(pc)
000048FC                          ADD.W     D1,D1
000048FE                          ADD.W     D1,D0
00004900                          MOVE.W    +$00(D7.W),D0
00004904                          SWAP      D0
00004906                          ASR.L     #8,D0
00004908                          MOVE.B    $0030(A0),D1
0000490C                          CMP.B     $005B(A0),D1
00004910                          BNE       $00004914
00004912                          NEG.L     D0
00004914                          ADD.L     Invalid
00004916                          OR.B      #$75,A4
0000491A                          MOVE.W    $0038(A0),D0
0000491E                          SWAP      D0
00004920                          ASR.L     #8,D0
00004922                          TST.B     $0030(A0)
00004926                          BEQ       $0000492A
00004928                          NEG.L     D0
0000492A                          ADD.L     Invalid
0000492C                          OR.B      #$75,A0
00004930                          OR.B      #$00,D0
00004934                          OR.B      #$00,D0
00004938                          OR.B      #$00,D0
0000493C                          OR.B      #$00,D0
00004940                          OR.B      #$00,D0
00004944                          OR.L      #$01000200,D0
0000494A                          Invalid
0000494C                          Invalid
0000494E                          Invalid
00004950                          OR.B      #$00,D0
00004954                          BTST      D0,D0
00004956                          AND.B     #$00,D0
0000495A                          Invalid
0000495C                          Invalid
0000495E                          Invalid
00004960                          OR.B      #$00,D0
00004964                          BCLR      D0,D0
00004966                          BTST      D1,D0
00004968                          ADD.B     #$00,D0
0000496C                          Invalid
0000496E                          Invalid
00004970                          OR.B      #$00,D0
00004974                          AND.B     #$00,D0
00004978                          BTST      #$00,D0
0000497C                          Invalid
0000497E                          Invalid
00004980                          OR.B      #$00,D0
00004984                          AND.L     #$05000A00,D0
0000498A                          Invalid
0000498C                          Invalid
0000498E                          Invalid
00004990                          OR.B      #$00,D0
00004994                          BTST      D1,D0
00004996                          ADD.B     #$00,D0
0000499A                          Invalid
0000499C                          Invalid
0000499E                          Invalid
000049A0                          OR.B      #$00,D0
000049A4                          BCLR      D1,D0
000049A6                          BTST      D3,D0
000049A8                          .B        #$00,D0
000049AC                          Invalid
000049AE                          Invalid
000049B0                          OR.B      #$00,D0
000049B4                          SUB.B     #$00,D0
000049B8                          MOVE.B    D0,D0
000049BA                          Invalid
000049BC                          Invalid
000049BE                          Invalid
000049C0                          OR.B      #$00,D0
000049C4                          SUB.L     #$09001200,D0
000049CA                          ASR.B     #7,D0
000049CC                          Invalid
000049CE                          Invalid
000049D0                          OR.B      #$00,D0
000049D4                          BTST      D2,D0
000049D6                          EOR.B     #$00,D0
000049DA                          ASR.B     #6,D0
000049DC                          Invalid
000049DE                          Invalid
000049E0                          OR.B      #$00,D0
000049E4                          BCLR      D2,D0
000049E6                          BTST      D5,D0
000049E8                          MOVE.B    D0,D3
000049EA                          ASR.B     #5,D0
000049EC                          Invalid
000049EE                          Invalid
000049F0                          OR.B      #$00,D0
000049F4                          ADD.B     #$00,D0
000049F8                          MOVE.B    D0,D4
000049FA                          ASR.B     #4,D0
000049FC                          Invalid
000049FE                          Invalid
00004A00                          OR.B      #$00,D0
00004A04                          ADD.L     #$0D001A00,D0
00004A0A                          ASR.B     #3,D0
00004A0C                          Invalid
00004A0E                          Invalid
00004A10                          OR.B      #$00,D0
00004A14                          BTST      D3,D0
00004A16                          .B        #$00,D0
00004A1A                          ASR.B     #2,D0
00004A1C                          Invalid
00004A1E                          Invalid
00004A20                          OR.B      #$00,D0
00004A24                          BCLR      D3,D0
00004A26                          BTST      D7,D0
00004A28                          MOVE.B    D0,D7
00004A2A                          ASR.B     #1,D0
00004A2C                          Invalid
00004A2E                          Invalid
00004A30                          JSR       $000014E6(pc)
00004A34                          LSL.W     #3,D0
00004A36                          MOVEQ     #$00,D1
00004A38                          MOVE.B    $0030(A0),D1
00004A3C                          ADD.W     D1,D1
00004A3E                          ADD.W     D1,D1
00004A40                          ADD.W     D0,D1
00004A42                          LEA       +$10(D6.W),A1
00004A46                          MOVE.W    (A1)+,D1
00004A48                          MOVE.W    D1,$0038(A0)
00004A4C                          LSL.L     #8,D1
00004A4E                          AND.L     #$00FFFF00,D1
00004A54                          MOVE.L    D1,$0044(A0)
00004A58                          MOVE.W    (A1),$003A(A0)
00004A5C                          LSR.W     #1,D0
00004A5E                          LEA       $00004D84(pc),A1
00004A62                          MOVE.L    $00(A1,D0.W),$0048(A0)
00004A68                          MOVE.B    #$01,$0036(A0)
00004A6E                          RTS
00004A70                          AND.B     #$00,D0
00004A74                          AND.B     #$00,D0
00004A78                          AND.B     #$FD,D0
00004A7C                          AND.B     #$FD,D0
00004A80                          BCLR      D0,D0
00004A82                          BCLR      D0,D0
00004A84                          AND.B     #$FD,$01(A3,D6.W)
00004A8A                          MOVEP.L   $0180(A4),D0
00004A8E                          BLS       $00004A8D
00004A90                          OR.L      #$00990100,(A1)+
00004A96                          BLS       $00004A95
00004A98                          BTST      D0,D0
00004A9A                          BLS       $00004A99
00004A9C                          OR.L      #$00990180,(A1)+
00004AA2                          BLS       $00004AA1
00004AA4                          MOVEP.L   $014C(A4),D0
00004AA8                          AND.B     #$FD,$01(A3,D0.W)
00004AAE                          BCLR      D0,D0
00004AB0                          MOVE.W    $0038(A0),D0
00004AB4                          MOVE.L    $0044(A0),D1
00004AB8                          CMP.B     #$02,$0036(A0)
00004ABE                          BEQ       $00004ACC
00004AC0                          CMP.W     $003A(A0),D0
00004AC4                          BGE       $00004AE2
00004AC6                          ADD.L     $0048(A0),D1
00004ACA                          BRA       $00004AD6
00004ACC                          CMP.W     $003A(A0),D0
00004AD0                          BLE       $00004AE2
00004AD2                          SUB.L     $0048(A0),D1
00004AD6                          MOVE.L    D1,$0044(A0)
00004ADA                          ROR.L     #8,D1
00004ADC                          MOVE.W    D1,$0038(A0)
00004AE0                          RTS
00004AE2                          MOVE.W    $003A(A0),$0038(A0)
00004AE8                          CLR.B     $0036(A0)
00004AEC                          RTS
00004AEE                          JSR       $00007F8E(pc)
00004AF2                          MOVEQ     #$05,D2
00004AF4                          CMP.W     #$0400,D1
00004AF8                          BGE       $00004B04
00004AFA                          AND.W     #$0380,D1
00004AFE                          LSR.W     #7,D1
00004B00                          MOVE.B    +$10(D1.L),D2
00004B04                          CMP.B     $005A(A0),D2
00004B08                          BEQ       $00004B12
00004B0A                          MOVE.B    D2,$005A(A0)
00004B0E                          JSR       $00007DDC(pc)
00004B12                          RTS
00004B14                          OR.B      #$00,D0
00004B18                          BTST      D0,D2
00004B1A                          BTST      D1,D4
00004B1C                          LEA       $00004B50(pc),A1
00004B20                          TST.B     $0030(A0)
00004B24                          BEQ       $00004B2A
00004B26                          LEA       $00004B90(pc),A1
00004B2A                          JSR       $0000153E(pc)
00004B2E                          8         D1,D0
00004B30                          LSL.W     #3,D0
00004B32                          ADD.W     D1,D0
00004B34                          MOVE.B    $00(A1,D0.W),D0
00004B38                          BEQ       $00004A30(pc)
00004B3C                          MOVE.W    +$00(D3.W),D0
00004B40                          JMP       +$00(D1.W)
00004B44                          OR.L      #$008E01FE,A4
00004B4A                          SUB.B     #$98,-(A2)
00004B4E                          AND.W     #$0008,-(A0)
00004B52                          EOR.B     #$08,A2
00004B56                          BTST      #$00,A0
00004B5A                          EOR.B     #$04,A2
00004B5E                          SUB.B     #$06,D4
00004B62                          OR.B      #$06,D6
00004B66                          ADD.B     #$06,D6
00004B6A                          ADD.B     #$06,D0
00004B6E                          ADD.B     #$06,D6
00004B72                          ADD.B     #$06,D6
00004B76                          ADD.B     #$08,D6
00004B7A                          EOR.B     #$00,A2
00004B7E                          BTST      #$08,A0
00004B82                          EOR.B     #$04,A2
00004B86                          OR.B      #$08,A0
00004B8A                          EOR.B     #$04,A2
00004B8E                          SUB.B     #$08,D0
00004B92                          BTST      #$0A,A0
00004B96                          EOR.B     #$00,A2
00004B9A                          SUB.B     #$0A,D4
00004B9E                          EOR.B     #$08,A2
00004BA2                          OR.B      #$0A,D4
00004BA6                          EOR.B     #$08,A2
00004BAA                          BTST      #$0A,D0
00004BAE                          EOR.B     #$08,A2
00004BB2                          BTST      #$0A,A0
00004BB6                          EOR.B     #$06,A2
00004BBA                          ADD.B     #$00,D6
00004BBE                          ADD.B     #$06,D6
00004BC2                          ADD.B     #$06,D6
00004BC6                          OR.B      #$06,D6
00004BCA                          ADD.B     #$06,D6
00004BCE                          ADD.B     #$75,D0
00004BD2                          TST.W     $0038(A0)
00004BD6                          BEQ       $00004A30(pc)
00004BDA                          RTS
00004BDC                          CMP.W     #$0280,$0038(A0)
00004BE2                          BLE       $00004D42(pc)
00004BE6                          JSR       $0000153E(pc)
00004BEA                          LSL.W     #3,D0
00004BEC                          ADD.W     D1,D0
00004BEE                          MOVE.B    +$00(D3.L),D1
00004BF2                          BEQ       $00004D42(pc)
00004BF6                          CMP.W     #$0400,$0038(A0)
00004BFC                          BLT       $00004C02
00004BFE                          ADD.W     #$000A,D1
00004C02                          LEA       +$10(D1.W),A1
00004C06                          MOVE.W    (A1)+,D0
00004C08                          (CCC     #$0,$000C(A0)
00004C0C                          MOVE.L    (A1)+,D0
00004C0E                          MOVE.W    (A1)+,$003C(A0)
00004C12                          MOVE.W    (A1)+,$003E(A0)
00004C16                          TST.B     $0030(A0)
00004C1A                          BEQ       $00004C1E
00004C1C                          NEG.L     D0
00004C1E                          MOVE.L    D0,$0044(A0)
00004C22                          BRA       $00004DF6(pc)
00004C26                          OR.B      #$00,D0
00004C2A                          OR.B      #$00,D0
00004C2E                          MOVE.B    D0,D2
00004C30                          MOVE.L    #$50503C28,D4
00004C36                          BCC       $00004CB0(pc)
00004C3A                          OR.B      D0,D6
00004C3C                          OR.B      #$00,D0
00004C40                          OR.B      #$00,D0
00004C44                          OR.B      #$00,D0
00004C48                          OR.B      #$00,D0
00004C4C                          OR.B      #$00,D0
00004C50                          OR.B      #$00,D0
00004C54                          OR.B      #$00,D0
00004C58                          OR.B      #$B4,D0
00004C5C                          OR.B      #$00,D0
00004C60                          OR.B      #$78,D0
00004C64                          OR.B      D0,D6
00004C66                          OR.B      #$02,D0
00004C6A                          OR.B      D0,D0
00004C6C                          OR.B      #$00,D0
00004C70                          OR.B      #$03,D0
00004C74                          OR.B      #$00,D0
00004C78                          OR.B      #$00,D0
00004C7C                          OR.B      #$00,D2
00004C80                          OR.B      #$63,D0
00004C84                          OR.B      #$03,D0
00004C88                          OR.B      #$00,D0
00004C8C                          OR.W      #$0002,(A6)+
00004C90                          OR.B      #$00,D2
00004C94                          OR.B      #$63,D0
00004C98                          OR.B      #$03,D5
00004C9C                          OR.B      #$00,D0
00004CA0                          OR.W      #$0007,(A6)+
00004CA4                          OR.B      #$00,D2
00004CA8                          OR.B      #$85,D0
00004CAC                          OR.B      #$03,A2
00004CB0                          OR.B      #$00,D0
00004CB4                          OR.L      #$000F0002,A7
00004CBA                          OR.B      D0,D0
00004CBC                          OR.B      #$C7,D0
00004CC0                          OR.B      #$03,(A1)
00004CC4                          OR.B      #$00,D0
00004CC8                          Invalid
00004CCA                          OR.B      #$02,D0
00004CCE                          OR.B      D0,D0
00004CD0                          Invalid
00004CD2                          OR.L      #$00000003,$00004CD4(pc)
00004CDA                          Invalid
00004CDC                          Invalid
00004CDE                          OR.B      #$02,A4
00004CE2                          OR.B      D0,D0
00004CE4                          Invalid
00004CE6                          OR.L      #$000F0003,$00004CE8(pc)
00004CEE                          Invalid
00004CF0                          Invalid
00004CF2                          OR.B      #$02,D2
00004CF6                          OR.B      D0,D0
00004CF8                          Invalid
00004CFA                          Invalid
00004CFC                          OR.B      #$03,D5
00004D00                          OR.B      #$C3,D0
00004D04                          Invalid
00004D06                          OR.B      #$02,D0
00004D0A                          OR.B      D0,D0
00004D0C                          Invalid
00004D0E                          OR.W      #$0000,-(A3)
00004D12                          OR.B      #$00,D3
00004D16                          Invalid
00004D18                          BTST      D0,(A6)+
00004D1A                          OR.B      #$02,D5
00004D1E                          OR.B      D0,D0
00004D20                          Invalid
00004D22                          OR.W      #$0007,-(A3)
00004D26                          OR.B      #$00,D3
00004D2A                          Invalid
00004D2C                          BTST      D0,(A6)+
00004D2E                          OR.B      #$02,D0
00004D32                          OR.B      D0,D0
00004D34                          Invalid
00004D36                          OR.W      #$0000,-(A3)
00004D3A                          OR.B      #$00,D3
00004D3E                          Invalid
00004D40                          BTST      D0,(A6)+
00004D42                          JSR       $000014E6(pc)
00004D46                          TST.B     D0
00004D48                          BEQ       $00007DF0(pc)
00004D4C                          MOVE.W    $0038(A0),D0
00004D50                          JSR       $00007FBE(pc)
00004D54                          MOVE.W    D1,$0038(A0)
00004D58                          LSL.L     #8,D1
00004D5A                          AND.L     #$00FFFF00,D1
00004D60                          MOVE.L    D1,$0044(A0)
00004D64                          MOVE.W    #$270F,$003A(A0)
00004D6A                          JSR       $000014E6(pc)
00004D6E                          ADD.W     D0,D0
00004D70                          ADD.W     D0,D0
00004D72                          MOVE.L    +$00(D0.L),D0
00004D76                          LSR.L     #1,D0
00004D78                          MOVE.L    D0,$0048(A0)
00004D7C                          MOVE.B    #$01,$0036(A0)
00004D82                          RTS
00004D84                          OR.B      #$00,D0
00004D88                          OR.B      #$00,D0
00004D8C                          OR.B      #$FC,D0
00004D90                          OR.B      #$95,D0
00004D94                          OR.B      #$8C,D0
00004D98                          OR.B      #$8C,D0
00004D9C                          OR.B      #$95,D0
00004DA0                          OR.B      #$FC,D0
00004DA4                          CMP.W     #$0280,$0038(A0)
00004DAA                          BLT       $00004E14(pc)
00004DAE                          JSR       $0000153E(pc)
00004DB2                          LSL.W     #3,D0
00004DB4                          ADD.B     D1,D0
00004DB6                          MOVE.B    +$00(D5.W),D1
00004DBA                          BEQ       $00004A30(pc)
00004DBE                          CMP.W     #$0400,$0038(A0)
00004DC4                          BLT       $00004DCA
00004DC6                          ADD.W     #$000A,D1
00004DCA                          LEA       $00004E62(pc),A1
00004DCE                          LEA       $10(A1,D0.W),A1
00004DD2                          MOVE.W    (A1)+,D0
00004DD4                          TST.B     $005B(A0)
00004DD8                          BEQ       $00004DDC
00004DDA                          NEG.W     D0
00004DDC                          (CCC     #$0,$000C(A0)
00004DE0                          MOVE.L    (A1)+,D0
00004DE2                          MOVE.W    (A1)+,$003C(A0)
00004DE6                          MOVE.W    (A1)+,$003E(A0)
00004DEA                          TST.B     $0030(A0)
00004DEE                          BEQ       $00004DF2
00004DF0                          NEG.L     D0
00004DF2                          MOVE.L    D0,$0044(A0)
00004DF6                          MOVE.B    #$0C,$0003(A0)
00004DFC                          MOVE.B    #$08,$0004(A0)
00004E02                          MOVE.B    $0030(A0),D0
00004E06                          AND.B     #$01,D0
00004E0A                          LEA       $00041192,A4
00004E10                          BRA       $00007E40(pc)
00004E14                          JSR       $000014E6(pc)
00004E18                          TST.B     D0
00004E1A                          BEQ       $00007DF0(pc)
00004E1E                          BRA       $00004A30(pc)
00004E22                          OR.B      #$00,D0
00004E26                          OR.B      #$00,D0
00004E2A                          OR.B      #$00,D0
00004E2E                          OR.B      #$00,D0
00004E32                          MOVE.B    (A4),D2
00004E34                          OR.B      #$28,D0
00004E38                          MOVE.W    (A0),A6
00004E3A                          BCC       $00004EA0
00004E3C                          OR.B      #$78,D0
00004E40                          OR.L      -(A0),D6
00004E42                          CMP.L     $00(A4,D0.W),D2
00004E46                          Invalid
00004E48                          ADD.W     $B4(A0,D2.L),A6
00004E4C                          Invalid
00004E4E                          AND.B     D0,D4
00004E50                          OR.B      #$64,D0
00004E54                          Invalid
00004E56                          MOVEQ     #$00,D4
00004E58                          OR.B      #$14,D0
00004E5C                          ADDQ.B    #$0,#$00
00004E60                          OR.B      #$00,D0
00004E64                          OR.B      #$00,D2
00004E68                          OR.B      #$00,D0
00004E6C                          OR.B      #$03,D0
00004E70                          OR.B      #$00,D0
00004E74                          OR.B      #$02,D0
00004E78                          OR.B      #$00,D2
00004E7C                          BTST      D0,D7
00004E7E                          OR.B      #$05,$0003(A0)
00004E84                          OR.B      #$3A,D0
00004E88                          OR.B      #$14,-(A6)
00004E8C                          OR.B      #$00,D2
00004E90                          BTST      D0,D7
00004E92                          OR.W      #$0019,(A6)+
00004E96                          OR.B      #$00,D3
00004E9A                          BTST      D0,$00004F0F(pc)
00004E9E                          OR.B      #$02,A4
00004EA2                          OR.B      D0,D0
00004EA4                          BTST      D0,D7
00004EA6                          OR.W      #$000F,D5
00004EAA                          OR.B      #$00,D3
00004EAE                          BTST      D0,$00004EFF(pc)
00004EB2                          OR.B      #$02,D7
00004EB6                          OR.B      D0,D0
00004EB8                          BTST      D0,D7
00004EBA                          OR.B      #$0A,$00004EBF(pc)
00004EC0                          OR.B      #$3A,D0
00004EC4                          OR.W      #$0005,D0
00004EC8                          OR.B      #$00,D2
00004ECC                          AND.W     #$0063,
00004ED0                          OR.B      #$03,D7
00004ED4                          OR.B      #$FD,D0
00004ED8                          OR.W      #$0011,(A6)+
00004EDC                          OR.B      #$00,D2
00004EE0                          AND.W     #$0075,
00004EE4                          OR.B      #$03,(A4)
00004EE8                          OR.B      #$FD,D0
00004EEC                          OR.L      #$000A0002,A7
00004EF2                          OR.B      D0,D0
00004EF4                          AND.W     #$0063,
00004EF8                          OR.B      #$03,A4
00004EFC                          OR.B      #$FD,D0
00004F00                          OR.W      #$0007,$00(A3,D1.W)
00004F06                          OR.B      D0,D0
00004F08                          AND.W     #$0063,
00004F0C                          OR.B      #$03,D7
00004F10                          OR.B      #$FD,D0
00004F14                          OR.W      #$0005,$0002(A3)
00004F1A                          OR.B      D0,D0
00004F1C                          ADD.B     #$F0,D7
00004F20                          OR.B      #$03,A2
00004F24                          OR.B      #$3A,D0
00004F28                          Invalid
00004F2A                          OR.B      #$02,A2
00004F2E                          OR.B      D0,D0
00004F30                          ADD.B     #$AE,D7
00004F34                          OR.B      #$03,(A1)
00004F38                          OR.B      #$3A,D0
00004F3C                          Invalid
00004F3E                          OR.B      #$02,D7
00004F42                          OR.B      D0,D0
00004F44                          ADD.B     #$A8,D7
00004F48                          OR.B      #$03,A7
00004F4C                          OR.B      #$3A,D0
00004F50                          Invalid
00004F52                          OR.B      #$02,D5
00004F56                          OR.B      D0,D0
00004F58                          ADD.B     #$C0,D7
00004F5C                          OR.B      #$03,A4
00004F60                          OR.B      #$3A,D0
00004F64                          Invalid
00004F66                          MOVE.W    $0038(A0),D0
00004F6A                          JSR       $00007FBE(pc)
00004F6E                          MOVE.W    D1,$0038(A0)
00004F72                          LSL.L     #8,D1
00004F74                          AND.L     #$00FFFF00,D1
00004F7A                          MOVE.L    D1,$0044(A0)
00004F7E                          JSR       $000014E6(pc)
00004F82                          ADD.W     D0,D0
00004F84                          MOVE.W    +$00(D4.L),$003A(A0)
00004F8A                          ADD.W     D0,D0
00004F8C                          LEA       $00004D84(pc),A1
00004F90                          MOVE.L    $00(A1,D0.W),$0048(A0)
00004F96                          MOVE.B    #$02,$0036(A0)
00004F9C                          RTS
00004F9E                          AND.B     #$00,D0
00004FA2                          MOVEP.L   D0,$014C(A4)
00004FA6                          OR.L      #$0099014C,(A1)+
00004FAC                          MOVEP.L   D0,$0028(A4)
00004FB0                          OR.W      #$0053,(A0)
00004FB4                          BCLR      #$05,$0053(A0)
00004FBA                          BNE       $0000503E(pc)
00004FBE                          BCLR      #$07,$0053(A0)
00004FC4                          BNE       $0000503E(pc)
00004FC8                          JSR       $00001C3C(pc)
00004FCC                          BCS       $00004FD4
00004FCE                          JSR       $00001C94(pc)
00004FD2                          BCC       $00004FD8
00004FD4                          ADD.W     Invalid
00004FD6                          OR.B      #$75,A0
00004FDA                          BSET      #$04,$0053(A0)
00004FE0                          BCLR      #$05,$0053(A0)
00004FE6                          BNE       $0000503E(pc)
00004FEA                          JSR       $00001C3C(pc)
00004FEE                          BCC       $0000500C
00004FF0                          CMP.B     #$01,$0024(A0)
00004FF6                          BNE       $0000500C
00004FF8                          BTST      #$03,D2
00004FFC                          BNE       $00005008
00004FFE                          CMP.B     #$03,$0016(A0)
00005004                          BNE       $0000503A(pc)
00005008                          ADD.W     Invalid
0000500A                          OR.B      #$75,A0
0000500E                          BSET      #$06,$0053(A0)
00005014                          BCLR      #$07,$0053(A0)
0000501A                          BNE       $0000503E(pc)
0000501E                          JSR       $00001C94(pc)
00005022                          BCC       $0000500C
00005024                          CMP.B     #$02,$0024(A0)
0000502A                          BNE       $0000500C
0000502C                          BTST      #$03,D2
00005030                          BNE       $00005008
00005032                          CMP.B     #$05,$0016(A0)
00005038                          BEQ       $00005008
0000503A                          ADD.W     Invalid
0000503C                          OR.B      #$7C,A0
00005040                          OR.B      #$03,D4
00005044                          JSR       $00007DC4(pc)
00005048                          MOVE.L    (A7)+,D0
0000504A                          RTS
0000504C                          TST.B     $E199
00005050                          BEQ       $00005084
00005052                          LEA       $000019D4,A1
00005058                          TST.B     $005B(A0)
0000505C                          BEQ       $00005064
0000505E                          LEA       $00001AAE,A1
00005064                          JSR       (A1)
00005066                          BCC       $00005084
00005068                          BTST      #$03,D2
0000506C                          BNE       $00005084
0000506E                          ADD.W     Invalid
00005070                          OR.B      #$7C,A4
00005074                          OR.B      #$4D,D1
00005078                          MOVE.B    #$08,$0003(A0)
0000507E                          JSR       $00007DC4(pc)
00005082                          MOVE.L    (A7)+,D0
00005084                          RTS
00005086                          BSET      #$00,$0053(A0)
0000508C                          TST.B     $005B(A0)
00005090                          BNE       $000050A2
00005092                          BCLR      #$01,$0053(A0)
00005098                          BNE       $000050BE
0000509A                          JSR       $00001AAE(pc)
0000509E                          BCS       $000050B0
000050A0                          RTS
000050A2                          BCLR      #$01,$0053(A0)
000050A8                          BNE       $000050B6
000050AA                          JSR       $000019D4(pc)
000050AE                          BCC       $000050C4
000050B0                          ADD.W     Invalid
000050B2                          OR.B      #$75,A4
000050B6                          MOVE.B    $0054(A0),$0023(A0)
000050BC                          RTS
000050BE                          MOVE.B    $0054(A0),$0022(A0)
000050C4                          RTS
000050C6                          CMP.B     #$01,$0034(A0)
000050CC                          BEQ       $000050FA
000050CE                          BGT       $000050FE
000050D0                          JSR       $00028A1C
000050D6                          BCS       $000050FE
000050D8                          MOVE.B    #$01,$0037(A0)
000050DE                          MOVE.B    #$00,$0006(A1)
000050E4                          MOVE.W    #$0100,$0000(A1)
000050EA                          MOVE.B    $0030(A0),$0030(A1)
000050F0                          MOVE.B    #$00,$0035(A0)
000050F6                          JSR       $00005134(pc)
000050FA                          ADDQ.B    #$1,$0034(A0)
000050FE                          RTS
00005100                          TST.B     $0034(A0)
00005104                          BEQ       $000050FE
00005106                          MOVE.B    $0026(A0),D0
0000510A                          AND.B     #$F0,D0
0000510E                          CMP.B     #$10,D0
00005112                          BEQ       $000050FE
00005114                          SUBQ.B    #$1,$0034(A0)
00005118                          BEQ       $000050FE
0000511A                          JSR       $00028A1C
00005120                          BCS       $0000512E
00005122                          MOVE.W    #$0100,$0000(A1)
00005128                          MOVE.B    $0030(A0),$0030(A1)
0000512E                          EOR.B     #$00,$0035(A0)
00005134                          TST.B     $0035(A0)
00005138                          BNE       $00005158
0000513A                          LEA       $000051BC(pc),A4
0000513E                          BTST      #$04,$0043(A0)
00005144                          BNE       $00005190
00005146                          LEA       $000051DC(pc),A4
0000514A                          BTST      #$01,$0043(A0)
00005150                          BEQ       $00005174
00005152                          LEA       $000051FC(pc),A4
00005156                          BRA       $00005174
00005158                          LEA       $0000521C(pc),A4
0000515C                          BTST      #$04,$0043(A0)
00005162                          BNE       $00005190
00005164                          LEA       $0000523C(pc),A4
00005168                          BTST      #$01,$0043(A0)
0000516E                          BEQ       $00005174
00005170                          LEA       $0000525C(pc),A4
00005174                          JSR       $000014E6(pc)
00005178                          AND.W     #$0007,D0
0000517C                          ADD.W     D0,D0
0000517E                          ADD.W     D0,D0
00005180                          MOVE.L    $00(A4,D0.W),A4
00005184                          MOVE.B    $0030(A0),D0
00005188                          AND.B     #$01,D0
0000518C                          BRA       $00007E40(pc)
00005190                          JSR       $000014E6(pc)
00005194                          AND.W     #$0007,D0
00005198                          ADD.W     D0,D0
0000519A                          ADD.W     D0,D0
0000519C                          MOVE.L    $00(A4,D0.W),A4
000051A0                          MOVE.B    $0026(A0),D0
000051A4                          AND.B     #$03,D0
000051A8                          ADD.B     D0,D0
000051AA                          ADD.B     D0,D0
000051AC                          LEA       $00(A4,D0.W),A4
000051B0                          MOVE.B    $0030(A0),D0
000051B4                          AND.B     #$01,D0
000051B8                          BRA       $00007E40(pc)
000051BC                          OR.B      #$4A,D4
000051C0                          OR.B      #$4A,D4
000051C4                          OR.B      #$9A,D4
000051C8                          OR.B      #$2A,D4
000051CC                          OR.B      #$6A,D4
000051D0                          OR.B      #$02,D4
000051D4                          OR.B      #$C2,D4
000051D8                          OR.B      #$5A,D4
000051DC                          OR.B      #$92,D4
000051E0                          OR.B      #$92,D4
000051E4                          OR.B      #$2A,D4
000051E8                          OR.B      #$3A,D4
000051EC                          OR.B      #$7A,D4
000051F0                          OR.B      #$92,D4
000051F4                          OR.B      #$52,D4
000051F8                          OR.B      #$42,D4
000051FC                          OR.B      #$BA,D4
00005200                          OR.B      #$BA,D4
00005204                          OR.B      #$7A,D4
00005208                          OR.B      #$3A,D4
0000520C                          OR.B      #$7A,D4
00005210                          OR.B      #$92,D4
00005214                          OR.B      #$52,D4
00005218                          OR.B      #$92,D4
0000521C                          OR.B      #$4A,D4
00005220                          OR.B      #$4A,D4
00005224                          OR.B      #$9A,D4
00005228                          OR.B      #$2A,D4
0000522C                          OR.B      #$6A,D4
00005230                          OR.B      #$02,D4
00005234                          OR.B      #$C2,D4
00005238                          OR.B      #$5A,D4
0000523C                          OR.B      #$92,D4
00005240                          OR.B      #$92,D4
00005244                          OR.B      #$2A,D4
00005248                          OR.B      #$3A,D4
0000524C                          OR.B      #$7A,D4
00005250                          OR.B      #$92,D4
00005254                          OR.B      #$52,D4
00005258                          OR.B      #$42,D4
0000525C                          OR.B      #$BA,D4
00005260                          OR.B      #$BA,D4
00005264                          OR.B      #$7A,D4
00005268                          OR.B      #$3A,D4
0000526C                          OR.B      #$7A,D4
00005270                          OR.B      #$92,D4
00005274                          OR.B      #$52,D4
00005278                          OR.B      #$92,D4
0000527C                          TST.B     $F0AE
00005280                          BNE       $0000528E
00005282                          MOVE.B    $001E(A0),D0
00005286                          CMP.B     $0056(A0),D0
0000528A                          BNE       $00007D58(pc)
0000528E                          TST.B     $0004(A0)
00005292                          BNE       $00005404(pc)
00005296                          BTST      #$04,$E109
0000529C                          BNE       $00005410(pc)
000052A0                          BTST      #$06,$E109
000052A6                          BNE       $00005FAE(pc)
000052AA                          BTST      #$05,$E109
000052B0                          BNE       $00005FAE(pc)
000052B4                          MOVE.B    $0016(A0),$0042(A0)
000052BA                          MOVEQ     #$00,D0
000052BC                          MOVE.B    $0024(A0),D0
000052C0                          BEQ       $00005348(pc)
000052C4                          SUBQ.B    #$1,D0
000052C6                          BNE       $00005382(pc)
000052CA                          BSET      #$04,$0053(A0)
000052D0                          JSR       $00007F1E(pc)
000052D4                          CMP.B     $0042(A0),D0
000052D8                          BEQ       $000052E2
000052DA                          JSR       $0000549A(pc)
000052DE                          BNE       $000054F2(pc)
000052E2                          MOVE.B    $0016(A0),D0
000052E6                          EXT.W     D0
000052E8                          ADD.W     D0,D0
000052EA                          MOVE.W    +$00(D3.W),D0
000052EE                          JMP       +$00(D1.W)
000052F2                          OR.B      #$12,(A2)
000052F6                          OR.B      #$22,$00(A2,D1.W)
000052FC                          OR.B      #$32,-(A2)
00005300                          OR.B      #$32,(A2)
00005304                          JSR       $0000554A(pc)
00005308                          JSR       $00005720(pc)
0000530C                          JSR       $00005566(pc)
00005310                          BRA       $00007E6C(pc)
00005314                          JSR       $00005552(pc)
00005318                          JSR       $00005790(pc)
0000531C                          JSR       $00005594(pc)
00005320                          BRA       $00007E6C(pc)
00005324                          BCLR      #$05,$0053(A0)
0000532A                          BNE       $00005344
0000532C                          JSR       $00001C3C(pc)
00005330                          BCC       $00005348
00005332                          CMP.B     #$01,$0024(A0)
00005338                          BNE       $00005348
0000533A                          BTST      #$03,D2
0000533E                          BNE       $00005348
00005340                          ADD.W     Invalid
00005342                          OR.B      #$75,A0
00005346                          MOVE.L    (A7)+,D0
00005348                          MOVE.B    #$0C,$0003(A0)
0000534E                          MOVE.B    #$0C,$0004(A0)
00005354                          MOVE.B    #$00,$0005(A0)
0000535A                          MOVE.W    #$0000,$003C(A0)
00005360                          MOVE.W    #$0030,$003E(A0)
00005366                          MOVE.L    #$00000000,$0044(A0)
0000536E                          MOVE.B    $0030(A0),D0
00005372                          AND.B     #$01,D0
00005376                          LEA       $00041192,A4
0000537C                          BRA       $00007E40(pc)
00005380                          RTS
00005382                          BSET      #$06,$0053(A0)
00005388                          JSR       $00007F1E(pc)
0000538C                          CMP.B     $0042(A0),D0
00005390                          BEQ       $0000539A
00005392                          JSR       $0000549A(pc)
00005396                          BNE       $000054F2(pc)
0000539A                          MOVE.B    $0016(A0),D0
0000539E                          EXT.W     D0
000053A0                          ADD.W     D0,D0
000053A2                          MOVE.W    +$00(D3.W),D0
000053A6                          JMP       +$00(D1.W)
000053AA                          OR.B      #$12,(A2)
000053AE                          OR.B      #$22,$00(A2,D1.W)
000053B4                          OR.B      #$32,-(A2)
000053B8                          OR.B      #$32,(A2)
000053BC                          JSR       $0000554A(pc)
000053C0                          JSR       $0000574E(pc)
000053C4                          JSR       $000055BC(pc)
000053C8                          BRA       $00007E6C(pc)
000053CC                          JSR       $00005552(pc)
000053D0                          JSR       $000057C2(pc)
000053D4                          JSR       $000055EA(pc)
000053D8                          BRA       $00007E6C(pc)
000053DC                          BCLR      #$07,$0053(A0)
000053E2                          BNE       $00005402
000053E4                          JSR       $00001C94(pc)
000053E8                          BCC       $00005348(pc)
000053EC                          CMP.B     #$02,$0024(A0)
000053F2                          BNE       $00005348(pc)
000053F6                          BTST      #$03,D2
000053FA                          BNE       $00005348(pc)
000053FE                          ADD.W     Invalid
00005400                          OR.B      #$75,A0
00005404                          SUBQ.B    #$1,$0041(A0)
00005408                          BNE       $00007E6C(pc)
0000540C                          BRA       $00007DD6(pc)
00005410                          MOVE.B    #$00,$0030(A0)
00005416                          MOVE.B    $0024(A0),D1
0000541A                          BEQ       $0000542E
0000541C                          JSR       $00007F1E(pc)
00005420                          AND.W     #$0003,D1
00005424                          LSL.W     #4,D1
00005426                          ADD.W     D1,D0
00005428                          MOVE.B    +$00(D0.W),$0030(A0)
0000542E                          JSR       $00028A1C
00005434                          BCS       $0000544E
00005436                          MOVE.B    #$01,$0037(A0)
0000543C                          MOVE.B    #$00,$0006(A0)
00005442                          MOVE.W    #$0100,$0000(A1)
00005448                          MOVE.B    $0030(A0),$0030(A1)
0000544E                          MOVE.B    #$02,$0004(A0)
00005454                          MOVE.B    #$0E,$0041(A0)
0000545A                          LEA       $00040ECE,A4
00005460                          CMP.B     #$01,$0024(A0)
00005466                          BLS       $0000546E
00005468                          LEA       $00040EE6,A4
0000546E                          MOVE.B    $0030(A0),D0
00005472                          AND.B     #$01,D0
00005476                          BRA       $00007E40(pc)
0000547A                          OR.B      #$00,D0
0000547E                          OR.B      #$01,D1
00005482                          OR.B      #$00,D0
00005486                          OR.B      #$00,D0
0000548A                          BTST      D0,D0
0000548C                          OR.B      #$01,D0
00005490                          BTST      D0,D1
00005492                          BTST      D0,D0
00005494                          OR.B      #$00,D0
00005498                          OR.B      #$00,D0
0000549C                          MOVE.B    $0024(A0),D1
000054A0                          BEQ       $000054B6
000054A2                          LSL.W     #4,D1
000054A4                          ADD.B     D0,D1
000054A6                          MOVE.B    +$10(D5.W),$0030(A0)
000054AC                          MOVE.B    +$00(D2.L),D0
000054B0                          CMP.B     $0033(A0),D0
000054B4                          RTS
000054B6                          CLR.B     $0030(A0)
000054BA                          CLR.B     D0
000054BC                          CMP.B     $0033(A0),D0
000054C0                          RTS
000054C2                          OR.B      #$00,D0
000054C6                          OR.B      #$00,D0
000054CA                          OR.B      #$00,D0
000054CE                          OR.B      #$00,D0
000054D2                          BTST      D0,D1
000054D4                          OR.B      #$01,D1
000054D8                          BTST      D0,D1
000054DA                          BTST      D0,D0
000054DC                          OR.B      #$00,D0
000054E0                          OR.B      #$00,D0
000054E4                          BTST      D1,D1
000054E6                          BTST      D0,D1
000054E8                          BTST      D1,D0
000054EA                          AND.B     #$00,D0
000054EE                          OR.B      #$00,D0
000054F2                          MOVE.B    D0,$0033(A0)
000054F6                          ADD.W     D0,D0
000054F8                          ADD.W     D0,D0
000054FA                          AND.W     #$000F,D0
000054FE                          MOVE.B    $0024(A0),D1
00005502                          AND.W     #$0003,D1
00005506                          LSL.W     #4,D1
00005508                          ADD.W     D1,D0
0000550A                          MOVE.L    +$00(D7.W),A4
0000550E                          MOVE.B    $0030(A0),D0
00005512                          AND.B     #$01,D0
00005516                          BRA       $00007E40(pc)
0000551A                          OR.B      #$FE,D4
0000551E                          OR.B      #$14,D4
00005522                          OR.B      #$AE,D4
00005526                          OR.B      #$AE,D4
0000552A                          OR.B      #$FE,D4
0000552E                          OR.B      #$14,D4
00005532                          OR.B      #$AE,D4
00005536                          OR.B      #$AE,D4
0000553A                          OR.B      #$2A,D4
0000553E                          OR.B      #$40,D4
00005542                          OR.B      #$BE,D4
00005546                          OR.B      #$BE,D4
0000554A                          MOVE.L    #$FFFF0000,D0
00005550                          BRA       $00005558
00005552                          MOVE.L    #$00010000,D0
00005558                          TST.B     $005B(A0)
0000555C                          BEQ       $00005560
0000555E                          NEG.L     D0
00005560                          ADD.L     Invalid
00005562                          OR.B      #$75,A4
00005566                          BCLR      #$05,$0053(A0)
0000556C                          BNE       $00005592
0000556E                          JSR       $00001C3C(pc)
00005572                          BCC       $00005612(pc)
00005576                          CMP.B     #$01,$0024(A0)
0000557C                          BNE       $00005612(pc)
00005580                          BTST      #$03,D2
00005584                          BNE       $00005632(pc)
00005588                          ADD.W     Invalid
0000558A                          OR.B      #$1F,A0
0000558E                          BRA       $00007E6C(pc)
00005592                          RTS
00005594                          BCLR      #$05,$0053(A0)
0000559A                          BNE       $000055BA
0000559C                          JSR       $00001C3C(pc)
000055A0                          BCC       $00005678(pc)
000055A4                          CMP.B     #$01,$0024(A0)
000055AA                          BNE       $00005678(pc)
000055AE                          BTST      #$03,D2
000055B2                          BNE       $00005346(pc)
000055B6                          ADD.W     Invalid
000055B8                          OR.B      #$75,A0
000055BC                          BCLR      #$07,$0053(A0)
000055C2                          BNE       $000055E8
000055C4                          JSR       $00001C94(pc)
000055C8                          BCC       $000056AA(pc)
000055CC                          CMP.B     #$02,$0024(A0)
000055D2                          BNE       $000056AA(pc)
000055D6                          BTST      #$03,D2
000055DA                          BNE       $000056CC(pc)
000055DE                          ADD.W     Invalid
000055E0                          OR.B      #$1F,A0
000055E4                          BRA       $00007E6C(pc)
000055E8                          RTS
000055EA                          BCLR      #$07,$0053(A0)
000055F0                          BNE       $00005610
000055F2                          JSR       $00001C94(pc)
000055F6                          BCC       $000056DA(pc)
000055FA                          CMP.B     #$02,$0024(A0)
00005600                          BNE       $000056DA(pc)
00005604                          BTST      #$03,D2
00005608                          BNE       $00005346(pc)
0000560C                          ADD.W     Invalid
0000560E                          OR.B      #$75,A0
00005612                          TST.B     $005B(A0)
00005616                          BNE       $00005626
00005618                          BSR       $0000565C
0000561A                          JSR       $00001AAE(pc)
0000561E                          BCS       $00005714(pc)
00005622                          BSR       $0000566A
00005624                          BRA       $00005632
00005626                          BSR       $00005640
00005628                          JSR       $000019D4(pc)
0000562C                          BCS       $00005714(pc)
00005630                          BSR       $0000564E
00005632                          JSR       $00005552(pc)
00005636                          MOVE.B    #$01,$0024(A0)
0000563C                          MOVE.L    (A7)+,D0
0000563E                          RTS
00005640                          ADD.W     #$0020,$000C(A0)
00005646                          ADD.W     #$000A,$0008(A0)
0000564C                          RTS
0000564E                          SUB.W     #$0020,$000C(A0)
00005654                          SUB.W     #$000A,$0008(A0)
0000565A                          RTS
0000565C                          SUB.W     #$0020,$000C(A0)
00005662                          ADD.W     #$000A,$0008(A0)
00005668                          RTS
0000566A                          ADD.W     #$0020,$000C(A0)
00005670                          SUB.W     #$000A,$0008(A0)
00005676                          RTS
00005678                          TST.B     $005B(A0)
0000567C                          BNE       $00005694
0000567E                          BSR       $00005640
00005680                          JSR       $000019D4(pc)
00005684                          BCC       $0000568E
00005686                          ADD.W     Invalid
00005688                          OR.B      #$00,A4
0000568C                          Invalid
0000568E                          BSR       $0000564E
00005690                          BRA       $00005346(pc)
00005694                          BSR       $0000565C
00005696                          JSR       $00001AAE(pc)
0000569A                          BCC       $000056A4
0000569C                          ADD.W     Invalid
0000569E                          OR.B      #$00,A4
000056A2                          Invalid
000056A4                          BSR       $0000566A
000056A6                          BRA       $00005346(pc)
000056AA                          TST.B     $005B(A0)
000056AE                          BNE       $000056BE
000056B0                          BSR       $0000564E
000056B2                          JSR       $00001AAE(pc)
000056B6                          BCS       $00005714(pc)
000056BA                          BSR       $00005640
000056BC                          BRA       $000056CC
000056BE                          BSR       $0000566A
000056C0                          JSR       $000019D4(pc)
000056C4                          BCS       $00005714(pc)
000056C8                          JSR       $0000565C(pc)
000056CC                          JSR       $00005552(pc)
000056D0                          MOVE.B    #$02,$0024(A0)
000056D6                          MOVE.L    (A7)+,D0
000056D8                          RTS
000056DA                          TST.B     $005B(A0)
000056DE                          BNE       $000056FA
000056E0                          JSR       $0000566A(pc)
000056E4                          JSR       $000019D4(pc)
000056E8                          BCC       $000056F2
000056EA                          ADD.W     Invalid
000056EC                          OR.B      #$00,A4
000056F0                          OR.L      #$4EBAFF68,A4
000056F6                          BRA       $00005346(pc)
000056FA                          JSR       $0000564E(pc)
000056FE                          JSR       $00001AAE(pc)
00005702                          BCC       $0000570C
00005704                          ADD.W     Invalid
00005706                          OR.B      #$00,A4
0000570A                          OR.W      #$4EBA,$FF(A2,D1.L)
00005710                          BRA       $00005346(pc)
00005714                          ADD.W     Invalid
00005716                          OR.B      #$BA,A4
0000571A                          MOVE.L    -(A2),(A3)
0000571C                          MOVE.L    (A7)+,D0
0000571E                          RTS
00005720                          LEA       $000019D4,A1
00005726                          TST.B     $005B(A0)
0000572A                          BEQ       $00005732
0000572C                          LEA       $00001AAE,A1
00005732                          JSR       (A1)
00005734                          BCC       $0000574C
00005736                          ADD.W     Invalid
00005738                          OR.B      #$02,A4
0000573C                          OR.B      #$0C,D3
00005740                          CMP.B     #$01,$0016(A0)
00005746                          BNE       $0000577C(pc)
0000574A                          MOVE.L    (A7)+,D0
0000574C                          RTS
0000574E                          LEA       $000019D4,A1
00005754                          TST.B     $005B(A0)
00005758                          BEQ       $00005760
0000575A                          LEA       $00001AAE,A1
00005760                          JSR       (A1)
00005762                          BCC       $0000577A
00005764                          ADD.W     Invalid
00005766                          OR.B      #$02,A4
0000576A                          OR.B      #$0C,D3
0000576E                          CMP.B     #$07,$0016(A0)
00005774                          BNE       $0000577C(pc)
00005778                          MOVE.L    (A7)+,D0
0000577A                          RTS
0000577C                          MOVE.B    #$01,$004D(A0)
00005782                          MOVE.B    #$08,$0003(A0)
00005788                          JSR       $00007DD0(pc)
0000578C                          MOVE.L    (A7)+,D0
0000578E                          RTS
00005790                          LEA       $00001AAE,A1
00005796                          TST.B     $005B(A0)
0000579A                          BEQ       $000057A2
0000579C                          LEA       $000019D4,A1
000057A2                          JSR       (A1)
000057A4                          BCC       $000057B8
000057A6                          CMP.B     #$03,$0016(A0)
000057AC                          BEQ       $000057BA
000057AE                          ADD.W     Invalid
000057B0                          OR.B      #$BA,A4
000057B4                          MOVE.L    A0,D3
000057B6                          MOVE.L    (A7)+,D0
000057B8                          RTS
000057BA                          ADD.W     Invalid
000057BC                          OR.B      #$1F,A4
000057C0                          RTS
000057C2                          LEA       $00001AAE,A1
000057C8                          TST.B     $005B(A0)
000057CC                          BEQ       $000057D4
000057CE                          LEA       $000019D4,A1
000057D4                          JSR       (A1)
000057D6                          BCC       $000057EA
000057D8                          CMP.B     #$05,$0016(A0)
000057DE                          BEQ       $000057EC
000057E0                          ADD.W     Invalid
000057E2                          OR.B      #$BA,A4
000057E6                          MOVE.L    (A6),$00007807(pc)
000057EA                          RTS
000057EC                          ADD.W     Invalid
000057EE                          OR.B      #$1F,A4
000057F2                          RTS
000057F4                          TST.B     $F0AE
000057F8                          BNE       $00005806
000057FA                          MOVE.B    $001E(A0),D0
000057FE                          CMP.B     $0056(A0),D0
00005802                          BNE       $00007D54(pc)
00005806                          TST.B     $0004(A0)
0000580A                          BNE       $000059A8(pc)
0000580E                          BTST      #$04,$E109
00005814                          BNE       $000059D2(pc)
00005818                          BTST      #$06,$E109
0000581E                          BNE       $00005F26(pc)
00005822                          BTST      #$05,$E109
00005828                          BNE       $00005F26(pc)
0000582C                          TST.B     $005B(A0)
00005830                          BNE       $00005838
00005832                          BSET      #$02,$0053(A0)
00005838                          JSR       $00005E7C(pc)
0000583C                          MOVE.B    $0023(A0),$0032(A0)
00005842                          MOVE.B    $0022(A0),$0031(A0)
00005848                          MOVE.B    $0016(A0),$0042(A0)
0000584E                          JSR       $00007F1E(pc)
00005852                          CMP.B     $0042(A0),D0
00005856                          BEQ       $00005868
00005858                          JSR       $00005A64(pc)
0000585C                          JSR       $00005AF8(pc)
00005860                          CMP.B     $0033(A0),D0
00005864                          BNE       $00005B52(pc)
00005868                          JSR       $00001512(pc)
0000586C                          LSL.W     #4,D0
0000586E                          ADD.B     $0016(A0),D0
00005872                          MOVE.B    +$00(D3.L),D0
00005876                          ADD.W     D0,D0
00005878                          MOVE.W    +$00(D3.W),D0
0000587C                          JMP       +$00(D1.W)
00005880                          OR.L      #$008A00A6,$00(A4,D7.L)
00005888                          OR.L      #$04040404,$00005C8E(pc)
00005890                          SUB.B     #$00,D4
00005894                          OR.B      #$00,D0
00005898                          OR.B      #$03,D0
0000589C                          BTST      D0,D4
0000589E                          SUB.B     #$03,D4
000058A2                          OR.B      #$00,D0
000058A6                          OR.B      #$00,D0
000058AA                          BTST      D1,D0
000058AC                          BTST      D0,D1
000058AE                          SUB.B     #$02,D4
000058B2                          OR.B      #$00,D0
000058B6                          OR.B      #$00,D0
000058BA                          AND.B     #$01,D0
000058BE                          SUB.B     #$02,D4
000058C2                          OR.B      #$00,D0
000058C6                          OR.B      #$00,D0
000058CA                          AND.B     #$01,D0
000058CE                          SUB.B     #$02,D4
000058D2                          OR.B      #$00,D0
000058D6                          OR.B      #$00,D0
000058DA                          BTST      D0,D1
000058DC                          BTST      D0,D4
000058DE                          SUB.B     #$00,D2
000058E2                          OR.B      #$00,D0
000058E6                          OR.B      #$00,D0
000058EA                          BTST      D0,D1
000058EC                          BTST      D0,D4
000058EE                          SUB.B     #$00,D2
000058F2                          OR.B      #$00,D0
000058F6                          OR.B      #$00,D0
000058FA                          BTST      D1,D1
000058FC                          BTST      D0,D4
000058FE                          SUB.B     #$00,D2
00005902                          OR.B      #$00,D0
00005906                          OR.B      #$00,D0
0000590A                          JSR       $00005C14(pc)
0000590E                          JSR       $00007E6C(pc)
00005912                          JSR       $00005C88(pc)
00005916                          JSR       $00005CCA(pc)
0000591A                          JSR       $00005D2C(pc)
0000591E                          JSR       $00005CEC(pc)
00005922                          BRA       $00005C06(pc)
00005926                          JSR       $00005C14(pc)
0000592A                          JSR       $00007E6C(pc)
0000592E                          JSR       $00005CA4(pc)
00005932                          BRA       $00005916
00005934                          JSR       $00004FC8(pc)
00005938                          BRA       $00005916
0000593A                          MOVE.B    #$0C,$0003(A0)
00005940                          MOVE.B    #$06,$0004(A0)
00005946                          TST.B     $004D(A0)
0000594A                          BNE       $00005958
0000594C                          MOVE.B    #$12,$0004(A0)
00005952                          MOVE.B    #$10,$0041(A0)
00005958                          MOVEQ     #$00,D7
0000595A                          MOVE.B    D7,$0005(A0)
0000595E                          MOVE.L    D7,$0044(A0)
00005962                          MOVE.W    D7,$003C(A0)
00005966                          MOVE.W    #$0030,$003E(A0)
0000596C                          MOVE.B    $0030(A0),D0
00005970                          AND.B     #$01,D0
00005974                          LEA       $00041192,A4
0000597A                          BRA       $00007E40(pc)
0000597E                          TST.B     $004D(A0)
00005982                          BEQ       $0000598C
00005984                          CMP.B     #$02,$004D(A0)
0000598A                          BNE       $000059A6
0000598C                          MOVEQ     #$00,D7
0000598E                          MOVE.B    D7,$0026(A0)
00005992                          MOVE.B    D7,$0004(A0)
00005996                          MOVE.B    D7,$0005(A0)
0000599A                          MOVE.B    #$01,$0011(A0)
000059A0                          MOVE.B    #$18,$0003(A0)
000059A6                          RTS
000059A8                          TST.B     $004D(A0)
000059AC                          BNE       $000059C6
000059AE                          BSET      #$02,$0053(A0)
000059B4                          BCLR      #$03,$0053(A0)
000059BA                          BEQ       $000059C6
000059BC                          MOVE.B    $0054(A0),$0022(A0)
000059C2                          JSR       $00005E7C(pc)
000059C6                          SUBQ.B    #$1,$0041(A0)
000059CA                          BNE       $00007E6C(pc)
000059CE                          BRA       $00007DD6(pc)
000059D2                          TST.B     $004D(A0)
000059D6                          BNE       $000059F6
000059D8                          TST.B     $005B(A0)
000059DC                          BNE       $000059F2
000059DE                          BSET      #$02,$0053(A0)
000059E4                          BCLR      #$03,$0053(A0)
000059EA                          BEQ       $000059F6
000059EC                          MOVE.B    $0054(A0),$0022(A0)
000059F2                          JSR       $00005E7C(pc)
000059F6                          JSR       $00028A1C
000059FC                          BCS       $00005A16
000059FE                          MOVE.B    #$01,$0037(A0)
00005A04                          MOVE.B    #$00,$0006(A0)
00005A0A                          MOVE.W    #$0100,$0000(A1)
00005A10                          MOVE.B    $0030(A0),$0030(A1)
00005A16                          MOVE.B    #$02,$0004(A0)
00005A1C                          MOVE.B    #$0E,$0041(A0)
00005A22                          JSR       $00005A32(pc)
00005A26                          MOVE.B    $0030(A0),D0
00005A2A                          AND.B     #$01,D0
00005A2E                          BRA       $00007E40(pc)
00005A32                          JSR       $00001512(pc)
00005A36                          AND.W     #$0007,D0
00005A3A                          ADD.W     D0,D0
00005A3C                          ADD.W     D0,D0
00005A3E                          MOVE.L    +$00(D2.W),A4
00005A42                          RTS
00005A44                          OR.B      #$66,D4
00005A48                          OR.B      #$66,D4
00005A4C                          OR.B      #$B6,D4
00005A50                          OR.B      #$56,D4
00005A54                          OR.B      #$F0,D4
00005A58                          OR.B      #$3A,D4
00005A5C                          OR.B      #$A6,D4
00005A60                          OR.B      #$06,D4
00005A64                          JSR       $00001528(pc)
00005A68                          LSL.W     #4,D1
00005A6A                          ADD.B     D0,D1
00005A6C                          MOVE.B    +$10(D5.W),D1
00005A70                          BMI       $00005A76
00005A72                          MOVE.B    D1,$0030(A0)
00005A76                          RTS
00005A78                          Invalid
00005A7A                          Invalid
00005A7C                          Invalid
00005A7E                          Invalid
00005A80                          Invalid
00005A82                          Invalid
00005A84                          Invalid
00005A86                          Invalid
00005A88                          Invalid
00005A8A                          Invalid
00005A8C                          Invalid
00005A8E                          BTST      D0,D1
00005A90                          Invalid
00005A92                          OR.B      #$00,D0
00005A96                          OR.B      #$FF,D0
00005A9A                          OR.B      #$FF,D0
00005A9E                          BTST      D0,D1
00005AA0                          Invalid
00005AA2                          OR.B      #$00,D0
00005AA6                          OR.B      #$FF,D0
00005AAA                          OR.B      #$FF,D0
00005AAE                          BTST      D0,D1
00005AB0                          Invalid
00005AB2                          OR.B      #$00,D0
00005AB6                          OR.B      #$FF,D0
00005ABA                          OR.B      #$FF,D0
00005ABE                          BTST      D0,D1
00005AC0                          Invalid
00005AC2                          OR.B      #$00,D0
00005AC6                          OR.B      #$00,D0
00005ACA                          Invalid
00005ACC                          Invalid
00005ACE                          BSET      D0,
00005AD0                          Invalid
00005AD2                          OR.B      #$00,D0
00005AD6                          OR.B      #$00,D0
00005ADA                          Invalid
00005ADC                          Invalid
00005ADE                          BSET      D0,
00005AE0                          Invalid
00005AE2                          OR.B      #$00,D0
00005AE6                          OR.B      #$00,D0
00005AEA                          Invalid
00005AEC                          Invalid
00005AEE                          BSET      D0,
00005AF0                          Invalid
00005AF2                          OR.B      #$00,D0
00005AF6                          OR.B      #$28,D0
00005AFA                          OR.B      #$16,A0
00005AFE                          BLT       $00005B04
00005B00                          MOVEQ     #$00,D0
00005B02                          RTS
00005B04                          JSR       $00001528(pc)
00005B08                          LSL.W     #3,D1
00005B0A                          ADD.W     D0,D1
00005B0C                          MOVE.B    +$10(D2.W),D0
00005B10                          RTS
00005B12                          OR.B      #$00,D0
00005B16                          OR.B      #$00,D0
00005B1A                          OR.B      #$00,D3
00005B1E                          OR.B      #$04,D0
00005B22                          AND.B     #$01,D0
00005B26                          OR.B      #$02,D0
00005B2A                          AND.B     #$01,D0
00005B2E                          OR.B      #$02,D0
00005B32                          AND.B     #$01,D0
00005B36                          OR.B      #$02,D0
00005B3A                          BTST      D0,D1
00005B3C                          BTST      D0,D0
00005B3E                          OR.B      #$00,D2
00005B42                          BTST      D0,D1
00005B44                          BTST      D0,D0
00005B46                          OR.B      #$00,D2
00005B4A                          BTST      D0,D1
00005B4C                          BTST      D0,D0
00005B4E                          OR.B      #$00,D2
00005B52                          MOVE.B    D0,$0033(A0)
00005B56                          JSR       $00001528(pc)
00005B5A                          AND.W     #$0007,D1
00005B5E                          ADD.W     D1,D1
00005B60                          ADD.W     D1,D1
00005B62                          MOVE.L    +$10(D5.L),A4
00005B66                          AND.W     #$0003,D0
00005B6A                          ADD.W     D0,D0
00005B6C                          ADD.W     D0,D0
00005B6E                          MOVE.L    $00(A4,D0.W),A4
00005B72                          MOVE.B    $0030(A0),D0
00005B76                          AND.B     #$01,D0
00005B7A                          BRA       $00007E40(pc)
00005B7E                          OR.B      #$9E,D0
00005B82                          OR.B      #$AA,D0
00005B86                          OR.B      #$BE,D0
00005B8A                          OR.B      #$CA,D0
00005B8E                          OR.B      #$D6,D0
00005B92                          OR.B      #$E2,D0
00005B96                          OR.B      #$EE,D0
00005B9A                          OR.B      #$FA,D0
00005B9E                          OR.B      #$56,D4
00005BA2                          OR.B      #$7E,D4
00005BA6                          OR.B      #$7E,D4
00005BAA                          OR.B      #$56,D4
00005BAE                          OR.B      #$7E,D4
00005BB2                          OR.B      #$7E,D4
00005BB6                          OR.B      #$56,D4
00005BBA                          OR.B      #$56,D4
00005BBE                          OR.B      #$A6,D4
00005BC2                          OR.B      #$CE,D4
00005BC6                          OR.B      #$CE,D4
00005BCA                          OR.B      #$46,D4
00005BCE                          OR.B      #$6E,D4
00005BD2                          OR.B      #$6E,D4
00005BD6                          OR.B      #$E6,D4
00005BDA                          OR.B      #$08,D4
00005BDE                          OR.B      #$08,D4
00005BE2                          OR.B      #$30,D4
00005BE6                          OR.B      #$52,D4
00005BEA                          OR.B      #$52,D4
00005BEE                          OR.B      #$96,D4
00005BF2                          OR.B      #$BE,D4
00005BF6                          OR.B      #$BE,D4
00005BFA                          OR.B      #$F6,D4
00005BFE                          OR.B      #$1E,D4
00005C02                          OR.B      #$1E,D4
00005C06                          MOVE.B    $0023(A0),D0
00005C0A                          CMP.B     $0032(A0),D0
00005C0E                          BNE       $00007DDC(pc)
00005C12                          RTS
00005C14                          JSR       $00001512(pc)
00005C18                          MOVEQ     #$00,D1
00005C1A                          MOVE.B    $0030(A0),D1
00005C1E                          LSL.W     #3,D0
00005C20                          ADD.W     D1,D1
00005C22                          ADD.W     D1,D1
00005C24                          ADD.W     D1,D0
00005C26                          MOVE.W    +$00(D0.W),D1
00005C2A                          MOVE.W    +$00(D7.L),D2
00005C2E                          SWAP      D1
00005C30                          ASR.L     #8,D1
00005C32                          ADD.L     Invalid
00005C34                          OR.B      #$42,A0
00005C38                          ASR.L     #8,D2
00005C3A                          TST.B     $005B(A0)
00005C3E                          BEQ       $00005C42
00005C40                          NEG.W     D2
00005C42                          (CCC     #$2,$000C(A0)
00005C46                          RTS
00005C48                          AND.B     #$00,D0
00005C4C                          Invalid
00005C4E                          OR.B      #$00,D0
00005C52                          OR.B      #$00,D0
00005C56                          OR.B      #$80,D0
00005C5A                          Invalid
00005C5C                          Invalid
00005C5E                          Invalid
00005C60                          BTST      D0,D0
00005C62                          Invalid
00005C64                          Invalid
00005C66                          BTST      D0,D0
00005C68                          BTST      D0,D0
00005C6A                          Invalid
00005C6C                          Invalid
00005C6E                          AND.B     #$99,D0
00005C72                          AND.B     #$00,D0
00005C76                          Invalid
00005C78                          MOVEP.L   $0100(A4),D0
00005C7C                          Invalid
00005C7E                          Invalid
00005C80                          BCLR      D0,D0
00005C82                          Invalid
00005C84                          Invalid
00005C86                          Invalid
00005C88                          JSR       $00001C3C(pc)
00005C8C                          BCC       $00005CA2
00005C8E                          CMP.B     #$01,$0024(A0)
00005C94                          BNE       $00005CA2
00005C96                          BTST      #$03,D2
00005C9A                          BEQ       $00005CB8(pc)
00005C9E                          ADD.W     Invalid
00005CA0                          OR.B      #$75,A0
00005CA4                          JSR       $00001C94(pc)
00005CA8                          BCC       $00005CA2
00005CAA                          CMP.B     #$02,$0024(A0)
00005CB0                          BNE       $00005CA2
00005CB2                          BTST      #$03,D2
00005CB6                          BNE       $00005C9E
00005CB8                          ADD.W     Invalid
00005CBA                          OR.B      #$7C,A0
00005CBE                          OR.B      #$03,D4
00005CC2                          JSR       $00007DD6(pc)
00005CC6                          MOVE.L    (A7)+,D0
00005CC8                          RTS
00005CCA                          LEA       $00001AE2,A1
00005CD0                          TST.B     $005B(A0)
00005CD4                          BEQ       $00005CDC
00005CD6                          LEA       $000019F8,A1
00005CDC                          JSR       (A1)
00005CDE                          BCC       $00005CEA
00005CE0                          ADD.W     Invalid
00005CE2                          OR.B      #$BA,A4
00005CE6                          MOVE.L    (A6),(A0)+
00005CE8                          MOVE.L    (A7)+,D0
00005CEA                          RTS
00005CEC                          TST.B     $E19A
00005CF0                          BEQ       $00005D2A
00005CF2                          TST.B     $004D(A0)
00005CF6                          BNE       $00005D2A
00005CF8                          LEA       $000019D4,A1
00005CFE                          TST.B     $005B(A0)
00005D02                          BEQ       $00005D0A
00005D04                          LEA       $00001AAE,A1
00005D0A                          JSR       (A1)
00005D0C                          BCC       $00005D2A
00005D0E                          BTST      #$03,D2
00005D12                          BNE       $00005D2A
00005D14                          ADD.W     Invalid
00005D16                          OR.B      #$7C,A4
00005D1A                          OR.B      #$4D,D1
00005D1E                          MOVE.B    #$08,$0003(A0)
00005D24                          JSR       $00007DC4(pc)
00005D28                          MOVE.L    (A7)+,D0
00005D2A                          RTS
00005D2C                          TST.B     $004D(A0)
00005D30                          BNE       $00005D6E
00005D32                          LEA       $000019F0,A1
00005D38                          TST.B     $005B(A0)
00005D3C                          BNE       $00005D5A
00005D3E                          BSET      #$02,$0053(A0)
00005D44                          BCLR      #$03,$0053(A0)
00005D4A                          BEQ       $00005D54
00005D4C                          MOVE.B    $0054(A0),$0022(A0)
00005D52                          BRA       $00005D6A
00005D54                          LEA       $00001D78,A1
00005D5A                          JSR       (A1)
00005D5C                          BCC       $00005D6A
00005D5E                          BTST      #$03,D2
00005D62                          BNE       $0000593A(pc)
00005D66                          ADD.W     Invalid
00005D68                          OR.B      #$00,A4
00005D6C                          BTST      D0,(A0)
00005D6E                          LEA       $00001AAE,A1
00005D74                          TST.B     $005B(A0)
00005D78                          BNE       $00005D8E
00005D7A                          BSET      #$02,$0053(A0)
00005D80                          BCLR      #$03,$0053(A0)
00005D86                          BNE       $00005DB0
00005D88                          LEA       $000019D4,A1
00005D8E                          JSR       (A1)
00005D90                          BCC       $00005DB2
00005D92                          BTST      #$03,D2
00005D96                          BNE       $0000593A(pc)
00005D9A                          ADD.W     Invalid
00005D9C                          OR.B      #$7C,A4
00005DA0                          OR.B      #$4D,D1
00005DA4                          BTST      #$06,D2
00005DA8                          BEQ       $00005DB0
00005DAA                          MOVE.B    #$02,$004D(A0)
00005DB0                          RTS
00005DB2                          TST.B     $0030(A0)
00005DB6                          BEQ       $00005E1A
00005DB8                          TST.B     $005B(A0)
00005DBC                          BNE       $00005DEC
00005DBE                          SUBQ.W    #$4,$0008(A0)
00005DC2                          SUB.W     #$0030,$000C(A0)
00005DC8                          JSR       $00001C3C(pc)
00005DCC                          BCC       $00005DDE
00005DCE                          CMP.B     #$01,$0024(A0)
00005DD4                          BNE       $00005DDE
00005DD6                          BTST      #$03,D2
00005DDA                          BEQ       $00005CB8(pc)
00005DDE                          ADDQ.W    #$4,$0008(A0)
00005DE2                          ADD.W     #$0030,$000C(A0)
00005DE8                          BRA       $0000593A(pc)
00005DEC                          SUBQ.W    #$4,$0008(A0)
00005DF0                          ADD.W     #$0030,$000C(A0)
00005DF6                          JSR       $00001C3C(pc)
00005DFA                          BCC       $00005E0C
00005DFC                          CMP.B     #$01,$0024(A0)
00005E02                          BNE       $00005E0C
00005E04                          BTST      #$03,D2
00005E08                          BEQ       $00005CB8(pc)
00005E0C                          ADDQ.W    #$4,$0008(A0)
00005E10                          SUB.W     #$0030,$000C(A0)
00005E16                          BRA       $0000593A(pc)
00005E1A                          TST.B     $005B(A0)
00005E1E                          BNE       $00005E4E
00005E20                          ADDQ.W    #$4,$0008(A0)
00005E24                          SUB.W     #$0030,$000C(A0)
00005E2A                          JSR       $00001C94(pc)
00005E2E                          BCC       $00005E40
00005E30                          CMP.B     #$02,$0024(A0)
00005E36                          BNE       $00005E40
00005E38                          BTST      #$03,D2
00005E3C                          BEQ       $00005CB8(pc)
00005E40                          SUBQ.W    #$4,$0008(A0)
00005E44                          ADD.W     #$0030,$000C(A0)
00005E4A                          BRA       $0000593A(pc)
00005E4E                          ADDQ.W    #$4,$0008(A0)
00005E52                          ADD.W     #$0030,$000C(A0)
00005E58                          JSR       $00001C94(pc)
00005E5C                          BCC       $00005E6E
00005E5E                          CMP.B     #$02,$0024(A0)
00005E64                          BNE       $00005E6E
00005E66                          BTST      #$03,D2
00005E6A                          BEQ       $00005CB8(pc)
00005E6E                          SUBQ.W    #$4,$0008(A0)
00005E72                          SUB.W     #$0030,$000C(A0)
00005E78                          BRA       $0000593A(pc)
00005E7C                          TST.B     $004D(A0)
00005E80                          BNE       $00005EA0
00005E82                          MOVEQ     #$00,D1
00005E84                          TST.B     $005B(A0)
00005E88                          BNE       $00005E96
00005E8A                          MOVE.B    $0022(A0),D1
00005E8E                          MOVE.B    +$10(D1.L),$0023(A0)
00005E94                          RTS
00005E96                          MOVE.B    $0023(A0),D1
00005E9A                          MOVE.B    +$10(D3.W),$0022(A0)
00005EA0                          RTS
00005EA2                          OR.B      #$06,D1
00005EA6                          BTST      D2,D4
00005EA8                          BTST      D1,D2
00005EAA                          MOVE.B    #$20,$0052(A0)
00005EB0                          JSR       $00007F1E(pc)
00005EB4                          JSR       $00006782(pc)
00005EB8                          LSL.W     #3,D0
00005EBA                          JSR       $000014FC(pc)
00005EBE                          ADD.W     D1,D0
00005EC0                          MOVE.B    +$00(D5.W),D1
00005EC4                          MOVE.W    +$10(D7.W),D1
00005EC8                          JMP       +$10(D5.W)
00005ECC                          OR.B      #$04,D2
00005ED0                          SUB.B     #$02,D4
00005ED4                          OR.B      #$08,D6
00005ED8                          EOR.B     #$06,D6
00005EDC                          OR.B      #$08,D6
00005EE0                          EOR.B     #$06,D6
00005EE4                          OR.B      #$08,A4
00005EE8                          EOR.B     #$10,D6
00005EEC                          OR.B      #$04,A4
00005EF0                          SUB.B     #$0C,D4
00005EF4                          OR.B      #$06,A4
00005EF8                          ADD.B     #$0E,A2
00005EFC                          OR.B      #$06,D6
00005F00                          ADD.B     #$06,A2
00005F04                          OR.B      #$06,D6
00005F08                          ADD.B     #$06,A2
00005F0C                          OR.B      #$04,D2
00005F10                          SUB.B     #$02,D4
00005F14                          BCHG      D0,$0180
00005F18                          AND.B     #$CC,$0386
00005F1E                          BSET      D1,$045A(A6)
00005F22                          SUB.W     #$045A,(A2)+
00005F26                          JSR       $00007F1E(pc)
00005F2A                          TST.B     $004D(A0)
00005F2E                          BEQ       $00005F94(pc)
00005F32                          LSL.W     #3,D0
00005F34                          JSR       $00001528(pc)
00005F38                          ADD.W     D1,D0
00005F3A                          MOVE.B    +$00(D0.L),D1
00005F3E                          BEQ       $00006018(pc)
00005F42                          SUBQ.B    #$1,D1
00005F44                          BEQ       $0000601A(pc)
00005F48                          BRA       $0000602A(pc)
00005F4C                          OR.B      #$00,D0
00005F50                          OR.B      #$01,D0
00005F54                          OR.B      #$00,D1
00005F58                          OR.B      #$01,D1
00005F5C                          OR.B      #$00,D1
00005F60                          OR.B      #$01,D1
00005F64                          OR.B      #$00,D1
00005F68                          OR.B      #$01,D1
00005F6C                          OR.B      #$00,D0
00005F70                          OR.B      #$01,D0
00005F74                          OR.B      #$02,D2
00005F78                          AND.B     #$01,D0
00005F7C                          OR.B      #$02,D2
00005F80                          AND.B     #$01,D0
00005F84                          OR.B      #$02,D2
00005F88                          AND.B     #$01,D0
00005F8C                          OR.B      #$02,D0
00005F90                          AND.B     #$01,D1
00005F94                          MOVE.B    +$00(D7.W),D0
00005F98                          BMI       $00006094(pc)
00005F9C                          MOVE.B    D0,$0030(A0)
00005FA0                          BRA       $000061E4(pc)
00005FA4                          Invalid
00005FA6                          OR.B      #$01,D0
00005FAA                          BTST      D0,D1
00005FAC                          Invalid
00005FAE                          JSR       $00007F1E(pc)
00005FB2                          LSL.W     #2,D0
00005FB4                          MOVEQ     #$00,D1
00005FB6                          MOVE.B    $0024(A0),D1
00005FBA                          ADD.W     D1,D0
00005FBC                          MOVE.B    +$00(D3.L),D1
00005FC0                          BEQ       $00006076(pc)
00005FC4                          SUBQ.B    #$1,D1
00005FC6                          BEQ       $0000606C(pc)
00005FCA                          SUBQ.B    #$1,D1
00005FCC                          BEQ       $00006078(pc)
00005FD0                          BRA       $00006082(pc)
00005FD4                          OR.B      #$00,D0
00005FD8                          OR.B      #$00,D0
00005FDC                          OR.B      #$00,D0
00005FE0                          OR.B      #$00,D1
00005FE4                          OR.B      #$00,D1
00005FE8                          OR.B      #$00,D2
00005FEC                          OR.B      #$00,D2
00005FF0                          OR.B      #$00,D2
00005FF4                          OR.B      #$00,D0
00005FF8                          MOVEQ     #$00,D1
00005FFA                          MOVE.B    $0022(A0),D1
00005FFE                          ADD.W     D1,D1
00006000                          MOVE.W    +$10(D3.W),D1
00006004                          JMP       +$10(D1.W)
00006008                          OR.L      #$008C008C,D4
0000600E                          MOVEP.L   $0148(A0),D0
00006012                          MOVEP.L   $0148(A0),D0
00006016                          OR.L      #$4E75117C,A4
0000601C                          OR.B      #$30,D0
00006020                          MOVE.L    #$0002AAA6,$0044(A0)
00006028                          BRA       $00006038
0000602A                          MOVE.B    #$01,$0030(A0)
00006030                          MOVE.L    #$FFFD555A,$0044(A0)
00006038                          MOVE.W    #$0000,$003C(A0)
0000603E                          MOVE.W    #$0028,$003E(A0)
00006044                          MOVE.B    #$0C,$0003(A0)
0000604A                          MOVE.B    #$04,$0004(A0)
00006050                          MOVE.B    #$1E,$0041(A0)
00006056                          CLR.B     $0034(A0)
0000605A                          MOVE.B    $0030(A0),D0
0000605E                          AND.B     #$01,D0
00006062                          LEA       $00041192,A4
00006068                          BRA       $00007E40(pc)
0000606C                          JSR       $0000608C(pc)
00006070                          MOVE.B    #$06,$0004(A0)
00006076                          RTS
00006078                          MOVE.B    #$01,$0030(A0)
0000607E                          BRA       $000061E4(pc)
00006082                          MOVE.B    #$00,$0030(A0)
00006088                          BRA       $000061E4(pc)
0000608C                          BRA       $00007DF8(pc)
00006090                          JSR       $00006BD2(pc)
00006094                          MOVEQ     #$00,D0
00006096                          MOVE.B    $0055(A0),D0
0000609A                          ADD.W     D0,D0
0000609C                          MOVE.W    +$00(D6.W),$003C(A0)
000060A2                          MOVE.W    #$0030,$003E(A0)
000060A8                          MOVE.B    #$0C,$0003(A0)
000060AE                          MOVE.B    #$00,$0004(A0)
000060B4                          MOVE.B    #$3C,$0041(A0)
000060BA                          CLR.B     $0034(A0)
000060BE                          MOVE.B    $0030(A0),D0
000060C2                          AND.B     #$01,D0
000060C6                          LEA       $0004117A,A4
000060CC                          JSR       $00007E40(pc)
000060D0                          MOVE.W    $0038(A0),D0
000060D4                          MOVEQ     #$30,D1
000060D6                          CMP.W     #$0300,D0
000060DA                          BGE       $000060E4
000060DC                          LSR.W     #4,D0
000060DE                          AND.W     #$0018,D0
000060E2                          MOVE.W    D0,D1
000060E4                          MOVEQ     #$00,D0
000060E6                          MOVE.W    D0,$0038(A0)
000060EA                          MOVE.B    D0,$0036(A0)
000060EE                          MOVE.B    $0030(A0),D0
000060F2                          ADD.W     D0,D0
000060F4                          ADD.W     D0,D0
000060F6                          ADD.W     D0,D1
000060F8                          MOVE.L    +$10(D5.L),$0044(A0)
000060FE                          MOVE.B    #$01,$0043(A0)
00006104                          JMP       $00020148
0000610A                          BSET      D2,$08(A0,D1.W)
0000610E                          BTST      #$99,$0705(A6)
00006114                          OR.B      #$00,D0
00006118                          OR.B      #$00,D0
0000611C                          OR.B      #$00,D0
00006120                          OR.B      #$00,D0
00006124                          OR.B      #$00,D0
00006128                          OR.B      #$00,D0
0000612C                          OR.B      #$00,D0
00006130                          OR.B      #$00,D0
00006134                          OR.B      #$3F,D0
00006138                          Invalid
0000613A                          CMP.L     D1,A5
0000613C                          OR.B      #$86,D0
00006140                          Invalid
00006142                          MOVEQ     #$7A,D3
00006144                          OR.B      #$CC,D0
00006148                          Invalid
0000614A                          MOVE.W    $4E(A4,D5.L),-(A1)
0000614E                          EOR.L     #$70001028,D4
00006154                          OR.W      #$D040,(A5)
00006158                          MOVE.W    +$00(D6.W),$003C(A0)
0000615E                          MOVE.W    #$0030,$003E(A0)
00006164                          MOVE.B    #$0C,$0003(A0)
0000616A                          MOVE.B    #$00,$0004(A0)
00006170                          MOVE.B    #$3C,$0041(A0)
00006176                          CLR.B     $0034(A0)
0000617A                          MOVE.B    $0030(A0),D0
0000617E                          AND.B     #$01,D0
00006182                          LEA       $0004117A,A4
00006188                          JSR       $00007E40(pc)
0000618C                          MOVE.L    #$00000000,$0044(A0)
00006194                          MOVE.W    $0038(A0),D0
00006198                          BEQ       $000061B2
0000619A                          MOVEQ     #$00,D1
0000619C                          SUB.W     #$0200,D0
000061A0                          BMI       $000061A4
000061A2                          MOVEQ     #$08,D1
000061A4                          TST.B     $0030(A0)
000061A8                          BEQ       $000061AC
000061AA                          ADDQ.W    #$4,D1
000061AC                          MOVE.L    +$10(D1.W),$0044(A0)
000061B2                          CLR.W     $0038(A0)
000061B6                          CLR.B     $0036(A0)
000061BA                          MOVE.B    #$01,$0043(A0)
000061C0                          JMP       $00020148
000061C6                          BSET      D2,(A4)+
000061C8                          BTST      #$99,$0705(A6)
000061CE                          ADD.W     #$0001,$11(A0,D6.W)
000061D4                          Invalid
000061D6                          CMP       $00(A4,A0.W)
000061DA                          (CCC     #$4,(A1)+
000061DC                          Invalid
000061DE                          BNE       $00006247
000061E0                          JSR       $00006BD2(pc)
000061E4                          MOVE.B    #$20,$0052(A0)
000061EA                          MOVEQ     #$00,D0
000061EC                          MOVE.B    $0055(A0),D0
000061F0                          ADD.W     D0,D0
000061F2                          MOVE.W    +$00(D2.W),$003C(A0)
000061F8                          MOVE.W    #$0028,$003E(A0)
000061FE                          MOVE.B    #$0C,$0003(A0)
00006204                          MOVE.B    #$00,$0004(A0)
0000620A                          MOVE.B    #$3C,$0041(A0)
00006210                          CLR.B     $0034(A0)
00006214                          MOVE.B    $0030(A0),D0
00006218                          LEA       $0004119A,A4
0000621E                          AND.B     #$01,D0
00006222                          JSR       $00007E40(pc)
00006226                          MOVE.W    $0038(A0),D0
0000622A                          MOVEQ     #$30,D1
0000622C                          CMP.W     #$0300,D0
00006230                          BGE       $0000623A
00006232                          LSR.W     #4,D0
00006234                          AND.W     #$0018,D0
00006238                          MOVE.W    D0,D1
0000623A                          MOVEQ     #$00,D0
0000623C                          MOVE.B    $0030(A0),D0
00006240                          ADD.W     D0,D0
00006242                          ADD.W     D0,D0
00006244                          ADD.W     D0,D1
00006246                          MOVE.L    +$10(D5.L),$0044(A0)
0000624C                          MOVE.B    #$04,$0043(A0)
00006252                          JMP       $00020148
00006258                          Invalid
0000625A                          ADD.L     #$063D05C2,$0547
00006262                          OR.B      #$A6,D2
00006266                          Invalid
00006268                          SUBQ.W    #$2,(A2)+
0000626A                          OR.B      #$A6,D2
0000626E                          Invalid
00006270                          SUBQ.W    #$2,(A2)+
00006272                          OR.B      #$A6,D2
00006276                          Invalid
00006278                          SUBQ.W    #$2,(A2)+
0000627A                          OR.B      #$A6,D2
0000627E                          Invalid
00006280                          SUBQ.W    #$2,(A2)+
00006282                          OR.B      #$33,D3
00006286                          Invalid
00006288                          MULU      A5,D6
0000628A                          OR.B      #$D3,D3
0000628E                          Invalid
00006290                          NEG.W     D7
00006292                          OR.B      #$3F,D4
00006296                          Invalid
00006298                          CMP.L     D1,A5
0000629A                          JSR       $00006BD2(pc)
0000629E                          MOVE.B    #$20,$0052(A0)
000062A4                          MOVEQ     #$00,D0
000062A6                          MOVE.B    $0055(A0),D0
000062AA                          ADD.W     D0,D0
000062AC                          MOVE.W    +$00(D5.W),$003C(A0)
000062B2                          MOVE.W    #$0028,$003E(A0)
000062B8                          MOVE.B    #$0C,$0003(A0)
000062BE                          MOVE.B    #$00,$0004(A0)
000062C4                          MOVE.B    #$3C,$0041(A0)
000062CA                          CLR.B     $0034(A0)
000062CE                          MOVE.B    $0030(A0),D0
000062D2                          LEA       $0004117A,A4
000062D8                          AND.B     #$01,D0
000062DC                          JSR       $00007E40(pc)
000062E0                          MOVE.L    #$00026666,$0044(A0)
000062E8                          TST.B     $0030(A0)
000062EC                          BEQ       $000062F6
000062EE                          MOVE.L    #$FFFD999A,$0044(A0)
000062F6                          MOVE.B    #$01,$0043(A0)
000062FC                          JMP       $00020148
00006302                          JSR       $00006BD2(pc)
00006306                          MOVEQ     #$00,D0
00006308                          MOVE.B    $0055(A0),D0
0000630C                          ADD.W     D0,D0
0000630E                          MOVE.W    +$00(D2.L),$003C(A0)
00006314                          MOVE.W    #$0028,$003E(A0)
0000631A                          MOVE.B    #$0C,$0003(A0)
00006320                          MOVE.B    #$00,$0004(A0)
00006326                          MOVE.B    #$3C,$0041(A0)
0000632C                          CLR.B     $0034(A0)
00006330                          MOVE.B    $0030(A0),D0
00006334                          AND.B     #$01,D0
00006338                          LEA       $0004117A,A4
0000633E                          JSR       $00007E40(pc)
00006342                          MOVE.L    #$0001110C,$0044(A0)
0000634A                          TST.B     $0030(A0)
0000634E                          BEQ       $00006358
00006350                          MOVE.L    #$FFFEEEF4,$0044(A0)
00006358                          MOVE.B    #$01,$0043(A0)
0000635E                          JMP       $00020148
00006364                          Invalid
00006366                          ADD.L     #$063D05C2,$0547
0000636E                          CMP.B     #$2C,$0003(A0)
00006374                          BEQ       $000063A0
00006376                          BSET      #$00,$0053(A0)
0000637C                          MOVE.B    #$08,$0043(A0)
00006382                          MOVE.B    #$01,$0011(A0)
00006388                          MOVE.B    #$00,$0026(A0)
0000638E                          MOVE.B    #$18,$0003(A0)
00006394                          MOVE.B    #$02,$0004(A0)
0000639A                          JMP       $00020170
000063A0                          RTS
000063A2                          BSR       $000063CE
000063A4                          MOVEQ     #$00,D0
000063A6                          MOVE.B    $0004(A0),D0
000063AA                          MOVE.W    +$00(D3.W),D0
000063AE                          JMP       +$00(D1.W)
000063B2                          OR.W      #$00AE,$000064B0(pc)
000063B8                          BTST      D0,$01(A2,D4.W)
000063BC                          BCLR      D0,#$01DC
000063C0                          AND.B     #$54,(A2)+
000063C4                          BCHG      D0,(A0)+
000063C6                          MOVEP.W   $0202(A0),D1
000063CA                          BTST      D1,-(A4)
000063CC                          MOVEP.L   D1,$4A38(A0)
000063D0                          Invalid
000063D2                          BNE       $0000642A
000063D4                          MOVE.B    $001E(A0),D0
000063D8                          CMP.B     $0056(A0),D0
000063DC                          BEQ       $0000642A
000063DE                          CMP.B     #$16,$0004(A0)
000063E4                          BEQ       $0000642A
000063E6                          CMP.B     #$1A,$0004(A0)
000063EC                          BEQ       $0000642A
000063EE                          JSR       $000201A2
000063F4                          JSR       $000015B6
000063FA                          ADDQ.B    #$1,$FC4E
000063FE                          MOVE.B    $FC27,D0
00006402                          CMP.B     $FC4E,D0
00006406                          BLE       $00007D96(pc)
0000640A                          MOVE.W    #$0096,$FC4C
00006410                          MOVE.B    #$03,$0006(A0)
00006416                          MOVE.B    #$1C,$0003(A0)
0000641C                          MOVE.B    #$00,$0004(A0)
00006422                          MOVE.B    #$00,$0005(A0)
00006428                          MOVE.L    (A7)+,D0
0000642A                          RTS
0000642C                          JSR       $00007F64(pc)
00006430                          JSR       $00007E6C(pc)
00006434                          JSR       $00006C6E(pc)
00006438                          JSR       $00006D28(pc)
0000643C                          JSR       $000068E8(pc)
00006440                          JSR       $000067D0(pc)
00006444                          JSR       $00006B54(pc)
00006448                          JSR       $00006C20(pc)
0000644C                          CMP.W     #$0000,$003C(A0)
00006452                          BGT       $0000645A
00006454                          MOVE.B    #$02,$0004(A0)
0000645A                          SUBQ.B    #$1,$0041(A0)
0000645E                          RTS
00006460                          JSR       $00007F64(pc)
00006464                          JSR       $00007E6C(pc)
00006468                          JSR       $00006C6E(pc)
0000646C                          JSR       $00006D28(pc)
00006470                          JSR       $0000694E(pc)
00006474                          JSR       $000067D0(pc)
00006478                          JSR       $000069FA(pc)
0000647C                          JSR       $00006B54(pc)
00006480                          JSR       $00006C20(pc)
00006484                          SUBQ.B    #$1,$0041(A0)
00006488                          BPL       $000064AC
0000648A                          MOVE.B    #$04,$0004(A0)
00006490                          MOVE.B    #$01,$0043(A0)
00006496                          CLR.B     $0034(A0)
0000649A                          MOVE.B    $0030(A0),D0
0000649E                          LEA       $00041192,A4
000064A4                          AND.B     #$01,D0
000064A8                          BRA       $00007E40(pc)
000064AC                          RTS
000064AE                          JSR       $000067A6(pc)
000064B2                          JSR       $00007F64(pc)
000064B6                          JSR       $00007E6C(pc)
000064BA                          JSR       $00006C6E(pc)
000064BE                          JSR       $00006D28(pc)
000064C2                          JSR       $0000694E(pc)
000064C6                          JSR       $000067D0(pc)
000064CA                          JSR       $000069FA(pc)
000064CE                          JSR       $00006B54(pc)
000064D2                          JSR       $00006C20(pc)
000064D6                          TST.W     $0044(A0)
000064DA                          BNE       $000064E2
000064DC                          MOVE.B    #$06,$0004(A0)
000064E2                          RTS
000064E4                          MOVE.B    #$01,$0043(A0)
000064EA                          JSR       $00007F66(pc)
000064EE                          JSR       $00007E6C(pc)
000064F2                          JSR       $00006C6E(pc)
000064F6                          JSR       $00006D28(pc)
000064FA                          JSR       $0000694E(pc)
000064FE                          JSR       $000067EA(pc)
00006502                          JSR       $000069FA(pc)
00006506                          BRA       $00006C20(pc)
0000650A                          MOVE.B    #$01,$0043(A0)
00006510                          JSR       $00007F66(pc)
00006514                          JSR       $00007E6C(pc)
00006518                          JSR       $00006C6E(pc)
0000651C                          JSR       $00006D28(pc)
00006520                          JSR       $000067EA(pc)
00006524                          JSR       $000069FA(pc)
00006528                          JSR       $00006C20(pc)
0000652C                          SUBQ.B    #$1,$0041(A0)
00006530                          BNE       $00006538
00006532                          MOVE.B    #$06,$0004(A0)
00006538                          RTS
0000653A                          JSR       $00007F64(pc)
0000653E                          JSR       $00007E6C(pc)
00006542                          JSR       $00006C6E(pc)
00006546                          JSR       $00006D28(pc)
0000654A                          JSR       $000067D0(pc)
0000654E                          JSR       $000068E8(pc)
00006552                          JSR       $00006B54(pc)
00006556                          JSR       $00006C20(pc)
0000655A                          CMP.W     #$0000,$003C(A0)
00006560                          BGT       $00006568
00006562                          MOVE.B    #$0A,$0004(A0)
00006568                          SUBQ.B    #$1,$0041(A0)
0000656C                          RTS
0000656E                          JSR       $00007F64(pc)
00006572                          JSR       $00007E6C(pc)
00006576                          JSR       $00006C6E(pc)
0000657A                          JSR       $00006D28(pc)
0000657E                          JSR       $000067D0(pc)
00006582                          JSR       $00006AA2(pc)
00006586                          JSR       $00006B54(pc)
0000658A                          BRA       $00006C20(pc)
0000658E                          MOVE.B    #$01,$0043(A0)
00006594                          JSR       $00007F66(pc)
00006598                          JSR       $00007E6C(pc)
0000659C                          JSR       $00006C6E(pc)
000065A0                          JSR       $00006D28(pc)
000065A4                          JSR       $0000680C(pc)
000065A8                          JSR       $0000694E(pc)
000065AC                          JSR       $000069FA(pc)
000065B0                          BRA       $00006C20(pc)
000065B4                          JSR       $00007F64(pc)
000065B8                          JSR       $00007E6C(pc)
000065BC                          JSR       $000067D0(pc)
000065C0                          JSR       $00006AFA(pc)
000065C4                          JSR       $00006B54(pc)
000065C8                          BRA       $00006C20(pc)
000065CC                          BSET      #$00,$0053(A0)
000065D2                          BTST      #$06,$E109
000065D8                          BNE       $00005EAA(pc)
000065DC                          BTST      #$05,$E109
000065E2                          BNE       $00005EAA(pc)
000065E6                          SUBQ.B    #$1,$0041(A0)
000065EA                          BNE       $00006604
000065EC                          JSR       $00007DB4(pc)
000065F0                          CMP.B     #$01,$0036(A0)
000065F6                          BNE       $000065FC
000065F8                          BRA       $00004D42(pc)
000065FC                          CLR.B     $0036(A0)
00006600                          CLR.W     $0038(A0)
00006604                          RTS
00006606                          TST.B     $005B(A0)
0000660A                          BNE       $00006612
0000660C                          BSET      #$00,$0053(A0)
00006612                          SUBQ.B    #$1,$0011(A0)
00006616                          BNE       $00006668
00006618                          MOVE.B    $0026(A0),D0
0000661C                          ADDQ.B    #$1,D0
0000661E                          AND.W     #$0007,D0
00006622                          ADD.W     D0,D0
00006624                          MOVE.W    D0,D1
00006626                          ADD.W     D0,D0
00006628                          ADD.W     D0,D0
0000662A                          ADD.W     D1,D0
0000662C                          LEA       $0000666A(pc),A1
00006630                          LEA       $00(A1,D0.W),A1
00006634                          MOVE.B    (A1)+,$0011(A0)
00006638                          BMI       $0000689A(pc)
0000663C                          MOVE.B    (A1)+,$0026(A0)
00006640                          MOVE.W    (A1)+,D0
00006642                          ADD.W     Invalid
00006644                          OR.B      #$19,A0
00006648                          TST.B     $005B(A0)
0000664C                          BEQ       $00006650
0000664E                          NEG.W     D0
00006650                          ADD.W     Invalid
00006652                          OR.B      #$51,A4
00006656                          MOVE.B    $0030(A0),D0
0000665A                          EXT.W     D0
0000665C                          ADD.W     D0,D0
0000665E                          MOVE.W    $00(A4,D0.W),D0
00006662                          MOVE.W    $00(A6,D0.W),$0012(A0)
00006668                          RTS
0000666A                          OR.B      (A0),D0
0000666C                          OR.B      #$00,D0
00006670                          OR.B      #$9E,D4
00006674                          AND.B     #$00,(A0)
00006678                          OR.B      #$04,D0
0000667C                          MOVE.B    (A6)+,$02(A0,A0.L)
00006680                          OR.B      #$00,D0
00006684                          OR.B      #$A2,D4
00006688                          AND.B     #$00,(A2)
0000668C                          OR.B      #$04,D0
00006690                          MOVE.B    -(A6),$02(A0,A2.L)
00006694                          OR.B      #$00,D0
00006698                          OR.B      #$AE,D4
0000669C                          AND.B     #$00,(A6)
000066A0                          OR.B      #$04,D0
000066A4                          MOVE.B    $02(A2,A3.L),$00(A0,D0.W)
000066AA                          OR.B      #$04,D0
000066AE                          MOVE.B    $80(A6,A3.L),$00(A0,D0.W)
000066B4                          OR.B      #$04,D0
000066B8                          MOVE.B    $4A(A6,D4.W),$00(A0,A5.L)
000066BE                          BNE       $000066C6
000066C0                          BSET      #$00,$0053(A0)
000066C6                          SUBQ.B    #$1,$0011(A0)
000066CA                          BNE       $000066D4
000066CC                          MOVE.B    $0026(A0),D0
000066D0                          BRA       $0000661E(pc)
000066D4                          RTS
000066D6                          TST.B     $005B(A0)
000066DA                          BNE       $000066E2
000066DC                          BSET      #$00,$0053(A0)
000066E2                          SUBQ.B    #$1,$0011(A0)
000066E6                          BNE       $0000671A
000066E8                          ADDQ.B    #$1,$0026(A0)
000066EC                          MOVE.B    $0026(A0),D0
000066F0                          AND.W     #$0007,D0
000066F4                          CMP.B     #$06,D0
000066F8                          BHI       $0000671A
000066FA                          ADD.W     D0,D0
000066FC                          ADD.W     D0,D0
000066FE                          MOVE.L    +$00(D5.W),A4
00006702                          MOVE.B    $0030(A0),D0
00006706                          EXT.W     D0
00006708                          ADD.W     D0,D0
0000670A                          MOVE.W    $00(A4,D0.W),D0
0000670E                          MOVE.W    $00(A6,D0.W),$0012(A0)
00006714                          MOVE.B    #$02,$0011(A0)
0000671A                          JSR       $000014E6(pc)
0000671E                          ADD.W     D0,D0
00006720                          MOVE.W    +$00(D4.W),$0038(A0)
00006726                          JSR       $000048EA(pc)
0000672A                          LEA       $00001AAE,A1
00006730                          TST.B     $005B(A0)
00006734                          BEQ       $0000673C
00006736                          LEA       $000019D4,A1
0000673C                          JSR       (A1)
0000673E                          BCC       $00007DA8(pc)
00006742                          ADD.W     Invalid
00006744                          OR.B      #$00,A4
00006748                          MOVE.B    -(A0),A3
0000674A                          OR.B      #$9E,D4
0000674E                          OR.B      #$A2,D4
00006752                          OR.B      #$A6,D4
00006756                          OR.B      #$AA,D4
0000675A                          OR.B      #$AE,D4
0000675E                          OR.B      #$B2,D4
00006762                          OR.B      #$B6,D4
00006766                          OR.B      #$B6,D4
0000676A                          AND.B     #$00,D0
0000676E                          BSET      D0,(A0)+
00006770                          BCHG      D0,$00C3(A1)
00006774                          Invalid
00006776                          BCHG      D0,$01D8(A1)
0000677A                          ADDQ.W    #$1,$000C(A0)
0000677E                          BRA       $00007E6C(pc)
00006782                          MOVE.B    +$00(D4.L),D1
00006786                          BMI       $0000679A
00006788                          CMP.B     $0030(A0),D1
0000678C                          BEQ       $00006796
0000678E                          CLR.W     $0038(A0)
00006792                          CLR.B     $0036(A0)
00006796                          MOVE.B    D1,$0030(A0)
0000679A                          RTS
0000679C                          Invalid
0000679E                          OR.B      #$01,D0
000067A2                          BTST      D0,D1
000067A4                          Invalid
000067A6                          TST.L     $0044(A0)
000067AA                          BEQ       $000067CE
000067AC                          TST.B     $0030(A0)
000067B0                          BNE       $000067BE
000067B2                          SUB.L     #$000007AE,$0044(A0)
000067BA                          BMI       $000067CA
000067BC                          RTS
000067BE                          ADD.L     #$000007AE,$0044(A0)
000067C6                          BPL       $000067CA
000067C8                          RTS
000067CA                          CLR.L     $0044(A0)
000067CE                          RTS
000067D0                          TST.W     $0044(A0)
000067D4                          BEQ       $000067DE
000067D6                          BMI       $0000686A(pc)
000067DA                          BRA       $00006842(pc)
000067DE                          TST.B     $0030(A0)
000067E2                          BNE       $0000686A(pc)
000067E6                          BRA       $00006842(pc)
000067EA                          JSR       $00007F1E(pc)
000067EE                          AND.W     #$000F,D0
000067F2                          MOVE.B    +$00(D7.W),D0
000067F6                          BEQ       $00006800
000067F8                          BMI       $0000686A(pc)
000067FC                          BRA       $00006842(pc)
00006800                          RTS
00006802                          OR.B      #$01,D1
00006806                          Invalid
00006808                          Invalid
0000680A                          OR.B      #$BA,D0
0000680E                          CMP.B     $6414(A6),D2
00006812                          ADD.W     Invalid
00006814                          OR.B      #$28,A0
00006818                          OR.B      #$24,D1
0000681C                          BNE       $00006826
0000681E                          BTST      #$03,D2
00006822                          BEQ       $00006892(pc)
00006826                          JSR       $00001C94(pc)
0000682A                          BCC       $00006840
0000682C                          ADD.W     Invalid
0000682E                          OR.B      #$28,A0
00006832                          OR.B      #$24,D2
00006836                          BNE       $00006840
00006838                          BTST      #$03,D2
0000683C                          BEQ       $00006892(pc)
00006840                          RTS
00006842                          BSET      #$04,$0053(A0)
00006848                          BCLR      #$05,$0053(A0)
0000684E                          BNE       $00006892
00006850                          JSR       $00001C3C(pc)
00006854                          BCC       $00006868
00006856                          ADD.W     Invalid
00006858                          OR.B      #$28,A0
0000685C                          OR.B      #$24,D1
00006860                          BNE       $00006868
00006862                          BTST      #$03,D2
00006866                          BEQ       $00006892
00006868                          RTS
0000686A                          BSET      #$06,$0053(A0)
00006870                          BCLR      #$07,$0053(A0)
00006876                          BNE       $00006892
00006878                          JSR       $00001C94(pc)
0000687C                          BCC       $00006890
0000687E                          ADD.W     Invalid
00006880                          OR.B      #$28,A0
00006884                          OR.B      #$24,D2
00006888                          BNE       $00006890
0000688A                          BTST      #$03,D2
0000688E                          BEQ       $00006892
00006890                          RTS
00006892                          BTST      #$05,$0026(A0)
00006898                          BNE       $000068AE
0000689A                          MOVE.B    #$04,$0003(A0)
000068A0                          JSR       $00007DC4(pc)
000068A4                          MOVE.L    (A7)+,D0
000068A6                          JSR       $000201BE
000068AC                          RTS
000068AE                          MOVE.B    $0026(A0),D0
000068B2                          AND.W     #$0007,D0
000068B6                          MOVE.B    +$00(D4.W),D0
000068BA                          BEQ       $0000689A
000068BC                          BMI       $000068D8
000068BE                          BRA       $000068C8
000068C0                          Invalid
000068C2                          Invalid
000068C4                          BTST      D0,D1
000068C6                          BTST      D0,D0
000068C8                          MOVE.B    #$01,$0011(A0)
000068CE                          MOVE.B    #$10,$0004(A0)
000068D4                          MOVE.L    (A7)+,D0
000068D6                          RTS
000068D8                          MOVE.B    #$01,$0011(A0)
000068DE                          MOVE.B    #$14,$0004(A0)
000068E4                          MOVE.L    (A7)+,D0
000068E6                          RTS
000068E8                          MOVE.B    #$01,$004D(A0)
000068EE                          LEA       $00001AAE,A1
000068F4                          TST.B     $005B(A0)
000068F8                          BNE       $00006910
000068FA                          BSET      #$02,$0053(A0)
00006900                          BCLR      #$03,$0053(A0)
00006906                          BNE       $000069A4(pc)
0000690A                          LEA       $000019D4,A1
00006910                          MOVE.L    $000C(A0),$E010
00006916                          JSR       (A1)
00006918                          BCC       $00006946
0000691A                          BTST      #$03,D2
0000691E                          BNE       $000069B8(pc)
00006922                          BTST      #$02,D2
00006926                          BNE       $0000693E
00006928                          BTST      #$05,$0026(A0)
0000692E                          BEQ       $0000693E
00006930                          MOVE.B    $0026(A0),D1
00006934                          AND.B     #$0F,D1
00006938                          CMP.B     #$04,D1
0000693C                          BLT       $00006946
0000693E                          ADD.W     Invalid
00006940                          OR.B      #$00,A4
00006944                          OR.W      #$2178,-(A0)
00006948                          ROXR.B    #8,D0
0000694A                          OR.B      #$75,A4
0000694E                          BTST      #$05,$0026(A0)
00006954                          BEQ       $00006964
00006956                          MOVE.B    $0026(A0),D0
0000695A                          AND.B     #$0F,D0
0000695E                          CMP.B     #$04,D0
00006962                          BLT       $000069A2
00006964                          MOVE.B    #$00,$004D(A0)
0000696A                          LEA       $000019F0,A1
00006970                          TST.B     $005B(A0)
00006974                          BNE       $00006992
00006976                          BSET      #$02,$0053(A0)
0000697C                          BCLR      #$03,$0053(A0)
00006982                          BEQ       $0000698C
00006984                          MOVE.B    $0054(A0),$0022(A0)
0000698A                          BRA       $000069A4
0000698C                          LEA       $00001D9A,A1
00006992                          JSR       (A1)
00006994                          BCC       $000069A2
00006996                          BTST      #$02,D2
0000699A                          BNE       $000069A2
0000699C                          ADD.W     Invalid
0000699E                          OR.B      #$02,A4
000069A2                          RTS
000069A4                          JSR       $000201BE
000069AA                          JSR       $00007DC4(pc)
000069AE                          MOVE.B    #$08,$0003(A0)
000069B4                          MOVE.L    (A7)+,D0
000069B6                          RTS
000069B8                          JSR       $000201B4
000069BE                          MOVE.L    (A7)+,D0
000069C0                          MOVE.B    #$0C,$0003(A0)
000069C6                          MOVE.B    #$12,$0004(A0)
000069CC                          MOVE.B    #$00,$0005(A0)
000069D2                          BTST      #$05,D2
000069D6                          BNE       $000069F8
000069D8                          NEG.W     $003C(A0)
000069DC                          MOVE.B    #$10,$0041(A0)
000069E2                          CLR.B     $0034(A0)
000069E6                          MOVE.B    $0030(A0),D0
000069EA                          LEA       $00041192,A4
000069F0                          AND.B     #$01,D0
000069F4                          BRA       $00007E40(pc)
000069F8                          RTS
000069FA                          BSET      #$00,$0053(A0)
00006A00                          TST.B     $005B(A0)
00006A04                          BNE       $00006A1E
00006A06                          BCLR      #$01,$0053(A0)
00006A0C                          BEQ       $00006A16
00006A0E                          MOVE.B    $0054(A0),$0022(A0)
00006A14                          BRA       $00006A38
00006A16                          JSR       $00001AAE(pc)
00006A1A                          BCC       $00006A80
00006A1C                          BRA       $00006A34
00006A1E                          BCLR      #$01,$0053(A0)
00006A24                          BEQ       $00006A2E
00006A26                          MOVE.B    $0054(A0),$0023(A0)
00006A2C                          BRA       $00006A38
00006A2E                          JSR       $000019D4(pc)
00006A32                          BCC       $00006A80
00006A34                          ADD.W     Invalid
00006A36                          OR.B      #$1F,A4
00006A3A                          JSR       $00006BD2(pc)
00006A3E                          JSR       $0002015C
00006A44                          BTST      #$02,$0043(A0)
00006A4A                          BNE       $00006A74
00006A4C                          MOVE.B    #$0E,$0004(A0)
00006A52                          MOVE.B    #$06,$0041(A0)
00006A58                          JSR       $000014E6(pc)
00006A5C                          AND.W     #$0007,D0
00006A60                          ADD.W     D0,D0
00006A62                          ADD.W     D0,D0
00006A64                          MOVE.L    +$00(D6.L),A4
00006A68                          MOVE.B    $0030(A0),D0
00006A6C                          AND.B     #$01,D0
00006A70                          BRA       $00007E40(pc)
00006A74                          MOVE.B    #$01,$0011(A0)
00006A7A                          MOVE.B    #$18,$0004(A0)
00006A80                          RTS
00006A82                          OR.B      #$AA,D4
00006A86                          OR.B      #$AA,D4
00006A8A                          OR.B      #$5A,D4
00006A8E                          OR.B      #$1A,D4
00006A92                          OR.B      #$5A,D4
00006A96                          OR.B      #$6A,D4
00006A9A                          OR.B      #$2A,D4
00006A9E                          OR.B      #$6A,D4
00006AA2                          BSET      #$00,$0053(A0)
00006AA8                          TST.B     $005B(A0)
00006AAC                          BNE       $00006AC6
00006AAE                          BCLR      #$01,$0053(A0)
00006AB4                          BEQ       $00006ABE
00006AB6                          MOVE.B    $0054(A0),$0022(A0)
00006ABC                          BRA       $00006AE0
00006ABE                          JSR       $00001AAE(pc)
00006AC2                          BCC       $00006AF8
00006AC4                          BRA       $00006ADC
00006AC6                          BCLR      #$01,$0053(A0)
00006ACC                          BEQ       $00006AD6
00006ACE                          MOVE.B    $0054(A0),$0023(A0)
00006AD4                          BRA       $00006AE0
00006AD6                          JSR       $000019D4(pc)
00006ADA                          BCC       $00006AF8
00006ADC                          ADD.W     Invalid
00006ADE                          OR.B      #$1F,A4
00006AE2                          JSR       $0002015C
00006AE8                          MOVE.B    #$08,$0003(A0)
00006AEE                          MOVE.B    #$00,$0004(A0)
00006AF4                          BRA       $000065EC(pc)
00006AF8                          RTS
00006AFA                          BSET      #$00,$0053(A0)
00006B00                          TST.B     $005B(A0)
00006B04                          BNE       $00006B1E
00006B06                          BCLR      #$01,$0053(A0)
00006B0C                          BEQ       $00006B16
00006B0E                          MOVE.B    $0054(A0),$0022(A0)
00006B14                          BRA       $00006B34
00006B16                          JSR       $00001AAE(pc)
00006B1A                          BCC       $00006B52
00006B1C                          BRA       $00006B34
00006B1E                          BCLR      #$01,$0053(A0)
00006B24                          BEQ       $00006B2E
00006B26                          MOVE.B    $0054(A0),$0023(A0)
00006B2C                          BRA       $00006B34
00006B2E                          JSR       $000019D4(pc)
00006B32                          BCC       $00006B52
00006B34                          MOVE.L    (A7)+,D0
00006B36                          JSR       $0002015C
00006B3C                          MOVE.B    #$00,$0006(A0)
00006B42                          MOVE.B    #$1C,$0003(A0)
00006B48                          MOVE.B    #$00,$0004(A0)
00006B4E                          BRA       $00004D42(pc)
00006B52                          RTS
00006B54                          TST.L     $0044(A0)
00006B58                          BEQ       $00006B7A
00006B5A                          TST.B     $0030(A0)
00006B5E                          BNE       $00006B7C
00006B60                          LEA       $0000188A,A1
00006B66                          TST.B     $005B(A0)
00006B6A                          BEQ       $00006B72
00006B6C                          LEA       $000018A8,A1
00006B72                          JSR       (A1)
00006B74                          TST.B     D1
00006B76                          BNE       $000069FA(pc)
00006B7A                          RTS
00006B7C                          LEA       $0000188A,A1
00006B82                          TST.B     $005B(A0)
00006B86                          BEQ       $00006B8E
00006B88                          LEA       $000018A8,A1
00006B8E                          JSR       (A1)
00006B90                          TST.B     D1
00006B92                          BNE       $000069FA(pc)
00006B96                          RTS
00006B98                          MOVE.L    (A7)+,D0
00006B9A                          JSR       $000201B4
00006BA0                          TST.W     $003C(A0)
00006BA4                          BMI       $00006BAA
00006BA6                          NEG.W     $003C(A0)
00006BAA                          MOVE.B    #$10,$0041(A0)
00006BB0                          MOVE.B    #$0C,$0003(A0)
00006BB6                          MOVE.B    #$0C,$0004(A0)
00006BBC                          CLR.B     $0034(A0)
00006BC0                          MOVE.B    $0030(A0),D0
00006BC4                          LEA       $00041192,A4
00006BCA                          AND.B     #$01,D0
00006BCE                          BRA       $00007E40(pc)
00006BD2                          MOVEQ     #$00,D2
00006BD4                          BSR       $00006BDC
00006BD6                          MOVEQ     #$01,D2
00006BD8                          BSR       $00006BDC
00006BDA                          MOVEQ     #$02,D2
00006BDC                          JSR       $00028A3A
00006BE2                          BCS       $00006C1E
00006BE4                          MOVE.W    #$0100,$0000(A1)
00006BEA                          MOVE.B    D2,$0006(A1)
00006BEE                          MOVE.B    $0030(A0),$0030(A1)
00006BF4                          MOVE.B    $0022(A0),$0022(A1)
00006BFA                          MOVE.B    $0023(A0),$0023(A1)
00006C00                          MOVE.W    $0008(A0),$0008(A1)
00006C06                          MOVE.W    $000C(A0),$000C(A1)
00006C0C                          ADD.W     #$0020,$000C(A1)
00006C12                          TST.B     $005B(A0)
00006C16                          BEQ       $00006C1E
00006C18                          SUB.W     #$0040,$000C(A1)
00006C1E                          RTS
00006C20                          BTST      #$02,$0043(A0)
00006C26                          BNE       $00006C6C
00006C28                          JSR       $00007F1E(pc)
00006C2C                          LEA       $0000679C(pc),A2
00006C30                          MOVE.B    $00(A2,D0.W),D0
00006C34                          BMI       $00006C6C
00006C36                          CMP.B     $0030(A0),D0
00006C3A                          BEQ       $00006C6C
00006C3C                          MOVE.B    D0,$0030(A0)
00006C40                          CLR.B     $0034(A0)
00006C44                          MOVE.B    $0011(A0),D5
00006C48                          LEA       $00041192,A4
00006C4E                          CMP.B     #$00,$0026(A0)
00006C54                          BNE       $00006C5C
00006C56                          LEA       $0004117A,A4
00006C5C                          AND.B     #$01,D0
00006C60                          JSR       $00007E40(pc)
00006C64                          JSR       $00007E6C(pc)
00006C68                          MOVE.B    D5,$0011(A0)
00006C6C                          RTS
00006C6E                          BTST      #$04,$E109
00006C74                          BEQ       $00006CA8
00006C76                          CMP.B     #$01,$0034(A0)
00006C7C                          BEQ       $00006CA4
00006C7E                          BGT       $00006CA8
00006C80                          JSR       $00028A1C
00006C86                          BCS       $00006CA8
00006C88                          MOVE.B    #$01,$0037(A0)
00006C8E                          MOVE.B    #$00,$0006(A1)
00006C94                          MOVE.W    #$0100,$0000(A1)
00006C9A                          MOVE.B    $0030(A0),$0030(A1)
00006CA0                          JSR       $00006CAA(pc)
00006CA4                          ADDQ.B    #$1,$0034(A0)
00006CA8                          RTS
00006CAA                          JSR       $00007F1E(pc)
00006CAE                          LEA       $0000679C(pc),A2
00006CB2                          MOVE.B    $00(A2,D0.W),D0
00006CB6                          BMI       $00006CBC
00006CB8                          MOVE.B    D0,$0030(A1)
00006CBC                          MOVE.B    $0011(A0),$F0A8
00006CC2                          MOVE.B    $0026(A0),$F0A9
00006CC8                          MOVE.W    $0012(A0),$F0AA
00006CCE                          MOVE.W    $0014(A0),$F0AC
00006CD4                          BTST      #$02,$0043(A0)
00006CDA                          BEQ       $00006D0A
00006CDC                          MOVE.B    $0026(A0),D0
00006CE0                          AND.W     #$0007,D0
00006CE4                          ADD.W     D0,D0
00006CE6                          ADD.W     D0,D0
00006CE8                          LEA       $000412CA,A4
00006CEE                          TST.B     $0030(A0)
00006CF2                          BEQ       $00006CFA
00006CF4                          LEA       $00041372,A4
00006CFA                          LEA       $00(A4,D0.W),A4
00006CFE                          MOVE.B    $0030(A1),D0
00006D02                          AND.B     #$01,D0
00006D06                          BRA       $00007E40(pc)
00006D0A                          MOVE.B    $0026(A0),D0
00006D0E                          AND.B     #$01,D0
00006D12                          LEA       $0004129A,A4
00006D18                          TST.B     $0030(A0)
00006D1C                          BEQ       $00006D24
00006D1E                          LEA       $000412B2,A4
00006D24                          BRA       $00007E40(pc)
00006D28                          TST.B     $0034(A0)
00006D2C                          BEQ       $00006D72
00006D2E                          MOVE.B    $0026(A0),D0
00006D32                          AND.B     #$F0,D0
00006D36                          BTST      #$04,D0
00006D3A                          BNE       $00006D74
00006D3C                          MOVE.B    $F0A8,$0011(A0)
00006D42                          MOVE.B    $F0A9,$0026(A0)
00006D48                          MOVE.W    $F0AA,$0012(A0)
00006D4E                          MOVE.W    $F0AC,$0014(A0)
00006D54                          SUBQ.B    #$1,$0034(A0)
00006D58                          BEQ       $00006D72
00006D5A                          JSR       $00028A1C
00006D60                          BCS       $00006D72
00006D62                          MOVE.W    #$0100,$0000(A1)
00006D68                          MOVE.B    $0030(A0),$0030(A1)
00006D6E                          BRA       $00006CAA(pc)
00006D72                          RTS
00006D74                          MOVE.B    $0011(A0),D0
00006D78                          MOVE.B    $0026(A0),D1
00006D7C                          MOVE.W    $0012(A0),D2
00006D80                          MOVE.W    $0014(A0),D3
00006D84                          MOVEM.W   D0-D3,-(A7)
00006D88                          MOVE.B    $F0A8,$0011(A0)
00006D8E                          MOVE.B    $F0A9,$0026(A0)
00006D94                          MOVE.W    $F0AA,$0012(A0)
00006D9A                          MOVE.W    $F0AC,$0014(A0)
00006DA0                          JSR       $00007E6C(pc)
00006DA4                          MOVE.B    $0011(A0),$F0A8
00006DAA                          MOVE.B    $0026(A0),$F0A9
00006DB0                          MOVE.W    $0012(A0),$F0AA
00006DB6                          MOVE.W    $0014(A0),$F0AC
00006DBC                          MOVEM.W   (A7)+,D0-D3
00006DC0                          MOVE.B    D0,$0011(A0)
00006DC4                          MOVE.B    D1,$0026(A0)
00006DC8                          MOVE.W    D2,$0012(A0)
00006DCC                          MOVE.W    D3,$0014(A0)
00006DD0                          RTS
00006DD2                          MOVE.W    D1,D5
00006DD4                          MOVE.W    $0038(A0),D0
00006DD8                          JSR       $000014FC(pc)
00006DDC                          ADD.W     D1,D1
00006DDE                          ADD.W     D1,D1
00006DE0                          MOVE.L    +$10(D6.W),A1
00006DE4                          JSR       (A1)
00006DE6                          SUB.W     #$0202,D1
00006DEA                          BMI       $00006E02
00006DEC                          MOVE.L    (A7)+,D2
00006DEE                          MOVE.B    #$14,$0003(A0)
00006DF4                          MOVE.B    #$00,$0004(A0)
00006DFA                          JSR       $00007164(pc)
00006DFE                          BRA       $00007010(pc)
00006E02                          MOVE.W    D5,D1
00006E04                          TST.B     D1
00006E06                          BMI       $00006E0C
00006E08                          MOVE.B    D1,$0030(A0)
00006E0C                          RTS
00006E0E                          OR.B      #$68,D0
00006E12                          OR.B      #$68,D0
00006E16                          OR.B      #$6C,D0
00006E1A                          OR.B      #$92,D0
00006E1E                          OR.B      #$BC,D0
00006E22                          OR.B      #$BC,D0
00006E26                          OR.B      #$92,D0
00006E2A                          OR.B      #$6C,D0
00006E2E                          TST.B     $F0AE
00006E32                          BNE       $00006E40
00006E34                          MOVE.B    $001E(A0),D0
00006E38                          CMP.B     $0056(A0),D0
00006E3C                          BNE       $00007D5C(pc)
00006E40                          BTST      #$04,$E109
00006E46                          BNE       $00006E5E
00006E48                          BTST      #$06,$E109
00006E4E                          BNE       $00005FF8(pc)
00006E52                          BTST      #$05,$E109
00006E58                          BNE       $00005FF8(pc)
00006E5C                          BRA       $00006E8C
00006E5E                          JSR       $00028A1C
00006E64                          BCS       $00006E8C
00006E66                          MOVE.B    #$01,$0037(A0)
00006E6C                          MOVE.B    #$00,$0006(A1)
00006E72                          MOVE.W    #$0100,$0000(A1)
00006E78                          MOVE.B    $0030(A0),$0030(A1)
00006E7E                          JSR       $00007F1E(pc)
00006E82                          JSR       $0000710A(pc)
00006E86                          BMI       $00006E8C
00006E88                          MOVE.B    D1,$0030(A1)
00006E8C                          TST.B     $005B(A0)
00006E90                          BEQ       $00006E98
00006E92                          BSET      #$00,$0053(A0)
00006E98                          MOVE.B    $0022(A0),$0031(A0)
00006E9E                          MOVE.B    $0023(A0),$0032(A0)
00006EA4                          JSR       $00007F1E(pc)
00006EA8                          JSR       $0000710A(pc)
00006EAC                          BMI       $00006EB6
00006EAE                          CMP.B     $0030(A0),D1
00006EB2                          BEQ       $00007DA8(pc)
00006EB6                          MOVEQ     #$00,D0
00006EB8                          MOVE.B    $0016(A0),D0
00006EBC                          LSL.W     #3,D0
00006EBE                          JSR       $000014FC(pc)
00006EC2                          ADD.B     D1,D0
00006EC4                          TST.B     $0030(A0)
00006EC8                          BEQ       $00006ECE
00006ECA                          ADD.W     #$0048,D0
00006ECE                          MOVE.B    +$00(D7.W),D0
00006ED2                          BEQ       $00006F6E(pc)
00006ED6                          BMI       $00006FAC(pc)
00006EDA                          BRA       $00006F86(pc)
00006EDE                          OR.B      #$00,D0
00006EE2                          OR.B      #$00,D2
00006EE6                          OR.B      #$00,D0
00006EEA                          OR.B      #$00,D0
00006EEE                          OR.B      #$00,D0
00006EF2                          OR.B      #$00,D0
00006EF6                          OR.B      #$FF,D0
00006EFA                          Invalid
00006EFC                          OR.B      #$FF,D0
00006F00                          Invalid
00006F02                          AND.B     #$FF,D0
00006F06                          OR.B      #$02,D2
00006F0A                          Invalid
00006F0C                          Invalid
00006F0E                          OR.B      #$02,D2
00006F12                          AND.B     #$02,D2
00006F16                          OR.B      #$FD,D2
00006F1A                          Invalid
00006F1C                          AND.B     #$00,D2
00006F20                          Invalid
00006F22                          Invalid
00006F24                          OR.B      #$00,D0
00006F28                          OR.B      #$00,D2
00006F2C                          Invalid
00006F2E                          OR.B      #$02,D2
00006F32                          Invalid
00006F34                          Invalid
00006F36                          OR.B      #$02,D2
00006F3A                          AND.B     #$02,D2
00006F3E                          OR.B      #$FF,D2
00006F42                          Invalid
00006F44                          AND.B     #$FF,D2
00006F48                          Invalid
00006F4A                          OR.B      #$FF,D2
00006F4E                          OR.B      #$00,D0
00006F52                          Invalid
00006F54                          Invalid
00006F56                          OR.B      #$00,D0
00006F5A                          OR.B      #$00,D0
00006F5E                          OR.B      #$00,D0
00006F62                          OR.B      #$00,D0
00006F66                          OR.B      #$00,D0
00006F6A                          Invalid
00006F6C                          Invalid
00006F6E                          JSR       $000071EE(pc)
00006F72                          JSR       $00007010(pc)
00006F76                          JSR       $000014E6(pc)
00006F7A                          ADD.W     D0,D0
00006F7C                          MOVE.W    +$00(D7.L),D0
00006F80                          LSR.W     #1,D0
00006F82                          BRA       $00006FBE(pc)
00006F86                          JSR       $000071E4(pc)
00006F8A                          JSR       $00007E6C(pc)
00006F8E                          JSR       $000014E6(pc)
00006F92                          ADD.W     D0,D0
00006F94                          MOVE.W    +$00(D3.W),D0
00006F98                          BRA       $00006FBE(pc)
00006F9C                          OR.B      #$33,$00(A3,A7.W)
00006FA2                          OR.B      #$11,-(A4)
00006FA6                          OR.B      #$24,(A1)
00006FAA                          OR.B      #$BA,$0240(A7)
00006FB0                          JSR       $00007010(pc)
00006FB4                          JSR       $000014E6(pc)
00006FB8                          ADD.W     D0,D0
00006FBA                          MOVE.W    +$00(D0.W),D0
00006FBE                          (CCC     #$0,$0038(A0)
00006FC2                          BPL       $00006FCE
00006FC4                          MOVE.W    #$0000,$0038(A0)
00006FCA                          BEQ       $00007DA8(pc)
00006FCE                          JSR       $0000704E(pc)
00006FD2                          JSR       $0000708A(pc)
00006FD6                          LEA       $000019D4,A1
00006FDC                          TST.B     $005B(A0)
00006FE0                          BNE       $00006FF8
00006FE2                          BCLR      #$01,$0053(A0)
00006FE8                          BEQ       $00006FF2
00006FEA                          MOVE.B    $0054(A0),$0022(A0)
00006FF0                          BRA       $00007002
00006FF2                          LEA       $00001AAE,A1
00006FF8                          JSR       (A1)
00006FFA                          BCC       $00007E04(pc)
00006FFE                          ADD.W     Invalid
00007000                          OR.B      #$28,A4
00007004                          OR.B      #$BA,$0FB6
0000700A                          MOVE.W    D1,$0038(A0)
0000700E                          RTS
00007010                          JSR       $000014E6(pc)
00007014                          ADD.W     D0,D0
00007016                          ADD.W     D0,D0
00007018                          LEA       $0000702E(pc),A1
0000701C                          MOVE.L    $00(A1,D0.W),A4
00007020                          MOVE.B    $0030(A0),D0
00007024                          AND.B     #$01,D0
00007028                          JMP       $00007E40
0000702E                          OR.B      #$06,D4
00007032                          OR.B      #$06,D4
00007036                          OR.B      #$12,D4
0000703A                          OR.B      #$16,D4
0000703E                          OR.B      #$56,D4
00007042                          OR.B      #$60,D4
00007046                          OR.B      #$20,D4
0000704A                          OR.B      #$56,D4
0000704E                          MOVE.B    #$00,$0043(A0)
00007054                          TST.B     $0030(A0)
00007058                          BNE       $0000706A
0000705A                          MOVE.W    $0008(A0),D0
0000705E                          SUB.W     $EA08,D0
00007062                          CMP.W     #$0160,D0
00007066                          BLE       $00007078
00007068                          RTS
0000706A                          MOVE.W    $0008(A0),D0
0000706E                          SUB.W     $EA08,D0
00007072                          CMP.W     #$0020,D0
00007076                          BLE       $00007068
00007078                          BRA       $000048EA(pc)
0000707C                          MOVE.B    $0022(A0),D0
00007080                          CMP.B     $0031(A0),D0
00007084                          BNE       $00007DDC(pc)
00007088                          RTS
0000708A                          TST.B     $0030(A0)
0000708E                          BNE       $000070CA
00007090                          JSR       $00001C3C(pc)
00007094                          BCC       $00007108
00007096                          CMP.B     #$01,$0024(A0)
0000709C                          BNE       $00007108
0000709E                          BTST      #$03,D2
000070A2                          BNE       $00007104
000070A4                          ADD.W     Invalid
000070A6                          OR.B      #$7C,A0
000070AA                          OR.B      #$03,D4
000070AE                          MOVE.L    (A7)+,D0
000070B0                          JSR       $000201BE
000070B6                          MOVE.B    $0030(A0),D0
000070BA                          AND.B     #$01,D0
000070BE                          LEA       $00040E9A,A4
000070C4                          JMP       $00007E40
000070CA                          JSR       $00001C94(pc)
000070CE                          BCC       $00007108
000070D0                          CMP.B     #$02,$0024(A0)
000070D6                          BNE       $00007108
000070D8                          BTST      #$03,D2
000070DC                          BNE       $00007104
000070DE                          ADD.W     Invalid
000070E0                          OR.B      #$7C,A0
000070E4                          OR.B      #$03,D4
000070E8                          MOVE.L    (A7)+,D0
000070EA                          JSR       $000201BE
000070F0                          MOVE.B    $0030(A0),D0
000070F4                          AND.B     #$01,D0
000070F8                          LEA       $00040EA4,A4
000070FE                          JMP       $00007E40
00007104                          ADD.W     Invalid
00007106                          OR.B      #$75,A0
0000710A                          CMP.B     #$08,D0
0000710E                          BLT       $00007116
00007110                          MOVE.W    #$FFFF,D1
00007114                          RTS
00007116                          JSR       $000014FC(pc)
0000711A                          LSL.W     #3,D1
0000711C                          ADD.B     D0,D1
0000711E                          MOVE.B    +$10(D2.W),D1
00007122                          RTS
00007124                          Invalid
00007126                          Invalid
00007128                          Invalid
0000712A                          Invalid
0000712C                          Invalid
0000712E                          OR.B      #$01,D0
00007132                          BTST      D0,D1
00007134                          Invalid
00007136                          OR.B      #$01,D0
0000713A                          BTST      D0,D1
0000713C                          OR.B      #$FF,D0
00007140                          BTST      D0,D1
00007142                          BSET      D0,
00007144                          OR.B      #$FF,D0
00007148                          BTST      D0,D1
0000714A                          BSET      D0,
0000714C                          BSET      D0,
0000714E                          OR.B      #$FF,D0
00007152                          BTST      D0,D1
00007154                          BSET      D0,
00007156                          OR.B      #$FF,D0
0000715A                          BTST      D0,D1
0000715C                          Invalid
0000715E                          OR.B      #$01,D0
00007162                          BTST      D0,D1
00007164                          MOVEQ     #$00,D2
00007166                          JSR       $00007172(pc)
0000716A                          MOVEQ     #$01,D2
0000716C                          JSR       $00007172(pc)
00007170                          MOVEQ     #$02,D2
00007172                          JSR       $00020DB0
00007178                          BTST      #$00,D0
0000717C                          BEQ       $000071C0
0000717E                          JSR       $00028A3A
00007184                          BCS       $000071D4
00007186                          MOVE.W    #$0100,$0000(A1)
0000718C                          MOVE.B    $0030(A0),$0030(A1)
00007192                          MOVE.B    $0022(A0),$0022(A1)
00007198                          MOVE.B    $0023(A0),$0023(A1)
0000719E                          MOVE.W    $0008(A0),$0008(A1)
000071A4                          MOVE.W    $000C(A0),$000C(A1)
000071AA                          MOVE.B    D2,$0006(A1)
000071AE                          ADD.W     #$0020,$000C(A1)
000071B4                          TST.B     $005B(A0)
000071B8                          BEQ       $000071C0
000071BA                          SUB.W     #$0040,$000C(A1)
000071C0                          JSR       $00020DB0
000071C6                          AND.W     #$0007,D0
000071CA                          MOVE.B    +$00(D0.L),D0
000071CE                          MOVE.B    D0,$20(A0,D7.W)
000071D2                          RTS
000071D4                          MOVE.B    #$01,$20(A0,D7.W)
000071DA                          RTS
000071DC                          MOVEP.W   $0303(A1),D1
000071E0                          BTST      D4,D3
000071E2                          MOVEP.W   $1C28(A1),D4
000071E6                          OR.B      #$46,$00(A0,D0.W)
000071EC                          BRA       $000071F2
000071EE                          MOVE.B    $0030(A0),D6
000071F2                          MOVEQ     #$00,D2
000071F4                          JSR       $00007200(pc)
000071F8                          MOVEQ     #$01,D2
000071FA                          JSR       $00007200(pc)
000071FE                          MOVEQ     #$02,D2
00007200                          SUBQ.B    #$1,$20(A0,D7.W)
00007204                          BEQ       $0000717E(pc)
00007208                          RTS
0000720A                          TST.B     $F0AE
0000720E                          BNE       $0000721C
00007210                          MOVE.B    $001E(A0),D0
00007214                          CMP.B     $0056(A0),D0
00007218                          BNE       $00007D5C(pc)
0000721C                          MOVEQ     #$00,D0
0000721E                          MOVE.B    $0004(A0),D0
00007222                          MOVE.W    +$00(D3.W),D0
00007226                          JMP       +$00(D1.W)
0000722A                          OR.B      #$7E,A4
0000722E                          Invalid
00007230                          BTST      D0,(A0)+
00007232                          BTST      D0,#$74
00007236                          TST.B     $005B(A0)
0000723A                          BNE       $00007242
0000723C                          OR.B      #$05,$0053(A0)
00007242                          SUBQ.B    #$1,$0011(A0)
00007246                          BNE       $0000728C
00007248                          MOVE.B    $0026(A0),D0
0000724C                          AND.W     #$0003,D0
00007250                          LSL.W     #3,D0
00007252                          LEA       $0000728E(pc),A1
00007256                          MOVE.B    $00(A1,D0.W),$0011(A0)
0000725C                          BMI       $00007DB4(pc)
00007260                          MOVE.B    $00(A1,A0.W),$0026(A0)
00007266                          MOVE.W    $00(A1,D1.W),D1
0000726A                          TST.B     $005B(A0)
0000726E                          BEQ       $00007272
00007270                          NEG.W     D1
00007272                          ADD.W     Invalid
00007274                          OR.B      #$71,A4
00007278                          OR.B      #$28,D4
0000727C                          OR.B      #$80,$D0(A0,D0.W)
00007282                          MOVE.W    $00(A4,D0.W),D0
00007286                          MOVE.W    $00(A6,D0.W),$0012(A0)
0000728C                          RTS
0000728E                          ADD.B     #$00,D1
00007292                          OR.B      #$3A,D4
00007296                          ADD.B     #$00,D2
0000729A                          OR.B      #$3E,D4
0000729E                          ADD.B     #$C0,D3
000072A2                          OR.B      #$42,D4
000072A6                          Invalid
000072A8                          JSR       $00007494(pc)
000072AC                          JSR       $0000745A(pc)
000072B0                          JSR       $00007164(pc)
000072B4                          MOVE.L    #$00020000,$0044(A0)
000072BC                          MOVE.W    $0038(A0),D0
000072C0                          CMP.W     #$0200,D0
000072C4                          BLE       $000072CE
000072C6                          SWAP      D0
000072C8                          ASR.L     #8,D0
000072CA                          MOVE.L    D0,$0044(A0)
000072CE                          MOVE.L    #$00003333,$0048(A0)
000072D6                          MOVE.B    #$0A,$0041(A0)
000072DC                          MOVE.B    #$04,$0004(A0)
000072E2                          MOVE.B    $0030(A0),D0
000072E6                          LEA       $000411F6,A4
000072EC                          AND.B     #$01,D0
000072F0                          JMP       $00007E40
000072F6                          BSR       $00007332
000072F8                          JSR       $00007494(pc)
000072FC                          JSR       $0000745A(pc)
00007300                          JSR       $00007408(pc)
00007304                          JSR       $00007E6C(pc)
00007308                          SUBQ.B    #$1,$0041(A0)
0000730C                          BNE       $00007330
0000730E                          MOVE.B    #$05,$0041(A0)
00007314                          MOVE.B    #$06,$0004(A0)
0000731A                          JSR       $000014E6(pc)
0000731E                          ADDQ.B    #$1,D0
00007320                          AND.W     #$0007,D0
00007324                          CMP.W     #$0003,D0
00007328                          BGT       $000073D6(pc)
0000732C                          BRA       $00007552(pc)
00007330                          RTS
00007332                          JSR       $000071F2(pc)
00007336                          JSR       $000075E4(pc)
0000733A                          JSR       $000073E2(pc)
0000733E                          BRA       $00007430(pc)
00007342                          BSR       $00007332
00007344                          JSR       $000074CC(pc)
00007348                          JSR       $0000745A(pc)
0000734C                          JSR       $00007408(pc)
00007350                          SUBQ.B    #$1,$0041(A0)
00007354                          BNE       $00007364
00007356                          MOVE.L    #$0000147A,$0048(A0)
0000735E                          MOVE.B    #$08,$0004(A0)
00007364                          RTS
00007366                          JSR       $000071F2(pc)
0000736A                          JSR       $000075E4(pc)
0000736E                          JSR       $000073E2(pc)
00007372                          JSR       $0000741C(pc)
00007376                          JSR       $00007430(pc)
0000737A                          JSR       $000074CC(pc)
0000737E                          JSR       $0000745A(pc)
00007382                          CMP.W     #$0300,$0038(A0)
00007388                          BGT       $0000739C
0000738A                          MOVE.L    #$0000147A,$0048(A0)
00007392                          MOVE.B    #$0A,$0004(A0)
00007398                          BRA       $000075B4(pc)
0000739C                          RTS
0000739E                          JSR       $000075E4(pc)
000073A2                          BTST      #$06,$E109
000073A8                          BNE       $00005EAA(pc)
000073AC                          BTST      #$05,$E109
000073B2                          BNE       $00005EAA(pc)
000073B6                          JSR       $000071F2(pc)
000073BA                          JSR       $000073E2(pc)
000073BE                          JSR       $0000741C(pc)
000073C2                          JSR       $00007430(pc)
000073C6                          JSR       $00007502(pc)
000073CA                          JSR       $0000745A(pc)
000073CE                          CMP.W     #$0200,$0038(A0)
000073D4                          BGT       $000073E0
000073D6                          MOVE.B    #$00,$0003(A0)
000073DC                          BRA       $00007DC4(pc)
000073E0                          RTS
000073E2                          MOVE.B    $0022(A0),$0031(A0)
000073E8                          MOVE.B    $0023(A0),$0032(A0)
000073EE                          MOVE.W    $0038(A0),D0
000073F2                          JSR       $00007FBE(pc)
000073F6                          SWAP      D1
000073F8                          ASR.L     #8,D1
000073FA                          TST.B     $0030(A0)
000073FE                          BEQ       $00007402
00007400                          NEG.L     D1
00007402                          ADD.L     Invalid
00007404                          OR.B      #$75,A0
00007408                          MOVE.L    $0044(A0),D0
0000740C                          ADD.L     $0048(A0),D0
00007410                          MOVE.L    D0,$0044(A0)
00007414                          ROR.L     #8,D0
00007416                          MOVE.W    D0,$0038(A0)
0000741A                          RTS
0000741C                          MOVE.L    $0044(A0),D0
00007420                          SUB.L     $0048(A0),D0
00007424                          MOVE.L    D0,$0044(A0)
00007428                          ROR.L     #8,D0
0000742A                          MOVE.W    D0,$0038(A0)
0000742E                          RTS
00007430                          TST.B     $0030(A0)
00007434                          BNE       $00007446
00007436                          JSR       $00001C3C(pc)
0000743A                          BCC       $00007458
0000743C                          CMP.B     #$01,$0024(A0)
00007442                          BNE       $00007458
00007444                          BRA       $00007454
00007446                          JSR       $00001C94(pc)
0000744A                          BCC       $00007458
0000744C                          CMP.B     #$02,$0024(A0)
00007452                          BNE       $00007458
00007454                          ADD.W     Invalid
00007456                          OR.B      #$75,A0
0000745A                          TST.B     $E199
0000745E                          BEQ       $00007492
00007460                          LEA       $000019D4,A1
00007466                          TST.B     $005B(A0)
0000746A                          BEQ       $00007472
0000746C                          LEA       $00001AAE,A1
00007472                          JSR       (A1)
00007474                          BCC       $00007492
00007476                          BTST      #$03,D2
0000747A                          BNE       $00007492
0000747C                          ADD.W     Invalid
0000747E                          OR.B      #$7C,A4
00007482                          OR.B      #$4D,D1
00007486                          MOVE.B    #$08,$0003(A0)
0000748C                          JSR       $00007DC4(pc)
00007490                          MOVE.L    (A7)+,D0
00007492                          RTS
00007494                          LEA       $000019D4,A1
0000749A                          TST.B     $005B(A0)
0000749E                          BNE       $000074BC
000074A0                          BSET      #$00,$0053(A0)
000074A6                          BCLR      #$01,$0053(A0)
000074AC                          BEQ       $000074B6
000074AE                          MOVE.B    $0054(A0),$0022(A0)
000074B4                          RTS
000074B6                          LEA       $00001AAE,A1
000074BC                          JSR       (A1)
000074BE                          BCC       $000074C6
000074C0                          ADD.W     Invalid
000074C2                          OR.B      #$75,A4
000074C6                          MOVE.L    (A7)+,D0
000074C8                          BRA       $000073D6(pc)
000074CC                          LEA       $000019D4,A1
000074D2                          TST.B     $005B(A0)
000074D6                          BNE       $000074F6
000074D8                          BSET      #$00,$0053(A0)
000074DE                          BCLR      #$01,$0053(A0)
000074E4                          BEQ       $000074F0
000074E6                          MOVE.B    $0054(A0),$0022(A0)
000074EC                          BRA       $0000753A(pc)
000074F0                          LEA       $00001AAE,A1
000074F6                          JSR       (A1)
000074F8                          BCC       $000074C6
000074FA                          ADD.W     Invalid
000074FC                          OR.B      #$00,A4
00007500                          OR.B      #$F9,$00007502(pc)
00007506                          MOVE.B    (A4),#$28
0000750A                          OR.W      #$661E,(A3)+
0000750E                          BSET      #$00,$0053(A0)
00007514                          BCLR      #$01,$0053(A0)
0000751A                          BEQ       $00007526
0000751C                          MOVE.B    $0054(A0),$0022(A0)
00007522                          BRA       $0000759C(pc)
00007526                          LEA       $00001AAE,A1
0000752C                          JSR       (A1)
0000752E                          BCC       $000073D6(pc)
00007532                          ADD.W     Invalid
00007534                          OR.B      #$00,A4
00007538                          OR.W      #$6140,-(A4)
0000753C                          MOVE.B    D0,D1
0000753E                          ADDQ.B    #$1,D0
00007540                          AND.W     #$0007,D0
00007544                          CMP.W     #$0003,D0
00007548                          BGT       $000074C6(pc)
0000754C                          CMP.B     D2,D1
0000754E                          BNE       $00007552
00007550                          RTS
00007552                          AND.W     #$0003,D0
00007556                          ADD.W     D0,D0
00007558                          ADD.W     D0,D0
0000755A                          MOVE.L    +$00(D0.L),A4
0000755E                          MOVE.B    $0030(A0),D0
00007562                          AND.B     #$01,D0
00007566                          JMP       $00007E40
0000756C                          OR.B      #$6A,D4
00007570                          OR.B      #$16,D4
00007574                          OR.B      #$16,D4
00007578                          OR.B      #$5A,D4
0000757C                          MOVE.B    $0022(A0),D0
00007580                          MOVE.B    $0031(A0),D2
00007584                          TST.B     $005B(A0)
00007588                          BEQ       $00007592
0000758A                          MOVE.B    $0023(A0),D0
0000758E                          MOVE.B    $0032(A0),D2
00007592                          AND.W     #$0007,D0
00007596                          AND.W     #$0007,D2
0000759A                          RTS
0000759C                          BSR       $0000757C
0000759E                          MOVE.B    D0,D1
000075A0                          ADDQ.B    #$1,D0
000075A2                          AND.W     #$0007,D0
000075A6                          CMP.W     #$0003,D0
000075AA                          BGT       $000073D6(pc)
000075AE                          CMP.B     D2,D1
000075B0                          BNE       $000075B4
000075B2                          RTS
000075B4                          JSR       $000014E6(pc)
000075B8                          ADDQ.B    #$1,D0
000075BA                          AND.W     #$0003,D0
000075BE                          ADD.W     D0,D0
000075C0                          ADD.W     D0,D0
000075C2                          MOVE.L    +$00(D0.L),A4
000075C6                          MOVE.B    $0030(A0),D0
000075CA                          AND.B     #$01,D0
000075CE                          JMP       $00007E40
000075D4                          OR.B      #$1A,D4
000075D8                          OR.B      #$26,D4
000075DC                          OR.B      #$26,D4
000075E0                          OR.B      #$0A,D4
000075E4                          BTST      #$04,$E109
000075EA                          BEQ       $0000760C
000075EC                          JSR       $00028A1C
000075F2                          BCS       $0000760C
000075F4                          MOVE.B    #$01,$0037(A0)
000075FA                          MOVE.B    #$00,$0006(A1)
00007600                          MOVE.W    #$0100,$0000(A1)
00007606                          MOVE.B    $0030(A0),$0030(A1)
0000760C                          RTS
0000760E                          MOVE.B    $0022(A0),$0031(A0)
00007614                          MOVE.B    $0023(A0),$0032(A0)
0000761A                          JSR       $0000785A(pc)
0000761E                          TST.B     $0004(A0)
00007622                          BNE       $000076FE(pc)
00007626                          MOVE.B    $001E(A0),$0056(A0)
0000762C                          MOVEQ     #$00,D0
0000762E                          MOVE.B    D0,$0036(A0)
00007632                          MOVE.W    D0,$0038(A0)
00007636                          MOVE.B    D0,$0051(A0)
0000763A                          MOVE.B    $0006(A0),D0
0000763E                          ADD.W     D0,D0
00007640                          MOVE.W    +$00(D3.W),D0
00007644                          JMP       +$00(D1.W)
00007648                          OR.B      #$50,A0
0000764C                          OR.W      #$005A,(A0)
00007650                          MOVE.B    $001F(A0),$0030(A0)
00007656                          JSR       $000014E6(pc)
0000765A                          MOVE.B    +$00(D2.L),D0
0000765E                          BEQ       $000076A2
00007660                          BMI       $00007668
00007662                          SUBQ.B    #$1,D0
00007664                          MOVE.B    D0,$0030(A0)
00007668                          MOVE.B    #$28,$0057(A0)
0000766E                          MOVE.B    $0030(A0),D0
00007672                          LEA       $00041262,A4
00007678                          AND.B     #$01,D0
0000767C                          JSR       $00007E40(pc)
00007680                          MOVE.B    #$04,$0004(A0)
00007686                          MOVE.B    #$00,$0005(A0)
0000768C                          BRA       $00007164(pc)
00007690                          Invalid
00007692                          Invalid
00007694                          AND.B     #$FF,D1
00007698                          CLR.W     $003C(A0)
0000769C                          CLR.L     $0044(A0)
000076A0                          BRA       $000076C0
000076A2                          MOVE.W    #$0000,$003C(A0)
000076A8                          MOVE.L    #$00040000,$0044(A0)
000076B0                          TST.B     $0030(A0)
000076B4                          BNE       $000076C0
000076B6                          TST.B     $001F(A0)
000076BA                          BEQ       $000076C0
000076BC                          NEG.L     $0044(A0)
000076C0                          MOVE.W    #$008F,$003E(A0)
000076C6                          MOVE.B    $FC27,D0
000076CA                          CMP.B     $FC4E,D0
000076CE                          BLE       $00007D98(pc)
000076D2                          MOVE.B    #$0C,$0003(A0)
000076D8                          MOVE.B    #$16,$0004(A0)
000076DE                          MOVE.B    $001F(A0),D0
000076E2                          MOVE.B    D0,$0030(A0)
000076E6                          MOVE.B    $0030(A0),D0
000076EA                          EOR.B     #$01,D0
000076EE                          LEA       $00041262,A4
000076F4                          AND.B     #$01,D0
000076F8                          JMP       $00007E40
000076FE                          JSR       $000071F2(pc)
00007702                          JSR       $0000799E(pc)
00007706                          TST.B     $0030(A0)
0000770A                          BNE       $0000771A
0000770C                          JSR       $000078FE(pc)
00007710                          JSR       $00007888(pc)
00007714                          JSR       $000078CC(pc)
00007718                          BRA       $00007726
0000771A                          JSR       $000078FE(pc)
0000771E                          JSR       $0000789C(pc)
00007722                          JSR       $000078CC(pc)
00007726                          MOVE.B    $0022(A0),D0
0000772A                          MOVE.B    $0031(A0),D1
0000772E                          TST.B     $005B(A0)
00007732                          BEQ       $0000773C
00007734                          MOVE.B    $0023(A0),D0
00007738                          MOVE.B    $0032(A0),D1
0000773C                          AND.W     #$0007,D0
00007740                          AND.W     #$0007,D1
00007744                          CMP.B     D1,D0
00007746                          BEQ       $00007E6C(pc)
0000774A                          LEA       $00007772(pc),A1
0000774E                          TST.B     $0030(A0)
00007752                          BEQ       $00007758
00007754                          LEA       $000077B2(pc),A1
00007758                          AND.W     #$0007,D0
0000775C                          LSL.W     #3,D0
0000775E                          ADD.B     D1,D0
00007760                          MOVE.B    $00(A1,D0.W),D0
00007764                          MOVE.W    +$00(D3.W),D0
00007768                          JMP       +$00(D1.W)
0000776C                          OR.L      #$01F00096,D6
00007772                          OR.B      #$00,D4
00007776                          OR.B      #$04,D4
0000777A                          OR.B      #$00,D2
0000777E                          OR.B      #$02,D2
00007782                          OR.B      #$00,D2
00007786                          OR.B      #$02,D2
0000778A                          OR.B      #$00,D0
0000778E                          OR.B      #$00,D0
00007792                          OR.B      #$00,D0
00007796                          OR.B      #$00,D0
0000779A                          OR.B      #$00,D4
0000779E                          OR.B      #$04,D2
000077A2                          OR.B      #$00,D2
000077A6                          OR.B      #$02,D2
000077AA                          OR.B      #$00,D2
000077AE                          OR.B      #$02,D2
000077B2                          OR.B      #$04,D4
000077B6                          SUB.B     #$04,D0
000077BA                          OR.B      #$02,D2
000077BE                          AND.B     #$02,D0
000077C2                          OR.B      #$02,D2
000077C6                          AND.B     #$02,D0
000077CA                          OR.B      #$02,D2
000077CE                          AND.B     #$02,D0
000077D2                          OR.B      #$04,D4
000077D6                          AND.B     #$04,D0
000077DA                          OR.B      #$00,D0
000077DE                          OR.B      #$00,D0
000077E2                          OR.B      #$00,D0
000077E6                          OR.B      #$00,D0
000077EA                          OR.B      #$02,D2
000077EE                          AND.B     #$02,D0
000077F2                          MOVE.B    $FC27,D0
000077F6                          CMP.B     $FC4E,D0
000077FA                          BLE       $00007D98(pc)
000077FE                          BRA       $00007DB4(pc)
00007802                          CMP.W     #$0280,$0038(A0)
00007808                          BLE       $0000795C(pc)
0000780C                          LEA       $00004D1A(pc),A1
00007810                          CMP.W     #$0400,$0038(A0)
00007816                          BLE       $0000781C
00007818                          LEA       $000A(A1),A1
0000781C                          MOVE.W    (A1)+,D0
0000781E                          TST.B     $005B(A0)
00007822                          BEQ       $00007826
00007824                          NEG.W     D0
00007826                          ADD.W     Invalid
00007828                          OR.B      #$19,A4
0000782C                          MOVE.W    (A1)+,$003C(A0)
00007830                          MOVE.W    (A1)+,$003E(A0)
00007834                          TST.B     $0030(A0)
00007838                          BEQ       $0000783C
0000783A                          NEG.L     D0
0000783C                          MOVE.L    D0,$0044(A0)
00007840                          MOVE.B    $FC27,D0
00007844                          CMP.B     $FC4E,D0
00007848                          BLE       $00007D98(pc)
0000784C                          MOVE.B    #$0C,$0003(A0)
00007852                          MOVE.B    #$16,$0004(A0)
00007858                          RTS
0000785A                          MOVE.B    $E109,D0
0000785E                          AND.B     #$50,D0
00007862                          BEQ       $00007886
00007864                          ADDQ.B    #$1,$0051(A0)
00007868                          CMP.B     #$02,$0051(A0)
0000786E                          BLT       $00007886
00007870                          MOVE.B    $FC27,D0
00007874                          CMP.B     $FC4E,D0
00007878                          BLE       $00007D98(pc)
0000787C                          MOVE.B    #$00,$0051(A0)
00007882                          BRA       $00007DB4(pc)
00007886                          RTS
00007888                          JSR       $00001C3C(pc)
0000788C                          BCC       $000078CA
0000788E                          ADD.W     Invalid
00007890                          OR.B      #$28,A0
00007894                          OR.B      #$24,D1
00007898                          BNE       $000078CA
0000789A                          BRA       $000078AE
0000789C                          JSR       $00001C94(pc)
000078A0                          BCC       $000078CA
000078A2                          ADD.W     Invalid
000078A4                          OR.B      #$28,A0
000078A8                          OR.B      #$24,D2
000078AC                          BNE       $000078CA
000078AE                          ADD.W     Invalid
000078B0                          OR.B      #$BA,A0
000078B4                          BTST      D2,(A4)+
000078B6                          MOVE.B    #$04,$0003(A0)
000078BC                          MOVE.L    (A7)+,D0
000078BE                          MOVE.B    $FC27,D0
000078C2                          CMP.B     $FC4E,D0
000078C6                          BLE       $00007D98(pc)
000078CA                          RTS
000078CC                          LEA       $000019D4,A1
000078D2                          TST.B     $005B(A0)
000078D6                          BNE       $000078F4
000078D8                          LEA       $00001AAE,A1
000078DE                          BSET      #$00,$0053(A0)
000078E4                          BCLR      #$01,$0053(A0)
000078EA                          BEQ       $000078F4
000078EC                          MOVE.B    $0054(A0),$0022(A0)
000078F2                          RTS
000078F4                          JSR       (A1)
000078F6                          BCC       $000078FC
000078F8                          ADD.W     Invalid
000078FA                          OR.B      #$75,A4
000078FE                          TST.B     $0036(A0)
00007902                          BEQ       $0000790A
00007904                          JSR       $00004AB0(pc)
00007908                          BRA       $00007914
0000790A                          TST.W     $0038(A0)
0000790E                          BNE       $00007914
00007910                          JSR       $00007918(pc)
00007914                          BRA       $000048EA(pc)
00007918                          JSR       $000014E6(pc)
0000791C                          LSL.W     #3,D0
0000791E                          MOVEQ     #$00,D1
00007920                          MOVE.B    $0030(A0),D1
00007924                          ADD.W     D1,D1
00007926                          ADD.W     D1,D1
00007928                          ADD.W     D0,D1
0000792A                          LEA       $00004A70(pc),A1
0000792E                          LEA       $10(A1,D0.W),A1
00007932                          MOVE.W    (A1)+,D1
00007934                          MOVE.W    D1,$0038(A0)
00007938                          LSL.L     #8,D1
0000793A                          AND.L     #$00FFFF00,D1
00007940                          MOVE.L    D1,$0044(A0)
00007944                          MOVE.W    (A1),$003A(A0)
00007948                          LSR.W     #1,D0
0000794A                          LEA       $00004D84(pc),A1
0000794E                          MOVE.L    $00(A1,D0.W),$0048(A0)
00007954                          MOVE.B    #$01,$0036(A0)
0000795A                          RTS
0000795C                          JSR       $000014E6(pc)
00007960                          TST.B     D0
00007962                          BEQ       $00007870(pc)
00007966                          MOVE.W    $0038(A0),D0
0000796A                          JSR       $00007FBE(pc)
0000796E                          MOVE.W    D1,$0038(A0)
00007972                          LSL.L     #8,D1
00007974                          AND.L     #$00FFFF00,D1
0000797A                          MOVE.L    D1,$0044(A0)
0000797E                          MOVE.W    #$270F,$003A(A0)
00007984                          JSR       $000014E6(pc)
00007988                          ADD.W     D0,D0
0000798A                          ADD.W     D0,D0
0000798C                          LEA       $00004D84(pc),A1
00007990                          MOVE.L    $00(A1,D0.W),$0048(A0)
00007996                          MOVE.B    #$01,$0036(A0)
0000799C                          RTS
0000799E                          JSR       $000014E6(pc)
000079A2                          ADD.W     D0,D0
000079A4                          MOVE.W    +$00(D3.W),D0
000079A8                          JMP       +$00(D1.W)
000079AC                          OR.B      #$10,(A4)+
000079B0                          OR.B      #$1C,(A0)
000079B4                          OR.B      #$1C,(A4)+
000079B8                          OR.B      #$10,(A4)+
000079BC                          TST.B     $0057(A0)
000079C0                          BEQ       $000079CE
000079C2                          SUBQ.B    #$1,$0057(A0)
000079C6                          RTS
000079C8                          CLR.B     $0057(A0)
000079CC                          RTS
000079CE                          MOVE.B    $FC27,D0
000079D2                          CMP.B     $FC4E,D0
000079D6                          BLE       $00007D98(pc)
000079DA                          BRA       $00007DB4(pc)
000079DE                          MOVE.B    $0004(A0),D0
000079E2                          BEQ       $000079EA
000079E4                          SUBQ.B    #$1,D0
000079E6                          BEQ       $00007A04
000079E8                          BRA       $00007A06
000079EA                          MOVE.B    $0030(A0),D0
000079EE                          LEA       $00041262,A4
000079F4                          AND.B     #$01,D0
000079F8                          JMP       $00007E40
000079FE                          MOVE.B    #$02,$0004(A0)
00007A04                          RTS
00007A06                          TST.B     $0030(A0)
00007A0A                          BNE       $00007A3E
00007A0C                          ADDQ.W    #$6,$0008(A0)
00007A10                          CMP.W     #$1378,$0008(A0)
00007A16                          BLT       $00007A28
00007A18                          MOVE.W    #$1378,$0008(A0)
00007A1E                          MOVE.B    #$01,$0030(A0)
00007A24                          BRA       $00007DEA(pc)
00007A28                          JSR       $00001C3C(pc)
00007A2C                          BCC       $00007E6C(pc)
00007A30                          ADD.W     Invalid
00007A32                          OR.B      #$7C,A0
00007A36                          OR.B      #$30,D1
00007A3A                          BRA       $00007DEA(pc)
00007A3E                          SUBQ.W    #$6,$0008(A0)
00007A42                          CMP.W     #$11C8,$0008(A0)
00007A48                          BGT       $00007A5A
00007A4A                          MOVE.W    #$11C8,$0008(A0)
00007A50                          MOVE.B    #$00,$0030(A0)
00007A56                          BRA       $00007DEA(pc)
00007A5A                          JSR       $00001C94(pc)
00007A5E                          BCC       $00007E6C(pc)
00007A62                          ADD.W     Invalid
00007A64                          OR.B      #$7C,A0
00007A68                          OR.B      #$30,D0
00007A6C                          BRA       $00007DEA(pc)
00007A70                          MOVEQ     #$00,D0
00007A72                          MOVE.B    $0004(A0),D0
00007A76                          MOVE.W    +$00(D3.W),D0
00007A7A                          JMP       +$00(D1.W)
00007A7E                          OR.B      #$26,A0
00007A82                          OR.W      #$005E,D4
00007A86                          CLR.B     $F0B0
00007A8A                          MOVE.B    #$FF,$F0B1
00007A90                          MOVE.B    #$02,$0004(A0)
00007A96                          MOVE.B    $001E(A0),D0
00007A9A                          CMP.B     $0056(A0),D0
00007A9E                          BNE       $00007D26(pc)
00007AA2                          RTS
00007AA4                          JSR       $00007AE8(pc)
00007AA8                          JSR       $00007B08(pc)
00007AAC                          JSR       $00007B50(pc)
00007AB0                          JSR       $00007BD6(pc)
00007AB4                          MOVE.B    $001E(A0),D0
00007AB8                          CMP.B     $0056(A0),D0
00007ABC                          BNE       $00007D26(pc)
00007AC0                          RTS
00007AC2                          MOVE.B    $0030(A0),D0
00007AC6                          LEA       $00041262,A4
00007ACC                          AND.B     #$01,D0
00007AD0                          JSR       $00007E40(pc)
00007AD4                          MOVE.B    #$06,$0004(A0)
00007ADA                          RTS
00007ADC                          JSR       $00007E6C(pc)
00007AE0                          JSR       $00007C38(pc)
00007AE4                          BRA       $00007C5A(pc)
00007AE8                          MOVE.B    $F0B0,D0
00007AEC                          ADDQ.B    #$0,D0
00007AEE                          LSR.B     #4,D0
00007AF0                          CMP.B     $F0B1,D0
00007AF4                          BEQ       $00007B06
00007AF6                          MOVE.B    D0,$F0B1
00007AFA                          LEA       $0004141A,A4
00007B00                          JMP       $00007E40
00007B06                          RTS
00007B08                          MOVE.B    $F0B0,D0
00007B0C                          ADDQ.B    #$2,D0
00007B0E                          LSR.B     #2,D0
00007B10                          ADD.B     #$10,D0
00007B14                          AND.W     #$003F,D0
00007B18                          ADD.W     D0,D0
00007B1A                          ADD.W     D0,D0
00007B1C                          LEA       $00040430,A1
00007B22                          MOVEQ     #$00,D1
00007B24                          MOVE.W    $00(A1,D0.W),D1
00007B28                          MOVE.W    D1,$F0B2
00007B2C                          SWAP      D1
00007B2E                          ASR.L     #8,D1
00007B30                          ADD.L     Invalid
00007B32                          OR.B      #$A8,A0
00007B36                          OR.B      #$00,A0
00007B3A                          MOVE.W    $00(A1,D1.W),D1
00007B3E                          MOVE.W    D1,$F0B4
00007B42                          SWAP      D1
00007B44                          ASR.L     #8,D1
00007B46                          (CCC     #$1,$000C(A0)
00007B4A                          (CCC     #$1,$000C(A0)
00007B4E                          RTS
00007B50                          BTST      #$04,$E109
00007B56                          BEQ       $00007B92
00007B58                          CMP.B     #$01,$0034(A0)
00007B5E                          BEQ       $00007B8E
00007B60                          BGT       $00007B92
00007B62                          JSR       $00007F1E(pc)
00007B66                          JSR       $00007C22(pc)
00007B6A                          JSR       $00028A1C
00007B70                          BCS       $00007B92
00007B72                          MOVE.B    #$01,$0037(A0)
00007B78                          MOVE.B    #$00,$0006(A1)
00007B7E                          MOVE.W    #$0100,$0000(A1)
00007B84                          MOVE.B    $0030(A0),$0030(A1)
00007B8A                          JSR       $00007B94(pc)
00007B8E                          ADDQ.B    #$1,$0034(A0)
00007B92                          RTS
00007B94                          MOVEQ     #$00,D0
00007B96                          MOVE.B    $F0B0,D0
00007B9A                          ADD.B     #$10,D0
00007B9E                          LSR.B     #5,D0
00007BA0                          ADD.W     D0,D0
00007BA2                          ADD.W     D0,D0
00007BA4                          MOVE.L    +$00(D0.L),A4
00007BA8                          MOVE.B    $0030(A0),D0
00007BAC                          AND.B     #$01,D0
00007BB0                          JMP       $00007E40
00007BB6                          OR.B      #$3A,D4
00007BBA                          OR.B      #$3E,D4
00007BBE                          OR.B      #$42,D4
00007BC2                          OR.B      #$46,D4
00007BC6                          OR.B      #$4A,D4
00007BCA                          OR.B      #$4E,D4
00007BCE                          OR.B      #$52,D4
00007BD2                          OR.B      #$56,D4
00007BD6                          TST.B     $0034(A0)
00007BDA                          BEQ       $00007C20
00007BDC                          JSR       $00007E6C(pc)
00007BE0                          BTST      #$04,$0026(A0)
00007BE6                          BNE       $00007C20
00007BE8                          SUBQ.B    #$1,$0034(A0)
00007BEC                          BEQ       $00007C1A
00007BEE                          JSR       $00007F1E(pc)
00007BF2                          JSR       $00007C22(pc)
00007BF6                          JSR       $00028A1C
00007BFC                          BCS       $00007B94
00007BFE                          MOVE.B    #$01,$0037(A0)
00007C04                          MOVE.B    #$00,$0006(A1)
00007C0A                          MOVE.W    #$0100,$0000(A1)
00007C10                          MOVE.B    $0030(A0),$0030(A1)
00007C16                          BRA       $00007B94(pc)
00007C1A                          MOVE.B    #$FF,$F0B1
00007C20                          RTS
00007C22                          MOVE.B    +$00(D5.W),D1
00007C26                          BMI       $00007C2C
00007C28                          MOVE.B    D1,$0030(A0)
00007C2C                          RTS
00007C2E                          Invalid
00007C30                          OR.B      #$01,D0
00007C34                          BTST      D0,D1
00007C36                          Invalid
00007C38                          MOVE.W    $F0B2,D0
00007C3C                          SWAP      D0
00007C3E                          ASR.L     #8,D0
00007C40                          ADD.L     Invalid
00007C42                          OR.B      #$A8,A0
00007C46                          OR.B      #$38,A0
00007C4A                          Invalid
00007C4C                          SWAP      D0
00007C4E                          ASR.L     #8,D0
00007C50                          (CCC     #$0,$000C(A0)
00007C54                          (CCC     #$0,$000C(A0)
00007C58                          RTS
00007C5A                          BSET      #$00,$0053(A0)
00007C60                          BCLR      #$01,$0053(A0)
00007C66                          BNE       $00007C8A
00007C68                          JSR       $00001AE2(pc)
00007C6C                          BCS       $00007C98
00007C6E                          JSR       $000019F8(pc)
00007C72                          BCS       $00007C98
00007C74                          TST.W     $F0B2
00007C78                          BMI       $00007C82
00007C7A                          JSR       $00001C3C(pc)
00007C7E                          BCS       $00007C9E
00007C80                          RTS
00007C82                          JSR       $00001C94(pc)
00007C86                          BCS       $00007C9E
00007C88                          RTS
00007C8A                          MOVE.B    $0054(A0),$0022(A0)
00007C90                          MOVE.B    $0054(A0),$0023(A0)
00007C96                          BRA       $00007CA2
00007C98                          ADD.W     Invalid
00007C9A                          OR.B      #$04,A4
00007C9E                          ADD.W     Invalid
00007CA0                          OR.B      #$38,A0
00007CA4                          Invalid
00007CA6                          BEQ       $00007D5C(pc)
00007CAA                          MOVE.B    #$00,$0006(A0)
00007CB0                          BRA       $00007D7C(pc)
00007CB4                          MOVEQ     #$00,D0
00007CB6                          MOVE.B    $0004(A0),D0
00007CBA                          MOVE.W    +$00(D3.W),D0
00007CBE                          JMP       +$00(D1.W)
00007CC2                          OR.B      #$22,A0
00007CC6                          OR.W      #$0058,D4
00007CCA                          BSET      #$00,$0053(A0)
00007CD0                          MOVEQ     #$00,D0
00007CD2                          LEA       $00040606,A4
00007CD8                          JSR       $00007E40(pc)
00007CDC                          MOVE.B    #$02,$0004(A0)
00007CE2                          RTS
00007CE4                          BSET      #$00,$0053(A0)
00007CEA                          MOVE.B    $0054(A0),$0022(A0)
00007CF0                          BTST      #$05,$E109
00007CF6                          BNE       $00005EAA(pc)
00007CFA                          BTST      #$06,$E109
00007D00                          BNE       $00005EAA(pc)
00007D04                          RTS
00007D06                          MOVEQ     #$00,D0
00007D08                          LEA       $0004119A,A4
00007D0E                          JSR       $00007E40(pc)
00007D12                          MOVE.B    #$06,$0004(A0)
00007D18                          RTS
00007D1A                          ADD.W     #$000A,$0008(A0)
00007D20                          JMP       $00007E6C
00007D26                          MOVE.B    $001E(A0),$0056(A0)
00007D2C                          TST.B     $F0AE
00007D30                          BNE       $00007D52
00007D32                          JSR       $000201A2
00007D38                          JSR       $000015B6
00007D3E                          ADDQ.B    #$1,$FC4E
00007D42                          MOVE.B    $FC27,D0
00007D46                          CMP.B     $FC4E,D0
00007D4A                          BLE       $00007D96
00007D4C                          MOVE.W    #$0096,$FC4C
00007D52                          RTS
00007D54                          MOVEQ     #$02,D0
00007D56                          BRA       $00007D5E
00007D58                          MOVEQ     #$01,D0
00007D5A                          BRA       $00007D5E
00007D5C                          MOVEQ     #$00,D0
00007D5E                          MOVE.B    D0,$0006(A0)
00007D62                          JSR       $000201A2
00007D68                          JSR       $000015B6
00007D6E                          ADDQ.B    #$1,$FC4E
00007D72                          MOVE.B    $FC27,D0
00007D76                          CMP.B     $FC4E,D0
00007D7A                          BLE       $00007D96
00007D7C                          MOVE.W    #$0096,$FC4C
00007D82                          MOVE.B    #$1C,$0003(A0)
00007D88                          MOVE.B    #$00,$0004(A0)
00007D8E                          MOVE.B    #$00,$0005(A0)
00007D94                          RTS
00007D96                          MOVE.L    (A7)+,D0
00007D98                          MOVE.L    #$04000000,$0002(A0)
00007DA0                          JSR       $000201AC
00007DA6                          RTS
00007DA8                          MOVE.B    #$00,$0036(A0)
00007DAE                          MOVE.W    #$0000,$0038(A0)
00007DB4                          MOVE.B    #$00,$0003(A0)
00007DBA                          BRA       $00007DD6
00007DBC                          MOVE.B    #$00,$0003(A0)
00007DC2                          BRA       $00007DD0
00007DC4                          MOVE.B    #$00,$0036(A0)
00007DCA                          MOVE.W    #$0000,$0038(A0)
00007DD0                          MOVE.B    #$00,$0005(A0)
00007DD6                          MOVE.B    #$00,$0004(A0)
00007DDC                          MOVE.B    #$FF,$0016(A0)
00007DE2                          MOVE.B    #$FF,$0033(A0)
00007DE8                          RTS
00007DEA                          MOVE.B    #$00,$0005(A0)
00007DF0                          MOVE.B    #$0C,$0004(A0)
00007DF6                          BRA       $00007E0A
00007DF8                          MOVE.B    #$0C,$0004(A0)
00007DFE                          CLR.B     $0034(A0)
00007E02                          BRA       $00007E0A
00007E04                          MOVE.B    #$04,$0004(A0)
00007E0A                          JSR       $000201B4
00007E10                          MOVEQ     #$00,D7
00007E12                          MOVE.L    D7,$0044(A0)
00007E16                          MOVE.W    D7,$003C(A0)
00007E1A                          MOVE.W    D7,$0038(A0)
00007E1E                          MOVE.B    D7,$0036(A0)
00007E22                          MOVE.W    #$0030,$003E(A0)
00007E28                          MOVE.B    #$0C,$0003(A0)
00007E2E                          MOVE.B    $0030(A0),D0
00007E32                          LEA       $00041192,A4
00007E38                          BRA       $00007E40(pc)
00007E3C                          MOVE.B    $0016(A0),D0
00007E40                          EXT.W     D0
00007E42                          ADD.W     D0,D0
00007E44                          MOVE.W    $00(A4,D0.W),D0
00007E48                          MOVE.W    D0,$0014(A0)
00007E4C                          MOVE.W    $00(A6,D0.W),D0
00007E50                          MOVE.W    D0,$0012(A0)
00007E54                          MOVE.B    #$01,$0011(A0)
00007E5A                          LEA       $00(A6,A0.W),A4
00007E5E                          MOVE.B    (A4)+,$0010(A0)
00007E62                          MOVE.B    (A4)+,$0007(A0)
00007E66                          MOVE.B    (A4)+,$0026(A0)
00007E6A                          RTS
00007E6C                          SUBQ.B    #$1,$0011(A0)
00007E70                          BNE       $00007EA8
00007E72                          MOVE.W    $0014(A0),D0
00007E76                          MOVE.W    $00(A6,D0.W),D1
00007E7A                          MOVE.W    D1,$0012(A0)
00007E7E                          MOVE.B    $10(A6,A0.W),$0010(A0)
00007E84                          MOVE.B    $10(A6,D1.W),$0007(A0)
00007E8A                          MOVE.B    $10(A6,A1.W),$0026(A0)
00007E90                          ADDQ.W    #$4,$0014(A0)
00007E94                          MOVE.B    $00(A6,A1.W),D1
00007E98                          BPL       $00007EA4
00007E9A                          MOVE.W    $00(A6,D2.W),$0014(A0)
00007EA0                          AND.B     #$7F,D1
00007EA4                          MOVE.B    D1,$0011(A0)
00007EA8                          RTS
00007EAA                          MOVE.B    $0016(A0),D0
00007EAE                          EXT.W     D0
00007EB0                          ADD.W     D0,D0
00007EB2                          MOVE.W    $00(A4,D0.W),D0
00007EB6                          MOVE.W    D0,$0014(A0)
00007EBA                          MOVE.W    $00(A6,D0.W),D0
00007EBE                          MOVE.W    D0,$0012(A0)
00007EC2                          MOVE.B    #$01,$0011(A0)
00007EC8                          LEA       $00(A6,D0.W),A4
00007ECC                          MOVE.B    $0001(A4),$0010(A0)
00007ED2                          MOVE.B    $0004(A4),$0007(A0)
00007ED8                          MOVE.B    $0002(A4),$0026(A0)
00007EDE                          RTS
00007EE0                          SUBQ.B    #$1,$0011(A0)
00007EE4                          BNE       $00007F1C
00007EE6                          MOVE.W    $0014(A0),D0
00007EEA                          MOVE.W    $00(A6,D0.W),D1
00007EEE                          MOVE.W    D1,$0012(A0)
00007EF2                          MOVE.B    $10(A6,D2.W),$0007(A0)
00007EF8                          MOVE.B    $10(A6,A0.W),$0010(A0)
00007EFE                          MOVE.B    $10(A6,D1.W),$0026(A0)
00007F04                          ADDQ.W    #$4,$0014(A0)
00007F08                          MOVE.B    $00(A6,A1.W),D1
00007F0C                          BPL       $00007F18
00007F0E                          MOVE.W    $00(A6,D2.W),$0014(A0)
00007F14                          AND.B     #$7F,D1
00007F18                          MOVE.B    D1,$0011(A0)
00007F1C                          RTS
00007F1E                          MOVE.B    $E108,D0
00007F22                          AND.W     #$000F,D0
00007F26                          TST.B     $005B(A0)
00007F2A                          BEQ       $00007F30
00007F2C                          ADD.W     #$0010,D0
00007F30                          MOVE.B    +$00(D4.W),D0
00007F34                          MOVE.B    D0,$0016(A0)
00007F38                          RTS
00007F3A                          BTST      #$08,D2
00007F3E                          SUB.B     #$08,D3
00007F42                          OR.B      #$08,D1
00007F46                          BTST      #$08,A0
00007F4A                          BTST      #$08,D2
00007F4E                          OR.B      #$08,D1
00007F52                          SUB.B     #$08,D3
00007F56                          BTST      #$08,A0
00007F5A                          MOVE.L    $0044(A0),D0
00007F5E                          ADD.L     Invalid
00007F60                          OR.B      #$75,A0
00007F64                          BSR       $00007F5A
00007F66                          MOVEQ     #$00,D0
00007F68                          MOVE.W    $003C(A0),D0
00007F6C                          SWAP      D0
00007F6E                          ASR.L     #8,D0
00007F70                          TST.B     $005B(A0)
00007F74                          BNE       $00007F78
00007F76                          NEG.L     D0
00007F78                          ADD.L     Invalid
00007F7A                          OR.B      #$68,A4
00007F7E                          Invalid
00007F80                          OR.B      #$08,CCR
00007F84                          MOVE.W    $003E(A0),D0
00007F88                          (CCC     #$0,$003C(A0)
00007F8C                          RTS
00007F8E                          MOVE.W    $0038(A0),D0
00007F92                          MOVE.B    $0022(A0),D1
00007F96                          TST.B     $005B(A0)
00007F9A                          BEQ       $00007FA0
00007F9C                          MOVE.B    $0023(A0),D1
00007FA0                          AND.W     #$0007,D1
00007FA4                          ADD.W     D1,D1
00007FA6                          MOVE.W    +$10(D3.W),D1
00007FAA                          JMP       +$10(D1.W)
00007FAE                          OR.L      #$00BA00BE,$00008094(pc)
00007FB6                          MOVEP.W   $010E(A6),D0
00007FBA                          Invalid
00007FBC                          OR.L      #$72007400,
00007FC2                          MOVE.B    $0022(A0),D2
00007FC6                          MOVE.B    $0031(A0),D1
00007FCA                          TST.B     $005B(A0)
00007FCE                          BEQ       $00007FD8
00007FD0                          MOVE.B    $0023(A0),D2
00007FD4                          MOVE.B    $0032(A0),D1
00007FD8                          LSL.W     #3,D2
00007FDA                          ADD.B     D1,D2
00007FDC                          MOVE.B    +$20(D5.W),D2
00007FE0                          MOVE.W    +$20(D3.W),D2
00007FE4                          JMP       +$20(D1.W)
00007FE8                          MOVE.B    (A0)+,D4
00007FEA                          MOVE.B    (A0)+,D4
00007FEC                          MOVE.B    (A0)+,D4
00007FEE                          MOVE.B    (A0)+,D4
00007FF0                          MOVE.B    (A0)+,D4
00007FF2                          ADD.B     #$12,A4
00007FF6                          CMP.B     #$00,D6
00007FFA                          MOVE.B    A6,D4
00007FFC                          MOVE.B    (A4),D2
00007FFE                          .B        #$02,(A0)+
00008002                          BTST      #$16,(A0)+
00008006                          MOVE.B    A0,D4
00008008                          MOVE.B    D4,D4
0000800A                          EOR.B     #$18,(A0)
0000800E                          MOVE.B    A2,D0
00008010                          MOVE.B    D4,D4
00008012                          EOR.B     #$18,(A0)
00008016                          MOVE.B    A2,D0
00008018                          MOVE.B    D2,D4
0000801A                          BTST      #$16,(A0)+
0000801E                          MOVE.B    A0,D4
00008020                          MOVE.B    D0,D4
00008022                          MOVE.B    A6,D4
00008024                          MOVE.B    (A4),D2
00008026                          .B        #$1E,(A0)+
0000802A                          OR.B      #$32,-(A6)
0000802E                          OR.W      #$004C,D4
00008032                          OR.W      #$006A,(A4)+
00008036                          OR.W      #$0086,$00(A6,D2.L)
0000803C                          OR.L      #$00B8001A,-(A6)
00008042                          MOVE.W    D0,D1
00008044                          RTS
00008046                          MOVE.W    D0,D1
00008048                          LSR.W     #4,D0
0000804A                          SUB.W     D0,D1
0000804C                          RTS
0000804E                          MOVE.W    D0,D1
00008050                          LSR.W     #2,D0
00008052                          SUB.W     D0,D1
00008054                          LSR.W     #5,D0
00008056                          ADD.W     D0,D1
00008058                          RTS
0000805A                          LSR.W     #1,D0
0000805C                          MOVE.W    D0,D1
0000805E                          LSR.W     #7,D0
00008060                          SUB.W     D0,D1
00008062                          LSR.W     #1,D0
00008064                          SUB.W     D0,D1
00008066                          RTS
00008068                          MOVE.W    D0,D1
0000806A                          RTS
0000806C                          MOVE.W    D0,D1
0000806E                          LSR.W     #4,D0
00008070                          ADD.W     D0,D1
00008072                          RTS
00008074                          MOVE.W    D0,D1
00008076                          LSR.W     #2,D0
00008078                          SUB.W     D0,D1
0000807A                          LSR.W     #2,D0
0000807C                          ADD.W     D0,D1
0000807E                          LSR.W     #4,D0
00008080                          SUB.W     D0,D1
00008082                          RTS
00008084                          LSR.W     #1,D0
00008086                          MOVE.W    D0,D1
00008088                          LSR.W     #5,D0
0000808A                          ADD.W     D0,D1
0000808C                          LSR.W     #2,D0
0000808E                          ADD.W     D0,D1
00008090                          RTS
00008092                          MOVE.W    D0,D1
00008094                          LSR.W     #2,D0
00008096                          ADD.W     D0,D1
00008098                          LSR.W     #2,D0
0000809A                          ADD.W     D0,D1
0000809C                          RTS
0000809E                          MOVE.W    D0,D1
000080A0                          LSR.W     #2,D0
000080A2                          ADD.W     D0,D1
000080A4                          LSR.W     #4,D0
000080A6                          SUB.W     D0,D1
000080A8                          LSR.W     #2,D0
000080AA                          SUB.W     D0,D1
000080AC                          RTS
000080AE                          LSR.W     #1,D0
000080B0                          MOVE.W    D0,D1
000080B2                          LSR.W     #2,D0
000080B4                          ADD.W     D0,D1
000080B6                          LSR.W     #3,D0
000080B8                          ADD.W     D0,D1
000080BA                          RTS
000080BC                          MOVE.W    D0,D1
000080BE                          ADD.W     D0,D1
000080C0                          LSR.W     #4,D0
000080C2                          ADD.W     D0,D1
000080C4                          LSR.W     #2,D0
000080C6                          SUB.W     D0,D1
000080C8                          LSR.W     #2,D0
000080CA                          SUB.W     D0,D1
000080CC                          RTS
000080CE                          MOVE.W    D0,D1
000080D0                          ADD.W     D0,D1
000080D2                          LSR.W     #4,D0
000080D4                          SUB.W     D0,D1
000080D6                          LSR.W     #2,D0
000080D8                          SUB.W     D0,D1
000080DA                          LSR.W     #2,D0
000080DC                          SUB.W     D0,D1
000080DE                          RTS
000080E0                          MOVE.W    D0,D1
000080E2                          LSR.W     #1,D0
000080E4                          ADD.W     D0,D1
000080E6                          LSR.W     #3,D0
000080E8                          ADD.W     D0,D1
000080EA                          LSR.W     #3,D0
000080EC                          SUB.W     D0,D1
000080EE                          RTS
000080F0                          TST.B     $0040(A0)
000080F4                          BNE       $00008110
000080F6                          MOVE.B    #$02,$0040(A0)
000080FC                          MOVE.W    #$0101,$0000(A0)
00008102                          MOVE.B    $0030(A0),D0
00008106                          LEA       $00041192,A4
0000810C                          BRA       $00007E40(pc)
00008110                          MOVE.W    $0008(A0),D0
00008114                          SUB.W     $EA08,D0
00008118                          SUB.W     #$0088,D0
0000811C                          CMP.W     #$0010,D0
00008120                          BLS       $0000812E
00008122                          ADDQ.W    #$3,$0008(A0)
00008126                          TST.W     D0
00008128                          BMI       $0000812E
0000812A                          SUBQ.W    #$6,$0008(A0)
0000812E                          MOVE.W    $000C(A0),D0
00008132                          SUB.W     $EA0C,D0
00008136                          SUB.W     #$0030,D0
0000813A                          CMP.W     #$0010,D0
0000813E                          BLS       $0000814E
00008140                          SUBQ.W    #$6,$000C(A0)
00008144                          TST.W     D0
00008146                          BPL       $0000814E
00008148                          ADD.W     #$0009,$000C(A0)
0000814E                          RTS
00008150                          LEA       $EA00,A0
00008154                          BSR       $00008160
00008156                          LEA       $EA80,A0
0000815A                          JSR       $000086D2(pc)
0000815E                          RTS
00008160                          AND.W     #$1FFF,$0008(A0)
00008166                          AND.W     #$0FFF,$000C(A0)
0000816C                          MOVE.B    $0001(A0),D0
00008170                          AND.B     #$1E,D0
00008174                          BEQ       $0000819E
00008176                          MOVE.W    $0008(A0),$0010(A0)
0000817C                          MOVE.W    $000C(A0),$0012(A0)
00008182                          BTST      #$03,$0001(A0)
00008188                          BNE       $00008474(pc)
0000818C                          BTST      #$04,$0001(A0)
00008192                          BNE       $00008534(pc)
00008196                          BTST      #$01,$0001(A0)
0000819C                          BNE       $000081A0
0000819E                          RTS
000081A0                          MOVE.W    $0012(A0),-(A7)
000081A4                          BSR       $000081C4
000081A6                          MOVE.W    (A7)+,$0012(A0)
000081AA                          MOVE.W    $A00C,D0
000081AE                          SUB.W     $000C(A0),D0
000081B2                          CMP.W     $0038(A0),D0
000081B6                          BLT       $00008328(pc)
000081BA                          CMP.W     $0036(A0),D0
000081BE                          BGT       $000083CE(pc)
000081C2                          RTS
000081C4                          MOVE.W    $A008,D0
000081C8                          SUB.W     $0008(A0),D0
000081CC                          CMP.W     $0032(A0),D0
000081D0                          BGT       $000081DE(pc)
000081D4                          CMP.W     $0034(A0),D0
000081D8                          BLT       $00008284(pc)
000081DC                          RTS
000081DE                          MOVE.W    #$0020,D5
000081E2                          SUB.W     $0032(A0),D0
000081E6                          CMP.W     #$0040,D0
000081EA                          BCC       $000081F4
000081EC                          MOVE.W    D0,D5
000081EE                          AND.W     #$0038,D5
000081F2                          LSR.W     #1,D5
000081F4                          LEA       $00008CDA(pc),A1
000081F8                          MOVE.L    $50(A1,D0.W),D0
000081FC                          MOVE.L    D0,$E008
00008200                          ADD.L     $0008(A0),D0
00008204                          SWAP      D0
00008206                          CMP.W     $002A(A0),D0
0000820A                          BGE       $00008282
0000820C                          SWAP      D0
0000820E                          MOVE.L    D0,$0008(A0)
00008212                          MOVE.W    $0010(A0),D0
00008216                          MOVE.W    $0008(A0),D1
0000821A                          AND.W     #$000F,D0
0000821E                          AND.W     #$000F,D1
00008222                          CMP.W     D0,D1
00008224                          BHI       $00008240
00008226                          MOVE.W    $0008(A0),$0010(A0)
0000822C                          MOVE.W    $000C(A0),$0012(A0)
00008232                          ADD.W     #$01F0,$0010(A0)
00008238                          MOVE.W    D5,-(A7)
0000823A                          JSR       $00008650(pc)
0000823E                          MOVE.W    (A7)+,D5
00008240                          BTST      #$01,$0001(A0)
00008246                          BEQ       $00008282
00008248                          BTST      #$02,$EA81
0000824E                          BEQ       $00008264
00008250                          MOVE.L    A0,-(A7)
00008252                          LEA       $EA80,A0
00008256                          MOVE.W    $0008(A0),$0010(A0)
0000825C                          JSR       $00008760(pc)
00008260                          MOVE.L    (A7)+,A0
00008262                          RTS
00008264                          BTST      #$07,$EA81
0000826A                          BEQ       $00008282
0000826C                          MOVE.L    A0,-(A7)
0000826E                          LEA       $EA80,A0
00008272                          MOVE.W    $0008(A0),$0010(A0)
00008278                          MOVE.L    $E008,D0
0000827C                          JSR       $00008766(pc)
00008280                          MOVE.L    (A7)+,A0
00008282                          RTS
00008284                          MOVE.W    #$0020,D5
00008288                          MOVE.W    $0034(A0),D1
0000828C                          SUB.W     D0,D1
0000828E                          CMP.W     #$0040,D1
00008292                          BCC       $0000829C
00008294                          MOVE.W    D1,D5
00008296                          AND.W     #$0038,D5
0000829A                          LSR.W     #1,D5
0000829C                          LEA       $00008D02(pc),A1
000082A0                          MOVE.L    $50(A1,D0.W),D0
000082A4                          MOVE.L    D0,$E008
000082A8                          ADD.L     $0008(A0),D0
000082AC                          SWAP      D0
000082AE                          CMP.W     $002C(A0),D0
000082B2                          BLE       $00008326
000082B4                          SWAP      D0
000082B6                          MOVE.L    D0,$0008(A0)
000082BA                          MOVE.W    $0010(A0),D0
000082BE                          MOVE.W    $0008(A0),D1
000082C2                          AND.W     #$000F,D0
000082C6                          AND.W     #$000F,D1
000082CA                          CMP.W     D0,D1
000082CC                          BCS       $000082E2
000082CE                          MOVE.W    $0008(A0),$0010(A0)
000082D4                          MOVE.W    $000C(A0),$0012(A0)
000082DA                          MOVE.W    D5,-(A7)
000082DC                          JSR       $00008650(pc)
000082E0                          MOVE.W    (A7)+,D5
000082E2                          BTST      #$01,$0001(A0)
000082E8                          BEQ       $00008326
000082EA                          BTST      #$02,$EA81
000082F0                          BEQ       $00008306
000082F2                          MOVE.L    A0,-(A7)
000082F4                          LEA       $EA80,A0
000082F8                          MOVE.W    $0008(A0),$0010(A0)
000082FE                          JSR       $000087D6(pc)
00008302                          MOVE.L    (A7)+,A0
00008304                          RTS
00008306                          BTST      #$07,$EA81
0000830C                          BEQ       $00008326
0000830E                          MOVE.L    A0,-(A7)
00008310                          LEA       $EA80,A0
00008314                          MOVE.W    $0008(A0),$0010(A0)
0000831A                          MOVE.L    $E008,D0
0000831E                          NEG.L     D0
00008320                          JSR       $000087DE(pc)
00008324                          MOVE.L    (A7)+,A0
00008326                          RTS
00008328                          MOVE.W    #$0020,D5
0000832C                          MOVE.W    $0038(A0),D1
00008330                          SUB.W     D0,D1
00008332                          CMP.W     #$0040,D1
00008336                          BCC       $00008340
00008338                          MOVE.W    D1,D5
0000833A                          AND.W     #$0038,D5
0000833E                          LSR.W     #1,D5
00008340                          LEA       $00008D02(pc),A1
00008344                          MOVE.L    $50(A1,D0.W),D0
00008348                          MOVE.L    D0,$E00C
0000834C                          ADD.L     $000C(A0),D0
00008350                          SWAP      D0
00008352                          CMP.W     $0030(A0),D0
00008356                          BLE       $000083CC
00008358                          SWAP      D0
0000835A                          MOVE.L    D0,$000C(A0)
0000835E                          MOVE.W    $0012(A0),D0
00008362                          MOVE.W    $000C(A0),D1
00008366                          AND.W     #$000F,D0
0000836A                          AND.W     #$000F,D1
0000836E                          CMP.W     D0,D1
00008370                          BCS       $00008386
00008372                          MOVE.W    $0008(A0),$0010(A0)
00008378                          MOVE.W    $000C(A0),$0012(A0)
0000837E                          MOVE.W    D5,-(A7)
00008380                          JSR       $000085CC(pc)
00008384                          MOVE.W    (A7)+,D5
00008386                          BTST      #$01,$0001(A0)
0000838C                          BEQ       $000083CC
0000838E                          BTST      #$02,$EA81
00008394                          BEQ       $000083AC
00008396                          MOVE.L    A0,-(A7)
00008398                          LEA       $EA80,A0
0000839C                          MOVE.W    $000C(A0),$0012(A0)
000083A2                          JSR       $00008828
000083A8                          MOVE.L    (A7)+,A0
000083AA                          RTS
000083AC                          BTST      #$07,$EA81
000083B2                          BEQ       $000083CC
000083B4                          MOVE.L    A0,-(A7)
000083B6                          LEA       $EA80,A0
000083BA                          MOVE.W    $000C(A0),$0012(A0)
000083C0                          MOVE.L    $E00C,D0
000083C4                          NEG.L     D0
000083C6                          JSR       $00008830(pc)
000083CA                          MOVE.L    (A7)+,A0
000083CC                          RTS
000083CE                          MOVE.W    #$0020,D5
000083D2                          SUB.W     $0036(A0),D0
000083D6                          CMP.W     #$0040,D0
000083DA                          BCC       $000083E4
000083DC                          MOVE.W    D0,D5
000083DE                          AND.W     #$0038,D5
000083E2                          LSR.W     #1,D5
000083E4                          LEA       $00008CDE(pc),A1
000083E8                          MOVE.L    $50(A1,D0.W),D0
000083EC                          MOVE.L    D0,$E00C
000083F0                          ADD.L     $000C(A0),D0
000083F4                          SWAP      D0
000083F6                          CMP.W     $002E(A0),D0
000083FA                          BGE       $00008472
000083FC                          SWAP      D0
000083FE                          MOVE.L    D0,$000C(A0)
00008402                          MOVE.W    $0012(A0),D0
00008406                          MOVE.W    $000C(A0),D1
0000840A                          AND.W     #$000F,D0
0000840E                          AND.W     #$000F,D1
00008412                          CMP.W     D0,D1
00008414                          BHI       $00008430
00008416                          MOVE.W    $0008(A0),$0010(A0)
0000841C                          MOVE.W    $000C(A0),$0012(A0)
00008422                          ADD.W     #$00F0,$0012(A0)
00008428                          MOVE.W    D5,-(A7)
0000842A                          JSR       $000085CC(pc)
0000842E                          MOVE.W    (A7)+,D5
00008430                          BTST      #$01,$0001(A0)
00008436                          BEQ       $00008472
00008438                          BTST      #$02,$EA81
0000843E                          BEQ       $00008454
00008440                          MOVE.L    A0,-(A7)
00008442                          LEA       $EA80,A0
00008446                          MOVE.W    $000C(A0),$0012(A0)
0000844C                          JSR       $00008878(pc)
00008450                          MOVE.L    (A7)+,A0
00008452                          RTS
00008454                          BTST      #$07,$EA81
0000845A                          BEQ       $00008472
0000845C                          MOVE.L    A0,-(A7)
0000845E                          LEA       $EA80,A0
00008462                          MOVE.W    $000C(A0),$0012(A0)
00008468                          MOVE.L    $E00C,D0
0000846C                          JSR       $00008880(pc)
00008470                          MOVE.L    (A7)+,A0
00008472                          RTS
00008474                          MOVEQ     #$00,D0
00008476                          MOVE.B    $0006(A0),D0
0000847A                          ADD.W     D0,D0
0000847C                          LEA       $000090CE(pc),A6
00008480                          MOVE.W    $00(A6,D0.W),$0018(A0)
00008486                          JSR       $00028870
0000848C                          MOVE.B    $0016(A0),D0
00008490                          EXT.W     D0
00008492                          ADD.W     D0,D0
00008494                          MOVE.W    +$00(D3.W),D0
00008498                          JMP       +$00(D1.W)
0000849C                          OR.B      #$28,(A0)
000084A0                          OR.B      #$3C,(A4)+
000084A4                          OR.B      #$50,(A6)
000084A8                          OR.B      #$64,-(A2)
000084AC                          BSR       $00008524
000084AE                          BRA       $0000835E(pc)
000084B2                          BSR       $00008524
000084B4                          BRA       $00008402(pc)
000084B8                          BSR       $00008514
000084BA                          BRA       $00008212(pc)
000084BE                          BSR       $00008514
000084C0                          BRA       $000082BA(pc)
000084C4                          MOVE.W    $0010(A0),-(A7)
000084C8                          BSR       $00008524
000084CA                          JSR       $0000835E(pc)
000084CE                          MOVE.W    (A7)+,$0010(A0)
000084D2                          BSR       $00008514
000084D4                          BRA       $00008212(pc)
000084D8                          MOVE.W    $0010(A0),-(A7)
000084DC                          BSR       $00008524
000084DE                          JSR       $00008402(pc)
000084E2                          MOVE.W    (A7)+,$0010(A0)
000084E6                          BSR       $00008514
000084E8                          BRA       $00008212(pc)
000084EC                          MOVE.W    $0010(A0),-(A7)
000084F0                          BSR       $00008524
000084F2                          JSR       $00008402(pc)
000084F6                          MOVE.W    (A7)+,$0010(A0)
000084FA                          BSR       $00008514
000084FC                          BRA       $000082BA(pc)
00008500                          MOVE.W    $0010(A0),-(A7)
00008504                          BSR       $00008524
00008506                          JSR       $0000835E(pc)
0000850A                          MOVE.W    (A7)+,$0010(A0)
0000850E                          BSR       $00008514
00008510                          BRA       $000082BA(pc)
00008514                          MOVE.W    $0008(A0),D5
00008518                          SUB.W     $0010(A0),D5
0000851C                          AND.W     #$0038,D5
00008520                          LSR.W     #1,D5
00008522                          RTS
00008524                          MOVE.W    $000C(A0),D5
00008528                          SUB.W     $0012(A0),D5
0000852C                          AND.W     #$0038,D5
00008530                          LSR.W     #1,D5
00008532                          RTS
00008534                          MOVE.W    $0012(A0),-(A7)
00008538                          JSR       $00008564(pc)
0000853C                          MOVE.W    (A7)+,$0012(A0)
00008540                          MOVEQ     #$00,D0
00008542                          MOVE.W    $003A(A0),D0
00008546                          BEQ       $00008562
00008548                          BPL       $00008558
0000854A                          MOVE.W    #$0000,$003A(A0)
00008550                          MOVEQ     #$00,D1
00008552                          SUB.W     D0,D1
00008554                          BRA       $000083DC(pc)
00008558                          MOVE.W    #$0000,$003A(A0)
0000855E                          BRA       $00008338(pc)
00008562                          RTS
00008564                          MOVE.W    #$0020,D5
00008568                          MOVEQ     #$00,D0
0000856A                          MOVE.W    $003C(A0),D0
0000856E                          BEQ       $00008562
00008570                          BPL       $00008574
00008572                          BRA       $0000857E
00008574                          MOVE.W    #$0000,$003C(A0)
0000857A                          BRA       $000081EC(pc)
0000857E                          MOVE.W    #$0000,$003C(A0)
00008584                          MOVEQ     #$00,D1
00008586                          SUB.W     D0,D1
00008588                          BRA       $00008294(pc)
0000858C                          LEA       $EA00,A0
00008590                          MOVE.W    $0008(A0),$0010(A0)
00008596                          MOVE.W    $000C(A0),$0012(A0)
0000859C                          MOVE.W    #$000F,D6
000085A0                          MOVE.W    $0010(A0),-(A7)
000085A4                          MOVE.W    $0012(A0),-(A7)
000085A8                          MOVEM.L   D6/A0,-(A7)
000085AC                          JSR       $000085CC(pc)
000085B0                          JSR       $00008C3E(pc)
000085B4                          MOVEM.L   (A7)+,D6/A0
000085B8                          MOVE.W    (A7)+,$0012(A0)
000085BC                          MOVE.W    (A7)+,$0010(A0)
000085C0                          ADD.W     #$0010,$0012(A0)
000085C6                          DBRA      D6,$000085A0
000085CA                          RTS
000085CC                          LEA       $9800,A2
000085D0                          MOVE.W    $0010(A0),D0
000085D4                          MOVE.W    $0012(A0),D1
000085D8                          MOVE.W    D0,D2
000085DA                          MOVE.W    D1,D3
000085DC                          LSL.W     #4,D3
000085DE                          AND.W     #$0F00,D3
000085E2                          LSR.W     #2,D2
000085E4                          AND.W     #$007C,D2
000085E8                          ADD.W     D3,D2
000085EA                          OR.W      #$4000,D2
000085EE                          MOVE.W    D2,(A2)+
000085F0                          MOVEQ     #$10,D2
000085F2                          MOVEQ     #$1F,D3
000085F4                          MOVE.W    D0,D4
000085F6                          MOVE.W    D1,D5
000085F8                          LEA       $EB00,A6
000085FC                          LSR.W     #8,D4
000085FE                          LSR.W     #3,D5
00008600                          AND.W     #$FFE0,D5
00008604                          ADD.W     D5,D4
00008606                          MOVE.B    $40(A6,D0.W),D4
0000860A                          LEA       $00FF0000,A6
00008610                          BCLR      #$07,D4
00008614                          BEQ       $0000861C
00008616                          LEA       $00FF3000,A6
0000861C                          LSL.W     #8,D4
0000861E                          ADD.W     D4,D4
00008620                          MOVE.W    D0,D5
00008622                          LSR.W     #3,D5
00008624                          AND.W     #$001E,D5
00008628                          ADD.W     D5,D4
0000862A                          MOVE.W    D1,D5
0000862C                          ADD.W     D5,D5
0000862E                          AND.W     #$01E0,D5
00008632                          ADD.W     D5,D4
00008634                          MOVE.W    $40(A6,D0.W),D4
00008638                          MOVE.L    $E018,A6
0000863C                          LSL.W     #3,D4
0000863E                          MOVE.L    $40(A6,D0.W),(A2)+
00008642                          MOVE.L    $40(A6,D2.W),$007C(A2)
00008648                          ADD.W     D2,D0
0000864A                          DBRA      D3,$000085F4
0000864E                          RTS
00008650                          LEA       $9902,A2
00008654                          MOVE.W    $0010(A0),D0
00008658                          MOVE.W    $0012(A0),D1
0000865C                          MOVE.W    D0,D2
0000865E                          MOVE.W    D1,D3
00008660                          LSL.W     #4,D3
00008662                          AND.W     #$0F00,D3
00008666                          LSR.W     #2,D2
00008668                          AND.W     #$007C,D2
0000866C                          ADD.W     D3,D2
0000866E                          OR.W      #$4000,D2
00008672                          MOVE.W    D2,(A2)+
00008674                          MOVEQ     #$10,D2
00008676                          MOVEQ     #$0F,D3
00008678                          MOVE.W    D0,D4
0000867A                          MOVE.W    D1,D5
0000867C                          LEA       $EB00,A6
00008680                          LSR.W     #8,D4
00008682                          LSR.W     #3,D5
00008684                          AND.W     #$FFE0,D5
00008688                          ADD.W     D5,D4
0000868A                          MOVE.B    $40(A6,D0.W),D4
0000868E                          LEA       $00FF0000,A6
00008694                          BCLR      #$07,D4
00008698                          BEQ       $000086A0
0000869A                          LEA       $00FF3000,A6
000086A0                          LSL.W     #8,D4
000086A2                          ADD.W     D4,D4
000086A4                          MOVE.W    D0,D5
000086A6                          LSR.W     #3,D5
000086A8                          AND.W     #$001E,D5
000086AC                          ADD.W     D5,D4
000086AE                          MOVE.W    D1,D5
000086B0                          ADD.W     D5,D5
000086B2                          AND.W     #$01E0,D5
000086B6                          ADD.W     D5,D4
000086B8                          MOVE.W    $40(A6,D0.W),D4
000086BC                          MOVE.L    $E018,A6
000086C0                          LSL.W     #3,D4
000086C2                          MOVE.L    $40(A6,D0.W),(A2)+
000086C6                          MOVE.L    $40(A6,D2.W),(A2)+
000086CA                          ADD.W     D2,D1
000086CC                          DBRA      D3,$00008678
000086D0                          RTS
000086D2                          AND.W     #$1FFF,$0008(A0)
000086D8                          AND.W     #$0FFF,$000C(A0)
000086DE                          MOVE.B    $0001(A0),D0
000086E2                          BEQ       $0000870C
000086E4                          MOVE.W    $0008(A0),$0010(A0)
000086EA                          MOVE.W    $000C(A0),$0012(A0)
000086F0                          BTST      #$03,$0001(A0)
000086F6                          BNE       $000088B8(pc)
000086FA                          BTST      #$04,$0001(A0)
00008700                          BNE       $00008930(pc)
00008704                          BTST      #$01,$0001(A0)
0000870A                          BNE       $0000870E
0000870C                          RTS
0000870E                          MOVE.W    $0012(A0),-(A7)
00008712                          BSR       $00008732
00008714                          MOVE.W    (A7)+,$0012(A0)
00008718                          MOVE.W    $A00C,D0
0000871C                          SUB.W     $000C(A0),D0
00008720                          CMP.W     #$0070,D0
00008724                          BLT       $00008810(pc)
00008728                          CMP.W     #$0090,D0
0000872C                          BGT       $00008862(pc)
00008730                          RTS
00008732                          MOVE.W    $A008,D0
00008736                          SUB.W     $0008(A0),D0
0000873A                          CMP.W     #$00F0,D0
0000873E                          BGT       $0000874A
00008740                          CMP.W     #$0090,D0
00008744                          BLT       $000087BE(pc)
00008748                          RTS
0000874A                          MOVE.W    #$0020,D5
0000874E                          SUB.W     #$00F0,D0
00008752                          CMP.W     #$0040,D0
00008756                          BCC       $00008760
00008758                          MOVE.W    D0,D5
0000875A                          AND.W     #$0038,D5
0000875E                          LSR.W     #1,D5
00008760                          LEA       $00008D26(pc),A1
00008764                          BSR       $0000879E
00008766                          ADD.L     Invalid
00008768                          OR.B      #$28,A0
0000876C                          OR.B      #$01,D6
00008770                          BNE       $0000879C
00008772                          MOVE.W    $0010(A0),D0
00008776                          MOVE.W    $0008(A0),D1
0000877A                          AND.W     #$000F,D0
0000877E                          AND.W     #$000F,D1
00008782                          CMP.W     D0,D1
00008784                          BHI       $0000879C
00008786                          MOVE.W    $0008(A0),$0010(A0)
0000878C                          MOVE.W    $000C(A0),$0012(A0)
00008792                          ADD.W     #$01F0,$0010(A0)
00008798                          JSR       $00008B8A(pc)
0000879C                          RTS
0000879E                          MOVEQ     #$00,D0
000087A0                          MOVE.B    $FC0A,D0
000087A4                          ADD.W     D0,D0
000087A6                          ADD.W     D0,D0
000087A8                          MOVE.L    $00(A1,D0.W),A1
000087AC                          MOVE.B    $0007(A0),D0
000087B0                          MULU      #$24,D0
000087B4                          LEA       $00(A1,D0.W),A1
000087B8                          MOVE.L    $50(A1,D0.W),D0
000087BC                          RTS
000087BE                          MOVE.W    #$0020,D5
000087C2                          MOVE.W    #$0090,D1
000087C6                          SUB.W     D0,D1
000087C8                          CMP.W     #$0040,D1
000087CC                          BCC       $000087D6
000087CE                          MOVE.W    D1,D5
000087D0                          AND.W     #$0038,D5
000087D4                          LSR.W     #1,D5
000087D6                          LEA       $00008D26(pc),A1
000087DA                          JSR       $0000879E(pc)
000087DE                          (CCC     #$0,$0008(A0)
000087E2                          BTST      #$06,$0001(A0)
000087E8                          BNE       $0000880E
000087EA                          MOVE.W    $0010(A0),D0
000087EE                          MOVE.W    $0008(A0),D1
000087F2                          AND.W     #$000F,D0
000087F6                          AND.W     #$000F,D1
000087FA                          CMP.W     D0,D1
000087FC                          BCS       $0000880E
000087FE                          MOVE.W    $0008(A0),$0010(A0)
00008804                          MOVE.W    $000C(A0),$0012(A0)
0000880A                          JSR       $00008B8A(pc)
0000880E                          RTS
00008810                          MOVE.W    #$0020,D5
00008814                          MOVE.W    #$0070,D1
00008818                          SUB.W     D0,D1
0000881A                          CMP.W     #$0040,D1
0000881E                          BCC       $00008828
00008820                          MOVE.W    D1,D5
00008822                          AND.W     #$0038,D5
00008826                          LSR.W     #1,D5
00008828                          LEA       $00008F96(pc),A1
0000882C                          JSR       $0000879E(pc)
00008830                          (CCC     #$0,$000C(A0)
00008834                          BTST      #$06,$0001(A0)
0000883A                          BNE       $00008860
0000883C                          MOVE.W    $0012(A0),D0
00008840                          MOVE.W    $000C(A0),D1
00008844                          AND.W     #$000F,D0
00008848                          AND.W     #$000F,D1
0000884C                          CMP.W     D0,D1
0000884E                          BCS       $00008860
00008850                          MOVE.W    $0008(A0),$0010(A0)
00008856                          MOVE.W    $000C(A0),$0012(A0)
0000885C                          JSR       $00008A4A(pc)
00008860                          RTS
00008862                          MOVE.W    #$0020,D5
00008866                          SUB.W     #$0090,D0
0000886A                          CMP.W     #$0040,D0
0000886E                          BCC       $00008878
00008870                          MOVE.W    D0,D5
00008872                          AND.W     #$0038,D5
00008876                          LSR.W     #1,D5
00008878                          LEA       $00008E5E(pc),A1
0000887C                          JSR       $0000879E(pc)
00008880                          ADD.L     Invalid
00008882                          OR.B      #$28,A4
00008886                          OR.B      #$01,D6
0000888A                          BNE       $000088B6
0000888C                          MOVE.W    $0012(A0),D0
00008890                          MOVE.W    $000C(A0),D1
00008894                          AND.W     #$000F,D0
00008898                          AND.W     #$000F,D1
0000889C                          CMP.W     D0,D1
0000889E                          BHI       $000088B6
000088A0                          MOVE.W    $0008(A0),$0010(A0)
000088A6                          MOVE.W    $000C(A0),$0012(A0)
000088AC                          ADD.W     #$00F0,$0012(A0)
000088B2                          JSR       $00008A4A(pc)
000088B6                          RTS
000088B8                          MOVEQ     #$00,D0
000088BA                          MOVE.B    $0006(A0),D0
000088BE                          ADD.W     D0,D0
000088C0                          LEA       $000090CE(pc),A6
000088C4                          MOVE.W    $00(A6,D0.W),$0018(A0)
000088CA                          JSR       $00028870
000088D0                          MOVE.B    $0016(A0),D0
000088D4                          EXT.W     D0
000088D6                          ADD.W     D0,D0
000088D8                          MOVE.W    +$00(D3.W),D0
000088DC                          JMP       +$00(D1.W)
000088E0                          Invalid
000088E2                          OR.B      #$8A,(A0)
000088E6                          OR.B      #$A4,-(A0)
000088EA                          OR.B      #$02,$00(A0,D0.W)
000088F0                          MOVE.W    $0010(A0),-(A7)
000088F4                          JSR       $00008834(pc)
000088F8                          MOVE.W    (A7)+,$0010(A0)
000088FC                          BRA       $0000876A(pc)
00008900                          MOVE.W    $0010(A0),-(A7)
00008904                          JSR       $00008884(pc)
00008908                          MOVE.W    (A7)+,$0010(A0)
0000890C                          BRA       $0000876A(pc)
00008910                          MOVE.W    $0010(A0),-(A7)
00008914                          JSR       $00008884(pc)
00008918                          MOVE.W    (A7)+,$0010(A0)
0000891C                          BRA       $000087E2(pc)
00008920                          MOVE.W    $0010(A0),-(A7)
00008924                          JSR       $00008834(pc)
00008928                          MOVE.W    (A7)+,$0010(A0)
0000892C                          BRA       $000087E2(pc)
00008930                          MOVE.W    $0012(A0),-(A7)
00008934                          BSR       $0000896C
00008936                          MOVE.W    (A7)+,$0012(A0)
0000893A                          MOVEQ     #$00,D0
0000893C                          MOVE.W    $003A(A0),D0
00008940                          BEQ       $00008946
00008942                          BPL       $00008948
00008944                          BRA       $0000895A
00008946                          RTS
00008948                          MOVE.W    #$0000,$003A(A0)
0000894E                          SWAP      D0
00008950                          ASR.L     #8,D0
00008952                          ADD.L     Invalid
00008954                          OR.B      #$00,A4
00008958                          Invalid
0000895A                          MOVE.W    #$0000,$003A(A0)
00008960                          SWAP      D0
00008962                          ASR.L     #8,D0
00008964                          ADD.L     Invalid
00008966                          OR.B      #$00,A4
0000896A                          Invalid
0000896C                          MOVEQ     #$00,D0
0000896E                          MOVE.W    $003C(A0),D0
00008972                          BEQ       $00008978
00008974                          BPL       $0000898C
00008976                          BRA       $0000897A
00008978                          RTS
0000897A                          MOVE.W    #$0000,$003C(A0)
00008980                          SWAP      D0
00008982                          ASR.L     #8,D0
00008984                          ADD.L     Invalid
00008986                          OR.B      #$00,A0
0000898A                          Invalid
0000898C                          MOVE.W    #$0000,$003C(A0)
00008992                          SWAP      D0
00008994                          ASR.L     #8,D0
00008996                          ADD.L     Invalid
00008998                          OR.B      #$00,A0
0000899C                          Invalid
0000899E                          LEA       $EA80,A0
000089A2                          MOVE.W    $0008(A0),$0010(A0)
000089A8                          MOVE.W    $000C(A0),$0012(A0)
000089AE                          MOVE.W    #$000F,D6
000089B2                          MOVE.W    $0010(A0),-(A7)
000089B6                          MOVE.W    $0012(A0),-(A7)
000089BA                          MOVEM.L   D6/A0,-(A7)
000089BE                          JSR       $00008A4A(pc)
000089C2                          JSR       $00008C3E(pc)
000089C6                          MOVEM.L   (A7)+,D6/A0
000089CA                          MOVE.W    (A7)+,$0012(A0)
000089CE                          MOVE.W    (A7)+,$0010(A0)
000089D2                          ADD.W     #$0010,$0012(A0)
000089D8                          DBRA      D6,$000089B2
000089DC                          RTS
000089DE                          LEA       $EA80,A0
000089E2                          MOVE.W    $0008(A0),$0010(A0)
000089E8                          MOVE.W    $000C(A0),$0012(A0)
000089EE                          JSR       $00008A4A(pc)
000089F2                          JSR       $00008C3E(pc)
000089F6                          LEA       $EA80,A0
000089FA                          MOVE.W    $0008(A0),$0010(A0)
00008A00                          MOVE.W    $000C(A0),$0012(A0)
00008A06                          ADD.W     #$00F0,$0012(A0)
00008A0C                          JSR       $00008A4A(pc)
00008A10                          JSR       $00008C3E(pc)
00008A14                          LEA       $EA80,A0
00008A18                          MOVE.W    $0008(A0),$0010(A0)
00008A1E                          MOVE.W    $000C(A0),$0012(A0)
00008A24                          JSR       $00008B8A(pc)
00008A28                          JSR       $00008C3E(pc)
00008A2C                          LEA       $EA80,A0
00008A30                          MOVE.W    $0008(A0),$0010(A0)
00008A36                          MOVE.W    $000C(A0),$0012(A0)
00008A3C                          ADD.W     #$01C0,$0010(A0)
00008A42                          JSR       $00008B8A(pc)
00008A46                          JMP       $00008C3E(pc)
00008A4A                          BTST      #$05,$0001(A0)
00008A50                          BNE       $00008AD8(pc)
00008A54                          LEA       $9984,A2
00008A58                          MOVE.W    $0010(A0),D0
00008A5C                          MOVE.W    $0012(A0),D1
00008A60                          MOVE.W    D0,D2
00008A62                          MOVE.W    D1,D3
00008A64                          LSL.W     #4,D3
00008A66                          AND.W     #$0F00,D3
00008A6A                          LSR.W     #2,D2
00008A6C                          AND.W     #$007C,D2
00008A70                          ADD.W     D3,D2
00008A72                          OR.W      #$6000,D2
00008A76                          MOVE.W    D2,(A2)+
00008A78                          MOVEQ     #$10,D2
00008A7A                          MOVEQ     #$1F,D3
00008A7C                          MOVE.W    D0,D4
00008A7E                          MOVE.W    D1,D5
00008A80                          LEA       $ED00,A6
00008A84                          LSR.W     #8,D4
00008A86                          LSR.W     #3,D5
00008A88                          AND.W     #$FFE0,D5
00008A8C                          ADD.W     D5,D4
00008A8E                          MOVE.B    $40(A6,D0.W),D4
00008A92                          LEA       $00FF6000,A6
00008A98                          BCLR      #$07,D4
00008A9C                          BEQ       $00008AA4
00008A9E                          LEA       $00FF7C00,A6
00008AA4                          LSL.W     #8,D4
00008AA6                          ADD.W     D4,D4
00008AA8                          MOVE.W    D0,D5
00008AAA                          LSR.W     #3,D5
00008AAC                          AND.W     #$001E,D5
00008AB0                          ADD.W     D5,D4
00008AB2                          MOVE.W    D1,D5
00008AB4                          ADD.W     D5,D5
00008AB6                          AND.W     #$01E0,D5
00008ABA                          ADD.W     D5,D4
00008ABC                          MOVE.W    $40(A6,D0.W),D4
00008AC0                          MOVE.L    $E018,A6
00008AC4                          LSL.W     #3,D4
00008AC6                          MOVE.L    $40(A6,D0.W),(A2)+
00008ACA                          MOVE.L    $40(A6,D2.W),$007C(A2)
00008AD0                          ADD.W     D2,D0
00008AD2                          DBRA      D3,$00008A7C
00008AD6                          RTS
00008AD8                          LEA       $9984,A2
00008ADC                          MOVE.W    $0010(A0),D0
00008AE0                          MOVE.W    $0012(A0),D1
00008AE4                          MOVE.W    D0,D2
00008AE6                          MOVE.W    D1,D3
00008AE8                          LSL.W     #4,D3
00008AEA                          AND.W     #$0F00,D3
00008AEE                          LSR.W     #2,D2
00008AF0                          AND.W     #$007C,D2
00008AF4                          ADD.W     D3,D2
00008AF6                          OR.W      #$6000,D2
00008AFA                          MOVE.W    D2,(A2)+
00008AFC                          MOVEQ     #$10,D2
00008AFE                          MOVEQ     #$1F,D3
00008B00                          CMP.B     #$02,$FC0A
00008B06                          BEQ       $00008B1A
00008B08                          MOVE.W    D0,D4
00008B0A                          MOVE.W    D1,D5
00008B0C                          SUB.W     #$0700,D4
00008B10                          AND.W     #$03FF,D4
00008B14                          ADD.W     #$0700,D4
00008B18                          BRA       $00008B32
00008B1A                          AND.W     #$01FF,D0
00008B1E                          AND.W     #$01FF,$0008(A0)
00008B24                          AND.W     #$00FF,D1
00008B28                          AND.W     #$00FF,$000C(A0)
00008B2E                          MOVE.W    D0,D4
00008B30                          MOVE.W    D1,D5
00008B32                          LEA       $ED00,A6
00008B36                          LSR.W     #8,D4
00008B38                          LSR.W     #3,D5
00008B3A                          AND.W     #$FFE0,D5
00008B3E                          ADD.W     D5,D4
00008B40                          MOVE.B    $40(A6,D0.W),D4
00008B44                          LEA       $00FF6000,A6
00008B4A                          BCLR      #$07,D4
00008B4E                          BEQ       $00008B56
00008B50                          LEA       $00FF7C00,A6
00008B56                          LSL.W     #8,D4
00008B58                          ADD.W     D4,D4
00008B5A                          MOVE.W    D0,D5
00008B5C                          LSR.W     #3,D5
00008B5E                          AND.W     #$001E,D5
00008B62                          ADD.W     D5,D4
00008B64                          MOVE.W    D1,D5
00008B66                          ADD.W     D5,D5
00008B68                          AND.W     #$01E0,D5
00008B6C                          ADD.W     D5,D4
00008B6E                          MOVE.W    $40(A6,D0.W),D4
00008B72                          MOVE.L    $E018,A6
00008B76                          LSL.W     #3,D4
00008B78                          MOVE.L    $40(A6,D0.W),(A2)+
00008B7C                          MOVE.L    $40(A6,D2.W),$007C(A2)
00008B82                          ADD.W     D2,D0
00008B84                          DBRA      D3,$00008B00
00008B88                          RTS
00008B8A                          BTST      #$05,$0001(A0)
00008B90                          BEQ       $00008BBC
00008B92                          CMP.B     #$02,$FC0A
00008B98                          BEQ       $00008BB0
00008B9A                          MOVE.W    $0010(A0),D0
00008B9E                          SUB.W     #$0700,D0
00008BA2                          AND.W     #$03FF,D0
00008BA6                          ADD.W     #$0700,D0
00008BAA                          MOVE.W    D0,$0010(A0)
00008BAE                          BRA       $00008BBC
00008BB0                          MOVE.W    $0010(A0),D0
00008BB4                          AND.W     #$01FF,D0
00008BB8                          MOVE.W    D0,$0010(A0)
00008BBC                          LEA       $9A86,A2
00008BC0                          MOVE.W    $0010(A0),D0
00008BC4                          MOVE.W    $0012(A0),D1
00008BC8                          MOVE.W    D0,D2
00008BCA                          MOVE.W    D1,D3
00008BCC                          LSL.W     #4,D3
00008BCE                          AND.W     #$0F00,D3
00008BD2                          LSR.W     #2,D2
00008BD4                          AND.W     #$007C,D2
00008BD8                          ADD.W     D3,D2
00008BDA                          OR.W      #$6000,D2
00008BDE                          MOVE.W    D2,(A2)+
00008BE0                          MOVEQ     #$10,D2
00008BE2                          MOVEQ     #$0F,D3
00008BE4                          MOVE.W    D0,D4
00008BE6                          MOVE.W    D1,D5
00008BE8                          LEA       $ED00,A6
00008BEC                          LSR.W     #8,D4
00008BEE                          LSR.W     #3,D5
00008BF0                          AND.W     #$FFE0,D5
00008BF4                          ADD.W     D5,D4
00008BF6                          MOVE.B    $40(A6,D0.W),D4
00008BFA                          LEA       $00FF6000,A6
00008C00                          BCLR      #$07,D4
00008C04                          BEQ       $00008C0C
00008C06                          LEA       $00FF7C00,A6
00008C0C                          LSL.W     #8,D4
00008C0E                          ADD.W     D4,D4
00008C10                          MOVE.W    D0,D5
00008C12                          LSR.W     #3,D5
00008C14                          AND.W     #$001E,D5
00008C18                          ADD.W     D5,D4
00008C1A                          MOVE.W    D1,D5
00008C1C                          ADD.W     D5,D5
00008C1E                          AND.W     #$01E0,D5
00008C22                          ADD.W     D5,D4
00008C24                          MOVE.W    $40(A6,D0.W),D4
00008C28                          MOVE.L    $E018,A6
00008C2C                          LSL.W     #3,D4
00008C2E                          MOVE.L    $40(A6,D0.W),(A2)+
00008C32                          MOVE.L    $40(A6,D2.W),(A2)+
00008C36                          ADD.W     D2,D1
00008C38                          DBRA      D3,$00008BE4
00008C3C                          RTS
00008C3E                          LEA       $00C00004,A0
00008C44                          LEA       $00C00000,A1
00008C4A                          MOVE.L    #$40000010,(A0)
00008C50                          MOVE.W    $EA0C,(A1)
00008C54                          MOVE.W    $EA8C,(A1)
00008C58                          MOVE.L    #$7C000003,$00C00004
00008C62                          MOVE.W    $EA08,D0
00008C66                          NEG.W     D0
00008C68                          MOVE.W    D0,(A1)
00008C6A                          MOVE.W    $EA88,D0
00008C6E                          NEG.W     D0
00008C70                          MOVE.W    D0,(A1)
00008C72                          MOVE.W    #$0080,D2
00008C76                          MOVE.W    #$EFFF,D3
00008C7A                          MOVEQ     #$03,D4
00008C7C                          MOVE.W    #$FF7F,D5
00008C80                          LEA       $EA00,A2
00008C84                          LEA       $9800,A3
00008C88                          LEA       $9902,A4
00008C8C                          BSR       $00008C9A
00008C8E                          LEA       $EA80,A2
00008C92                          LEA       $9984,A3
00008C96                          LEA       $9A86,A4
00008C9A                          MOVE.W    (A4),D0
00008C9C                          BEQ       $00008CB2
00008C9E                          MOVE.W    #$0000,(A4)+
00008CA2                          MOVEQ     #$1F,D7
00008CA4                          MOVE.W    D0,(A0)
00008CA6                          MOVE.W    D4,(A0)
00008CA8                          MOVE.L    (A4)+,(A1)
00008CAA                          ADD.W     D2,D0
00008CAC                          AND.W     D3,D0
00008CAE                          DBRA      D7,$00008CA4
00008CB2                          MOVE.W    (A3),D0
00008CB4                          BEQ       $00008CD8
00008CB6                          MOVE.W    #$0000,(A3)+
00008CBA                          LEA       $0080(A3),A4
00008CBE                          MOVEQ     #$1F,D7
00008CC0                          MOVE.W    D0,(A0)
00008CC2                          MOVE.W    D4,(A0)
00008CC4                          MOVE.L    (A3)+,(A1)
00008CC6                          MOVE.W    D0,D6
00008CC8                          ADD.W     D2,D6
00008CCA                          MOVE.W    D6,(A0)
00008CCC                          MOVE.W    D4,(A0)
00008CCE                          MOVE.L    (A4)+,(A1)
00008CD0                          ADDQ.W    #$4,D0
00008CD2                          AND.W     D5,D0
00008CD4                          DBRA      D7,$00008CC0
00008CD8                          RTS
00008CDA                          OR.B      #$00,D2
00008CDE                          OR.B      #$00,D3
00008CE2                          OR.B      #$00,D4
00008CE6                          OR.B      #$00,D5
00008CEA                          OR.B      #$00,D6
00008CEE                          OR.B      #$00,D7
00008CF2                          OR.B      #$00,A0
00008CF6                          OR.B      #$00,A1
00008CFA                          OR.B      #$00,A2
00008CFE                          OR.B      #$00,A3
00008D02                          Invalid
00008D04                          OR.B      #$FD,D0
00008D08                          OR.B      #$FC,D0
00008D0C                          OR.B      #$FB,D0
00008D10                          OR.B      #$FA,D0
00008D14                          OR.B      #$F9,D0
00008D18                          OR.B      #$F8,D0
00008D1C                          OR.B      #$F7,D0
00008D20                          OR.B      #$F6,D0
00008D24                          OR.B      #$00,D0
00008D28                          SBCD      ,D6
00008D2A                          OR.B      #$62,D0
00008D2E                          OR.B      #$62,D0
00008D32                          OR.B      #$62,D0
00008D36                          OR.B      #$62,D0
00008D3A                          OR.B      #$62,D0
00008D3E                          OR.B      #$99,D1
00008D42                          OR.B      #$66,D1
00008D46                          OR.B      #$33,D2
00008D4A                          OR.B      #$00,D2
00008D4E                          OR.B      #$CC,D3
00008D52                          OR.B      #$99,D3
00008D56                          OR.B      #$66,D4
00008D5A                          OR.B      #$33,D4
00008D5E                          OR.B      #$00,D5
00008D62                          OR.B      #$66,D1
00008D66                          OR.B      #$99,D2
00008D6A                          OR.B      #$CC,D2
00008D6E                          OR.B      #$00,D3
00008D72                          OR.B      #$33,D4
00008D76                          OR.B      #$66,D4
00008D7A                          OR.B      #$99,D5
00008D7E                          OR.B      #$CC,D6
00008D82                          OR.B      #$00,D7
00008D86                          OR.B      #$00,D1
00008D8A                          OR.B      #$00,D1
00008D8E                          OR.B      #$00,D2
00008D92                          OR.B      #$00,D2
00008D96                          OR.B      #$00,D3
00008D9A                          OR.B      #$00,D3
00008D9E                          OR.B      #$00,D4
00008DA2                          OR.B      #$00,D4
00008DA6                          OR.B      #$00,D5
00008DAA                          OR.B      #$00,D2
00008DAE                          OR.B      #$00,D3
00008DB2                          OR.B      #$00,D4
00008DB6                          OR.B      #$00,D5
00008DBA                          OR.B      #$00,D6
00008DBE                          OR.B      #$00,D7
00008DC2                          OR.B      #$00,A0
00008DC6                          OR.B      #$00,A1
00008DCA                          OR.B      #$00,A2
00008DCE                          OR.B      #$00,D1
00008DD2                          OR.B      #$00,D1
00008DD6                          OR.B      #$00,D2
00008DDA                          OR.B      #$00,D2
00008DDE                          OR.B      #$00,D3
00008DE2                          OR.B      #$00,D3
00008DE6                          OR.B      #$00,D4
00008DEA                          OR.B      #$00,D4
00008DEE                          OR.B      #$00,D5
00008DF2                          OR.B      #$00,D0
00008DF6                          OR.B      #$00,D0
00008DFA                          OR.B      #$00,D1
00008DFE                          OR.B      #$00,D1
00008E02                          OR.B      #$00,D1
00008E06                          OR.B      #$00,D1
00008E0A                          OR.B      #$00,D2
00008E0E                          OR.B      #$00,D2
00008E12                          OR.B      #$00,D2
00008E16                          OR.B      #$99,D1
00008E1A                          OR.B      #$66,D1
00008E1E                          OR.B      #$33,D2
00008E22                          OR.B      #$00,D2
00008E26                          OR.B      #$CC,D3
00008E2A                          OR.B      #$99,D3
00008E2E                          OR.B      #$66,D4
00008E32                          OR.B      #$33,D4
00008E36                          OR.B      #$00,D5
00008E3A                          OR.B      #$CC,D5
00008E3E                          OR.B      #$33,D7
00008E42                          OR.B      #$99,A2
00008E46                          OR.B      #$00,A5
00008E4A                          OR.B      #$66,A7
00008E4E                          OR.B      #$CC,(A2)
00008E52                          OR.B      #$33,(A5)
00008E56                          OR.B      #$99,(A7)
00008E5A                          OR.B      #$00,(A2)+
00008E5E                          OR.B      #$76,D0
00008E62                          OR.B      #$9A,D0
00008E66                          OR.B      #$9A,D0
00008E6A                          OR.B      #$9A,D0
00008E6E                          OR.B      #$9A,D0
00008E72                          OR.B      #$9A,D0
00008E76                          OR.B      #$00,D2
00008E7A                          OR.B      #$00,D3
00008E7E                          OR.B      #$00,D3
00008E82                          OR.B      #$00,D4
00008E86                          OR.B      #$00,D5
00008E8A                          OR.B      #$00,D6
00008E8E                          OR.B      #$00,D6
00008E92                          OR.B      #$00,D7
00008E96                          OR.B      #$00,A0
00008E9A                          OR.B      #$66,D2
00008E9E                          OR.B      #$33,D3
00008EA2                          OR.B      #$00,D4
00008EA6                          OR.B      #$CC,D4
00008EAA                          OR.B      #$99,D5
00008EAE                          OR.B      #$66,D6
00008EB2                          OR.B      #$33,D7
00008EB6                          OR.B      #$00,A0
00008EBA                          OR.B      #$AA,A6
00008EBE                          OR.B      #$00,D1
00008EC2                          OR.B      #$00,D2
00008EC6                          OR.B      #$00,D2
00008ECA                          OR.B      #$00,D3
00008ECE                          OR.B      #$00,D3
00008ED2                          OR.B      #$00,D4
00008ED6                          OR.B      #$00,D4
00008EDA                          OR.B      #$00,D5
00008EDE                          OR.B      #$AA,A1
00008EE2                          OR.B      #$00,D3
00008EE6                          OR.B      #$00,D4
00008EEA                          OR.B      #$00,D5
00008EEE                          OR.B      #$00,D6
00008EF2                          OR.B      #$00,D7
00008EF6                          OR.B      #$00,A0
00008EFA                          OR.B      #$00,A1
00008EFE                          OR.B      #$00,A2
00008F02                          OR.B      #$55,(A2)
00008F06                          OR.B      #$00,D3
00008F0A                          OR.B      #$00,D4
00008F0E                          OR.B      #$00,D5
00008F12                          OR.B      #$00,D6
00008F16                          OR.B      #$00,D7
00008F1A                          OR.B      #$00,A0
00008F1E                          OR.B      #$00,A1
00008F22                          OR.B      #$00,A2
00008F26                          OR.B      #$55,(A2)
00008F2A                          OR.B      #$66,D0
00008F2E                          OR.B      #$33,D1
00008F32                          OR.B      #$00,D1
00008F36                          OR.B      #$CC,D1
00008F3A                          OR.B      #$99,D2
00008F3E                          OR.B      #$66,D2
00008F42                          OR.B      #$33,D2
00008F46                          OR.B      #$00,D3
00008F4A                          OR.B      #$00,D5
00008F4E                          OR.B      #$00,D2
00008F52                          OR.B      #$00,D3
00008F56                          OR.B      #$00,D3
00008F5A                          OR.B      #$00,D4
00008F5E                          OR.B      #$00,D5
00008F62                          OR.B      #$00,D6
00008F66                          OR.B      #$00,D6
00008F6A                          OR.B      #$00,D7
00008F6E                          OR.B      #$00,A0
00008F72                          OR.B      #$66,D2
00008F76                          OR.B      #$33,D3
00008F7A                          OR.B      #$00,D4
00008F7E                          OR.B      #$CC,D4
00008F82                          OR.B      #$99,D5
00008F86                          OR.B      #$66,D6
00008F8A                          OR.B      #$33,D7
00008F8E                          OR.B      #$00,A0
00008F92                          OR.B      #$AA,A6
00008F96                          OR.B      #$AE,D0
00008F9A                          OR.B      #$D2,D0
00008F9E                          OR.B      #$D2,D0
00008FA2                          OR.B      #$D2,D0
00008FA6                          OR.B      #$D2,D0
00008FAA                          OR.B      #$D2,D0
00008FAE                          OR.B      #$00,D1
00008FB2                          OR.B      #$00,D2
00008FB6                          OR.B      #$00,D3
00008FBA                          OR.B      #$00,D3
00008FBE                          OR.B      #$00,D4
00008FC2                          OR.B      #$00,D5
00008FC6                          OR.B      #$AA,A4
00008FCA                          OR.B      #$00,A6
00008FCE                          OR.B      #$55,A7
00008FD2                          OR.B      #$99,D1
00008FD6                          OR.B      #$66,D2
00008FDA                          OR.B      #$33,D3
00008FDE                          OR.B      #$00,D4
00008FE2                          OR.B      #$CC,D4
00008FE6                          OR.B      #$99,D5
00008FEA                          OR.B      #$66,D6
00008FEE                          OR.B      #$33,D7
00008FF2                          OR.B      #$00,A0
00008FF6                          OR.B      #$00,D1
00008FFA                          OR.B      #$00,D1
00008FFE                          OR.B      #$00,D2
00009002                          OR.B      #$00,D2
00009006                          OR.B      #$00,D3
0000900A                          OR.B      #$00,D3
0000900E                          OR.B      #$00,D4
00009012                          OR.B      #$00,D4
00009016                          OR.B      #$00,D5
0000901A                          OR.B      #$00,D2
0000901E                          OR.B      #$00,D3
00009022                          OR.B      #$00,D4
00009026                          OR.B      #$00,D5
0000902A                          OR.B      #$00,D6
0000902E                          OR.B      #$00,D7
00009032                          OR.B      #$00,A0
00009036                          OR.B      #$00,A1
0000903A                          OR.B      #$00,A2
0000903E                          OR.B      #$00,D2
00009042                          OR.B      #$00,D3
00009046                          OR.B      #$00,D4
0000904A                          OR.B      #$00,D5
0000904E                          OR.B      #$00,D6
00009052                          OR.B      #$00,D7
00009056                          OR.B      #$00,A0
0000905A                          OR.B      #$00,A1
0000905E                          OR.B      #$00,A2
00009062                          OR.B      #$99,D0
00009066                          OR.B      #$66,D0
0000906A                          OR.B      #$33,D1
0000906E                          OR.B      #$00,D1
00009072                          OR.B      #$CC,D1
00009076                          OR.B      #$99,D2
0000907A                          OR.B      #$66,D2
0000907E                          OR.B      #$33,D2
00009082                          OR.B      #$00,D3
00009086                          OR.B      #$00,D1
0000908A                          OR.B      #$00,D2
0000908E                          OR.B      #$00,D3
00009092                          OR.B      #$00,D3
00009096                          OR.B      #$00,D4
0000909A                          OR.B      #$00,D5
0000909E                          OR.B      #$00,D6
000090A2                          OR.B      #$00,D6
000090A6                          OR.B      #$00,D7
000090AA                          OR.B      #$99,D1
000090AE                          OR.B      #$66,D2
000090B2                          OR.B      #$33,D3
000090B6                          OR.B      #$00,D4
000090BA                          OR.B      #$CC,D4
000090BE                          OR.B      #$99,D5
000090C2                          OR.B      #$66,D6
000090C6                          OR.B      #$33,D7
000090CA                          OR.B      #$00,A0
000090CE                          OR.B      #$36,(A6)
000090D2                          OR.W      #$0076,(A6)
000090D6                          OR.L      #$00B600D6,(A6)
000090DC                          Invalid
000090DE                          BTST      D0,(A6)
000090E0                          BTST      D0,$01(A6,D3.L)
000090E4                          OR.B      #$00,D0
000090E8                          OR.L      #$00B40100,$00(A4,D0.W)
000090F0                          OR.L      #$FF4C0000,$FF(A4,D0.W)
000090F8                          Invalid
000090FA                          Invalid
000090FC                          Invalid
000090FE                          OR.B      #$4C,D0
00009102                          OR.L      #$00000200,$01(A4,A4.W)
0000910A                          BCHG      D0,$0200(A1)
0000910E                          OR.B      #$69,D0
00009112                          Invalid
00009114                          OR.B      #$00,D0
00009118                          Invalid
0000911A                          Invalid
0000911C                          Invalid
0000911E                          OR.B      #$97,D0
00009122                          BCHG      D0,$0000(A1)
00009126                          SUB.B     #$D3,D0
0000912A                          Invalid
0000912C                          SUB.B     #$00,D0
00009130                          Invalid
00009132                          Invalid
00009134                          OR.B      #$00,D0
00009138                          Invalid
0000913A                          Invalid
0000913C                          Invalid
0000913E                          OR.B      #$2D,D0
00009142                          Invalid
00009144                          OR.B      #$00,D0
00009148                          BCLR      D2,-(A7)
0000914A                          BCLR      D2,-(A7)
0000914C                          BTST      #$00,D0
00009150                          BCLR      D2,-(A7)
00009152                          Invalid
00009154                          OR.B      #$00,D0
00009158                          Invalid
0000915A                          Invalid
0000915C                          Invalid
0000915E                          OR.B      #$59,D0
00009162                          BCLR      D2,-(A7)
00009164                          OR.B      #$00,D0
00009168                          BCHG      #$7B,+$0C(D0.W)
0000916E                          OR.B      #$7B,D0
00009172                          Invalid
00009174                          OR.B      #$00,D0
00009178                          Invalid
0000917A                          Invalid
0000917C                          Invalid
0000917E                          OR.B      #$85,D0
00009182                          BCHG      #$00,+$10(D0.W)
00009188                          MOVEP.L   $0B4F(A7),D5
0000918C                          MOVE.B    D0,D0
0000918E                          OR.B      #$4F,D0
00009192                          Invalid
00009194                          OR.B      #$00,D0
00009198                          Invalid
0000919A                          Invalid
0000919C                          Invalid
0000919E                          OR.B      #$B1,D0
000091A2                          MOVEP.L   $0000(A7),D5
000091A6                          MOVE.B    D0,D2
000091A8                          .B        #$23,-(A3)
000091AC                          MOVE.B    D0,D2
000091AE                          OR.B      #$23,D0
000091B2                          Invalid
000091B4                          OR.B      #$00,D0
000091B8                          Invalid
000091BA                          Invalid
000091BC                          ASR.B     #6,D0
000091BE                          OR.B      #$DD,D0
000091C2                          .B        #$00,-(A3)
000091C6                          MOVE.B    D0,-(A7)
000091C8                          MOVE.B    $15EA(A2),$0000B0CA(pc)
000091CE                          OR.B      #$EA,D0
000091D2                          ROXR.B    #5,D6
000091D4                          OR.B      #$00,D0
000091D8                          ROXR.B    #5,D6
000091DA                          ROXR.B    #5,D6
000091DC                          ASL.B     #8,D0
000091DE                          OR.B      #$16,D0
000091E2                          MOVE.B    $0000(A2),$000094E4(pc)
000091E8                          AND.B     #$1E,(A6)+
000091EC                          BTST      D1,D0
000091EE                          OR.B      #$1E,D0
000091F2                          Invalid
000091F4                          OR.B      #$00,D0
000091F8                          Invalid
000091FA                          Invalid
000091FC                          Invalid
000091FE                          OR.B      #$E2,D0
00009202                          AND.B     #$00,(A6)+
00009206                          BTST      D2,D0
00009208                          MOVEP.W   D1,$0388(A0)
0000920C                          BTST      D2,D0
0000920E                          OR.B      #$88,D0
00009212                          Invalid
00009214                          OR.B      #$00,D0
00009218                          Invalid
0000921A                          Invalid
0000921C                          Invalid
0000921E                          OR.B      #$78,D0
00009222                          MOVEP.W   D1,$0000(A0)
00009226                          OR.L      #$005A005A,D0
0000922C                          OR.L      #$0000005A,D0
00009232                          Invalid
00009234                          OR.B      #$80,D0
00009238                          Invalid
0000923A                          Invalid
0000923C                          Invalid
0000923E                          OR.B      #$A6,D0
00009242                          OR.W      #$41F8,(A2)+
00009246                          Invalid
00009248                          MOVE.W    #$0005,D7
0000924C                          SWAP      D7
0000924E                          TST.B     $0000(A0)
00009252                          BEQ       $00009256
00009254                          BSR       $00009262
00009256                          LEA       $0050(A0),A0
0000925A                          SWAP      D7
0000925C                          DBRA      D7,$0000924C
00009260                          RTS
00009262                          MOVE.L    $001A(A0),A6
00009266                          TST.B     $0006(A0)
0000926A                          BNE       $00009504(pc)
0000926E                          MOVEQ     #$00,D0
00009270                          MOVE.B    $0002(A0),D0
00009274                          MOVE.W    +$00(D3.W),D0
00009278                          JMP       +$00(D1.W)
0000927C                          OR.B      #$E6,A0
00009280                          AND.W     #$0266,(A6)+
00009284                          MOVE.B    #$02,$0002(A0)
0000928A                          LEA       $000095CA(pc),A6
0000928E                          MOVE.L    A6,$001A(A0)
00009292                          LEA       $000095DE(pc),A3
00009296                          MOVE.W    $0000(A3),$0018(A0)
0000929C                          MOVE.W    #$0096,$0028(A0)
000092A2                          MOVE.B    #$10,$0038(A0)
000092A8                          MOVE.L    $0008(A0),$0030(A0)
000092AE                          MOVE.L    $000C(A0),$003C(A0)
000092B4                          LEA       $A000,A1
000092B8                          MOVE.B    #$08,$0045(A0)
000092BE                          TST.B     $0030(A1)
000092C2                          BEQ       $000092CA
000092C4                          MOVE.B    #$18,$0045(A0)
000092CA                          MOVE.W    $000C(A1),D0
000092CE                          ADD.W     #$0020,D0
000092D2                          CMP.W     $000C(A0),D0
000092D6                          BLS       $00009356
000092D8                          SUB.W     #$0040,D0
000092DC                          CMP.W     $000C(A0),D0
000092E0                          BCC       $00009356
000092E2                          MOVE.W    $0008(A1),D0
000092E6                          TST.B     $0030(A1)
000092EA                          BNE       $0000930A
000092EC                          SUB.W     #$0010,D0
000092F0                          CMP.W     $0008(A0),D0
000092F4                          BLS       $00009356
000092F6                          MOVE.W    $0008(A0),$004A(A0)
000092FC                          ADD.W     #$0100,$004A(A0)
00009302                          MOVE.W    $000C(A0),$004C(A0)
00009308                          BRA       $00009326
0000930A                          ADD.W     #$0010,D0
0000930E                          CMP.W     $0008(A0),D0
00009312                          BCC       $00009356
00009314                          MOVE.W    $0008(A0),$004A(A0)
0000931A                          SUB.W     #$0100,$004A(A0)
00009320                          MOVE.W    $000C(A0),$004C(A0)
00009326                          MOVE.W    $000C(A1),D0
0000932A                          CMP.W     $000C(A0),D0
0000932E                          BCC       $0000933E
00009330                          MOVE.W    $000C(A1),$0048(A0)
00009336                          ADD.W     #$0020,$0048(A0)
0000933C                          BRA       $0000934A
0000933E                          MOVE.W    $000C(A1),$0048(A0)
00009344                          SUB.W     #$0020,$0048(A0)
0000934A                          MOVE.W    $0008(A1),$0046(A0)
00009350                          MOVE.B    #$04,$0003(A0)
00009356                          MOVEQ     #$00,D0
00009358                          LEA       $00009664(pc),A4
0000935C                          JMP       $000287D6
00009362                          MOVE.L    $0008(A0),$0030(A0)
00009368                          MOVE.L    $000C(A0),$003C(A0)
0000936E                          MOVEQ     #$00,D0
00009370                          MOVE.B    $0003(A0),D0
00009374                          MOVE.W    +$00(D6.W),D0
00009378                          JSR       +$00(D4.W)
0000937C                          JMP       $00028C4A
00009382                          OR.B      #$32,D6
00009386                          OR.L      #$53280038,D2
0000938C                          BNE       $000093AE
0000938E                          MOVE.B    #$02,$0003(A0)
00009394                          MOVE.L    $002C(A0),A1
00009398                          MOVE.W    $0008(A1),D0
0000939C                          MOVE.W    $000C(A1),D1
000093A0                          JSR       $000288F2
000093A6                          ADDQ.B    #$4,D6
000093A8                          LSR.B     #3,D6
000093AA                          MOVE.B    D6,$0016(A0)
000093AE                          JMP       $00028842
000093B4                          MOVE.L    $002C(A0),A1
000093B8                          MOVE.W    $0008(A1),D0
000093BC                          MOVE.W    $000C(A1),D1
000093C0                          JSR       $000288F2
000093C6                          ADDQ.B    #$4,D6
000093C8                          LSR.B     #3,D6
000093CA                          MOVE.B    D6,$0016(A0)
000093CE                          JSR       $00028842
000093D4                          MOVE.L    $002C(A0),A1
000093D8                          MOVE.W    $0008(A1),D0
000093DC                          SUB.W     $0008(A0),D0
000093E0                          ADD.W     #$0010,D0
000093E4                          CMP.W     #$0018,D0
000093E8                          BCC       $00009402
000093EA                          MOVE.W    $000C(A1),D0
000093EE                          SUB.W     $000C(A0),D0
000093F2                          ADD.W     #$0010,D0
000093F6                          CMP.W     #$0018,D0
000093FA                          BCC       $00009402
000093FC                          MOVE.B    #$04,$0002(A0)
00009402                          RTS
00009404                          TST.B     $0004(A0)
00009408                          BNE       $00009498(pc)
0000940C                          MOVE.W    $0046(A0),D0
00009410                          MOVE.W    $0048(A0),D1
00009414                          JSR       $000288F2
0000941A                          ADDQ.B    #$4,D6
0000941C                          LSR.B     #3,D6
0000941E                          MOVE.B    D6,$0016(A0)
00009422                          JSR       $00028842
00009428                          MOVE.W    $0046(A0),D0
0000942C                          SUB.W     $0008(A0),D0
00009430                          ADD.W     #$0010,D0
00009434                          CMP.W     #$0018,D0
00009438                          BCC       $0000944C
0000943A                          MOVE.W    $0048(A0),D0
0000943E                          SUB.W     $000C(A0),D0
00009442                          ADD.W     #$0010,D0
00009446                          CMP.W     #$0018,D0
0000944A                          BCS       $00009468
0000944C                          MOVE.W    $004A(A0),D0
00009450                          CMP.B     #$10,$0045(A0)
00009456                          BHI       $00009460
00009458                          CMP.W     $0008(A0),D0
0000945C                          BCS       $00009470
0000945E                          RTS
00009460                          CMP.W     $0008(A0),D0
00009464                          BHI       $00009470
00009466                          RTS
00009468                          MOVE.B    #$02,$0004(A0)
0000946E                          RTS
00009470                          MOVE.B    #$00,$0004(A0)
00009476                          MOVE.B    #$02,$0003(A0)
0000947C                          MOVE.L    $002C(A0),A1
00009480                          MOVE.W    $0008(A1),D0
00009484                          MOVE.W    $000C(A1),D1
00009488                          JSR       $000288F2
0000948E                          ADDQ.B    #$4,D6
00009490                          LSR.B     #3,D6
00009492                          MOVE.B    D6,$0016(A0)
00009496                          RTS
00009498                          MOVE.W    $004A(A0),D0
0000949C                          MOVE.W    $004C(A0),D1
000094A0                          JSR       $000288F2
000094A6                          ADDQ.B    #$4,D6
000094A8                          LSR.B     #3,D6
000094AA                          MOVE.B    D6,$0016(A0)
000094AE                          JSR       $00028842
000094B4                          MOVE.W    $004A(A0),D0
000094B8                          SUB.W     $0008(A0),D0
000094BC                          ADD.W     #$0010,D0
000094C0                          CMP.W     #$0018,D0
000094C4                          BCC       $000094D8
000094C6                          MOVE.W    $004C(A0),D0
000094CA                          SUB.W     $000C(A0),D0
000094CE                          ADD.W     #$0010,D0
000094D2                          CMP.W     #$0018,D0
000094D6                          BCS       $00009470
000094D8                          RTS
000094DA                          MOVE.B    #$06,$0002(A0)
000094E0                          RTS
000094E2                          MOVE.L    $002C(A0),A1
000094E6                          TST.B     $0050(A1)
000094EA                          BNE       $000094F8
000094EC                          MOVE.B    #$00,$F0A1
000094F2                          JMP       $00028B38
000094F8                          MOVE.B    #$00,$F0A2
000094FE                          JMP       $00028B38
00009504                          MOVE.L    $002C(A0),A1
00009508                          CMP.B     #$04,$0002(A1)
0000950E                          BNE       $0000951C
00009510                          MOVE.B    #$04,$0002(A0)
00009516                          MOVE.B    #$02,$0038(A0)
0000951C                          MOVEQ     #$00,D0
0000951E                          MOVE.B    $0002(A0),D0
00009522                          MOVE.W    +$00(D3.W),D0
00009526                          JMP       +$00(D1.W)
0000952A                          OR.B      #$6E,A0
0000952E                          OR.L      #$009A4A28,A4
00009534                          OR.B      #$26,D3
00009538                          MOVE.B    #$02,$0003(A0)
0000953E                          LEA       $000095CA(pc),A6
00009542                          MOVE.L    A6,$001A(A0)
00009546                          LEA       $000095DE(pc),A3
0000954A                          MOVE.W    $0000(A3),$0018(A0)
00009550                          MOVEQ     #$00,D0
00009552                          MOVE.B    $003A(A0),D0
00009556                          MOVE.B    +$00(D6.L),$0038(A0)
0000955C                          RTS
0000955E                          SUBQ.B    #$1,$0038(A0)
00009562                          BNE       $0000955C
00009564                          MOVE.B    #$02,$0002(A0)
0000956A                          MOVE.B    #$00,$0003(A0)
00009570                          MOVE.L    $0030(A1),$0008(A0)
00009576                          MOVE.L    $0008(A0),$0030(A0)
0000957C                          MOVE.L    $003C(A1),$000C(A0)
00009582                          MOVE.L    $000C(A0),$003C(A0)
00009588                          MOVEQ     #$00,D0
0000958A                          LEA       $00009664(pc),A4
0000958E                          JMP       $000287D6
00009594                          AND.B     #$00,D4
00009598                          MOVE.L    $0008(A0),$0030(A0)
0000959E                          MOVE.L    $0030(A1),$0008(A0)
000095A4                          MOVE.L    $000C(A0),$003C(A0)
000095AA                          MOVE.L    $003C(A1),$000C(A0)
000095B0                          JMP       $00028C4A
000095B6                          SUBQ.B    #$1,$0038(A0)
000095BA                          BNE       $000095C2
000095BC                          MOVE.B    #$06,$0002(A0)
000095C2                          RTS
000095C4                          JMP       $00028B38
000095CA                          OR.B      #$01,D0
000095CE                          OR.B      #$01,D0
000095D2                          OR.B      #$01,D0
000095D6                          OR.B      #$01,D0
000095DA                          OR.B      #$01,D0
000095DE                          OR.B      #$00,(A6)
000095E2                          MOVE.B    D0,D0
000095E4                          BTST      D1,(A6)+
000095E6                          BCLR      D7,$061C(A6)
000095EA                          Invalid
000095EC                          BSET      #$4B,-(A1)
000095F0                          MOVEP.L   $0B4F(A7),D5
000095F4                          MOVEP.L   $08E1(A3),D6
000095F8                          Invalid
000095FA                          ADD.B     #$AE,(A4)+
000095FE                          BTST      D1,(A6)+
00009600                          MOVE.B    D0,D0
00009602                          OR.B      #$AE,D0
00009606                          Invalid
00009608                          Invalid
0000960A                          Invalid
0000960C                          MOVEP.L   -$08E1(A3),D6
00009610                          MOVEP.L   -$0B4F(A7),D5
00009614                          BSET      #$B5,-(A1)
00009618                          ADD.B     #$3C,(A4)+
0000961C                          BTST      D1,(A6)+
0000961E                          Invalid
00009620                          OR.B      #$00,D0
00009624                          Invalid
00009626                          Invalid
00009628                          Invalid
0000962A                          Invalid
0000962C                          Invalid
0000962E                          Invalid
00009630                          Invalid
00009632                          Invalid
00009634                          Invalid
00009636                          Invalid
00009638                          Invalid
0000963A                          Invalid
0000963C                          Invalid
0000963E                          Invalid
00009640                          Invalid
00009642                          OR.B      #$52,D0
00009646                          BTST      D1,(A6)+
00009648                          Invalid
0000964A                          ADD.B     #$B5,(A4)+
0000964E                          BSET      #$B1,-(A1)
00009652                          MOVEP.L   -$08E1(A7),D5
00009656                          MOVEP.L   -$061C(A3),D6
0000965A                          Invalid
0000965C                          Invalid
0000965E                          BCLR      D7,$0000(A6)
00009662                          EOR.B     #$9C,A2
00009666                          OR.B      #$01,D0
0000966A                          Invalid
0000966C                          OR.B      #$A8,D0
00009670                          OR.L      #$0000F805,(A4)+
00009676                          OR.B      #$00,$4A(A0,D4.W)
0000967C                          OR.B      #$00,D2
00009680                          MOVEP.L   $4A28(A0),D0
00009684                          OR.B      #$0A,D6
00009688                          TST.B     $F0A1
0000968C                          BNE       $000097C0(pc)
00009690                          BRA       $0000969A
00009692                          TST.B     $F0A2
00009696                          BNE       $000097C0(pc)
0000969A                          TST.B     $0003(A0)
0000969E                          BNE       $00009708(pc)
000096A2                          JSR       $00028A50
000096A8                          BCS       $000096B8
000096AA                          BSR       $000096C0
000096AC                          ADDQ.B    #$1,$0032(A0)
000096B0                          CMP.B     #$03,$0032(A0)
000096B6                          BNE       $000096BE
000096B8                          MOVE.B    #$02,$0003(A0)
000096BE                          RTS
000096C0                          MOVE.L    A1,D1
000096C2                          MOVE.L    $003A(A0),D0
000096C6                          BEQ       $00009702
000096C8                          CMP.L     D1,D0
000096CA                          BCC       $000096E6
000096CC                          MOVE.L    D1,$003A(A0)
000096D0                          MOVE.L    $003E(A0),D1
000096D4                          MOVE.L    D0,$003E(A0)
000096D8                          MOVE.L    $0042(A0),D0
000096DC                          MOVE.L    D1,$0042(A0)
000096E0                          MOVE.L    D0,$0048(A0)
000096E4                          RTS
000096E6                          MOVE.L    $003E(A0),D0
000096EA                          CMP.L     D1,D0
000096EC                          BCC       $000096F2
000096EE                          MOVE.L    D1,D0
000096F0                          BRA       $000096D0
000096F2                          MOVE.L    $0042(A0),D0
000096F6                          CMP.L     D1,D0
000096F8                          BCC       $000096FC
000096FA                          BRA       $000096D8
000096FC                          MOVE.L    D1,$0048(A0)
00009700                          RTS
00009702                          MOVE.L    D1,$003A(A0)
00009706                          RTS
00009708                          MOVE.L    $004C(A0),A2
0000970C                          MOVE.L    $003A(A0),D0
00009710                          BEQ       $000097AC(pc)
00009714                          MOVE.L    $003A(A0),A1
00009718                          MOVE.B    #$01,$0000(A1)
0000971E                          MOVE.B    #$00,$0006(A1)
00009724                          MOVE.B    #$08,$0016(A1)
0000972A                          TST.B     $A030
0000972E                          BEQ       $00009736
00009730                          MOVE.B    #$18,$0016(A1)
00009736                          MOVE.W    $0008(A2),$0008(A1)
0000973C                          MOVE.W    $000C(A2),$000C(A1)
00009742                          MOVE.L    A2,$002C(A1)
00009746                          MOVE.L    $003E(A0),D0
0000974A                          BEQ       $000097AC
0000974C                          MOVE.L    $003E(A0),A1
00009750                          MOVE.B    #$01,$0000(A1)
00009756                          MOVE.B    #$01,$0006(A1)
0000975C                          MOVE.B    #$00,$003A(A1)
00009762                          MOVE.L    $003A(A0),$002C(A1)
00009768                          MOVE.L    $0042(A0),D0
0000976C                          BEQ       $000097AC
0000976E                          MOVE.L    $0042(A0),A1
00009772                          MOVE.B    #$01,$0000(A1)
00009778                          MOVE.B    #$01,$0006(A1)
0000977E                          MOVE.B    #$01,$003A(A1)
00009784                          MOVE.L    $003E(A0),$002C(A1)
0000978A                          MOVE.L    $0048(A0),D0
0000978E                          BEQ       $000097AC
00009790                          MOVE.L    $0048(A0),A1
00009794                          MOVE.B    #$01,$0000(A1)
0000979A                          MOVE.B    #$01,$0006(A1)
000097A0                          MOVE.B    #$02,$003A(A1)
000097A6                          MOVE.L    $0042(A0),$002C(A1)
000097AC                          TST.B     $0006(A0)
000097B0                          BNE       $000097BA
000097B2                          MOVE.B    #$01,$F0A1
000097B8                          BRA       $000097C0
000097BA                          MOVE.B    #$01,$F0A2
000097C0                          MOVE.B    #$02,$0002(A0)
000097C6                          RTS
000097C8                          TST.B     $0006(A0)
000097CC                          BNE       $000097D6
000097CE                          MOVE.B    #$01,$F0A1
000097D4                          BRA       $000097DC
000097D6                          MOVE.B    #$01,$F0A2
000097DC                          JMP       $00028B58
000097E2                          LEA       $A320,A0
000097E6                          BSR       $000097F0
000097E8                          BSR       $000097F0
000097EA                          BSR       $000097F0
000097EC                          BSR       $000097F0
000097EE                          NOP
000097F0                          TST.B     $0000(A0)
000097F4                          BEQ       $000097F8
000097F6                          BSR       $000097FE
000097F8                          LEA       $0060(A0),A0
000097FC                          RTS
000097FE                          MOVE.L    $001A(A0),A6
00009802                          CMP.B     #$01,$0017(A0)
00009808                          BEQ       $0000B664(pc)
0000980C                          CMP.B     #$02,$0017(A0)
00009812                          BEQ       $0000D1B4(pc)
00009816                          LEA       $0000AB16(pc),A6
0000981A                          LEA       $A000,A1
0000981E                          MOVEQ     #$00,D0
00009820                          MOVE.B    $0002(A0),D0
00009824                          MOVE.W    +$00(D3.W),D1
00009828                          JMP       +$10(D1.W)
0000982C                          OR.B      #$8E,A0
00009830                          BSET      D7,$1000
00009834                          MOVE.L    A6,$001A(A0)
00009838                          LEA       $0000B4B0(pc),A2
0000983C                          SUB.L     A6,A2
0000983E                          MOVE.W    A2,$0028(A0)
00009842                          LEA       $0000AB16(pc),A3
00009846                          MOVE.W    $0004(A3),$0018(A0)
0000984C                          MOVE.B    #$02,$0002(A0)
00009852                          MOVE.B    #$0C,$0003(A0)
00009858                          MOVE.W    $0008(A1),$0008(A0)
0000985E                          MOVE.W    $000C(A1),$000C(A0)
00009864                          MOVE.W    $0008(A1),$0030(A0)
0000986A                          MOVE.W    $000C(A1),$0034(A0)
00009870                          SUB.W     #$0010,$0034(A0)
00009876                          MOVE.B    $0030(A1),$003E(A0)
0000987C                          MOVE.B    #$00,$003C(A0)
00009882                          LEA       $0000A914(pc),A2
00009886                          TST.B     $003E(A0)
0000988A                          BEQ       $00009890
0000988C                          LEA       $0000A8C0(pc),A2
00009890                          MOVE.L    A2,$0038(A0)
00009894                          MOVE.B    (A2)+,$003D(A0)
00009898                          MOVE.B    (A2),$0016(A0)
0000989C                          MOVE.B    $0016(A0),D0
000098A0                          LSR.B     #1,D0
000098A2                          ADDQ.B    #$0,D0
000098A4                          JSR       $00009F88(pc)
000098A8                          JSR       $00028870
000098AE                          JSR       $000201C8
000098B4                          JMP       $00028C60
000098BA                          BTST      #$04,$E109
000098C0                          BEQ       $000098C8
000098C2                          BSR       $0000990A
000098C4                          LEA       $A000,A1
000098C8                          CMP.B     #$0C,$0003(A0)
000098CE                          BEQ       $000098DA
000098D0                          CMP.B     #$16,$0003(A0)
000098D6                          BEQ       $000098DA
000098D8                          BSR       $00009932
000098DA                          MOVE.L    $0038(A0),A2
000098DE                          MOVEQ     #$00,D0
000098E0                          MOVE.B    $0003(A0),D0
000098E4                          MOVE.W    +$00(D6.W),D1
000098E8                          JSR       +$10(D4.W)
000098EC                          JMP       $00028C60
000098F2                          OR.L      #$007A008A,A2
000098F8                          OR.L      #$00A600B4,(A0)
000098FE                          BSET      D6,$0178
00009902                          SUB.W     #$0396,$05C6
00009908                          BCLR      D6,(A6)+
0000990A                          NOP
0000990C                          NOP
0000990E                          NOP
00009910                          NOP
00009912                          JSR       $00028A92
00009918                          BCS       $00009930
0000991A                          MOVE.B    #$01,$0000(A1)
00009920                          MOVE.B    #$30,$0017(A1)
00009926                          MOVE.B    $0050(A0),$0006(A1)
0000992C                          MOVE.L    A0,$004C(A1)
00009930                          RTS
00009932                          MOVE.W    $A00C,D0
00009936                          SUB.W     $000C(A0),D0
0000993A                          BPL       $0000993E
0000993C                          NEG.W     D0
0000993E                          CMP.W     #$01C0,D0
00009942                          BHI       $00009956
00009944                          MOVE.W    $A008,D0
00009948                          SUB.W     $0008(A0),D0
0000994C                          BPL       $00009950
0000994E                          NEG.W     D0
00009950                          CMP.W     #$01C0,D0
00009954                          BLS       $0000996A
00009956                          MOVE.B    #$00,$0004(A0)
0000995C                          MOVE.B    #$16,$0003(A0)
00009962                          MOVE.W    $0000AB1A,$0018(A0)
0000996A                          RTS
0000996C                          SUBQ.B    #$1,$0046(A0)
00009970                          BNE       $00009978
00009972                          MOVE.B    #$04,$0003(A0)
00009978                          SUBQ.W    #$1,$000C(A0)
0000997C                          BSR       $000099BE
0000997E                          BRA       $0000A1A2(pc)
00009982                          SUBQ.B    #$1,$0046(A0)
00009986                          BNE       $0000998E
00009988                          MOVE.B    #$00,$0003(A0)
0000998E                          ADDQ.W    #$1,$000C(A0)
00009992                          BSR       $000099BE
00009994                          BRA       $0000A1A2(pc)
00009998                          BSR       $000099BE
0000999A                          TST.B     $002E(A0)
0000999E                          BNE       $0000A25E(pc)
000099A2                          BRA       $0000A232(pc)
000099A6                          BSR       $000099BE
000099A8                          TST.B     $002E(A0)
000099AC                          BEQ       $0000A2B2(pc)
000099B0                          CMP.B     #$02,$002E(A0)
000099B6                          BEQ       $0000A2C8(pc)
000099BA                          BRA       $0000A306(pc)
000099BE                          BSR       $000099D8
000099C0                          JSR       $0000A1EE(pc)
000099C4                          JSR       $00028870
000099CA                          JSR       $00009F5A(pc)
000099CE                          MOVE.B    $0003(A0),$0044(A0)
000099D4                          BRA       $0000A33E(pc)
000099D8                          TST.B     $0004(A0)
000099DC                          BNE       $000099E8
000099DE                          SUBQ.B    #$1,$003D(A0)
000099E2                          BEQ       $00009FC8(pc)
000099E6                          RTS
000099E8                          SUBQ.B    #$1,$003D(A0)
000099EC                          BNE       $00009A48
000099EE                          ADDQ.B    #$1,$0042(A0)
000099F2                          MOVE.B    $0043(A0),D0
000099F6                          CMP.B     $0042(A0),D0
000099FA                          BCS       $00009A50
000099FC                          MOVE.B    $0040(A0),D0
00009A00                          CMP.B     $0042(A0),D0
00009A04                          BNE       $00009A10
00009A06                          LEA       $0000AB16(pc),A3
00009A0A                          MOVE.W    $0002(A3),$0018(A0)
00009A10                          MOVEQ     #$00,D0
00009A12                          MOVE.B    $0042(A0),D0
00009A16                          MOVE.B    $00(A2,D0.W),$003D(A0)
00009A1C                          TST.B     $0005(A0)
00009A20                          BEQ       $00009A36
00009A22                          SUBQ.B    #$1,$0016(A0)
00009A26                          MOVE.B    $0041(A0),D0
00009A2A                          CMP.B     $0042(A0),D0
00009A2E                          BNE       $00009A48
00009A30                          SUBQ.B    #$1,$0016(A0)
00009A34                          BRA       $00009A48
00009A36                          ADDQ.B    #$1,$0016(A0)
00009A3A                          MOVE.B    $0041(A0),D0
00009A3E                          CMP.B     $0042(A0),D0
00009A42                          BNE       $00009A48
00009A44                          ADDQ.B    #$1,$0016(A0)
00009A48                          AND.B     #$1F,$0016(A0)
00009A4E                          RTS
00009A50                          LEA       $0000AB16(pc),A3
00009A54                          MOVE.W    $0000(A3),$0018(A0)
00009A5A                          MOVE.B    $0045(A0),$003C(A0)
00009A60                          MOVE.B    #$00,$0004(A0)
00009A66                          BRA       $00009E90(pc)
00009A6A                          MOVEQ     #$00,D0
00009A6C                          MOVE.B    $0005(A0),D0
00009A70                          CMP.B     #$01,$0022(A0)
00009A76                          BNE       $00009A84
00009A78                          MOVE.W    +$00(D3.L),D0
00009A7C                          JSR       +$00(D1.L)
00009A80                          BRA       $0000A1C8(pc)
00009A84                          MOVE.W    +$00(D7.W),D0
00009A88                          JSR       +$00(D5.W)
00009A8C                          BRA       $0000A1C8(pc)
00009A90                          OR.B      #$4A,A0
00009A94                          Invalid
00009A96                          BCHG      D0,(A6)+
00009A98                          JSR       $0000A612(pc)
00009A9C                          MOVE.W    $000C(A0),D0
00009AA0                          CMP.W     $004E(A0),D0
00009AA4                          BCS       $00009E74(pc)
00009AA8                          CMP.B     #$10,$0016(A0)
00009AAE                          BHI       $00009AC0
00009AB0                          MOVE.W    $0008(A0),D0
00009AB4                          CMP.W     $004C(A0),D0
00009AB8                          BCS       $00009AD0
00009ABA                          JMP       $00028804
00009AC0                          MOVE.W    $004C(A0),D0
00009AC4                          CMP.W     $0008(A0),D0
00009AC8                          BCS       $00009AD0
00009ACA                          JMP       $00028804
00009AD0                          MOVE.B    #$02,$0005(A0)
00009AD6                          JSR       $0000A65E(pc)
00009ADA                          JSR       $00028870
00009AE0                          CMP.B     #$10,$0016(A0)
00009AE6                          BHI       $00009AFC
00009AE8                          MOVE.W    $004C(A0),D0
00009AEC                          CMP.W     $0008(A0),D0
00009AF0                          BCC       $00009B06
00009AF2                          JSR       $0000A612(pc)
00009AF6                          JSR       $0000A5BA(pc)
00009AFA                          BRA       $00009B06
00009AFC                          MOVE.W    $0008(A0),D0
00009B00                          CMP.W     $004C(A0),D0
00009B04                          BCS       $00009AF2
00009B06                          CMP.B     #$10,$0016(A0)
00009B0C                          BHI       $00009B26
00009B0E                          JSR       $00001CE6
00009B14                          BCC       $00009B3E
00009B16                          CMP.B     #$01,$0024(A0)
00009B1C                          BNE       $00009B3E
00009B1E                          ADD.W     Invalid
00009B20                          OR.B      #$00,A0
00009B24                          BCLR      D4,$4EB9(A2)
00009B28                          OR.B      #$2E,D0
00009B2C                          BCC       $00009B3E
00009B2E                          CMP.B     #$02,$0024(A0)
00009B34                          BNE       $00009B3E
00009B36                          ADD.W     Invalid
00009B38                          OR.B      #$00,A0
00009B3C                          MOVEP.W   D4,$4EB9(A6)
00009B40                          OR.B      #$AE,D0
00009B44                          BCC       $0000A53E(pc)
00009B48                          ADD.W     Invalid
00009B4A                          OR.B      #$BA,A4
00009B4E                          BTST      D5,(A0)
00009B50                          CMP.B     #$01,$0022(A0)
00009B56                          BEQ       $00009B7A
00009B58                          MOVE.B    #$00,$0005(A0)
00009B5E                          MOVE.B    #$00,$0004(A0)
00009B64                          CMP.B     #$08,$0016(A0)
00009B6A                          BLS       $00009B7A
00009B6C                          CMP.B     #$18,$0016(A0)
00009B72                          BHI       $00009B7A
00009B74                          MOVE.B    #$02,$0004(A0)
00009B7A                          JMP       $00028804
00009B80                          JSR       $0000A612(pc)
00009B84                          CMP.B     #$10,$0016(A0)
00009B8A                          BHI       $00009B98
00009B8C                          MOVE.W    $004C(A0),D0
00009B90                          CMP.W     $0008(A0),D0
00009B94                          BCS       $00009BA2
00009B96                          BRA       $00009BC2
00009B98                          MOVE.W    $0008(A0),D0
00009B9C                          CMP.W     $004C(A0),D0
00009BA0                          BCC       $00009BC2
00009BA2                          TST.B     $0004(A0)
00009BA6                          BNE       $00009BB6
00009BA8                          MOVE.W    $004E(A0),D0
00009BAC                          CMP.W     $000C(A0),D0
00009BB0                          BCS       $00009E74(pc)
00009BB4                          BRA       $00009BC2
00009BB6                          MOVE.W    $000C(A0),D0
00009BBA                          CMP.W     $004E(A0),D0
00009BBE                          BCS       $00009E74(pc)
00009BC2                          TST.B     $0004(A0)
00009BC6                          BNE       $00009BD8
00009BC8                          MOVE.W    $000C(A0),D0
00009BCC                          CMP.W     $004E(A0),D0
00009BD0                          BCS       $00009BE8
00009BD2                          JMP       $00028804
00009BD8                          MOVE.W    $004E(A0),D0
00009BDC                          CMP.W     $000C(A0),D0
00009BE0                          BCS       $00009BE8
00009BE2                          JMP       $00028804
00009BE8                          MOVE.B    #$02,$0005(A0)
00009BEE                          JSR       $0000A65E(pc)
00009BF2                          JSR       $00028870
00009BF8                          TST.B     $0004(A0)
00009BFC                          BNE       $00009C0A
00009BFE                          MOVE.W    $004E(A0),D0
00009C02                          CMP.W     $000C(A0),D0
00009C06                          BCC       $00009C1C
00009C08                          BRA       $00009C14
00009C0A                          MOVE.W    $000C(A0),D0
00009C0E                          CMP.W     $004E(A0),D0
00009C12                          BCC       $00009C1C
00009C14                          JSR       $0000A612(pc)
00009C18                          JSR       $0000A572(pc)
00009C1C                          CMP.B     #$10,$0016(A0)
00009C22                          BHI       $00009C3C
00009C24                          JSR       $00001CE6
00009C2A                          BCC       $00009C54
00009C2C                          CMP.B     #$01,$0024(A0)
00009C32                          BNE       $00009C54
00009C34                          ADD.W     Invalid
00009C36                          OR.B      #$00,A0
00009C3A                          BCLR      #$B9,(A4)
00009C3E                          OR.B      #$2E,D0
00009C42                          BCC       $00009C54
00009C44                          CMP.B     #$02,$0024(A0)
00009C4A                          BNE       $00009C54
00009C4C                          ADD.W     Invalid
00009C4E                          OR.B      #$00,A0
00009C52                          BCHG      #$B9,$0000
00009C58                          MOVE.B    $6400(A6),(A5)
00009C5C                          BSET      #$68,-(A2)
00009C60                          OR.B      #$BA,A4
00009C64                          BSET      D4,$0000AC8E(pc)
00009C68                          OR.B      #$28,
00009C6C                          OR.B      #$04,-(A2)
00009C70                          JSR       $00009F98(pc)
00009C74                          CMP.B     #$01,$0022(A0)
00009C7A                          BNE       $00009C82
00009C7C                          MOVE.B    #$00,$0005(A0)
00009C82                          JMP       $00028804
00009C88                          TST.B     $0005(A0)
00009C8C                          BNE       $00009CD0
00009C8E                          JSR       $0000A612(pc)
00009C92                          MOVE.W    $004E(A0),D0
00009C96                          CMP.W     $000C(A0),D0
00009C9A                          BCS       $00009E74(pc)
00009C9E                          CMP.B     #$0A,$0016(A0)
00009CA4                          BHI       $00009CB6
00009CA6                          MOVE.W    $0008(A0),D0
00009CAA                          CMP.W     $004C(A0),D0
00009CAE                          BCS       $00009CC6
00009CB0                          JMP       $00028804
00009CB6                          MOVE.W    $004C(A0),D0
00009CBA                          CMP.W     $0008(A0),D0
00009CBE                          BCS       $00009CC6
00009CC0                          JMP       $00028804
00009CC6                          MOVE.B    #$02,$0005(A0)
00009CCC                          JSR       $0000A674(pc)
00009CD0                          JSR       $00028870
00009CD6                          CMP.B     #$10,$0016(A0)
00009CDC                          BHI       $00009D14
00009CDE                          MOVE.W    $004C(A0),D0
00009CE2                          CMP.W     $0008(A0),D0
00009CE6                          BCC       $00009CF0
00009CE8                          JSR       $0000A612(pc)
00009CEC                          JSR       $0000A5BA(pc)
00009CF0                          JSR       $000019D4
00009CF6                          BCS       $00009D4A
00009CF8                          JSR       $00001C3C
00009CFE                          BCC       $0000A53E(pc)
00009D02                          ADD.W     Invalid
00009D04                          OR.B      #$28,A0
00009D08                          OR.B      #$24,D1
00009D0C                          BNE       $0000A53E(pc)
00009D10                          BRA       $0000A4CE(pc)
00009D14                          MOVE.W    $0008(A0),D0
00009D18                          CMP.W     $004C(A0),D0
00009D1C                          BCC       $00009D26
00009D1E                          JSR       $0000A612(pc)
00009D22                          JSR       $0000A5BA(pc)
00009D26                          JSR       $000019D4
00009D2C                          BCS       $00009D4A
00009D2E                          JSR       $00001C94
00009D34                          BCC       $0000A53E(pc)
00009D38                          ADD.W     Invalid
00009D3A                          OR.B      #$28,A0
00009D3E                          OR.B      #$24,D2
00009D42                          BNE       $0000A53E(pc)
00009D46                          BRA       $0000A4CA(pc)
00009D4A                          ADD.W     Invalid
00009D4C                          OR.B      #$BA,A4
00009D50                          BTST      D4,-(A4)
00009D52                          MOVE.B    $003F(A0),D0
00009D56                          CMP.B     $0023(A0),D0
00009D5A                          BEQ       $00009D64
00009D5C                          JSR       $0000A674(pc)
00009D60                          BRA       $00009FB0(pc)
00009D64                          JMP       $00028804
00009D6A                          TST.B     $0005(A0)
00009D6E                          BNE       $00009DA4
00009D70                          JSR       $0000A612(pc)
00009D74                          JSR       $0000A5EC(pc)
00009D78                          TST.B     $0004(A0)
00009D7C                          BNE       $00009D8E
00009D7E                          MOVE.W    $000C(A0),D0
00009D82                          CMP.W     $004E(A0),D0
00009D86                          BCS       $00009D9E
00009D88                          JMP       $00028804
00009D8E                          MOVE.W    $004E(A0),D0
00009D92                          CMP.W     $000C(A0),D0
00009D96                          BCS       $00009D9E
00009D98                          JMP       $00028804
00009D9E                          MOVE.B    #$02,$0005(A0)
00009DA4                          JSR       $00028870
00009DAA                          TST.B     $0004(A0)
00009DAE                          BNE       $00009DC2
00009DB0                          MOVE.W    $004E(A0),D0
00009DB4                          CMP.W     $000C(A0),D0
00009DB8                          BCC       $00009DD4
00009DBA                          JSR       $0000A612(pc)
00009DBE                          JSR       $0000A5EC(pc)
00009DC2                          MOVE.W    $000C(A0),D0
00009DC6                          CMP.W     $004E(A0),D0
00009DCA                          BCC       $00009DD4
00009DCC                          JSR       $0000A612(pc)
00009DD0                          JSR       $0000A5EC(pc)
00009DD4                          BTST      #$00,$0024(A0)
00009DDA                          BEQ       $00009E28
00009DDC                          SUBQ.W    #$1,$0008(A0)
00009DE0                          TST.B     $0004(A0)
00009DE4                          BNE       $00009DF6
00009DE6                          JSR       $000019D4
00009DEC                          BCC       $00009E06
00009DEE                          ADD.W     Invalid
00009DF0                          OR.B      #$00,A4
00009DF4                          ADD.W     #$4EB9,-(A6)
00009DF8                          OR.B      #$AE,D0
00009DFC                          BCC       $00009E06
00009DFE                          ADD.W     Invalid
00009E00                          OR.B      #$00,A4
00009E04                          BSET      D2,D2
00009E06                          ADDQ.W    #$1,$0008(A0)
00009E0A                          JSR       $00001C3C
00009E10                          BCC       $0000A53E(pc)
00009E14                          ADD.W     Invalid
00009E16                          OR.B      #$28,A0
00009E1A                          OR.B      #$24,D1
00009E1E                          BNE       $0000A53E(pc)
00009E22                          JMP       $00028804
00009E28                          ADDQ.W    #$1,$0008(A0)
00009E2C                          TST.B     $0004(A0)
00009E30                          BNE       $00009E42
00009E32                          JSR       $000019D4
00009E38                          BCC       $00009E52
00009E3A                          ADD.W     Invalid
00009E3C                          OR.B      #$00,A4
00009E40                          ADD.B     #$B9,(A2)+
00009E44                          OR.B      #$AE,D0
00009E48                          BCC       $00009E52
00009E4A                          ADD.W     Invalid
00009E4C                          OR.B      #$00,A4
00009E50                          BCLR      D2,-(A0)
00009E52                          SUBQ.W    #$1,$0008(A0)
00009E56                          JSR       $00001C94
00009E5C                          BCC       $0000A53E(pc)
00009E60                          ADD.W     Invalid
00009E62                          OR.B      #$28,A0
00009E66                          OR.B      #$24,D2
00009E6A                          BNE       $0000A53E(pc)
00009E6E                          JMP       $00028804
00009E74                          LEA       $0000AB16(pc),A3
00009E78                          MOVE.W    $0000(A3),$0018(A0)
00009E7E                          MOVE.B    $0044(A0),$0003(A0)
00009E84                          MOVE.B    #$00,$0004(A0)
00009E8A                          BSR       $00009E90
00009E8C                          BRA       $00009F5A(pc)
00009E90                          LEA       $0000A838(pc),A2
00009E94                          TST.B     $003E(A0)
00009E98                          BEQ       $00009E9E
00009E9A                          LEA       $0000A87C(pc),A2
00009E9E                          MOVE.L    A2,$0038(A0)
00009EA2                          MOVEQ     #$00,D0
00009EA4                          MOVE.B    $003C(A0),D0
00009EA8                          ADD.W     D0,D0
00009EAA                          MOVE.B    $00(A2,D0.W),$003D(A0)
00009EB0                          MOVE.B    $00(A2,A0.W),$0016(A0)
00009EB6                          RTS
00009EB8                          JSR       $00028870
00009EBE                          JSR       $0000A1C8(pc)
00009EC2                          ADDQ.B    #$1,$003D(A0)
00009EC6                          BTST      #$00,$003D(A0)
00009ECC                          BEQ       $00009EE4
00009ECE                          MOVE.W    $004C(A0),D0
00009ED2                          MOVE.W    $004E(A0),D1
00009ED6                          JSR       $000288F2
00009EDC                          ADDQ.B    #$4,D6
00009EDE                          LSR.B     #3,D6
00009EE0                          MOVE.B    D6,$0016(A0)
00009EE4                          MOVE.W    $004C(A0),D0
00009EE8                          SUB.W     $0008(A0),D0
00009EEC                          ADD.W     #$0010,D0
00009EF0                          CMP.W     #$0018,D0
00009EF4                          BCC       $00009F0A
00009EF6                          MOVE.W    $004E(A0),D0
00009EFA                          SUB.W     $000C(A0),D0
00009EFE                          ADD.W     #$0010,D0
00009F02                          CMP.W     #$0018,D0
00009F06                          BCS       $00009E74(pc)
00009F0A                          CMP.B     #$08,$0016(A0)
00009F10                          BEQ       $00009F3C
00009F12                          BLS       $00009F2A
00009F14                          CMP.B     #$18,$0016(A0)
00009F1A                          BEQ       $00009F4A
00009F1C                          BHI       $00009F2A
00009F1E                          MOVE.W    $000C(A0),D0
00009F22                          CMP.W     $004E(A0),D0
00009F26                          BCC       $00009F56
00009F28                          BRA       $00009F34
00009F2A                          MOVE.W    $004E(A0),D0
00009F2E                          CMP.W     $000C(A0),D0
00009F32                          BCC       $00009F56
00009F34                          CMP.B     #$10,$0016(A0)
00009F3A                          BHI       $00009F4A
00009F3C                          MOVE.W    $004C(A0),D0
00009F40                          CMP.W     $0008(A0),D0
00009F44                          BCS       $00009E74(pc)
00009F48                          BRA       $00009F56
00009F4A                          MOVE.W    $0008(A0),D0
00009F4E                          CMP.W     $004C(A0),D0
00009F52                          BCS       $00009E74(pc)
00009F56                          BRA       $0000A33E(pc)
00009F5A                          LEA       $A000,A1
00009F5E                          MOVE.W    $0008(A1),D0
00009F62                          MOVE.W    $000C(A1),D1
00009F66                          JSR       $000288F2
00009F6C                          ADDQ.B    #$0,D6
00009F6E                          LSR.B     #4,D6
00009F70                          ADDQ.B    #$0,D6
00009F72                          AND.B     #$0F,D6
00009F76                          CMP.B     $003F(A0),D6
00009F7A                          BEQ       $00009F86
00009F7C                          MOVE.B    D6,$003F(A0)
00009F80                          MOVEQ     #$00,D0
00009F82                          MOVE.B    D6,D0
00009F84                          BRA       $00009F88
00009F86                          RTS
00009F88                          DBRA      D0,$0000A18A
00009F8C                          OR.B      #$FA,A7
00009F90                          MOVE.B    -(A4),-(A2)
00009F92                          JMP       $000287D6
00009F98                          MOVE.B    $0022(A0),$003F(A0)
00009F9E                          MOVEQ     #$00,D0
00009FA0                          MOVE.B    $0022(A0),D0
00009FA4                          LEA       $0000B4D4,A4
00009FAA                          JMP       $000287D6
00009FB0                          MOVE.B    $0023(A0),$003F(A0)
00009FB6                          MOVEQ     #$00,D0
00009FB8                          MOVE.B    $0023(A0),D0
00009FBC                          LEA       $0000B4E8,A4
00009FC2                          JMP       $000287D6
00009FC8                          LEA       $A000,A1
00009FCC                          MOVE.B    $0030(A1),D0
00009FD0                          CMP.B     $003E(A0),D0
00009FD4                          BEQ       $0000A030
00009FD6                          MOVE.B    #$00,$0042(A0)
00009FDC                          MOVEQ     #$00,D0
00009FDE                          MOVE.B    $003C(A0),D0
00009FE2                          ADD.W     D0,D0
00009FE4                          MOVE.W    +$00(D3.W),D1
00009FE8                          JMP       +$10(D1.W)
00009FEC                          OR.W      #$007E,$00(A0,D2.W)
00009FF2                          OR.L      #$00440044,A4
00009FF8                          OR.W      #$0044,D4
00009FFC                          OR.L      #$00440044,(A2)+
0000A002                          OR.W      #$00A8,D4
0000A006                          OR.W      #$00B6,D4
0000A00A                          Invalid
0000A00C                          Invalid
0000A00E                          Invalid
0000A010                          Invalid
0000A012                          OR.W      #$00F2,D4
0000A016                          OR.W      #$0044,D4
0000A01A                          OR.W      #$0044,D4
0000A01E                          Invalid
0000A020                          OR.W      #$0044,D4
0000A024                          OR.W      #$010A,D4
0000A028                          OR.W      #$0116,D4
0000A02C                          BTST      D0,-(A2)
0000A02E                          BTST      D0,$5228(A6)
0000A032                          OR.B      #$28,CCR
0000A036                          OR.B      #$3C,-(A1)
0000A03A                          BLS       $0000A046
0000A03C                          MOVE.B    #$00,$003C(A0)
0000A042                          JSR       $0000A16C(pc)
0000A046                          MOVEQ     #$00,D0
0000A048                          MOVE.B    $003C(A0),D0
0000A04C                          ADD.W     D0,D0
0000A04E                          MOVE.B    $00(A2,D0.W),$003D(A0)
0000A054                          MOVE.B    $00(A2,A0.W),$0016(A0)
0000A05A                          RTS
0000A05C                          LEA       $0000A9F0(pc),A2
0000A060                          MOVE.L    #$130F0B17,D0
0000A066                          BRA       $0000A124(pc)
0000A06A                          LEA       $0000AA9E(pc),A2
0000A06E                          MOVE.L    #$10060C15,D0
0000A074                          BRA       $0000A124(pc)
0000A078                          LEA       $0000AA20(pc),A2
0000A07C                          MOVE.L    #$11FF0E19,D0
0000A082                          BRA       $0000A124(pc)
0000A086                          LEA       $0000AA3A(pc),A2
0000A08A                          MOVE.L    #$1206101D,D0
0000A090                          BRA       $0000A124(pc)
0000A094                          LEA       $0000AA58(pc),A2
0000A098                          MOVE.L    #$11100E1F,D0
0000A09E                          BRA       $0000A124(pc)
0000A0A2                          LEA       $0000AAB4(pc),A2
0000A0A6                          MOVE.L    #$0F141120,D0
0000A0AC                          BRA       $0000A124
0000A0AE                          LEA       $0000AA78(pc),A2
0000A0B2                          MOVE.L    #$10FF0B24,D0
0000A0B8                          BRA       $0000A124
0000A0BA                          LEA       $0000AAD6(pc),A2
0000A0BE                          MOVE.L    #$010F0F16,D0
0000A0C4                          BRA       $0000A124
0000A0C6                          LEA       $0000A9F0(pc),A2
0000A0CA                          MOVE.L    #$020F0B17,D0
0000A0D0                          BRA       $0000A124
0000A0D2                          LEA       $0000AA9E(pc),A2
0000A0D6                          MOVE.L    #$21060C15,D0
0000A0DC                          BRA       $0000A124
0000A0DE                          LEA       $0000AA20(pc),A2
0000A0E2                          MOVE.L    #$00FF0E19,D0
0000A0E8                          BRA       $0000A124
0000A0EA                          LEA       $0000AA3A(pc),A2
0000A0EE                          MOVE.L    #$0106101D,D0
0000A0F4                          BRA       $0000A124
0000A0F6                          LEA       $0000AA58(pc),A2
0000A0FA                          MOVE.L    #$00100E1F,D0
0000A100                          BRA       $0000A124
0000A102                          LEA       $0000AAB4(pc),A2
0000A106                          MOVE.L    #$20141120,D0
0000A10C                          BRA       $0000A124
0000A10E                          LEA       $0000AA78(pc),A2
0000A112                          MOVE.L    #$21FF0B24,D0
0000A118                          BRA       $0000A124
0000A11A                          LEA       $0000AAD6(pc),A2
0000A11E                          MOVE.L    #$120F0F16,D0
0000A124                          MOVE.L    A2,$0038(A0)
0000A128                          MOVE.B    (A2),$003D(A0)
0000A12C                          MOVE.B    D0,$0043(A0)
0000A130                          LSR.L     #8,D0
0000A132                          MOVE.B    D0,$0040(A0)
0000A136                          LSR.L     #8,D0
0000A138                          MOVE.B    D0,$0041(A0)
0000A13C                          LSR.L     #8,D0
0000A13E                          MOVE.B    D0,$0045(A0)
0000A142                          MOVE.B    #$02,$0004(A0)
0000A148                          MOVE.B    $0030(A1),$003E(A0)
0000A14E                          TST.B     $003E(A0)
0000A152                          BNE       $0000A160
0000A154                          MOVE.B    #$00,$0005(A0)
0000A15A                          ADDQ.B    #$1,$0016(A0)
0000A15E                          BRA       $0000A164
0000A160                          SUBQ.B    #$1,$0016(A0)
0000A164                          AND.B     #$1F,$0016(A0)
0000A16A                          RTS
0000A16C                          LEA       $A000,A1
0000A170                          MOVE.W    $000C(A0),$002C(A0)
0000A176                          MOVE.W    $0034(A0),D0
0000A17A                          SUB.W     #$0010,D0
0000A17E                          (CCC     #$0,$002C(A0)
0000A182                          MOVE.W    $0008(A0),$002A(A0)
0000A188                          MOVE.W    $0030(A0),D0
0000A18C                          TST.B     $003E(A0)
0000A190                          BNE       $0000A198
0000A192                          ADD.W     #$0028,D0
0000A196                          BRA       $0000A19C
0000A198                          SUB.W     #$0028,D0
0000A19C                          (CCC     #$0,$002A(A0)
0000A1A0                          RTS
0000A1A2                          MOVE.W    $A008,D0
0000A1A6                          SUB.W     $0030(A0),D0
0000A1AA                          ADD.W     Invalid
0000A1AC                          OR.B      #$78,A0
0000A1B0                          Invalid
0000A1B2                          OR.B      #$38,$A0(A0,D6.W)
0000A1B8                          SUB.W     $0034(A0),D0
0000A1BC                          ADD.W     Invalid
0000A1BE                          OR.B      #$78,A4
0000A1C2                          Invalid
0000A1C4                          OR.B      #$75,$43(A4,D4.L)
0000A1CA                          Invalid
0000A1CC                          MOVE.W    $000C(A1),D0
0000A1D0                          SUB.W     $0034(A0),D0
0000A1D4                          ADD.W     Invalid
0000A1D6                          OR.B      #$68,$00(A4,D7.W)
0000A1DC                          MOVE.W    $0008(A1),D0
0000A1E0                          SUB.W     $0030(A0),D0
0000A1E4                          ADD.W     Invalid
0000A1E6                          OR.B      #$68,$00(A0,D6.W)
0000A1EC                          RTS
0000A1EE                          MOVEQ     #$01,D2
0000A1F0                          MOVE.W    $002C(A0),D0
0000A1F4                          BEQ       $0000A1FA
0000A1F6                          BMI       $0000A20A
0000A1F8                          BPL       $0000A214
0000A1FA                          MOVE.W    $002A(A0),D0
0000A1FE                          BEQ       $0000A204
0000A200                          BMI       $0000A21E
0000A202                          BPL       $0000A228
0000A204                          DBRA      D2,$0000A1F0
0000A208                          RTS
0000A20A                          ADDQ.W    #$1,$002C(A0)
0000A20E                          ADDQ.W    #$1,$000C(A0)
0000A212                          BRA       $0000A1FA
0000A214                          SUBQ.W    #$1,$002C(A0)
0000A218                          SUBQ.W    #$1,$000C(A0)
0000A21C                          BRA       $0000A1FA
0000A21E                          ADDQ.W    #$1,$002A(A0)
0000A222                          ADDQ.W    #$1,$0008(A0)
0000A226                          BRA       $0000A204
0000A228                          SUBQ.W    #$1,$002A(A0)
0000A22C                          SUBQ.W    #$1,$0008(A0)
0000A230                          BRA       $0000A204
0000A232                          BSR       $0000A288
0000A234                          ADD.L     #$0000B300,$0048(A0)
0000A23C                          ADDQ.B    #$1,$0047(A0)
0000A240                          CMP.B     #$1E,$0047(A0)
0000A246                          BLS       $0000A25C
0000A248                          MOVE.B    #$02,$002E(A0)
0000A24E                          MOVE.B    #$1D,$0047(A0)
0000A254                          SUB.L     #$00016600,$0048(A0)
0000A25C                          RTS
0000A25E                          BSR       $0000A288
0000A260                          SUB.L     #$0000B300,$0048(A0)
0000A268                          SUBQ.B    #$1,$0047(A0)
0000A26C                          BCC       $0000A286
0000A26E                          MOVE.B    $0044(A0),$0003(A0)
0000A274                          MOVE.B    #$00,$002E(A0)
0000A27A                          MOVE.B    #$00,$0047(A0)
0000A280                          BCLR      #$02,$0043(A1)
0000A286                          RTS
0000A288                          LEA       $A000,A1
0000A28C                          MOVE.L    $0048(A0),D0
0000A290                          ADD.L     $000C(A1),D0
0000A294                          SUB.L     $0034(A0),D0
0000A298                          ADD.L     Invalid
0000A29A                          OR.B      #$A8,$00(A4,D6.W)
0000A2A0                          MOVE.W    $0008(A1),D0
0000A2A4                          SUB.W     $0030(A0),D0
0000A2A8                          ADD.W     Invalid
0000A2AA                          OR.B      #$68,$00(A0,D4.W)
0000A2B0                          RTS
0000A2B2                          SUBQ.B    #$1,$0047(A0)
0000A2B6                          BNE       $0000A2C6
0000A2B8                          MOVE.B    #$02,$002E(A0)
0000A2BE                          MOVE.L    #$0005DC00,$0048(A0)
0000A2C6                          RTS
0000A2C8                          MOVE.L    $0048(A0),D0
0000A2CC                          ADD.L     Invalid
0000A2CE                          OR.B      #$A8,$00(A4,D6.W)
0000A2D4                          SUB.L     #$00000300,$0048(A0)
0000A2DC                          ADDQ.B    #$1,$0047(A0)
0000A2E0                          CMP.B     #$1E,$0047(A0)
0000A2E6                          BLS       $0000A304
0000A2E8                          MOVE.B    #$1D,$0047(A0)
0000A2EE                          MOVE.B    #$04,$002E(A0)
0000A2F4                          ADD.L     #$00000300,$0048(A0)
0000A2FC                          ADD.L     #$00000300,$0048(A0)
0000A304                          RTS
0000A306                          LEA       $A000,A1
0000A30A                          MOVE.L    $0048(A0),D0
0000A30E                          (CCC     #$0,$0034(A0)
0000A312                          (CCC     #$0,$000C(A0)
0000A316                          ADD.L     #$00000300,$0048(A0)
0000A31E                          SUBQ.B    #$1,$0047(A0)
0000A322                          BCC       $0000A33C
0000A324                          MOVE.B    #$00,$0047(A0)
0000A32A                          MOVE.B    $0044(A0),$0003(A0)
0000A330                          MOVE.B    #$00,$002E(A0)
0000A336                          BCLR      #$00,$0043(A1)
0000A33C                          RTS
0000A33E                          CMP.B     #$00,$0016(A0)
0000A344                          BEQ       $0000A372
0000A346                          CMP.B     #$10,$0016(A0)
0000A34C                          BHI       $0000A372
0000A34E                          JSR       $00001CE6
0000A354                          BCC       $0000A396
0000A356                          ADD.W     Invalid
0000A358                          OR.B      #$28,A0
0000A35C                          OR.B      #$24,D1
0000A360                          BNE       $0000A3C4
0000A362                          JSR       $00001AE2
0000A368                          BCC       $0000A4CE(pc)
0000A36C                          ADD.W     Invalid
0000A36E                          OR.B      #$54,A4
0000A372                          JSR       $00001D2E
0000A378                          BCC       $0000A396
0000A37A                          ADD.W     Invalid
0000A37C                          OR.B      #$28,A0
0000A380                          OR.B      #$24,D2
0000A384                          BNE       $0000A3C4
0000A386                          JSR       $00001AE2
0000A38C                          BCC       $0000A4CA(pc)
0000A390                          ADD.W     Invalid
0000A392                          OR.B      #$30,A4
0000A396                          CMP.B     #$08,$0016(A0)
0000A39C                          BLS       $0000A3B4
0000A39E                          CMP.B     #$18,$0016(A0)
0000A3A4                          BHI       $0000A3B4
0000A3A6                          JSR       $00001AE2
0000A3AC                          BCC       $0000A3C4
0000A3AE                          ADD.W     Invalid
0000A3B0                          OR.B      #$12,A4
0000A3B4                          JSR       $000019D4
0000A3BA                          BCC       $0000A3C4
0000A3BC                          ADD.W     Invalid
0000A3BE                          OR.B      #$00,A4
0000A3C2                          OR.L      #$4E750C28,(A0)+
0000A3C8                          OR.B      #$03,A2
0000A3CC                          BHI       $0000A3D4
0000A3CE                          TST.B     $0004(A0)
0000A3D2                          BNE       $0000A3DC
0000A3D4                          CMP.B     #$0C,$0003(A0)
0000A3DA                          BNE       $0000A3E8
0000A3DC                          MOVE.B    #$00,$003C(A0)
0000A3E2                          MOVE.B    #$00,$0044(A0)
0000A3E8                          MOVE.B    #$00,$0005(A0)
0000A3EE                          BRA       $0000A3F6
0000A3F0                          MOVE.B    #$02,$0005(A0)
0000A3F6                          MOVE.B    #$0E,$0003(A0)
0000A3FC                          LEA       $0000AB16(pc),A3
0000A400                          MOVE.W    $0006(A3),$0018(A0)
0000A406                          TST.B     $0050(A0)
0000A40A                          BEQ       $0000A412
0000A40C                          MOVE.W    $0008(A3),$0018(A0)
0000A412                          MOVE.B    #$00,$003F(A0)
0000A418                          JSR       $00009F98(pc)
0000A41C                          LEA       $0000AAEE(pc),A2
0000A420                          TST.B     $003E(A0)
0000A424                          BEQ       $0000A42A
0000A426                          LEA       $0000AAF6(pc),A2
0000A42A                          MOVEQ     #$00,D0
0000A42C                          MOVE.B    $0022(A0),D0
0000A430                          MOVE.B    $00(A2,D0.W),$0016(A0)
0000A436                          TST.B     $0022(A0)
0000A43A                          BEQ       $0000A458
0000A43C                          MOVE.B    #$00,$0004(A0)
0000A442                          CMP.B     #$08,$0016(A0)
0000A448                          BLS       $0000A458
0000A44A                          CMP.B     #$18,$0016(A0)
0000A450                          BHI       $0000A458
0000A452                          MOVE.B    #$02,$0004(A0)
0000A458                          RTS
0000A45A                          CMP.B     #$0A,$0003(A0)
0000A460                          BHI       $0000A468
0000A462                          TST.B     $0004(A0)
0000A466                          BNE       $0000A470
0000A468                          CMP.B     #$0C,$0003(A0)
0000A46E                          BNE       $0000A47C
0000A470                          MOVE.B    #$00,$003C(A0)
0000A476                          MOVE.B    #$00,$0044(A0)
0000A47C                          LEA       $0000AB06(pc),A2
0000A480                          TST.B     $003E(A0)
0000A484                          BEQ       $0000A48A
0000A486                          LEA       $0000AB0E(pc),A2
0000A48A                          MOVEQ     #$00,D0
0000A48C                          MOVE.B    $0023(A0),D0
0000A490                          MOVE.B    $00(A2,D0.W),$0016(A0)
0000A496                          MOVE.B    #$12,$0003(A0)
0000A49C                          MOVE.B    #$00,$0005(A0)
0000A4A2                          LEA       $0000AB16(pc),A3
0000A4A6                          MOVE.W    $0006(A3),$0018(A0)
0000A4AC                          TST.B     $0050(A0)
0000A4B0                          BEQ       $0000A4B8
0000A4B2                          MOVE.W    $0008(A3),$0018(A0)
0000A4B8                          MOVEQ     #$00,D0
0000A4BA                          MOVE.B    $0023(A0),D0
0000A4BE                          LEA       $0000B4E8,A4
0000A4C4                          JMP       $000287D6
0000A4CA                          MOVEQ     #$01,D0
0000A4CC                          BRA       $0000A4D0
0000A4CE                          MOVEQ     #$00,D0
0000A4D0                          CMP.B     #$0A,$0003(A0)
0000A4D6                          BHI       $0000A4DE
0000A4D8                          TST.B     $0004(A0)
0000A4DC                          BNE       $0000A4E6
0000A4DE                          CMP.B     #$0C,$0003(A0)
0000A4E4                          BNE       $0000A4F2
0000A4E6                          MOVE.B    #$00,$003C(A0)
0000A4EC                          MOVE.B    #$00,$0044(A0)
0000A4F2                          MOVE.B    #$00,$0016(A0)
0000A4F8                          MOVE.B    #$00,$0004(A0)
0000A4FE                          MOVE.B    #$00,$0005(A0)
0000A504                          TST.B     $003E(A0)
0000A508                          BNE       $0000A516
0000A50A                          MOVE.B    #$10,$0016(A0)
0000A510                          MOVE.B    #$02,$0004(A0)
0000A516                          MOVE.B    #$10,$0003(A0)
0000A51C                          LEA       $0000AB16(pc),A3
0000A520                          MOVE.W    $0006(A3),$0018(A0)
0000A526                          TST.B     $0050(A0)
0000A52A                          BEQ       $0000A532
0000A52C                          MOVE.W    $0008(A3),$0018(A0)
0000A532                          LEA       $0000B4E4,A4
0000A538                          JMP       $000287D6
0000A53E                          LEA       $0000AB16(pc),A3
0000A542                          MOVE.W    $0000(A3),$0018(A0)
0000A548                          MOVE.W    $004C(A0),D0
0000A54C                          MOVE.W    $004E(A0),D1
0000A550                          JSR       $000288F2
0000A556                          ADDQ.B    #$4,D6
0000A558                          LSR.B     #3,D6
0000A55A                          MOVE.B    D6,$0016(A0)
0000A55E                          MOVE.B    #$14,$0003(A0)
0000A564                          MOVE.B    #$00,$0004(A0)
0000A56A                          LEA       $A000,A1
0000A56E                          BRA       $00009F5A(pc)
0000A572                          MOVE.W    $004C(A0),D0
0000A576                          MOVE.W    $004E(A0),D1
0000A57A                          JSR       $000288F2
0000A580                          ADDQ.B    #$4,D6
0000A582                          LSR.B     #3,D6
0000A584                          MOVE.B    $0016(A0),D0
0000A588                          MOVE.B    D0,$0051(A0)
0000A58C                          ADD.B     #$10,D0
0000A590                          AND.B     #$1F,D0
0000A594                          MOVE.B    D0,$0052(A0)
0000A598                          MOVE.B    $0051(A0),D0
0000A59C                          MOVE.B    $0052(A0),D1
0000A5A0                          CMP.B     #$10,$0016(A0)
0000A5A6                          BHI       $0000A5AA
0000A5A8                          8         D1,D0
0000A5AA                          CMP.B     D6,D0
0000A5AC                          BCS       $0000A5B4
0000A5AE                          CMP.B     D1,D6
0000A5B0                          BCS       $0000A5B4
0000A5B2                          RTS
0000A5B4                          MOVE.L    (A7)+,D0
0000A5B6                          BRA       $00009E74(pc)
0000A5BA                          MOVE.W    $000C(A0),D0
0000A5BE                          CMP.W     $004E(A0),D0
0000A5C2                          BCS       $0000A5C6
0000A5C4                          RTS
0000A5C6                          CMP.B     #$10,$0016(A0)
0000A5CC                          BHI       $0000A5DA
0000A5CE                          MOVE.W    $004C(A0),D0
0000A5D2                          CMP.W     $0008(A0),D0
0000A5D6                          BCS       $0000A5E6
0000A5D8                          RTS
0000A5DA                          MOVE.W    $0008(A0),D0
0000A5DE                          CMP.W     $004C(A0),D0
0000A5E2                          BCS       $0000A5E6
0000A5E4                          RTS
0000A5E6                          MOVE.L    (A7)+,D0
0000A5E8                          BRA       $00009E74(pc)
0000A5EC                          BTST      #$00,$0024(A0)
0000A5F2                          BEQ       $0000A600
0000A5F4                          MOVE.W    $004C(A0),D0
0000A5F8                          CMP.W     $0008(A0),D0
0000A5FC                          BCS       $0000A60C
0000A5FE                          RTS
0000A600                          MOVE.W    $0008(A0),D0
0000A604                          CMP.W     $004C(A0),D0
0000A608                          BCS       $0000A60C
0000A60A                          RTS
0000A60C                          MOVE.L    (A7)+,D0
0000A60E                          BRA       $00009E74(pc)
0000A612                          LEA       $0000A968(pc),A2
0000A616                          TST.B     $003E(A0)
0000A61A                          BEQ       $0000A620
0000A61C                          LEA       $0000A9AC(pc),A2
0000A620                          MOVE.L    A2,$0038(A0)
0000A624                          ADDQ.B    #$1,$003C(A0)
0000A628                          CMP.B     #$21,$003C(A0)
0000A62E                          BLS       $0000A636
0000A630                          MOVE.B    #$00,$003C(A0)
0000A636                          MOVEQ     #$00,D0
0000A638                          MOVEQ     #$00,D1
0000A63A                          MOVE.B    $003C(A0),D0
0000A63E                          ADD.W     D0,D0
0000A640                          MOVE.B    $00(A2,D0.W),D1
0000A644                          EXT.W     D1
0000A646                          ADD.W     $0030(A0),D1
0000A64A                          MOVE.W    D1,$004C(A0)
0000A64E                          MOVE.B    $00(A2,A0.W),D1
0000A652                          EXT.W     D1
0000A654                          ADD.W     $0034(A0),D1
0000A658                          MOVE.W    D1,$004E(A0)
0000A65C                          RTS
0000A65E                          LEA       $0000AAEE(pc),A2
0000A662                          TST.B     $003E(A0)
0000A666                          BEQ       $0000A66C
0000A668                          LEA       $0000AAF6(pc),A2
0000A66C                          MOVEQ     #$00,D0
0000A66E                          MOVE.B    $0022(A0),D0
0000A672                          BRA       $0000A688
0000A674                          LEA       $0000AB06(pc),A2
0000A678                          TST.B     $003E(A0)
0000A67C                          BEQ       $0000A682
0000A67E                          LEA       $0000AB0E(pc),A2
0000A682                          MOVEQ     #$00,D0
0000A684                          MOVE.B    $0023(A0),D0
0000A688                          MOVE.B    $00(A2,D0.W),$0016(A0)
0000A68E                          RTS
0000A690                          MOVE.B    $F09F,D0
0000A694                          AND.B     #$07,D0
0000A698                          BNE       $0000A6BA
0000A69A                          MOVE.W    $A008,D0
0000A69E                          MOVE.W    $A00C,D1
0000A6A2                          JSR       $000288F2
0000A6A8                          ADDQ.B    #$4,D6
0000A6AA                          LSR.B     #3,D6
0000A6AC                          MOVE.B    D6,$0016(A0)
0000A6B0                          MOVE.B    D6,D0
0000A6B2                          LSR.B     #1,D0
0000A6B4                          ADDQ.B    #$0,D0
0000A6B6                          JSR       $00009F88(pc)
0000A6BA                          JSR       $00028842
0000A6C0                          MOVE.W    $A008,D0
0000A6C4                          SUB.W     $0008(A0),D0
0000A6C8                          ADD.W     #$0010,D0
0000A6CC                          CMP.W     #$0020,D0
0000A6D0                          BCC       $0000A6E8
0000A6D2                          MOVE.W    $A00C,D0
0000A6D6                          SUB.W     $000C(A0),D0
0000A6DA                          ADD.W     #$0020,D0
0000A6DE                          CMP.W     #$0040,D0
0000A6E2                          BCC       $0000A6E8
0000A6E4                          BRA       $0000A7DA(pc)
0000A6E8                          RTS
0000A6EA                          MOVE.L    $0038(A0),A2
0000A6EE                          MOVEQ     #$00,D0
0000A6F0                          MOVE.B    $0004(A0),D0
0000A6F4                          MOVE.W    +$00(D7.W),D0
0000A6F8                          JSR       +$00(D5.W)
0000A6FC                          JSR       $0000A1A2(pc)
0000A700                          BRA       $0000A33E(pc)
0000A704                          OR.B      #$6A,D6
0000A708                          Invalid
0000A70A                          SUBQ.B    #$1,$003D(A0)
0000A70E                          BNE       $0000A73C
0000A710                          ADDQ.B    #$1,$003C(A0)
0000A714                          CMP.B     #$13,$003C(A0)
0000A71A                          BEQ       $0000A742
0000A71C                          MOVEQ     #$00,D0
0000A71E                          MOVE.B    $003C(A0),D0
0000A722                          ADD.W     D0,D0
0000A724                          MOVE.B    $00(A2,D0.W),$003D(A0)
0000A72A                          MOVE.B    $00(A2,A0.W),$0016(A0)
0000A730                          MOVE.B    $0016(A0),D0
0000A734                          LSR.B     #1,D0
0000A736                          ADDQ.B    #$0,D0
0000A738                          JSR       $00009F88(pc)
0000A73C                          JMP       $00028870
0000A742                          MOVE.B    #$03,$0042(A0)
0000A748                          MOVE.B    #$02,$0004(A0)
0000A74E                          ADDQ.B    #$4,$0016(A0)
0000A752                          TST.B     $003E(A0)
0000A756                          BEQ       $0000A75C
0000A758                          DBRA      D0,$0000A770
0000A75C                          AND.B     #$1F,$0016(A0)
0000A762                          MOVE.B    $0016(A0),D0
0000A766                          LSR.B     #1,D0
0000A768                          ADDQ.B    #$0,D0
0000A76A                          BRA       $00009F88(pc)
0000A76E                          SUBQ.B    #$1,$0042(A0)
0000A772                          BEQ       $0000A79C
0000A774                          TST.B     $003E(A0)
0000A778                          BNE       $0000A786
0000A77A                          ADDQ.B    #$4,$0016(A0)
0000A77E                          AND.B     #$1F,$0016(A0)
0000A784                          BRA       $0000A790
0000A786                          SUBQ.B    #$4,$0016(A0)
0000A78A                          AND.B     #$1F,$0016(A0)
0000A790                          MOVE.B    $0016(A0),D0
0000A794                          LSR.B     #1,D0
0000A796                          ADDQ.B    #$0,D0
0000A798                          BRA       $00009F88(pc)
0000A79C                          MOVE.B    #$04,$0004(A0)
0000A7A2                          MOVEQ     #$00,D0
0000A7A4                          MOVE.B    $003C(A0),D0
0000A7A8                          ADD.W     D0,D0
0000A7AA                          MOVE.B    $00(A2,D0.W),$003D(A0)
0000A7B0                          MOVE.B    $00(A2,A0.W),$0016(A0)
0000A7B6                          MOVE.B    $0016(A0),D0
0000A7BA                          LSR.B     #1,D0
0000A7BC                          ADDQ.B    #$0,D0
0000A7BE                          JSR       $00009F88(pc)
0000A7C2                          JMP       $00028870
0000A7C8                          SUBQ.B    #$1,$003D(A0)
0000A7CC                          BNE       $0000A7C2
0000A7CE                          ADDQ.B    #$1,$003C(A0)
0000A7D2                          CMP.B     #$29,$003C(A0)
0000A7D8                          BLS       $0000A7A2
0000A7DA                          LEA       $A000,A1
0000A7DE                          MOVE.B    #$00,$0004(A0)
0000A7E4                          LEA       $0000AB16(pc),A3
0000A7E8                          MOVE.W    $0000(A3),$0018(A0)
0000A7EE                          MOVE.B    #$00,$0003(A0)
0000A7F4                          MOVE.B    #$00,$003C(A0)
0000A7FA                          LEA       $0000A838(pc),A2
0000A7FE                          MOVE.B    $0030(A1),$003E(A0)
0000A804                          BEQ       $0000A80A
0000A806                          LEA       $0000A87C(pc),A2
0000A80A                          JSR       $0000A16C(pc)
0000A80E                          MOVE.L    A2,$0038(A0)
0000A812                          MOVE.B    (A2)+,$003D(A0)
0000A816                          MOVE.B    (A2),$0016(A0)
0000A81A                          JSR       $00009F5A(pc)
0000A81E                          JMP       $00028870
0000A824                          MOVE.B    #$06,$0002(A0)
0000A82A                          RTS
0000A82C                          MOVE.B    #$00,$0000(A0)
0000A832                          JMP       $00028B48
0000A838                          SUB.B     #$15,(A4)
0000A83C                          SUB.B     #$17,(A6)
0000A840                          AND.B     #$19,(A0)+
0000A844                          BTST      D0,(A2)+
0000A846                          BTST      D0,(A3)+
0000A848                          BTST      D0,(A4)+
0000A84A                          BTST      D0,(A5)+
0000A84C                          BTST      D0,(A6)+
0000A84E                          AND.B     #$00,(A7)+
0000A852                          BTST      D1,D1
0000A854                          SUB.B     #$03,D2
0000A858                          SUB.B     #$04,D4
0000A85C                          SUB.B     #$06,D5
0000A860                          BTST      D1,D7
0000A862                          AND.B     #$09,A0
0000A866                          MOVEP.W   $010B(A2),D0
0000A86A                          MOVEP.W   $010D(A4),D0
0000A86E                          MOVEP.W   $020F(A6),D0
0000A872                          AND.B     #$11,(A0)
0000A876                          SUB.B     #$13,(A2)
0000A87A                          SUB.B     #$0C,(A4)
0000A87E                          SUB.B     #$0A,A3
0000A882                          MOVEP.W   $0208(A1),D1
0000A886                          AND.B     #$06,D7
0000A88A                          BTST      D0,D5
0000A88C                          BTST      D0,D4
0000A88E                          BTST      D0,D3
0000A890                          BTST      D0,D2
0000A892                          AND.B     #$00,D1
0000A896                          BTST      D1,(A7)+
0000A898                          SUB.B     #$1D,(A6)+
0000A89C                          SUB.B     #$1C,(A4)+
0000A8A0                          SUB.B     #$1A,(A3)+
0000A8A4                          BTST      D1,(A1)+
0000A8A6                          AND.B     #$17,(A0)+
0000A8AA                          BTST      D0,(A6)
0000A8AC                          BTST      D0,(A5)
0000A8AE                          BTST      D0,(A4)
0000A8B0                          BTST      D0,(A3)
0000A8B2                          BTST      D0,(A2)
0000A8B4                          AND.B     #$10,(A1)
0000A8B8                          MOVEP.W   $040E(A7),D1
0000A8BC                          SUB.B     #$0C,A5
0000A8C0                          BTST      D0,(A2)
0000A8C2                          BTST      D1,(A3)
0000A8C4                          BTST      D1,(A4)
0000A8C6                          BTST      D0,(A5)
0000A8C8                          BTST      D0,(A6)
0000A8CA                          BTST      D0,(A7)
0000A8CC                          BTST      D1,(A0)+
0000A8CE                          BTST      D0,(A1)+
0000A8D0                          BTST      D0,(A2)+
0000A8D2                          BTST      D0,(A3)+
0000A8D4                          BTST      D0,(A4)+
0000A8D6                          BTST      D1,(A5)+
0000A8D8                          SUB.B     #$1B,(A4)+
0000A8DC                          BTST      D0,(A2)+
0000A8DE                          AND.B     #$16,(A0)+
0000A8E2                          BTST      D0,(A5)
0000A8E4                          BTST      D0,(A4)
0000A8E6                          MOVEP.W   $0309(A0),D0
0000A8EA                          MOVEP.W   $0107(A0),D0
0000A8EE                          BTST      D0,D6
0000A8F0                          BTST      D1,D4
0000A8F2                          BTST      D1,D1
0000A8F4                          AND.B     #$01,D0
0000A8F8                          BTST      D0,D2
0000A8FA                          BTST      D0,D3
0000A8FC                          AND.B     #$08,D5
0000A900                          AND.B     #$0C,A3
0000A904                          MOVEP.W   $0210(A7),D0
0000A908                          AND.B     #$10,(A1)
0000A90C                          AND.B     #$0E,A7
0000A910                          AND.B     #$0C,A5
0000A914                          MOVEP.W   $030D(A6),D0
0000A918                          MOVEP.W   $010B(A4),D1
0000A91C                          MOVEP.W   $0109(A2),D0
0000A920                          MOVEP.W   $0107(A0),D1
0000A924                          BTST      D0,D6
0000A926                          BTST      D0,D5
0000A928                          BTST      D0,D4
0000A92A                          BTST      D1,D3
0000A92C                          SUB.B     #$05,D4
0000A930                          BTST      D0,D6
0000A932                          AND.B     #$0A,A0
0000A936                          MOVEP.W   $010C(A3),D0
0000A93A                          BTST      D0,(A0)+
0000A93C                          BTST      D1,(A7)
0000A93E                          BTST      D0,(A0)+
0000A940                          BTST      D0,(A1)+
0000A942                          BTST      D0,(A2)+
0000A944                          BTST      D1,(A4)+
0000A946                          BTST      D1,(A7)+
0000A948                          AND.B     #$1F,D0
0000A94C                          BTST      D0,(A6)+
0000A94E                          BTST      D0,(A5)+
0000A950                          AND.B     #$18,(A3)+
0000A954                          AND.B     #$14,(A5)
0000A958                          BTST      D0,(A1)
0000A95A                          AND.B     #$0F,(A0)
0000A95E                          BTST      D1,(A0)
0000A960                          AND.B     #$12,(A1)
0000A964                          AND.B     #$14,(A3)
0000A968                          MOVE.L    -(A0),(A4)+
0000A96A                          MOVE.B    (A2),(A4)+
0000A96C                          Invalid
0000A96E                          EOR       D0
0000A970                          ADD.L     ,D6
0000A972                          ADD.L     ,D0
0000A974                          AND.L     ,D2
0000A976                          CMP.L     D1,A7
0000A978                          CMP.L     D3,A4
0000A97A                          CMP.W     D7,A3
0000A97C                          CMP.L     A3,A0
0000A97E                          CMP.W     (A0),A0
0000A980                          Invalid
0000A982                          Invalid
0000A984                          CMP.W     $BA(A1,A1.W),A0
0000A988                          AND.B     (A2),D4
0000A98A                          ADD.B     -(A0),D4
0000A98C                          LSL.B     D4,D5
0000A98E                          Invalid
0000A990                          MOVE.B    D0,A2
0000A992                          MOVE.L    D3,A3
0000A994                          MOVE.W    D3,A1
0000A996                          MOVE.W    D2,A6
0000A998                          CLR.W     D0
0000A99A                          NBCD      
0000A99C                          Invalid
0000A99E                          Invalid
0000A9A0                          ADDQ.B    #$1,$54(A0,A3.W)
0000A9A4                          ADDQ.B    #$2,(A6)+
0000A9A6                          ADDQ.B    #$0,(A0)
0000A9A8                          MOVEM.L   D2-D3/D5-D7/A3-A5,
0000A9AC                          ADD.W     -(A0),A4
0000A9AE                          AND       (A2)
0000A9B0                          Invalid
0000A9B2                          MOVE.B    D0,$24BD31BD
0000A9B8                          MOVE.W    ,$42(A6,A0.W)
0000A9BC                          EXT.L     D4
0000A9BE                          LEA       D7,A5
0000A9C0                          LEA       A4,A7
0000A9C2                          ððB       Internal check fail
0000A9C4                          ððB       Internal check fail
0000A9C6                          ððB       Internal check fail
0000A9C8                          LEA       $46(A2,D2.W),A7
0000A9CC                          MOVE.W    (A4),D4
0000A9CE                          MOVE.L    -(A0),-(A4)
0000A9D0                          MOVE.B    $043A(A7),D4
0000A9D4                          ASR.W     #7,D1
0000A9D6                          ADD.W     D4,D6
0000A9D8                          ADD.W     D4,D0
0000A9DA                          AND.W     D1,D2
0000A9DC                          AND.W     D0,D0
0000A9DE                          VS        $0000AA1D
0000A9E0                          CS        $0000AA1B
0000A9E2                          CMP.B     $B0(A5,D0.L),D1
0000A9E6                          Invalid
0000A9E8                          Invalid
0000A9EA                          .B        A7,D0
0000A9EC                          CMP.L     ,A5
0000A9EE                          MULU      $0201(A5),D4
0000A9F2                          BTST      D1,D3
0000A9F4                          BTST      D0,D1
0000A9F6                          BTST      D0,D1
0000A9F8                          BTST      D0,D1
0000A9FA                          BTST      D0,D1
0000A9FC                          BTST      D0,D2
0000A9FE                          BTST      D0,D2
0000AA00                          BTST      D0,D2
0000AA02                          BTST      D1,D3
0000AA04                          BTST      D2,D4
0000AA06                          ADD.B     #$02,D3
0000AA0A                          BTST      D0,D1
0000AA0C                          BTST      D0,D1
0000AA0E                          BTST      D0,D1
0000AA10                          BTST      D0,D2
0000AA12                          BTST      D0,D1
0000AA14                          BTST      D0,D1
0000AA16                          BTST      D1,D1
0000AA18                          BTST      D0,D1
0000AA1A                          SUB.B     #$05,D4
0000AA1E                          AND.B     #$01,D1
0000AA22                          BTST      D0,D2
0000AA24                          BTST      D0,D1
0000AA26                          BTST      D0,D2
0000AA28                          BTST      D0,D1
0000AA2A                          AND.B     #$02,D2
0000AA2E                          AND.B     #$02,D1
0000AA32                          BTST      D1,D3
0000AA34                          BTST      D1,D3
0000AA36                          BTST      D3,D2
0000AA38                          AND.B     #$01,D3
0000AA3C                          BTST      D0,D1
0000AA3E                          BTST      D0,D1
0000AA40                          BTST      D0,D1
0000AA42                          BTST      D0,D1
0000AA44                          BTST      D0,D1
0000AA46                          BTST      D0,D1
0000AA48                          BTST      D0,D1
0000AA4A                          BTST      D0,D2
0000AA4C                          BTST      D0,D2
0000AA4E                          AND.B     #$03,D5
0000AA52                          BTST      D2,D2
0000AA54                          BTST      D0,D2
0000AA56                          AND.B     #$05,D2
0000AA5A                          SUB.B     #$01,D4
0000AA5E                          BTST      D0,D1
0000AA60                          BTST      D0,D1
0000AA62                          BTST      D0,D1
0000AA64                          BTST      D0,D2
0000AA66                          BTST      D0,D3
0000AA68                          BTST      D0,D1
0000AA6A                          BTST      D0,D3
0000AA6C                          AND.B     #$04,D5
0000AA70                          BTST      D1,D2
0000AA72                          BTST      D0,D3
0000AA74                          BTST      D0,D2
0000AA76                          BTST      D0,D1
0000AA78                          BTST      D0,D1
0000AA7A                          AND.B     #$01,D1
0000AA7E                          BTST      D0,D1
0000AA80                          AND.B     #$01,D1
0000AA84                          AND.B     #$02,D2
0000AA88                          BTST      D1,D1
0000AA8A                          BTST      D0,D1
0000AA8C                          AND.B     #$05,D2
0000AA90                          SUB.B     #$02,D3
0000AA94                          AND.B     #$01,D1
0000AA98                          AND.B     #$03,D1
0000AA9C                          SUB.B     #$03,D1
0000AAA0                          BTST      D0,D1
0000AAA2                          AND.B     #$01,D2
0000AAA6                          AND.B     #$01,D1
0000AAAA                          BTST      D0,D2
0000AAAC                          AND.B     #$03,D2
0000AAB0                          BTST      D1,D3
0000AAB2                          ADD.B     #$01,D4
0000AAB6                          BTST      D0,D1
0000AAB8                          BTST      D0,D1
0000AABA                          BTST      D0,D2
0000AABC                          AND.B     #$02,D2
0000AAC0                          AND.B     #$01,D1
0000AAC4                          AND.B     #$01,D1
0000AAC8                          AND.B     #$07,D3
0000AACC                          SUB.B     #$01,D4
0000AAD0                          BTST      D0,D1
0000AAD2                          BTST      D0,D1
0000AAD4                          BTST      D0,D1
0000AAD6                          AND.B     #$01,D2
0000AADA                          AND.B     #$01,D1
0000AADE                          AND.B     #$02,D3
0000AAE2                          BTST      D0,D1
0000AAE4                          BTST      D0,D1
0000AAE6                          BTST      D0,D1
0000AAE8                          BTST      D0,D5
0000AAEA                          EOR.B     #$01,D4
0000AAEE                          OR.B      #$14,(A0)+
0000AAF2                          MOVE.B    (A6)+,D1
0000AAF4                          MOVE.B    (A2)+,D6
0000AAF6                          OR.B      #$04,A0
0000AAFA                          AND.B     #$0A,A6
0000AAFE                          OR.B      #$03,D7
0000AB02                          MOVEP.W   $0B09(A5),D0
0000AB06                          OR.B      #$0C,A0
0000AB0A                          .B        #$06,D2
0000AB0E                          OR.B      #$1C,(A0)+
0000AB12                          MOVE.B    (A2),D7
0000AB14                          MOVE.B    (A6),D2
0000AB16                          OR.B      #$8A,A2
0000AB1A                          MOVEP.W   $018A(A2),D0
0000AB1E                          AND.B     #$00,A2
0000AB22                          ADD.B     #$2B,D0
0000AB26                          BSET      D2,-(A1)
0000AB28                          AND.W     #$0589,A2
0000AB2C                          BCHG      D1,(A4)
0000AB2E                          Invalid
0000AB30                          SUB.B     #$3D,
0000AB34                          Invalid
0000AB36                          BCHG      D1,(A4)
0000AB38                          MOVEP.W   D2,$024A(A1)
0000AB3C                          BSET      D2,-(A1)
0000AB3E                          BTST      D0,$0600(A3)
0000AB42                          OR.B      #$E1,D0
0000AB46                          Invalid
0000AB48                          MOVEP.W   D2,-$024A(A1)
0000AB4C                          Invalid
0000AB4E                          Invalid
0000AB50                          SUB.B     #$C3,
0000AB54                          BCHG      D1,(A4)
0000AB56                          Invalid
0000AB58                          AND.W     #$FA77,A2
0000AB5C                          BTST      D0,-$05E1(A3)
0000AB60                          OR.B      #$00,D0
0000AB64                          Invalid
0000AB66                          Invalid
0000AB68                          Invalid
0000AB6A                          Invalid
0000AB6C                          Invalid
0000AB6E                          Invalid
0000AB70                          Invalid
0000AB72                          Invalid
0000AB74                          Invalid
0000AB76                          Invalid
0000AB78                          Invalid
0000AB7A                          Invalid
0000AB7C                          Invalid
0000AB7E                          Invalid
0000AB80                          Invalid
0000AB82                          OR.B      #$1F,D0
0000AB86                          BTST      D0,-$0589(A3)
0000AB8A                          AND.W     #$FB04,A2
0000AB8E                          BCHG      D1,(A4)
0000AB90                          Invalid
0000AB92                          SUB.B     #$AC,
0000AB96                          Invalid
0000AB98                          Invalid
0000AB9A                          MOVEP.W   D2,-$012B(A1)
0000AB9E                          BSET      D2,-(A1)
0000ABA0                          OR.B      #$00,D0
0000ABA4                          MOVEP.W   D0,$07D7(A7)
0000ABA8                          MOVEP.W   $0762(A6),D1
0000ABAC                          SUB.W     #$06A5,$05(A0,A3.W)
0000ABB2                          BCLR      D2,-(A7)
0000ABB4                          ADD.L     #$04700762,-(A5)
0000ABBA                          MOVEP.W   $07D7(A6),D1
0000ABBE                          MOVEP.W   D0,$0800(A7)
0000ABC2                          OR.B      #$D7,D0
0000ABC6                          Invalid
0000ABC8                          BCHG      D3,-(A2)
0000ABCA                          Invalid
0000ABCC                          ADD.L     #$FB9005A7,-(A5)
0000ABD2                          Invalid
0000ABD4                          SUB.W     #$F95B,$03(A0,D7.W)
0000ABDA                          Invalid
0000ABDC                          MOVEP.W   D0,-$07D7(A7)
0000ABE0                          OR.B      #$00,D0
0000ABE4                          Invalid
0000ABE6                          Invalid
0000ABE8                          Invalid
0000ABEA                          Invalid
0000ABEC                          Invalid
0000ABEE                          Invalid
0000ABF0                          Invalid
0000ABF2                          Invalid
0000ABF4                          Invalid
0000ABF6                          Invalid
0000ABF8                          Invalid
0000ABFA                          Invalid
0000ABFC                          Invalid
0000ABFE                          Invalid
0000AC00                          Invalid
0000AC02                          OR.B      #$29,D0
0000AC06                          MOVEP.W   D0,-$0762(A7)
0000AC0A                          MOVEP.W   -$06A5(A6),D1
0000AC0E                          SUB.W     #$FA59,$05(A0,A3.W)
0000AC14                          Invalid
0000AC16                          ADD.L     #$FCF20762,-(A5)
0000AC1C                          Invalid
0000AC1E                          BSET      D3,(A7)
0000AC20                          OR.B      #$00,D0
0000AC24                          BCHG      D0,(A5)+
0000AC26                          Invalid
0000AC28                          AND.L     #$067603E2,$05D1(A4)
0000AC30                          Invalid
0000AC32                          Invalid
0000AC34                          BSET      D2,(A1)
0000AC36                          BSET      D1,-(A2)
0000AC38                          ADD.W     #$02AC,$06(A6,D6.L)
0000AC3E                          BCHG      D0,(A5)+
0000AC40                          BTST      D3,D0
0000AC42                          OR.B      #$DC,D0
0000AC46                          Invalid
0000AC48                          ADD.W     #$FD54,$05(A6,A0.L)
0000AC4E                          Invalid
0000AC50                          Invalid
0000AC52                          Invalid
0000AC54                          BSET      D1,-(A2)
0000AC56                          Invalid
0000AC58                          AND.L     #$F98A015D,-$06DC(A4)
0000AC60                          OR.B      #$00,D0
0000AC64                          Invalid
0000AC66                          Invalid
0000AC68                          Invalid
0000AC6A                          Invalid
0000AC6C                          Invalid
0000AC6E                          Invalid
0000AC70                          Invalid
0000AC72                          Invalid
0000AC74                          Invalid
0000AC76                          Invalid
0000AC78                          Invalid
0000AC7A                          Invalid
0000AC7C                          Invalid
0000AC7E                          Invalid
0000AC80                          Invalid
0000AC82                          OR.B      #$24,D0
0000AC86                          BCHG      D0,(A5)+
0000AC88                          Invalid
0000AC8A                          AND.L     #$FA2F03E2,-$04F2(A4)
0000AC92                          Invalid
0000AC94                          Invalid
0000AC96                          BSET      D2,(A1)
0000AC98                          Invalid
0000AC9A                          ADD.W     #$FEA3,$06(A6,D6.L)
0000ACA0                          OR.B      #$00,D0
0000ACA4                          OR.W      #$01F5,-(A3)
0000ACA8                          Invalid
0000ACAA                          BSET      D0,(A0)+
0000ACAC                          BTST      D0,(A4)+
0000ACAE                          BCLR      D0,$0169(A1)
0000ACB2                          BCHG      D0,$01A9(A1)
0000ACB6                          BTST      D0,(A4)+
0000ACB8                          BSET      D0,(A0)+
0000ACBA                          Invalid
0000ACBC                          BSET      D0,$00(A5,A1.W)
0000ACC0                          AND.B     #$00,D0
0000ACC4                          BSET      D0,$FF(A5,A6.L)
0000ACC8                          BSET      D0,(A0)+
0000ACCA                          Invalid
0000ACCC                          BCLR      D0,-$011C(A1)
0000ACD0                          BCHG      D0,-$0169(A1)
0000ACD4                          BTST      D0,(A4)+
0000ACD6                          Invalid
0000ACD8                          Invalid
0000ACDA                          Invalid
0000ACDC                          OR.W      #$FE0B,-(A3)
0000ACE0                          OR.B      #$00,D0
0000ACE4                          Invalid
0000ACE6                          Invalid
0000ACE8                          Invalid
0000ACEA                          Invalid
0000ACEC                          Invalid
0000ACEE                          Invalid
0000ACF0                          Invalid
0000ACF2                          Invalid
0000ACF4                          Invalid
0000ACF6                          Invalid
0000ACF8                          Invalid
0000ACFA                          Invalid
0000ACFC                          Invalid
0000ACFE                          Invalid
0000AD00                          Invalid
0000AD02                          OR.B      #$0B,D0
0000AD06                          OR.W      #$FE28,-(A3)
0000AD0A                          Invalid
0000AD0C                          Invalid
0000AD0E                          BTST      D0,(A4)+
0000AD10                          Invalid
0000AD12                          BCHG      D0,-$011C(A1)
0000AD16                          BCLR      D0,-$00C3(A1)
0000AD1A                          BSET      D0,(A0)+
0000AD1C                          Invalid
0000AD1E                          BSET      D0,$00(A5,D0.W)
0000AD22                          SUB.B     #$C7,D0
0000AD26                          BSET      D1,$0187(A3)
0000AD2A                          BCLR      D1,$02(A1,D4.L)
0000AD2E                          BCHG      D1,(A2)
0000AD30                          Invalid
0000AD32                          Invalid
0000AD34                          BCHG      D1,(A2)
0000AD36                          AND.B     #$B1,$0187
0000AD3C                          BSET      D1,$00C7(A3)
0000AD40                          SUB.B     #$00,D0
0000AD44                          BSET      D1,-$00C7(A3)
0000AD48                          BCLR      D1,$FE(A1,A4.L)
0000AD4C                          BCHG      D1,(A2)
0000AD4E                          Invalid
0000AD50                          Invalid
0000AD52                          Invalid
0000AD54                          AND.B     #$AE,$0187
0000AD5A                          Invalid
0000AD5C                          Invalid
0000AD5E                          Invalid
0000AD60                          OR.B      #$00,D0
0000AD64                          Invalid
0000AD66                          Invalid
0000AD68                          Invalid
0000AD6A                          Invalid
0000AD6C                          Invalid
0000AD6E                          Invalid
0000AD70                          Invalid
0000AD72                          Invalid
0000AD74                          Invalid
0000AD76                          Invalid
0000AD78                          Invalid
0000AD7A                          Invalid
0000AD7C                          Invalid
0000AD7E                          Invalid
0000AD80                          Invalid
0000AD82                          OR.B      #$15,D0
0000AD86                          Invalid
0000AD88                          Invalid
0000AD8A                          BCLR      D0,D7
0000AD8C                          Invalid
0000AD8E                          AND.B     #$2D,$02D3
0000AD94                          Invalid
0000AD96                          BCHG      D1,(A2)
0000AD98                          Invalid
0000AD9A                          BCLR      D1,$FF(A1,A4.L)
0000AD9E                          BSET      D1,-$25D0(A3)
0000ADA2                          ADD.B     Invalid
0000ADA4                          ADD.B     $DD(A0,D0.L),D6
0000ADA8                          ADD.B     $DF(A0,D0.L),D7
0000ADAC                          ADD.L     Invalid
0000ADAE                          ROXR.B    D0,D0
0000ADB0                          OR.B      #$00,D0
0000ADB4                          OR.W      #$02FC,(A2)+
0000ADB8                          OR.L      #$02F0010B,$02(A3,D7.L)
0000ADC0                          BCHG      D0,-(A0)
0000ADC2                          Invalid
0000ADC4                          BCLR      D0,$02(A2,A2.W)
0000ADC8                          BSET      D0,
0000ADCA                          AND.W     #$0248,
0000ADCE                          AND.W     #$028B,(A1)
0000ADD2                          AND.B     #$C8,(A6)+
0000ADD6                          BSET      D0,-(A6)
0000ADD8                          Invalid
0000ADDA                          BCLR      D0,$032C(A2)
0000ADDE                          BCHG      D0,$0352(A1)
0000ADE2                          BTST      D0,-(A5)
0000ADE4                          BCHG      D1,$00(A1,D7.L)
0000ADE8                          BCLR      D1,D7
0000ADEA                          OR.L      #$0394004B,(A5)
0000ADF0                          BCLR      D1,(A1)+
0000ADF2                          OR.B      #$94,D0
0000ADF6                          Invalid
0000ADF8                          BCLR      D1,D7
0000ADFA                          Invalid
0000ADFC                          BCHG      D1,$FF(A1,D1.W)
0000AE00                          BCHG      D1,(A2)
0000AE02                          Invalid
0000AE04                          BTST      D1,-$0169(A4)
0000AE08                          Invalid
0000AE0A                          Invalid
0000AE0C                          Invalid
0000AE0E                          Invalid
0000AE10                          AND.L     #$FDE20248,A3
0000AE16                          Invalid
0000AE18                          BSET      D0,
0000AE1A                          Invalid
0000AE1C                          BCLR      D0,$FD(A2,A5.L)
0000AE20                          BCHG      D0,-(A0)
0000AE22                          Invalid
0000AE24                          MOVEP.W   -$02DE(A3),D0
0000AE28                          OR.L      #$FD10005A,$FD(A3,D2.W)
0000AE30                          OR.B      #$00,D0
0000AE34                          Invalid
0000AE36                          Invalid
0000AE38                          Invalid
0000AE3A                          Invalid
0000AE3C                          Invalid
0000AE3E                          Invalid
0000AE40                          Invalid
0000AE42                          Invalid
0000AE44                          Invalid
0000AE46                          Invalid
0000AE48                          Invalid
0000AE4A                          Invalid
0000AE4C                          Invalid
0000AE4E                          Invalid
0000AE50                          Invalid
0000AE52                          Invalid
0000AE54                          Invalid
0000AE56                          Invalid
0000AE58                          Invalid
0000AE5A                          Invalid
0000AE5C                          Invalid
0000AE5E                          Invalid
0000AE60                          Invalid
0000AE62                          Invalid
0000AE64                          Invalid
0000AE66                          Invalid
0000AE68                          Invalid
0000AE6A                          Invalid
0000AE6C                          Invalid
0000AE6E                          Invalid
0000AE70                          Invalid
0000AE72                          OR.B      #$6C,D0
0000AE76                          OR.W      #$FC79,A3
0000AE7A                          OR.L      #$FC8F00DE,(A5)
0000AE80                          Invalid
0000AE82                          BTST      D0,-(A5)
0000AE84                          Invalid
0000AE86                          BCHG      D0,-$02FD(A1)
0000AE8A                          BCLR      D0,-$02C8(A2)
0000AE8E                          BSET      D0,-(A6)
0000AE90                          Invalid
0000AE92                          AND.B     #$B8,(A6)+
0000AE96                          AND.W     #$FE01,(A1)
0000AE9A                          AND.W     #$FE4E,
0000AE9E                          AND.L     #$FEA002C4,-(A5)
0000AEA4                          Invalid
0000AEA6                          Invalid
0000AEA8                          Invalid
0000AEAA                          Invalid
0000AEAC                          Invalid
0000AEAE                          Invalid
0000AEB0                          OR.B      #$80,D0
0000AEB4                          OR.W      #$027C,A3
0000AEB8                          OR.L      #$027300DE,(A5)
0000AEBE                          AND.W     #$0125,-(A4)
0000AEC2                          AND.W     #$0169,A6
0000AEC6                          AND.B     #$AA,$02(A4,A1.L)
0000AECC                          BSET      D0,-(A6)
0000AECE                          BSET      D0,$021E(A6)
0000AED2                          BSET      D0,D4
0000AED4                          AND.W     #$0195,(A1)
0000AED8                          AND.W     #$0163,
0000AEDC                          AND.L     #$012D02C4,-(A5)
0000AEE2                          Invalid
0000AEE4                          Invalid
0000AEE6                          OR.L      #$02F0007C,$02FC003E
0000AEF0                          BTST      D1,D0
0000AEF2                          OR.B      #$FC,D0
0000AEF6                          Invalid
0000AEF8                          Invalid
0000AEFA                          Invalid
0000AEFC                          Invalid
0000AEFE                          Invalid
0000AF00                          Invalid
0000AF02                          Invalid
0000AF04                          AND.L     #$FED3027E,-(A5)
0000AF0A                          Invalid
0000AF0C                          AND.W     #$FE6B,(A1)
0000AF10                          AND.B     #$3C,(A6)+
0000AF14                          BSET      D0,-(A6)
0000AF16                          Invalid
0000AF18                          BCLR      D0,-$0213(A2)
0000AF1C                          BCHG      D0,-$0234(A1)
0000AF20                          BTST      D0,-(A5)
0000AF22                          Invalid
0000AF24                          Invalid
0000AF26                          Invalid
0000AF28                          OR.L      #$FD8D004B,(A5)
0000AF2E                          Invalid
0000AF30                          OR.B      #$80,D0
0000AF34                          Invalid
0000AF36                          Invalid
0000AF38                          Invalid
0000AF3A                          Invalid
0000AF3C                          Invalid
0000AF3E                          Invalid
0000AF40                          Invalid
0000AF42                          Invalid
0000AF44                          Invalid
0000AF46                          Invalid
0000AF48                          Invalid
0000AF4A                          Invalid
0000AF4C                          Invalid
0000AF4E                          Invalid
0000AF50                          Invalid
0000AF52                          Invalid
0000AF54                          Invalid
0000AF56                          Invalid
0000AF58                          Invalid
0000AF5A                          Invalid
0000AF5C                          Invalid
0000AF5E                          Invalid
0000AF60                          Invalid
0000AF62                          Invalid
0000AF64                          Invalid
0000AF66                          Invalid
0000AF68                          Invalid
0000AF6A                          Invalid
0000AF6C                          Invalid
0000AF6E                          Invalid
0000AF70                          Invalid
0000AF72                          OR.B      #$04,D0
0000AF76                          OR.B      #$10,
0000AF7A                          OR.W      #$FD22,SR
0000AF7E                          OR.L      #$FD3C00F4,$FD5B012D
0000AF88                          Invalid
0000AF8A                          BCHG      D0,-(A3)
0000AF8C                          Invalid
0000AF8E                          BCLR      D0,(A5)
0000AF90                          Invalid
0000AF92                          BSET      D0,D4
0000AF94                          Invalid
0000AF96                          BSET      D0,-$01AA(A6)
0000AF9A                          AND.B     #$97,(A3)
0000AF9E                          AND.B     #$DB,$02(A4,D7.W)
0000AFA4                          Invalid
0000AFA6                          AND.W     #$FF6B,-(A4)
0000AFAA                          AND.W     #$FFB5,$02(A3,D6.L)
0000AFB0                          OR.B      #$00,D0
0000AFB4                          OR.B      #$FD,CCR
0000AFB8                          OR.W      #$01F5,$00(A7,D1.L)
0000AFBE                          BSET      D0,$00EA(A1)
0000AFC2                          BSET      D0,(A0)+
0000AFC4                          BTST      D0,-(A1)
0000AFC6                          BSET      D0,D3
0000AFC8                          BCHG      D0,(A4)
0000AFCA                          BCLR      D0,$0185(A1)
0000AFCE                          MOVEP.W   D0,$01B2(A3)
0000AFD2                          BCHG      D0,$01DA(A1)
0000AFD6                          BCHG      D0,D4
0000AFD8                          BSET      D0,
0000AFDA                          BTST      D0,(A4)+
0000AFDC                          AND.B     #$F1,(A5)+
0000AFE0                          AND.B     #$C3,$02(A7,A5.W)
0000AFE6                          OR.L      #$025A0063,(A4)
0000AFEC                          AND.W     #$0032,-(A3)
0000AFF0                          AND.W     #$0000,-(A6)
0000AFF4                          AND.W     #$FFCE,-(A3)
0000AFF8                          AND.W     #$FF9D,(A2)+
0000AFFC                          AND.W     #$FF6C,A3
0000B000                          AND.B     #$3D,$02(A7,A6.L)
0000B006                          Invalid
0000B008                          BSET      D0,
0000B00A                          Invalid
0000B00C                          BSET      D0,(A2)+
0000B00E                          Invalid
0000B010                          BCLR      D0,$FE(A2,A3.L)
0000B014                          BCLR      D0,D5
0000B016                          Invalid
0000B018                          BCHG      D0,(A4)
0000B01A                          Invalid
0000B01C                          BTST      D0,-(A1)
0000B01E                          Invalid
0000B020                          Invalid
0000B022                          Invalid
0000B024                          OR.L      #$FE170077,$FE(A2,A5.W)
0000B02C                          OR.B      #$03,CCR
0000B030                          OR.B      #$00,D0
0000B034                          Invalid
0000B036                          Invalid
0000B038                          Invalid
0000B03A                          Invalid
0000B03C                          Invalid
0000B03E                          Invalid
0000B040                          Invalid
0000B042                          Invalid
0000B044                          Invalid
0000B046                          Invalid
0000B048                          Invalid
0000B04A                          Invalid
0000B04C                          Invalid
0000B04E                          Invalid
0000B050                          Invalid
0000B052                          Invalid
0000B054                          Invalid
0000B056                          Invalid
0000B058                          Invalid
0000B05A                          Invalid
0000B05C                          Invalid
0000B05E                          Invalid
0000B060                          Invalid
0000B062                          Invalid
0000B064                          Invalid
0000B066                          Invalid
0000B068                          Invalid
0000B06A                          Invalid
0000B06C                          Invalid
0000B06E                          Invalid
0000B070                          Invalid
0000B072                          OR.B      #$9D,D0
0000B076                          OR.B      #$A6,$00(A2,A1.W)
0000B07C                          Invalid
0000B07E                          OR.L      #$FDC900C3,(A4)
0000B084                          Invalid
0000B086                          Invalid
0000B088                          Invalid
0000B08A                          BTST      D0,(A4)+
0000B08C                          Invalid
0000B08E                          BCHG      D0,D4
0000B090                          Invalid
0000B092                          BCHG      D0,-$0185(A1)
0000B096                          MOVEP.W   D0,-$0154(A3)
0000B09A                          BCLR      D0,-$0121(A1)
0000B09E                          BSET      D0,D3
0000B0A0                          Invalid
0000B0A2                          BSET      D0,(A0)+
0000B0A4                          Invalid
0000B0A6                          BSET      D0,-$0077(A1)
0000B0AA                          BSET      D0,$FF(A5,D2.W)
0000B0AE                          BSET      D0,
0000B0B0                          OR.B      #$80,D0
0000B0B4                          OR.B      #$7E,$0059(A5)
0000B0BA                          BCHG      D0,$0085
0000B0BE                          BCHG      D0,$00B0(A7)
0000B0C2                          BCHG      D0,-(A2)
0000B0C4                          Invalid
0000B0C6                          BCHG      D0,(A2)
0000B0C8                          Invalid
0000B0CA                          BTST      D0,
0000B0CC                          BTST      D0,-(A4)
0000B0CE                          BTST      D0,$0145(A0)
0000B0D2                          MOVEP.W   $0164(A7),D0
0000B0D6                          Invalid
0000B0D8                          BCHG      D0,
0000B0DA                          Invalid
0000B0DC                          BCLR      D0,(A6)
0000B0DE                          OR.L      #$01A90092,$01(A4,D4.L)
0000B0E6                          OR.W      #$01C3,$004A(A7)
0000B0EC                          MOVEP.L   D0,$0025(A2)
0000B0F0                          MOVEP.L   D0,$0000(A4)
0000B0F4                          MOVEP.L   D0,-$0025(A2)
0000B0F8                          BSET      D0,D3
0000B0FA                          Invalid
0000B0FC                          BCLR      D0,$FF91
0000B100                          BCLR      D0,-$0092(A1)
0000B104                          BCLR      D0,(A6)
0000B106                          Invalid
0000B108                          BCHG      D0,
0000B10A                          Invalid
0000B10C                          BCHG      D0,-(A4)
0000B10E                          Invalid
0000B110                          BCHG      D0,D5
0000B112                          Invalid
0000B114                          BTST      D0,-(A4)
0000B116                          Invalid
0000B118                          Invalid
0000B11A                          Invalid
0000B11C                          Invalid
0000B11E                          Invalid
0000B120                          OR.L      #$FE9E0085,$FE(A0,A0.L)
0000B128                          OR.W      #$FE88,(A1)+
0000B12C                          OR.B      #$82,$0000(A5)
0000B132                          Invalid
0000B134                          Invalid
0000B136                          Invalid
0000B138                          Invalid
0000B13A                          Invalid
0000B13C                          Invalid
0000B13E                          Invalid
0000B140                          Invalid
0000B142                          Invalid
0000B144                          Invalid
0000B146                          Invalid
0000B148                          Invalid
0000B14A                          Invalid
0000B14C                          Invalid
0000B14E                          Invalid
0000B150                          Invalid
0000B152                          Invalid
0000B154                          Invalid
0000B156                          Invalid
0000B158                          Invalid
0000B15A                          Invalid
0000B15C                          Invalid
0000B15E                          Invalid
0000B160                          Invalid
0000B162                          Invalid
0000B164                          Invalid
0000B166                          Invalid
0000B168                          Invalid
0000B16A                          Invalid
0000B16C                          Invalid
0000B16E                          Invalid
0000B170                          Invalid
0000B172                          OR.B      #$36,D0
0000B176                          OR.B      #$3D,-(A5)
0000B17A                          OR.W      #$FE48,A2
0000B17E                          OR.W      #$FE57,$0092(A7)
0000B184                          Invalid
0000B186                          OR.L      #$FE8200D5,$FE(A4,D6.L)
0000B18E                          Invalid
0000B190                          Invalid
0000B192                          MOVEP.W   -$0124(A7),D0
0000B196                          BTST      D0,-$00FF(A0)
0000B19A                          BTST      D0,
0000B19C                          Invalid
0000B19E                          BCHG      D0,(A2)
0000B1A0                          Invalid
0000B1A2                          BCHG      D0,-(A2)
0000B1A4                          Invalid
0000B1A6                          BCHG      D0,-$0059(A7)
0000B1AA                          BCHG      D0,$FFD3
0000B1AE                          BCHG      D0,
0000B1B0                          OR.B      #$00,D0
0000B1B4                          OR.B      #$FE,(A6)+
0000B1B8                          OR.B      #$FA,+$00(A4.L)
0000B1BE                          Invalid
0000B1C0                          OR.W      #$00EC,$00(A5,D0.L)
0000B1C6                          Invalid
0000B1C8                          OR.L      #$00D400C2,$00C5(A2)
0000B1D0                          Invalid
0000B1D2                          OR.L      #$00ED00A2,$00(A4,A7.L)
0000B1DA                          OR.L      #$010E0078,A6
0000B1E0                          BTST      D0,(A3)+
0000B1E2                          OR.W      #$0125,-(A1)
0000B1E6                          OR.W      #$012D,A2
0000B1EA                          OR.B      #$31,$00(A1,A4.L)
0000B1F0                          BTST      D0,$00(A3,D0.W)
0000B1F4                          BTST      D0,$FF(A1,A3.W)
0000B1F8                          BTST      D0,-$0031(A5)
0000B1FC                          BTST      D0,-(A5)
0000B1FE                          Invalid
0000B200                          BTST      D0,(A3)+
0000B202                          Invalid
0000B204                          MOVEP.W   -$0078(A6),D0
0000B208                          Invalid
0000B20A                          Invalid
0000B20C                          Invalid
0000B20E                          Invalid
0000B210                          Invalid
0000B212                          Invalid
0000B214                          Invalid
0000B216                          Invalid
0000B218                          OR.L      #$FF2C0090,-$00E1(A2)
0000B220                          OR.W      #$FF14,$00(A5,A4.L)
0000B226                          Invalid
0000B228                          OR.B      #$06,+$00(D7.L)
0000B22E                          Invalid
0000B230                          OR.B      #$00,D0
0000B234                          Invalid
0000B236                          Invalid
0000B238                          Invalid
0000B23A                          Invalid
0000B23C                          Invalid
0000B23E                          Invalid
0000B240                          Invalid
0000B242                          Invalid
0000B244                          Invalid
0000B246                          Invalid
0000B248                          Invalid
0000B24A                          Invalid
0000B24C                          Invalid
0000B24E                          Invalid
0000B250                          Invalid
0000B252                          Invalid
0000B254                          Invalid
0000B256                          Invalid
0000B258                          Invalid
0000B25A                          Invalid
0000B25C                          Invalid
0000B25E                          Invalid
0000B260                          Invalid
0000B262                          Invalid
0000B264                          Invalid
0000B266                          Invalid
0000B268                          Invalid
0000B26A                          Invalid
0000B26C                          Invalid
0000B26E                          Invalid
0000B270                          Invalid
0000B272                          OR.B      #$CF,D0
0000B276                          OR.B      #$D3,(A1)+
0000B27A                          OR.B      #$DB,$00(A1,D5.W)
0000B280                          Invalid
0000B282                          OR.W      #$FEF2,-(A1)
0000B286                          OR.W      #$FF01,$008E
0000B28C                          Invalid
0000B28E                          OR.L      #$FF2700B4,-(A2)
0000B294                          Invalid
0000B296                          Invalid
0000B298                          Invalid
0000B29A                          Invalid
0000B29C                          Invalid
0000B29E                          Invalid
0000B2A0                          Invalid
0000B2A2                          Invalid
0000B2A4                          Invalid
0000B2A6                          Invalid
0000B2A8                          Invalid
0000B2AA                          Invalid
0000B2AC                          Invalid
0000B2AE                          Invalid
0000B2B0                          OR.B      #$80,D0
0000B2B4                          OR.B      #$7F,A7
0000B2B8                          OR.B      #$7D,(A5)+
0000B2BC                          OR.B      #$7A,$003A(A4)
0000B2C2                          OR.W      #$0048,$00(A6,D0.L)
0000B2C8                          OR.W      #$006A,(A5)
0000B2CC                          OR.W      #$0062,-(A1)
0000B2D0                          OR.W      #$005A,$0076(A4)
0000B2D6                          OR.W      #$007F,(A1)
0000B2DA                          OR.W      #$0087,D7
0000B2DE                          OR.B      #$8D,CCR
0000B2E2                          OR.B      #$92,$00(A0,A2.W)
0000B2E8                          OR.L      #$00180098,(A6)
0000B2EE                          OR.B      #$99,A4
0000B2F2                          OR.B      #$98,D0
0000B2F6                          Invalid
0000B2F8                          OR.L      #$FFE80092,(A6)
0000B2FE                          Invalid
0000B300                          OR.L      #$FFD00087,A5
0000B306                          Invalid
0000B308                          OR.W      #$FFB9,
0000B30C                          OR.W      #$FFAF,$00(A6,D6.W)
0000B312                          Invalid
0000B314                          OR.W      #$FF9E,-(A1)
0000B318                          OR.W      #$FF96,(A5)
0000B31C                          OR.W      #$FF90,A0
0000B320                          OR.B      #$8A,$0000B34E(pc)
0000B326                          Invalid
0000B328                          OR.B      #$83,(A5)+
0000B32C                          OR.B      #$81,A7
0000B330                          OR.B      #$80,D0
0000B334                          Invalid
0000B336                          Invalid
0000B338                          Invalid
0000B33A                          Invalid
0000B33C                          Invalid
0000B33E                          Invalid
0000B340                          Invalid
0000B342                          Invalid
0000B344                          Invalid
0000B346                          Invalid
0000B348                          Invalid
0000B34A                          Invalid
0000B34C                          Invalid
0000B34E                          Invalid
0000B350                          Invalid
0000B352                          Invalid
0000B354                          Invalid
0000B356                          Invalid
0000B358                          Invalid
0000B35A                          Invalid
0000B35C                          Invalid
0000B35E                          Invalid
0000B360                          Invalid
0000B362                          Invalid
0000B364                          Invalid
0000B366                          Invalid
0000B368                          Invalid
0000B36A                          Invalid
0000B36C                          Invalid
0000B36E                          Invalid
0000B370                          Invalid
0000B372                          OR.B      #$68,D0
0000B376                          OR.B      #$6A,A4
0000B37A                          OR.B      #$6E,(A0)+
0000B37E                          OR.B      #$73,-(A5)
0000B382                          OR.B      #$79,$00(A0,D6.L)
0000B388                          Invalid
0000B38A                          OR.W      #$FF8A,D7
0000B38E                          OR.W      #$FF94,(A1)
0000B392                          OR.W      #$FF9F,(A2)+
0000B396                          OR.W      #$FFAB,-(A2)
0000B39A                          OR.W      #$FFB8,$0070(A2)
0000B3A0                          Invalid
0000B3A2                          OR.W      #$FFD4,$00(A6,D5.L)
0000B3A8                          Invalid
0000B3AA                          OR.W      #$FFF1,
0000B3AE                          OR.W      #$0000,
0000B3B2                          BTST      D2,D0
0000B3B4                          OR.L      #$04F9012B,(A6)
0000B3BA                          Invalid
0000B3BC                          BCLR      D0,
0000B3BE                          Invalid
0000B3C0                          AND.W     #$049D,A2
0000B3C4                          Invalid
0000B3C6                          SUB.W     #$0354,$0427(A0)
0000B3CC                          MOVEP.L   D1,$03DD(A5)
0000B3D0                          SUB.B     #$88,
0000B3D4                          SUB.L     #$032B04FC,-(A3)
0000B3DA                          Invalid
0000B3DC                          MOVEP.L   $025A(A2),D2
0000B3E0                          MOVEP.W   D2,$01E8(A1)
0000B3E4                          BCLR      D2,
0000B3E6                          BCHG      D0,$05(A3,A0.W)
0000B3EA                          Invalid
0000B3EC                          BSET      D2,$007D
0000B3F0                          ADD.B     #$00,D0
0000B3F4                          BSET      D2,$FF83
0000B3F8                          BSET      D2,-(A1)
0000B3FA                          Invalid
0000B3FC                          BCLR      D2,
0000B3FE                          Invalid
0000B400                          MOVEP.W   D2,-$01E8(A1)
0000B404                          MOVEP.L   -$025A(A2),D2
0000B408                          Invalid
0000B40A                          Invalid
0000B40C                          SUB.L     #$FCD5043D,-(A3)
0000B412                          Invalid
0000B414                          MOVEP.L   D1,-$03DD(A5)
0000B418                          BCHG      D1,(A4)
0000B41A                          Invalid
0000B41C                          Invalid
0000B41E                          Invalid
0000B420                          AND.W     #$FB63,A2
0000B424                          BCLR      D0,
0000B426                          Invalid
0000B428                          BTST      D0,-$04E6(A3)
0000B42C                          OR.L      #$FB070000,(A6)
0000B432                          Invalid
0000B434                          Invalid
0000B436                          Invalid
0000B438                          Invalid
0000B43A                          Invalid
0000B43C                          Invalid
0000B43E                          Invalid
0000B440                          Invalid
0000B442                          Invalid
0000B444                          Invalid
0000B446                          Invalid
0000B448                          Invalid
0000B44A                          Invalid
0000B44C                          Invalid
0000B44E                          Invalid
0000B450                          Invalid
0000B452                          Invalid
0000B454                          Invalid
0000B456                          Invalid
0000B458                          Invalid
0000B45A                          Invalid
0000B45C                          Invalid
0000B45E                          Invalid
0000B460                          Invalid
0000B462                          Invalid
0000B464                          Invalid
0000B466                          Invalid
0000B468                          Invalid
0000B46A                          Invalid
0000B46C                          Invalid
0000B46E                          Invalid
0000B470                          Invalid
0000B472                          OR.B      #$08,D0
0000B476                          OR.W      #$FA1F,
0000B47A                          Invalid
0000B47C                          Invalid
0000B47E                          BCHG      D0,$FA(A3,A3.L)
0000B482                          BSET      D0,-$054A(A0)
0000B486                          AND.W     #$FB04,(A2)+
0000B48A                          Invalid
0000B48C                          Invalid
0000B48E                          BTST      D1,-$043D(A3)
0000B492                          MOVEP.W   D1,-$03CD(A0)
0000B496                          BSET      D1,(A5)+
0000B498                          Invalid
0000B49A                          SUB.B     #$2D,-(A7)
0000B49E                          SUB.W     #$FDB6,$049D(A0)
0000B4A4                          Invalid
0000B4A6                          Invalid
0000B4A8                          Invalid
0000B4AA                          Invalid
0000B4AC                          Invalid
0000B4AE                          Invalid
0000B4B0                          OR.B      #$0C,D0
0000B4B4                          BSET      D4,-(A2)
0000B4B6                          BSET      D4,$09EC(A4)
0000B4BA                          BSET      D4,$09F6(A4)
0000B4BE                          EOR.B     #$00,D0
0000B4C2                          EOR.B     #$0A,D0
0000B4C6                          EOR.B     #$14,(A4)
0000B4CA                          EOR.B     #$1E,(A4)
0000B4CE                          EOR.B     #$28,$0A28(A0)
0000B4D4                          EOR.B     #$32,$0A(A2,D4.W)
0000B4DA                          EOR.W     #$0A48,A0
0000B4DE                          EOR.W     #$0A5E,(A6)+
0000B4E2                          EOR.W     #$0A74,(A6)+
0000B4E6                          EOR.L     #$0AA00AA0,A2
0000B4EC                          EOR.L     #$0AB60AB6,$0A(A6,D6.W)
0000B4F4                          Invalid
0000B4F6                          Invalid
0000B4F8                          OR.W      #$0004,D3
0000B4FC                          OR.B      #$40,D0
0000B500                          BTST      D5,$00(A2,A1.W)
0000B504                          OR.B      #$00,D4
0000B508                          OR.W      #$0B3A,D0
0000B50C                          OR.W      #$0004,D3
0000B510                          OR.B      #$00,D0
0000B514                          BCHG      D5,D2
0000B516                          OR.W      #$0004,D3
0000B51A                          OR.B      #$00,D0
0000B51E                          BTST      D5,$0000B563(pc)
0000B522                          OR.B      #$00,D4
0000B526                          OR.B      #$32,D0
0000B52A                          OR.W      #$0004,D3
0000B52E                          OR.B      #$20,D0
0000B532                          BTST      D5,$0000B577(pc)
0000B536                          OR.B      #$00,D4
0000B53A                          OR.B      #$42,-(A0)
0000B53E                          OR.W      #$0004,D3
0000B542                          OR.B      #$60,D0
0000B546                          BTST      D5,$0000B58B(pc)
0000B54A                          OR.B      #$06,D5
0000B54E                          OR.B      #$EE,D0
0000B552                          OR.W      #$0005,D3
0000B556                          OR.L      #$00200AEE,D6
0000B55C                          EOR.B     #$43,$00(A2,A3.W)
0000B562                          OR.B      #$00,D6
0000B566                          BTST      D5,D2
0000B568                          OR.W      #$0007,D3
0000B56C                          OR.L      #$00000B0E,D6
0000B572                          EOR.W     #$0043,A0
0000B576                          OR.B      #$06,D6
0000B57A                          OR.B      #$02,-(A0)
0000B57E                          OR.W      #$0005,D3
0000B582                          OR.L      #$00200B0E,D6
0000B588                          EOR.W     #$0043,(A6)+
0000B58C                          OR.B      #$06,D5
0000B590                          OR.W      #$0B26,D0
0000B594                          OR.W      #$0005,D3
0000B598                          OR.L      #$00000B26,D6
0000B59E                          EOR.W     #$0043,$00(A4,A2.W)
0000B5A4                          OR.B      #$60,D6
0000B5A8                          BTST      D5,-(A6)
0000B5AA                          OR.W      #$0005,D3
0000B5AE                          OR.L      #$00200B26,D6
0000B5B4                          EOR.L     #$00430005,A2
0000B5BA                          OR.B      #$40,D6
0000B5BE                          Invalid
0000B5C0                          OR.W      #$0005,D3
0000B5C4                          OR.L      #$00600AEE,D6
0000B5CA                          EOR.L     #$00430007,-(A0)
0000B5D0                          OR.B      #$40,D6
0000B5D4                          BTST      D5,D2
0000B5D6                          OR.W      #$0007,D3
0000B5DA                          OR.L      #$00400B0E,D6
0000B5E0                          EOR.L     #$00430006,$00(A6,D3.W)
0000B5E8                          OR.W      #$0B02,-(A0)
0000B5EC                          OR.W      #$0005,D3
0000B5F0                          OR.L      #$00600B0E,D6
0000B5F6                          Invalid
0000B5F8                          BTST      D0,D0
0000B5FA                          Invalid
0000B5FC                          OR.B      #$F4,D0
0000B600                          ADD.B     #$00,A0
0000B604                          BTST      D0,D1
0000B606                          Invalid
0000B608                          OR.B      #$08,D0
0000B60C                          SUB.B     #$F5,D0
0000B610                          OR.B      #$0A,D2
0000B614                          OR.B      #$00,D0
0000B618                          BTST      D0,D3
0000B61A                          Invalid
0000B61C                          OR.B      #$08,D0
0000B620                          BTST      #$F8,D0
0000B624                          BTST      D0,D4
0000B626                          Invalid
0000B628                          OR.B      #$F8,D0
0000B62C                          .B        #$F0,D0
0000B630                          BTST      D0,D5
0000B632                          Invalid
0000B634                          OR.B      #$00,D0
0000B638                          BTST      D4,(A0)
0000B63A                          Invalid
0000B63C                          BTST      D0,D6
0000B63E                          Invalid
0000B640                          OR.B      #$08,D0
0000B644                          SUB.B     #$F8,D0
0000B648                          OR.B      #$0B,D7
0000B64C                          OR.B      #$00,D0
0000B650                          OR.B      #$0A,A0
0000B654                          OR.B      #$00,D0
0000B658                          MOVEP.W   -$0FF3(A1),D0
0000B65C                          OR.B      #$00,D0
0000B660                          BTST      D6,(A0)
0000B662                          Invalid
0000B664                          MOVE.L    $001A(A0),A6
0000B668                          MOVEQ     #$00,D0
0000B66A                          MOVE.B    $0002(A0),D0
0000B66E                          MOVE.W    +$00(D3.W),D0
0000B672                          JMP       +$00(D1.W)
0000B676                          OR.B      #$6C,A0
0000B67A                          MOVE.B    $11B4(A4),$4A(A0,D4.W)
0000B680                          OR.B      #$08,D6
0000B684                          JSR       $0002897E
0000B68A                          BEQ       $0000B6E0
0000B68C                          LEA       $0000CC46(pc),A6
0000B690                          MOVE.L    A6,$001A(A0)
0000B694                          MOVE.W    #$0084,$0028(A0)
0000B69A                          JSR       $0000C870(pc)
0000B69E                          MOVE.W    #$0200,$0002(A0)
0000B6A4                          TST.B     $0006(A0)
0000B6A8                          BNE       $0000B6C4
0000B6AA                          MOVE.B    $A030,$0030(A0)
0000B6B0                          MOVE.B    $FC26,$003B(A0)
0000B6B6                          MOVE.B    $A008,$0032(A0)
0000B6BC                          MOVE.B    $A00C,$0034(A0)
0000B6C2                          BRA       $0000B6CA
0000B6C4                          MOVE.B    #$08,$0003(A0)
0000B6CA                          LEA       $0000CCD0(pc),A4
0000B6CE                          TST.B     $0030(A0)
0000B6D2                          BEQ       $0000B6D8
0000B6D4                          LEA       $0000CCCE(pc),A4
0000B6D8                          MOVEQ     #$00,D0
0000B6DA                          JMP       $000287D6
0000B6E0                          RTS
0000B6E2                          CMP.B     #$02,$A002
0000B6E8                          BHI       $0000B6F8
0000B6EA                          TST.B     $0006(A0)
0000B6EE                          BNE       $0000B6F8
0000B6F0                          SUBQ.W    #$1,$FC50
0000B6F4                          BEQ       $0000B734(pc)
0000B6F8                          MOVEQ     #$00,D0
0000B6FA                          MOVE.B    $0003(A0),D0
0000B6FE                          MOVE.W    +$00(D1.W),D0
0000B702                          JSR       +$00(D7.L)
0000B706                          CMP.B     #$0A,$0003(A0)
0000B70C                          BEQ       $0000B732
0000B70E                          MOVE.W    $A008,$0032(A0)
0000B714                          MOVE.W    $A00C,$0034(A0)
0000B71A                          MOVE.W    $0008(A0),$003C(A0)
0000B720                          MOVE.W    $000C(A0),$003E(A0)
0000B726                          JSR       $0002889E
0000B72C                          JMP       $00028C76
0000B732                          RTS
0000B734                          MOVE.B    #$01,$F0D8
0000B73A                          MOVE.W    #$0600,$0002(A0)
0000B740                          RTS
0000B742                          OR.B      #$A2,A6
0000B746                          EOR.B     #$80,$0C9E(A4)
0000B74C                          BCLR      D6,$0F84(A2)
0000B750                          MOVEQ     #$00,D0
0000B752                          MOVE.B    $0004(A0),D0
0000B756                          MOVE.W    +$00(D3.W),D0
0000B75A                          JMP       +$00(D1.W)
0000B75E                          OR.B      #$3A,D4
0000B762                          MOVE.B    #$02,$0004(A0)
0000B768                          MOVE.W    #$0100,$002C(A0)
0000B76E                          TST.B     $0030(A0)
0000B772                          BNE       $0000B786
0000B774                          MOVE.W    #$0001,$002A(A0)
0000B77A                          MOVEQ     #$00,D0
0000B77C                          LEA       $0000CCFA(pc),A4
0000B780                          JMP       $000287D6
0000B786                          MOVE.W    #$FFFF,$002A(A0)
0000B78C                          MOVEQ     #$00,D0
0000B78E                          LEA       $0000CCFC(pc),A4
0000B792                          JMP       $000287D6
0000B798                          JSR       $00001AAE
0000B79E                          BCS       $0000B7B4(pc)
0000B7A2                          MOVE.W    $002A(A0),D0
0000B7A6                          ADD.W     Invalid
0000B7A8                          OR.B      #$00,A0
0000B7AC                          MOVE.B    ,$4EF90002
0000B7B2                          OR.B      D4,D4
0000B7B4                          ADD.W     Invalid
0000B7B6                          OR.B      #$7C,A4
0000B7BA                          OR.B      #$03,D2
0000B7BE                          MOVE.W    #$0000,$0004(A0)
0000B7C4                          BSR       $0000C870(pc)
0000B7C8                          MOVE.B    $A030,$0030(A0)
0000B7CE                          MOVEQ     #$00,D0
0000B7D0                          LEA       $0000CCD0(pc),A4
0000B7D4                          TST.B     $0030(A0)
0000B7D8                          BEQ       $0000B7DE
0000B7DA                          LEA       $0000CCCE(pc),A4
0000B7DE                          JMP       $000287D6
0000B7E4                          MOVEQ     #$00,D0
0000B7E6                          MOVE.B    $0004(A0),D0
0000B7EA                          MOVE.W    +$00(D3.W),D0
0000B7EE                          JMP       +$00(D1.W)
0000B7F2                          OR.B      #$3E,A6
0000B7F6                          AND.W     #$03FA,$05(A4,D7.L)
0000B7FC                          ADD.L     #$08187000,$10(A4,D4.W)
0000B804                          OR.B      #$3B,D5
0000B808                          OR.B      #$BB,A2
0000B80C                          OR.B      #$00,D6
0000B810                          MOVE.B    A4,D2
0000B812                          OR.B      #$50,D6
0000B816                          OR.L      #$117C0002,-(A4)
0000B81C                          OR.B      #$7C,D5
0000B820                          OR.B      #$3A,D1
0000B824                          MOVEQ     #$00,D0
0000B826                          LEA       $0000CC46(pc),A1
0000B82A                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000B830                          BSR       $0000C870(pc)
0000B834                          MOVE.B    #$04,$0016(A0)
0000B83A                          LEA       $0000CCD0(pc),A4
0000B83E                          TST.B     $0030(A0)
0000B842                          BEQ       $0000B84E
0000B844                          MOVE.B    #$0C,$0016(A0)
0000B84A                          LEA       $0000CCCE(pc),A4
0000B84E                          MOVEQ     #$00,D0
0000B850                          JSR       $000287D6
0000B856                          JSR       $00028842
0000B85C                          JMP       $00028804
0000B862                          BSR       $0000C862(pc)
0000B866                          MOVE.W    $002E(A0),D0
0000B86A                          SUB.W     $0008(A0),D0
0000B86E                          ADDQ.W    #$0,D0
0000B870                          CMP.W     #$0010,D0
0000B874                          BHI       $0000B88A(pc)
0000B878                          MOVE.B    #$04,$0005(A0)
0000B87E                          MOVEQ     #$02,D0
0000B880                          LEA       $0000CC46(pc),A1
0000B884                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000B88A                          MOVE.W    $002E(A0),D0
0000B88E                          TST.B     $0030(A0)
0000B892                          BNE       $0000B8A4
0000B894                          CMP.W     $0008(A0),D0
0000B898                          BCS       $0000B8B4
0000B89A                          BSR       $0000C8A8(pc)
0000B89E                          JMP       $00028804
0000B8A4                          CMP.W     $0008(A0),D0
0000B8A8                          BCC       $0000B8B4
0000B8AA                          BSR       $0000C8A8(pc)
0000B8AE                          JMP       $00028804
0000B8B4                          RTS
0000B8B6                          BSR       $0000C862(pc)
0000B8BA                          TST.B     $004A(A0)
0000B8BE                          BNE       $0000B8EA
0000B8C0                          MOVE.B    $0030(A0),D0
0000B8C4                          CMP.B     $A030,D0
0000B8C8                          BEQ       $0000B8DA
0000B8CA                          MOVE.B    #$00,$0005(A0)
0000B8D0                          MOVE.B    $A030,$0030(A0)
0000B8D6                          BRA       $0000B926(pc)
0000B8DA                          MOVE.W    $002E(A0),D0
0000B8DE                          SUB.W     $0008(A0),D0
0000B8E2                          ADDQ.W    #$0,D0
0000B8E4                          CMP.W     #$0010,D0
0000B8E8                          BLS       $0000B926
0000B8EA                          MOVE.W    $002E(A0),D0
0000B8EE                          TST.B     $0030(A0)
0000B8F2                          BNE       $0000B8FE
0000B8F4                          CMP.W     $0008(A0),D0
0000B8F8                          BCS       $0000B908
0000B8FA                          BRA       $0000B912(pc)
0000B8FE                          CMP.W     $0008(A0),D0
0000B902                          BCC       $0000B908
0000B904                          BRA       $0000B912(pc)
0000B908                          MOVE.B    #$00,$004A(A0)
0000B90E                          BRA       $0000CBBE(pc)
0000B912                          MOVEQ     #$00,D0
0000B914                          LEA       $0000CC46(pc),A1
0000B918                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000B91E                          MOVE.B    #$02,$0005(A0)
0000B924                          RTS
0000B926                          BSR       $0000C8A8(pc)
0000B92A                          JMP       $00028804
0000B930                          MOVEQ     #$00,D0
0000B932                          MOVE.B    $0005(A0),D0
0000B936                          MOVE.W    +$00(D5.W),D0
0000B93A                          JSR       +$00(D3.W)
0000B93E                          BRA       $0000CC1C(pc)
0000B942                          OR.B      #$50,D6
0000B946                          OR.L      #$117C0002,-(A4)
0000B94C                          OR.B      #$00,D5
0000B950                          LEA       $0000CC46(pc),A1
0000B954                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000B95A                          BSR       $0000C870(pc)
0000B95E                          MOVEQ     #$00,D0
0000B960                          TST.B     $0030(A0)
0000B964                          BEQ       $0000B97C
0000B966                          MOVE.B    #$0D,$0016(A0)
0000B96C                          LEA       $0000CCD4(pc),A4
0000B970                          JSR       $000287D6
0000B976                          JMP       $00028804
0000B97C                          MOVE.B    #$03,$0016(A0)
0000B982                          LEA       $0000CCD2(pc),A4
0000B986                          JSR       $000287D6
0000B98C                          JMP       $00028804
0000B992                          BSR       $0000C862(pc)
0000B996                          MOVE.W    $002E(A0),D0
0000B99A                          SUB.W     $0008(A0),D0
0000B99E                          ADDQ.W    #$0,D0
0000B9A0                          CMP.W     #$0010,D0
0000B9A4                          BHI       $0000B9BA(pc)
0000B9A8                          MOVE.B    #$04,$0005(A0)
0000B9AE                          MOVEQ     #$02,D0
0000B9B0                          LEA       $0000CC46(pc),A1
0000B9B4                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000B9BA                          MOVE.W    $002E(A0),D0
0000B9BE                          TST.B     $0030(A0)
0000B9C2                          BNE       $0000B9D4
0000B9C4                          CMP.W     $0008(A0),D0
0000B9C8                          BCS       $0000B9E4
0000B9CA                          BSR       $0000C8A8(pc)
0000B9CE                          JMP       $00028804
0000B9D4                          CMP.W     $0008(A0),D0
0000B9D8                          BCC       $0000B9E4
0000B9DA                          BSR       $0000C8A8(pc)
0000B9DE                          JMP       $00028804
0000B9E4                          RTS
0000B9E6                          BSR       $0000C862(pc)
0000B9EA                          TST.B     $004A(A0)
0000B9EE                          BNE       $0000BA20
0000B9F0                          MOVE.B    $0030(A0),D0
0000B9F4                          CMP.B     $A030,D0
0000B9F8                          BEQ       $0000BA10
0000B9FA                          MOVE.B    #$00,$0005(A0)
0000BA00                          MOVE.B    $A030,$0030(A0)
0000BA06                          MOVE.B    #$00,$003A(A0)
0000BA0C                          BRA       $0000BA5C(pc)
0000BA10                          MOVE.W    $002E(A0),D0
0000BA14                          SUB.W     $0008(A0),D0
0000BA18                          ADDQ.W    #$0,D0
0000BA1A                          CMP.W     #$0010,D0
0000BA1E                          BLS       $0000BA5C
0000BA20                          MOVE.W    $002E(A0),D0
0000BA24                          TST.B     $0030(A0)
0000BA28                          BNE       $0000BA34
0000BA2A                          CMP.W     $0008(A0),D0
0000BA2E                          BCS       $0000BA3E
0000BA30                          BRA       $0000BA48(pc)
0000BA34                          CMP.W     $0008(A0),D0
0000BA38                          BCC       $0000BA3E
0000BA3A                          BRA       $0000BA48(pc)
0000BA3E                          MOVE.B    #$00,$004A(A0)
0000BA44                          BRA       $0000CBBE(pc)
0000BA48                          MOVEQ     #$00,D0
0000BA4A                          LEA       $0000CC46(pc),A1
0000BA4E                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000BA54                          MOVE.B    #$02,$0005(A0)
0000BA5A                          RTS
0000BA5C                          BSR       $0000C8A8(pc)
0000BA60                          JMP       $00028804
0000BA66                          MOVEQ     #$00,D0
0000BA68                          MOVE.B    $0005(A0),D0
0000BA6C                          MOVE.W    +$00(D3.W),D0
0000BA70                          JMP       +$00(D1.W)
0000BA74                          OR.B      #$5A,D6
0000BA78                          Invalid
0000BA7A                          MOVE.B    #$02,$0005(A0)
0000BA80                          BSR       $0000C870(pc)
0000BA84                          MOVE.B    #$1E,$0039(A0)
0000BA8A                          MOVEQ     #$00,D0
0000BA8C                          MOVE.W    $002E(A0),D0
0000BA90                          SUB.W     $0008(A0),D0
0000BA94                          SWAP      D0
0000BA96                          ASR.L     #6,D0
0000BA98                          LSR.L     #8,D0
0000BA9A                          MOVE.W    D0,$002A(A0)
0000BA9E                          MOVE.W    #$0680,$002C(A0)
0000BAA4                          TST.B     $0030(A0)
0000BAA8                          BNE       $0000BABC
0000BAAA                          LEA       $0000CD00(pc),A4
0000BAAE                          MOVEQ     #$00,D0
0000BAB0                          JSR       $000287D6
0000BAB6                          JMP       $00028804
0000BABC                          LEA       $0000CCFE(pc),A4
0000BAC0                          MOVEQ     #$00,D0
0000BAC2                          JSR       $000287D6
0000BAC8                          JMP       $00028804
0000BACE                          BSR       $0000C862(pc)
0000BAD2                          TST.B     $0039(A0)
0000BAD6                          BNE       $0000BB30
0000BAD8                          JSR       $00001AAE
0000BADE                          BCC       $0000BB34
0000BAE0                          ADD.W     Invalid
0000BAE2                          OR.B      #$28,A4
0000BAE6                          OR.B      #$28,-(A2)
0000BAEA                          OR.B      #$00,$0000C8EE(pc)
0000BAF0                          TST.B     $0048(A0)
0000BAF4                          BEQ       $0000BB0C
0000BAF6                          MOVE.B    #$00,$0048(A0)
0000BAFC                          MOVE.B    #$0C,$0003(A0)
0000BB02                          MOVE.W    #$0000,$0004(A0)
0000BB08                          BRA       $0000BB12(pc)
0000BB0C                          MOVE.B    #$04,$0005(A0)
0000BB12                          TST.B     $0030(A0)
0000BB16                          BNE       $0000BB24
0000BB18                          LEA       $0000CD00(pc),A4
0000BB1C                          MOVEQ     #$00,D0
0000BB1E                          JMP       $000287D6
0000BB24                          LEA       $0000CCFE(pc),A4
0000BB28                          MOVEQ     #$00,D0
0000BB2A                          JMP       $000287D6
0000BB30                          SUBQ.B    #$1,$0039(A0)
0000BB34                          JSR       $00001C3C
0000BB3A                          BCS       $0000BB52
0000BB3C                          JSR       $00001C94
0000BB42                          BCS       $0000BB52
0000BB44                          JSR       $000019F8
0000BB4A                          BCS       $0000BB5E
0000BB4C                          BSR       $0000CB9E(pc)
0000BB50                          BRA       $0000BB62
0000BB52                          MOVE.B    #$01,$0048(A0)
0000BB58                          BSR       $0000CBAA(pc)
0000BB5C                          BRA       $0000BB62
0000BB5E                          BSR       $0000CB90(pc)
0000BB62                          JMP       $00028804
0000BB68                          BSR       $0000C862(pc)
0000BB6C                          MOVE.B    $A030,D0
0000BB70                          CMP.B     $0030(A0),D0
0000BB74                          BNE       $0000BBDE(pc)
0000BB78                          MOVE.W    $0008(A0),D0
0000BB7C                          SUB.W     $A008,D0
0000BB80                          ADD.W     #$0040,D0
0000BB84                          CMP.W     #$0080,D0
0000BB88                          BHI       $0000BBEA(pc)
0000BB8C                          MOVE.B    #$02,$0005(A0)
0000BB92                          MOVE.B    #$0A,$0039(A0)
0000BB98                          MOVEQ     #$20,D0
0000BB9A                          TST.B     $0030(A0)
0000BB9E                          BEQ       $0000BBA2
0000BBA0                          MOVEQ     #$E0,D0
0000BBA2                          ADD.L     D0,D0
0000BBA4                          SWAP      D0
0000BBA6                          ASR.L     #6,D0
0000BBA8                          LSR.L     #8,D0
0000BBAA                          MOVE.W    D0,$002A(A0)
0000BBAE                          MOVE.W    #$0400,$002C(A0)
0000BBB4                          TST.B     $0030(A0)
0000BBB8                          BNE       $0000BBCC
0000BBBA                          LEA       $0000CCD6(pc),A4
0000BBBE                          MOVEQ     #$00,D0
0000BBC0                          JSR       $000287D6
0000BBC6                          JMP       $00028804
0000BBCC                          LEA       $0000CCDA(pc),A4
0000BBD0                          MOVEQ     #$00,D0
0000BBD2                          JSR       $000287D6
0000BBD8                          JMP       $00028804
0000BBDE                          MOVE.B    $A030,$0030(A0)
0000BBE4                          MOVE.W    #$0A00,$0004(A0)
0000BBEA                          RTS
0000BBEC                          MOVEQ     #$00,D0
0000BBEE                          MOVE.B    $0005(A0),D0
0000BBF2                          MOVE.W    +$00(D3.W),D0
0000BBF6                          JMP       +$00(D1.W)
0000BBFA                          OR.B      #$5A,D6
0000BBFE                          Invalid
0000BC00                          MOVE.B    #$02,$0005(A0)
0000BC06                          BSR       $0000C870(pc)
0000BC0A                          MOVE.B    #$1E,$0039(A0)
0000BC10                          MOVEQ     #$00,D0
0000BC12                          MOVE.W    $002E(A0),D0
0000BC16                          SUB.W     $0008(A0),D0
0000BC1A                          SWAP      D0
0000BC1C                          ASR.L     #6,D0
0000BC1E                          LSR.L     #8,D0
0000BC20                          MOVE.W    D0,$002A(A0)
0000BC24                          MOVE.W    #$0800,$002C(A0)
0000BC2A                          TST.B     $0030(A0)
0000BC2E                          BNE       $0000BC42
0000BC30                          LEA       $0000CD02(pc),A4
0000BC34                          MOVEQ     #$00,D0
0000BC36                          JSR       $000287D6
0000BC3C                          JMP       $00028804
0000BC42                          LEA       $0000CD06(pc),A4
0000BC46                          MOVEQ     #$00,D0
0000BC48                          JSR       $000287D6
0000BC4E                          JMP       $00028804
0000BC54                          BSR       $0000C862(pc)
0000BC58                          TST.B     $0039(A0)
0000BC5C                          BNE       $0000BCB4
0000BC5E                          JSR       $00001AAE
0000BC64                          BCC       $0000BCB8
0000BC66                          ADD.W     Invalid
0000BC68                          OR.B      #$28,A4
0000BC6C                          OR.B      #$28,-(A2)
0000BC70                          OR.B      #$00,$0000C8EE(pc)
0000BC76                          TST.B     $0048(A0)
0000BC7A                          BEQ       $0000BC90
0000BC7C                          MOVE.B    #$00,$0048(A0)
0000BC82                          MOVE.B    #$0C,$0003(A0)
0000BC88                          MOVE.W    #$0000,$0004(A0)
0000BC8E                          BRA       $0000BC96
0000BC90                          MOVE.B    #$04,$0005(A0)
0000BC96                          TST.B     $0030(A0)
0000BC9A                          BNE       $0000BCA8
0000BC9C                          LEA       $0000CCDE(pc),A4
0000BCA0                          MOVEQ     #$00,D0
0000BCA2                          JMP       $000287D6
0000BCA8                          LEA       $0000CCE2(pc),A4
0000BCAC                          MOVEQ     #$00,D0
0000BCAE                          JMP       $000287D6
0000BCB4                          SUBQ.B    #$1,$0039(A0)
0000BCB8                          JSR       $00001C3C
0000BCBE                          BCS       $0000BCD6
0000BCC0                          JSR       $00001C94
0000BCC6                          BCS       $0000BCD6
0000BCC8                          JSR       $000019F8
0000BCCE                          BCS       $0000BCE2
0000BCD0                          BSR       $0000CB9E(pc)
0000BCD4                          BRA       $0000BCE6
0000BCD6                          MOVE.B    #$01,$0048(A0)
0000BCDC                          BSR       $0000CBAA(pc)
0000BCE0                          BRA       $0000BCE6
0000BCE2                          BSR       $0000CB90(pc)
0000BCE6                          JMP       $00028804
0000BCEC                          BSR       $0000C862(pc)
0000BCF0                          MOVE.B    $A030,D0
0000BCF4                          CMP.B     $0030(A0),D0
0000BCF8                          BNE       $0000BD62(pc)
0000BCFC                          MOVE.W    $0008(A0),D0
0000BD00                          SUB.W     $A008,D0
0000BD04                          ADD.W     #$0040,D0
0000BD08                          CMP.W     #$0080,D0
0000BD0C                          BHI       $0000BD6E(pc)
0000BD10                          MOVE.B    #$02,$0005(A0)
0000BD16                          MOVE.B    #$19,$0039(A0)
0000BD1C                          MOVEQ     #$20,D0
0000BD1E                          TST.B     $0030(A0)
0000BD22                          BEQ       $0000BD26
0000BD24                          MOVEQ     #$E0,D0
0000BD26                          ADD.L     D0,D0
0000BD28                          SWAP      D0
0000BD2A                          ASR.L     #6,D0
0000BD2C                          LSR.L     #8,D0
0000BD2E                          MOVE.W    D0,$002A(A0)
0000BD32                          MOVE.W    #$0400,$002C(A0)
0000BD38                          TST.B     $0030(A0)
0000BD3C                          BNE       $0000BD50
0000BD3E                          LEA       $0000CCDE(pc),A4
0000BD42                          MOVEQ     #$00,D0
0000BD44                          JSR       $000287D6
0000BD4A                          JMP       $00028804
0000BD50                          LEA       $0000CCE2(pc),A4
0000BD54                          MOVEQ     #$00,D0
0000BD56                          JSR       $000287D6
0000BD5C                          JMP       $00028804
0000BD62                          MOVE.B    $A030,$0030(A0)
0000BD68                          MOVE.W    #$0C00,$0004(A0)
0000BD6E                          RTS
0000BD70                          MOVEQ     #$00,D0
0000BD72                          MOVE.B    $0005(A0),D0
0000BD76                          MOVE.W    +$00(D5.W),D0
0000BD7A                          JSR       +$00(D3.W)
0000BD7E                          BRA       $0000CC1C(pc)
0000BD82                          OR.B      #$50,D6
0000BD86                          OR.L      #$117C0002,-(A4)
0000BD8C                          OR.B      #$00,D5
0000BD90                          LEA       $0000CC46(pc),A1
0000BD94                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000BD9A                          BSR       $0000C870(pc)
0000BD9E                          MOVEQ     #$00,D0
0000BDA0                          TST.B     $0030(A0)
0000BDA4                          BEQ       $0000BDBC
0000BDA6                          MOVE.B    #$0B,$0016(A0)
0000BDAC                          LEA       $0000CCF8(pc),A4
0000BDB0                          JSR       $000287D6
0000BDB6                          JMP       $00028804
0000BDBC                          MOVE.B    #$05,$0016(A0)
0000BDC2                          LEA       $0000CCF6(pc),A4
0000BDC6                          JSR       $000287D6
0000BDCC                          JMP       $00028804
0000BDD2                          BSR       $0000C862(pc)
0000BDD6                          MOVE.W    $002E(A0),D0
0000BDDA                          SUB.W     $0008(A0),D0
0000BDDE                          ADDQ.W    #$0,D0
0000BDE0                          CMP.W     #$0010,D0
0000BDE4                          BHI       $0000BDFA(pc)
0000BDE8                          MOVE.B    #$04,$0005(A0)
0000BDEE                          MOVEQ     #$02,D0
0000BDF0                          LEA       $0000CC46(pc),A1
0000BDF4                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000BDFA                          MOVE.W    $002E(A0),D0
0000BDFE                          TST.B     $0030(A0)
0000BE02                          BNE       $0000BE14
0000BE04                          CMP.W     $0008(A0),D0
0000BE08                          BCS       $0000BE24
0000BE0A                          BSR       $0000C8A8(pc)
0000BE0E                          JMP       $00028804
0000BE14                          CMP.W     $0008(A0),D0
0000BE18                          BCC       $0000BE24
0000BE1A                          BSR       $0000C8A8(pc)
0000BE1E                          JMP       $00028804
0000BE24                          RTS
0000BE26                          BSR       $0000C862(pc)
0000BE2A                          TST.B     $004A(A0)
0000BE2E                          BNE       $0000BE60
0000BE30                          MOVE.B    $0030(A0),D0
0000BE34                          CMP.B     $A030,D0
0000BE38                          BEQ       $0000BE50
0000BE3A                          MOVE.B    #$00,$0005(A0)
0000BE40                          MOVE.B    $A030,$0030(A0)
0000BE46                          MOVE.B    #$00,$003A(A0)
0000BE4C                          BRA       $0000BE9C(pc)
0000BE50                          MOVE.W    $002E(A0),D0
0000BE54                          SUB.W     $0008(A0),D0
0000BE58                          ADDQ.W    #$0,D0
0000BE5A                          CMP.W     #$0010,D0
0000BE5E                          BLS       $0000BE9C
0000BE60                          MOVE.W    $002E(A0),D0
0000BE64                          TST.B     $0030(A0)
0000BE68                          BNE       $0000BE74
0000BE6A                          CMP.W     $0008(A0),D0
0000BE6E                          BCS       $0000BE7E
0000BE70                          BRA       $0000BE88(pc)
0000BE74                          CMP.W     $0008(A0),D0
0000BE78                          BCC       $0000BE7E
0000BE7A                          BRA       $0000BE88(pc)
0000BE7E                          MOVE.B    #$00,$004A(A0)
0000BE84                          BRA       $0000CBBE(pc)
0000BE88                          MOVEQ     #$00,D0
0000BE8A                          LEA       $0000CC46(pc),A1
0000BE8E                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000BE94                          MOVE.B    #$02,$0005(A0)
0000BE9A                          RTS
0000BE9C                          BSR       $0000C8A8(pc)
0000BEA0                          JMP       $00028804
0000BEA6                          MOVEQ     #$00,D0
0000BEA8                          MOVE.B    $0005(A0),D0
0000BEAC                          MOVE.W    +$00(D3.W),D0
0000BEB0                          JMP       +$00(D1.W)
0000BEB4                          OR.B      #$5A,D6
0000BEB8                          Invalid
0000BEBA                          MOVE.B    #$02,$0005(A0)
0000BEC0                          BSR       $0000C870(pc)
0000BEC4                          MOVE.B    #$0F,$0039(A0)
0000BECA                          MOVEQ     #$00,D0
0000BECC                          MOVE.W    $002E(A0),D0
0000BED0                          SUB.W     $0008(A0),D0
0000BED4                          SWAP      D0
0000BED6                          ASR.L     #6,D0
0000BED8                          LSR.L     #8,D0
0000BEDA                          MOVE.W    D0,$002A(A0)
0000BEDE                          MOVE.W    #$0000,$002C(A0)
0000BEE4                          TST.B     $0030(A0)
0000BEE8                          BNE       $0000BEFC
0000BEEA                          LEA       $0000CD0E(pc),A4
0000BEEE                          MOVEQ     #$00,D0
0000BEF0                          JSR       $000287D6
0000BEF6                          JMP       $00028804
0000BEFC                          LEA       $0000CD10(pc),A4
0000BF00                          MOVEQ     #$00,D0
0000BF02                          JSR       $000287D6
0000BF08                          JMP       $00028804
0000BF0E                          BSR       $0000C862(pc)
0000BF12                          TST.B     $0039(A0)
0000BF16                          BNE       $0000BF54
0000BF18                          JSR       $00001AAE
0000BF1E                          BCC       $0000BF58
0000BF20                          ADD.W     Invalid
0000BF22                          OR.B      #$28,A4
0000BF26                          OR.B      #$28,-(A2)
0000BF2A                          OR.B      #$00,$0000C8EE(pc)
0000BF30                          MOVE.B    #$04,$0005(A0)
0000BF36                          TST.B     $0030(A0)
0000BF3A                          BNE       $0000BF48
0000BF3C                          LEA       $0000CCEE(pc),A4
0000BF40                          MOVEQ     #$00,D0
0000BF42                          JMP       $000287D6
0000BF48                          LEA       $0000CCF2(pc),A4
0000BF4C                          MOVEQ     #$00,D0
0000BF4E                          JMP       $000287D6
0000BF54                          SUBQ.B    #$1,$0039(A0)
0000BF58                          JSR       $00001C3C
0000BF5E                          BCS       $0000BF74(pc)
0000BF62                          JSR       $00001C94
0000BF68                          BCS       $0000BF74(pc)
0000BF6C                          BSR       $0000CB9E(pc)
0000BF70                          BRA       $0000BF78(pc)
0000BF74                          BSR       $0000CBAA(pc)
0000BF78                          TST.B     $0039(A0)
0000BF7C                          BNE       $0000BF80
0000BF7E                          RTS
0000BF80                          JMP       $00028804
0000BF86                          BSR       $0000C862(pc)
0000BF8A                          MOVE.B    $A030,D0
0000BF8E                          CMP.B     $0030(A0),D0
0000BF92                          BNE       $0000BFFC(pc)
0000BF96                          MOVE.W    $0008(A0),D0
0000BF9A                          SUB.W     $A008,D0
0000BF9E                          ADD.W     #$0040,D0
0000BFA2                          CMP.W     #$0080,D0
0000BFA6                          BHI       $0000C008(pc)
0000BFAA                          MOVE.B    #$02,$0005(A0)
0000BFB0                          MOVE.B    #$0A,$0039(A0)
0000BFB6                          MOVEQ     #$20,D0
0000BFB8                          TST.B     $0030(A0)
0000BFBC                          BEQ       $0000BFC0
0000BFBE                          MOVEQ     #$E0,D0
0000BFC0                          ADD.L     D0,D0
0000BFC2                          SWAP      D0
0000BFC4                          ASR.L     #5,D0
0000BFC6                          LSR.L     #8,D0
0000BFC8                          MOVE.W    D0,$002A(A0)
0000BFCC                          MOVE.W    #$0000,$002C(A0)
0000BFD2                          TST.B     $0030(A0)
0000BFD6                          BNE       $0000BFEA
0000BFD8                          LEA       $0000CCEE(pc),A4
0000BFDC                          MOVEQ     #$00,D0
0000BFDE                          JSR       $000287D6
0000BFE4                          JMP       $00028804
0000BFEA                          LEA       $0000CCF2(pc),A4
0000BFEE                          MOVEQ     #$00,D0
0000BFF0                          JSR       $000287D6
0000BFF6                          JMP       $00028804
0000BFFC                          MOVE.B    $A030,$0030(A0)
0000C002                          MOVE.W    #$0400,$0004(A0)
0000C008                          RTS
0000C00A                          MOVEQ     #$00,D0
0000C00C                          MOVE.B    $0005(A0),D0
0000C010                          MOVE.W    +$00(D3.W),D0
0000C014                          JMP       +$00(D1.W)
0000C018                          OR.B      #$5A,D6
0000C01C                          Invalid
0000C01E                          MOVE.B    #$02,$0005(A0)
0000C024                          BSR       $0000C870(pc)
0000C028                          MOVE.B    #$0F,$0039(A0)
0000C02E                          MOVEQ     #$00,D0
0000C030                          MOVE.W    $002E(A0),D0
0000C034                          SUB.W     $0008(A0),D0
0000C038                          SWAP      D0
0000C03A                          ASR.L     #6,D0
0000C03C                          LSR.L     #8,D0
0000C03E                          MOVE.W    D0,$002A(A0)
0000C042                          MOVE.W    #$0100,$002C(A0)
0000C048                          TST.B     $0030(A0)
0000C04C                          BNE       $0000C060
0000C04E                          LEA       $0000CD0A(pc),A4
0000C052                          MOVEQ     #$00,D0
0000C054                          JSR       $000287D6
0000C05A                          JMP       $00028804
0000C060                          LEA       $0000CD0C(pc),A4
0000C064                          MOVEQ     #$00,D0
0000C066                          JSR       $000287D6
0000C06C                          JMP       $00028804
0000C072                          BSR       $0000C862(pc)
0000C076                          TST.B     $0039(A0)
0000C07A                          BNE       $0000C0B8
0000C07C                          JSR       $00001AAE
0000C082                          BCC       $0000C0BC
0000C084                          ADD.W     Invalid
0000C086                          OR.B      #$28,A4
0000C08A                          OR.B      #$28,-(A2)
0000C08E                          OR.B      #$00,$0000C8EE(pc)
0000C094                          MOVE.B    #$04,$0005(A0)
0000C09A                          TST.B     $0030(A0)
0000C09E                          BNE       $0000C0AC
0000C0A0                          LEA       $0000CCE6(pc),A4
0000C0A4                          MOVEQ     #$00,D0
0000C0A6                          JMP       $000287D6
0000C0AC                          LEA       $0000CCEA(pc),A4
0000C0B0                          MOVEQ     #$00,D0
0000C0B2                          JMP       $000287D6
0000C0B8                          SUBQ.B    #$1,$0039(A0)
0000C0BC                          JSR       $00001C3C
0000C0C2                          BCS       $0000C0D8(pc)
0000C0C6                          JSR       $00001C94
0000C0CC                          BCS       $0000C0D8(pc)
0000C0D0                          BSR       $0000CB9E(pc)
0000C0D4                          BRA       $0000C0DC(pc)
0000C0D8                          BSR       $0000CBAA(pc)
0000C0DC                          TST.B     $0039(A0)
0000C0E0                          BEQ       $0000C0E8
0000C0E2                          JMP       $00028804
0000C0E8                          RTS
0000C0EA                          BSR       $0000C862(pc)
0000C0EE                          MOVE.B    $A030,D0
0000C0F2                          CMP.B     $0030(A0),D0
0000C0F6                          BNE       $0000C160(pc)
0000C0FA                          MOVE.W    $0008(A0),D0
0000C0FE                          SUB.W     $A008,D0
0000C102                          ADD.W     #$0040,D0
0000C106                          CMP.W     #$0080,D0
0000C10A                          BHI       $0000C16C(pc)
0000C10E                          MOVE.B    #$02,$0005(A0)
0000C114                          MOVE.B    #$0A,$0039(A0)
0000C11A                          MOVEQ     #$20,D0
0000C11C                          TST.B     $0030(A0)
0000C120                          BEQ       $0000C124
0000C122                          MOVEQ     #$E0,D0
0000C124                          ADD.L     D0,D0
0000C126                          SWAP      D0
0000C128                          ASR.L     #5,D0
0000C12A                          LSR.L     #8,D0
0000C12C                          MOVE.W    D0,$002A(A0)
0000C130                          MOVE.W    #$0100,$002C(A0)
0000C136                          TST.B     $0030(A0)
0000C13A                          BNE       $0000C14E
0000C13C                          LEA       $0000CCE6(pc),A4
0000C140                          MOVEQ     #$00,D0
0000C142                          JSR       $000287D6
0000C148                          JMP       $00028804
0000C14E                          LEA       $0000CCEA(pc),A4
0000C152                          MOVEQ     #$00,D0
0000C154                          JSR       $000287D6
0000C15A                          JMP       $00028804
0000C160                          MOVE.B    $A030,$0030(A0)
0000C166                          MOVE.W    #$0600,$0004(A0)
0000C16C                          RTS
0000C16E                          MOVEQ     #$00,D0
0000C170                          MOVE.B    $0004(A0),D0
0000C174                          MOVE.W    +$00(D5.W),D0
0000C178                          JSR       +$00(D3.W)
0000C17C                          BRA       $0000CC16(pc)
0000C180                          OR.B      #$EC,D4
0000C184                          TST.B     $004A(A0)
0000C188                          BNE       $0000C1BE
0000C18A                          MOVE.B    $0030(A0),D0
0000C18E                          CMP.B     $A030,D0
0000C192                          BEQ       $0000C1BE
0000C194                          MOVE.B    $A030,$0030(A0)
0000C19A                          ADDQ.W    #$2,$0008(A0)
0000C19E                          TST.B     $0030(A0)
0000C1A2                          BEQ       $0000C1A8
0000C1A4                          SUBQ.W    #$4,$0008(A0)
0000C1A8                          MOVE.B    #$02,$0003(A0)
0000C1AE                          MOVE.B    $003A(A0),$0022(A0)
0000C1B4                          MOVE.B    #$00,$003A(A0)
0000C1BA                          BRA       $0000C8F0(pc)
0000C1BE                          TST.B     $0030(A0)
0000C1C2                          BNE       $0000C1D8
0000C1C4                          MOVE.W    $A008,D0
0000C1C8                          SUB.W     #$0060,D0
0000C1CC                          CMP.W     $0008(A0),D0
0000C1D0                          BCC       $0000C216(pc)
0000C1D4                          BRA       $0000C1E8(pc)
0000C1D8                          MOVE.W    $A008,D0
0000C1DC                          ADD.W     #$0060,D0
0000C1E0                          CMP.W     $0008(A0),D0
0000C1E4                          BCS       $0000C216(pc)
0000C1E8                          MOVE.B    #$00,$004A(A0)
0000C1EE                          BSR       $0000CBBE(pc)
0000C1F2                          MOVE.W    $A00C,D0
0000C1F6                          SUB.W     $000C(A0),D0
0000C1FA                          ADD.W     #$0060,D0
0000C1FE                          CMP.W     #$00C0,D0
0000C202                          BLS       $0000C26A(pc)
0000C206                          MOVE.B    #$06,$0003(A0)
0000C20C                          MOVE.W    #$0000,$0004(A0)
0000C212                          BRA       $0000C26A(pc)
0000C216                          MOVE.B    #$02,$0004(A0)
0000C21C                          MOVE.W    #$00A0,D0
0000C220                          TST.B     $0030(A0)
0000C224                          BEQ       $0000C22A
0000C226                          MOVE.W    #$FF60,D0
0000C22A                          SWAP      D0
0000C22C                          ASR.L     #6,D0
0000C22E                          LSR.L     #8,D0
0000C230                          MOVE.W    D0,$002A(A0)
0000C234                          MOVE.W    #$0400,$002C(A0)
0000C23A                          MOVE.B    #$1E,$0039(A0)
0000C240                          TST.B     $0030(A0)
0000C244                          BNE       $0000C258
0000C246                          LEA       $0000CCFA(pc),A4
0000C24A                          MOVEQ     #$00,D0
0000C24C                          JSR       $000287D6
0000C252                          JMP       $00028804
0000C258                          LEA       $0000CCFC(pc),A4
0000C25C                          MOVEQ     #$00,D0
0000C25E                          JSR       $000287D6
0000C264                          JMP       $00028804
0000C26A                          RTS
0000C26C                          TST.B     $0039(A0)
0000C270                          BNE       $0000C28C
0000C272                          JSR       $00001AAE
0000C278                          BCC       $0000C290(pc)
0000C27C                          MOVE.B    #$00,$003A(A0)
0000C282                          MOVE.B    #$02,$0003(A0)
0000C288                          BRA       $0000C8E0(pc)
0000C28C                          SUBQ.B    #$1,$0039(A0)
0000C290                          JSR       $00001C3C
0000C296                          BCS       $0000C2B0
0000C298                          JSR       $00001C3C
0000C29E                          BCS       $0000C2B0
0000C2A0                          JSR       $000019F8
0000C2A6                          BCS       $0000C2B8
0000C2A8                          BSR       $0000CB9E(pc)
0000C2AC                          BRA       $0000C2BC(pc)
0000C2B0                          BSR       $0000CBAA(pc)
0000C2B4                          BRA       $0000C2BC(pc)
0000C2B8                          BSR       $0000CB90(pc)
0000C2BC                          JMP       $00028804
0000C2C2                          MOVEQ     #$00,D0
0000C2C4                          MOVE.B    $0004(A0),D0
0000C2C8                          MOVE.W    +$00(D3.W),D0
0000C2CC                          JMP       +$00(D1.W)
0000C2D0                          OR.B      #$2C,D6
0000C2D4                          OR.L      #$0C380004,
0000C2DA                          Invalid
0000C2DC                          BEQ       $0000C2FA(pc)
0000C2E0                          CMP.B     #$08,$A003
0000C2E6                          BEQ       $0000C2FA(pc)
0000C2EA                          CMP.B     #$0C,$A003
0000C2F0                          BEQ       $0000C2FA(pc)
0000C2F4                          MOVE.B    #$02,$0004(A0)
0000C2FA                          RTS
0000C2FC                          MOVE.B    #$04,$0004(A0)
0000C302                          MOVE.W    $000C(A0),D0
0000C306                          CMP.W     $A00C,D0
0000C30A                          BCC       $0000C34E(pc)
0000C30E                          MOVE.W    $A008,D0
0000C312                          SUB.W     $0008(A0),D0
0000C316                          SWAP      D0
0000C318                          ASR.L     #6,D0
0000C31A                          LSR.L     #8,D0
0000C31C                          MOVE.W    D0,$002A(A0)
0000C320                          MOVE.W    #$0800,$002C(A0)
0000C326                          MOVE.B    #$00,$0039(A0)
0000C32C                          MOVE.W    $0008(A0),D0
0000C330                          CMP.W     $A008,D0
0000C334                          BCS       $0000C342
0000C336                          MOVEQ     #$00,D0
0000C338                          LEA       $0000CD06(pc),A4
0000C33C                          JMP       $000287D6
0000C342                          MOVEQ     #$00,D0
0000C344                          LEA       $0000CD02(pc),A4
0000C348                          JMP       $000287D6
0000C34E                          MOVE.W    $A008,D0
0000C352                          SUB.W     $0008(A0),D0
0000C356                          SWAP      D0
0000C358                          ASR.L     #6,D0
0000C35A                          LSR.L     #8,D0
0000C35C                          MOVE.W    D0,$002A(A0)
0000C360                          MOVE.W    #$0080,$002C(A0)
0000C366                          MOVE.B    #$14,$0039(A0)
0000C36C                          MOVE.W    $0008(A0),D0
0000C370                          CMP.W     $A008,D0
0000C374                          BCS       $0000C382
0000C376                          MOVEQ     #$00,D0
0000C378                          LEA       $0000CD0C(pc),A4
0000C37C                          JMP       $000287D6
0000C382                          MOVEQ     #$00,D0
0000C384                          LEA       $0000CD0A(pc),A4
0000C388                          JMP       $000287D6
0000C38E                          TST.B     $0039(A0)
0000C392                          BEQ       $0000C39A
0000C394                          SUBQ.B    #$1,$0039(A0)
0000C398                          BRA       $0000C3A4
0000C39A                          JSR       $00001AAE
0000C3A0                          BCS       $0000C3CC(pc)
0000C3A4                          MOVE.W    $002A(A0),D0
0000C3A8                          BEQ       $0000C3B2
0000C3AA                          EXT.L     D0
0000C3AC                          LSL.L     #8,D0
0000C3AE                          ADD.L     Invalid
0000C3B0                          OR.B      #$28,A0
0000C3B4                          OR.B      #$08,$48C0(A4)
0000C3BA                          LSL.L     #8,D0
0000C3BC                          (CCC     #$0,$000C(A0)
0000C3C0                          SUB.W     #$0040,$002C(A0)
0000C3C6                          JMP       $00028804
0000C3CC                          ADD.W     Invalid
0000C3CE                          OR.B      #$7C,A4
0000C3D2                          OR.B      #$03,D2
0000C3D6                          MOVE.B    #$00,$003A(A0)
0000C3DC                          BRA       $0000C8F0(pc)
0000C3E0                          MOVEQ     #$00,D0
0000C3E2                          MOVE.B    $0004(A0),D0
0000C3E6                          MOVE.W    +$00(D3.W),D0
0000C3EA                          JMP       +$00(D1.W)
0000C3EE                          OR.B      #$7A,D4
0000C3F2                          MOVE.B    #$02,$0004(A0)
0000C3F8                          TST.B     $0006(A0)
0000C3FC                          BNE       $0000C440(pc)
0000C400                          MOVE.B    #$06,$0046(A0)
0000C406                          MOVE.B    #$14,$0039(A0)
0000C40C                          TST.B     $0030(A0)
0000C410                          BNE       $0000C41E
0000C412                          MOVE.W    $A008,D0
0000C416                          ADD.W     #$00F6,D0
0000C41A                          BRA       $0000C426(pc)
0000C41E                          MOVE.W    $A008,D0
0000C422                          SUB.W     #$00F6,D0
0000C426                          SUB.W     $0008(A0),D0
0000C42A                          SWAP      D0
0000C42C                          ASR.L     #6,D0
0000C42E                          LSR.L     #8,D0
0000C430                          MOVE.W    D0,$002A(A0)
0000C434                          MOVE.W    #$0600,$002C(A0)
0000C43A                          MOVEQ     #$00,D0
0000C43C                          BRA       $0000C44C(pc)
0000C440                          MOVE.B    #$0E,$0038(A0)
0000C446                          MOVEQ     #$00,D0
0000C448                          MOVE.B    $0047(A0),D0
0000C44C                          TST.B     $0030(A0)
0000C450                          BNE       $0000C45E(pc)
0000C454                          LEA       $0000CD12(pc),A4
0000C458                          JMP       $000287D6
0000C45E                          LEA       $0000CD1A(pc),A4
0000C462                          JMP       $000287D6
0000C468                          TST.B     $0006(A0)
0000C46C                          BNE       $0000C4D8(pc)
0000C470                          JSR       $00001C3C
0000C476                          BCS       $0000C48E
0000C478                          JSR       $00001C94
0000C47E                          BCS       $0000C48E
0000C480                          JSR       $000019F8
0000C486                          BCS       $0000C494
0000C488                          BSR       $0000CB9E(pc)
0000C48C                          BRA       $0000C49A
0000C48E                          BSR       $0000CBAA(pc)
0000C492                          BRA       $0000C49A
0000C494                          BSR       $0000CB90(pc)
0000C498                          BRA       $0000C4A0
0000C49A                          SUB.W     #$0010,$002C(A0)
0000C4A0                          TST.B     $0039(A0)
0000C4A4                          BNE       $0000C4BE
0000C4A6                          JSR       $00001AAE
0000C4AC                          BCC       $0000C4C2
0000C4AE                          MOVE.B    #$02,$0003(A0)
0000C4B4                          MOVE.B    #$00,$003A(A0)
0000C4BA                          BRA       $0000C8E0(pc)
0000C4BE                          SUBQ.B    #$1,$0039(A0)
0000C4C2                          SUBQ.B    #$1,$0046(A0)
0000C4C6                          BNE       $0000C4D2
0000C4C8                          BSR       $0000CB5A(pc)
0000C4CC                          MOVE.B    #$06,$0046(A0)
0000C4D2                          JMP       $00028804
0000C4D8                          SUBQ.B    #$1,$0038(A0)
0000C4DC                          BNE       $0000C4EA
0000C4DE                          MOVE.W    #$0600,$0002(A0)
0000C4E4                          MOVE.W    #$0000,$0004(A0)
0000C4EA                          RTS
0000C4EC                          MOVEQ     #$00,D0
0000C4EE                          MOVE.B    $0004(A0),D0
0000C4F2                          MOVE.W    +$00(D3.W),D0
0000C4F6                          JMP       +$00(D1.W)
0000C4FA                          OR.B      #$2C,D6
0000C4FE                          BTST      D0,$0C38(A4)
0000C502                          OR.B      #$03,D4
0000C506                          BEQ       $0000C524(pc)
0000C50A                          CMP.B     #$08,$A003
0000C510                          BEQ       $0000C524(pc)
0000C514                          CMP.B     #$0C,$A003
0000C51A                          BEQ       $0000C524(pc)
0000C51E                          MOVE.B    #$02,$0004(A0)
0000C524                          RTS
0000C526                          MOVE.B    #$04,$0004(A0)
0000C52C                          MOVE.W    $EA0C,D0
0000C530                          ADD.W     #$0080,D0
0000C534                          SUB.W     $003E(A0),D0
0000C538                          ADD.W     #$0080,D0
0000C53C                          CMP.W     #$0100,D0
0000C540                          BLS       $0000C5A0(pc)
0000C544                          MOVE.W    $003E(A0),D0
0000C548                          CMP.W     $0034(A0),D0
0000C54C                          BCS       $0000C576
0000C54E                          MOVE.W    $A008,$0008(A0)
0000C554                          MOVE.W    $EA0C,$000C(A0)
0000C55A                          ADDQ.W    #$0,$000C(A0)
0000C55E                          MOVE.W    #$0000,$002A(A0)
0000C564                          MOVE.W    #$0800,$002C(A0)
0000C56A                          MOVEQ     #$01,D0
0000C56C                          LEA       $0000CD02(pc),A4
0000C570                          JMP       $000287D6
0000C576                          MOVE.W    $A008,$0008(A0)
0000C57C                          MOVE.W    $EA0C,D0
0000C580                          ADD.W     #$00F8,D0
0000C584                          MOVE.W    D0,$000C(A0)
0000C588                          MOVE.W    #$0000,$002A(A0)
0000C58E                          MOVE.W    #$0000,$002C(A0)
0000C594                          MOVEQ     #$00,D0
0000C596                          LEA       $0000CD0A(pc),A4
0000C59A                          JMP       $000287D6
0000C5A0                          MOVE.W    $003C(A0),D0
0000C5A4                          CMP.W     $0032(A0),D0
0000C5A8                          BCS       $0000C5EC(pc)
0000C5AC                          MOVE.W    $EA08,D0
0000C5B0                          ADD.W     #$0200,D0
0000C5B4                          MOVE.W    D0,$0008(A0)
0000C5B8                          MOVE.W    $EA0C,D1
0000C5BC                          ADD.W     #$0080,D1
0000C5C0                          MOVE.W    D1,$000C(A0)
0000C5C4                          MOVE.W    $A008,D0
0000C5C8                          SUB.W     #$0030,D0
0000C5CC                          SUB.W     $0008(A0),D0
0000C5D0                          SWAP      D0
0000C5D2                          ASR.L     #6,D0
0000C5D4                          LSR.L     #8,D0
0000C5D6                          MOVE.W    D0,$002A(A0)
0000C5DA                          MOVE.W    #$0400,$002C(A0)
0000C5E0                          MOVEQ     #$00,D0
0000C5E2                          LEA       $0000CCFC(pc),A4
0000C5E6                          JMP       $000287D6
0000C5EC                          MOVE.W    $EA08,$0008(A0)
0000C5F2                          MOVE.W    $EA0C,D1
0000C5F6                          ADD.W     #$0080,D1
0000C5FA                          MOVE.W    D1,$000C(A0)
0000C5FE                          MOVE.W    $A008,D0
0000C602                          ADD.W     #$0030,D0
0000C606                          SUB.W     $0008(A0),D0
0000C60A                          SWAP      D0
0000C60C                          ASR.L     #6,D0
0000C60E                          LSR.L     #8,D0
0000C610                          MOVE.W    D0,$002A(A0)
0000C614                          MOVE.W    #$0400,$002C(A0)
0000C61A                          MOVEQ     #$00,D0
0000C61C                          LEA       $0000CCFA(pc),A4
0000C620                          JMP       $000287D6
0000C626                          JSR       $00001AAE
0000C62C                          BCS       $0000C68C(pc)
0000C630                          MOVE.W    $002A(A0),D0
0000C634                          BEQ       $0000C63E
0000C636                          EXT.L     D0
0000C638                          LSL.L     #8,D0
0000C63A                          ADD.L     Invalid
0000C63C                          OR.B      #$28,A0
0000C640                          OR.B      #$C0,-$1E78(A4)
0000C646                          (CCC     #$0,$000C(A0)
0000C64A                          MOVE.W    $EA08,D0
0000C64E                          ADD.W     #$0100,D0
0000C652                          SUB.W     $0008(A0),D0
0000C656                          ADD.W     #$010E,D0
0000C65A                          CMP.W     #$021C,D0
0000C65E                          BHI       $0000C6A6(pc)
0000C662                          MOVE.W    $EA0C,D0
0000C666                          ADD.W     #$0080,D0
0000C66A                          SUB.W     $000C(A0),D0
0000C66E                          ADD.W     #$008C,D0
0000C672                          CMP.W     #$0118,D0
0000C676                          BHI       $0000C6A6(pc)
0000C67A                          SUB.W     #$0040,$002C(A0)
0000C680                          JSR       $00028804
0000C686                          JMP       $00028C60
0000C68C                          ADD.W     Invalid
0000C68E                          OR.B      #$7C,A4
0000C692                          OR.B      #$03,D2
0000C696                          MOVE.B    #$00,$003A(A0)
0000C69C                          BSR       $0000C8F0(pc)
0000C6A0                          JMP       $00028C60
0000C6A6                          MOVE.W    $0008(A0),$003C(A0)
0000C6AC                          MOVE.W    $000C(A0),$003E(A0)
0000C6B2                          MOVE.W    $A008,$0032(A0)
0000C6B8                          MOVE.W    $A00C,$0034(A0)
0000C6BE                          MOVE.B    #$00,$0004(A0)
0000C6C4                          RTS
0000C6C6                          MOVEQ     #$00,D0
0000C6C8                          MOVE.B    $0004(A0),D0
0000C6CC                          MOVE.W    +$00(D3.W),D0
0000C6D0                          JMP       +$00(D1.W)
0000C6D4                          OR.B      #$1E,D6
0000C6D8                          Invalid
0000C6DA                          MOVE.W    #$0020,$0042(A0)
0000C6E0                          MOVE.W    $000C(A0),$0040(A0)
0000C6E6                          ADD.W     #$0100,$0040(A0)
0000C6EC                          MOVE.B    #$02,$0004(A0)
0000C6F2                          MOVE.B    $0030(A0),D0
0000C6F6                          CMP.B     $A030,D0
0000C6FA                          BEQ       $0000C712
0000C6FC                          MOVE.B    #$02,$0003(A0)
0000C702                          MOVE.W    #$0000,$0004(A0)
0000C708                          MOVE.B    #$00,$003A(A0)
0000C70E                          BRA       $0000C8F0(pc)
0000C712                          ADD.W     #$0030,$0008(A0)
0000C718                          TST.B     $0030(A0)
0000C71C                          BEQ       $0000C724
0000C71E                          SUB.W     #$0060,$0008(A0)
0000C724                          MOVE.W    $0042(A0),D0
0000C728                          (CCC     #$0,$000C(A0)
0000C72C                          MOVE.W    $0040(A0),D0
0000C730                          CMP.W     $000C(A0),D0
0000C734                          BCC       $0000C746
0000C736                          MOVE.B    #$04,$0003(A0)
0000C73C                          MOVE.W    #$0000,$0004(A0)
0000C742                          BRA       $0000C750(pc)
0000C746                          JSR       $00001AE2
0000C74C                          BCS       $0000C770(pc)
0000C750                          SUB.W     #$0030,$0008(A0)
0000C756                          TST.B     $0030(A0)
0000C75A                          BEQ       $0000C762
0000C75C                          ADD.W     #$0060,$0008(A0)
0000C762                          MOVE.W    $0042(A0),D0
0000C766                          ADD.W     Invalid
0000C768                          OR.B      #$68,A4
0000C76C                          OR.W      #$4E75,D2
0000C770                          MOVE.W    $000C(A0),D5
0000C774                          SUB.W     #$0030,$0008(A0)
0000C77A                          TST.B     $0030(A0)
0000C77E                          BEQ       $0000C786
0000C780                          ADD.W     #$0060,$0008(A0)
0000C786                          MOVE.W    $0042(A0),D0
0000C78A                          ADD.W     Invalid
0000C78C                          OR.B      #$3C,A4
0000C790                          OR.B      #$28,-(A0)
0000C794                          OR.B      #$04,$30(A0,D6.L)
0000C79A                          Invalid
0000C79C                          SWAP      D0
0000C79E                          ASR.L     #5,D0
0000C7A0                          ASR.L     #8,D0
0000C7A2                          MOVE.W    D0,$002A(A0)
0000C7A6                          SUB.W     $000C(A0),D5
0000C7AA                          SWAP      D5
0000C7AC                          ASR.L     #5,D5
0000C7AE                          ASR.L     #4,D5
0000C7B0                          LSR.L     #8,D5
0000C7B2                          MOVE.W    D5,$0044(A0)
0000C7B6                          ASL.L     #5,D5
0000C7B8                          MOVE.W    D5,$002C(A0)
0000C7BC                          MOVE.B    #$0A,$0039(A0)
0000C7C2                          MOVE.B    #$04,$0004(A0)
0000C7C8                          MOVEQ     #$00,D0
0000C7CA                          BRA       $0000C44C(pc)
0000C7CE                          MOVE.W    $002A(A0),D0
0000C7D2                          EXT.L     D0
0000C7D4                          LSL.L     #8,D0
0000C7D6                          ADD.L     Invalid
0000C7D8                          OR.B      #$28,A0
0000C7DC                          OR.B      #$C0,-$1E78(A4)
0000C7E2                          (CCC     #$0,$000C(A0)
0000C7E6                          MOVE.W    $0044(A0),D0
0000C7EA                          (CCC     #$0,$002C(A0)
0000C7EE                          TST.B     $0039(A0)
0000C7F2                          BEQ       $0000C7FE
0000C7F4                          SUBQ.B    #$1,$0039(A0)
0000C7F8                          JMP       $00028804
0000C7FE                          JSR       $00001AE2
0000C804                          BCS       $0000C80E(pc)
0000C808                          JMP       $00028804
0000C80E                          ADD.W     Invalid
0000C810                          OR.B      #$7C,A4
0000C814                          OR.B      #$3A,D0
0000C818                          MOVE.B    #$02,$0003(A0)
0000C81E                          BRA       $0000C8F0(pc)
0000C822                          MOVE.W    #$0600,$0002(A0)
0000C828                          RTS
0000C82A                          TST.B     $F0D8
0000C82E                          BNE       $0000C850
0000C830                          TST.B     $0006(A0)
0000C834                          BNE       $0000C84A
0000C836                          MOVE.B    #$02,$0002(A0)
0000C83C                          MOVE.B    #$0A,$0003(A0)
0000C842                          MOVE.W    #$0000,$0004(A0)
0000C848                          RTS
0000C84A                          JMP       $00028B48
0000C850                          JSR       $000017F2
0000C856                          MOVE.B    #$00,$F0D8
0000C85C                          JMP       $00028B48
0000C862                          MOVE.W    $A008,D0
0000C866                          SUB.W     $0032(A0),D0
0000C86A                          ADD.W     Invalid
0000C86C                          OR.B      #$75,$3038(A6)
0000C872                          Invalid
0000C874                          TST.B     $004A(A0)
0000C878                          BEQ       $0000C884
0000C87A                          TST.B     $0030(A0)
0000C87E                          BEQ       $0000C88A
0000C880                          BRA       $0000C892(pc)
0000C884                          TST.B     $A030
0000C888                          BNE       $0000C892
0000C88A                          ADD.W     #$0060,D0
0000C88E                          BRA       $0000C896(pc)
0000C892                          SUB.W     #$0060,D0
0000C896                          MOVE.W    D0,$002E(A0)
0000C89A                          MOVEQ     #$00,D0
0000C89C                          LEA       $0000CC46(pc),A1
0000C8A0                          MOVE.W    $00(A1,D0.W),$0018(A0)
0000C8A6                          RTS
0000C8A8                          JSR       $00001C3C
0000C8AE                          BCS       $0000C8D2(pc)
0000C8B2                          JSR       $00001C94
0000C8B8                          BCS       $0000C8D2(pc)
0000C8BC                          JSR       $00001AAE
0000C8C2                          BCS       $0000C8E0
0000C8C4                          MOVE.B    #$04,$0003(A0)
0000C8CA                          MOVE.W    #$0000,$0004(A0)
0000C8D0                          RTS
0000C8D2                          MOVE.B    #$0C,$0003(A0)
0000C8D8                          MOVE.W    #$0000,$0004(A0)
0000C8DE                          RTS
0000C8E0                          ADD.W     Invalid
0000C8E2                          OR.B      #$28,A4
0000C8E6                          OR.B      #$28,$0000C90A(pc)
0000C8EC                          BEQ       $0000C9AA(pc)
0000C8F0                          CMP.B     #$01,$0022(A0)
0000C8F6                          BNE       $0000C90C(pc)
0000C8FA                          MOVE.B    $0022(A0),$003A(A0)
0000C900                          TST.B     $0030(A0)
0000C904                          BNE       $0000C9CA(pc)
0000C908                          BRA       $0000C9B6(pc)
0000C90C                          CMP.B     #$02,$0022(A0)
0000C912                          BNE       $0000C928(pc)
0000C916                          MOVE.B    $0022(A0),$003A(A0)
0000C91C                          TST.B     $0030(A0)
0000C920                          BEQ       $0000C9F2(pc)
0000C924                          BRA       $0000CABA(pc)
0000C928                          CMP.B     #$03,$0022(A0)
0000C92E                          BNE       $0000C944(pc)
0000C932                          MOVE.B    $0022(A0),$003A(A0)
0000C938                          TST.B     $0030(A0)
0000C93C                          BEQ       $0000CA2E(pc)
0000C940                          BRA       $0000CAF6(pc)
0000C944                          CMP.B     #$04,$0022(A0)
0000C94A                          BNE       $0000C960(pc)
0000C94E                          MOVE.B    $0022(A0),$003A(A0)
0000C954                          TST.B     $0030(A0)
0000C958                          BEQ       $0000CA6A(pc)
0000C95C                          BRA       $0000CB32(pc)
0000C960                          CMP.B     #$05,$0022(A0)
0000C966                          BNE       $0000C97C(pc)
0000C96A                          MOVE.B    $0022(A0),$003A(A0)
0000C970                          TST.B     $0030(A0)
0000C974                          BEQ       $0000CB1E(pc)
0000C978                          BRA       $0000CA7E(pc)
0000C97C                          CMP.B     #$06,$0022(A0)
0000C982                          BNE       $0000C998(pc)
0000C986                          MOVE.B    $0022(A0),$003A(A0)
0000C98C                          TST.B     $0030(A0)
0000C990                          BEQ       $0000CAE2(pc)
0000C994                          BRA       $0000CA42(pc)
0000C998                          MOVE.B    $0022(A0),$003A(A0)
0000C99E                          TST.B     $0030(A0)
0000C9A2                          BEQ       $0000CAA6(pc)
0000C9A6                          BRA       $0000CA06(pc)
0000C9AA                          MOVE.B    $0022(A0),$003A(A0)
0000C9B0                          JMP       $00028842
0000C9B6                          MOVE.B    #$00,$0030(A0)
0000C9BC                          MOVE.B    #$04,$0016(A0)
0000C9C2                          LEA       $0000CCD0(pc),A4
0000C9C6                          BRA       $0000C9DA(pc)
0000C9CA                          MOVE.B    #$01,$0030(A0)
0000C9D0                          MOVE.B    #$0C,$0016(A0)
0000C9D6                          LEA       $0000CCCE(pc),A4
0000C9DA                          MOVE.B    #$00,$0004(A0)
0000C9E0                          MOVE.B    #$04,$0005(A0)
0000C9E6                          MOVEQ     #$00,D0
0000C9E8                          JSR       $000287D6
0000C9EE                          BRA       $0000C870(pc)
0000C9F2                          MOVE.B    #$00,$0030(A0)
0000C9F8                          MOVE.B    #$03,$0016(A0)
0000C9FE                          LEA       $0000CCD2(pc),A4
0000CA02                          BRA       $0000CA16(pc)
0000CA06                          MOVE.B    #$01,$0030(A0)
0000CA0C                          MOVE.B    #$0D,$0016(A0)
0000CA12                          LEA       $0000CCD4(pc),A4
0000CA16                          MOVE.B    #$02,$0004(A0)
0000CA1C                          MOVE.B    #$04,$0005(A0)
0000CA22                          MOVEQ     #$00,D0
0000CA24                          JSR       $000287D6
0000CA2A                          BRA       $0000C870(pc)
0000CA2E                          MOVE.B    #$00,$0030(A0)
0000CA34                          MOVE.B    #$02,$0016(A0)
0000CA3A                          LEA       $0000CCD6(pc),A4
0000CA3E                          BRA       $0000CA52(pc)
0000CA42                          MOVE.B    #$01,$0030(A0)
0000CA48                          MOVE.B    #$0E,$0016(A0)
0000CA4E                          LEA       $0000CCDA(pc),A4
0000CA52                          MOVE.B    #$04,$0004(A0)
0000CA58                          MOVE.B    #$04,$0005(A0)
0000CA5E                          MOVEQ     #$00,D0
0000CA60                          JSR       $000287D6
0000CA66                          BRA       $0000C870(pc)
0000CA6A                          MOVE.B    #$00,$0030(A0)
0000CA70                          MOVE.B    #$01,$0016(A0)
0000CA76                          LEA       $0000CCDE(pc),A4
0000CA7A                          BRA       $0000CA8E(pc)
0000CA7E                          MOVE.B    #$01,$0030(A0)
0000CA84                          MOVE.B    #$0F,$0016(A0)
0000CA8A                          LEA       $0000CCE2(pc),A4
0000CA8E                          MOVE.B    #$06,$0004(A0)
0000CA94                          MOVE.B    #$04,$0005(A0)
0000CA9A                          MOVEQ     #$00,D0
0000CA9C                          JSR       $000287D6
0000CAA2                          BRA       $0000C870(pc)
0000CAA6                          MOVE.B    #$00,$0030(A0)
0000CAAC                          MOVE.B    #$05,$0016(A0)
0000CAB2                          LEA       $0000CCF6(pc),A4
0000CAB6                          BRA       $0000CACA(pc)
0000CABA                          MOVE.B    #$01,$0030(A0)
0000CAC0                          MOVE.B    #$0B,$0016(A0)
0000CAC6                          LEA       $0000CCF8(pc),A4
0000CACA                          MOVE.B    #$08,$0004(A0)
0000CAD0                          MOVE.B    #$04,$0005(A0)
0000CAD6                          MOVEQ     #$00,D0
0000CAD8                          JSR       $000287D6
0000CADE                          BRA       $0000C870(pc)
0000CAE2                          MOVE.B    #$00,$0030(A0)
0000CAE8                          MOVE.B    #$06,$0016(A0)
0000CAEE                          LEA       $0000CCEE(pc),A4
0000CAF2                          BRA       $0000CB06(pc)
0000CAF6                          MOVE.B    #$01,$0030(A0)
0000CAFC                          MOVE.B    #$0A,$0016(A0)
0000CB02                          LEA       $0000CCF2(pc),A4
0000CB06                          MOVE.B    #$0A,$0004(A0)
0000CB0C                          MOVE.B    #$04,$0005(A0)
0000CB12                          MOVEQ     #$00,D0
0000CB14                          JSR       $000287D6
0000CB1A                          BRA       $0000C870(pc)
0000CB1E                          MOVE.B    #$00,$0030(A0)
0000CB24                          MOVE.B    #$07,$0016(A0)
0000CB2A                          LEA       $0000CCE6(pc),A4
0000CB2E                          BRA       $0000CB42(pc)
0000CB32                          MOVE.B    #$01,$0030(A0)
0000CB38                          MOVE.B    #$09,$0016(A0)
0000CB3E                          LEA       $0000CCEA(pc),A4
0000CB42                          MOVE.B    #$0C,$0004(A0)
0000CB48                          MOVE.B    #$04,$0005(A0)
0000CB4E                          MOVEQ     #$00,D0
0000CB50                          JSR       $000287D6
0000CB56                          BRA       $0000C870(pc)
0000CB5A                          JSR       $00028A66
0000CB60                          BCS       $0000CB8E(pc)
0000CB64                          MOVE.B    #$01,$0017(A1)
0000CB6A                          MOVE.B    #$01,$0000(A1)
0000CB70                          MOVE.B    #$01,$0006(A1)
0000CB76                          MOVE.B    $0030(A0),$0030(A1)
0000CB7C                          MOVE.B    $0026(A0),$0047(A1)
0000CB82                          MOVE.W    $0008(A0),$0008(A1)
0000CB88                          MOVE.W    $000C(A0),$000C(A1)
0000CB8E                          RTS
0000CB90                          MOVE.W    $002A(A0),D0
0000CB94                          EXT.L     D0
0000CB96                          LSL.L     #8,D0
0000CB98                          ADD.L     Invalid
0000CB9A                          OR.B      #$75,A0
0000CB9E                          MOVE.W    $002A(A0),D0
0000CBA2                          EXT.L     D0
0000CBA4                          LSL.L     #8,D0
0000CBA6                          ADD.L     Invalid
0000CBA8                          OR.B      #$28,A0
0000CBAC                          OR.B      #$C0,-$1E78(A4)
0000CBB2                          (CCC     #$0,$000C(A0)
0000CBB6                          SUB.W     #$0020,$002C(A0)
0000CBBC                          RTS
0000CBBE                          ADDQ.B    #$1,$0049(A0)
0000CBC2                          CMP.B     #$5A,$0049(A0)
0000CBC8                          BNE       $0000CC14
0000CBCA                          MOVE.B    #$00,$0049(A0)
0000CBD0                          JSR       $00020DB0
0000CBD6                          BTST      #$00,D0
0000CBDA                          BEQ       $0000CC14
0000CBDC                          BCHG      #$00,$0030(A0)
0000CBE2                          MOVE.B    #$01,$004A(A0)
0000CBE8                          MOVE.B    #$02,$0003(A0)
0000CBEE                          TST.B     $0022(A0)
0000CBF2                          BNE       $0000CC0A
0000CBF4                          MOVE.B    $003A(A0),$0022(A0)
0000CBFA                          ADDQ.W    #$2,$0008(A0)
0000CBFE                          TST.B     $0030(A0)
0000CC02                          BEQ       $0000CC0A(pc)
0000CC06                          SUBQ.W    #$4,$0008(A0)
0000CC0A                          MOVE.B    #$00,$003A(A0)
0000CC10                          BRA       $0000C8F0(pc)
0000CC14                          RTS
0000CC16                          TST.B     $0004(A0)
0000CC1A                          BNE       $0000CC44
0000CC1C                          MOVE.B    $E109,D3
0000CC20                          BTST      #$06,D3
0000CC24                          BNE       $0000CC44
0000CC26                          BTST      #$05,D3
0000CC2A                          BEQ       $0000CC44
0000CC2C                          BTST      #$02,D3
0000CC30                          BNE       $0000CC44
0000CC32                          AND.B     #$03,D3
0000CC36                          BEQ       $0000CC44
0000CC38                          MOVE.B    #$08,$0003(A0)
0000CC3E                          MOVE.W    #$0000,$0004(A0)
0000CC44                          RTS
0000CC46                          OR.B      #$44,D4
0000CC4A                          OR.B      #$CC,D0
0000CC4E                          OR.L      #$01A90145,$01(A0,A2.W)
0000CC56                          BCLR      D0,$00B0(A1)
0000CC5A                          MOVEP.L   D0,$0000(A4)
0000CC5E                          BCLR      D0,-$00B0(A1)
0000CC62                          BCHG      D0,D5
0000CC64                          Invalid
0000CC66                          OR.L      #$FE570000,$FE(A0,D2.L)
0000CC6E                          Invalid
0000CC70                          Invalid
0000CC72                          Invalid
0000CC74                          Invalid
0000CC76                          Invalid
0000CC78                          Invalid
0000CC7A                          Invalid
0000CC7C                          OR.B      #$57,D0
0000CC80                          OR.L      #$FEBB0145,$FF(A0,D0.L)
0000CC88                          BCLR      D0,$0000(A1)
0000CC8C                          MOVEP.L   D0,$00B0(A4)
0000CC90                          BCLR      D0,$0145(A1)
0000CC94                          BCHG      D0,D5
0000CC96                          BCLR      D0,$00B0(A1)
0000CC9A                          MOVEP.L   D0,$0000(A4)
0000CC9E                          BCLR      D0,-$00B0(A1)
0000CCA2                          BCHG      D0,D5
0000CCA4                          Invalid
0000CCA6                          OR.L      #$FE570000,$FE(A0,D2.L)
0000CCAE                          Invalid
0000CCB0                          Invalid
0000CCB2                          Invalid
0000CCB4                          Invalid
0000CCB6                          Invalid
0000CCB8                          Invalid
0000CCBA                          Invalid
0000CCBC                          OR.B      #$57,D0
0000CCC0                          OR.L      #$FEBB0145,$FF(A0,D0.L)
0000CCC8                          BCLR      D0,$0000(A1)
0000CCCC                          MOVE.L    -(A0),D0
0000CCCE                          BCLR      D0,$00(A2,D6.L)
0000CCD2                          BTST      D0,D6
0000CCD4                          BSET      D0,(A4)+
0000CCD6                          BTST      D0,$01(A0,D5.L)
0000CCDA                          AND.B     #$10,D6
0000CCDE                          BCHG      D0,D6
0000CCE0                          BCHG      D0,(A0)
0000CCE2                          AND.B     #$26,(A4)+
0000CCE6                          BCHG      D0,(A4)+
0000CCE8                          BCHG      D0,-(A6)
0000CCEA                          AND.B     #$3C,$01(A2,D1.L)
0000CCF0                          BCHG      D0,#$48
0000CCF4                          AND.W     #$0188,(A2)
0000CCF8                          AND.W     #$0288,(A6)+
0000CCFC                          AND.L     #$02B602A0,(A4)
0000CD02                          Invalid
0000CD04                          Invalid
0000CD06                          Invalid
0000CD08                          Invalid
0000CD0A                          BTST      D1,$034E(A6)
0000CD0E                          Invalid
0000CD10                          MOVEP.W   $036E(A6),D1
0000CD14                          BCHG      D1,$0382
0000CD18                          MOVEP.W   D1,$0398(A4)
0000CD1C                          BCLR      D1,-(A2)
0000CD1E                          BCLR      D1,$03B6(A4)
0000CD22                          Invalid
0000CD24                          OR.B      #$06,A6
0000CD28                          OR.B      #$C2,D0
0000CD2C                          Invalid
0000CD2E                          OR.B      #$06,A3
0000CD32                          OR.B      #$D4,D0
0000CD36                          Invalid
0000CD38                          OR.B      #$06,A6
0000CD3C                          OR.B      #$E6,D0
0000CD40                          Invalid
0000CD42                          OR.B      #$86,A4
0000CD46                          OR.B      #$D4,D0
0000CD4A                          Invalid
0000CD4C                          Invalid
0000CD4E                          OR.B      #$06,A6
0000CD52                          OR.B      #$4C,D0
0000CD56                          Invalid
0000CD58                          OR.B      #$06,A5
0000CD5C                          OR.B      #$6C,D0
0000CD60                          Invalid
0000CD62                          OR.B      #$06,A6
0000CD66                          OR.B      #$88,D0
0000CD6A                          Invalid
0000CD6C                          OR.B      #$86,A5
0000CD70                          OR.B      #$6C,D0
0000CD74                          BTST      D0,D6
0000CD76                          Invalid
0000CD78                          OR.B      #$06,A5
0000CD7C                          OR.B      #$AE,D0
0000CD80                          Invalid
0000CD82                          OR.B      #$99,A2
0000CD86                          OR.B      #$AE,D0
0000CD8A                          BCHG      D1,$00E0
0000CD8E                          OR.B      #$06,A2
0000CD92                          OR.B      #$E6,D0
0000CD96                          Invalid
0000CD98                          OR.B      #$99,A4
0000CD9C                          OR.B      #$E6,D0
0000CDA0                          BCHG      D1,$00E0(A6)
0000CDA4                          OR.B      #$06,A4
0000CDA8                          OR.B      #$02,D0
0000CDAC                          Invalid
0000CDAE                          OR.B      #$99,A5
0000CDB2                          OR.B      #$02,D0
0000CDB6                          BCHG      D0,(A4)+
0000CDB8                          Invalid
0000CDBA                          OR.B      #$06,A4
0000CDBE                          OR.B      #$CA,D0
0000CDC2                          Invalid
0000CDC4                          OR.B      #$99,A5
0000CDC8                          OR.B      #$CA,D0
0000CDCC                          BCHG      D0,$00(A2,D0.W)
0000CDD0                          OR.B      #$06,A5
0000CDD4                          OR.B      #$F8,D0
0000CDD8                          Invalid
0000CDDA                          OR.B      #$06,A5
0000CDDE                          OR.B      #$14,D0
0000CDE2                          Invalid
0000CDE4                          OR.B      #$06,A5
0000CDE8                          OR.B      #$30,D0
0000CDEC                          Invalid
0000CDEE                          OR.B      #$86,A5
0000CDF2                          OR.B      #$14,D0
0000CDF6                          MOVEP.W   D0,$00E0(A0)
0000CDFA                          OR.B      #$06,A6
0000CDFE                          OR.B      #$C2,-(A0)
0000CE02                          Invalid
0000CE04                          OR.B      #$06,A3
0000CE08                          OR.B      #$D4,-(A0)
0000CE0C                          Invalid
0000CE0E                          OR.B      #$06,A6
0000CE12                          OR.B      #$E6,-(A0)
0000CE16                          Invalid
0000CE18                          OR.B      #$86,A4
0000CE1C                          OR.B      #$D4,-(A0)
0000CE20                          BCLR      D0,$00(A2,D0.W)
0000CE24                          OR.B      #$06,A6
0000CE28                          OR.B      #$4C,-(A0)
0000CE2C                          Invalid
0000CE2E                          OR.B      #$06,A5
0000CE32                          OR.B      #$6C,-(A0)
0000CE36                          Invalid
0000CE38                          OR.B      #$06,A6
0000CE3C                          OR.B      #$88,-(A0)
0000CE40                          Invalid
0000CE42                          OR.B      #$86,A5
0000CE46                          OR.B      #$6C,-(A0)
0000CE4A                          BSET      D0,(A4)+
0000CE4C                          Invalid
0000CE4E                          OR.B      #$06,A5
0000CE52                          OR.B      #$AE,-(A0)
0000CE56                          Invalid
0000CE58                          OR.B      #$99,A2
0000CE5C                          OR.B      #$AE,-(A0)
0000CE60                          BCLR      D1,-(A2)
0000CE62                          Invalid
0000CE64                          OR.B      #$06,A3
0000CE68                          OR.B      #$E6,-(A0)
0000CE6C                          Invalid
0000CE6E                          OR.B      #$99,A4
0000CE72                          OR.B      #$E6,-(A0)
0000CE76                          BCLR      D1,(A0)+
0000CE78                          Invalid
0000CE7A                          OR.B      #$06,A4
0000CE7E                          OR.B      #$02,-(A0)
0000CE82                          Invalid
0000CE84                          OR.B      #$99,A5
0000CE88                          OR.B      #$02,-(A0)
0000CE8C                          AND.B     #$E0,$00(A2,D6.W)
0000CE92                          OR.B      #$20,D6
0000CE96                          Invalid
0000CE98                          Invalid
0000CE9A                          OR.B      #$99,A5
0000CE9E                          OR.B      #$CA,-(A0)
0000CEA2                          AND.W     #$00E0,A0
0000CEA6                          OR.B      #$06,A5
0000CEAA                          OR.B      #$F8,-(A0)
0000CEAE                          Invalid
0000CEB0                          OR.B      #$06,A5
0000CEB4                          OR.B      #$14,-(A0)
0000CEB8                          Invalid
0000CEBA                          OR.B      #$06,A5
0000CEBE                          OR.B      #$30,-(A0)
0000CEC2                          Invalid
0000CEC4                          OR.B      #$86,A5
0000CEC8                          OR.B      #$14,-(A0)
0000CECC                          AND.W     #$00E0,(A6)+
0000CED0                          OR.B      #$9E,A7
0000CED4                          OR.B      #$38,D0
0000CED8                          AND.L     #$00E0000F,A0
0000CEDE                          OR.L      #$00200538,(A6)+
0000CEE4                          AND.L     #$00E0000D,(A4)
0000CEEA                          OR.B      #$00,D6
0000CEEE                          BTST      D2,-(A2)
0000CEF0                          Invalid
0000CEF2                          OR.B      #$9E,A2
0000CEF6                          OR.B      #$22,D0
0000CEFA                          BCHG      D1,$00E0
0000CEFE                          OR.B      #$06,A5
0000CF02                          OR.B      #$22,-(A0)
0000CF06                          Invalid
0000CF08                          OR.B      #$9E,A2
0000CF0C                          OR.B      #$22,-(A0)
0000CF10                          BCLR      D1,-(A2)
0000CF12                          Invalid
0000CF14                          OR.B      #$06,A2
0000CF18                          OR.B      #$58,D0
0000CF1C                          Invalid
0000CF1E                          OR.B      #$99,A4
0000CF22                          OR.B      #$58,D0
0000CF26                          BCHG      D1,$00E0(A6)
0000CF2A                          OR.B      #$0A,A3
0000CF2E                          OR.B      #$58,-(A0)
0000CF32                          Invalid
0000CF34                          OR.B      #$9E,A4
0000CF38                          OR.B      #$58,-(A0)
0000CF3C                          BCLR      D1,(A0)+
0000CF3E                          Invalid
0000CF40                          OR.B      #$0A,A4
0000CF44                          OR.B      #$58,D0
0000CF48                          Invalid
0000CF4A                          OR.B      #$9E,A5
0000CF4E                          OR.B      #$58,D0
0000CF52                          BCHG      D1,$00E0
0000CF56                          OR.B      #$06,A4
0000CF5A                          OR.B      #$58,-(A0)
0000CF5E                          Invalid
0000CF60                          OR.B      #$19,A5
0000CF64                          OR.B      #$58,-(A0)
0000CF68                          Invalid
0000CF6A                          OR.B      #$86,A4
0000CF6E                          OR.B      #$58,-(A0)
0000CF72                          BTST      D1,-(A2)
0000CF74                          Invalid
0000CF76                          OR.B      #$0A,A4
0000CF7A                          OR.B      #$58,D0
0000CF7E                          Invalid
0000CF80                          OR.B      #$1E,A5
0000CF84                          OR.B      #$58,D0
0000CF88                          Invalid
0000CF8A                          OR.B      #$F8,A4
0000CF8E                          OR.B      #$58,D0
0000CF92                          BCHG      D1,D2
0000CF94                          Invalid
0000CF96                          OR.B      #$06,A4
0000CF9A                          OR.B      #$58,-(A0)
0000CF9E                          Invalid
0000CFA0                          OR.B      #$19,A5
0000CFA4                          OR.B      #$58,-(A0)
0000CFA8                          Invalid
0000CFAA                          OR.B      #$86,A4
0000CFAE                          OR.B      #$58,-(A0)
0000CFB2                          BCHG      D1,-(A2)
0000CFB4                          Invalid
0000CFB6                          OR.B      #$19,A2
0000CFBA                          OR.B      #$22,D0
0000CFBE                          Invalid
0000CFC0                          MOVEP.W   $0019(A7),D0
0000CFC4                          OR.B      #$38,D0
0000CFC8                          Invalid
0000CFCA                          AND.B     #$19,A5
0000CFCE                          OR.B      #$58,D0
0000CFD2                          Invalid
0000CFD4                          MOVEP.W   $009E(A5),D1
0000CFD8                          OR.B      #$58,D0
0000CFDC                          MOVEP.W   D1,$00E0(A4)
0000CFE0                          OR.B      #$19,A2
0000CFE4                          OR.B      #$22,-(A0)
0000CFE8                          Invalid
0000CFEA                          MOVEP.W   $0019(A7),D0
0000CFEE                          OR.B      #$38,-(A0)
0000CFF2                          Invalid
0000CFF4                          AND.B     #$19,A5
0000CFF8                          OR.B      #$58,-(A0)
0000CFFC                          Invalid
0000CFFE                          MOVEP.W   $0099(A5),D1
0000D002                          OR.B      #$58,-(A0)
0000D006                          BCLR      D1,$02(A6,D0.W)
0000D00A                          Invalid
0000D00C                          OR.B      #$F0,D0
0000D010                          BTST      D7,D0
0000D012                          Invalid
0000D014                          Invalid
0000D016                          OR.B      #$00,(A6)
0000D01A                          AND.B     #$0B,D1
0000D01E                          OR.B      #$F0,D0
0000D022                          ADD.B     #$F4,D0
0000D026                          Invalid
0000D028                          OR.B      #$00,(A2)
0000D02C                          AND.B     #$06,D2
0000D030                          OR.B      #$F0,D0
0000D034                          BTST      D7,D0
0000D036                          Invalid
0000D038                          Invalid
0000D03A                          OR.B      #$00,(A6)
0000D03E                          SUB.B     #$06,D3
0000D042                          OR.B      #$E8,D0
0000D046                          BTST      D7,D0
0000D048                          Invalid
0000D04A                          BTST      #$16,D4
0000D04E                          Invalid
0000D050                          BTST      D7,D0
0000D052                          MOVE.B    A0,D4
0000D054                          MOVE.B    A0,D0
0000D056                          OR.B      #$00,$0404(A0)
0000D05C                          LSR.B     #4,D6
0000D05E                          OR.B      #$00,D0
0000D062                          SUB.B     #$DC,D0
0000D066                          Invalid
0000D068                          OR.B      #$F8,A6
0000D06C                          ADD.B     #$14,D0
0000D070                          MOVE.B    A0,D0
0000D072                          OR.B      #$00,-(A0)
0000D076                          SUB.B     #$06,D5
0000D07A                          OR.B      #$E8,D0
0000D07E                          BTST      D7,D0
0000D080                          Invalid
0000D082                          BTST      #$16,D4
0000D086                          Invalid
0000D088                          BTST      D7,D0
0000D08A                          MOVE.B    A0,D4
0000D08C                          MOVE.B    A0,D0
0000D08E                          OR.B      #$00,$0506(A0)
0000D094                          SUB.B     #$00,D1
0000D098                          ADD.B     (A4),D6
0000D09A                          BTST      #$DC,D0
0000D09E                          Invalid
0000D0A0                          OR.B      #$E4,D5
0000D0A4                          BTST      D2,D0
0000D0A6                          MOVE.B    $E4(A4,A7.W),$0000D0C1(pc)
0000D0AC                          SUB.B     #$00,D4
0000D0B0                          MOVE.L    D4,-(A4)
0000D0B2                          SUB.B     #$0A,D7
0000D0B6                          OR.B      #$10,D0
0000D0BA                          BTST      D6,D0
0000D0BC                          BSET      D4,-(A0)
0000D0BE                          Invalid
0000D0C0                          OR.B      #$E8,(A1)
0000D0C4                          BTST      #$08,D0
0000D0C8                          Invalid
0000D0CA                          OR.B      #$00,(A2)+
0000D0CE                          ADD.B     #$0A,A0
0000D0D2                          OR.B      #$F4,D0
0000D0D6                          BTST      D6,D0
0000D0D8                          BSET      D4,-(A0)
0000D0DA                          SUB.B     #$11,A4
0000D0DE                          Invalid
0000D0E0                          SUB.B     #$F0,D0
0000D0E4                          ASR.B     #2,D5
0000D0E6                          OR.B      #$E4,(A7)
0000D0EA                          BTST      D7,D0
0000D0EC                          MOVE.B    D0,-(A5)
0000D0EE                          Invalid
0000D0F0                          OR.B      #$00,$0409(A3)
0000D0F6                          SUB.B     #$00,A6
0000D0FA                          ASR       -(A4)
0000D0FC                          BTST      D7,D0
0000D0FE                          Invalid
0000D100                          SUB.B     #$1C,D4
0000D104                          Invalid
0000D106                          BTST      D0,D0
0000D108                          MOVE.B    -$1BFA(A0),(A7)+
0000D10C                          OR.B      #$00,-(A0)
0000D110                          SUB.B     #$0B,A2
0000D114                          OR.B      #$E8,D0
0000D118                          SUB.B     #$FC,D0
0000D11C                          OR.B      #$0E,D1
0000D120                          Invalid
0000D122                          BTST      D7,D0
0000D124                          MOVE.B    #$09,(A0)+
0000D128                          OR.B      #$00,-(A0)
0000D12C                          SUB.B     #$08,A3
0000D130                          OR.B      #$F8,D0
0000D134                          BTST      D7,D0
0000D136                          BSET      D1,-$17FB(A0)
0000D13A                          OR.B      #$E0,(A3)
0000D13E                          .B        #$F8,D0
0000D142                          Invalid
0000D144                          OR.B      #$00,-(A3)
0000D148                          MOVEP.W   -$2400(A4),D2
0000D14C                          OR.B      #$E4,D0
0000D150                          .B        #$E8,D0
0000D154                          ASR.B     #6,D5
0000D156                          OR.B      #$FC,A5
0000D15A                          BTST      D0,D0
0000D15C                          MOVE.B    $FC(A0,A7.W),$0013
0000D162                          Invalid
0000D164                          OR.B      #$00,D0
0000D168                          MOVEP.W   $0806(A5),D1
0000D16C                          OR.B      #$F8,D0
0000D170                          BTST      D6,D0
0000D172                          Invalid
0000D174                          ASR.B     #8,D6
0000D176                          OR.B      #$E0,A6
0000D17A                          .B        #$00,D0
0000D17E                          MOVEP.W   -$03F6(A6),D2
0000D182                          OR.B      #$F4,D0
0000D186                          OR.B      #$D8,D0
0000D18A                          Invalid
0000D18C                          OR.B      #$EC,A2
0000D190                          EOR.B     #$F8,D0
0000D194                          ASR.B     #6,D4
0000D196                          OR.B      #$F4,(A4)+
0000D19A                          .B        #$10,D0
0000D19E                          MOVEP.W   -$1FF7(A7),D1
0000D1A2                          OR.B      #$E0,D0
0000D1A6                          BTST      D3,D0
0000D1A8                          Invalid
0000D1AA                          Invalid
0000D1AC                          OR.B      #$00,A6
0000D1B0                          BTST      D5,D0
0000D1B2                          MOVE.B    A0,D2
0000D1B4                          MOVE.L    $001A(A0),A6
0000D1B8                          LEA       $A000,A1
0000D1BC                          MOVEQ     #$00,D0
0000D1BE                          MOVE.B    $0002(A0),D0
0000D1C2                          MOVE.W    +$00(D3.W),D1
0000D1C6                          JMP       +$10(D1.W)
0000D1CA                          OR.B      #$44,A0
0000D1CE                          MOVEP.L   $0150(A0),D0
0000D1D2                          MOVE.B    #$02,$0002(A0)
0000D1D8                          LEA       $0000D380(pc),A6
0000D1DC                          MOVE.L    A6,$001A(A0)
0000D1E0                          MOVE.W    #$0014,$0028(A0)
0000D1E6                          LEA       $0000ADA0(pc),A2
0000D1EA                          MOVE.W    $0000(A2),$0018(A0)
0000D1F0                          MOVE.W    $0008(A0),$0036(A0)
0000D1F6                          MOVE.W    $000C(A0),$003C(A0)
0000D1FC                          MOVE.W    #$0060,$003E(A0)
0000D202                          MOVEQ     #$00,D0
0000D204                          LEA       $0000D398(pc),A4
0000D208                          JMP       $000287D6
0000D20E                          BSR       $0000D222
0000D210                          TST.B     $0026(A0)
0000D214                          BNE       $0000D21C
0000D216                          JMP       $00028C60
0000D21C                          JMP       $00028C76
0000D222                          TST.B     $0003(A0)
0000D226                          BNE       $0000D268
0000D228                          LEA       $0000D320(pc),A2
0000D22C                          MOVEQ     #$00,D0
0000D22E                          MOVE.B    #$00,$0031(A0)
0000D234                          MOVE.B    (A2)+,$0016(A0)
0000D238                          MOVE.B    (A2)+,$0030(A0)
0000D23C                          MOVE.B    (A2)+,$0032(A0)
0000D240                          MOVE.B    (A2)+,D0
0000D242                          MOVE.L    A2,$0038(A0)
0000D246                          LEA       $0000ADA0(pc),A2
0000D24A                          ADD.W     D0,D0
0000D24C                          MOVE.W    $00(A2,D0.W),$0018(A0)
0000D252                          MOVE.B    #$02,$0003(A0)
0000D258                          MOVEQ     #$00,D0
0000D25A                          MOVE.B    $0032(A0),D0
0000D25E                          LEA       $0000D398(pc),A4
0000D262                          JMP       $000287D6
0000D268                          BSR       $0000D294
0000D26A                          BSR       $0000D272
0000D26C                          JMP       $00028842
0000D272                          MOVE.W    $003E(A0),D0
0000D276                          BEQ       $0000D292
0000D278                          SUBQ.W    #$1,$000C(A0)
0000D27C                          SUBQ.W    #$1,$003C(A0)
0000D280                          SUBQ.W    #$1,$003E(A0)
0000D284                          BEQ       $0000D292
0000D286                          SUBQ.W    #$1,$000C(A0)
0000D28A                          SUBQ.W    #$1,$003C(A0)
0000D28E                          SUBQ.W    #$1,$003E(A0)
0000D292                          RTS
0000D294                          SUBQ.B    #$1,$0030(A0)
0000D298                          BNE       $0000D310
0000D29A                          ADDQ.B    #$1,$0031(A0)
0000D29E                          CMP.B     #$17,$0031(A0)
0000D2A4                          BLS       $0000D2CC
0000D2A6                          MOVE.W    $0036(A0),$0008(A0)
0000D2AC                          MOVE.W    $003C(A0),$000C(A0)
0000D2B2                          ADD.W     #$0018,$000C(A0)
0000D2B8                          MOVE.B    #$01,$0034(A0)
0000D2BE                          MOVE.B    #$00,$0031(A0)
0000D2C4                          MOVE.L    #$0000D320,$0038(A0)
0000D2CC                          MOVEQ     #$00,D0
0000D2CE                          MOVE.L    $0038(A0),A2
0000D2D2                          MOVE.B    (A2)+,$0016(A0)
0000D2D6                          MOVE.B    (A2)+,$0030(A0)
0000D2DA                          MOVE.B    (A2)+,$0033(A0)
0000D2DE                          MOVE.B    (A2)+,D0
0000D2E0                          MOVE.L    A2,$0038(A0)
0000D2E4                          ADD.W     D0,D0
0000D2E6                          LEA       $0000ADA0(pc),A2
0000D2EA                          MOVE.W    $00(A2,D0.W),$0018(A0)
0000D2F0                          MOVE.B    $0032(A0),D0
0000D2F4                          CMP.B     $0033(A0),D0
0000D2F8                          BEQ       $0000D310
0000D2FA                          MOVE.B    $0033(A0),$0032(A0)
0000D300                          MOVEQ     #$00,D0
0000D302                          MOVE.B    $0032(A0),D0
0000D306                          LEA       $0000D398(pc),A4
0000D30A                          JMP       $000287D6
0000D310                          RTS
0000D312                          MOVE.B    #$06,$0002(A0)
0000D318                          RTS
0000D31A                          JMP       $00028B48
0000D320                          MOVE.B    (A5),-(A0)
0000D322                          OR.B      #$08,D0
0000D326                          OR.B      #$07,D1
0000D32A                          BTST      D0,D2
0000D32C                          MOVE.B    D6,D3
0000D32E                          BTST      D0,D3
0000D330                          MOVE.B    A0,D3
0000D332                          BTST      D0,D4
0000D334                          MOVE.B    A4,-(A5)
0000D336                          AND.B     #$0C,D5
0000D33A                          AND.B     #$06,D5
0000D33E                          AND.B     #$08,D4
0000D342                          BTST      D1,D3
0000D344                          MOVE.L    D6,-(A6)
0000D346                          BTST      D1,D2
0000D348                          MOVE.L    A1,D7
0000D34A                          SUB.B     #$15,D1
0000D34E                          SUB.B     #$15,D0
0000D352                          SUB.B     #$08,D0
0000D356                          SUB.B     #$07,D1
0000D35A                          BTST      D2,D2
0000D35C                          MOVE.W    D7,-(A2)
0000D35E                          BTST      D2,D3
0000D360                          MOVE.W    A0,-(A3)
0000D362                          BTST      D2,D4
0000D364                          MOVE.W    A4,-(A5)
0000D366                          ADD.B     #$0C,D5
0000D36A                          ADD.B     #$06,D5
0000D36E                          ADD.B     #$05,D4
0000D372                          BTST      D3,D3
0000D374                          CMP.B     #$02,D6
0000D378                          .B        #$01,A1
0000D37C                          BTST      D7,(A5)
0000D37E                          OR.B      #$00,D0
0000D382                          BCHG      D1,D0
0000D384                          OR.B      #$40,D0
0000D388                          OR.B      #$A0,D0
0000D38C                          OR.B      #$40,D0
0000D390                          OR.B      #$40,D0
0000D394                          OR.B      #$0C,D0
0000D398                          OR.B      #$34,$0040(A0)
0000D39E                          OR.W      #$0058,A4
0000D3A2                          OR.W      #$0070,-(A4)
0000D3A6                          OR.W      #$0436,SR
0000D3AA                          OR.B      #$F8,D7
0000D3AE                          OR.B      #$88,D0
0000D3B2                          OR.B      #$36,$0007(A0)
0000D3B8                          Invalid
0000D3BA                          OR.B      #$88,D0
0000D3BE                          OR.B      #$36,$00(A4,D3.W)
0000D3C4                          Invalid
0000D3C6                          OR.B      #$9E,D0
0000D3CA                          OR.W      #$0401,D0
0000D3CE                          BTST      D0,D4
0000D3D0                          DIVU      $0000,D0
0000D3D4                          OR.L      #$004C0401,$01(A0,D2.W)
0000D3DC                          DIVU      $0000,D0
0000D3E0                          OR.L      #$00580431,SR
0000D3E6                          OR.B      #$F8,D5
0000D3EA                          OR.B      #$C4,D0
0000D3EE                          OR.W      #$0444,-(A4)
0000D3F2                          OR.B      #$F8,D5
0000D3F6                          OR.B      #$D0,D0
0000D3FA                          OR.W      #$0402,$00(A0,D3.W)
0000D400                          Invalid
0000D402                          OR.B      #$D0,D0
0000D406                          OR.W      #$0300,SR
0000D40A                          ASR.B     #6,D5
0000D40C                          OR.B      #$FC,D0
0000D410                          BTST      D4,D0
0000D412                          Invalid
0000D414                          Invalid
0000D416                          OR.B      #$0C,A2
0000D41A                          SUB.B     #$F8,D0
0000D41E                          AND.B     #$0D,D0
0000D422                          OR.B      #$F0,(A2)
0000D426                          OR.B      #$08,D0
0000D42A                          BTST      #$1B,A0
0000D42E                          Invalid
0000D430                          BTST      D0,D0
0000D432                          Invalid
0000D434                          OR.B      #$00,(A6)+
0000D438                          CMP.B     #$F0,D0
0000D43C                          OR.B      #$05,D0
0000D440                          OR.B      #$00,-(A4)
0000D444                          BTST      D0,D0
0000D446                          Invalid
0000D448                          OR.B      #$FC,$0900(A0)
0000D44E                          MOVE.L    $0200,(A5)+
0000D452                          Invalid
0000D454                          OR.B      #$F8,$0D(A0,D0.W)
0000D45A                          MOVE.W    $08(A0,D2.W),(A1)+
0000D45E                          OR.B      #$00,$00014460(pc)
0000D464                          MOVE.B    $0002(A0),D0
0000D468                          MOVE.W    +$00(D3.W),D1
0000D46C                          JMP       +$10(D1.W)
0000D470                          OR.B      #$48,(A6)+
0000D474                          OR.W      #$0062,(A2)+
0000D478                          MOVEQ     #$00,D0
0000D47A                          MOVE.B    $0002(A0),D0
0000D47E                          MOVE.W    +$00(D3.W),D1
0000D482                          JMP       +$10(D1.W)
0000D486                          OR.B      #$32,A6
0000D48A                          OR.W      #$004C,D4
0000D48E                          LEA       $0000D4F0(pc),A4
0000D492                          BRA       $0000D498
0000D494                          LEA       $0000D4EC(pc),A4
0000D498                          LEA       $0000D4D8(pc),A6
0000D49C                          MOVE.L    A6,$001A(A0)
0000D4A0                          MOVE.B    #$02,$0002(A0)
0000D4A6                          MOVE.B    #$10,$0036(A0)
0000D4AC                          MOVEQ     #$00,D0
0000D4AE                          MOVE.B    $001F(A0),D0
0000D4B2                          JMP       $000287D6
0000D4B8                          SUBQ.B    #$1,$0036(A0)
0000D4BC                          BEQ       $0000D4CA
0000D4BE                          JSR       $00028804
0000D4C4                          JMP       $00028BE2
0000D4CA                          MOVE.B    #$06,$0002(A0)
0000D4D0                          RTS
0000D4D2                          JMP       $00028B08
0000D4D8                          OR.B      #$01,D0
0000D4DC                          OR.B      #$01,D0
0000D4E0                          OR.B      #$01,D0
0000D4E4                          OR.B      #$01,D0
0000D4E8                          OR.B      #$01,D0
0000D4EC                          OR.B      #$28,(A4)+
0000D4F0                          OR.B      #$40,$08(A4,D1.W)
0000D4F6                          OR.B      #$F8,D4
0000D4FA                          OR.B      #$4C,D0
0000D4FE                          OR.B      #$02,(A4)+
0000D502                          OR.B      #$F8,D4
0000D506                          OR.B      #$4C,-(A0)
0000D50A                          OR.B      #$02,$0004(A0)
0000D510                          DIVU      $0020,D0
0000D514                          OR.W      #$0034,(A0)+
0000D518                          BTST      #$04,D2
0000D51C                          DIVU      $0000,D0
0000D520                          OR.W      #$0040,(A0)+
0000D524                          BTST      D0,D0
0000D526                          Invalid
0000D528                          OR.L      #$F8000510,-(A6)
0000D52E                          Invalid
0000D530                          BTST      D0,D0
0000D532                          Invalid
0000D534                          OR.L      #$F8000510,$B0(A0,D4.L)
0000D53C                          MOVE.L    $001A(A0),A6
0000D540                          MOVEQ     #$00,D0
0000D542                          MOVE.B    $0002(A0),D0
0000D546                          MOVE.W    +$00(D3.W),D1
0000D54A                          JMP       +$10(D1.W)
0000D54E                          OR.B      #$42,A0
0000D552                          BTST      D0,-(A0)
0000D554                          BTST      D0,$4DF9(A0)
0000D558                          OR.B      #$7C,D0
0000D55C                          MOVE.L    A6,$001A(A0)
0000D560                          LEA       $0000D690,A3
0000D566                          MOVE.W    $0006(A3),$0018(A0)
0000D56C                          MOVE.B    #$02,$0002(A0)
0000D572                          MOVE.B    #$06,$0003(A0)
0000D578                          MOVE.W    #$008C,$0030(A0)
0000D57E                          MOVE.B    #$05,$0016(A0)
0000D584                          MOVEQ     #$00,D0
0000D586                          LEA       $0000D898(pc),A4
0000D58A                          JMP       $00007E40
0000D590                          MOVEQ     #$00,D0
0000D592                          MOVE.B    $0003(A0),D0
0000D596                          MOVE.W    +$00(D6.W),D0
0000D59A                          JSR       +$00(D4.W)
0000D59E                          JMP       $00028CEE
0000D5A4                          OR.B      #$72,A0
0000D5A8                          OR.L      #$009E5368,(A2)
0000D5AE                          OR.B      #$2C,$4A(A0,D4.L)
0000D5B4                          Invalid
0000D5B6                          BNE       $0000D5DE
0000D5B8                          CMP.W     #$0064,$0030(A0)
0000D5BE                          BNE       $0000D5D2
0000D5C0                          ADD.W     #$0024,$000C(A0)
0000D5C6                          MOVEQ     #$00,D0
0000D5C8                          LEA       $0000D89A,A4
0000D5CE                          JSR       $00007E40(pc)
0000D5D2                          JSR       $00028870
0000D5D8                          ADDQ.W    #$4,$0008(A0)
0000D5DC                          RTS
0000D5DE                          MOVE.B    #$02,$0003(A0)
0000D5E4                          MOVE.B    #$12,$0032(A0)
0000D5EA                          LEA       $0000D690,A3
0000D5F0                          MOVE.W    $0002(A3),$0018(A0)
0000D5F6                          MOVE.B    #$08,$0016(A0)
0000D5FC                          SUB.W     #$0038,$000C(A0)
0000D602                          MOVEQ     #$00,D0
0000D604                          LEA       $0000D89C,A4
0000D60A                          JSR       $00007E40
0000D610                          JMP       $00028870
0000D616                          SUBQ.B    #$1,$0032(A0)
0000D61A                          BNE       $0000D610
0000D61C                          MOVE.B    #$12,$0032(A0)
0000D622                          SUBQ.B    #$1,$0016(A0)
0000D626                          CMP.B     #$06,$0016(A0)
0000D62C                          BNE       $0000D610
0000D62E                          MOVE.B    #$04,$0003(A0)
0000D634                          BRA       $0000D610
0000D636                          JSR       $00028870
0000D63C                          JMP       $0002889E
0000D642                          SUBQ.W    #$1,$0030(A0)
0000D646                          BNE       $0000D66C
0000D648                          MOVE.W    $EA08,$0008(A0)
0000D64E                          MOVE.W    $EA0C,$000C(A0)
0000D654                          ADD.W     #$00E0,$000C(A0)
0000D65A                          MOVE.W    #$0100,$0030(A0)
0000D660                          MOVE.B    #$01,$0001(A0)
0000D666                          MOVE.B    #$00,$0003(A0)
0000D66C                          RTS
0000D66E                          MOVE.B    #$06,$0002(A0)
0000D674                          RTS
0000D676                          JMP       $00028AD0
0000D67C                          OR.B      #$00,D0
0000D680                          OR.B      #$00,D0
0000D684                          OR.B      #$00,D0
0000D688                          OR.B      #$00,D0
0000D68C                          OR.B      #$00,D0
0000D690                          OR.B      #$9C,(A4)+
0000D694                          BTST      D0,(A4)+
0000D696                          BCLR      D0,(A4)+
0000D698                          OR.B      #$80,D0
0000D69C                          OR.B      #$7D,(A0)+
0000D6A0                          OR.B      #$76,$00(A0,A3.W)
0000D6A6                          OR.W      #$005A,$005A(A2)
0000D6AC                          OR.W      #$0047,$0076(A2)
0000D6B2                          OR.B      #$7D,$00(A0,D4.L)
0000D6B8                          OR.L      #$0000007D,D0
0000D6BE                          Invalid
0000D6C0                          OR.W      #$FFD0,$00(A6,D5.W)
0000D6C6                          Invalid
0000D6C8                          OR.W      #$FFA6,(A2)+
0000D6CC                          OR.W      #$FF96,D7
0000D6D0                          OR.B      #$8A,$00(A0,D4.L)
0000D6D6                          Invalid
0000D6D8                          OR.B      #$80,D0
0000D6DC                          Invalid
0000D6DE                          Invalid
0000D6E0                          Invalid
0000D6E2                          Invalid
0000D6E4                          Invalid
0000D6E6                          Invalid
0000D6E8                          Invalid
0000D6EA                          Invalid
0000D6EC                          Invalid
0000D6EE                          Invalid
0000D6F0                          Invalid
0000D6F2                          Invalid
0000D6F4                          Invalid
0000D6F6                          Invalid
0000D6F8                          Invalid
0000D6FA                          OR.B      #$83,D0
0000D6FE                          OR.B      #$8A,(A0)+
0000D702                          OR.B      #$96,$00(A0,A3.W)
0000D708                          Invalid
0000D70A                          OR.W      #$FFB9,(A2)+
0000D70E                          OR.W      #$FFD0,$0076(A2)
0000D714                          Invalid
0000D716                          OR.W      #$0000,
0000D71A                          BTST      D2,D0
0000D71C                          Invalid
0000D71E                          Invalid
0000D720                          BSET      D0,$049D(A0)
0000D724                          Invalid
0000D726                          SUB.B     #$88,-(A7)
0000D72A                          MOVEP.W   D1,$0427(A0)
0000D72E                          Invalid
0000D730                          SUB.L     #$01E804E6,(A5)+
0000D736                          Invalid
0000D738                          BTST      D2,D0
0000D73A                          OR.B      #$E6,D0
0000D73E                          Invalid
0000D740                          SUB.L     #$FE180427,(A5)+
0000D746                          Invalid
0000D748                          MOVEP.W   D1,-$0388(A0)
0000D74C                          Invalid
0000D74E                          Invalid
0000D750                          BSET      D0,-$049D(A0)
0000D754                          Invalid
0000D756                          Invalid
0000D758                          OR.B      #$00,D0
0000D75C                          Invalid
0000D75E                          Invalid
0000D760                          Invalid
0000D762                          Invalid
0000D764                          Invalid
0000D766                          Invalid
0000D768                          Invalid
0000D76A                          Invalid
0000D76C                          Invalid
0000D76E                          Invalid
0000D770                          Invalid
0000D772                          Invalid
0000D774                          Invalid
0000D776                          Invalid
0000D778                          Invalid
0000D77A                          OR.B      #$1A,D0
0000D77E                          Invalid
0000D780                          Invalid
0000D782                          BSET      D0,-$0427(A0)
0000D786                          Invalid
0000D788                          Invalid
0000D78A                          MOVEP.W   D1,-$02C6(A0)
0000D78E                          SUB.B     #$18,-(A7)
0000D792                          SUB.L     #$FF0704E6,(A5)+
0000D798                          OR.B      #$00,D0
0000D79C                          OR.W      #$01F5,-(A3)
0000D7A0                          Invalid
0000D7A2                          BSET      D0,(A0)+
0000D7A4                          BTST      D0,(A4)+
0000D7A6                          BCLR      D0,$0169(A1)
0000D7AA                          BCHG      D0,$01A9(A1)
0000D7AE                          BTST      D0,(A4)+
0000D7B0                          BSET      D0,(A0)+
0000D7B2                          Invalid
0000D7B4                          BSET      D0,$00(A5,A1.W)
0000D7B8                          AND.B     #$00,D0
0000D7BC                          BSET      D0,$FF(A5,A6.L)
0000D7C0                          BSET      D0,(A0)+
0000D7C2                          Invalid
0000D7C4                          BCLR      D0,-$011C(A1)
0000D7C8                          BCHG      D0,-$0169(A1)
0000D7CC                          BTST      D0,(A4)+
0000D7CE                          Invalid
0000D7D0                          Invalid
0000D7D2                          Invalid
0000D7D4                          OR.W      #$FE0B,-(A3)
0000D7D8                          OR.B      #$00,D0
0000D7DC                          Invalid
0000D7DE                          Invalid
0000D7E0                          Invalid
0000D7E2                          Invalid
0000D7E4                          Invalid
0000D7E6                          Invalid
0000D7E8                          Invalid
0000D7EA                          Invalid
0000D7EC                          Invalid
0000D7EE                          Invalid
0000D7F0                          Invalid
0000D7F2                          Invalid
0000D7F4                          Invalid
0000D7F6                          Invalid
0000D7F8                          Invalid
0000D7FA                          OR.B      #$0B,D0
0000D7FE                          OR.W      #$FE28,-(A3)
0000D802                          Invalid
0000D804                          Invalid
0000D806                          BTST      D0,(A4)+
0000D808                          Invalid
0000D80A                          BCHG      D0,-$011C(A1)
0000D80E                          BCLR      D0,-$00C3(A1)
0000D812                          BSET      D0,(A0)+
0000D814                          Invalid
0000D816                          BSET      D0,$00(A5,D0.W)
0000D81A                          BTST      D0,D0
0000D81C                          OR.B      #$FA,$00(A1,A0.W)
0000D822                          Invalid
0000D824                          OR.L      #$00D400B4,A6
0000D82A                          OR.L      #$00D4008E,$00(A4,D6.W)
0000D832                          OR.W      #$00FA,-(A1)
0000D836                          OR.B      #$00,$00(A1,D0.W)
0000D83C                          Invalid
0000D83E                          Invalid
0000D840                          Invalid
0000D842                          Invalid
0000D844                          Invalid
0000D846                          Invalid
0000D848                          OR.L      #$FF4C008E,$FF(A4,D6.W)
0000D850                          OR.W      #$FF14,-(A1)
0000D854                          OR.B      #$06,$00(A1,D0.W)
0000D85A                          Invalid
0000D85C                          Invalid
0000D85E                          Invalid
0000D860                          Invalid
0000D862                          Invalid
0000D864                          Invalid
0000D866                          Invalid
0000D868                          Invalid
0000D86A                          Invalid
0000D86C                          Invalid
0000D86E                          Invalid
0000D870                          Invalid
0000D872                          Invalid
0000D874                          Invalid
0000D876                          Invalid
0000D878                          Invalid
0000D87A                          OR.B      #$06,D0
0000D87E                          OR.B      #$14,$00(A1,A0.W)
0000D884                          Invalid
0000D886                          OR.L      #$FF4C00B4,A6
0000D88C                          Invalid
0000D88E                          Invalid
0000D890                          Invalid
0000D892                          Invalid
0000D894                          Invalid
0000D896                          Invalid
0000D898                          AND.B     #$28,-(A2)
0000D89C                          AND.B     #$34,$00F8(A6)
0000D8A2                          AND.B     #$44,-(A2)
0000D8A6                          Invalid
0000D8A8                          AND.B     #$54,$00F8(A0)
0000D8AE                          AND.B     #$1C,-$8000(A6)
0000D8B4                          AND.W     #$0000,-(A4)
0000D8B8                          OR.B      #$F0,D4
0000D8BC                          AND.L     #$0000001C,-(A2)
0000D8C2                          OR.B      D0,D0
0000D8C4                          AND.W     #$0000,-(A4)
0000D8C8                          OR.B      #$D4,D0
0000D8CC                          AND.L     #$0000001C,$8000
0000D8D4                          AND.W     #$0000,-(A4)
0000D8D8                          OR.B      #$00,D0
0000D8DC                          Invalid
0000D8DE                          OR.B      #$00,D0
0000D8E2                          LSR.B     #2,D0
0000D8E4                          OR.B      #$EC,D1
0000D8E8                          .B        #$C0,D0
0000D8EC                          SUB.B     #$10,D4
0000D8F0                          ADD.W     $0D(A4,D0.W),A4
0000D8F4                          MOVE.B    -(A0),(A1)+
0000D8F6                          LSR.B     #6,D4
0000D8F8                          OR.B      #$EC,(A2)+
0000D8FC                          OR.B      #$10,D0
0000D900                          Invalid
0000D902                          OR.B      #$04,(A7)+
0000D906                          BTST      D2,D0
0000D908                          MOVE.L    D0,-(A2)
0000D90A                          MOVE.B    D4,D2
0000D90C                          OR.B      #$F4,$0400(A1)
0000D912                          MOVE.L    (A0)+,-(A5)
0000D914                          Invalid
0000D916                          OR.B      #$04,$0400(A5)
0000D91C                          MOVE.W    $0300(A0),-(A1)
0000D920                          Invalid
0000D922                          OR.W      #$D4FC,A4
0000D926                          BTST      D6,D0
0000D928                          LEA       $0404(A4),A7
0000D92C                          OR.W      #$0CF4,(A7)
0000D930                          BTST      D4,D0
0000D932                          SUBQ.B    #$4,A4
0000D934                          BTST      D1,D0
0000D936                          ASR.B     #2,D4
0000D938                          OR.B      #$EC,$00(A5,D0.W)
0000D93E                          MOVE.W    (A0),-(A3)
0000D940                          LSR.B     #6,D7
0000D942                          OR.B      #$0C,$0500
0000D948                          NBCD      D0
0000D94A                          Invalid
0000D94C                          LEA       $DCE0,A0
0000D950                          MOVE.W    #$0005,D7
0000D954                          SWAP      D7
0000D956                          TST.B     $0000(A0)
0000D95A                          BEQ       $0000D962
0000D95C                          MOVE.L    A0,-(A7)
0000D95E                          BSR       $0000D96E
0000D960                          MOVE.L    (A7)+,A0
0000D962                          LEA       $0040(A0),A0
0000D966                          SWAP      D7
0000D968                          DBRA      D7,$0000D954
0000D96C                          RTS
0000D96E                          MOVEQ     #$00,D0
0000D970                          MOVE.B    $0017(A0),D0
0000D974                          ADD.W     D0,D0
0000D976                          ADD.W     D0,D0
0000D978                          MOVE.L    +$00(D2.W),A1
0000D97C                          JMP       (A1)
0000D97E                          OR.B      #$CA,D0
0000D982                          OR.B      #$CA,D0
0000D986                          OR.B      #$CC,D0
0000D98A                          OR.B      #$CA,D0
0000D98E                          OR.B      #$CA,D0
0000D992                          OR.B      #$CA,D0
0000D996                          OR.B      #$CA,D0
0000D99A                          OR.B      #$CA,D0
0000D99E                          OR.B      #$CA,D0
0000D9A2                          OR.B      #$CA,D0
0000D9A6                          OR.B      #$3C,D0
0000D9AA                          OR.B      #$CA,D0
0000D9AE                          OR.B      #$CA,D0
0000D9B2                          OR.B      #$CA,D0
0000D9B6                          OR.B      #$CA,D0
0000D9BA                          OR.B      #$CA,D0
0000D9BE                          OR.B      #$CA,D0
0000D9C2                          OR.B      #$CA,D0
0000D9C6                          OR.B      #$70,D0
0000D9CA                          RTS
0000D9CC                          MOVEQ     #$00,D0
0000D9CE                          MOVE.B    $0002(A0),D0
0000D9D2                          MOVE.W    +$00(D3.W),D0
0000D9D6                          JMP       +$00(D1.W)
0000D9DA                          OR.B      #$DA,A0
0000D9DE                          BCHG      D0,-(A2)
0000D9E0                          BSET      D0,D6
0000D9E2                          MOVE.B    #$02,$0002(A0)
0000D9E8                          MOVE.W    #$0100,$0030(A0)
0000D9EE                          TST.B     $0006(A0)
0000D9F2                          BEQ       $0000DA1A
0000D9F4                          MOVE.L    #$0000DC04,$001A(A0)
0000D9FC                          MOVE.L    #$00008000,$0028(A0)
0000DA04                          MOVE.L    #$00000160,$002C(A0)
0000DA0C                          MOVE.W    #$01C0,$0008(A0)
0000DA12                          MOVE.W    #$00E0,$000C(A0)
0000DA18                          RTS
0000DA1A                          MOVEQ     #$00,D0
0000DA1C                          MOVE.B    $FC0A,D0
0000DA20                          ADD.W     D0,D0
0000DA22                          MOVE.W    +$00(D3.W),D0
0000DA26                          JMP       +$00(D1.W)
0000DA2A                          OR.B      #$30,A2
0000DA2E                          OR.W      #$005C,D6
0000DA32                          OR.W      #$217C,$00(A4,D0.W)
0000DA38                          ADD.L     Invalid
0000DA3A                          OR.B      #$7C,(A2)+
0000DA3E                          OR.B      #$00,D2
0000DA42                          OR.B      #$7C,$0000(A0)
0000DA48                          AND.B     #$2C,D0
0000DA4C                          MOVE.W    #$01A0,$0008(A0)
0000DA52                          MOVE.W    #$00DD,$000C(A0)
0000DA58                          RTS
0000DA5A                          MOVE.L    #$0000DBC4,$001A(A0)
0000DA62                          MOVE.W    #$00D8,$0008(A0)
0000DA68                          MOVE.W    #$0060,$000C(A0)
0000DA6E                          RTS
0000DA70                          MOVE.L    #$0000DBD6,$001A(A0)
0000DA78                          MOVE.W    #$00D8,$0008(A0)
0000DA7E                          MOVE.W    #$0060,$000C(A0)
0000DA84                          RTS
0000DA86                          MOVE.L    #$0000DBE2,$001A(A0)
0000DA8E                          MOVE.W    #$00D8,$0008(A0)
0000DA94                          MOVE.W    #$0060,$000C(A0)
0000DA9A                          RTS
0000DA9C                          BRA       $0000DAB2
0000DA9E                          MOVE.L    #$0000DBEE,$001A(A0)
0000DAA6                          MOVE.W    #$00C8,$0008(A0)
0000DAAC                          MOVE.W    #$0060,$000C(A0)
0000DAB2                          RTS
0000DAB4                          MOVEQ     #$00,D0
0000DAB6                          MOVE.B    $0006(A0),D0
0000DABA                          ADD.W     D0,D0
0000DABC                          MOVE.W    +$00(D3.W),D0
0000DAC0                          JMP       +$00(D1.W)
0000DAC4                          OR.B      #$42,D6
0000DAC8                          OR.L      #$20280028,D0
0000DACE                          (CCC     #$0,$0008(A0)
0000DAD2                          SUB.L     $002C(A0),D0
0000DAD6                          MOVE.L    D0,$0028(A0)
0000DADA                          SUBQ.W    #$1,$0030(A0)
0000DADE                          BEQ       $0000DB3C
0000DAE0                          CMP.W     #$0080,$0030(A0)
0000DAE6                          BNE       $0000DAF4
0000DAE8                          MOVE.B    #$01,$0032(A0)
0000DAEE                          JSR       $00000FC0
0000DAF4                          TST.B     $0032(A0)
0000DAF8                          BNE       $0000DB00
0000DAFA                          JMP       $00028CDE
0000DB00                          JMP       $00028CCE
0000DB06                          MOVE.L    $0028(A0),D0
0000DB0A                          (CCC     #$0,$0008(A0)
0000DB0E                          ADD.L     $002C(A0),D0
0000DB12                          MOVE.L    D0,$0028(A0)
0000DB16                          SUBQ.W    #$1,$0030(A0)
0000DB1A                          BEQ       $0000DB3C
0000DB1C                          CMP.W     #$0080,$0030(A0)
0000DB22                          BNE       $0000DB2A
0000DB24                          MOVE.B    #$01,$0032(A0)
0000DB2A                          TST.B     $0032(A0)
0000DB2E                          BNE       $0000DB36
0000DB30                          JMP       $00028CCE
0000DB36                          JMP       $00028CDE
0000DB3C                          MOVE.B    #$06,$0002(A0)
0000DB42                          RTS
0000DB44                          TST.B     $0003(A0)
0000DB48                          BNE       $0000DB7A
0000DB4A                          ADDQ.B    #$1,$0030(A0)
0000DB4E                          AND.B     #$01,$0030(A0)
0000DB54                          BNE       $0000DB74
0000DB56                          ADDQ.B    #$1,$0032(A0)
0000DB5A                          CMP.B     #$10,$0032(A0)
0000DB60                          BEQ       $0000DB6E
0000DB62                          JSR       $00000FC0
0000DB68                          JMP       $00028CDE
0000DB6E                          MOVE.B    #$02,$0003(A0)
0000DB74                          JMP       $00028CDE
0000DB7A                          ADDQ.B    #$1,$0030(A0)
0000DB7E                          AND.B     #$03,$0030(A0)
0000DB84                          BNE       $0000DB92
0000DB86                          SUBQ.B    #$1,$0032(A0)
0000DB8A                          BCS       $0000DB98
0000DB8C                          JSR       $00000FC0
0000DB92                          JMP       $00028CDE
0000DB98                          MOVE.B    #$06,$0002(A0)
0000DB9E                          RTS
0000DBA0                          JSR       $00028AD0
0000DBA6                          RTS
0000DBA8                          SUB.B     #$0D,D0
0000DBAC                          OR.W      #$C0FC,(A7)+
0000DBB0                          BTST      D6,D0
0000DBB2                          BEQ       $0000DB94
0000DBB4                          Invalid
0000DBB6                          OR.W      #$00FC,$0D00(A7)
0000DBBC                          MOVEQ     #$20,D3
0000DBBE                          Invalid
0000DBC0                          OR.W      #$1800,
0000DBC4                          AND.B     #$04,D0
0000DBC8                          OR.L      #$E4FC0D00,(A5)
0000DBCE                          SUB.L     -(A4),A3
0000DBD0                          Invalid
0000DBD2                          OR.L      #$04000100,(A7)+
0000DBD8                          Invalid
0000DBDA                          OR.L      #$E8F80500,-(A5)
0000DBE0                          Invalid
0000DBE2                          BTST      D0,D0
0000DBE4                          Invalid
0000DBE6                          OR.L      #$E0F80D00,$B9(A1,D0.W)
0000DBEE                          BTST      D1,D0
0000DBF0                          Invalid
0000DBF2                          Invalid
0000DBF4                          ADD.W     #$0D00,A2
0000DBF8                          MULS      $F4(A4,D2.W),D4
0000DBFC                          Invalid
0000DBFE                          MOVE.B    #$00,(A2)+
0000DC02                          ADD.B     Invalid
0000DC04                          AND.B     #$0D,D0
0000DC08                          OR.L      #$D8F80D00,D1
0000DC0E                          DIVS      $F805,D4
0000DC12                          OR.L      #$180011FC,(A1)
0000DC18                          OR.B      #$82,D0
0000DC1C                          MOVE.L    #$00FFE280,$E000
0000DC24                          TST.W     $E128
0000DC28                          BEQ       $0000DC5A
0000DC2A                          SUBQ.W    #$1,$E128
0000DC2E                          CMP.W     #$00B4,$E128
0000DC34                          BNE       $0000DC3C
0000DC36                          JSR       $00000FC0
0000DC3C                          MOVE.W    $FC0A,D0
0000DC40                          ADD.W     D0,D0
0000DC42                          ADD.W     D0,D0
0000DC44                          MOVE.W    +$00(D0.W),D0
0000DC48                          MOVE.W    #$00F0,D4
0000DC4C                          MOVE.W    #$00F0,D3
0000DC50                          MOVE.W    #$8000,D5
0000DC54                          JMP       $00022F38
0000DC5A                          MOVE.L    $E000,A0
0000DC5E                          MOVE.L    #$00000000,(A0)
0000DC64                          RTS
0000DC66                          OR.B      #$C4,D0
0000DC6A                          OR.B      #$C4,D0
0000DC6E                          OR.B      #$D6,D0
0000DC72                          OR.B      #$E2,D0
0000DC76                          OR.B      #$EE,D0
0000DC7A                          MOVEQ     #$00,D0
0000DC7C                          MOVE.B    $FC34,D0
0000DC80                          MOVE.W    +$00(D1.L),D0
0000DC84                          JSR       +$00(D7.W)
0000DC88                          JSR       $0000D94C
0000DC8E                          JMP       $000004A0
0000DC94                          OR.B      #$18,A4
0000DC98                          OR.L      #$005C009A,$00B2(A2)
0000DCA0                          MOVE.B    #$02,$FC34
0000DCA6                          JMP       $00028D60
0000DCAC                          MOVE.B    #$04,$FC34
0000DCB2                          MOVE.B    #$04,$EA81
0000DCB8                          MOVE.B    #$00,$EA87
0000DCBE                          MOVE.B    #$02,$EA01
0000DCC4                          MOVE.W    #$00C0,$A008
0000DCCA                          MOVE.W    #$06E0,$A00C
0000DCD0                          MOVE.W    #$0138,$F068
0000DCD6                          MOVE.B    #$1E,$F066
0000DCDC                          JSR       $00028D1E
0000DCE2                          JSR       $00008150
0000DCE8                          MOVE.B    #$00,$F067
0000DCEE                          RTS
0000DCF0                          ADDQ.W    #$4,$A008
0000DCF4                          SUBQ.W    #$1,$F068
0000DCF8                          BNE       $0000DD10
0000DCFA                          MOVE.B    #$0A,$FC34
0000DD00                          MOVE.W    #$0589,$A008
0000DD06                          MOVE.W    #$06AC,$A00C
0000DD0C                          BRA       $0000DD4C(pc)
0000DD10                          SUBQ.B    #$1,$F066
0000DD14                          BNE       $0000DD28
0000DD16                          CMP.B     #$07,$F067
0000DD1C                          BEQ       $0000DD28
0000DD1E                          ADDQ.B    #$1,$F067
0000DD22                          MOVE.B    #$0C,$F066
0000DD28                          JMP       $00008150
0000DD2E                          CMP.W     #$0100,$EA0C
0000DD34                          BCS       $0000DCAC(pc)
0000DD38                          JMP       $00008150
0000DD3E                          MOVE.B    #$06,$FC34
0000DD44                          RTS
0000DD46                          JSR       $00008150
0000DD4C                          MOVE.W    #$0078,$F0DA
0000DD52                          MOVE.B    #$00,$FC12
0000DD58                          RTS
0000DD5A                          JSR       $000289C0
0000DD60                          BCS       $0000DD6E
0000DD62                          MOVE.B    #$01,$0000(A1)
0000DD68                          MOVE.B    #$12,$0017(A1)
0000DD6E                          RTS
0000DD70                          MOVEM.L   D0-D7/A0-A6,-(A7)
0000DD74                          MOVEQ     #$00,D0
0000DD76                          MOVE.B    $0002(A0),D0
0000DD7A                          MOVE.W    +$00(D6.W),D1
0000DD7E                          JSR       +$10(D4.W)
0000DD82                          MOVEM.L   (A7)+,D0-D7/A0-A6
0000DD86                          RTS
0000DD88                          OR.B      #$5A,A0
0000DD8C                          BSET      D0,-(A6)
0000DD8E                          BSET      D0,-(A6)
0000DD90                          MOVEQ     #$00,D0
0000DD92                          MOVE.L    D0,$E080
0000DD96                          MOVE.L    D0,$E084
0000DD9A                          MOVE.L    D0,$E088
0000DD9E                          MOVE.L    D0,$E08C
0000DDA2                          MOVE.B    #$01,$F132
0000DDA8                          MOVE.B    #$96,$0030(A0)
0000DDAE                          MOVE.B    #$02,$0002(A0)
0000DDB4                          MOVE.B    #$01,$F0CB
0000DDBA                          MOVE.L    #$60000003,$00C00004
0000DDC4                          LEA       $00C00000,A1
0000DDCA                          MOVEQ     #$00,D1
0000DDCC                          MOVE.W    #$03FF,D2
0000DDD0                          MOVE.L    D1,(A1)
0000DDD2                          DBRA      D2,$0000DDD0
0000DDD6                          JSR       $0000DFEC
0000DDDC                          JMP       $0000DFFA
0000DDE2                          MOVEQ     #$00,D0
0000DDE4                          MOVE.B    $0003(A0),D0
0000DDE8                          MOVE.W    +$00(D3.W),D0
0000DDEC                          JMP       +$00(D1.W)
0000DDF0                          OR.B      #$46,A6
0000DDF4                          OR.W      #$009A,$00B4(A4)
0000DDFA                          MOVEP.W   $00D8(A6),D0
0000DDFE                          SUBQ.B    #$1,$0030(A0)
0000DE02                          BNE       $0000DE34
0000DE04                          MOVE.B    #$02,$0003(A0)
0000DE0A                          MOVE.B    $EA81,$0022(A0)
0000DE10                          MOVE.B    #$01,$EA81
0000DE16                          MOVE.L    #$00000000,$EBB2
0000DE1E                          MOVE.L    #$00000000,$EBD2
0000DE26                          MOVE.L    #$00000000,$EBF2
0000DE2E                          MOVE.B    #$01,$E190
0000DE34                          RTS
0000DE36                          JSR       $0000E020(pc)
0000DE3A                          MOVE.W    #$0000,$EA88
0000DE40                          MOVE.W    #$0000,$EA8C
0000DE46                          MOVE.B    #$00,$0032(A0)
0000DE4C                          MOVE.B    #$04,$0003(A0)
0000DE52                          MOVEQ     #$0F,D0
0000DE54                          JSR       $00028F50
0000DE5A                          RTS
0000DE5C                          MOVE.W    #$00FA,$0030(A0)
0000DE62                          MOVE.B    #$06,$0003(A0)
0000DE68                          TST.B     $0032(A0)
0000DE6C                          BNE       $0000DE74
0000DE6E                          JSR       $00020128
0000DE74                          JSR       $0000DF80(pc)
0000DE78                          MOVE.L    #$34000000,D0
0000DE7E                          JSR       $00022D74
0000DE84                          JMP       $0000DFBA
0000DE8A                          SUBQ.W    #$1,$0030(A0)
0000DE8E                          BNE       $0000DEA2
0000DE90                          MOVE.B    #$26,$0030(A0)
0000DE96                          MOVE.B    #$08,$0003(A0)
0000DE9C                          JSR       $0000DFFA
0000DEA2                          RTS
0000DEA4                          SUBQ.B    #$1,$0030(A0)
0000DEA8                          BNE       $0000DEC6
0000DEAA                          ADDQ.B    #$1,$0032(A0)
0000DEAE                          CMP.B     #$01,$0032(A0)
0000DEB4                          BNE       $0000DEC0
0000DEB6                          MOVE.B    #$04,$0003(A0)
0000DEBC                          BRA       $0000E020(pc)
0000DEC0                          MOVE.B    #$0C,$0003(A0)
0000DEC6                          RTS
0000DEC8                          CMP.W     #$1600,$EA08
0000DECE                          BLS       $0000DEFC
0000DED0                          MOVE.L    #$00000000,$EA82
0000DED8                          MOVE.B    #$04,$EA81
0000DEDE                          MOVE.W    #$163F,$EA08
0000DEE4                          MOVE.W    #$0720,$EA0C
0000DEEA                          MOVE.W    #$0F31,$EA88
0000DEF0                          MOVE.W    #$0660,$EA8C
0000DEF6                          MOVE.B    #$0A,$0003(A0)
0000DEFC                          RTS
0000DEFE                          MOVE.B    #$06,$0002(A0)
0000DF04                          MOVE.L    #$01030005,$EBD6
0000DF0C                          MOVE.L    #$02040005,$EBF6
0000DF14                          MOVE.L    #$080A0000,$EBB8
0000DF1C                          MOVE.B    #$01,$E190
0000DF22                          MOVE.B    #$01,$E191
0000DF28                          MOVE.L    #$4F4B1A01,D0
0000DF2E                          JSR       $00022D74
0000DF34                          JSR       $0000DFAC
0000DF3A                          JSR       $0000DFBA
0000DF40                          MOVEQ     #$00,D0
0000DF42                          LEA       $000944FA,A0
0000DF48                          LEA       $00FF0000,A1
0000DF4E                          JSR       $0002077E
0000DF54                          LEA       $00094ABC,A0
0000DF5A                          LEA       $00FF6000,A1
0000DF60                          JSR       $0002077E
0000DF66                          MOVEQ     #$3E,D0
0000DF68                          JMP       $00029062
0000DF6E                          MOVE.B    #$00,$F0CB
0000DF74                          MOVE.B    #$00,$F132
0000DF7A                          JMP       $00028AD0
0000DF80                          MOVE.L    A0,-(A7)
0000DF82                          MOVE.W    #$E120,D0
0000DF86                          TST.B     $0032(A0)
0000DF8A                          BNE       $0000DF9C
0000DF8C                          LEA       $000243C0,A0
0000DF92                          JSR       $00023F8E
0000DF98                          MOVE.L    (A7)+,A0
0000DF9A                          RTS
0000DF9C                          LEA       $000243D2,A0
0000DFA2                          JSR       $00023F8E
0000DFA8                          MOVE.L    (A7)+,A0
0000DFAA                          RTS
0000DFAC                          LEA       $E0DC,A0
0000DFB0                          LEA       $E750,A1
0000DFB4                          LEA       $E7D0,A2
0000DFB8                          BRA       $0000DFC6
0000DFBA                          LEA       $E0E2,A0
0000DFBE                          LEA       $E770,A1
0000DFC2                          LEA       $E7F0,A2
0000DFC6                          MOVE.L    A1,-(A7)
0000DFC8                          JSR       $00001428
0000DFCE                          MOVE.L    (A7)+,A1
0000DFD0                          MOVEQ     #$00,D1
0000DFD2                          JSR       $00001364
0000DFD8                          MOVE.W    #$0010,$0000(A0)
0000DFDE                          MOVE.W    #$0102,$0002(A0)
0000DFE4                          MOVE.W    #$FFFF,$0004(A0)
0000DFEA                          RTS
0000DFEC                          LEA       $E0F4,A0
0000DFF0                          LEA       $E750,A1
0000DFF4                          LEA       $E7D0,A2
0000DFF8                          BRA       $0000E006
0000DFFA                          LEA       $E0FA,A0
0000DFFE                          LEA       $E770,A1
0000E002                          LEA       $E7F0,A2
0000E006                          JSR       $00001428
0000E00C                          MOVE.W    #$0010,$0000(A0)
0000E012                          MOVE.W    #$0102,$0002(A0)
0000E018                          MOVE.W    #$FFFF,$0004(A0)
0000E01E                          RTS
0000E020                          MOVE.L    #$60000003,$00C00004
0000E02A                          LEA       $00C00000,A1
0000E030                          MOVEQ     #$00,D1
0000E032                          MOVE.W    #$03FF,D2
0000E036                          MOVE.L    D1,(A1)
0000E038                          DBRA      D2,$0000E036
0000E03C                          RTS
0000E03E                          MOVE.W    #$8134,$00C00004
0000E046                          MOVE.W    #$9200,$00C00004
0000E04E                          JSR       $00022DBC
0000E054                          JSR       $000011BE
0000E05A                          MOVE.L    #$C0000000,$00C00004
0000E064                          MOVEQ     #$00,D1
0000E066                          LEA       $E710,A1
0000E06A                          JSR       $00001334
0000E070                          MOVE.W    #$9003,$00C00004
0000E078                          MOVE.W    #$8405,$00C00004
0000E080                          MOVE.L    #$07060510,D0
0000E086                          JSR       $00028F50
0000E08C                          MOVEQ     #$11,D0
0000E08E                          JSR       $00028F50
0000E094                          MOVE.L    #$18351700,D0
0000E09A                          JSR       $00022D74
0000E0A0                          MOVEQ     #$06,D7
0000E0A2                          LEA       $00FF0000,A1
0000E0A8                          MOVE.W    #$E001,D0
0000E0AC                          LEA       $0000E16A(pc),A2
0000E0B0                          MOVE.L    (A2)+,A0
0000E0B2                          MOVEM.L   D7/A1-A2,-(A7)
0000E0B6                          JSR       $0002077E
0000E0BC                          MOVEM.L   (A7)+,D7/A1-A2
0000E0C0                          ADD.W     #$0800,A1
0000E0C4                          DBRA      D7,$0000E0B0
0000E0C8                          MOVE.L    #$08030201,D0
0000E0CE                          JSR       $0000E846(pc)
0000E0D2                          LEA       $00056F74,A0
0000E0D8                          LEA       $00FF4000,A1
0000E0DE                          MOVEQ     #$01,D0
0000E0E0                          JSR       $0002077E
0000E0E6                          MOVE.W    #$03C0,$00FF431A
0000E0EE                          MOVE.W    #$03C1,$00FF431C
0000E0F6                          MOVE.W    #$0004,$E102
0000E0FC                          MOVE.B    #$01,$00FF7860
0000E104                          MOVE.W    #$0000,$E104
0000E10A                          MOVE.W    #$0008,$00FF7006
0000E112                          MOVE.W    #$0000,$00FF7008
0000E11A                          MOVE.W    #$FFE0,$EA0C
0000E120                          MOVE.W    #$FFF0,$EA8C
0000E126                          JSR       $0000149C
0000E12C                          MOVE.W    #$8174,$00C00004
0000E134                          JSR       $000014D4
0000E13A                          BSR       $0000E186
0000E13C                          JSR       $0000E8D6(pc)
0000E140                          MOVE.W    #$0004,D0
0000E144                          CMP.W     #$0028,$00FF7000
0000E14C                          BEQ       $0000E15A
0000E14E                          BTST      #$07,$E109
0000E154                          BEQ       $0000E134
0000E156                          MOVE.W    #$000B,D0
0000E15A                          JSR       $000011BE
0000E160                          MOVE.W    D0,$E100
0000E164                          JMP       $00020352
0000E16A                          OR.B      #$92,D5
0000E16E                          OR.B      #$EA,D5
0000E172                          OR.B      #$46,D5
0000E176                          OR.B      #$DC,D5
0000E17A                          OR.B      #$EE,D5
0000E17E                          OR.B      #$5C,D0
0000E182                          OR.B      #$00,D0
0000E186                          MOVE.W    $00FF7000,D0
0000E18C                          LEA       $0000E198(pc),A1
0000E190                          MOVE.W    $00(A1,D0.W),D0
0000E194                          JMP       $00(A1,D0.W)
0000E198                          OR.B      #$2A,CCR
0000E19C                          OR.W      #$00A4,$00B4
0000E1A2                          ADD.W     #$067C,$00(A4,D5.L)
0000E1A8                          BTST      D0,$0674(A6)
0000E1AC                          ADD.W     #$002C,SR
0000E1B0                          OR.B      #$98,$03B2(A2)
0000E1B6                          ADD.W     #$067C,$06(A4,D2.L)
0000E1BC                          ADD.W     #$03AE,SR
0000E1C0                          BCLR      D1,$4E(A2,A2.L)
0000E1C4                          MOVE.B    #$01,$00FF7800
0000E1CC                          ADDQ.W    #$2,$00FF7000
0000E1D2                          RTS
0000E1D4                          SUBQ.W    #$1,$00FF7006
0000E1DA                          BNE       $0000E1FE
0000E1DC                          MOVE.W    #$0008,$00FF7006
0000E1E4                          ADDQ.W    #$2,$00FF7008
0000E1EA                          CMP.W     #$0006,$00FF7008
0000E1F2                          BCC       $0000E200
0000E1F4                          MOVE.W    $00FF7008,$00FF7004
0000E1FE                          RTS
0000E200                          MOVE.B    #$01,$00FF7800
0000E208                          ADDQ.W    #$2,$00FF7000
0000E20E                          RTS
0000E210                          ADDQ.W    #$4,$EA0C
0000E214                          MOVE.W    #$0020,$00FF7006
0000E21C                          MOVE.B    #$01,$00FF78C0
0000E224                          MOVE.B    #$01,$00FF7920
0000E22C                          MOVE.B    #$01,$00FF7950
0000E234                          ADDQ.W    #$2,$00FF7000
0000E23A                          RTS
0000E23C                          SUBQ.W    #$1,$00FF7006
0000E242                          BNE       $0000E24A
0000E244                          ADDQ.W    #$2,$00FF7000
0000E24A                          RTS
0000E24C                          ADDQ.W    #$4,$EA0C
0000E250                          ADDQ.W    #$0,$EA08
0000E254                          ADD.W     #$FFFC,$EA8C
0000E25A                          ADD.W     #$FFF8,$EA88
0000E260                          CMP.W     #$0070,$EA08
0000E266                          BCS       $0000E282
0000E268                          MOVE.W    $EA0C,D0
0000E26C                          AND.W     #$000F,D0
0000E270                          BNE       $0000E282
0000E272                          ADDQ.W    #$1,$00FF700C
0000E278                          MOVE.W    $00FF700C,$00FF700A
0000E282                          CMP.W     #$0200,$EA08
0000E288                          BCS       $0000E290
0000E28A                          ADDQ.W    #$2,$00FF7000
0000E290                          RTS
0000E292                          MOVE.B    #$00,$00FF78C0
0000E29A                          MOVE.B    #$00,$00FF7920
0000E2A2                          MOVEQ     #$00,D1
0000E2A4                          MOVE.W    D1,$EA8C
0000E2A8                          MOVE.W    D1,$EA08
0000E2AC                          MOVE.W    D1,$EA88
0000E2B0                          MOVE.W    #$0008,$EA0C
0000E2B6                          MOVE.W    #$0006,$00FF7004
0000E2BE                          ADDQ.W    #$2,$00FF7000
0000E2C4                          RTS
0000E2C6                          TST.W     $E104
0000E2CA                          BNE       $0000E30E
0000E2CC                          MOVE.W    $00FF7002,D0
0000E2D2                          MOVEQ     #$0A,D2
0000E2D4                          CMP.W     #$000B,D0
0000E2D8                          BCS       $0000E2DC
0000E2DA                          MOVEQ     #$06,D2
0000E2DC                          MOVE.W    D2,$E104
0000E2E0                          MOVE.W    D0,D1
0000E2E2                          MULS      #$0020,D0
0000E2E6                          LEA       +$00(D4.W),A1
0000E2EA                          LEA       $E710,A2
0000E2EE                          JSR       $00001428
0000E2F4                          ADDQ.W    #$1,D1
0000E2F6                          CMP.W     #$0010,D1
0000E2FA                          BCS       $0000E302
0000E2FC                          ADDQ.W    #$2,$00FF7000
0000E302                          MOVE.W    D1,$00FF7002
0000E308                          MOVE.B    #$01,$E183
0000E30E                          RTS
0000E310                          OR.B      #$00,D0
0000E314                          OR.B      #$00,D0
0000E318                          OR.B      #$00,D0
0000E31C                          OR.B      #$00,D0
0000E320                          OR.B      #$00,D0
0000E324                          OR.B      #$00,D0
0000E328                          OR.B      #$00,D0
0000E32C                          OR.B      #$00,D0
0000E330                          OR.B      #$00,D0
0000E334                          OR.B      #$00,D0
0000E338                          OR.B      #$00,D0
0000E33C                          OR.B      #$00,D0
0000E340                          OR.B      #$20,D0
0000E344                          AND.B     #$20,-(A2)
0000E348                          AND.B     #$20,-(A2)
0000E34C                          OR.B      #$00,D0
0000E350                          OR.B      #$00,D0
0000E354                          OR.B      #$00,D0
0000E358                          OR.B      #$00,D0
0000E35C                          OR.B      #$00,D0
0000E360                          OR.B      #$40,D0
0000E364                          SUB.W     #$0440,D2
0000E368                          SUB.W     #$0440,D4
0000E36C                          OR.B      #$00,D0
0000E370                          OR.B      #$00,D0
0000E374                          OR.B      #$00,D0
0000E378                          OR.B      #$00,D0
0000E37C                          OR.B      #$00,D0
0000E380                          OR.B      #$40,D0
0000E384                          ADD.W     #$0660,-(A2)
0000E388                          ADD.W     #$0660,-(A6)
0000E38C                          OR.B      #$00,D0
0000E390                          OR.B      #$00,D0
0000E394                          OR.B      #$00,D0
0000E398                          OR.B      #$00,D0
0000E39C                          OR.B      #$00,D0
0000E3A0                          OR.B      #$40,D0
0000E3A4                          BCHG      #$80,-(A2)
0000E3A8                          BCLR      #$80,D6
0000E3AC                          OR.B      #$00,D0
0000E3B0                          OR.B      #$00,D0
0000E3B4                          OR.B      #$00,D0
0000E3B8                          OR.B      #$00,D0
0000E3BC                          OR.B      #$00,D0
0000E3C0                          OR.B      #$40,D0
0000E3C4                          EOR.W     #$0AA0,-(A2)
0000E3C8                          EOR.L     #$0AA00000,-(A6)
0000E3CE                          OR.B      #$00,D0
0000E3D2                          OR.B      #$00,D0
0000E3D6                          OR.B      #$00,D0
0000E3DA                          OR.B      #$00,D0
0000E3DE                          OR.B      #$00,D0
0000E3E2                          BCHG      #$62,D0
0000E3E6                          CMP.L     #$0CC60CC0,-(A0)
0000E3EC                          OR.B      #$00,D0
0000E3F0                          OR.B      #$00,D0
0000E3F4                          OR.B      #$00,D0
0000E3F8                          OR.B      #$00,D0
0000E3FC                          OR.B      #$00,D0
0000E400                          OR.B      #$40,D0
0000E404                          EOR.W     #$0CA4,-(A2)
0000E408                          Invalid
0000E40A                          Invalid
0000E40C                          OR.B      #$00,D0
0000E410                          OR.B      #$02,D0
0000E414                          AND.B     #$02,D2
0000E418                          AND.B     #$02,D2
0000E41C                          AND.B     #$02,D2
0000E420                          OR.B      #$40,D0
0000E424                          EOR.W     #$0CA4,-(A2)
0000E428                          Invalid
0000E42A                          Invalid
0000E42C                          OR.B      #$00,D0
0000E430                          OR.B      #$04,D0
0000E434                          SUB.B     #$04,D4
0000E438                          SUB.B     #$04,D4
0000E43C                          SUB.B     #$02,D4
0000E440                          OR.B      #$40,D0
0000E444                          EOR.W     #$0CA4,-(A2)
0000E448                          Invalid
0000E44A                          Invalid
0000E44C                          OR.B      #$00,D0
0000E450                          OR.B      #$06,D0
0000E454                          ADD.B     #$06,D6
0000E458                          ADD.B     #$06,D6
0000E45C                          SUB.B     #$02,D4
0000E460                          OR.B      #$40,D0
0000E464                          EOR.W     #$0CA4,-(A2)
0000E468                          Invalid
0000E46A                          Invalid
0000E46C                          OR.B      #$00,D0
0000E470                          OR.B      #$08,D0
0000E474                          BTST      #$08,A0
0000E478                          BTST      #$06,A0
0000E47C                          SUB.B     #$02,D4
0000E480                          OR.B      #$40,D0
0000E484                          EOR.W     #$0CA4,-(A2)
0000E488                          Invalid
0000E48A                          Invalid
0000E48C                          OR.B      #$00,D0
0000E490                          OR.B      #$0A,D0
0000E494                          EOR.B     #$0A,A2
0000E498                          BTST      #$06,A0
0000E49C                          SUB.B     #$02,D4
0000E4A0                          OR.B      #$40,D0
0000E4A4                          EOR.W     #$0CA4,-(A2)
0000E4A8                          Invalid
0000E4AA                          Invalid
0000E4AC                          OR.B      #$00,D0
0000E4B0                          OR.B      #$0C,D0
0000E4B4                          CMP.B     #$0A,A4
0000E4B8                          BTST      #$06,A0
0000E4BC                          SUB.B     #$02,D4
0000E4C0                          OR.B      #$40,D0
0000E4C4                          EOR.W     #$0CA4,-(A2)
0000E4C8                          Invalid
0000E4CA                          Invalid
0000E4CC                          OR.B      #$00,D0
0000E4D0                          OR.B      #$0E,D0
0000E4D4                          CMP.B     #$0A,A4
0000E4D8                          BTST      #$06,A0
0000E4DC                          SUB.B     #$02,D4
0000E4E0                          OR.B      #$40,D0
0000E4E4                          EOR.W     #$0CA4,-(A2)
0000E4E8                          Invalid
0000E4EA                          Invalid
0000E4EC                          OR.B      #$00,D0
0000E4F0                          OR.B      #$0E,D0
0000E4F4                          CMP.B     #$0A,A4
0000E4F8                          BTST      #$06,A0
0000E4FC                          SUB.B     #$02,D4
0000E500                          OR.B      #$40,D0
0000E504                          EOR.W     #$0CA4,-(A2)
0000E508                          Invalid
0000E50A                          Invalid
0000E50C                          OR.B      #$00,D0
0000E510                          OR.B      #$0E,D0
0000E514                          CMP.B     #$0A,A4
0000E518                          BTST      #$06,A0
0000E51C                          SUB.B     #$02,D4
0000E520                          OR.B      #$40,D0
0000E524                          EOR.W     #$0CA4,-(A2)
0000E528                          Invalid
0000E52A                          Invalid
0000E52C                          OR.B      #$00,D0
0000E530                          MOVE.W    #$0000,$00FF7002
0000E538                          MOVE.W    #$0000,$E104
0000E53E                          ADDQ.W    #$2,$00FF7000
0000E544                          RTS
0000E546                          MOVEQ     #$14,D2
0000E548                          BRA       $0000E54C
0000E54A                          MOVEQ     #$0D,D2
0000E54C                          TST.W     $E104
0000E550                          BNE       $0000E58A
0000E552                          MOVE.W    #$0003,$E104
0000E558                          MOVE.W    $00FF7002,D0
0000E55E                          MOVE.W    D0,D1
0000E560                          MULS      #$0020,D0
0000E564                          LEA       +$00(D3.W),A1
0000E568                          LEA       $E710,A2
0000E56C                          JSR       $00001428
0000E572                          ADDQ.W    #$1,D1
0000E574                          CMP.W     D2,D1
0000E576                          BCS       $0000E57E
0000E578                          ADDQ.W    #$2,$00FF7000
0000E57E                          MOVE.W    D1,$00FF7002
0000E584                          MOVE.B    #$01,$E183
0000E58A                          RTS
0000E58C                          OR.B      #$0E,D0
0000E590                          CMP.B     #$0A,A4
0000E594                          BTST      #$06,A0
0000E598                          SUB.B     #$02,D4
0000E59C                          OR.B      #$40,D0
0000E5A0                          EOR.W     #$0CA4,-(A2)
0000E5A4                          Invalid
0000E5A6                          Invalid
0000E5A8                          OR.B      #$00,D0
0000E5AC                          OR.B      #$0A,D0
0000E5B0                          CMP.B     #$0A,A4
0000E5B4                          BTST      #$06,A0
0000E5B8                          SUB.B     #$02,D4
0000E5BC                          OR.B      #$40,D0
0000E5C0                          EOR.W     #$0CA4,-(A2)
0000E5C4                          Invalid
0000E5C6                          Invalid
0000E5C8                          OR.B      #$00,D0
0000E5CC                          OR.B      #$08,D0
0000E5D0                          EOR.B     #$0A,A2
0000E5D4                          BTST      #$06,A0
0000E5D8                          SUB.B     #$02,D4
0000E5DC                          OR.B      #$40,D0
0000E5E0                          EOR.W     #$0CA4,-(A2)
0000E5E4                          Invalid
0000E5E6                          Invalid
0000E5E8                          OR.B      #$00,D0
0000E5EC                          OR.B      #$06,D0
0000E5F0                          BTST      #$0A,A0
0000E5F4                          BTST      #$06,A0
0000E5F8                          SUB.B     #$02,D4
0000E5FC                          OR.B      #$40,D0
0000E600                          EOR.W     #$0CA4,-(A2)
0000E604                          Invalid
0000E606                          Invalid
0000E608                          OR.B      #$00,D0
0000E60C                          OR.B      #$04,D0
0000E610                          ADD.B     #$08,D6
0000E614                          BTST      #$06,A0
0000E618                          SUB.B     #$02,D4
0000E61C                          OR.B      #$40,D0
0000E620                          EOR.W     #$0CA4,-(A2)
0000E624                          Invalid
0000E626                          Invalid
0000E628                          OR.B      #$00,D0
0000E62C                          OR.B      #$02,D0
0000E630                          SUB.B     #$06,D4
0000E634                          BTST      #$06,A0
0000E638                          SUB.B     #$02,D4
0000E63C                          OR.B      #$40,D0
0000E640                          EOR.W     #$0CA4,-(A2)
0000E644                          Invalid
0000E646                          Invalid
0000E648                          OR.B      #$00,D0
0000E64C                          OR.B      #$00,D0
0000E650                          AND.B     #$04,D2
0000E654                          ADD.B     #$06,D6
0000E658                          SUB.B     #$02,D4
0000E65C                          OR.B      #$40,D0
0000E660                          EOR.W     #$0CA4,-(A2)
0000E664                          Invalid
0000E666                          Invalid
0000E668                          OR.B      #$00,D0
0000E66C                          OR.B      #$00,D0
0000E670                          OR.B      #$02,D0
0000E674                          SUB.B     #$06,D4
0000E678                          SUB.B     #$02,D4
0000E67C                          OR.B      #$40,D0
0000E680                          EOR.W     #$0CA4,-(A2)
0000E684                          Invalid
0000E686                          Invalid
0000E688                          OR.B      #$00,D0
0000E68C                          OR.B      #$00,D0
0000E690                          OR.B      #$00,D0
0000E694                          AND.B     #$04,D2
0000E698                          SUB.B     #$02,D4
0000E69C                          OR.B      #$40,D0
0000E6A0                          EOR.W     #$0CA4,-(A2)
0000E6A4                          Invalid
0000E6A6                          Invalid
0000E6A8                          OR.B      #$00,D0
0000E6AC                          OR.B      #$00,D0
0000E6B0                          OR.B      #$00,D0
0000E6B4                          OR.B      #$02,D0
0000E6B8                          SUB.B     #$02,D4
0000E6BC                          OR.B      #$40,D0
0000E6C0                          EOR.W     #$0CA4,-(A2)
0000E6C4                          Invalid
0000E6C6                          Invalid
0000E6C8                          OR.B      #$00,D0
0000E6CC                          OR.B      #$00,D0
0000E6D0                          OR.B      #$00,D0
0000E6D4                          OR.B      #$00,D0
0000E6D8                          AND.B     #$02,D2
0000E6DC                          OR.B      #$40,D0
0000E6E0                          EOR.W     #$0CA4,-(A2)
0000E6E4                          Invalid
0000E6E6                          Invalid
0000E6E8                          OR.B      #$00,D0
0000E6EC                          OR.B      #$00,D0
0000E6F0                          OR.B      #$00,D0
0000E6F4                          OR.B      #$00,D0
0000E6F8                          OR.B      #$02,D0
0000E6FC                          OR.B      #$40,D0
0000E700                          EOR.W     #$0CA4,-(A2)
0000E704                          Invalid
0000E706                          Invalid
0000E708                          OR.B      #$00,D0
0000E70C                          OR.B      #$00,D0
0000E710                          OR.B      #$00,D0
0000E714                          OR.B      #$00,D0
0000E718                          OR.B      #$00,D0
0000E71C                          OR.B      #$40,D0
0000E720                          EOR.W     #$0CA4,-(A2)
0000E724                          Invalid
0000E726                          Invalid
0000E728                          OR.B      #$00,D0
0000E72C                          OR.B      #$00,D0
0000E730                          OR.B      #$00,D0
0000E734                          OR.B      #$00,D0
0000E738                          OR.B      #$00,D0
0000E73C                          OR.B      #$20,D0
0000E740                          BCHG      #$82,D0
0000E744                          CMP.L     #$0CC60000,-(A4)
0000E74A                          OR.B      #$00,D0
0000E74E                          OR.B      #$00,D0
0000E752                          OR.B      #$00,D0
0000E756                          OR.B      #$00,D0
0000E75A                          OR.B      #$00,D0
0000E75E                          SUB.B     #$20,D0
0000E762                          BCHG      #$82,-(A0)
0000E766                          EOR.L     #$00000000,-(A4)
0000E76C                          OR.B      #$00,D0
0000E770                          OR.B      #$00,D0
0000E774                          OR.B      #$00,D0
0000E778                          OR.B      #$00,D0
0000E77C                          OR.B      #$00,D0
0000E780                          SUB.B     #$40,D0
0000E784                          BCHG      #$82,-(A0)
0000E788                          OR.B      #$00,D0
0000E78C                          OR.B      #$00,D0
0000E790                          OR.B      #$00,D0
0000E794                          OR.B      #$00,D0
0000E798                          OR.B      #$00,D0
0000E79C                          OR.B      #$00,D0
0000E7A0                          AND.B     #$20,D0
0000E7A4                          ADD.W     #$0660,D0
0000E7A8                          OR.B      #$00,D0
0000E7AC                          OR.B      #$00,D0
0000E7B0                          OR.B      #$00,D0
0000E7B4                          OR.B      #$00,D0
0000E7B8                          OR.B      #$00,D0
0000E7BC                          OR.B      #$00,D0
0000E7C0                          OR.B      #$00,D0
0000E7C4                          SUB.B     #$40,-(A0)
0000E7C8                          OR.B      #$00,D0
0000E7CC                          OR.B      #$00,D0
0000E7D0                          OR.B      #$00,D0
0000E7D4                          OR.B      #$00,D0
0000E7D8                          OR.B      #$00,D0
0000E7DC                          OR.B      #$00,D0
0000E7E0                          OR.B      #$00,D0
0000E7E4                          AND.B     #$20,D0
0000E7E8                          OR.B      #$00,D0
0000E7EC                          OR.B      #$00,D0
0000E7F0                          OR.B      #$00,D0
0000E7F4                          OR.B      #$00,D0
0000E7F8                          OR.B      #$00,D0
0000E7FC                          OR.B      #$00,D0
0000E800                          OR.B      #$00,D0
0000E804                          OR.B      #$00,D0
0000E808                          OR.B      #$00,D0
0000E80C                          MOVE.W    #$0020,$E104
0000E812                          BRA       $0000E81A
0000E814                          TST.W     $E104
0000E818                          BNE       $0000E820
0000E81A                          ADDQ.W    #$2,$00FF7000
0000E820                          RTS
0000E822                          LEA       $00C00004,A2
0000E828                          LEA       $00C00000,A3
0000E82E                          MOVE.L    #$01000000,D4
0000E834                          MOVE.L    D0,(A2)
0000E836                          MOVE.W    D1,D3
0000E838                          MOVE.W    (A1)+,(A3)
0000E83A                          DBRA      D3,$0000E838
0000E83E                          ADD.L     D4,D0
0000E840                          DBRA      D2,$0000E834
0000E844                          RTS
0000E846                          MOVEQ     #$03,D2
0000E848                          MOVEQ     #$00,D1
0000E84A                          MOVE.B    D0,D1
0000E84C                          BEQ       $0000E86E
0000E84E                          MULU      #$0C,D1
0000E852                          MOVEM.L   D0-D2,-(A7)
0000E856                          MOVE.L    +$10(D1.L),A1
0000E85A                          MOVE.L    +$10(D1.L),D0
0000E85E                          MOVE.W    +$10(D2.L),D2
0000E862                          MOVE.W    +$10(D7.W),D1
0000E866                          BSR       $0000E822(pc)
0000E86A                          MOVEM.L   (A7)+,D0-D2
0000E86E                          ROR.L     #8,D0
0000E870                          DBRA      D2,$0000E848
0000E874                          RTS
0000E876                          Invalid
0000E878                          OR.B      #$08,D0
0000E87C                          OR.B      #$1F,D3
0000E880                          OR.B      #$FF,(A7)+
0000E884                          BTST      #$48,D0
0000E888                          OR.B      #$03,D3
0000E88C                          OR.B      #$FF,A7
0000E890                          MOVE.B    D0,D0
0000E892                          BRA       $0000E89C
0000E894                          OR.B      #$1F,D2
0000E898                          OR.B      #$FF,(A7)+
0000E89C                          MOVE.B    D0,D4
0000E89E                          Invalid
0000E8A0                          OR.B      #$14,D3
0000E8A4                          OR.B      #$FF,A4
0000E8A8                          NEGX.B    D0
0000E8AA                          Invalid
0000E8AC                          OR.B      #$27,D3
0000E8B0                          OR.B      #$FF,(A1)+
0000E8B4                          MOVE.L    D0,D0
0000E8B6                          Invalid
0000E8B8                          OR.B      #$14,D3
0000E8BC                          OR.B      #$FF,A4
0000E8C0                          MOVE.L    D0,D4
0000E8C2                          BRA       $0000E8CC
0000E8C4                          OR.B      #$1F,D2
0000E8C8                          OR.B      #$FF,(A7)+
0000E8CC                          MOVE.W    D0,D0
0000E8CE                          BHI       $0000E8D2(pc)
0000E8D2                          OR.B      #$0F,D3
0000E8D6                          MOVE.B    #$00,$E182
0000E8DC                          MOVE.L    #$00FFE280,$E000
0000E8E4                          LEA       $00FF7800,A0
0000E8EA                          JSR       $0000E936(pc)
0000E8EE                          LEA       $00FF7860,A0
0000E8F4                          JSR       $0000E9BA(pc)
0000E8F8                          LEA       $00FF78C0,A0
0000E8FE                          JSR       $0000E9F0(pc)
0000E902                          LEA       $00FF7920,A0
0000E908                          JSR       $0000E9F0(pc)
0000E90C                          LEA       $00FF7980,A0
0000E912                          JSR       $0000EA2A(pc)
0000E916                          MOVE.L    $E000,A0
0000E91A                          MOVE.L    #$00000000,(A0)
0000E920                          RTS
0000E922                          TST.B     $0000(A0)
0000E926                          BNE       $0000E92A
0000E928                          RTS
0000E92A                          MOVE.W    $0038(A0),D0
0000E92E                          MOVE.W    $00(A2,D0.W),D0
0000E932                          JMP       $00(A2,D0.W)
0000E936                          LEA       $0000E93E(pc),A2
0000E93A                          JMP       $0000E922(pc)
0000E93E                          OR.B      #$2A,D4
0000E942                          MOVE.L    #$00057AE8,$003A(A0)
0000E94A                          MOVE.W    #$0505,$0034(A0)
0000E950                          MOVE.W    #$0170,$0008(A0)
0000E956                          MOVE.W    #$0108,$000C(A0)
0000E95C                          MOVE.B    #$01,$0036(A0)
0000E962                          ADDQ.W    #$2,$0038(A0)
0000E966                          RTS
0000E968                          JSR       $0000EAC6(pc)
0000E96C                          MOVE.W    $0032(A0),D0
0000E970                          CMP.W     #$001C,D0
0000E974                          BCC       $0000E986
0000E976                          MOVE.W    +$00(D1.W),$0008(A0)
0000E97C                          MOVE.W    +$00(D7.L),$000C(A0)
0000E982                          JMP       $0000EAF0(pc)
0000E986                          ADDQ.W    #$2,$00FF7000
0000E98C                          MOVEQ     #$00,D1
0000E98E                          MOVE.B    D1,$0000(A0)
0000E992                          MOVE.L    A0,A1
0000E994                          JMP       $00001344
0000E99A                          BCHG      D0,$01(A0,D4.W)
0000E99E                          BCHG      D0,(A0)+
0000E9A0                          Invalid
0000E9A2                          BTST      D0,$00EC
0000E9A6                          BTST      D0,(A0)+
0000E9A8                          Invalid
0000E9AA                          Invalid
0000E9AC                          Invalid
0000E9AE                          Invalid
0000E9B0                          OR.L      #$00A800A4,$00(A4,D4.W)
0000E9B8                          OR.L      #$45FA0006,-(A4)
0000E9BE                          JMP       $0000E92A(pc)
0000E9C2                          OR.B      #$2A,D4
0000E9C6                          MOVE.L    #$00057BCA,$003A(A0)
0000E9CE                          MOVE.W    #$00F8,$0008(A0)
0000E9D4                          MOVE.W    #$0140,$000C(A0)
0000E9DA                          MOVE.L    #$0EEE0C44,$E752
0000E9E2                          MOVE.B    #$01,$E183
0000E9E8                          ADDQ.W    #$2,$0038(A0)
0000E9EC                          JMP       $0000EB10(pc)
0000E9F0                          LEA       $0000E9F8(pc),A2
0000E9F4                          JMP       $0000E922(pc)
0000E9F8                          OR.B      #$2E,D4
0000E9FC                          MOVE.W    #$00A0,D0
0000EA00                          MOVE.W    #$0088,D1
0000EA04                          TST.B     $0030(A0)
0000EA08                          BEQ       $0000EA12
0000EA0A                          MOVE.W    #$0140,D0
0000EA0E                          MOVE.W    #$00D8,D1
0000EA12                          MOVE.W    D0,$0008(A0)
0000EA16                          MOVE.W    D1,$000C(A0)
0000EA1A                          MOVE.L    #$00057B90,$003A(A0)
0000EA22                          ADDQ.W    #$2,$0038(A0)
0000EA26                          JMP       $0000EB0A(pc)
0000EA2A                          LEA       $0000EA32(pc),A2
0000EA2E                          JMP       $0000E92A(pc)
0000EA32                          OR.B      #$24,D4
0000EA36                          MOVE.W    #$00E8,$0008(A0)
0000EA3C                          MOVE.W    #$0128,$000C(A0)
0000EA42                          MOVE.L    #$0000EA74,$003A(A0)
0000EA4A                          MOVE.W    #$0012,$0030(A0)
0000EA50                          ADDQ.W    #$2,$0038(A0)
0000EA54                          RTS
0000EA56                          SUBQ.W    #$1,$0030(A0)
0000EA5A                          BNE       $0000EA68
0000EA5C                          MOVE.W    #$0012,$0030(A0)
0000EA62                          BCHG      #$00,$003E(A0)
0000EA68                          BTST      #$00,$003E(A0)
0000EA6E                          BNE       $0000EB04(pc)
0000EA72                          RTS
0000EA74                          BTST      D7,D0
0000EA76                          OR.B      #$10,D0
0000EA7A                          Invalid
0000EA7C                          OR.B      #$F8,D0
0000EA80                          OR.B      #$0F,D0
0000EA84                          OR.B      #$00,D0
0000EA88                          MOVE.B    A0,-(A1)
0000EA8A                          OR.B      #$13,D0
0000EA8E                          MOVE.B    D0,D0
0000EA90                          OR.B      #$20,D0
0000EA94                          OR.B      #$0C,D0
0000EA98                          MOVE.L    D0,D4
0000EA9A                          OR.B      #$30,D0
0000EA9E                          OR.B      #$15,D0
0000EAA2                          MOVE.W    D0,D4
0000EAA4                          OR.B      #$40,D0
0000EAA8                          OR.B      #$16,D0
0000EAAC                          ADDQ.B    #$0,D0
0000EAAE                          OR.B      #$58,D0
0000EAB2                          OR.B      #$0C,D0
0000EAB6                          BRA       $0000EAB8(pc)
0000EABA                          CMP.W     #$0000,$0000(A0)
0000EAC0                          MOVEQ     #$00,D0
0000EAC2                          OR.B      #$78,D0
0000EAC6                          TST.B     $0036(A0)
0000EACA                          BEQ       $0000EAEE
0000EACC                          MOVE.L    $003A(A0),A1
0000EAD0                          SUBQ.B    #$1,$0035(A0)
0000EAD4                          BNE       $0000EAEE
0000EAD6                          MOVE.B    $0034(A0),$0035(A0)
0000EADC                          ADDQ.W    #$4,$0032(A0)
0000EAE0                          MOVE.W    $0032(A0),D0
0000EAE4                          CMP.B     (A1),D0
0000EAE6                          BCS       $0000EAEE
0000EAE8                          MOVE.W    #$0000,$0032(A0)
0000EAEE                          RTS
0000EAF0                          MOVE.W    #$A300,D5
0000EAF4                          MOVE.L    $003A(A0),A1
0000EAF8                          ADDQ.L    #$2,A1
0000EAFA                          MOVE.W    $0032(A0),D0
0000EAFE                          MOVE.L    $00(A1,D0.W),A1
0000EB02                          BRA       $0000EB18
0000EB04                          MOVE.W    #$C400,D5
0000EB08                          BRA       $0000EB14
0000EB0A                          MOVE.W    #$6380,D5
0000EB0E                          BRA       $0000EB14
0000EB10                          MOVE.W    #$C300,D5
0000EB14                          MOVE.L    $003A(A0),A1
0000EB18                          MOVE.W    $0008(A0),D4
0000EB1C                          MOVE.W    $000C(A0),D3
0000EB20                          JMP       $00022F38
0000EB26                          MOVE.W    $00FF7004,D0
0000EB2C                          BEQ       $0000EB30
0000EB2E                          BSR       $0000EB3E
0000EB30                          MOVE.W    $00FF700A,D0
0000EB36                          BEQ       $0000EB3C
0000EB38                          JMP       $0000EBC0(pc)
0000EB3C                          RTS
0000EB3E                          MOVE.W    #$0000,$00FF7004
0000EB46                          LEA       $0000EB52(pc),A1
0000EB4A                          MOVE.W    $00(A1,D7.L),D0
0000EB4E                          JMP       $00(A1,D0.W)
0000EB52                          OR.B      #$0E,D6
0000EB56                          OR.B      #$04,(A2)+
0000EB5A                          JSR       $0000E846(pc)
0000EB5E                          RTS
0000EB60                          MOVE.L    #$00000706,D0
0000EB66                          JSR       $0000E846(pc)
0000EB6A                          RTS
0000EB6C                          MOVE.W    #$0100,$00A11100
0000EB74                          LEA       $00C00004,A6
0000EB7A                          MOVE.W    #$8F01,(A6)
0000EB7E                          MOVE.L    #$93FF943F,(A6)
0000EB84                          MOVE.W    #$9780,(A6)
0000EB88                          MOVE.L    #$60000082,(A6)
0000EB8E                          MOVE.W    #$0000,$00C00000
0000EB96                          BTST      #$01,$0001(A6)
0000EB9C                          BNE       $0000EB96
0000EB9E                          MOVE.L    #$60000002,(A6)
0000EBA4                          MOVE.W    #$0000,$00C00000
0000EBAC                          MOVE.W    #$8F02,(A6)
0000EBB0                          MOVE.W    #$0000,$00A11100
0000EBB8                          MOVEQ     #$05,D0
0000EBBA                          JSR       $0000E846(pc)
0000EBBE                          RTS
0000EBC0                          MOVE.W    #$0000,$00FF700A
0000EBC8                          LEA       $00C00004,A2
0000EBCE                          LEA       $00C00000,A3
0000EBD4                          MOVE.W    D0,D2
0000EBD6                          MOVE.W    #$4000,D4
0000EBDA                          MOVEQ     #$03,D5
0000EBDC                          SUBQ.W    #$1,D0
0000EBDE                          BSR       $0000EBEA
0000EBE0                          MOVEQ     #$0B,D0
0000EBE2                          MOVE.W    #$6000,D4
0000EBE6                          SUB.W     D2,D0
0000EBE8                          MOVEQ     #$02,D5
0000EBEA                          LSL.W     #4,D0
0000EBEC                          LSL.W     #5,D0
0000EBEE                          ADD.W     D4,D0
0000EBF0                          MOVE.W    D0,(A2)
0000EBF2                          MOVE.W    D5,(A2)
0000EBF4                          MOVEQ     #$00,D1
0000EBF6                          MOVEQ     #$7F,D7
0000EBF8                          MOVE.L    D1,(A3)
0000EBFA                          DBRA      D7,$0000EBF8
0000EBFE                          RTS
0000EC00                          BTST      #$02,D3
0000EC04                          OR.B      #$01,D4
0000EC08                          Invalid
0000EC0A                          OR.W      -(A0),D0
0000EC0C                          CMP.B     #$80,D4
0000EC10                          ADD.B     -(A0),D0
0000EC12                          SUB.L     #$41603C04,D1
0000EC18                          BTST      D3,D1
0000EC1A                          ADD.L     (A7)+,D0
0000EC1C                          Invalid
0000EC1E                          Invalid
0000EC20                          MOVE.L    $3C04,-(A0)
0000EC24                          SBCD      -(A0),D1
0000EC26                          SUB.B     A2,D5
0000EC28                          TST.B     D4
0000EC2A                          ASR       $04(A6,D1.W)
0000EC2E                          BRA       $0000EC8F
0000EC30                          Invalid
0000EC32                          MOVE.L    D4,D2
0000EC34                          NEGX.L    D0
0000EC36                          SUB.W     -(A4),D0
0000EC38                          SUB.L     #$50508422,D1
0000EC3E                          BCHG      D0,$20(A0,D5.W)
0000EC42                          BSET      D4,-(A7)
0000EC44                          BTST      D0,-(A0)
0000EC46                          DBRA      D0,$0000FE4F
0000EC4A                          CMP.B     (A4),D4
0000EC4C                          Invalid
0000EC4E                          MOVE.W    (A4)+,A0
0000EC50                          BTST      #$C0,D6
0000EC54                          MOVEQ     #$08,D2
0000EC56                          BTST      D6,D3
0000EC58                          DIVU      -$2000(A7),D0
0000EC5C                          BTST      D4,D3
0000EC5E                          OR.B      #$0A,D2
0000EC62                          MOVEQ     #$F7,D6
0000EC64                          ADD.L     Invalid
0000EC66                          MOVE.L    (A7)+,$7798(A2)
0000EC6A                          BTST      D6,D2
0000EC6C                          AND.W     #$4C3E,(A4)+
0000EC70                          ADD.W     Invalid
0000EC72                          MOVE.L    D0,$80(A6,D0.L)
0000EC76                          BTST      #$69,(A3)
0000EC7A                          Invalid
0000EC7C                          DIVS      (A4),D0
0000EC7E                          SUB.B     #$78,D3
0000EC82                          SUB.B     #$6C,D1
0000EC86                          Invalid
0000EC88                          LE        $0000EC26
0000EC8A                          MOVE.B    D1,-(A1)
0000EC8C                          NEGX.L    $02(A6,D0.W)
0000EC90                          AND.W     A4,D4
0000EC92                          MOVE.B    -(A0),$51(A1,D5.L)
0000EC96                          MOVE.L    (A7)+,$57(A3,D4.L)
0000EC9A                          MOVE.B    D3,-(A4)
0000EC9C                          OR.W      $01(A5,D0.W),D0
0000ECA0                          ADD.B     #$F0,D2
0000ECA4                          MOVE.B    -$7EDE(A2),D0
0000ECA8                          MOVEQ     #$50,D6
0000ECAA                          MOVE.W    (A4)+,(A7)
0000ECAC                          MOVE.B    A7,A0
0000ECAE                          ADD.B     A1,D6
0000ECB0                          MULU      $08(A0,A4.W),D0
0000ECB4                          ASR       $02(A2,A2.W)
0000ECB8                          NEGX.B    -(A4)
0000ECBA                          EOR.B     #$87,-(A5)
0000ECBE                          MOVE.W    ,$1FD8(A0)
0000ECC2                          BTST      #$E8,(A3)
0000ECC6                          MOVE.B    D1,D0
0000ECC8                          BTST      D0,#$80
0000ECCC                          Invalid
0000ECCE                          BCHG      #$0A,-(A0)
0000ECD2                          ADDQ.L    #$0,$51(A4,A1.W)
0000ECD6                          ROXR.W    #8,D1
0000ECD8                          MOVE.B    -(A4),
0000ECDA                          OR.L      D4,D0
0000ECDC                          Invalid
0000ECDE                          BSET      D3,-(A6)
0000ECE0                          AND.B     #$14,D0
0000ECE4                          BCHG      D5,D0
0000ECE6                          SUB.B     (A2),D6
0000ECE8                          MOVEP.L   D0,$2824(A2)
0000ECEC                          MULS      D5,D7
0000ECEE                          MOVE.B    -(A3),(A4)
0000ECF0                          Invalid
0000ECF2                          MOVEQ     #$40,D7
0000ECF4                          ADDQ.B    #$0,A1
0000ECF6                          OR.L      D0,D0
0000ECF8                          MOVE.B    D4,D4
0000ECFA                          NEGX.W    D0
0000ECFC                          MOVE.W    A6,D2
0000ECFE                          BSET      D0,(A0)
0000ED00                          MOVE.B    D6,D1
0000ED02                          TST.W     D0
0000ED04                          ABCD.L    #$84,D7
0000ED06                          BTST      D4,-$6B6A(A1)
0000ED0A                          MULU      $01(A0,D0.L),D0
0000ED0E                          MOVE.B    A1,D2
0000ED10                          Invalid
0000ED12                          OR.B      D2,D4
0000ED14                          AND.B     -(A0),D0
0000ED16                          BCLR      D5,D0
0000ED18                          DIVU      -(A2),D0
0000ED1A                          BTST      D4,D6
0000ED1C                          Invalid
0000ED1E                          Invalid
0000ED20                          Invalid
0000ED22                          Invalid
0000ED24                          OR.W      #$0A02,A0
0000ED28                          MOVE      SR,A4
0000ED2A                          BCLR      D4,D0
0000ED2C                          NEGX.B    A2
0000ED2E                          BTST      D0,-(A0)
0000ED30                          MOVE.L    (A2)+,D0
0000ED32                          ÑÑâ.WCC   $0000ECFA
0000ED34                          ASR.B     D6,D0
0000ED36                          Invalid
0000ED38                          Invalid
0000ED3A                          LSR.B     D3,D3
0000ED3C                          Invalid
0000ED3E                          NEGX.B    (A1)
0000ED40                          BTST      D0,D0
0000ED42                          MOVE.W    A0,D0
0000ED44                          AND.W     #$1205,D0
0000ED48                          SUB.B     #$02,-(A0)
0000ED4C                          (CCC     #$2,$CA(A1,A0.L)
0000ED50                          MOVE.L    D2,$4A(A0,A2.W)
0000ED54                          MOVEQ     #$90,D1
0000ED56                          MOVE.L    -(A6),(A2)+
0000ED58                          ÑÑâ.WGE   $0000ED1F
0000ED5A                          SUBQ.W    #$7,$1402
0000ED5E                          NEGX.W    (A0)
0000ED60                          ADD.B     #$86,D1
0000ED64                          BSET      D2,D0
0000ED66                          NEGX.L    $04C0(A2)
0000ED6A                          MOVE.L    D3,(A1)+
0000ED6C                          PEA       $43(A7,D2.W)
0000ED70                          SUB.W     A3,A2
0000ED72                          MOVE.B    -(A0),D2
0000ED74                          CHK       D7,D4
0000ED76                          (CCC     #$4,-(A6)
0000ED78                          BSET      D3,(A0)+
0000ED7A                          MOVE.B    D2,$40(A1,D4.L)
0000ED7E                          SUB.L     #$38250142,D1
0000ED84                          Invalid
0000ED86                          OR.L      #$99661F03,(A0)
0000ED8C                          ASR.B     D3,D0
0000ED8E                          MOVE.L    -(A6),(A2)
0000ED90                          ððB       Internal check fail
0000ED92                          OR.W      A4,D1
0000ED94                          MOVE.W    A1,(A6)+
0000ED96                          MOVE.B    -(A0),$38(A7,D2.W)
0000ED9A                          SBCD      Invalid
0000ED9C                          ADD.B     #$48,D1
0000EDA0                          MOVE.B    D0,$A1(A4,D6.L)
0000EDA4                          BCHG      #$3D,-(A0)
0000EDA8                          (CMP.L    Internal check fail
0000EDAA                          ADDQ.W    #$6,(A0)
0000EDAC                          LEA       (A0),A4
0000EDAE                          CMP.B     A0,D3
0000EDB0                          MOVEQ     #$DA,D1
0000EDB2                          SUBQ.L    #$5,A3
0000EDB4                          ADDQ.W    #$2,D0
0000EDB6                          CHK       D7,D4
0000EDB8                          (CCC     #$4,
0000EDBA                          Invalid
0000EDBC                          CMP.W     +$EF(D7.L),A7
0000EDC0                          Invalid
0000EDC2                          CMP.W     +$EF(D7.L),A7
0000EDC6                          Invalid
0000EDC8                          CMP.W     +$EF(D7.L),A7
0000EDCC                          Invalid
0000EDCE                          Invalid
0000EDD0                          TST.B     $E195
0000EDD4                          BEQ       $0000EE16
0000EDD6                          CMP.B     #$02,$E195
0000EDDC                          BEQ       $0000EFE4(pc)
0000EDE0                          MOVE.B    #$01,$E197
0000EDE6                          MOVE.W    $E11A,D0
0000EDEA                          AND.W     #$007F,D0
0000EDEE                          BNE       $0000EDF2
0000EDF0                          BSR       $0000EE18
0000EDF2                          MOVE.W    $EA08,D0
0000EDF6                          SUB.W     $EA88,D0
0000EDFA                          CMP.W     #$0280,D0
0000EDFE                          BCC       $0000EE16
0000EE00                          SUB.L     #$00008000,$EA88
0000EE08                          TST.B     $E196
0000EE0C                          BEQ       $0000EE16
0000EE0E                          ADD.L     #$00008000,$A008
0000EE16                          RTS
0000EE18                          BTST      #$07,$E11B
0000EE1E                          BEQ       $0000EE80
0000EE20                          MOVE.B    #$08,$ED8D
0000EE26                          MOVE.B    #$09,$ED8E
0000EE2C                          MOVE.B    #$0A,$ED8F
0000EE32                          MOVE.B    #$0B,$ED90
0000EE38                          MOVE.B    #$0C,$ED91
0000EE3E                          MOVE.B    #$0D,$ED92
0000EE44                          MOVE.B    #$0E,$ED93
0000EE4A                          MOVE.B    #$17,$EDAD
0000EE50                          MOVE.B    #$13,$EDAE
0000EE56                          MOVE.B    #$14,$EDAF
0000EE5C                          MOVE.B    #$00,$EDB0
0000EE62                          MOVE.B    #$15,$EDB1
0000EE68                          MOVE.B    #$16,$EDB2
0000EE6E                          MOVE.B    #$00,$EDB3
0000EE74                          MOVE.B    #$01,$E191
0000EE7A                          LEA       $0000EF40(pc),A2
0000EE7E                          BRA       $0000EEDE
0000EE80                          MOVE.B    #$01,$ED8D
0000EE86                          MOVE.B    #$02,$ED8E
0000EE8C                          MOVE.B    #$03,$ED8F
0000EE92                          MOVE.B    #$04,$ED90
0000EE98                          MOVE.B    #$05,$ED91
0000EE9E                          MOVE.B    #$06,$ED92
0000EEA4                          MOVE.B    #$07,$ED93
0000EEAA                          MOVE.B    #$00,$EDAD
0000EEB0                          MOVE.B    #$0F,$EDAE
0000EEB6                          MOVE.B    #$10,$EDAF
0000EEBC                          MOVE.B    #$00,$EDB0
0000EEC2                          MOVE.B    #$11,$EDB1
0000EEC8                          MOVE.B    #$12,$EDB2
0000EECE                          MOVE.B    #$00,$EDB3
0000EED4                          MOVE.B    #$01,$E191
0000EEDA                          LEA       $0000EF92(pc),A2
0000EEDE                          MOVE.W    $EA88,D0
0000EEE2                          MOVE.W    $A008,D3
0000EEE6                          SUB.W     $EA08,D3
0000EEEA                          ADD.W     D3,D0
0000EEEC                          MOVE.W    $EA8C,D1
0000EEF0                          MOVE.W    $A00C,D3
0000EEF4                          SUB.W     $EA0C,D3
0000EEF8                          ADD.W     D3,D1
0000EEFA                          TST.B     $A003
0000EEFE                          BNE       $0000EF3E
0000EF00                          CMP.W     #$0460,D1
0000EF04                          BCS       $0000EF3E
0000EF06                          CMP.W     #$0500,D1
0000EF0A                          BCC       $0000EF3E
0000EF0C                          CMP.W     #$0D80,D0
0000EF10                          BCS       $0000EF3E
0000EF12                          CMP.W     #$1330,D0
0000EF16                          BCC       $0000EF3E
0000EF18                          MOVE.W    #$0D80,D1
0000EF1C                          CMP.W     #$1010,D0
0000EF20                          BCS       $0000EF2C
0000EF22                          CMP.W     #$10A0,D0
0000EF26                          BCS       $0000EF3E
0000EF28                          MOVE.W    #$10A0,D1
0000EF2C                          SUB.W     D1,D0
0000EF2E                          BCS       $0000EF3E
0000EF30                          LSR.W     #3,D0
0000EF32                          AND.W     #$007E,D0
0000EF36                          MOVE.W    $00(A2,D0.W),D0
0000EF3A                          ADD.W     Invalid
0000EF3C                          Invalid
0000EF3E                          RTS
0000EF40                          Invalid
0000EF42                          Invalid
0000EF44                          Invalid
0000EF46                          Invalid
0000EF48                          Invalid
0000EF4A                          OR.B      #$04,D0
0000EF4E                          OR.B      #$16,(A0)
0000EF52                          OR.B      #$14,(A4)+
0000EF56                          OR.B      #$FC,A0
0000EF5A                          Invalid
0000EF5C                          Invalid
0000EF5E                          Invalid
0000EF60                          Invalid
0000EF62                          Invalid
0000EF64                          Invalid
0000EF66                          Invalid
0000EF68                          Invalid
0000EF6A                          Invalid
0000EF6C                          Invalid
0000EF6E                          Invalid
0000EF70                          Invalid
0000EF72                          Invalid
0000EF74                          Invalid
0000EF76                          Invalid
0000EF78                          Invalid
0000EF7A                          Invalid
0000EF7C                          Invalid
0000EF7E                          Invalid
0000EF80                          Invalid
0000EF82                          Invalid
0000EF84                          Invalid
0000EF86                          Invalid
0000EF88                          Invalid
0000EF8A                          Invalid
0000EF8C                          Invalid
0000EF8E                          Invalid
0000EF90                          Invalid
0000EF92                          OR.B      #$10,A4
0000EF96                          OR.B      #$10,(A2)
0000EF9A                          OR.B      #$00,D4
0000EF9E                          Invalid
0000EFA0                          Invalid
0000EFA2                          Invalid
0000EFA4                          Invalid
0000EFA6                          Invalid
0000EFA8                          Invalid
0000EFAA                          OR.B      #$0C,D4
0000EFAE                          OR.B      #$10,(A0)
0000EFB2                          OR.B      #$10,(A0)
0000EFB6                          OR.B      #$10,(A0)
0000EFBA                          OR.B      #$10,(A0)
0000EFBE                          OR.B      #$10,(A0)
0000EFC2                          OR.B      #$10,(A0)
0000EFC6                          OR.B      #$10,(A0)
0000EFCA                          OR.B      #$10,(A0)
0000EFCE                          OR.B      #$10,(A0)
0000EFD2                          OR.B      #$10,(A0)
0000EFD6                          OR.B      #$10,(A0)
0000EFDA                          OR.B      #$10,(A0)
0000EFDE                          OR.B      #$1C,(A0)+
0000EFE2                          OR.B      #$FC,(A4)+
0000EFE6                          MOVE.B    D0,-(A2)
0000EFE8                          LSR.L     #5,D0
0000EFEA                          MOVE.W    #$0200,$EA8C
0000EFF0                          MOVE.B    #$01,$E191
0000EFF6                          MOVE.B    #$00,$E195
0000EFFC                          RTS
0000EFFE                          TST.B     $E193
0000F002                          BNE       $0000F00E
0000F004                          RTS
0000F006                          MOVE.B    #$00,$E193
0000F00C                          RTS
0000F00E                          CMP.B     #$03,$FC26
0000F014                          BNE       $0000F006
0000F016                          LEA       $A000,A1
0000F01A                          MOVE.W    $0008(A1),D0
0000F01E                          MOVE.W    $000C(A1),D1
0000F022                          ADD.W     #$0020,D1
0000F026                          TST.B     $0003(A1)
0000F02A                          BEQ       $0000F038
0000F02C                          CMP.B     #$08,$0003(A1)
0000F032                          BNE       $0000F004
0000F034                          SUB.W     #$0040,D1
0000F038                          CMP.W     #$0310,D1
0000F03C                          BLT       $0000F004
0000F03E                          CMP.W     #$0321,D1
0000F042                          BGE       $0000F004
0000F044                          CMP.B     #$01,$E193
0000F04A                          BNE       $0000F066(pc)
0000F04E                          CMP.W     #$03E0,D0
0000F052                          BLT       $0000F0D0
0000F054                          CMP.W     #$0420,D0
0000F058                          BGE       $0000F0D0
0000F05A                          LEA       $0000F0F2(pc),A1
0000F05E                          MOVE.B    #$02,$E193
0000F064                          BRA       $0000F07C
0000F066                          CMP.W     #$03A0,D0
0000F06A                          BLT       $0000F0D0
0000F06C                          CMP.W     #$03E0,D0
0000F070                          BGE       $0000F0D0
0000F072                          LEA       $0000F0D2(pc),A1
0000F076                          MOVE.B    #$01,$E193
0000F07C                          MOVE.L    (A1)+,$00FF3234
0000F082                          MOVE.L    (A1)+,$00FF3238
0000F088                          MOVE.L    (A1)+,$00FF323C
0000F08E                          MOVE.L    (A1)+,$00FF3420
0000F094                          MOVE.L    (A1)+,$00FF3254
0000F09A                          MOVE.L    (A1)+,$00FF3258
0000F0A0                          MOVE.L    (A1)+,$00FF325C
0000F0A6                          MOVE.L    (A1)+,$00FF3440
0000F0AC                          MOVE.W    #$03A0,D0
0000F0B0                          MOVE.W    #$0310,D1
0000F0B4                          JSR       $0000F724(pc)
0000F0B8                          ADD.W     #$0020,D0
0000F0BC                          JSR       $0000F724(pc)
0000F0C0                          ADD.W     #$0020,D0
0000F0C4                          JSR       $0000F724(pc)
0000F0C8                          ADD.W     #$0020,D0
0000F0CC                          JSR       $0000F724(pc)
0000F0D0                          RTS
0000F0D2                          BCHG      D0,
0000F0D4                          BCHG      D0,
0000F0D6                          AND.B     #$0D,A4
0000F0DA                          AND.B     #$0F,A6
0000F0DE                          AND.B     #$11,(A0)
0000F0E2                          AND.B     #$13,(A2)
0000F0E6                          AND.B     #$15,(A4)
0000F0EA                          AND.B     #$17,(A6)
0000F0EE                          BCHG      D0,
0000F0F0                          BCHG      D0,
0000F0F2                          AND.B     #$19,(A0)+
0000F0F6                          AND.B     #$1B,(A2)+
0000F0FA                          AND.B     #$1D,(A4)+
0000F0FE                          BCHG      D0,
0000F100                          BCHG      D0,
0000F102                          BCHG      D0,
0000F104                          BCHG      D0,
0000F106                          AND.B     #$15,(A6)+
0000F10A                          AND.B     #$1F,(A6)
0000F10E                          AND.B     #$21,-(A0)
0000F112                          TST.B     $E186
0000F116                          BEQ       $0000F162
0000F118                          LEA       $0000F208(pc),A1
0000F11C                          LEA       $00C00004,A2
0000F122                          LEA       $00C00000,A3
0000F128                          MOVE.W    #$1500,D0
0000F12C                          MOVE.W    #$0100,D1
0000F130                          CMP.B     #$04,$FC0A
0000F136                          BNE       $0000F140
0000F138                          MOVE.W    #$0200,D0
0000F13C                          MOVE.W    #$0200,D1
0000F140                          MOVEQ     #$25,D7
0000F142                          MOVE.B    (A1)+,D0
0000F144                          MOVE.B    (A1)+,D1
0000F146                          MOVEQ     #$00,D2
0000F148                          MOVEQ     #$00,D3
0000F14A                          BSR       $0000F164
0000F14C                          MOVEQ     #$10,D2
0000F14E                          MOVEQ     #$00,D3
0000F150                          BSR       $0000F164
0000F152                          MOVEQ     #$00,D2
0000F154                          MOVEQ     #$10,D3
0000F156                          BSR       $0000F164
0000F158                          MOVEQ     #$10,D2
0000F15A                          MOVEQ     #$10,D3
0000F15C                          BSR       $0000F164
0000F15E                          DBRA      D7,$0000F142
0000F162                          RTS
0000F164                          MOVEM.W   D0-D1,-(A7)
0000F168                          ADD.W     D2,D0
0000F16A                          ADD.W     D3,D1
0000F16C                          MOVE.W    D0,D2
0000F16E                          MOVE.W    D1,D3
0000F170                          SUB.W     $EA88,D2
0000F174                          CMP.W     #$0150,D2
0000F178                          BGE       $0000F1D4
0000F17A                          CMP.W     #$FFF0,D2
0000F17E                          BLE       $0000F1D4
0000F180                          SUB.W     $EA8C,D3
0000F184                          CMP.W     #$00F0,D3
0000F188                          BGE       $0000F1D4
0000F18A                          CMP.W     #$FFF0,D3
0000F18E                          BLE       $0000F1D4
0000F190                          MOVE.W    D0,D2
0000F192                          MOVE.W    D1,D3
0000F194                          LSL.W     #4,D3
0000F196                          AND.W     #$0F80,D3
0000F19A                          LSR.W     #2,D2
0000F19C                          AND.W     #$007C,D2
0000F1A0                          ADD.W     D3,D2
0000F1A2                          OR.W      #$6000,D2
0000F1A6                          SWAP      D2
0000F1A8                          MOVE.W    #$0003,D2
0000F1AC                          MOVE.L    D2,(A2)
0000F1AE                          JSR       $0000127C
0000F1B4                          MOVE.W    $E116,D3
0000F1B8                          MOVE.L    $E018,A4
0000F1BC                          LSL.W     #3,D3
0000F1BE                          MOVE.L    $30(A4,D0.W),(A3)
0000F1C2                          ADD.L     #$00800000,D2
0000F1C8                          MOVE.L    D2,(A2)
0000F1CA                          MOVE.L    $30(A4,D2.W),(A3)
0000F1CE                          MOVEM.W   (A7)+,D0-D1
0000F1D2                          RTS
0000F1D4                          LSL.W     #4,D1
0000F1D6                          AND.W     #$0F80,D1
0000F1DA                          LSR.W     #2,D0
0000F1DC                          AND.W     #$007C,D0
0000F1E0                          ADD.W     D1,D0
0000F1E2                          OR.W      #$6000,D0
0000F1E6                          SWAP      D0
0000F1E8                          MOVE.W    #$0003,D0
0000F1EC                          MOVE.L    D0,(A2)
0000F1EE                          MOVE.L    #$00000000,(A3)
0000F1F4                          ADD.L     #$00800000,D0
0000F1FA                          MOVE.L    D0,(A2)
0000F1FC                          MOVE.L    #$00000000,(A3)
0000F202                          MOVEM.W   (A7)+,D0-D1
0000F206                          RTS
0000F208                          NEGX.B    D0
0000F20A                          NEGX.B    -(A0)
0000F20C                          BRA       $0000F22E
0000F20E                          OR.B      -(A0),D0
0000F210                          Invalid
0000F212                          AND.B     -(A0),D0
0000F214                          MOVE.L    D0,A0
0000F216                          NEGX.W    D0
0000F218                          BRA       $0000F25A
0000F21A                          OR.W      D0,D0
0000F21C                          Invalid
0000F21E                          AND.W     D0,D0
0000F220                          OR.W      #$2060,-(A0)
0000F224                          NEGX.W    -(A0)
0000F226                          BRA       $0000F288
0000F228                          OR.W      -(A0),D0
0000F22A                          Invalid
0000F22C                          AND.W     -(A0),D0
0000F22E                          MOVE.L    D0,(A0)
0000F230                          NEGX.L    D0
0000F232                          BRA       $0000F1B4
0000F234                          OR.L      D0,D0
0000F236                          Invalid
0000F238                          AND.L     D0,D0
0000F23A                          ASR.L     #8,D0
0000F23C                          MOVE.L    -(A0),(A0)
0000F23E                          NEGX.L    -(A0)
0000F240                          BRA       $0000F1E2
0000F242                          OR.L      -(A0),D0
0000F244                          Invalid
0000F246                          AND.L     -(A0),D0
0000F248                          MOVE.L    D0,(A0)+
0000F24A                          MOVE      SR,D0
0000F24C                          BRA       $0000F20E
0000F24E                          DIVU      D0,D0
0000F250                          Invalid
0000F252                          Invalid
0000F254                          TST.B     $E187
0000F258                          BNE       $0000F25C
0000F25A                          RTS
0000F25C                          MOVE.B    $E107,D0
0000F260                          AND.B     #$03,D0
0000F264                          BEQ       $0000F268
0000F266                          RTS
0000F268                          LEA       $E120,A1
0000F26C                          MOVEQ     #$00,D6
0000F26E                          MOVEQ     #$02,D7
0000F270                          JSR       $000012EA
0000F276                          MULU      #$48,D0
0000F27A                          LEA       $0000F2BC(pc),A1
0000F27E                          ADD.W     D0,A1
0000F280                          MOVE.W    (A1)+,$00FF051C
0000F286                          MOVE.W    (A1)+,$00FF051E
0000F28C                          MOVE.W    (A1)+,$00FF053C
0000F292                          MOVE.W    (A1)+,$00FF053E
0000F298                          LEA       $00FF0700,A2
0000F29E                          JSR       $00001418
0000F2A4                          MOVE.W    #$0EE0,D0
0000F2A8                          MOVE.W    #$0180,D1
0000F2AC                          MOVEQ     #$08,D6
0000F2AE                          JSR       $0000F724(pc)
0000F2B2                          ADD.W     #$0020,D0
0000F2B6                          DBRA      D6,$0000F2AE
0000F2BA                          RTS
0000F2BC                          SUB.W     #$047C,+$04(A5.W)
0000F2C2                          SUB.L     #$047D047E,A4
0000F2C8                          SUB.W     #$0480,
0000F2CC                          SUB.L     #$04820483,D1
0000F2D2                          SUB.L     #$04850486,D4
0000F2D8                          SUB.L     #$04880489,D7
0000F2DE                          SUB.L     #$049B049C,A2
0000F2E4                          SUB.L     #$048E048F,A5
0000F2EA                          SUB.L     #$04910492,(A0)
0000F2F0                          SUB.L     #$04940495,(A3)
0000F2F6                          SUB.L     #$04970498,(A6)
0000F2FC                          SUB.L     #$049A049D,(A1)+
0000F302                          SUB.L     #$049F04A0,(A6)+
0000F308                          SUB.L     #$04A904A1,$04A2(A0)
0000F310                          SUB.W     #$047F,
0000F314                          SUB.L     #$048104A3,D0
0000F31A                          SUB.L     #$04870488,-(A4)
0000F320                          SUB.L     #$04A504A6,A1
0000F326                          SUB.L     #$04B104B2,-(A7)
0000F32C                          SUB.L     #$04AB048E,$048F(A2)
0000F334                          SUB.L     #$049104AC,(A0)
0000F33A                          SUB.L     #$04970498,$0499(A5)
0000F342                          SUB.L     #$04AF04B0,$04B3(A6)
0000F34A                          SUB.L     #$04B504B6,$04(A4,D6.L)
0000F352                          SUB.L     #$04B704B8,
0000F358                          SUB.L     #$047E047F,-(A2)
0000F35E                          SUB.L     #$04B904BA,D0
0000F364                          SUB.L     #$04870488,D6
0000F36A                          SUB.L     #$04A504BB,A1
0000F370                          Invalid
0000F372                          Invalid
0000F374                          SUB.L     #$04BF04AB,
0000F37A                          SUB.L     #$048F0490,A6
0000F380                          Invalid
0000F382                          Invalid
0000F384                          SUB.L     #$04970498,(A6)
0000F38A                          SUB.L     #$04AE04C2,(A1)+
0000F390                          Invalid
0000F392                          Invalid
0000F394                          MOVEQ     #$00,D0
0000F396                          MOVE.B    $0002(A0),D0
0000F39A                          MOVE.W    +$00(D3.W),D0
0000F39E                          JMP       +$00(D1.W)
0000F3A2                          OR.B      #$BC,A0
0000F3A6                          BTST      D0,(A6)
0000F3A8                          BTST      D0,(A6)+
0000F3AA                          TST.B     $0003(A0)
0000F3AE                          BNE       $0000F3D6
0000F3B0                          LEA       $0000F648(pc),A6
0000F3B4                          MOVE.L    A6,$001A(A0)
0000F3B8                          MOVEQ     #$00,D0
0000F3BA                          LEA       $0000F64C(pc),A4
0000F3BE                          JSR       $000287D6
0000F3C4                          MOVE.B    #$3C,$0035(A0)
0000F3CA                          MOVE.B    #$02,$0003(A0)
0000F3D0                          JMP       $00028BF8
0000F3D6                          CMP.B     #$04,$0006(A0)
0000F3DC                          BGE       $0000F436
0000F3DE                          TST.B     $001E(A0)
0000F3E2                          BEQ       $0000F458
0000F3E4                          MOVEM.L   D0-D7/A0-A6,-(A7)
0000F3E8                          MOVEQ     #$00,D0
0000F3EA                          LEA       $00060DB6,A0
0000F3F0                          LEA       $00FF3000,A1
0000F3F6                          JSR       $0002077E
0000F3FC                          MOVE.B    #$83,$EB76
0000F402                          MOVE.B    #$81,$EB77
0000F408                          MOVE.B    #$82,$EB78
0000F40E                          MOVEQ     #$05,D0
0000F410                          JSR       $00029062
0000F416                          MOVEM.L   (A7)+,D0-D7/A0-A6
0000F41A                          MOVE.B    #$00,$001E(A0)
0000F420                          CMP.B     #$03,$0006(A0)
0000F426                          BGE       $0000F436
0000F428                          CMP.B     #$01,$0006(A0)
0000F42E                          BNE       $0000F436
0000F430                          MOVE.B    #$01,$F187
0000F436                          MOVEQ     #$00,D0
0000F438                          MOVE.B    $0006(A0),D0
0000F43C                          ADD.W     D0,D0
0000F43E                          ADD.W     D0,D0
0000F440                          LEA       $0000F4C6(pc),A2
0000F444                          MOVE.L    $00(A2,D0.W),A2
0000F448                          MOVE.L    A2,$0036(A0)
0000F44C                          MOVE.B    #$02,$0002(A0)
0000F452                          MOVE.B    #$00,$0003(A0)
0000F458                          JMP       $00028BF8
0000F45E                          TST.B     $0003(A0)
0000F462                          BNE       $0000F4AA
0000F464                          MOVE.L    $0036(A0),A2
0000F468                          MOVE.W    (A2),D0
0000F46A                          BEQ       $0000F4A2
0000F46C                          JSR       $00028A06
0000F472                          BCS       $0000F49C
0000F474                          MOVE.B    #$05,$0017(A1)
0000F47A                          MOVE.W    #$0100,$0000(A1)
0000F480                          MOVE.W    (A2)+,$0008(A1)
0000F484                          MOVE.W    (A2)+,$000C(A1)
0000F488                          MOVE.B    (A2)+,$0034(A1)
0000F48C                          MOVE.B    (A2)+,D0
0000F48E                          TST.B     D0
0000F490                          BEQ       $0000F468
0000F492                          MOVE.B    D0,$0035(A0)
0000F496                          MOVE.B    #$02,$0003(A0)
0000F49C                          MOVE.L    A2,$0036(A0)
0000F4A0                          RTS
0000F4A2                          MOVE.B    #$04,$0002(A0)
0000F4A8                          RTS
0000F4AA                          SUBQ.B    #$1,$0035(A0)
0000F4AE                          BNE       $0000F4B6
0000F4B0                          MOVE.B    #$00,$0003(A0)
0000F4B6                          RTS
0000F4B8                          MOVE.B    #$06,$0002(A0)
0000F4BE                          RTS
0000F4C0                          JMP       $00028AF8
0000F4C6                          OR.B      #$DE,D0
0000F4CA                          OR.B      #$DE,D0
0000F4CE                          OR.B      #$DE,D0
0000F4D2                          OR.B      #$DE,D0
0000F4D6                          OR.B      #$DE,D0
0000F4DA                          OR.B      #$DE,D0
0000F4DE                          MOVE.B    -(A0),+$03(D0.W)
0000F4E2                          BTST      D0,D0
0000F4E4                          MOVE.B    D0,+$03(D0.W)
0000F4E8                          BTST      D0,D0
0000F4EA                          MOVE.B    -(A0),$03(A3,D0.W)
0000F4EE                          BTST      D0,D0
0000F4F0                          MOVE.B    D0,$03(A3,D0.W)
0000F4F4                          BTST      D0,D0
0000F4F6                          MOVE.B    -(A0),$03E0(A3)
0000F4FA                          BTST      D0,D0
0000F4FC                          MOVE.B    -(A0),$03(A3,D0.W)
0000F500                          BTST      D0,D0
0000F502                          MOVE.B    -(A0),(A4)+
0000F504                          BSET      D1,-(A0)
0000F506                          BTST      D0,D0
0000F508                          MOVE.B    D0,(A4)+
0000F50A                          BSET      D1,-(A0)
0000F50C                          BTST      D0,D0
0000F50E                          MOVE.B    -(A0),(A4)
0000F510                          BSET      D1,-(A0)
0000F512                          BTST      D0,D0
0000F514                          MOVE.B    D0,(A4)
0000F516                          BSET      D1,-(A0)
0000F518                          BTST      D0,D0
0000F51A                          MOVE.B    -(A0),A4
0000F51C                          BSET      D1,-(A0)
0000F51E                          BTST      D0,D0
0000F520                          MOVE.B    D0,A4
0000F522                          BSET      D1,-(A0)
0000F524                          BTST      D0,D0
0000F526                          MOVE.B    -(A0),D4
0000F528                          BSET      D1,-(A0)
0000F52A                          BTST      D0,D0
0000F52C                          MOVE.B    D0,D4
0000F52E                          BSET      D1,-(A0)
0000F530                          BTST      D0,D1
0000F532                          MOVE.B    -(A0),A4
0000F534                          BSET      D1,D0
0000F536                          BTST      D0,D0
0000F538                          MOVE.B    D0,A4
0000F53A                          BSET      D1,D0
0000F53C                          BTST      D0,D0
0000F53E                          MOVE.B    D0,A4
0000F540                          BCLR      D1,-(A0)
0000F542                          BTST      D0,D0
0000F544                          MOVE.B    -(A0),D4
0000F546                          BSET      D1,D0
0000F548                          BTST      D0,D1
0000F54A                          MOVE.B    D0,A4
0000F54C                          BCLR      D1,D0
0000F54E                          BTST      D0,D0
0000F550                          MOVE.B    -(A0),D4
0000F552                          BCLR      D1,-(A0)
0000F554                          BTST      D0,D0
0000F556                          MOVE.B    D0,D4
0000F558                          BSET      D1,D0
0000F55A                          BTST      D0,D1
0000F55C                          MOVE.B    -(A0),D4
0000F55E                          BCLR      D1,D0
0000F560                          BTST      D0,D0
0000F562                          MOVE.B    D0,D4
0000F564                          BCLR      D1,-(A0)
0000F566                          BTST      D0,D0
0000F568                          MOVE.B    -(A0),+$03(D0.W)
0000F56C                          BTST      D0,D1
0000F56E                          MOVE.B    -(A0),D4
0000F570                          BCHG      D1,-(A0)
0000F572                          BTST      D0,D0
0000F574                          MOVE.B    D0,D4
0000F576                          BCLR      D1,D0
0000F578                          BTST      D0,D0
0000F57A                          MOVE.B    -(A0),+$03(D0.W)
0000F57E                          BTST      D0,D0
0000F580                          MOVE.B    D0,+$03(D0.W)
0000F584                          BTST      D0,D1
0000F586                          MOVE.B    D0,D4
0000F588                          BCHG      D1,-(A0)
0000F58A                          BTST      D0,D0
0000F58C                          MOVE.B    -(A0),+$03(D0.W)
0000F590                          BTST      D0,D0
0000F592                          MOVE.B    D0,+$03(D0.W)
0000F596                          BTST      D0,D0
0000F598                          MOVE.B    -(A0),$03(A3,D0.W)
0000F59C                          BTST      D0,D1
0000F59E                          MOVE.B    D0,D4
0000F5A0                          BCHG      D1,D0
0000F5A2                          BTST      D0,D0
0000F5A4                          MOVE.B    -(A0),+$03(D0.W)
0000F5A8                          BTST      D0,D0
0000F5AA                          MOVE.B    D0,+$03(D0.W)
0000F5AE                          BTST      D0,D0
0000F5B0                          MOVE.B    -(A0),$03(A3,D0.W)
0000F5B4                          BTST      D0,D0
0000F5B6                          MOVE.B    D0,$03(A3,D0.W)
0000F5BA                          BTST      D0,D1
0000F5BC                          MOVE.B    -(A0),+$03(D0.W)
0000F5C0                          BTST      D0,D0
0000F5C2                          MOVE.B    D0,+$03(D0.W)
0000F5C6                          BTST      D0,D0
0000F5C8                          MOVE.B    -(A0),$03(A3,D0.W)
0000F5CC                          BTST      D0,D0
0000F5CE                          MOVE.B    D0,$03(A3,D0.W)
0000F5D2                          BTST      D0,D0
0000F5D4                          MOVE.B    -(A0),$03C0(A3)
0000F5D8                          BTST      D0,D1
0000F5DA                          MOVE.B    -(A0),+$03(D0.W)
0000F5DE                          BTST      D0,D0
0000F5E0                          MOVE.B    D0,+$03(D0.W)
0000F5E4                          BTST      D0,D0
0000F5E6                          MOVE.B    -(A0),$03(A3,D0.W)
0000F5EA                          BTST      D0,D0
0000F5EC                          MOVE.B    D0,$03(A3,D0.W)
0000F5F0                          BTST      D0,D0
0000F5F2                          MOVE.B    -(A0),$03A0(A3)
0000F5F6                          BTST      D0,D1
0000F5F8                          MOVE.B    D0,+$03(D0.W)
0000F5FC                          BTST      D0,D0
0000F5FE                          MOVE.B    -(A0),$03(A3,D0.W)
0000F602                          BTST      D0,D0
0000F604                          MOVE.B    D0,$03(A3,D0.W)
0000F608                          BTST      D0,D0
0000F60A                          MOVE.B    -(A0),$0380(A3)
0000F60E                          BTST      D0,D1
0000F610                          MOVE.B    -(A0),$03(A3,D0.W)
0000F614                          BTST      D0,D0
0000F616                          MOVE.B    D0,$03(A3,D0.W)
0000F61A                          BTST      D0,D0
0000F61C                          MOVE.B    -(A0),$0360(A3)
0000F620                          BTST      D0,D1
0000F622                          MOVE.B    -(A0),$03(A3,D0.W)
0000F626                          BTST      D0,D0
0000F628                          MOVE.B    D0,$03(A3,D0.W)
0000F62C                          BTST      D0,D0
0000F62E                          MOVE.B    -(A0),$0340(A3)
0000F632                          BTST      D0,D1
0000F634                          MOVE.B    D0,$03(A3,D0.W)
0000F638                          BTST      D0,D0
0000F63A                          MOVE.B    -(A0),$0320(A3)
0000F63E                          BTST      D0,D0
0000F640                          MOVE.B    -(A0),$0300(A3)
0000F644                          BTST      D0,D1
0000F646                          OR.B      #$00,D0
0000F64A                          MOVE.W    $00(A0,D3.W),D0
0000F64E                          OR.B      #$01,D0
0000F652                          OR.B      #$00,D0
0000F656                          OR.B      #$00,(A0)
0000F65A                          MOVEQ     #$00,D0
0000F65C                          MOVE.B    $0002(A0),D0
0000F660                          MOVE.W    +$00(D3.W),D1
0000F664                          JMP       +$10(D1.W)
0000F668                          OR.B      #$18,A0
0000F66C                          OR.L      #$00AE4DFA,-(A6)
0000F672                          MOVEP.L   $214E(A2),D0
0000F676                          OR.B      #$7C,(A2)+
0000F67A                          OR.B      #$02,D2
0000F67E                          RTS
0000F680                          MOVEQ     #$00,D0
0000F682                          MOVE.B    $0003(A0),D0
0000F686                          MOVE.W    +$00(D6.W),D1
0000F68A                          JSR       +$10(D4.W)
0000F68E                          JMP       $00028BE8
0000F694                          OR.B      #$20,A0
0000F698                          OR.W      #$0068,D0
0000F69C                          MOVE.B    #$02,$0003(A0)
0000F6A2                          MOVE.B    #$05,$0035(A0)
0000F6A8                          MOVEQ     #$00,D0
0000F6AA                          LEA       $0000F7D0(pc),A4
0000F6AE                          JMP       $000287D6
0000F6B4                          SUBQ.B    #$1,$0035(A0)
0000F6B8                          BNE       $0000F6D2
0000F6BA                          MOVE.B    #$04,$0003(A0)
0000F6C0                          MOVE.B    #$02,$0035(A0)
0000F6C6                          MOVEQ     #$01,D0
0000F6C8                          LEA       $0000F7D0(pc),A4
0000F6CC                          JMP       $000287D6
0000F6D2                          RTS
0000F6D4                          SUBQ.B    #$1,$0035(A0)
0000F6D8                          BNE       $0000F6FA
0000F6DA                          TST.B     $0034(A0)
0000F6DE                          BEQ       $0000F6E2
0000F6E0                          BSR       $0000F71C
0000F6E2                          MOVE.B    #$06,$0003(A0)
0000F6E8                          MOVE.B    #$0A,$0035(A0)
0000F6EE                          MOVEQ     #$02,D0
0000F6F0                          LEA       $0000F7D0(pc),A4
0000F6F4                          JMP       $000287D6
0000F6FA                          RTS
0000F6FC                          SUBQ.B    #$1,$0035(A0)
0000F700                          BNE       $0000F708
0000F702                          MOVE.B    #$04,$0002(A0)
0000F708                          JMP       $00028804
0000F70E                          MOVE.B    #$06,$0002(A0)
0000F714                          RTS
0000F716                          JMP       $00028B08
0000F71C                          MOVE.W    $0008(A0),D0
0000F720                          MOVE.W    $000C(A0),D1
0000F724                          MOVEQ     #$00,D2
0000F726                          MOVEQ     #$00,D3
0000F728                          BSR       $0000F73A
0000F72A                          MOVEQ     #$10,D2
0000F72C                          MOVEQ     #$00,D3
0000F72E                          BSR       $0000F73A
0000F730                          MOVEQ     #$00,D2
0000F732                          MOVEQ     #$10,D3
0000F734                          BSR       $0000F73A
0000F736                          MOVEQ     #$10,D2
0000F738                          MOVEQ     #$10,D3
0000F73A                          MOVEM.W   D0-D1,-(A7)
0000F73E                          ADD.W     D2,D0
0000F740                          ADD.W     D3,D1
0000F742                          MOVE.W    D0,D2
0000F744                          MOVE.W    D1,D3
0000F746                          SUB.W     $EA08,D2
0000F74A                          CMP.W     #$0150,D2
0000F74E                          BGE       $0000F7B6
0000F750                          CMP.W     #$FFF0,D2
0000F754                          BLE       $0000F7B6
0000F756                          SUB.W     $EA0C,D3
0000F75A                          CMP.W     #$00F0,D3
0000F75E                          BGE       $0000F7B6
0000F760                          CMP.W     #$FFF0,D3
0000F764                          BLE       $0000F7B6
0000F766                          MOVE.W    D0,D2
0000F768                          MOVE.W    D1,D3
0000F76A                          LSL.W     #4,D3
0000F76C                          AND.W     #$0F80,D3
0000F770                          LSR.W     #2,D2
0000F772                          AND.W     #$007C,D2
0000F776                          ADD.W     D3,D2
0000F778                          OR.W      #$4000,D2
0000F77C                          SWAP      D2
0000F77E                          MOVE.W    #$0003,D2
0000F782                          MOVE.L    D2,$00C00004
0000F788                          MOVE.L    D2,D5
0000F78A                          JSR       $0000121C
0000F790                          MOVE.W    $E116,D4
0000F794                          MOVE.L    $E018,A1
0000F798                          LSL.W     #3,D4
0000F79A                          MOVE.L    $40(A1,D0.W),$00C00000
0000F7A2                          ADD.L     #$00800000,D5
0000F7A8                          MOVE.L    D5,$00C00004
0000F7AE                          MOVE.L    $40(A1,D2.W),$00C00000
0000F7B6                          MOVEM.W   (A7)+,D0-D1
0000F7BA                          RTS
0000F7BC                          OR.B      #$01,D0
0000F7C0                          OR.B      #$01,D0
0000F7C4                          OR.B      #$01,D0
0000F7C8                          OR.B      #$01,D0
0000F7CC                          OR.B      #$01,D0
0000F7D0                          OR.B      #$24,(A2)+
0000F7D4                          OR.B      #$02,(A2)+
0000F7D8                          OR.B      #$08,(A0)
0000F7DC                          OR.B      #$4E,D0
0000F7E0                          BCLR      #$18,(A1)+
0000F7E4                          OR.B      A0,D0
0000F7E6                          OR.B      #$4E,D0
0000F7EA                          BTST      #$18,D7
0000F7EE                          OR.B      A0,D0
0000F7F0                          OR.B      #$6A,D0
0000F7F4                          BTST      #$12,D7
0000F7F8                          OR.B      D4,D0
0000F7FA                          OR.B      #$86,D0
0000F7FE                          BSET      #$19,-(A0)
0000F802                          OR.L      D2,D0
0000F804                          OR.B      #$98,D0
0000F808                          OR.B      #$00,(A2)+
0000F80C                          Invalid
0000F80E                          OR.B      #$E8,$0E(A4,D0.W)
0000F814                          MOVE.W    (A0)+,(A6)+
0000F816                          Invalid
0000F818                          OR.W      #$F800,A0
0000F81C                          BTST      D6,(A0)
0000F81E                          MOVE.W    $000E,(A2)
0000F822                          MOVE.B    #$00,D0
0000F826                          SUB.B     #$03,D0
0000F82A                          OR.W      #$B8E8,A4
0000F82E                          .B        #$C0,D0
0000F832                          LSR.B     #4,D6
0000F834                          OR.W      #$E000,(A4)+
0000F838                          .B        #$C0,(A0)
0000F83C                          OR.B      #$5C,A6
0000F840                          ASR.B     #8,D0
0000F842                          AND.B     #$07,D0
0000F846                          OR.W      #$90F0,$0D00(A0)
0000F84C                          MOVEQ     #$A0,D0
0000F84E                          OR.B      #$70,A5
0000F852                          Invalid
0000F854                          AND.B     #$0E,D0
0000F858                          BCHG      #$04,(A4)+
0000F85C                          BTST      D4,D0
0000F85E                          MOVEQ     #$B0,D4
0000F860                          LSR.B     #6,D1
0000F862                          MOVE.B    $C800,A4
0000F866                          NOP
0000F868                          MOVEQ     #$00,D0
0000F86A                          MOVE.B    $0002(A0),D0
0000F86E                          ADD.W     +$00(D3.W),D0
0000F872                          JMP       +$00(D1.W)
0000F876                          OR.B      #$5A,A0
0000F87A                          OR.B      #$50,$4D(A6,A4.L)
0000F880                          OR.B      #$38,A4
0000F884                          MOVE.L    A6,$001A(A0)
0000F888                          MOVE.B    #$03,$0016(A0)
0000F88E                          MOVE.B    $FC0A,$003E(A0)
0000F894                          MOVE.B    $FC26,$003F(A0)
0000F89A                          MOVE.W    $000C5188,$0028(A0)
0000F8A2                          MOVE.W    #$0200,$0002(A0)
0000F8A8                          RTS
0000F8AA                          MOVE.W    #$0400,$0002(A0)
0000F8B0                          MOVEQ     #$0A,D0
0000F8B2                          JSR       $00020906
0000F8B8                          JSR       $000289BE
0000F8BE                          JMP       $00029FCE
0000F8C4                          MOVE.W    #$0600,$0002(A0)
0000F8CA                          RTS
0000F8CC                          JMP       $00028AF8
0000F8D2                          CMP.B     #$01,$001E(A0)
0000F8D8                          BCC       $0000F8AA
0000F8DA                          MOVEQ     #$00,D0
0000F8DC                          MOVE.B    $0003(A0),D0
0000F8E0                          ADD.W     +$00(D3.W),D0
0000F8E4                          JMP       +$00(D1.W)
0000F8E8                          OR.B      #$30,A0
0000F8EC                          OR.W      #$00F2,$11(A0,D6.L)
0000F8F2                          OR.B      #$03,D2
0000F8F6                          MOVEQ     #$00,D0
0000F8F8                          MOVE.B    $0006(A0),D0
0000F8FC                          ADD.W     D0,D0
0000F8FE                          LEA       $0000FAD2(pc),A1
0000F902                          ADD.W     $00(A1,D0.W),A1
0000F906                          MOVE.L    A1,$003A(A0)
0000F90A                          MOVE.B    #$01,$0038(A0)
0000F910                          MOVE.B    #$00,$0039(A0)
0000F916                          BRA       $0000F95C(pc)
0000F91A                          MOVE.W    $A008,D0
0000F91E                          SUB.W     $0008(A0),D0
0000F922                          ADD.W     #$00C0,D0
0000F926                          CMP.W     #$0180,D0
0000F92A                          BCC       $0000FA78(pc)
0000F92E                          MOVE.W    $A00C,D0
0000F932                          SUB.W     $000C(A0),D0
0000F936                          ADD.W     #$0100,D0
0000F93A                          CMP.W     #$0200,D0
0000F93E                          BCC       $0000FA78(pc)
0000F942                          MOVE.B    #$04,$0003(A0)
0000F948                          MOVE.B    #$78,$0038(A0)
0000F94E                          MOVE.B    #$78,$0039(A0)
0000F954                          AND.B     #$02,D0
0000F958                          OR.B      #$00,D0
0000F95C                          MOVEQ     #$00,D0
0000F95E                          MOVE.B    $003F(A0),D0
0000F962                          MOVE.B    +$00(D0.L),D0
0000F966                          CMP.B     $FC26,D0
0000F96A                          BEQ       $0000F8C4(pc)
0000F96E                          SUBQ.B    #$1,$0038(A0)
0000F972                          BNE       $0000FA18(pc)
0000F976                          MOVE.L    $003A(A0),A1
0000F97A                          MOVE.B    (A1)+,$0038(A0)
0000F97E                          BEQ       $0000F9A2(pc)
0000F982                          MOVE.B    (A1)+,$0039(A0)
0000F986                          MOVE.B    (A1)+,$0016(A0)
0000F98A                          MOVEQ     #$00,D0
0000F98C                          MOVE.B    (A1)+,D0
0000F98E                          MOVE.L    A1,$003A(A0)
0000F992                          LEA       $000C518E,A4
0000F998                          JSR       $000287D6
0000F99E                          BRA       $0000FA18(pc)
0000F9A2                          MOVE.B    #$06,$0003(A0)
0000F9A8                          MOVEQ     #$00,D0
0000F9AA                          MOVE.B    $0006(A0),D0
0000F9AE                          ADD.W     D0,D0
0000F9B0                          LEA       $0000FAD2(pc),A1
0000F9B4                          ADD.W     $00(A1,D0.W),A1
0000F9B8                          MOVE.L    A1,$003A(A0)
0000F9BC                          MOVE.B    $0002(A1),$0016(A0)
0000F9C2                          MOVE.B    $0003(A1),D0
0000F9C6                          LEA       $000C518E,A4
0000F9CC                          JSR       $000287D6
0000F9D2                          MOVEQ     #$00,D0
0000F9D4                          MOVE.B    $FC09,D0
0000F9D8                          ADD.W     D0,D0
0000F9DA                          MOVE.W    +$00(D6.L),$0038(A0)
0000F9E0                          SUBQ.W    #$1,$0038(A0)
0000F9E4                          BNE       $0000FA78(pc)
0000F9E8                          MOVE.B    #$04,$0003(A0)
0000F9EE                          MOVE.B    #$01,$0038(A0)
0000F9F4                          BRA       $0000FA78(pc)
0000F9F8                          BSET      D0,-(A0)
0000F9FA                          BCLR      D0,-(A4)
0000F9FC                          BCHG      D0,$012C(A0)
0000FA00                          BTST      D0,(A5)+
0000FA02                          MOVEP.W   $00FF(A6),D0
0000FA06                          Invalid
0000FA08                          Invalid
0000FA0A                          Invalid
0000FA0C                          OR.L      #$00960078,$00(A4,D5.L)
0000FA14                          OR.B      #$1E,CCR
0000FA18                          SUBQ.B    #$1,$0039(A0)
0000FA1C                          BMI       $0000FA72(pc)
0000FA20                          BNE       $0000FA78(pc)
0000FA24                          JSR       $00028AA8
0000FA2A                          BCS       $0000FA78(pc)
0000FA2E                          MOVE.W    #$0101,(A1)
0000FA32                          MOVE.B    #$02,$0017(A1)
0000FA38                          MOVE.W    $0008(A0),$0008(A1)
0000FA3E                          MOVE.W    $000C(A0),$000C(A1)
0000FA44                          MOVE.B    $0016(A0),$0016(A1)
0000FA4A                          LEA       $0000FA7E(pc),A2
0000FA4E                          MOVEQ     #$00,D0
0000FA50                          MOVE.B    $0016(A0),D0
0000FA54                          ADD.B     $0006(A0),D0
0000FA58                          ADD.W     D0,D0
0000FA5A                          ADD.W     D0,D0
0000FA5C                          ADD.W     D0,A2
0000FA5E                          MOVE.W    (A2)+,D0
0000FA60                          ADD.W     $0008(A1),D0
0000FA64                          MOVE.W    D0,$0008(A1)
0000FA68                          MOVE.W    $000C(A1),D0
0000FA6C                          SUB.W     (A2)+,D0
0000FA6E                          MOVE.W    D0,$000C(A1)
0000FA72                          MOVE.B    #$00,$0039(A0)
0000FA78                          JMP       $00028BF8
0000FA7E                          OR.B      #$00,D0
0000FA82                          OR.B      #$00,D0
0000FA86                          OR.B      #$15,(A2)
0000FA8A                          OR.B      #$0C,(A2)+
0000FA8E                          OR.B      #$FB,-(A4)
0000FA92                          OR.B      #$05,-(A4)
0000FA96                          OR.B      #$F4,(A2)+
0000FA9A                          OR.B      #$EB,(A2)
0000FA9E                          OR.B      #$00,D0
0000FAA2                          OR.B      #$00,D0
0000FAA6                          OR.B      #$00,D0
0000FAAA                          OR.B      #$00,D0
0000FAAE                          Invalid
0000FAB0                          Invalid
0000FAB2                          Invalid
0000FAB4                          Invalid
0000FAB6                          Invalid
0000FAB8                          OR.B      #$DC,D5
0000FABC                          Invalid
0000FABE                          Invalid
0000FAC0                          OR.B      #$EE,A4
0000FAC4                          OR.B      #$00,(A5)
0000FAC8                          OR.B      #$08,D0
0000FACC                          OR.B      #$32,(A5)+
0000FAD0                          OR.W      #$000C,D7
0000FAD4                          OR.B      #$36,-(A1)
0000FAD8                          OR.W      #$1E00,A3
0000FADC                          AND.B     #$3C,D0
0000FAE0                          AND.B     #$3C,D0
0000FAE4                          BTST      D1,D1
0000FAE6                          MOVEQ     #$3C,D4
0000FAE8                          SUB.B     #$00,D2
0000FAEC                          SUB.B     #$1E,D0
0000FAF0                          OR.B      #$78,D6
0000FAF4                          MOVE.W    D6,D6
0000FAF6                          BCHG      D2,$3C05
0000FAFA                          SUB.W     #$3C04,$033C
0000FB00                          OR.B      #$00,D4
0000FB04                          MOVE.B    D0,D7
0000FB06                          EOR.B     #$3C,D6
0000FB0A                          EOR.B     #$3C,D6
0000FB0E                          BTST      D5,D7
0000FB10                          MOVEQ     #$3C,D4
0000FB12                          CMP.B     #$00,A0
0000FB16                          CMP.B     #$1E,A0
0000FB1A                          OR.B      #$78,A6
0000FB1E                          MOVE.W    A6,D6
0000FB20                          BCHG      D5,$3C0D
0000FB24                          EOR.W     #$3C0C,$093C
0000FB2A                          OR.B      #$00,A4
0000FB2E                          MOVE.L    $001A(A0),A6
0000FB32                          MOVEQ     #$00,D0
0000FB34                          MOVE.B    $0002(A0),D0
0000FB38                          ADD.W     +$00(D3.W),D0
0000FB3C                          JMP       +$00(D1.W)
0000FB40                          OR.B      #$46,A0
0000FB44                          BCLR      D0,D4
0000FB46                          MOVEP.W   D0,$4EB9(A2)
0000FB4A                          OR.B      #$7E,D2
0000FB4E                          BEQ       $0000FB86
0000FB50                          MOVE.W    $0008(A0),$002C(A0)
0000FB56                          MOVE.W    #$0DF0,$002E(A0)
0000FB5C                          TST.B     $0006(A0)
0000FB60                          BEQ       $0000FB68
0000FB62                          MOVE.W    #$0E90,$002E(A0)
0000FB68                          LEA       $000C4D4C,A6
0000FB6E                          MOVE.L    A6,$001A(A0)
0000FB72                          MOVE.B    #$02,$0002(A0)
0000FB78                          MOVEQ     #$00,D0
0000FB7A                          LEA       $000C4D60,A4
0000FB80                          JMP       $000287D6
0000FB86                          RTS
0000FB88                          JSR       $0000FC74(pc)
0000FB8C                          MOVEQ     #$00,D0
0000FB8E                          MOVE.B    $0003(A0),D0
0000FB92                          MOVE.W    +$00(D6.W),D0
0000FB96                          JSR       +$00(D4.W)
0000FB9A                          JMP       $00028BF8
0000FBA0                          OR.B      #$26,D6
0000FBA4                          OR.L      #$4A28002A,
0000FBAA                          BEQ       $0000FBC4
0000FBAC                          MOVE.B    #$02,$0003(A0)
0000FBB2                          MOVE.B    #$00,$002B(A0)
0000FBB8                          MOVE.B    #$2C,$A003
0000FBBE                          MOVE.W    #$0000,$A004
0000FBC4                          RTS
0000FBC6                          MOVE.W    $F09E,D1
0000FBCA                          AND.W     #$0001,D1
0000FBCE                          BNE       $0000FBD2
0000FBD0                          BSR       $0000FC08
0000FBD2                          ADD.W     #$000D,$0008(A0)
0000FBD8                          MOVE.W    $0008(A0),D0
0000FBDC                          CMP.W     $002E(A0),D0
0000FBE0                          BCS       $0000FBF6
0000FBE2                          MOVE.B    #$04,$0003(A0)
0000FBE8                          TST.B     $002A(A0)
0000FBEC                          BEQ       $0000FC02
0000FBEE                          MOVE.B    #$04,$A004
0000FBF4                          RTS
0000FBF6                          TST.B     $002A(A0)
0000FBFA                          BEQ       $0000FC02
0000FBFC                          ADD.W     #$000D,$A008
0000FC02                          JMP       $00028BF8
0000FC08                          CMP.B     #$02,$002B(A0)
0000FC0E                          BNE       $0000FC16
0000FC10                          MOVE.B    #$00,$002B(A0)
0000FC16                          JSR       $00028A3A
0000FC1C                          BCS       $0000FC54(pc)
0000FC20                          MOVE.W    #$0100,$0000(A1)
0000FC26                          MOVE.B    #$00,$0030(A1)
0000FC2C                          MOVE.B    #$01,$0022(A1)
0000FC32                          MOVE.B    #$00,$0006(A1)
0000FC38                          MOVE.W    $0008(A0),$0008(A1)
0000FC3E                          MOVE.W    $000C(A0),$000C(A1)
0000FC44                          MOVE.B    $002B(A0),D0
0000FC48                          EXT.W     D0
0000FC4A                          MOVE.B    +$00(D7.W),D0
0000FC4E                          EXT.W     D0
0000FC50                          (CCC     #$0,$000C(A1)
0000FC54                          ADDQ.B    #$1,$002B(A0)
0000FC58                          RTS
0000FC5A                          MOVE.B    (A0)+,D0
0000FC5C                          MOVE.L    D0,D0
0000FC5E                          SUBQ.W    #$4,$0008(A0)
0000FC62                          MOVE.W    $0008(A0),D0
0000FC66                          CMP.W     $002C(A0),D0
0000FC6A                          BHI       $0000FC72
0000FC6C                          MOVE.B    #$00,$0003(A0)
0000FC72                          RTS
0000FC74                          BTST      #$00,$A053
0000FC7A                          BEQ       $0000FCC0
0000FC7C                          MOVE.W    $A008,D0
0000FC80                          MOVE.W    $A00C,D1
0000FC84                          ADD.W     #$0020,D1
0000FC88                          MOVE.W    $0008(A0),D4
0000FC8C                          SUB.W     D4,D0
0000FC8E                          ADD.W     #$0018,D0
0000FC92                          CMP.W     #$0030,D0
0000FC96                          BHI       $0000FCC0
0000FC98                          MOVE.W    $000C(A0),D2
0000FC9C                          SUB.W     D2,D1
0000FC9E                          ADDQ.W    #$0,D1
0000FCA0                          CMP.W     #$0010,D1
0000FCA4                          BHI       $0000FCC0
0000FCA6                          MOVE.B    #$01,$A054
0000FCAC                          BCLR      #$00,$A053
0000FCB2                          BSET      #$01,$A053
0000FCB8                          MOVE.B    #$FF,$002A(A0)
0000FCBE                          RTS
0000FCC0                          MOVE.B    #$00,$002A(A0)
0000FCC6                          RTS
0000FCC8                          MOVE.B    #$06,$0002(A0)
0000FCCE                          RTS
0000FCD0                          JMP       $00028AF8
0000FCD6                          TST.B     $0006(A0)
0000FCDA                          BMI       $0000FF38(pc)
0000FCDE                          MOVEQ     #$00,D0
0000FCE0                          MOVE.B    $0002(A0),D0
0000FCE4                          ADD.W     +$00(D3.W),D0
0000FCE8                          JMP       +$00(D1.W)
0000FCEC                          OR.B      #$FE,$05BC(A0)
0000FCF2                          BCLR      D2,$0000FCF4(pc)
0000FCF6                          OR.B      #$01,D1
0000FCFA                          BTST      D0,D2
0000FCFC                          AND.B     #$FF,D2
0000FD00                          Invalid
0000FD02                          Invalid
0000FD04                          BTST      D0,D2
0000FD06                          BTST      D1,D2
0000FD08                          BTST      D1,D3
0000FD0A                          SUB.B     #$05,D5
0000FD0E                          BTST      D2,D5
0000FD10                          BTST      D2,D5
0000FD12                          BTST      D2,D5
0000FD14                          ADDQ.B    #$2,$0002(A0)
0000FD18                          MOVE.W    #$0001,$0044(A0)
0000FD1E                          MOVE.B    $FC0A,$004C(A0)
0000FD24                          MOVE.B    $FC26,$004D(A0)
0000FD2A                          MOVE.B    #$FF,$004F(A0)
0000FD30                          MOVEQ     #$00,D0
0000FD32                          MOVE.B    $FC09,D0
0000FD36                          MOVE.B    +$00(D6.L),$004F(A0)
0000FD3C                          BEQ       $000102A4(pc)
0000FD40                          MOVE.B    +$00(D1.W),$0021(A0)
0000FD46                          MOVEQ     #$00,D0
0000FD48                          MOVE.B    $0006(A0),D0
0000FD4C                          ADD.W     D0,D0
0000FD4E                          MOVEQ     #$00,D0
0000FD50                          MOVE.B    D0,D0
0000FD52                          ADD.W     +$00(D3.W),D0
0000FD56                          JMP       +$00(D1.W)
0000FD5A                          OR.B      #$14,(A6)
0000FD5E                          OR.B      #$10,(A2)
0000FD62                          OR.B      #$0C,(A0)
0000FD66                          OR.B      #$08,(A0)+
0000FD6A                          OR.B      #$2A,-(A0)
0000FD6E                          OR.B      #$75,$0C(A4,D4.L)
0000FD74                          OR.B      #$09,D1
0000FD78                          BCS       $000102A4(pc)
0000FD7C                          RTS
0000FD7E                          CMP.B     #$07,$FC09
0000FD84                          BCS       $000102A4(pc)
0000FD88                          RTS
0000FD8A                          CMP.B     #$03,$FC00
0000FD90                          BCS       $000102A4(pc)
0000FD94                          RTS
0000FD96                          CMP.B     #$02,$FC09
0000FD9C                          BCS       $000102A4(pc)
0000FDA0                          RTS
0000FDA2                          CMP.B     #$02,$FC09
0000FDA8                          BCS       $000102A4(pc)
0000FDAC                          MOVE.B    #$01,$004F(A0)
0000FDB2                          RTS
0000FDB4                          OR.B      #$00,D0
0000FDB8                          OR.B      #$02,D2
0000FDBC                          MOVEP.W   $0303(A0),D3
0000FDC0                          ADD.B     #$04,A0
0000FDC4                          SUB.B     #$04,D4
0000FDC8                          BTST      D2,D5
0000FDCA                          BTST      D2,D5
0000FDCC                          BTST      D2,D6
0000FDCE                          ADD.B     #$06,D6
0000FDD2                          BTST      D3,D7
0000FDD4                          BTST      D1,D7
0000FDD6                          MOVEP.W   $0000(A0),D3
0000FDDA                          AND.B     #$00,A0
0000FDDE                          OR.B      #$00,D3
0000FDE2                          OR.B      #$00,D0
0000FDE6                          OR.B      #$00,D0
0000FDEA                          OR.B      #$00,D0
0000FDEE                          MOVE.B    $004D(A0),D0
0000FDF2                          MOVE.W    D0,D1
0000FDF4                          ADD.W     D0,D0
0000FDF6                          ADD.W     D0,D0
0000FDF8                          ADD.W     D1,D0
0000FDFA                          ADD.B     $004C(A0),D0
0000FDFE                          MOVE.B    +$00(D2.L),D0
0000FE02                          CMP.B     $FC26,D0
0000FE06                          BEQ       $000102A4(pc)
0000FE0A                          MOVEQ     #$00,D0
0000FE0C                          MOVE.B    $0003(A0),D0
0000FE10                          ADD.W     +$00(D3.W),D0
0000FE14                          JMP       +$00(D1.W)
0000FE18                          OR.B      #$A2,A2
0000FE1C                          ADDQ.B    #$2,$0003(A0)
0000FE20                          RTS
0000FE22                          SUBQ.W    #$1,$0044(A0)
0000FE26                          BNE       $0000FE20
0000FE28                          MOVE.W    #$0001,$0044(A0)
0000FE2E                          MOVEQ     #$00,D0
0000FE30                          MOVE.B    $0006(A0),D0
0000FE34                          ASL.W     #3,D0
0000FE36                          LEA       +$00(D6.W),A1
0000FE3A                          MOVE.W    $A008,D0
0000FE3E                          SUB.W     $0008(A0),D0
0000FE42                          ADD.W     (A1)+,D0
0000FE44                          CMP.W     (A1)+,D0
0000FE46                          BCC       $0000FE20
0000FE48                          MOVE.W    $A00C,D0
0000FE4C                          SUB.W     $000C(A0),D0
0000FE50                          ADD.W     (A1)+,D0
0000FE52                          CMP.W     (A1)+,D0
0000FE54                          BCC       $0000FE20
0000FE56                          MOVE.B    #$01,$0044(A0)
0000FE5C                          MOVE.B    $0021(A0),$003F(A0)
0000FE62                          BRA       $0000FE1C
0000FE64                          Invalid
0000FE66                          BCLR      D0,D0
0000FE68                          BTST      D0,D0
0000FE6A                          AND.B     #$00,D0
0000FE6E                          Invalid
0000FE70                          BTST      D0,D0
0000FE72                          AND.B     #$C0,D0
0000FE76                          BCLR      D0,D0
0000FE78                          Invalid
0000FE7A                          BTST      D0,D0
0000FE7C                          Invalid
0000FE7E                          BCLR      D0,D0
0000FE80                          BTST      D0,-(A0)
0000FE82                          BTST      D0,D0
0000FE84                          OR.W      #$0120,D0
0000FE88                          BCHG      D0,D0
0000FE8A                          BTST      D0,D0
0000FE8C                          Invalid
0000FE8E                          BCLR      D0,D0
0000FE90                          Invalid
0000FE92                          BTST      D0,D0
0000FE94                          OR.W      #$0180,D0
0000FE98                          OR.W      #$0100,D0
0000FE9C                          Invalid
0000FE9E                          BCLR      D0,D0
0000FEA0                          BCHG      D0,D0
0000FEA2                          BTST      D0,D0
0000FEA4                          OR.W      #$0180,-(A0)
0000FEA8                          Invalid
0000FEAA                          BTST      D0,D0
0000FEAC                          Invalid
0000FEAE                          BCLR      D0,D0
0000FEB0                          Invalid
0000FEB2                          BTST      D0,D0
0000FEB4                          OR.L      #$01800160,D0
0000FEBA                          BTST      D0,D0
0000FEBC                          SUBQ.B    #$1,$0044(A0)
0000FEC0                          BNE       $0000FE20(pc)
0000FEC4                          ADDQ.B    #$1,$0044(A0)
0000FEC8                          MOVE.W    $0008(A0),D0
0000FECC                          SUB.W     $EA08,D0
0000FED0                          CMP.W     #$0200,D0
0000FED4                          BCC       $0000FEEC(pc)
0000FED8                          MOVE.W    $000C(A0),D0
0000FEDC                          SUB.W     $EA0C,D0
0000FEE0                          ADD.W     #$0020,D0
0000FEE4                          CMP.W     #$0100,D0
0000FEE8                          BCS       $0000FE20(pc)
0000FEEC                          JSR       $000289DA
0000FEF2                          BCS       $0000FE20(pc)
0000FEF6                          MOVE.W    #$0100,(A1)
0000FEFA                          MOVE.B    #$22,$0017(A1)
0000FF00                          MOVE.B    #$FF,$0006(A1)
0000FF06                          MOVE.W    $0008(A0),$0008(A1)
0000FF0C                          MOVE.W    $000C(A0),$000C(A1)
0000FF12                          MOVE.B    #$2D,$0044(A0)
0000FF18                          SUBQ.B    #$1,$003F(A0)
0000FF1C                          BNE       $0000FE20(pc)
0000FF20                          MOVE.B    #$00,$0003(A0)
0000FF26                          MOVE.W    #$0168,$0044(A0)
0000FF2C                          SUBQ.B    #$1,$004F(A0)
0000FF30                          BEQ       $000102A4(pc)
0000FF34                          BRA       $0000FE20(pc)
0000FF38                          MOVEQ     #$00,D0
0000FF3A                          MOVE.B    $0002(A0),D0
0000FF3E                          ADD.W     +$00(D3.W),D0
0000FF42                          JMP       +$00(D1.W)
0000FF46                          OR.B      #$5A,A0
0000FF4A                          BTST      D1,(A2)+
0000FF4C                          BCHG      D1,-(A0)
0000FF4E                          ADDQ.B    #$2,$0002(A0)
0000FF52                          LEA       $000CD178,A6
0000FF58                          MOVE.L    A6,$001A(A0)
0000FF5C                          MOVE.W    $000CD21A,$0018(A0)
0000FF64                          MOVE.W    $000CD21C,$0028(A0)
0000FF6C                          MOVE.B    #$00,$001E(A0)
0000FF72                          MOVEQ     #$00,D0
0000FF74                          MOVEQ     #$02,D1
0000FF76                          MOVE.W    $A00C,D2
0000FF7A                          CMP.W     $000C(A0),D2
0000FF7E                          BCS       $0000FF84
0000FF80                          MOVEQ     #$10,D0
0000FF82                          MOVEQ     #$FE,D1
0000FF84                          MOVE.B    D0,$0016(A0)
0000FF88                          MOVE.W    D1,$004A(A0)
0000FF8C                          MOVEQ     #$00,D0
0000FF8E                          LEA       $000CD1A0,A4
0000FF94                          JSR       $000287D6
0000FF9A                          JMP       $00028B8A
0000FFA0                          RTS
0000FFA2                          CMP.B     #$02,$001E(A0)
0000FFA8                          BCC       $00010260(pc)
0000FFAC                          MOVEQ     #$00,D0
0000FFAE                          MOVE.B    $0003(A0),D0
0000FFB2                          ADD.W     +$00(D3.W),D0
0000FFB6                          JMP       +$00(D1.W)
0000FFBA                          OR.B      #$8E,(A4)
0000FFBE                          AND.B     #$28,-(A0)
0000FFC2                          OR.B      #$28,D3
0000FFC6                          OR.B      #$F9,D4
0000FFCA                          OR.B      #$8A,D2
0000FFCE                          TST.B     $0004(A0)
0000FFD2                          BNE       $0000FFE6(pc)
0000FFD6                          ADDQ.B    #$2,$0004(A0)
0000FFDA                          MOVE.B    #$01,$0044(A0)
0000FFE0                          MOVE.B    #$01,$0045(A0)
0000FFE6                          SUBQ.B    #$1,$0044(A0)
0000FFEA                          BNE       $00010024(pc)
0000FFEE                          MOVE.B    #$0A,$0044(A0)
0000FFF4                          MOVE.B    $0046(A0),D1
0000FFF8                          SUBQ.B    #$1,D1
0000FFFA                          MOVE.B    $0047(A0),D0
0000FFFE                          SUB.B     D1,D0
