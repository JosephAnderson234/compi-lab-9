#ifndef VISITOR_H
#define VISITOR_H

// =============================================================================
// visitor.h — Definición de los visitantes del AST
// =============================================================================
// Se implementan dos visitantes sobre el patrón Visitor:
//
//   TypeCheckerVisitor — Análisis semántico:
//     · Verifica que las variables usadas hayan sido declaradas.
//     · Verifica que las funciones llamadas existan.
//     · Verifica que las llamadas tengan el número correcto de argumentos.
//     · Cuenta las variables locales de cada función para la generación.
//
//   GenCodeVisitor — Generación de código x86-64 (AT&T syntax):
//     · Emite ensamblador para cada nodo del AST.
//     · Usa TypeCheckerVisitor para obtener metadatos (tamaño de frame, etc.).
// =============================================================================

#include "ast.h"
#include "environment.h"
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

// Forward declarations
class BinaryExp;
class Body;
class BreakStm;
class DoWhileStm;
class FunDec;
class FcallExp;
class IdExp;
class IndexExp;
class FieldAccessExp;
class MultiIndexExp;
class IfStm;
class NumberExp;
class PrintStm;
class Program;
class ReturnStm;
class Stm;
class SwitchStm;
class UnaryExp;
class VarDec;
class StructDec;
class WhileStm;
class ExpListSize;
class ExpListSize2D;
class ExpListVals;

// =============================================================================
// Clase base abstracta Visitor
// =============================================================================

class Visitor {
public:
  virtual int visit(BinaryExp *exp) = 0;
  virtual int visit(NumberExp *exp) = 0;
  virtual int visit(IdExp *exp) = 0;
  virtual int computeAddress(IdExp *exp) = 0;
  virtual int visit(UnaryExp *exp) = 0;
  virtual int visit(IndexExp *exp) = 0;
  virtual int computeAddress(IndexExp *exp) = 0;
  virtual int visit(FieldAccessExp *exp) = 0;
  virtual int computeAddress(FieldAccessExp *exp) = 0;
  virtual int visit(MultiIndexExp *exp) = 0;
  virtual int computeAddress(MultiIndexExp *exp) = 0;
  virtual int visit(Program *p) = 0;
  virtual int visit(PrintStm *stm) = 0;
  virtual int visit(WhileStm *stm) = 0;
  virtual int visit(DoWhileStm *stm) = 0;
  virtual int visit(IfStm *stm) = 0;
  virtual int visit(AssignStm *stm) = 0;
  virtual int visit(ExpListSize *stm) = 0;
  virtual int visit(ExpListSize2D *stm) = 0;
  virtual int visit(ExpListVals *stm) = 0;
  virtual int visit(BreakStm *stm) = 0;
  virtual int visit(SwitchStm *stm) = 0;
  virtual int visit(Body *body) = 0;
  virtual int visit(VarDec *vd) = 0;
  virtual int visit(StructDec *sd) = 0;
  virtual int visit(FcallExp *fc) = 0;
  virtual int visit(ReturnStm *r) = 0;
  virtual int visit(FunDec *fd) = 0;
};

// =============================================================================
// TypeCheckerVisitor — Análisis semántico
// =============================================================================

class TypeCheckerVisitor : public Visitor {
public:
  // Mapa función → número total de variables en el frame (parámetros + locales)
  std::unordered_map<std::string, int> funcontador;

  // Mapa función → número de parámetros (para verificar aridad en llamadas)
  std::unordered_map<std::string, int> funAridad;

  // Información de structs: nombre → lista de nombres de campos
  std::unordered_map<std::string, std::vector<std::string>> structInfo;

  // Tipo de cada variable (nombre var → nombre tipo struct)
  std::unordered_map<std::string, std::string> varStructType;

  // Contador de variables locales del cuerpo actual (acumulado por
  // visit(VarDec))
  int locales;

  // Entorno de variables: lleva el alcance (scope) actual
  Environment<int> entorno;

