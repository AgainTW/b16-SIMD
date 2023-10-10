start:
        j        main              # start from main
m_mul_1:
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
main:
        addi    sp,sp,-64
        sw      ra,60(sp)
        sw      s0,56(sp)
        addi    s0,sp,64
        li      a5,8323072
        addi    a5,a5,127
        sw      a5,-20(s0)
        li      a5,8323072
        addi    a5,a5,127
        sw      a5,-24(s0)
        lw      a5,-20(s0)
        srli    a5,a5,16
        sw      a5,-28(s0)
        lw      a5,-24(s0)
        srli    a5,a5,16
        sw      a5,-32(s0)
        lw      a4,-20(s0)
        li      a5,65536
        addi    a5,a5,-1
        and     a5,a4,a5
        sw      a5,-36(s0)
        lw      a4,-24(s0)
        li      a5,65536
        addi    a5,a5,-1
        and     a5,a4,a5
        sw      a5,-40(s0)
        lw      a5,-28(s0)
        lw      a4,-32(s0)
        mv      a1,a4
        mv      a0,a5
        call    m_mul_1
        mv      a5,a0
        sw      a5,-44(s0)
        lw      a5,-36(s0)
        lw      a4,-40(s0)
        mv      a1,a4
        mv      a0,a5
        call    m_mul_1
        mv      a5,a0
        sw      a5,-48(s0)
        lw      a5,-44(s0)
        slli    a5,a5,16
        lw      a4,-48(s0)
        add     a5,a4,a5
        sw      a5,-52(s0)
        nop
        lw      ra,60(sp)
        lw      s0,56(sp)
        addi    sp,sp,64
        li      a7,  10           # return 0
        ecall