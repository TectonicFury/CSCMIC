#ifndef EVAL_H
#define EVAL_H
#include "Scanner.h"
#include "Expr.h"
enum EVAL_VALUE_TYPE_TAG {NUMBER_VALUE, STRING_VALUE, IDENTIFIER_VALUE, LIST_VALUE};
void* eval(expr e) {
  if (is_self_evaluating(e)) {
    if (e-) {
      /* code */
    }
  }
}
#endif
