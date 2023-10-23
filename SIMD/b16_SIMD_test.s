start:
        j        main              # start from main
m_mul:
        addi    sp,sp,-48
        sw      s0,44(sp)
        addi    s0,sp,48
        sw      a0,-36(s0)
        sw      a1,-40(s0)
        sw      zero,-20(s0)
        lw      a5,-36(s0)
        slli    a5,a5,1
        sw      a5,-36(s0)
        j       L2
L4:
        lw      a5,-40(s0)
        andi    a5,a5,1
        beq     a5,zero,L3
        lw      a4,-20(s0)
        lw      a5,-36(s0)
        add     a5,a4,a5
        sw      a5,-20(s0)
L3:
        lw      a5,-40(s0)
        srai    a5,a5,1
        sw      a5,-40(s0)
        lw      a5,-20(s0)
        srai    a5,a5,1
        sw      a5,-20(s0)
L2:
        lw      a5,-40(s0)
        bne     a5,zero,L4
        lw      a5,-20(s0)
        mv      a0,a5
        lw      s0,44(sp)
        addi    sp,sp,48
        jr      ra
muti_data_mul:
        addi    sp,sp,-160
        sw      ra,156(sp)
        sw      s0,152(sp)
        addi    s0,sp,160
        sw      a0,-148(s0)
        sw      a1,-152(s0)
        lw      a5,-148(s0)
        sw      a5,-60(s0)
        lw      a5,-152(s0)
        sw      a5,-64(s0)
        sw      zero,-20(s0)
        sw      zero,-24(s0)
        lw      a5,-60(s0)
        srli    a5,a5,31
        sw      a5,-68(s0)
        lw      a5,-60(s0)
        srli    a5,a5,15
        andi    a5,a5,1
        sw      a5,-72(s0)
        lw      a5,-64(s0)
        srli    a5,a5,31
        sw      a5,-76(s0)
        lw      a5,-60(s0)
        srli    a5,a5,15
        andi    a5,a5,1
        sw      a5,-80(s0)
        lw      a5,-60(s0)
        srli    a5,a5,16
        andi    a5,a5,255
        ori     a5,a5,128
        sw      a5,-84(s0)
        lw      a5,-60(s0)
        andi    a5,a5,255
        ori     a5,a5,128
        sw      a5,-88(s0)
        lw      a5,-64(s0)
        srli    a5,a5,16
        andi    a5,a5,255
        ori     a5,a5,128
        sw      a5,-92(s0)
        lw      a5,-64(s0)
        andi    a5,a5,255
        ori     a5,a5,128
        sw      a5,-96(s0)
        lw      a5,-60(s0)
        srli    a5,a5,23
        andi    a5,a5,255
        sw      a5,-100(s0)
        lw      a5,-60(s0)
        srli    a5,a5,7
        andi    a5,a5,255
        sw      a5,-104(s0)
        lw      a5,-64(s0)
        srli    a5,a5,23
        andi    a5,a5,255
        sw      a5,-108(s0)
        lw      a5,-64(s0)
        srli    a5,a5,7
        andi    a5,a5,255
        sw      a5,-112(s0)
        lw      a4,-100(s0)
        li      a5,255
        bne     a4,a5,L7
        lw      a4,-84(s0)
        li      a5,128
        beq     a4,a5,L7
        lw      a5,-20(s0)
        bne     a5,zero,L7
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-52(s0)
        li      a5,1
        sw      a5,-20(s0)
L7:
        lw      a4,-108(s0)
        li      a5,255
        bne     a4,a5,L8
        lw      a4,-92(s0)
        li      a5,128
        beq     a4,a5,L8
        lw      a5,-20(s0)
        bne     a5,zero,L8
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-52(s0)
        li      a5,1
        sw      a5,-20(s0)
L8:
        lw      a4,-100(s0)
        li      a5,255
        bne     a4,a5,L9
        lw      a4,-84(s0)
        li      a5,128
        bne     a4,a5,L9
        lw      a5,-20(s0)
        bne     a5,zero,L9
        lw      a5,-108(s0)
        bne     a5,zero,L10
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-52(s0)
        li      a5,1
        sw      a5,-20(s0)
        j       L9
L10:
        lw      a4,-68(s0)
        lw      a5,-76(s0)
        xor     a5,a4,a5
        slli    a4,a5,15
        li      a5,32768
        addi    a5,a5,-128
        or      a5,a4,a5
        sw      a5,-52(s0)
        li      a5,1
        sw      a5,-20(s0)
