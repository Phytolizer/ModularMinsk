#include <argparse.h>
#include <ctype.h>
#include <errno.h>
#include <nonstd/strdup.h>
#include <nonstd/strtok.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* const kUsage[] = {
    "embed <FILE> [options]",
    NULL,
};

int main(int argc, const char** argv) {
  char* output_path = NULL;
  char* symbol_name = NULL;
  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_GROUP("Basic options"),
      OPT_STRING('o', "output", &output_path,
                 "path to write (default FILE_embed.[ch])"),
      OPT_STRING('s', "symbol-name", &symbol_name, "name of embedded symbol"),
      OPT_END(),
  };
  struct argparse argp;
  argparse_init(&argp, options, kUsage, 0);
  argparse_describe(&argp, "Embeds a text file into C code", "");
  argc = argparse_parse(&argp, argc, argv);

  if (argc < 1) {
    fprintf(stderr, "required argument not passed: <FILE>\n");
    argparse_usage(&argp);
    return 1;
  }

  FILE* inp = fopen(argv[0], "r");
  if (inp == NULL) {
    fprintf(stderr, "could not open %s for reading: %s\n", argv[0],
            strerror(errno));
    return 1;
  }
  fseek(inp, 0, SEEK_END);
  int inlen = ftell(inp);
  fseek(inp, 0, SEEK_SET);
  char* input = malloc(inlen + 1);
  fread(input, 1, inlen, inp);
  fclose(inp);
  input[inlen] = '\0';

  bool outpath_malloced = false;
  if (output_path == NULL) {
    output_path = malloc(strlen(argv[0]) + sizeof("_embed.c"));
    outpath_malloced = true;
    strcpy(output_path, argv[0]);
    strcat(output_path, "_embed.c");
  }

  char* dotc = &output_path[strlen(output_path) - 2];
  if (strcmp(dotc, ".c") != 0) {
    fprintf(stderr, "output filename should end with \".c\"\n");
    return 1;
  }

  dotc[1] = 'h';
  FILE* outp = fopen(output_path, "w");
  if (outp == NULL) {
    fprintf(stderr, "Can't write output header %s: %s\n", output_path,
            strerror(errno));
    return 1;
  }

  size_t output_path_len = strlen(output_path);
  char* header_guard = malloc(output_path_len + 2);
  for (size_t i = 0; i < output_path_len; ++i) {
    if (!isalnum(output_path[i])) {
      header_guard[i] = '_';
    } else {
      header_guard[i] = toupper(output_path[i]);
    }
  }
  header_guard[output_path_len] = '\0';
  strcat(header_guard, "_");

  fprintf(outp, "#ifndef %s\n", header_guard);
  fprintf(outp, "#define %s\n", header_guard);
  fputs("\n", outp);
  fputs("#include <stddef.h>\n", outp);
  fputs("\n", outp);
  fprintf(outp, "extern const char %s[];\n", symbol_name);
  fprintf(outp, "extern const size_t %s_size;\n", symbol_name);
  fputs("\n", outp);
  fprintf(outp, "#endif /* %s */\n", header_guard);
  fclose(outp);

  dotc[1] = 'c';
  outp = fopen(output_path, "w");
  dotc[1] = 'h';
  if (outp == NULL) {
    fprintf(stderr, "could not open %s for writing: %s\n", output_path,
            strerror(errno));
    return 1;
  }
  fprintf(outp, "#include \"%s\"\n\n", output_path);
  bool symbol_name_malloced = false;
  if (symbol_name == NULL) {
    symbol_name = NonstdStringDuplicate(argv[0]);
    symbol_name_malloced = true;
    for (char* c = symbol_name; *c != '\0'; ++c) {
      if (!isalnum(*c)) {
        *c = '_';
      }
    }
  }
  fprintf(outp, "const char %s[] =\n", symbol_name);
  char* saveptr;
  for (char* line = NonstdStringTokenizeReentrant(input, "\n", &saveptr);
       line != NULL;
       line = NonstdStringTokenizeReentrant(NULL, "\n", &saveptr)) {
    fputs("\"", outp);
    for (char* c = line; *c != '\0'; ++c) {
      if (*c == '"') {
        fputs("\\\"", outp);
      } else if (*c == '\\') {
        fputs("\\\\", outp);
      } else {
        fputc(*c, outp);
      }
    }
    fputs("\\n\"\n", outp);
  }
  fprintf(outp, ";\n");
  fprintf(outp, "const size_t %s_size = %d;\n", symbol_name, inlen);
  fclose(outp);

  if (outpath_malloced) {
    free(output_path);
  }
  if (symbol_name_malloced) {
    free(symbol_name);
  }
}