// =============================================================================
// visitor.cpp — Implementación de TypeCheckerVisitor y GenCodeVisitor
// =============================================================================

#include "visitor.h"
#include "ast.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

// =============================================================================
// Despacho del patrón Visitor (accept en cada nodo del AST)
// =============================================================================

int BinaryExp::accept(Visitor *v) { return v->visit(this); }
int NumberExp::accept(Visitor *v) { return v->visit(this); }
int IdExp::accept(Visitor *v) { return v->visit(this); }
int IdExp::computeAddress(Visitor *v) { return v->computeAddress(this); }
int Program::accept(Visitor *v) { return v->visit(this); }
int PrintStm::accept(Visitor *v) { return v->visit(this); }
int AssignStm::accept(Visitor *v) { return v->visit(this); }
int ExpListSize::accept(Visitor *v) { return v->visit(this); }
int ExpListVals::accept(Visitor *v) { return v->visit(this); }
int IfStm::accept(Visitor *v) { return v->visit(this); }
int WhileStm::accept(Visitor *v) { return v->visit(this); }
int Body::accept(Visitor *v) { return v->visit(this); }
int VarDec::accept(Visitor *v) { return v->visit(this); }
int StructDec::accept(Visitor *v) { return v->visit(this); }
int FcallExp::accept(Visitor *v) { return v->visit(this); }
int FunDec::accept(Visitor *v) { return v->visit(this); }
int ReturnStm::accept(Visitor *v) { return v->visit(this); }
int DoWhileStm::accept(Visitor *v) { return v->visit(this); }
int BreakStm::accept(Visitor *v) { return v->visit(this); }
int SwitchStm::accept(Visitor *v) { return v->visit(this); }
int UnaryExp::accept(Visitor *v) { return v->visit(this); }
int IndexExp::accept(Visitor *v) { return v->visit(this); }
int IndexExp ::computeAddress(Visitor *v) { return v->computeAddress(this); }
int FieldAccessExp::accept(Visitor *v) { return v->visit(this); }
int FieldAccessExp::computeAddress(Visitor *v) { return v->computeAddress(this); }
int MultiIndexExp::accept(Visitor *v) { return v->visit(this); }
int MultiIndexExp::computeAddress(Visitor *v) { return v->computeAddress(this); }
int ExpListSize2D::accept(Visitor *v) { return v->visit(this); }

// =============================================================================
// Implementación de accept()
// =============================================================================

// =============================================================================
// TypeCheckerVisitor — Análisis semántico
// =============================================================================
// Responsabilidades:
//   1. Registrar las funciones y su aridad antes de analizar los cuerpos.
//   2. Verificar que cada variable usada haya sido declarada en el scope.
//   3. Verificar que cada función llamada exista y reciba el número correcto
//      de argumentos.
//   4. Contar las variables locales de cada función para que GenCodeVisitor
//      pueda reservar el espacio correcto en el stack frame.
// =============================================================================

// -----------------------------------------------------------------------------
// TypeChecker — punto de entrada
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::TypeChecker(Program *program) {
  // Primera pasada: registrar structs
  for (auto sd : program->sdlist) {
    sd->accept(this);
  }

  // Segunda pasada: registrar todas las funciones y su aridad.
  // Esto permite llamadas hacia adelante (funciones que se usan antes de
  // ser definidas en el orden textual del fuente).
  for (auto fd : program->fdlist) {
    funAridad[fd->nombre] = static_cast<int>(fd->Pnombres.size());
  }

  // Tercera pasada: analizar cada función
  for (auto fd : program->fdlist) {
    fd->accept(this);
  }

  return 0;
}

// -----------------------------------------------------------------------------
// visit(FunDec) — analiza una declaración de función
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(FunDec *fd) {
  funcionActual = fd->nombre;
  locales = 0;
  int parametros = static_cast<int>(fd->Pnombres.size());

  // Abrir scope de la función
  entorno.add_level();

  // Registrar los parámetros como variables del scope actual
  for (size_t i = 0; i < fd->Pnombres.size(); i++) {
    entorno.add_var(fd->Pnombres[i], 0);
    if (structInfo.find(fd->Ptipos[i]) != structInfo.end())
      varStructType[fd->Pnombres[i]] = fd->Ptipos[i];
  }

  // Analizar el cuerpo
  fd->cuerpo->accept(this);

  // Cerrar scope
  entorno.remove_level();

  // Guardar el tamaño total del frame (parámetros + locales)
  funcontador[fd->nombre] = parametros + locales;
  return 0;
}

