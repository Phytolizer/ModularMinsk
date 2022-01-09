#ifndef MINSK_TESTS_PARSER_H_
#define MINSK_TESTS_PARSER_H_

#include <minsk/code_analysis/syntax/node.h>
#include <phyto/collections/deque.h>
#include <test/test.h>

PHYTO_COLLECTIONS_DEQUE_DECL(FlatSyntaxTree, MskSyntaxNode*);
TEST_SUITE_FUNC(ParserTests);

#endif  // MINSK_TESTS_PARSER_H_
