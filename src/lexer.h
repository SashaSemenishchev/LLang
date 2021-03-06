#pragma once

#include <iostream>
#include <vector>
#include <string>

class Token {
 public:
  enum class Kind {
    Number,
    String,
    Identifier,
    LeftParen,
    RightParen,
    LeftSquare,
    RightSquare,
    LeftCurly,
    RightCurly,
    LessThan,
    LessOrEquals,
    GreaterThan,
    GreaterOrEquals,
    Equal,
    Plus,
    Minus,
    Asterisk,
    Slash,
    Hash,
    Dot,
    Comma,
    Colon,
    Semicolon,
    SingleQuote,
    DoubleQuote,
    Comment,
    BinaryOperator,
    And,
    Or,
    Equals,
    Unary,
    End,
    Unexpected,

  };

  Token() : m_kind{Kind::Unexpected} {}

  Token(Kind kind) noexcept : m_kind{kind} {}

  static std::string kind_to_string(Kind kind) {
      static const char* const names[]{
              "Number", "String",      "Identifier",  "LeftParen",  "RightParen", "LeftSquare",
              "RightSquare", "LeftCurly",   "RightCurly", "LessThan", "LessOrEquals",
              "GreaterThan", "GreaterOrEquals",
              "Equal",       "Plus",        "Minus",      "Asterisk",   "Slash",
              "Hash",        "Dot",         "Comma",      "Colon",      "Semicolon",
              "SingleQuote", "DoubleQuote", "Comment",    "BinaryOperator", "And", "Or",
              "Equals", "Unary",       "End",          "Unexpected",
      };
      return names[static_cast<int>(kind)];
  }

  Token(Kind kind, const char* beg, std::size_t len) noexcept
      : m_kind{kind}, m_lexeme(beg, len), size(len) {}

  Token(Kind kind, const char* beg, const char* end) noexcept
      : m_kind{kind}, m_lexeme(beg, std::distance(beg, end)) {}

  Kind kind() const noexcept { return m_kind; }

  void kind(Kind kind) noexcept { m_kind = kind; }
  std::string kind_str();

  bool is(Kind kind) const noexcept { return m_kind == kind; }

  std::string to_string() {
      return "Token " + kind_str() + " (" + std::to_string(this->line) + ":" + std::to_string(this->column) +") value: " + lexeme();
  }

  bool is_not(Kind kind) const noexcept { return m_kind != kind; }

  bool is_one_of(Kind k1, Kind k2) const noexcept { return is(k1) || is(k2); }

  template <typename... Ts>
  bool is_one_of(Kind k1, Kind k2, Ts... ks) const noexcept {
    return is(k1) || is_one_of(k2, ks...);
  }

  std::string lexeme() const noexcept { return m_lexeme; }

  unsigned long line = 1;
  unsigned long column = 1;
  void lexeme(std::string lexeme) noexcept {
    m_lexeme = std::move(lexeme);
  }
  unsigned int size;

 private:
  Kind             m_kind{};
  std::string m_lexeme{};
};

class Lexer {
 public:
  Lexer(const char* beg) noexcept : m_beg{beg} {
  }

  Token next() noexcept;
  void test();

 private:
  Token identifier() noexcept;
  Token number() noexcept;
  Token slash_or_comment() noexcept;
  Token logical_or_binary_operator() noexcept;
  Token math_or_binary_operator() noexcept;
  Token equal_or_equals() noexcept;
  Token atom(Token::Kind) noexcept;
  Token parse_string(char quote) noexcept;

  char peek() const noexcept { return *m_beg; }
  char get() noexcept { return *m_beg++; }

  const char* m_beg = nullptr;
};