// -----------------------------------------------------------------------------
// visit(Body) — analiza declaraciones y sentencias del cuerpo
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(Body *body) {
  entorno.add_level();

  for (auto dec : body->declarations)
    dec->accept(this);
  for (auto stm : body->StmList)
    stm->accept(this);

  entorno.remove_level();
  return 0;
}

// -----------------------------------------------------------------------------
// visit(VarDec) — registra las variables declaradas y cuenta los locales
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(VarDec *vd) {
  // Si el tipo es un struct conocido, registrar el mapeo variable → struct
  if (structInfo.find(vd->type) != structInfo.end()) {
    for (auto &var : vd->vars) {
      varStructType[var] = vd->type;
    }
  }
  for (auto &nombre : vd->vars) {
    if (entorno.check(nombre)) {
      std::cerr << "[TypeChecker] Advertencia: la variable '" << nombre
                << "' ya fue declarada en este scope"
                << " (en función '" << funcionActual << "').\n";
    }
    entorno.add_var(nombre, 0);
    locales++;
  }
  return 0;
}

// -----------------------------------------------------------------------------
// visit(IdExp) — verifica que la variable esté declarada
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(IdExp *exp) {
  if (!entorno.check(exp->value)) {
    throw std::runtime_error("[TypeChecker] Variable no declarada: '" +
                             exp->value + "' usada en la función '" +
                             funcionActual + "'");
  }
  return 0;
}

// -----------------------------------------------------------------------------
// (IdExp) — verifica que la variable esté declarada
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::computeAddress(IdExp *exp) { return 0; }

// -----------------------------------------------------------------------------
// visit(AssignStm) — verifica variable y evalúa expresión
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(AssignStm *stm) { return 0; }

// -----------------------------------------------------------------------------
// visit(ExpListSize) — verifica variable y evalúa expresión
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(ExpListSize *StmListSize) { return 0; }

// -----------------------------------------------------------------------------
// visit(ExpListVals) — verifica variable y evalúa expresión
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(ExpListVals *StmListVals) { return 0; }

// -----------------------------------------------------------------------------
// visit(FcallExp) — verifica existencia de la función y aridad
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(FcallExp *fcall) {
  // Verificar que la función exista
  if (funAridad.find(fcall->nombre) == funAridad.end()) {
    throw std::runtime_error("[TypeChecker] Función no definida: '" +
                             fcall->nombre + "' llamada en '" + funcionActual +
                             "'");
  }

  // Verificar número de argumentos
  int esperados = funAridad[fcall->nombre];
  int recibidos = static_cast<int>(fcall->argumentos.size());
  if (recibidos != esperados) {
    throw std::runtime_error("[TypeChecker] La función '" + fcall->nombre +
                             "' espera " + std::to_string(esperados) +
                             " argumento(s), pero se pasaron " +
                             std::to_string(recibidos));
  }

  // Verificar cada argumento
  for (auto arg : fcall->argumentos)
    arg->accept(this);

  return 0;
}

// -----------------------------------------------------------------------------
// visit(IfStm) — cuenta máximo de locales entre ramas
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(IfStm *stm) {
  stm->condition->accept(this);

  int base = locales;

  // Rama then
  locales = 0;
  stm->then->accept(this);
  int maxLocales = locales;

  // Rama else (opcional)
  if (stm->els) {
    locales = 0;
    stm->els->accept(this);
    maxLocales = std::max(maxLocales, locales);
  }

  // El frame necesita el máximo de ambas ramas
  locales = base + maxLocales;
  return 0;
}

// -----------------------------------------------------------------------------
// visit(WhileStm)
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(WhileStm *stm) {
  stm->condition->accept(this);
  stm->b->accept(this);
  return 0;
}

// -----------------------------------------------------------------------------
// visit(PrintStm)
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(PrintStm *stm) {
  stm->e->accept(this);
  return 0;
}

// -----------------------------------------------------------------------------
// visit(ReturnStm)
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(ReturnStm *r) {
  r->e->accept(this);
  return 0;
}

