.data
print_fmt: .string "%ld \n"
ia: .quad 0

.text

.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $0, %rsp
  movq $16, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
 movq $1, %rax
 pushq %rax
 movq $4, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
 movq $8, %rax
 pushq %rax
 movq $2, %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  popq %rax
  movq %rax, ia(%rip)
 movq $0, %rax
  movq ia(%rip), %rbx
  salq $3, %rax
  addq %rax, %rbx
  movq (%rbx), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 movq $1, %rax
  movq ia(%rip), %rbx
  salq $3, %rax
  addq %rax, %rbx
  movq (%rbx), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
.end_main:
 leave
 ret

.section .note.GNU-stack,"",@progbits
