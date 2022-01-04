#ifndef MINSK_CODE_ANALYSIS_EVALUATOR_H_
#define MINSK_CODE_ANALYSIS_EVALUATOR_H_

#include "minsk/code_analysis/symbol_table.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/runtime/object.h"
#include "minsk/code_analysis/binding/expression.h"

typedef struct {
  MskBoundExpression* root;
  MskSymbolTable* symbols;
} MskEvaluator;

MskEvaluator MskEvaluatorNew(MskBoundExpression* root, MskSymbolTable* symbols);
MskRuntimeObject MskEvaluatorEvaluate(MskEvaluator* evaluator);

#endif  // MINSK_CODE_ANALYSIS_EVALUATOR_H_
