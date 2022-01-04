cmake_minimum_required(VERSION 3.12...3.23)

project(
  ModularMinsk
  LANGUAGES C
  VERSION 0.1.0
  DESCRIPTION "Minsk in C with a modular design"
  HOMEPAGE_URL "https://github.com/Phytolizer/ModularMinsk"
)

function(declare_module NAME)
  cmake_parse_arguments(
    PARSE_ARGV 0 "DM" "INTERNAL_INCLUDE" "KIND;TARGET_NAME;OUTPUT_NAME"
    "SOURCES;ABSOLUTE_SOURCES;DEPENDS;INCLUDES"
  )
  if(NOT DM_KIND)
    message(FATAL_ERROR "Missing module kind")
  endif()
  if(DM_KIND STREQUAL "library")
    set(DM_PUBLICITY PUBLIC)
  elseif(DM_KIND STREQUAL "executable")
    set(DM_PUBLICITY PRIVATE)
  elseif(DM_KIND STREQUAL "interface")
    set(DM_KIND "library")
    set(DM_INTERFACEKW INTERFACE)
    set(DM_PUBLICITY INTERFACE)
  else()
    message(FATAL_ERROR "Unknown module kind")
  endif()
  if(DM_TARGET_NAME)
    set(DM_TARGET_NAME ${DM_TARGET_NAME})
  else()
    set(DM_TARGET_NAME ${NAME})
  endif()
  list(TRANSFORM DM_SOURCES PREPEND "modules/${NAME}/source/")
  cmake_language(
    CALL "add_${DM_KIND}" ${DM_TARGET_NAME} ${DM_INTERFACEKW} ${DM_SOURCES}
    ${DM_ABSOLUTE_SOURCES}
  )
  target_link_libraries(${DM_TARGET_NAME} ${DM_PUBLICITY} ${DM_DEPENDS})
  target_include_directories(
    ${DM_TARGET_NAME} ${DM_PUBLICITY} ${DM_INCLUDES} "modules/${NAME}/include"
  )
  if(DM_INTERNAL_INCLUDE)
    target_include_directories(
      ${DM_TARGET_NAME} PRIVATE "modules/${NAME}/internal_include"
    )
  endif()
  if(DM_OUTPUT_NAME)
    set_target_properties(
      ${DM_TARGET_NAME} PROPERTIES OUTPUT_NAME ${DM_OUTPUT_NAME}
    )
  endif()
endfunction()

function(embed_file NAME)
  cmake_parse_arguments(PARSE_ARGV 0 "EF" "" "PART_OF;SYMBOL_NAME" "")
  if(NOT EF_PART_OF)
    message(
      FATAL_ERROR "Missing PART_OF flag; must be set to the name of the module"
    )
  endif()
  if(EF_SYMBOL_NAME)
    set(EF_SYMBOL_FLAG "-s" "${EF_SYMBOL_NAME}")
  endif()
  set(EF_OUTPUT_DIR "${PROJECT_BINARY_DIR}/embedded/${EF_PART_OF}")
  set(EF_INPUT_DIR "${PROJECT_SOURCE_DIR}/modules/${EF_PART_OF}/embedded")
  add_custom_command(
    OUTPUT "${EF_OUTPUT_DIR}/${NAME}.c" "${EF_OUTPUT_DIR}/${NAME}.h"
    DEPENDS "embed" "${EF_INPUT_DIR}/${NAME}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${EF_OUTPUT_DIR}"
    COMMAND "$<TARGET_FILE:embed>" "${EF_INPUT_DIR}/${NAME}" ${EF_SYMBOL_FLAG}
            -o "${EF_OUTPUT_DIR}/${NAME}.c"
  )
  target_sources(${EF_PART_OF} PUBLIC "${EF_OUTPUT_DIR}/${NAME}.c")
endfunction()

# Dependencies for the embed program
declare_module(
  argparse
  KIND library
  SOURCES argparse.c
)
declare_module(
  nonstd
  KIND library
  SOURCES asprintf.c strdup.c strpbrk.c strspn.c strtok.c
)

# Embeds a file as C code
declare_module(
  embed
  KIND executable
  SOURCES main.c
  DEPENDS argparse nonstd
)

# Test framework
declare_module(
  test
  TARGET_NAME testlib
  KIND interface
  DEPENDS nonstd
)

# Useful libraries
declare_module(
  vec
  KIND library
  SOURCES vec.c
)
declare_module(
  span
  KIND library
  SOURCES span.c
)
declare_module(
  string
  KIND library
  SOURCES string.c
  DEPENDS span vec
)
declare_module(
  hash
  KIND library
  SOURCES hash.c
  DEPENDS span vec
)
declare_module(annotate KIND interface)
declare_module(ansi_esc KIND interface)

# Following is actual Minsk-specific stuff.

# The core language as a library
declare_module(
  minsk
  KIND library
  SOURCES code_analysis/binding/assignment_expression.c
          code_analysis/binding/binary_expression.c
          code_analysis/binding/binary_operator.c
          code_analysis/binding/binder.c
          code_analysis/binding/expression.c
          code_analysis/binding/literal_expression.c
          code_analysis/binding/node.c
          code_analysis/binding/unary_expression.c
          code_analysis/binding/unary_operator.c
          code_analysis/binding/variable_expression.c
          code_analysis/compilation.c
          code_analysis/evaluator.c
          code_analysis/symbol_table.c
          code_analysis/syntax/assignment_expression.c
          code_analysis/syntax/binary_expression.c
          code_analysis/syntax/expression.c
          code_analysis/syntax/facts.c
          code_analysis/syntax/kind.c
          code_analysis/syntax/lexer.c
          code_analysis/syntax/literal_expression.c
          code_analysis/syntax/name_expression.c
          code_analysis/syntax/node.c
          code_analysis/syntax/parenthesized_expression.c
          code_analysis/syntax/parser.c
          code_analysis/syntax/token.c
          code_analysis/syntax/tree.c
          code_analysis/syntax/unary_expression.c
          code_analysis/text/diagnostic_bag.c
          code_analysis/text/diagnostic.c
          code_analysis/text/span.c
          code_analysis/variable_symbol.c
          runtime/object.c
  DEPENDS vec span string hash annotate ansi_esc
  INTERNAL_INCLUDE
)

# The main program
declare_module(
  mc
  KIND executable
  SOURCES main.c
  DEPENDS minsk ansi_esc
)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  # Unit tests
  declare_module(
    minsk_test
    KIND executable
    SOURCES main.c code_analysis/syntax/lexer_tests.c
            code_analysis/syntax/facts_tests.c
            code_analysis/syntax/parser_tests.c
    DEPENDS minsk testlib
  )
  enable_testing()
  add_test(NAME minsk_test COMMAND minsk_test)
endif()
