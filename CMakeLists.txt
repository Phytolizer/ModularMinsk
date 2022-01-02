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
    PARSE_ARGV 0 "DM" "" "KIND;OUTPUT_NAME"
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
  list(TRANSFORM DM_SOURCES PREPEND "modules/${NAME}/source/")
  cmake_language(
    CALL "add_${DM_KIND}" ${NAME} ${DM_INTERFACEKW} ${DM_SOURCES}
    ${DM_ABSOLUTE_SOURCES}
  )
  target_link_libraries(${NAME} ${DM_PUBLICITY} ${DM_DEPENDS})
  target_include_directories(
    ${NAME} ${DM_PUBLICITY} ${DM_INCLUDES} "modules/${NAME}/include"
  )
  if(DM_OUTPUT_NAME)
    set_target_properties(${NAME} PROPERTIES OUTPUT_NAME ${DM_OUTPUT_NAME})
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
declare_module(test KIND interface)

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

# Following is actual Minsk-specific stuff.

# The core language as a library
declare_module(
  minsk
  KIND library
  SOURCES code_analysis/syntax/kind.c code_analysis/syntax/lexer.c
          code_analysis/syntax/token.c runtime/object/object.c
  DEPENDS vec span string hash annotate
)

# The main program
declare_module(
  mc
  KIND executable
  SOURCES main.c
  DEPENDS minsk
)