// -----------------------------------------------------------------------------
// Nodos que no requieren verificación semántica adicional
// -----------------------------------------------------------------------------

int TypeCheckerVisitor::visit(BinaryExp *exp) {
  exp->left->accept(this);
  exp->right->accept(this);
  return 0;
}

int TypeCheckerVisitor::visit(UnaryExp *exp) {
  exp->operand->accept(this);
  return 0;
}
int TypeCheckerVisitor::visit(IndexExp *exp) {
  // TODO
  return 0;
}

int TypeCheckerVisitor::computeAddress(IndexExp *exp) { return 0; }

int TypeCheckerVisitor::visit(NumberExp *exp) { return 0; }
int TypeCheckerVisitor::visit(Program *p) {
  // Struct definitions already processed in TypeChecker entry point
  for (auto dec : p->vdlist)
    dec->accept(this);
  return 0;
}

int TypeCheckerVisitor::visit(DoWhileStm *stm) {
  stm->condition->accept(this);
  stm->b->accept(this);
  return 0;
}

int TypeCheckerVisitor::visit(BreakStm *stm) { return 0; }

int TypeCheckerVisitor::visit(SwitchStm *stm) {
  stm->e->accept(this);
  for (auto c : stm->cases) {
    for (auto s : c->body)
      s->accept(this);
  }
  for (auto s : stm->default_body)
    s->accept(this);
  return 0;
}

// =============================================================================
// TypeCheckerVisitor — Nuevos nodos
// =============================================================================

int TypeCheckerVisitor::visit(StructDec *sd) {
  std::vector<std::string> fieldNames;
  for (auto field : sd->fields) {
    for (auto &varName : field->vars) {
      fieldNames.push_back(varName);
    }
  }
  structInfo[sd->name] = fieldNames;
  return 0;
}

int TypeCheckerVisitor::visit(FieldAccessExp *exp) {
  if (!entorno.check(exp->name)) {
    throw std::runtime_error("[TypeChecker] Variable no declarada: '" +
                             exp->name + "'");
  }
  if (varStructType.find(exp->name) == varStructType.end()) {
    throw std::runtime_error("[TypeChecker] La variable '" + exp->name +
                             "' no es de tipo struct");
  }
  std::string structType = varStructType[exp->name];
  auto &fields = structInfo[structType];
  bool found = false;
  for (auto &f : fields) {
    if (f == exp->field) { found = true; break; }
  }
  if (!found) {
    throw std::runtime_error("[TypeChecker] El struct '" + structType +
                             "' no tiene campo '" + exp->field + "'");
  }
  return 0;
}

int TypeCheckerVisitor::computeAddress(FieldAccessExp *exp) { return 0; }

int TypeCheckerVisitor::visit(MultiIndexExp *exp) {
  if (!entorno.check(exp->name)) {
    throw std::runtime_error("[TypeChecker] Variable no declarada: '" +
                             exp->name + "'");
  }
  if (exp->indices.size() != 2) {
    throw std::runtime_error("[TypeChecker] La matriz '" + exp->name +
                             "' requiere exactamente 2 indices");
  }
  for (auto idx : exp->indices)
    idx->accept(this);
  return 0;
}

int TypeCheckerVisitor::computeAddress(MultiIndexExp *exp) { return 0; }

int TypeCheckerVisitor::visit(ExpListSize2D *stm) {
  stm->size1->accept(this);
  stm->size2->accept(this);
  return 0;
}

// =============================================================================
// GenCodeVisitor — Generación de código ensamblador x86-64 (AT&T syntax)
// =============================================================================
// Convenciones usadas:
//   · Registros de argumentos: %rdi, %rsi, %rdx, %rcx, %r8, %r9
//   · Resultado de expresiones en %rax
//   · Variables locales: offsets negativos desde %rbp
//   · Variables globales: símbolos en .data con acceso RIP-relativo
//   · printf para print: formato en print_fmt ("%ld \n")
// =============================================================================

// -----------------------------------------------------------------------------
// generar — punto de entrada de la generación
// -----------------------------------------------------------------------------

int GenCodeVisitor::generar(Program *program) {
  tipos.TypeChecker(program);
  funcontador = tipos.funcontador;
  program->accept(this);
  return 0;
}