  // Nombre de la función que se está analizando (para mensajes de error)
  std::string funcionActual;

  // Punto de entrada del análisis semántico
  int TypeChecker(Program *program);

  // ---- Visitas ----
  int visit(BinaryExp *exp) override;
  int visit(NumberExp *exp) override;
  int visit(IdExp *exp) override;
  int computeAddress(IdExp *exp) override;
  int visit(UnaryExp *exp) override;
  int visit(IndexExp *exp) override;
  int computeAddress(IndexExp *exp) override;
  int visit(FieldAccessExp *exp) override;
  int computeAddress(FieldAccessExp *exp) override;
  int visit(MultiIndexExp *exp) override;
  int computeAddress(MultiIndexExp *exp) override;
  int visit(Program *p) override;
  int visit(PrintStm *stm) override;
  int visit(AssignStm *stm) override;
  int visit(ExpListSize *stm) override;
  int visit(ExpListSize2D *stm) override;
  int visit(ExpListVals *stm) override;
  int visit(WhileStm *stm) override;
  int visit(DoWhileStm *stm) override;
  int visit(IfStm *stm) override;
  int visit(BreakStm *stm) override;
  int visit(SwitchStm *stm) override;
  int visit(Body *body) override;
  int visit(VarDec *vd) override;
  int visit(StructDec *sd) override;
  int visit(FcallExp *fc) override;
  int visit(ReturnStm *r) override;
  int visit(FunDec *fd) override;
};

// =============================================================================
// GenCodeVisitor — Generación de código ensamblador x86-64
// =============================================================================

class GenCodeVisitor : public Visitor {
private:
  std::ostream &out; // Stream de salida (archivo .s)

public:
  TypeCheckerVisitor tipos; // Resultado del análisis semántico

  // Mapa función → tamaño del frame (obtenido del TypeChecker)
  std::unordered_map<std::string, int> funcontador;

  // Mapa variable local → offset desde %rbp
  std::unordered_map<std::string, int> memoria;

  // Conjunto de variables globales
  std::unordered_map<std::string, bool> memoriaGlobal;

  int offset = -8;               // Próximo offset disponible en el frame local
  int labelcont = 0;             // Contador de etiquetas únicas para if/while
  bool entornoFuncion = false;   // ¿Estamos dentro de una función?
  std::string currentBreakLabel; // Etiqueta a la que deben saltar los break
                                 // dentro del ciclo actual
  std::string nombreFuncion;     // Nombre de la función actual

  GenCodeVisitor(std::ostream &out) : out(out) {}

  // Punto de entrada de la generación
  int generar(Program *program);

  // ---- Visitas ----
  int visit(BinaryExp *exp) override;
  int visit(NumberExp *exp) override;
  int visit(IdExp *exp) override;
  int computeAddress(IdExp *exp) override;
  int visit(UnaryExp *exp) override;
  int visit(IndexExp *exp) override;
  int computeAddress(IndexExp *exp) override;
  int visit(FieldAccessExp *exp) override;
  int computeAddress(FieldAccessExp *exp) override;
  int visit(MultiIndexExp *exp) override;
  int computeAddress(MultiIndexExp *exp) override;
  int visit(Program *p) override;
  int visit(PrintStm *stm) override;
  int visit(AssignStm *stm) override;
  int visit(ExpListSize *stm) override;
  int visit(ExpListSize2D *stm) override;
  int visit(ExpListVals *stm) override;
  int visit(WhileStm *stm) override;
  int visit(DoWhileStm *stm) override;
  int visit(IfStm *stm) override;
  int visit(BreakStm *stm) override;
  int visit(SwitchStm *stm) override;
  int visit(Body *body) override;
  int visit(VarDec *vd) override;
  int visit(StructDec *sd) override;
  int visit(FcallExp *fc) override;
  int visit(ReturnStm *r) override;
  int visit(FunDec *fd) override;
};

#endif // VISITOR_H
