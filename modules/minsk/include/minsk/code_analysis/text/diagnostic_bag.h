#ifndef MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_
#define MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_

#include <phyto/string/string.h>
#include <stdint.h>
#include <vec/vec.h>

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/text/diagnostic.h"
#include "minsk/runtime/object.h"

typedef VEC_TYPE(MskDiagnostic) MskDiagnosticBag;

void MskDiagnosticBagFree(MskDiagnosticBag* diagnostics);
void MskDiagnosticBagReportInvalidNumber(MskDiagnosticBag* diagnostics,
                                         MskTextSpan span,
                                         phyto_string_span_t text,
                                         phyto_string_span_t type);
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
    phyto_string_span_t operator_name,
    MskRuntimeObjectKind operand_type);
void MskDiagnosticBagReportUndefinedBinaryOperator(
    MskDiagnosticBag* diagnostics,
    MskTextSpan span,
    phyto_string_span_t operator_name,
    MskRuntimeObjectKind left_type,
    MskRuntimeObjectKind right_type);
void MskDiagnosticBagReportUndefinedName(MskDiagnosticBag* diagnostics,
                                         MskTextSpan span,
                                         phyto_string_span_t name);

#endif  // MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_
