**************************************************************
*                          FUNCTION                          *
**************************************************************
undefined __do_global_dtors_aux()
undefined         AL:1           <RETURN>
__do_global_dtors_aux                           XREF[3]:     Entry Point(*),
                                                             __libc_csu_init:00100719(c),
                                                             00300dc0(*)
00100600 80 3d 09        CMP        byte ptr [completed.7697],0x0                    = ??
0a 20 00 00
00100607 75 2f           JNZ        LAB_00100638
00100609 48 83 3d        CMP        qword ptr [->__cxa_finalize],0x0                 = 00302028
e7 09 20
00 00
00100611 55              PUSH       RBP
00100612 48 89 e5        MOV        RBP,RSP
00100615 74 0c           JZ         LAB_00100623
00100617 48 8b 3d        MOV        RDI,qword ptr [->__dso_handle]                   = 00301008
ea 09 20 00
0010061e e8 0d ff        CALL       __cxa_finalize                                   undefined __cxa_finalize()
ff ff
LAB_00100623                                    XREF[1]:     00100615(j)
00100623 e8 48 ff        CALL       deregister_tm_clones                             undefined deregister_tm_clones()
ff ff
00100628 c6 05 e1        MOV        byte ptr [completed.7697],0x1                    = ??
09 20 00 01
0010062f 5d              POP        RBP
00100630 c3              RET
00100631 0f              ??         0Fh
00100632 1f              ??         1Fh
00100633 80              ??         80h
00100634 00              ??         00h
00100635 00              ??         00h
00100636 00              ??         00h
00100637 00              ??         00h
LAB_00100638                                    XREF[1]:     00100607(j)
00100638 f3 c3           RET
0010063a 66              ??         66h    f
0010063b 0f              ??         0Fh
0010063c 1f              ??         1Fh
0010063d 44              ??         44h    D
0010063e 00              ??         00h
0010063f 00              ??         00h
**************************************************************
*                          FUNCTION                          *
**************************************************************
undefined frame_dummy()
undefined         AL:1           <RETURN>
frame_dummy                                     XREF[3]:     Entry Point(*),
                                                             __libc_csu_init:00100719(c),
                                                             00300db8(*)
00100640 55              PUSH       RBP
00100641 48 89 e5        MOV        RBP,RSP
00100644 5d              POP        RBP
00100645 e9 66 ff        JMP        register_tm_clones                               undefined register_tm_clones()
ff ff
-- Flow Override: CALL_RETURN (CALL_TERMINATOR)
**************************************************************
*                          FUNCTION                          *
**************************************************************
undefined main()
undefined         AL:1           <RETURN>
undefined4        Stack[-0xc]:4  local_c                                 XREF[2]:     00100684(W),
                                                                      001006ba(RW)
undefined4        Stack[-0x10]:4 local_10                                XREF[5]:     00100681(W),
                                                                      0010068d(R),
                                                                      001006ab(W),
                                                                      001006b4(R),
                                                                      001006be(R)
undefined4        Stack[-0x14]:4 local_14                                XREF[6]:     00100659(W),
                                                                      00100660(R),
                                                                      0010067c(R),
                                                                      001006a6(R),
                                                                      001006ae(R),
                                                                      001006b7(W)
undefined4        Stack[-0x18]:4 local_18                                XREF[5]:     00100652(W),
                                                                      00100663(R),
                                                                      00100679(R),
                                                                      001006a3(R),
                                                                      001006b1(W)
main                                            XREF[4]:     Entry Point(*),
                                                             _start:0010055d(*), 00100784,
                                                             00100830(*)
0010064a 55              PUSH       RBP
0010064b 48 89 e5        MOV        RBP,RSP
0010064e 48 83 ec 10     SUB        RSP,0x10
00100652 c7 45 f0        MOV        dword ptr [RBP + local_18],0x0
00 00 00 00
00100659 c7 45 f4        MOV        dword ptr [RBP + local_14],0x1
01 00 00 00
00100660 8b 55 f4        MOV        EDX,dword ptr [RBP + local_14]
00100663 8b 45 f0        MOV        EAX,dword ptr [RBP + local_18]
00100666 89 c6           MOV        ESI,EAX
00100668 48 8d 3d        LEA        RDI,[s__%d_%d_00100754]                          = "\n%d %d"
e5 00 00 00
0010066f b8 00 00        MOV        EAX,0x0
00 00
00100674 e8 a7 fe        CALL       printf                                           int printf(char * __format, ...)
ff ff
00100679 8b 55 f0        MOV        EDX,dword ptr [RBP + local_18]
0010067c 8b 45 f4        MOV        EAX,dword ptr [RBP + local_14]
0010067f 01 d0           ADD        EAX,EDX
00100681 89 45 f8        MOV        dword ptr [RBP + local_10],EAX
00100684 c7 45 fc        MOV        dword ptr [RBP + local_c],0x2
02 00 00 00
0010068b eb 31           JMP        LAB_001006be
LAB_0010068d                                    XREF[1]:     001006c5(j)
0010068d 8b 45 f8        MOV        EAX,dword ptr [RBP + local_10]
00100690 89 c6           MOV        ESI,EAX
00100692 48 8d 3d        LEA        RDI,[DAT_0010075b]                               = 20h
c2 00 00 00
00100699 b8 00 00        MOV        EAX,0x0
00 00
0010069e e8 7d fe        CALL       printf                                           int printf(char * __format, ...)
ff ff
001006a3 8b 55 f0        MOV        EDX,dword ptr [RBP + local_18]
001006a6 8b 45 f4        MOV        EAX,dword ptr [RBP + local_14]
001006a9 01 d0           ADD        EAX,EDX
001006ab 89 45 f8        MOV        dword ptr [RBP + local_10],EAX
001006ae 8b 45 f4        MOV        EAX,dword ptr [RBP + local_14]
001006b1 89 45 f0        MOV        dword ptr [RBP + local_18],EAX
001006b4 8b 45 f8        MOV        EAX,dword ptr [RBP + local_10]
001006b7 89 45 f4        MOV        dword ptr [RBP + local_14],EAX
001006ba 83 45 fc 01     ADD        dword ptr [RBP + local_c],0x1
LAB_001006be                                    XREF[1]:     0010068b(j)
001006be 81 7d f8        CMP        dword ptr [RBP + local_10],0x3e7
e7 03 00 00
001006c5 7e c6           JLE        LAB_0010068d
001006c7 b8 00 00        MOV        EAX,0x0
00 00
001006cc c9              LEAVE
001006cd c3              RET
001006ce 66              ??         66h    f
001006cf 90              ??         90h