// -----------------------------------------------------------------------------
// visit(Program)
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(Program *program) {
  // Sección de datos
  out << ".data\n";
  out << "print_fmt: .string \"%ld \\n\"\n";

  // Recolectar nombres de variables globales
  for (auto dec : program->vdlist)
    dec->accept(this);

  // Emitir las etiquetas .quad para las globales
  for (auto &[var, _] : memoriaGlobal)
    out << var << ": .quad 0\n";

  // Sección de texto (código)
  out << "\n.text\n";

  for (auto fd : program->fdlist)
    fd->accept(this);

  // Marca el stack como no ejecutable (requerido por el linker moderno)
  out << "\n.section .note.GNU-stack,\"\",@progbits\n";
  return 0;
}

// -----------------------------------------------------------------------------
// visit(VarDec) — registra variables en memoria (global o local)
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(VarDec *stm) {
  for (auto &var : stm->vars) {
    if (!entornoFuncion) {
      memoriaGlobal[var] = true;
    } else {
      memoria[var] = offset;
      offset -= 8;
    }
  }
  return 0;
}

// -----------------------------------------------------------------------------
// visit(NumberExp) — carga un inmediato en %rax
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(NumberExp *exp) {
  out << " movq $" << exp->value << ", %rax\n";
  return 0;
}

// -----------------------------------------------------------------------------
// visit(ExpListSize) — evalúa la expresión para reservar el espacio apropiado y
// dejarlo en rax
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(ExpListSize *stm) {
  // TEST
  // Lista de Int: malloc de 8*n bytes
  stm->size->accept(this);
  out << " movq $8, %rcx\n";
  out << "  imulq %rcx, %rax\n";
  out << "  movq %rax, %rdi\n"
      << "  call malloc@PLT\n";
  // El puntero se deja en rax
  return 0;
}

// -----------------------------------------------------------------------------
// visit(ExpListVals) — evalúa las expresiones expresión y las almacena en los
// espacios reservados de memoria y luego regresa el puntero
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(ExpListVals *stm) {
  // TEST
  // Lista de Int: malloc de 8*n bytes + almacenar cada entero
  int n = stm->values.size();
  out << "  movq $" << (n * 8) << ", %rdi\n"
      << "  call malloc@PLT\n"
      << "  pushq %rax\n";
  for (size_t i = 0; i < n; ++i) {
    out << "  pushq %rax\n";
    stm->values[i]->accept(this); // → %rax = valor entero
    out << "  movq %rax, %rcx\n";
    // Pops to have the rax pointer saved at the start
    out << "  popq %rax\n";
    // Uses rax to access the index values
    out << "  movq %rcx, " << (i * 8) << "(%rax)\n";
  }

  out << "  popq %rax\n";
  return 0;
}

// -----------------------------------------------------------------------------
// visit(IdExp) — carga el valor de una variable en %rax
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(IdExp *exp) {
  if (memoriaGlobal.count(exp->value))
    out << " movq " << exp->value << "(%rip), %rax\n";
  else
    out << " movq " << memoria[exp->value] << "(%rbp), %rax\n";
  return 0;
}

// visit(UnaryExp) — operador unario NOT
int GenCodeVisitor::visit(UnaryExp *exp) {
  exp->operand->accept(this);
  int lbl = labelcont++;
  out << " cmpq $0, %rax\n";
  out << " je not_true_" << lbl << "\n";
  out << " movq $0, %rax\n";
  out << " jmp not_end_" << lbl << "\n";
  out << "not_true_" << lbl << ":\n";
  out << " movq $1, %rax\n";
  out << "not_end_" << lbl << ":\n";
  return 0;
}

// visit(IndexExp) — operador para indices de listas
int GenCodeVisitor::visit(IndexExp *exp) {
  // TODO
  // 1) Evaluar el índice → %rax
  exp->index->accept(this);

  // 2) Cargar la dirección base del array en %rbx
  if (memoriaGlobal.count(exp->name)) {
    out << "  movq " << exp->name << "(%rip), %rbx\n"; // global
  } else {
    int off = memoria[exp->name];
    out << "  lea " << off << "(%rbp), %rbx\n"; // &var
    out << "  movq (%rbx), %rbx\n";             // ptr heap
  }

  // 3) Determinar tamaño de elemento (por defecto 8 por enteros)
  int esz = 8;

  // 4) Indexación para enteros

  out << "  salq $3, %rax\n";
  out << "  addq %rax, %rbx\n";
  out << "  movq (%rbx), %rax\n";
  return 0;
}

