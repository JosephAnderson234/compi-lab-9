.data
print_fmt: .string "%ld \n"

.text

.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
  movq $24, %rdi
  call malloc@PLT
  pushq %rax
  pushq %rax
 movq $5, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 0(%rax)
  pushq %rax
 movq $6, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 8(%rax)
  pushq %rax
 movq $7, %rax
  movq %rax, %rcx
  popq %rax
  movq %rcx, 16(%rax)
  popq %rax
  movq %rax, -8(%rbp)
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
 movq $9, %rax
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
