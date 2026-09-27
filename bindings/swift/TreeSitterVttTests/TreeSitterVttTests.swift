import XCTest
import SwiftTreeSitter
import TreeSitterVtt

final class TreeSitterVttTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_vtt())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading VTT grammar")
    }
}
