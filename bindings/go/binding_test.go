package tree_sitter_vtt_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_vtt "github.com/tree-sitter/tree-sitter-vtt/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_vtt.Language())
	if language == nil {
		t.Errorf("Error loading VTT grammar")
	}
}
