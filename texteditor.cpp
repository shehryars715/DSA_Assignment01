#include "texteditor.h"
#include <list>
#include <string>

class TextEditorImpl {
public:
    std::list<char> text;                // stores characters
    std::list<char>::iterator cursor;    // points to current cursor position

    TextEditorImpl() {
        cursor = text.begin(); // start with empty text, cursor at position 0
    }
};

// We’ll use a single hidden instance (since header can't store members)
static TextEditorImpl editor;

void TextEditor::insertChar(char c) {
    editor.text.insert(editor.cursor, c);
}

void TextEditor::deleteChar() {
    if (editor.cursor == editor.text.begin()) return; // nothing to delete
    auto toErase = editor.cursor;
    --toErase;
    editor.text.erase(toErase);
}

void TextEditor::moveLeft() {
    if (editor.cursor != editor.text.begin())
        --editor.cursor;
}

void TextEditor::moveRight() {
    if (editor.cursor != editor.text.end())
        ++editor.cursor;
}

std::string TextEditor::getTextWithCursor() const {
    std::string result;
    for (auto it = editor.text.begin(); it != editor.text.end(); ++it) {
        if (it == editor.cursor)
            result += '|';
        result += *it;
    }
    if (editor.cursor == editor.text.end())
        result += '|';
    return result;
}
