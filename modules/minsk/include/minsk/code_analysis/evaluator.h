#ifndef MINSK_CODE_ANALYSIS_EVALUATOR_H_
#define MINSK_CODE_ANALYSIS_EVALUATOR_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/runtime/object.h"

typedef struct MskEvaluatorImpl MskEvaluatorImpl;

typedef struct {
  MskEvaluatorImpl* impl;
} MskEvaluator;

MskEvaluator MskEvaluatorNew(MskExpressionSyntax* root);
MskRuntimeObject MskEvaluatorEvaluate(MskEvaluator* evaluator);
void MskEvaluatorFree(MskEvaluator* e);

#endif  // MINSK_CODE_ANALYSIS_EVALUATOR_H_
