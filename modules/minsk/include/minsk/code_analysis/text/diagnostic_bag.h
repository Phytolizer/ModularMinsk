#ifndef MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_
#define MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_

#include <phyto/collections/dynamic_array.h>
#include <phyto/string/string.h>
#include <stdint.h>

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/text/diagnostic.h"
#include "minsk/runtime/object.h"

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(MskDiagnosticBag, MskDiagnostic);

extern const MskDiagnosticBag_callbacks_t kMskDiagnosticBagCallbacks;

void MskDiagnosticBagReportInvalidNumber(MskDiagnosticBag_t* diagnostics,
                                         MskTextSpan span,
                                         phyto_string_span_t text,
                                         phyto_string_span_t type);
void MskDiagnosticBagReportBadCharacter(MskDiagnosticBag_t* diagnostics,
                                        uint64_t position,
                                        char ch);
void MskDiagnosticBagReportUnexpectedToken(MskDiagnosticBag_t* diagnostics,
                                           MskTextSpan span,
                                           MskSyntaxKind expected,
                                           MskSyntaxKind actual);
void MskDiagnosticBagReportUndefinedUnaryOperator(
    MskDiagnosticBag_t* diagnostics,
    MskTextSpan span,
    phyto_string_span_t operator_name,
    MskRuntimeObjectKind operand_type);
void MskDiagnosticBagReportUndefinedBinaryOperator(
    MskDiagnosticBag_t* diagnostics,
    MskTextSpan span,
    phyto_string_span_t operator_name,
    MskRuntimeObjectKind left_type,
    MskRuntimeObjectKind right_type);
void MskDiagnosticBagReportUndefinedName(MskDiagnosticBag_t* diagnostics,
                                         MskTextSpan span,
                                         phyto_string_span_t name);

#endif  // MINSK_CODE_ANALYSIS_DIAGNOSTIC_H_
