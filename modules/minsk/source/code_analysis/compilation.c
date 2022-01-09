#include "minsk/code_analysis/compilation.h"

#include "minsk/code_analysis/binding/binder.h"
#include "minsk/code_analysis/evaluation_result.h"
#include "minsk/code_analysis/evaluator.h"
#include "minsk/code_analysis/symbol_table.h"

MskCompilation MskCompilationNew(MskSyntaxTree syntax) {
  return (MskCompilation){
      .syntax = syntax,
  };
}

MskEvaluationResult MskCompilationEvaluate(MskCompilation* compilation,
                                           MskSymbolTable_t* symbols) {
  MskBinder binder = {.symbols = symbols};
  MskBoundExpression* root =
      MskBinderBindExpression(&binder, compilation->syntax.root);
  if (binder.diagnostics.size > 0) {
    return (MskEvaluationResult){
        .kind = kMskEvaluationResultFailure,
        .value = {.err = binder.diagnostics},
    };
  }
  MskEvaluator evaluator = MskEvaluatorNew(root, symbols);
  return (MskEvaluationResult){
      .kind = kMskEvaluationResultSuccess,
      .value = {.ok = MskEvaluatorEvaluate(&evaluator)},
  };
}
