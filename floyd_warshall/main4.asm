.data
n  qword ? ; <- graph size |V| loaded from RDX
n2 qword ? ; <- |V|^2

.code

; The Floyd Warshall algorithm runner. 
; Does initialization, executes the core inside the O(|V|^3) triple loop and cleans up.
floyd_warshall macro
    push r13
    push r12
    push r10
    push rdi

    xor  r8,  r8
    xor  r9,  r9
    xor  r10, r10
    
    mov  n,   rdx
    imul rdx, rdx
    mov  n2,  rdx

k_loop:
    cmp r8, n ; k == n is an exit condition
    je done

j_loop:
    cmp r9, n
    je k_loop_start
         
i_loop:
    cmp r10, n
    je j_loop_start

    mov  RDI, R9
    imul RDI, n
    add  RDI, R10
    shl RDI, 2
    vmovdqu ymm10, ymmword ptr [rcx + rdi]

    mov  RAX, R8
    imul RAX, n
    add  RAX, R10
    shl RAX, 2    
    vmovdqu ymm11, ymmword ptr [rcx + rax]

    mov  RAX, R8 
    imul RAX, n
    add  RAX, R9
    shl RAX, 2
    mov  r13,  [rcx + rax]
    movq xmm12, r13

    vpbroadcastd ymm12, xmm12
    vpaddd ymm13, ymm12, ymm11
    vpminud ymm14, ymm13, ymm10
    lea r12, [rcx + rdi]
    vmovdqu ymmword ptr [r12], ymm14

noop_iteration:
    add r10d, 8 ; add 8, 16 or 32 depending on weight type
    jmp i_loop
       
j_loop_start:
    xor r10d, r10d

    inc r9d
    jmp j_loop

k_loop_start:
    xor r9d, r9d

    inc r8d
    jmp k_loop
   
done:
    pop  rdi
    pop  r10
    pop  r12
    pop  r13
    ret
endm


end