// -----------------------------------------------------------------------------
// visit(BinaryExp) — evalúa left y right, aplica operador
// Convención: left en %rax, right en %rcx
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(BinaryExp *exp) {
  exp->left->accept(this);
  out << " pushq %rax\n";
  exp->right->accept(this);
  out << " movq %rax, %rcx\n";
  out << " popq %rax\n";

  switch (exp->op) {
  case PLUS_OP:
    out << " addq %rcx, %rax\n";
    break;
  case MINUS_OP:
    out << " subq %rcx, %rax\n";
    break;
  case MUL_OP:
    out << " imulq %rcx, %rax\n";
    break;
  case DIV_OP:
    // División entera con signo: idivq usa %rdx:%rax / %rcx
    out << " cqto\n";       // sign-extend %rax → %rdx:%rax
    out << " idivq %rcx\n"; // cociente en %rax
    break;
  case LE_OP:
    out << " cmpq %rcx, %rax\n";
    out << " movq $0, %rax\n";
    out << " setl %al\n";
    out << " movzbq %al, %rax\n";
    break;
  case GT_OP:
    out << " cmpq %rcx, %rax\n";
    out << " movq $0, %rax\n";
    out << " setg %al\n";
    out << " movzbq %al, %rax\n";
    break;
  case LEQ_OP:
    out << " cmpq %rcx, %rax\n";
    out << " movq $0, %rax\n";
    out << " setle %al\n";
    out << " movzbq %al, %rax\n";
    break;
  case GEQ_OP:
    out << " cmpq %rcx, %rax\n";
    out << " movq $0, %rax\n";
    out << " setge %al\n";
    out << " movzbq %al, %rax\n";
    break;
  case EQ_OP:
    out << " cmpq %rcx, %rax\n";
    out << " movq $0, %rax\n";
    out << " sete %al\n";
    out << " movzbq %al, %rax\n";
    break;
  case NE_OP:
    out << " cmpq %rcx, %rax\n";
    out << " movq $0, %rax\n";
    out << " setne %al\n";
    out << " movzbq %al, %rax\n";
    break;
  case AND_OP:
    out << " andq %rcx, %rax\n";
    break;
  case OR_OP:
    out << " orq %rcx, %rax\n";
    break;
  }
  return 0;
}

// -----------------------------------------------------------------------------
// visit(AssignStm) — evalúa expresión y almacena resultado
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(AssignStm *stm) {
  stm->e->accept(this);
  stm->target->computeAddress(this);
  return 0;
}

// -----------------------------------------------------------------------------
// computeAdress(IdExp) — Asigna el rax hacia la posicion correcta de memoria
// -----------------------------------------------------------------------------

int GenCodeVisitor::computeAddress(IdExp *id) {
  if (memoriaGlobal.count(id->value)) {
    out << "  movq %rax, " << id->value << "(%rip)\n";
  } else {
    int off = memoria[id->value];
    out << "  movq %rax, " << off << "(%rbp)\n";
  }
  return 0;
}

// -----------------------------------------------------------------------------
// computeAdress(IndexExp) — Asigna el rax hacia la posicion correcta de memoria
// -----------------------------------------------------------------------------

int GenCodeVisitor::computeAddress(IndexExp *idx) {
  // Not perfect as it usses a third register rdi
  // OPTIMIZE
  out << "  pushq %rax\n";

  idx->index->accept(this);

  out << "  movq %rax, %rdi\n"  // Saves the index in rdi
      << "  popq %rax\n"        // pops the index to have the value to assign
      << "  movq %rax, %rcx\n"; // Assigns the exp to rcx
  if (memoriaGlobal.count(idx->name)) {
    out << "  movq " << idx->name
        << "(%rip), %rax\n"; // Assigns the array in memory to the index
  } else {
    int off = memoria[idx->name];
    out << "  movq " << off
        << "(%rbp), %rax\n"; // Assigns the array in memory to the index
  }
  out << "  movq %rcx, (%rax, %rdi, 8)\n"; // asssigns the rcx wo the  array
                                           // in the correct offset

  return 0;
}

