/**
 * @file Tree-sitter parser for vtt files
 * @author IntangibleMatter
 * @license MIT
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

export default grammar({
  name: "vtt",

  externals: ($) => [$.cue_payload],

  rules: {
    // TODO: add the actual grammar rules
    source_file: ($) =>
      seq(
        $.file_start,
        $._line_terminator,
        repeat(
          choice(
            $.cue,
            $.comment,
            $._line_terminator,
            seq($.setting, $._line_terminator),
          ),
        ),
      ),

    file_start: ($) => "WEBVTT",

    setting: ($) => seq(/\w+/, ":", /\w+/),

    _line_terminator: ($) => /\x0D\x0A|\x0D|\x0A/,

    cue: ($) => seq($.cue_header, $._line_terminator, $.cue_payload),

    cue_header: ($) => choice($.cue_timing),

    cue_timing: ($) => seq($.timestamp, /\s+-->\s+/, $.timestamp),

    timestamp: ($) => /(\d\d+:)?[0-5]\d:[0-5]\d\.\d\d\d/,

    comment: ($) => /NOTE.*?(\x0D\x0A|\x0D|\x0A)/,
  },
});
