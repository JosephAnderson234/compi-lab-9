.data
print_fmt: .string "%ld \n"
ia: .quad 0

.text

.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $0, %rsp
 movq $3, %rax
 movq $8, %rcx
  imulq %rcx, %rax
  movq %rax, %rdi
  call malloc@PLT
  movq %rax, ia(%rip)
 movq $2, %rax
  pushq %rax
 movq $0, %rax
  movq %rax, %rdi
  popq %rax
  movq %rax, %rcx
  movq ia(%rip), %rax
  movq %rcx, (%rax, %rdi, 8)
 movq $2, %rax
 pushq %rax
 movq $3, %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
  pushq %rax
 movq $1, %rax
  movq %rax, %rdi
  popq %rax
  movq %rax, %rcx
  movq ia(%rip), %rax
  movq %rcx, (%rax, %rdi, 8)
 movq $2, %rax
 pushq %rax
 movq $3, %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 pushq %rax
 movq $4, %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
  pushq %rax
 movq $2, %rax
  movq %rax, %rdi
  popq %rax
  movq %rax, %rcx
  movq ia(%rip), %rax
  movq %rcx, (%rax, %rdi, 8)
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
 movq $2, %rax
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
