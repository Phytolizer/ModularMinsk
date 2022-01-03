#ifndef MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_
#define MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_

#include <stdint.h>
#include <string/string.h>
#include <vec/vec.h>

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/text/diagnostic.h"
#include "minsk/runtime/object.h"

typedef VEC_TYPE(MskDiagnostic) MskDiagnosticBag;

void MskDiagnosticBagFree(MskDiagnosticBag* diagnostics);
void MskDiagnosticBagReportInvalidNumber(MskDiagnosticBag* diagnostics,
                                         MskTextSpan span,
                                         StringView text,
                                         StringView type);
void MskDiagnosticBagReportBadCharacter(MskDiagnosticBag* diagnostics,
                                        uint64_t position,
                                        char ch);
void MskDiagnosticBagReportUnexpectedToken(MskDiagnosticBag* diagnostics,
                                           MskTextSpan span,
                                           MskSyntaxKind expected,
                                           MskSyntaxKind actual);
void MskDiagnosticBagReportUndefinedUnaryOperator(
    MskDiagnosticBag* diagnostics,
    MskTextSpan span,
    StringView operator_name,
    MskRuntimeObjectKind operand_type);
void MskDiagnosticBagReportUndefinedBinaryOperator(
    MskDiagnosticBag* diagnostics,
    MskTextSpan span,
    StringView operator_name,
    MskRuntimeObjectKind left_type,
    MskRuntimeObjectKind right_type);

#endif  // MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_