L9:
        lw      a4,-108(s0)
        li      a5,255
        bne     a4,a5,L11
        lw      a4,-92(s0)
        li      a5,128
        bne     a4,a5,L11
        lw      a5,-20(s0)
        bne     a5,zero,L11
        lw      a5,-100(s0)
        bne     a5,zero,L12
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-52(s0)
        li      a5,1
        sw      a5,-20(s0)
        j       L11
L12:
        lw      a4,-68(s0)
        lw      a5,-76(s0)
        xor     a5,a4,a5
        slli    a4,a5,15
        li      a5,32768
        addi    a5,a5,-128
        or      a5,a4,a5
        sw      a5,-52(s0)
        li      a5,1
        sw      a5,-20(s0)
L11:
        lw      a5,-100(s0)
        beq     a5,zero,L13
        lw      a5,-108(s0)
        bne     a5,zero,L14
L13:
        lw      a5,-20(s0)
        bne     a5,zero,L14
        lw      a4,-68(s0)
        lw      a5,-76(s0)
        xor     a5,a4,a5
        slli    a5,a5,15
        sw      a5,-52(s0)
        li      a5,1
        sw      a5,-20(s0)
L14:
        lw      a4,-104(s0)
        li      a5,255
        bne     a4,a5,L15
        lw      a4,-88(s0)
        li      a5,128
        beq     a4,a5,L15
        lw      a5,-24(s0)
        bne     a5,zero,L15
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-56(s0)
        li      a5,1
        sw      a5,-24(s0)
L15:
        lw      a4,-112(s0)
        li      a5,255
        bne     a4,a5,L16
        lw      a4,-96(s0)
        li      a5,128
        beq     a4,a5,L16
        lw      a5,-24(s0)
        bne     a5,zero,L16
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-56(s0)
        li      a5,1
        sw      a5,-24(s0)
L16:
        lw      a4,-104(s0)
        li      a5,255
        bne     a4,a5,L17
        lw      a4,-88(s0)
        li      a5,128
        bne     a4,a5,L17
        lw      a5,-24(s0)
        bne     a5,zero,L17
        lw      a5,-112(s0)
        bne     a5,zero,L18
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-56(s0)
        li      a5,1
        sw      a5,-24(s0)
        j       L17
L18:
        lw      a4,-72(s0)
        lw      a5,-80(s0)
        xor     a5,a4,a5
        slli    a4,a5,15
        li      a5,32768
        addi    a5,a5,-128
        or      a5,a4,a5
        sw      a5,-56(s0)
        li      a5,1
        sw      a5,-24(s0)
L17:
        lw      a4,-112(s0)
        li      a5,255
        bne     a4,a5,L19
        lw      a4,-96(s0)
        li      a5,128
        bne     a4,a5,L19
        lw      a5,-24(s0)
        bne     a5,zero,L19
        lw      a5,-104(s0)
        bne     a5,zero,L20
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-56(s0)
        li      a5,1
        sw      a5,-24(s0)
        j       L19
L20:
        lw      a4,-68(s0)
        lw      a5,-76(s0)
        xor     a5,a4,a5
        slli    a4,a5,15
        li      a5,32768
        addi    a5,a5,-128
        or      a5,a4,a5
        sw      a5,-56(s0)
        li      a5,1
        sw      a5,-24(s0)
L19:
        lw      a5,-104(s0)
        beq     a5,zero,L21
        lw      a5,-112(s0)
        bne     a5,zero,L22
L21:
        lw      a5,-24(s0)
        bne     a5,zero,L22
        lw      a4,-72(s0)
        lw      a5,-80(s0)
        xor     a5,a4,a5
        slli    a5,a5,15
        sw      a5,-56(s0)
        li      a5,1
        sw      a5,-24(s0)
