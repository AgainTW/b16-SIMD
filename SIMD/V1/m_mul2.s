start:
        j        main              # start from main
m_mul_2:
        addi    sp,sp,-64
        sw      s0,60(sp)
        addi    s0,sp,64
        sw      a0,-52(s0)
        sw      a1,-56(s0)
        sw      zero,-20(s0)
        lw      a5,-52(s0)
        slli    a4,a5,1
        li      a5,-131072
        and     a5,a4,a5
        sw      a5,-32(s0)
        lw      a5,-52(s0)
        slli    a4,a5,1
        li      a5,131072
        addi    a5,a5,-2
        and     a5,a4,a5
        sw      a5,-36(s0)
        lw      a5,-56(s0)
        srli    a5,a5,16
        sw      a5,-24(s0)
        lw      a4,-56(s0)
        li      a5,65536
        addi    a5,a5,-1
        and     a5,a4,a5
        sw      a5,-28(s0)
        j       .L2
.L5:
        lw      a5,-24(s0)
        andi    a5,a5,1
        beq     a5,zero,.L3
        lw      a4,-20(s0)
        lw      a5,-32(s0)
        add     a5,a4,a5
        sw      a5,-20(s0)
.L3:
        lw      a5,-28(s0)
        andi    a5,a5,1
        beq     a5,zero,.L4
        lw      a4,-20(s0)
        lw      a5,-36(s0)
        add     a5,a4,a5
        sw      a5,-20(s0)
.L4:
        lw      a5,-24(s0)
        srli    a5,a5,1
        sw      a5,-24(s0)
        lw      a5,-28(s0)
        srli    a5,a5,1
        sw      a5,-28(s0)
        lw      a5,-20(s0)
        srai    a4,a5,1
        li      a5,-32768
        addi    a5,a5,-1
        and     a5,a4,a5
        sw      a5,-20(s0)
.L2:
        lw      a5,-24(s0)
        bne     a5,zero,.L5
        lw      a5,-28(s0)
        bne     a5,zero,.L5
        lw      a5,-20(s0)
        mv      a0,a5
        lw      s0,60(sp)
        addi    sp,sp,64
        jr      ra
main:
        addi    sp,sp,-32
        sw      ra,28(sp)
        sw      s0,24(sp)
        addi    s0,sp,32
        li      a5,8323072
        addi    a5,a5,127
        sw      a5,-20(s0)
        li      a5,8323072
        addi    a5,a5,127
        sw      a5,-24(s0)
        lw      a5,-20(s0)
        lw      a4,-24(s0)
        mv      a1,a4
        mv      a0,a5
        call    m_mul_2
        mv      a5,a0
        sw      a5,-28(s0)
        nop
        lw      ra,28(sp)
        lw      s0,24(sp)
        addi    sp,sp,32
        li      a7,  10           # return 0
        ecall