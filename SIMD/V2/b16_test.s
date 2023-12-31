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
data_mul:
        addi    sp,sp,-96
        sw      ra,92(sp)
        sw      s0,88(sp)
        addi    s0,sp,96
        sw      a0,-84(s0)
        sw      a1,-88(s0)
        lw      a5,-84(s0)
        sw      a5,-40(s0)
        lw      a5,-88(s0)
        sw      a5,-44(s0)
        sw      zero,-20(s0)
        lw      a5,-40(s0)
        srli    a5,a5,31
        sw      a5,-48(s0)
        lw      a5,-44(s0)
        srli    a5,a5,31
        sw      a5,-52(s0)
        lw      a5,-40(s0)
        srli    a5,a5,16
        andi    a5,a5,255
        ori     a5,a5,128
        sw      a5,-56(s0)
        lw      a5,-44(s0)
        srli    a5,a5,16
        andi    a5,a5,255
        ori     a5,a5,128
        sw      a5,-60(s0)
        lw      a5,-40(s0)
        srli    a5,a5,23
        andi    a5,a5,255
        sw      a5,-64(s0)
        lw      a5,-44(s0)
        srli    a5,a5,23
        andi    a5,a5,255
        sw      a5,-68(s0)
        lw      a4,-64(s0)
        li      a5,255
        bne     a4,a5,L7
        lw      a4,-56(s0)
        li      a5,128
        beq     a4,a5,L7
        lw      a5,-20(s0)
        bne     a5,zero,L7
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-36(s0)
        li      a5,1
        sw      a5,-20(s0)
L7:
        lw      a4,-68(s0)
        li      a5,255
        bne     a4,a5,L8
        lw      a4,-60(s0)
        li      a5,128
        beq     a4,a5,L8
        lw      a5,-20(s0)
        bne     a5,zero,L8
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-36(s0)
        li      a5,1
        sw      a5,-20(s0)
L8:
        lw      a4,-64(s0)
        li      a5,255
        bne     a4,a5,L9
        lw      a4,-56(s0)
        li      a5,128
        bne     a4,a5,L9
        lw      a5,-20(s0)
        bne     a5,zero,L9
        lw      a5,-68(s0)
        bne     a5,zero,L10
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-36(s0)
        li      a5,1
        sw      a5,-20(s0)
        j       L9
L10:
        lw      a4,-48(s0)
        lw      a5,-52(s0)
        xor     a5,a4,a5
        slli    a4,a5,15
        li      a5,32768
        addi    a5,a5,-128
        or      a5,a4,a5
        sw      a5,-36(s0)
        li      a5,1
        sw      a5,-20(s0)
L9:
        lw      a4,-68(s0)
        li      a5,255
        bne     a4,a5,L11
        lw      a4,-60(s0)
        li      a5,128
        bne     a4,a5,L11
        lw      a5,-20(s0)
        bne     a5,zero,L11
        lw      a5,-64(s0)
        bne     a5,zero,L12
        li      a5,32768
        addi    a5,a5,-127
        sw      a5,-36(s0)
        li      a5,1
        sw      a5,-20(s0)
        j       L11
L12:
        lw      a4,-48(s0)
        lw      a5,-52(s0)
        xor     a5,a4,a5
        slli    a4,a5,15
        li      a5,32768
        addi    a5,a5,-128
        or      a5,a4,a5
        sw      a5,-36(s0)
        li      a5,1
        sw      a5,-20(s0)
L11:
        lw      a5,-64(s0)
        beq     a5,zero,L13
        lw      a5,-68(s0)
        bne     a5,zero,L14
L13:
        lw      a5,-20(s0)
        bne     a5,zero,L14
        lw      a4,-48(s0)
        lw      a5,-52(s0)
        xor     a5,a4,a5
        slli    a5,a5,15
        sw      a5,-36(s0)
        li      a5,1
        sw      a5,-20(s0)
L14:
        lw      a5,-20(s0)
        bne     a5,zero,L15
        lw      a4,-48(s0)
        lw      a5,-52(s0)
        xor     a5,a4,a5
        sw      a5,-24(s0)
        lw      a1,-60(s0)
        lw      a0,-56(s0)
        call    m_mul
        sw      a0,-72(s0)
        lw      a4,-64(s0)
        lw      a5,-68(s0)
        add     a5,a4,a5
        addi    a5,a5,-127
        sw      a5,-76(s0)
        lw      a5,-72(s0)
        srai    a5,a5,8
        andi    a5,a5,1
        sw      a5,-80(s0)
        lw      a5,-80(s0)
        lw      a4,-72(s0)
        sra     a5,a4,a5
        sw      a5,-28(s0)
        lw      a4,-76(s0)
        lw      a5,-80(s0)
        add     a5,a4,a5
        sw      a5,-32(s0)
        lw      a5,-32(s0)
        bgt     a5,zero,L16
        lw      a5,-20(s0)
        bne     a5,zero,L16
        lw      a4,-48(s0)
        lw      a5,-52(s0)
        xor     a5,a4,a5
        slli    a5,a5,15
        sw      a5,-36(s0)
        li      a5,1
        sw      a5,-20(s0)
L16:
        lw      a4,-32(s0)
        li      a5,254
        ble     a4,a5,L15
        lw      a5,-20(s0)
        bne     a5,zero,L15
        lw      a4,-48(s0)
        lw      a5,-52(s0)
        xor     a5,a4,a5
        slli    a4,a5,15
        li      a5,32768
        addi    a5,a5,-128
        or      a5,a4,a5
        sw      a5,-36(s0)
        li      a5,1
        sw      a5,-20(s0)
L15:
        lw      a5,-20(s0)
        bne     a5,zero,L17
        lw      a5,-24(s0)
        slli    a4,a5,15
        lw      a5,-32(s0)
        slli    a3,a5,7
        li      a5,32768
        addi    a5,a5,-128
        and     a5,a3,a5
        or      a4,a4,a5
        lw      a5,-28(s0)
        andi    a5,a5,127
        or      a5,a4,a5
        sw      a5,-36(s0)
L17:
        lw      a5,-36(s0)
        slli    a5,a5,16
        sw      a5,-36(s0)
        lw      a5,-36(s0)
        mv      a0,a5
        lw      ra,92(sp)
        lw      s0,88(sp)
        addi    sp,sp,96
        jr      ra
main:
        addi    sp,sp,-48
        sw      ra,44(sp)
        sw      s0,40(sp)
        addi    s0,sp,48
        li      a5,1069547520
        sw      a5,-20(s0)
        li      a5,1113718784
        sw      a5,-24(s0)
        li      a5,1075642368
        sw      a5,-28(s0)
        li      a5,1120403456
        sw      a5,-32(s0)
        sw      zero,-36(s0)
        sw      zero,-40(s0)
        lw      a1,-24(s0)
        lw      a0,-20(s0)
        call    data_mul
        sw      a0,-36(s0)
        lw      a1,-32(s0)
        lw      a0,-28(s0)
        call    data_mul
        sw      a0,-40(s0)
        nop
        lw      ra,44(sp)
        lw      s0,40(sp)
        addi    sp,sp,48
        li      a7,  10           # return 0
        ecall