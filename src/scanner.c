#include "tree_sitter/alloc.h"
#include "tree_sitter/array.h"
#include "tree_sitter/parser.h"
#include <stdint.h>

#define IS_NEWLINE(x) (x == 0x0A || x == 0x0D)
#define IS_NEWLINE_PAIR(a, b) (a == 0x0D && b == 0x0A)

enum TokenType {
	CUE_PAYLOAD,
	// NEWLINE,
};

typedef struct {
	uint8_t consecutive_newlines;
} Scanner;

static uint16_t advance_to_next_newline(TSLexer *lexer) {
	uint16_t count = 0;
	while (!IS_NEWLINE(lexer->lookahead)) {
		lexer->advance(lexer, false);
		++count;
		if (lexer->eof(lexer)) {
			break;
		}
	}
	return count;
}

void *tree_sitter_vtt_external_scanner_create() {
	Scanner *scanner = ts_malloc(sizeof(Scanner));
	scanner->consecutive_newlines = 0;
	return scanner;
}

void tree_sitter_vtt_external_scanner_destroy(void *payload) {
	Scanner *scanner = (Scanner *)payload;
	ts_free(scanner);
}

unsigned tree_sitter_vtt_external_scanner_serialize(void *payload,
													char *buffer) {
	Scanner *scanner = (Scanner *)payload;
	buffer[0] = scanner->consecutive_newlines;

	return 1;
}

void tree_sitter_vtt_external_scanner_deserialize(void *payload,
												  const char *buffer,
												  unsigned length) {
	Scanner *scanner = (Scanner *)payload;
	if (length > 0) {
		scanner->consecutive_newlines = buffer[0];
	}
}

// This feels really really really fragile. Like it's going to break the second
// input is malformed. Be careful. I need to do better but this works for now.
bool tree_sitter_vtt_external_scanner_scan(void *payload, TSLexer *lexer,
										   const bool *valid_symbols) {
	Scanner *s = (Scanner *)payload;

	// TODO: make it account for Windows newlines. Sorry.

	if (valid_symbols[CUE_PAYLOAD]) {
		lexer->result_symbol = CUE_PAYLOAD;
		while (true) {
			uint16_t line_length = advance_to_next_newline(lexer);
			lexer->advance(lexer, false);
			if (IS_NEWLINE(lexer->lookahead)) {
				lexer->mark_end(lexer);
				return true;
			} else if (lexer->eof(lexer)) {
				lexer->mark_end(lexer);
				return true;
			}
		}
		// s->consecutive_newlines++;
		// if (line_length <= 5) {
		// }
	}
	return false;
}
