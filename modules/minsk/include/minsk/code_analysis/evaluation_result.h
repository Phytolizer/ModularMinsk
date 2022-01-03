#ifndef MINSK_CODE_ANALYSIS_EVALUATION_RESULT_H_
#define MINSK_CODE_ANALYSIS_EVALUATION_RESULT_H_

#include "minsk/code_analysis/diagnostics.h"
#include "minsk/runtime/object.h"

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

#endif  // MINSK_CODE_ANALYSIS_EVALUATION_RESULT_H_
