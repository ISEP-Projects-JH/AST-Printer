#pragma once
#include <string>
#include <vector>
#include <stdexcept>

enum class TokenType
{
    If,
    Else,
    While,
    Int,
    StringKw,
    Print,
    Prints,
    Assign,
    Comparison,
    Arth,
    L1,         // '('
    R1,         // ')'
    L2,         // '{'
    R2,         // '}'
    Semicolon,  // ';'
    Separator,  // ','
    Var,        // Variable identifier
    IntLit,     // Integer literal
    String,     // String literal
    End         // EOF
};

struct Token
{
    TokenType type;
    std::string value;
    int line;

    Token(TokenType t, std::string v, int l)
            : type(t), value(std::move(v)), line(l) {}
    Token() : type(TokenType::End), line(0) {}
};

class TokenArray
{
public:
    void push(const Token &t) { tokens.push_back(t); }

    [[nodiscard]] const Token &current() const
    {
        if (pos >= tokens.size())
            throw std::out_of_range("Token position out of range");
        return tokens[pos];
    }

    void next()
    {
        if (pos + 1 < tokens.size())
            ++pos;
    }

    [[maybe_unused]] [[nodiscard]] bool empty() const { return tokens.empty(); }

    [[maybe_unused]] void reset() { pos = 0; }

    void appendEndIfMissing()
    {
        if (tokens.empty() || tokens.back().type != TokenType::End)
            tokens.emplace_back(TokenType::End, "END", tokens.empty() ? 1 : tokens.back().line);
    }

    [[nodiscard]] size_t size() const { return tokens.size(); }

    const Token &operator[](size_t i) const
    {
        if (i >= tokens.size())
            throw std::out_of_range("Token index out of range");
        return tokens[i];
    }

    std::vector<Token> tokens;
    size_t pos{0};
};
