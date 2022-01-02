#ifndef MINSK_CODE_ANALYSIS_BINDING_BINDER_H_
#define MINSK_CODE_ANALYSIS_BINDING_BINDER_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk_private/code_analysis/binding/expression.h"

MskBoundExpression* MskBinderBind(MskExpressionSyntax* syntax);

#endif  // MINSK_CODE_ANALYSIS_BINDING_BINDER_H_
