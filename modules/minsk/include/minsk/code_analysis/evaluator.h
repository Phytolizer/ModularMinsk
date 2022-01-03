#ifndef MINSK_CODE_ANALYSIS_EVALUATOR_H_
#define MINSK_CODE_ANALYSIS_EVALUATOR_H_

#include "minsk/code_analysis/diagnostics.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/runtime/object.h"

typedef struct {
  MskExpressionSyntax* root;
} MskEvaluator;

typedef enum {
  kMskEvaluationResultSuccess,
  kMskEvaluationResultFailure,
} MskEvaluationResultKind;

typedef struct {
  MskEvaluationResultKind kind;
  union {
    MskRuntimeObject ok;
    MskDiagnostics err;
  } value;
} MskEvaluationResult;

MskEvaluator MskEvaluatorNew(MskExpressionSyntax* root);
MskEvaluationResult MskEvaluatorEvaluate(MskEvaluator* evaluator);

#endif  // MINSK_CODE_ANALYSIS_EVALUATOR_H_