L22:
        lw      a5,-20(s0)
        bne     a5,zero,L23
        lw      a5,-24(s0)
        bne     a5,zero,L23
        lw      a4,-68(s0)
        lw      a5,-76(s0)
        xor     a5,a4,a5
        sw      a5,-28(s0)
        lw      a1,-92(s0)
        lw      a0,-84(s0)
        call    m_mul
        sw      a0,-116(s0)
        lw      a4,-100(s0)
        lw      a5,-108(s0)
        add     a5,a4,a5
        addi    a5,a5,-127
        sw      a5,-120(s0)
        lw      a4,-72(s0)
        lw      a5,-80(s0)
        xor     a5,a4,a5
        sw      a5,-32(s0)
        lw      a1,-96(s0)
        lw      a0,-88(s0)
        call    m_mul
        sw      a0,-124(s0)
        lw      a4,-104(s0)
        lw      a5,-112(s0)
        add     a5,a4,a5
        addi    a5,a5,-127
        sw      a5,-128(s0)
        lw      a5,-116(s0)
        srai    a5,a5,8
        andi    a5,a5,1
        sw      a5,-132(s0)
        lw      a5,-132(s0)
        lw      a4,-116(s0)
        sra     a5,a4,a5
        sw      a5,-36(s0)
        lw      a4,-120(s0)
        lw      a5,-132(s0)
        add     a5,a4,a5
        sw      a5,-44(s0)
        lw      a5,-124(s0)
        srai    a5,a5,8
        andi    a5,a5,1
        sw      a5,-136(s0)
        lw      a5,-136(s0)
        lw      a4,-124(s0)
        sra     a5,a4,a5
        sw      a5,-40(s0)
        lw      a4,-128(s0)
        lw      a5,-136(s0)
        add     a5,a4,a5
        sw      a5,-48(s0)
        lw      a5,-44(s0)
        bgt     a5,zero,L24
        lw      a5,-20(s0)
        bne     a5,zero,L24
        lw      a4,-68(s0)
        lw      a5,-76(s0)
        xor     a5,a4,a5
        slli    a5,a5,15
        sw      a5,-52(s0)
        li      a5,1
        sw      a5,-20(s0)
L24:
        lw      a4,-44(s0)
        li      a5,254
        ble     a4,a5,L25
        lw      a5,-20(s0)
        bne     a5,zero,L25
        lw      a4,-68(s0)
        lw      a5,-76(s0)
        xor     a5,a4,a5
        slli    a4,a5,15
        li      a5,32768
        addi    a5,a5,-128
        or      a5,a4,a5
        sw      a5,-52(s0)
        li      a5,1
        sw      a5,-20(s0)
L25:
        lw      a5,-48(s0)
        bgt     a5,zero,L26
        lw      a5,-24(s0)
        bne     a5,zero,L26
        lw      a4,-72(s0)
        lw      a5,-80(s0)
        xor     a5,a4,a5
        slli    a5,a5,15
        sw      a5,-56(s0)
        li      a5,1
        sw      a5,-24(s0)
L26:
        lw      a4,-48(s0)
        li      a5,254
        ble     a4,a5,L23
        lw      a5,-24(s0)
        bne     a5,zero,L23
        lw      a4,-72(s0)
        lw      a5,-80(s0)
        xor     a5,a4,a5
        slli    a4,a5,15
        li      a5,32768
        addi    a5,a5,-128
        or      a5,a4,a5
        sw      a5,-56(s0)
        li      a5,1
        sw      a5,-24(s0)
L23:
        lw      a5,-20(s0)
        bne     a5,zero,L27
        lw      a5,-28(s0)
        slli    a4,a5,15
        lw      a5,-44(s0)
        slli    a3,a5,7
        li      a5,32768
        addi    a5,a5,-128
        and     a5,a3,a5
        or      a4,a4,a5
        lw      a5,-36(s0)
        andi    a5,a5,127
        or      a5,a4,a5
        sw      a5,-52(s0)
L27:
        lw      a5,-24(s0)
        bne     a5,zero,L28
        lw      a5,-32(s0)
        slli    a4,a5,15
        lw      a5,-48(s0)
        slli    a3,a5,7
        li      a5,32768
        addi    a5,a5,-128
        and     a5,a3,a5
        or      a4,a4,a5
        lw      a5,-40(s0)
        andi    a5,a5,127
        or      a5,a4,a5
        sw      a5,-56(s0)
L28:
        lw      a5,-52(s0)
        slli    a5,a5,16
        lw      a4,-56(s0)
        add     a5,a4,a5
        sw      a5,-140(s0)
        lw      a5,-140(s0)
        mv      a0,a5
        lw      ra,156(sp)
        lw      s0,152(sp)
        addi    sp,sp,160
        jr      ra
main:
        addi    sp,sp,-32
        sw      ra,28(sp)
        sw      s0,24(sp)
        addi    s0,sp,32
        li      a5,1069563904
        addi    a5,a5,29
        sw      a5,-20(s0)
        li      a5,1113735168
        addi    a5,a5,712
        sw      a5,-24(s0)
        sw      zero,-28(s0)
        lw      a1,-24(s0)
        lw      a0,-20(s0)
        call    muti_data_mul
        sw      a0,-28(s0)
        nop
        lw      ra,28(sp)
        lw      s0,24(sp)
        addi    sp,sp,32
        li      a7,  10           # return 0
        ecall