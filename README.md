Github Repo: https://github.com/shehryars715/DSA_Assignment01/

# Data Structures Project

This project contains three different implementations: a Polynomial ADT, a Text Editor simulation, and a UNO game. Here's what each one does and how I built them.


## Polynomial ADT (`polynomial.cpp`)

### What it does

This is basically a way to represent and work with polynomials like **3x² + 2x - 5**. You can add terms, combine polynomials, multiply them, and even find derivatives.

### The tricky parts

Building this wasn't super straightforward. The main challenges were:

1. Making sure terms with the same exponent get combined automatically (like 3x² + 2x² should become 5x²)
2. Keeping everything sorted by exponent so it looks right when you print it
3. Getting rid of terms that end up with zero coefficients
4. Making sure operations create new polynomials instead of changing the original ones
5. Formatting the output to look like actual math (handling plus/minus signs, not showing the "1" in "1x", etc.)

### How I solved it

I used a `std::map` with exponents as keys and coefficients as values. The map keeps things sorted automatically, which is nice. When you insert a term, if that exponent already exists, it just adds to the coefficient. If the coefficient becomes zero after adding, I remove that term entirely.

For addition, I just go through both polynomials and combine all the terms. Multiplication is a bit more work - you multiply each term in the first polynomial by each term in the second, then combine like terms.

The derivative was actually the easiest part - just multiply each coefficient by its exponent and reduce the exponent by 1.

---

## Text Editor Simulation (`texteditor.cpp`)

### What it does

This simulates a basic text editor cursor, kind of like what you see in Notepad. You can insert characters, delete them (like hitting backspace), move the cursor around, and see where the cursor is with a `|` symbol.

### The tricky parts

The main issues I ran into:

1. Making insertions and deletions fast, especially since the cursor can be anywhere
2. Making sure the cursor doesn't go past the beginning or end of the text
3. Showing where the cursor is visually
4. Working with the header file I was given without changing it

### How I solved it

I used a doubly linked list to store the characters. This makes inserting and deleting at the cursor really fast (O(1) time). The cursor is just an iterator pointing to a position in the list.

When you type, the character goes in before the cursor. When you delete, it removes the character before the cursor (like backspace). Moving left and right is just moving the iterator, with checks to make sure you don't go out of bounds.

To display the text, I walk through the list and insert a `|` where the cursor is.

Example: if you have "abc" and the cursor is after 'b', it shows as `ab|c`

---

## UNO Game Simulation (`uno.cpp`)

### What it does

This is a simplified version of UNO for 2-4 players. It has the four colors (Red, Green, Blue, Yellow), number cards 0-9, and action cards (Skip, Reverse, Draw Two). No wild cards to keep things simpler.

### The tricky parts

This one had a lot of moving parts:

1. Figuring out how to represent cards, the deck, and each player's hand
2. Implementing the action cards correctly (especially Reverse)
3. Keeping track of whose turn it is and which direction we're going
4. Checking if a card can be played (matching color or value)
5. Making the game reproducible with a fixed random seed
6. Formatting the game state output correctly

### How I solved it

Each card is just a simple struct with a color and value. The deck is a deque (so I can draw from it efficiently), and each player has a vector of cards.

On each turn, the player looks for a card they can play. They check for color matches first, then value matches, then action cards in a specific order (Skip, then Reverse, then Draw Two). If they can't play anything, they draw a card and try to play it if it matches.

Action cards do their thing immediately when played - Skip skips the next player, Reverse changes the direction, and Draw Two makes the next player draw cards.

The game keeps going until someone runs out of cards (they win) or the deck runs out and nobody can play (stalemate).

The state output looks like this:
```
Player 0's turn, Direction: Clockwise, Top: Blue 3, Players cards: P0:7, P1:7
```

---

## Data Structures Used

Here's a quick summary of what I used for each part:

- **Polynomial**: `std::map<int, int>` - automatically keeps exponents sorted and makes combining terms easy
- **Text Editor**: `std::list<char>` with an iterator - makes cursor operations fast
- **UNO**: `std::deque` for the deck and `std::vector` for player hands - efficient for drawing cards and managing hands






