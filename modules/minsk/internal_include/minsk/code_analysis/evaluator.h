#ifndef MINSK_CODE_ANALYSIS_EVALUATOR_H_
#define MINSK_CODE_ANALYSIS_EVALUATOR_H_

#include "minsk/code_analysis/binding/expression.h"
#include "minsk/code_analysis/symbol_table.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/runtime/object.h"

typedef struct {
  MskBoundExpression* root;
  MskSymbolTable_t* symbols;
} MskEvaluator;

MskEvaluator MskEvaluatorNew(MskBoundExpression* root,
                             MskSymbolTable_t* symbols);
MskRuntimeObject MskEvaluatorEvaluate(MskEvaluator* evaluator);

#endif  // MINSK_CODE_ANALYSIS_EVALUATOR_H_
