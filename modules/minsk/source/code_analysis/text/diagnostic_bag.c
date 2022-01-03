#include "minsk/code_analysis/text/diagnostic_bag.h"

#include <stdint.h>
#include <string/string.h>

#include "minsk/code_analysis/syntax/kind.h"

static void Report(MskDiagnosticBag* diagnostics,
                   MskTextSpan span,
                   String message);

void MskDiagnosticBagFree(MskDiagnosticBag* diagnostics) {
  for (uint64_t i = 0; i < diagnostics->size; ++i) {
    MskDiagnosticFree(&diagnostics->data[i]);
  }
  VEC_FREE(diagnostics);
}

void MskDiagnosticBagReportInvalidNumber(MskDiagnosticBag* diagnostics,
                                         MskTextSpan span,
                                         StringView text,
                                         StringView type) {
  String message = StringFormat(
      "The number %" STRING_VIEW_FMT " isn't a valid %" STRING_VIEW_FMT ".",
      STRING_VIEW_PRINT(text), STRING_VIEW_PRINT(type));
  Report(diagnostics, span, message);
}

void MskDiagnosticBagReportBadCharacter(MskDiagnosticBag* diagnostics,
                                        uint64_t position,
                                        char ch) {
  String message = StringFormat("The character '%c' isn't valid here.", ch);
  MskTextSpan span = MskTextSpanNew(position, position + 1);
  Report(diagnostics, span, message);
}

void MskDiagnosticBagReportUnexpectedToken(MskDiagnosticBag* diagnostics,
                                           MskTextSpan span,
                                           MskSyntaxKind expected,
                                           MskSyntaxKind actual) {
  StringView expected_name = MskSyntaxKindName(expected);
  StringView actual_name = MskSyntaxKindName(actual);
  String message = StringFormat(
      "Expected %" STRING_VIEW_FMT " but got %" STRING_VIEW_FMT ".",
      STRING_VIEW_PRINT(expected_name), STRING_VIEW_PRINT(actual_name));
  Report(diagnostics, span, message);
}

void MskDiagnosticBagReportUndefinedUnaryOperator(
    MskDiagnosticBag* diagnostics,
    MskTextSpan span,
    StringView operator_name,
    MskRuntimeObjectKind operand_type) {
  StringView operand_type_name = MskRuntimeObjectKindName(operand_type);
  String message = StringFormat("The operator '%" STRING_VIEW_FMT
                                "' isn't defined for %" STRING_VIEW_FMT ".",
                                STRING_VIEW_PRINT(operator_name),
                                STRING_VIEW_PRINT(operand_type_name));
  Report(diagnostics, span, message);
}

void MskDiagnosticBagReportUndefinedBinaryOperator(
    MskDiagnosticBag* diagnostics,
    MskTextSpan span,
    StringView operator_name,
    MskRuntimeObjectKind left_type,
    MskRuntimeObjectKind right_type) {
  StringView left_type_name = MskRuntimeObjectKindName(left_type);
  StringView right_type_name = MskRuntimeObjectKindName(right_type);
  String message = StringFormat(
      "The operator '%" STRING_VIEW_FMT "' isn't defined for %" STRING_VIEW_FMT
      " and %" STRING_VIEW_FMT ".",
      STRING_VIEW_PRINT(operator_name), STRING_VIEW_PRINT(left_type_name),
      STRING_VIEW_PRINT(right_type_name));
  Report(diagnostics, span, message);
}

void Report(MskDiagnosticBag* diagnostics, MskTextSpan span, String message) {
  MskDiagnostic diagnostic = {
      .span = span,
      .message = message,
  };
  VEC_PUSH(diagnostics, diagnostic);
}
