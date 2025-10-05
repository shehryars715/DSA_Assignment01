#include "uno.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <deque>

using namespace std;

struct Card {
    std::string color;
    std::string value; // "0"-"9", "Skip", "Reverse", "Draw Two"
};

static const vector<string> COLORS = {"Red", "Green", "Blue", "Yellow"};
static const vector<string> VALUES = {"0","1","2","3","4","5","6","7","8","9"};
static const vector<string> ACTIONS = {"Skip", "Reverse", "Draw Two"};

class UNOGameImpl {
public:
    int numPlayers;
    vector<vector<Card>> hands;
    deque<Card> deck;
    vector<Card> discard;
    int currentPlayer = 0;
    bool clockwise = true;
    bool gameOver = false;
    int winner = -1;

    UNOGameImpl(int n): numPlayers(n), hands(n) {}

    // Builds 76-card deck (no wilds)
    void buildDeck() {
        deck.clear();
        for (const auto& color : COLORS) {
            // one 0 card
            deck.push_back({color, "0"});
            // two of 1-9 and two of each action
            for (int i = 1; i <= 9; ++i) {
                deck.push_back({color, VALUES[i]});
                deck.push_back({color, VALUES[i]});
            }
            for (const auto& act : ACTIONS) {
                deck.push_back({color, act});
                deck.push_back({color, act});
            }
        }
    }

    void shuffleDeck() {
        std::mt19937 rng(1234); // fixed seed
        std::shuffle(deck.begin(), deck.end(), rng);
    }

    void deal() {
        for (int i = 0; i < 7; ++i)
            for (int p = 0; p < numPlayers; ++p) {
                if (!deck.empty()) {
                    hands[p].push_back(deck.front());
                    deck.pop_front();
                }
            }
        discard.push_back(deck.front());
        deck.pop_front();
    }

    bool matches(const Card& c, const Card& top) const {
        return (c.color == top.color || c.value == top.value);
    }

    void nextPlayer(int skip = 0) {
        int step = clockwise ? 1 : -1;
        for (int i = 0; i <= skip; ++i) {
            currentPlayer = (currentPlayer + step + numPlayers) % numPlayers;
        }
    }

    void handleAction(const string& value) {
        if (value == "Skip") nextPlayer(); // skip one extra
        else if (value == "Reverse") clockwise = !clockwise;
        else if (value == "Draw Two") {
            nextPlayer();
            int next = currentPlayer;
            for (int i = 0; i < 2 && !deck.empty(); ++i) {
                hands[next].push_back(deck.front());
                deck.pop_front();
            }
            nextPlayer(); // skip after drawing
        }
    }

    Card* findPlayableCard(vector<Card>& hand, const Card& top) {
        // 1. color match
        for (auto& c : hand)
            if (c.color == top.color) return &c;
        // 2. value match
        for (auto& c : hand)
            if (c.value == top.value) return &c;
        // 3. action priority order: Skip, Reverse, Draw Two
        for (string act : ACTIONS) {
            for (auto& c : hand)
                if (c.value == act && matches(c, top))
                    return &c;
        }
        return nullptr;
    }

    void playTurn() {
        if (gameOver) return;

        vector<Card>& hand = hands[currentPlayer];
        Card top = discard.back();

        Card* play = findPlayableCard(hand, top);
        if (!play) {
            // draw one card
            if (!deck.empty()) {
                Card drawn = deck.front(); deck.pop_front();
                if (matches(drawn, top)) {
                    discard.push_back(drawn);
                    handleAction(drawn.value);
                } else {
                    hand.push_back(drawn);
                    nextPlayer();
                }
            } else {
                nextPlayer(); // no cards to draw → skip
            }
        } else {
            discard.push_back(*play);
            string action = play->value;
            // remove card
            hand.erase(remove_if(hand.begin(), hand.end(),
                                 [&](const Card& c) {
                                     return c.color == play->color && c.value == play->value;
                                 }),
                       hand.end());

            if (hand.empty()) {
                gameOver = true;
                winner = currentPlayer;
                return;
            }

            if (hand.size() == 1) {
                cout << "UNO! Player " << currentPlayer << " has one card left!\n";
            }

            handleAction(action);
        }
    }

    string getState() const {
        ostringstream out;
        out << "Player " << currentPlayer << "'s turn, Direction: "
            << (clockwise ? "Clockwise" : "Counter-clockwise")
            << ", Top: " << discard.back().color << " " << discard.back().value
            << ", Players cards: ";
        for (int i = 0; i < numPlayers; ++i) {
            out << "P" << i << ":" << hands[i].size();
            if (i < numPlayers - 1) out << ", ";
        }
        return out.str();
    }

    bool isGameOver() const { return gameOver; }
    int getWinner() const { return winner; }
};

// Global single instance (since header has no members)
static UNOGameImpl* impl = nullptr;

// --- UNOGame public methods ---
UNOGame::UNOGame(int numPlayers) {
    impl = new UNOGameImpl(numPlayers);
}

void UNOGame::initialize() {
    impl->buildDeck();
    impl->shuffleDeck();
    impl->deal();
}

void UNOGame::playTurn() { impl->playTurn(); }

bool UNOGame::isGameOver() const { return impl->isGameOver(); }

int UNOGame::getWinner() const { return impl->getWinner(); }

std::string UNOGame::getState() const { return impl->getState(); }