// -----------------------------------------------------------------------------
// visit(PrintStm) — imprime un entero con printf
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(PrintStm *stm) {
  stm->e->accept(this);
  out << " movq %rax, %rsi\n";
  out << " leaq print_fmt(%rip), %rdi\n";
  out << " movq $0, %rax\n";
  out << " call printf@PLT\n";
  return 0;
}

// -----------------------------------------------------------------------------
// visit(Body) — procesa declaraciones y sentencias
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(Body *b) {
  for (auto dec : b->declarations)
    dec->accept(this);
  for (auto stm : b->StmList)
    stm->accept(this);
  return 0;
}

// -----------------------------------------------------------------------------
// visit(IfStm) — emite bloque if-then-else con etiquetas únicas
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(IfStm *stm) {
  int lbl = labelcont++;
  stm->condition->accept(this);
  out << " cmpq $0, %rax\n";
  out << " je else_" << lbl << "\n";
  stm->then->accept(this);
  out << " jmp endif_" << lbl << "\n";
  out << "else_" << lbl << ":\n";
  if (stm->els)
    stm->els->accept(this);
  out << "endif_" << lbl << ":\n";
  return 0;
}

// -----------------------------------------------------------------------------
// visit(WhileStm) — emite bucle while con etiquetas únicas
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(WhileStm *stm) {

  int lbl = labelcont++;

  std::string oldBreak = currentBreakLabel;
  currentBreakLabel = "endwhile_" + std::to_string(lbl);

  out << "while_" << lbl << ":\n";

  stm->condition->accept(this);

  out << " cmpq $0, %rax\n";
  out << " je endwhile_" << lbl << "\n";

  stm->b->accept(this);

  out << " jmp while_" << lbl << "\n";

  out << "endwhile_" << lbl << ":\n";

  currentBreakLabel = oldBreak;

  return 0;
}

// -----------------------------------------------------------------------------
// visit(ReturnStm) — salta al epílogo de la función
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(ReturnStm *stm) {
  stm->e->accept(this);
  out << " jmp .end_" << nombreFuncion << "\n";
  return 0;
}

// -----------------------------------------------------------------------------
// visit(FunDec) — emite prólogo, cuerpo y epílogo de una función
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(FunDec *f) {
  entornoFuncion = true;
  memoria.clear();
  offset = -8;
  nombreFuncion = f->nombre;

  const std::vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx",
                                            "%rcx", "%r8",  "%r9"};

  // ---- Prólogo ----
  out << "\n.globl " << f->nombre << "\n";
  out << f->nombre << ":\n";
  out << " pushq %rbp\n";
  out << " movq %rsp, %rbp\n";
  out << " subq $" << funcontador[f->nombre] * 8 << ", %rsp\n";

  // Guardar parámetros en el frame local
  int nParams = static_cast<int>(f->Pnombres.size());
  for (int i = 0; i < nParams; i++) {
    memoria[f->Pnombres[i]] = offset;
    out << " movq " << argRegs[i] << ", " << offset << "(%rbp)\n";
    offset -= 8;
  }

  // Registrar variables locales declaradas (ajusta 'offset' y 'memoria')
  for (auto dec : f->cuerpo->declarations)
    dec->accept(this);

  // ---- Cuerpo ----
  for (auto stm : f->cuerpo->StmList)
    stm->accept(this);

  // ---- Epílogo ----
  out << ".end_" << f->nombre << ":\n";
  out << " leave\n";
  out << " ret\n";

  entornoFuncion = false;
  return 0;
}

// -----------------------------------------------------------------------------
// visit(FcallExp) — emite una llamada a función
// Argumentos en registros según la ABI System V x86-64
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(FcallExp *exp) {
  const std::vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx",
                                            "%rcx", "%r8",  "%r9"};

  int nArgs = static_cast<int>(exp->argumentos.size());
  for (int i = 0; i < nArgs; i++) {
    exp->argumentos[i]->accept(this);
    out << " movq %rax, " << argRegs[i] << "\n";
  }
  out << " call " << exp->nombre << "\n";
  return 0;
}

