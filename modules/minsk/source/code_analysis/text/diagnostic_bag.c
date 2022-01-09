#include "minsk/code_analysis/text/diagnostic_bag.h"

#include <phyto/string/string.h>
#include <stdint.h>

#include "minsk/code_analysis/syntax/kind.h"

static void Report(MskDiagnosticBag* diagnostics,
                   MskTextSpan span,
                   phyto_string_t message);

void MskDiagnosticBagFree(MskDiagnosticBag* diagnostics) {
  for (uint64_t i = 0; i < VEC_SIZE(diagnostics); ++i) {
    MskDiagnosticFree(&diagnostics->data[i]);
  }
  VEC_FREE(diagnostics);
}

void MskDiagnosticBagReportInvalidNumber(MskDiagnosticBag* diagnostics,
                                         MskTextSpan span,
                                         phyto_string_span_t text,
                                         phyto_string_span_t type) {
  phyto_string_t message = phyto_string_from_sprintf(
      "The number %" PHYTO_STRING_FORMAT " isn't a valid %" PHYTO_STRING_FORMAT
      ".",
      PHYTO_STRING_VIEW_PRINTF_ARGS(text), PHYTO_STRING_VIEW_PRINTF_ARGS(type));
  Report(diagnostics, span, message);
}

void MskDiagnosticBagReportBadCharacter(MskDiagnosticBag* diagnostics,
                                        uint64_t position,
                                        char ch) {
  phyto_string_t message =
      phyto_string_from_sprintf("The character '%c' isn't valid here.", ch);
  MskTextSpan span = MskTextSpanNew(position, position + 1);
  Report(diagnostics, span, message);
}

void MskDiagnosticBagReportUnexpectedToken(MskDiagnosticBag* diagnostics,
                                           MskTextSpan span,
                                           MskSyntaxKind expected,
                                           MskSyntaxKind actual) {
  phyto_string_span_t expected_name = MskSyntaxKindName(expected);
  phyto_string_span_t actual_name = MskSyntaxKindName(actual);
  phyto_string_t message = phyto_string_from_sprintf(
      "Expected %" PHYTO_STRING_FORMAT " but got %" PHYTO_STRING_FORMAT ".",
      PHYTO_STRING_VIEW_PRINTF_ARGS(expected_name),
      PHYTO_STRING_VIEW_PRINTF_ARGS(actual_name));
  Report(diagnostics, span, message);
}

void MskDiagnosticBagReportUndefinedUnaryOperator(
    MskDiagnosticBag* diagnostics,
    MskTextSpan span,
    phyto_string_span_t operator_name,
    MskRuntimeObjectKind operand_type) {
  phyto_string_span_t operand_type_name =
      MskRuntimeObjectKindName(operand_type);
  phyto_string_t message = phyto_string_from_sprintf(
      "The operator '%" PHYTO_STRING_FORMAT
      "' isn't defined for %" PHYTO_STRING_FORMAT ".",
      PHYTO_STRING_VIEW_PRINTF_ARGS(operator_name),
      PHYTO_STRING_VIEW_PRINTF_ARGS(operand_type_name));
  Report(diagnostics, span, message);
}

void MskDiagnosticBagReportUndefinedBinaryOperator(
    MskDiagnosticBag* diagnostics,
    MskTextSpan span,
    phyto_string_span_t operator_name,
    MskRuntimeObjectKind left_type,
    MskRuntimeObjectKind right_type) {
  phyto_string_span_t left_type_name = MskRuntimeObjectKindName(left_type);
  phyto_string_span_t right_type_name = MskRuntimeObjectKindName(right_type);
  phyto_string_t message =
      phyto_string_from_sprintf("The operator '%" PHYTO_STRING_FORMAT
                                "' isn't defined for %" PHYTO_STRING_FORMAT
                                " and %" PHYTO_STRING_FORMAT ".",
                                PHYTO_STRING_VIEW_PRINTF_ARGS(operator_name),
                                PHYTO_STRING_VIEW_PRINTF_ARGS(left_type_name),
                                PHYTO_STRING_VIEW_PRINTF_ARGS(right_type_name));
  Report(diagnostics, span, message);
}

void MskDiagnosticBagReportUndefinedName(MskDiagnosticBag* diagnostics,
                                         MskTextSpan span,
                                         phyto_string_span_t name) {
  phyto_string_t message = phyto_string_from_sprintf(
      "The name '%" PHYTO_STRING_FORMAT "' isn't defined.",
      PHYTO_STRING_VIEW_PRINTF_ARGS(name));
  Report(diagnostics, span, message);
}

void Report(MskDiagnosticBag* diagnostics,
            MskTextSpan span,
            phyto_string_t message) {
  MskDiagnostic diagnostic = {
      .span = span,
      .message = message,
  };
  VEC_PUSH(diagnostics, diagnostic);
}
