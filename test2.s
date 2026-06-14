.data
print_fmt: .string "%ld \n"

.text

.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq $3, %rax
 movq $8, %rcx
  imulq %rcx, %rax
  movq %rax, %rdi
  call malloc@PLT
  movq %rax, -8(%rbp)
 movq $2, %rax
  pushq %rax
 movq $0, %rax
  movq %rax, %rdi
  popq %rax
  movq %rax, %rcx
  movq -8(%rbp), %rax
  movq %rcx, (%rax, %rdi, 8)
 movq $3, %rax
  pushq %rax
 movq $1, %rax
  movq %rax, %rdi
  popq %rax
  movq %rax, %rcx
  movq -8(%rbp), %rax
  movq %rcx, (%rax, %rdi, 8)
 movq $4, %rax
  pushq %rax
 movq $2, %rax
  movq %rax, %rdi
  popq %rax
  movq %rax, %rcx
  movq -8(%rbp), %rax
  movq %rcx, (%rax, %rdi, 8)
 movq $0, %rax
  lea -8(%rbp), %rbx
  movq (%rbx), %rbx
  salq $3, %rax
  addq %rax, %rbx
  movq (%rbx), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 movq $1, %rax
  lea -8(%rbp), %rbx
  movq (%rbx), %rbx
  salq $3, %rax
  addq %rax, %rbx
  movq (%rbx), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 movq $2, %rax
  lea -8(%rbp), %rbx
  movq (%rbx), %rbx
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