int GenCodeVisitor::visit(DoWhileStm *stm) {

  int lbl = labelcont++;

  std::string oldBreak = currentBreakLabel;
  currentBreakLabel = "endwhile_" + std::to_string(lbl);

  out << "dowhile_" << lbl << ":\n";

  stm->b->accept(this);

  stm->condition->accept(this);

  out << " cmpq $0, %rax\n";
  out << " jne dowhile_" << lbl << "\n";

  out << "endwhile_" << lbl << ":\n";

  currentBreakLabel = oldBreak;

  return 0;
}

int GenCodeVisitor::visit(BreakStm *stm) {

  if (currentBreakLabel.empty()) {
    std::cerr << "Error: break fuera de while, do-while o switch\n";
    exit(1);
  }

  out << " jmp " << currentBreakLabel << "\n";

  return 0;
}

int GenCodeVisitor::visit(SwitchStm *stm) {

  int lbl = labelcont++;

  stm->e->accept(this);

  out << " movq %rax, %r10\n";

  for (auto c : stm->cases) {

    out << " movq $" << c->value << ", %rax\n";
    out << " cmpq %rax, %r10\n";
    out << " je case_" << lbl << "_" << c->value << "\n";
  }

  if (!stm->default_body.empty())
    out << " jmp default_" << lbl << "\n";
  else
    out << " jmp endswitch_" << lbl << "\n";

  std::string oldBreak = currentBreakLabel;
  currentBreakLabel = "endswitch_" + std::to_string(lbl);

  for (auto c : stm->cases) {

    out << "case_" << lbl << "_" << c->value << ":\n";

    for (auto s : c->body)
      s->accept(this);

    out << " jmp endswitch_" << lbl << "\n";
  }

  if (!stm->default_body.empty()) {

    out << "default_" << lbl << ":\n";

    for (auto s : stm->default_body)
      s->accept(this);
  }

  currentBreakLabel = oldBreak;

  out << "endswitch_" << lbl << ":\n";

  return 0;
}

// =============================================================================
// GenCodeVisitor — Nuevos nodos
// =============================================================================

// -----------------------------------------------------------------------------
// StructDec — las definiciones de struct son información de tipo,
// no generan código ensamblador.
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(StructDec *sd) { return 0; }

// -----------------------------------------------------------------------------
// FieldAccessExp — acceso a campo de struct (lectura)
//   carga el puntero del struct y accede al offset del campo
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(FieldAccessExp *exp) {
  // Cargar puntero del struct
  if (memoriaGlobal.count(exp->name))
    out << " movq " << exp->name << "(%rip), %rax\n";
  else
    out << " movq " << memoria[exp->name] << "(%rbp), %rax\n";

  // Encontrar offset del campo
  std::string structType = tipos.varStructType[exp->name];
  auto &fields = tipos.structInfo[structType];
  int fieldIndex = 0;
  for (size_t i = 0; i < fields.size(); i++) {
    if (fields[i] == exp->field) { fieldIndex = i; break; }
  }
  int fieldOffset = fieldIndex * 8;

  out << " movq " << fieldOffset << "(%rax), %rax\n";
  return 0;
}

// -----------------------------------------------------------------------------
// computeAddress(FieldAccessExp) — asignación a campo de struct (escritura)
//   p.x = expr  → el valor está en %rax, se almacena en el offset del campo
// -----------------------------------------------------------------------------

int GenCodeVisitor::computeAddress(FieldAccessExp *idx) {
  out << " pushq %rax\n";

  if (memoriaGlobal.count(idx->name))
    out << " movq " << idx->name << "(%rip), %rax\n";
  else
    out << " movq " << memoria[idx->name] << "(%rbp), %rax\n";

  std::string structType = tipos.varStructType[idx->name];
  auto &fields = tipos.structInfo[structType];
  int fieldIndex = 0;
  for (size_t i = 0; i < fields.size(); i++) {
    if (fields[i] == idx->field) { fieldIndex = i; break; }
  }
  int fieldOffset = fieldIndex * 8;

  out << " popq %rcx\n";
  out << " movq %rcx, " << fieldOffset << "(%rax)\n";
  return 0;
}

// -----------------------------------------------------------------------------
// ExpListSize2D — new int[rows][cols]
//   Asigna (rows * cols + 1) * 8 bytes (cabecera guarda nCols)
//   Retorna puntero al área de datos (base + 8)
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(ExpListSize2D *stm) {
  // Evaluar rows
  stm->size1->accept(this);
  out << " pushq %rax\n";       // guardar rows

  // Evaluar cols
  stm->size2->accept(this);
  out << " pushq %rax\n";       // guardar cols

  // Calcular total = (rows * cols + 1) * 8
  out << " popq %rcx\n";        // rcx = cols
  out << " popq %rax\n";        // rax = rows
  out << " pushq %rcx\n";       // guardar cols para después de malloc
  out << " imulq %rcx, %rax\n"; // rax = rows * cols
  out << " addq $1, %rax\n";    // +1 para cabecera
  out << " salq $3, %rax\n";    // * 8
  out << " movq %rax, %rdi\n";
  out << " call malloc@PLT\n";  // rax = ptr

  // Guardar nCols en cabecera (offset 0)
  out << " popq %rcx\n";        // rcx = cols
  out << " movq %rcx, 0(%rax)\n";  // ptr[0] = nCols

  // Retornar puntero al área de datos (ptr + 8)
  out << " lea 8(%rax), %rax\n";
  return 0;
}

// -----------------------------------------------------------------------------
// MultiIndexExp — acceso a matriz 2D (lectura)
//   m[i][j] → carga puntero, lee nCols de cabecera, calcula offset
// -----------------------------------------------------------------------------

int GenCodeVisitor::visit(MultiIndexExp *exp) {
  // Evaluar j (segundo índice)
  exp->indices[1]->accept(this);
  out << " movq %rax, %rdi\n";   // rdi = j

  // Evaluar i (primer índice)
  exp->indices[0]->accept(this);
  out << " pushq %rax\n";        // guardar i

  // Cargar puntero base (apunta al área de datos = malloc + 8)
  if (memoriaGlobal.count(exp->name))
    out << " movq " << exp->name << "(%rip), %rbx\n";
  else
    out << " movq " << memoria[exp->name] << "(%rbp), %rbx\n";

  // Leer nCols de la cabecera (en malloc+0, o sea rbx-8)
  out << " movq -8(%rbx), %rsi\n"; // rsi = nCols

  // Calcular offset = i * nCols + j
  out << " popq %rax\n";          // rax = i
  out << " imulq %rsi, %rax\n";   // rax = i * nCols
  out << " addq %rdi, %rax\n";    // rax = offset

  // Leer valor en (rbx + offset * 8), rbx ya apunta al area de datos
  out << " movq (%rbx, %rax, 8), %rax\n";
  return 0;
}

// -----------------------------------------------------------------------------
// computeAddress(MultiIndexExp) — asignación a matriz 2D (escritura)
//   m[i][j] = expr
// -----------------------------------------------------------------------------

int GenCodeVisitor::computeAddress(MultiIndexExp *idx) {
  out << " pushq %rax\n";        // guardar valor a asignar

  // Evaluar j (segundo índice) → rdi
  idx->indices[1]->accept(this);
  out << " movq %rax, %rdi\n";

  // Evaluar i (primer índice) → rsi
  idx->indices[0]->accept(this);
  out << " movq %rax, %rsi\n";

  // Cargar puntero base (apunta al área de datos = malloc + 8)
  if (memoriaGlobal.count(idx->name))
    out << " movq " << idx->name << "(%rip), %rbx\n";
  else
    out << " movq " << memoria[idx->name] << "(%rbp), %rbx\n";

  // Leer nCols de la cabecera (en malloc+0, o sea rbx-8)
  out << " movq -8(%rbx), %r8\n";  // r8 = nCols

  // Calcular offset = i * nCols + j
  out << " movq %rsi, %rax\n";    // rax = i
  out << " imulq %r8, %rax\n";    // rax = i * nCols
  out << " addq %rdi, %rax\n";    // rax = offset

  // Restaurar valor a asignar
  out << " popq %rcx\n";          // rcx = valor

  // Almacenar en (rbx + offset * 8), rbx ya apunta al area de datos
  out << " movq %rcx, (%rbx, %rax, 8)\n";
  return 0;
}
