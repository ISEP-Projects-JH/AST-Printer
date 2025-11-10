#pragma once
#include <memory>
#include <string>
#include <vector>
#include "tokens.hpp"

struct Node
{
    virtual ~Node() = default;
};

// ===== Literal & Identifier =====
struct NumberNode : Node
{
    Token tok;
    explicit NumberNode(Token t) : tok(std::move(t)) {}
    NumberNode() = default;
    [[maybe_unused]] [[nodiscard]] std::string getValue() const { return tok.value; }
};

struct StringNode : Node
{
    Token tok;
    explicit StringNode(Token t) : tok(std::move(t)) {}
    StringNode() = default;
    [[maybe_unused]] [[nodiscard]] std::string getValue() const { return tok.value; }
};

struct IdentifierNode : Node
{
    Token tok;
    explicit IdentifierNode(Token t) : tok(std::move(t)) {}
    IdentifierNode() = default;
    [[maybe_unused]] [[nodiscard]] std::string getValue() const { return tok.value; }
};

// ===== Binary Operation =====
struct BinOpNode : Node
{
    std::shared_ptr<Node> left;
    Token op_tok;
    std::shared_ptr<Node> right;

    BinOpNode(std::shared_ptr<Node> l, Token op, std::shared_ptr<Node> r)
            : left(std::move(l)), op_tok(std::move(op)), right(std::move(r)) {}
    BinOpNode() = default;
};

// ===== Statement =====
struct Statement : Node
{
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    explicit Statement(std::shared_ptr<Node> l, std::shared_ptr<Node> r = nullptr)
            : left(std::move(l)), right(std::move(r)) {}
    Statement() = default;
};

// ===== Condition =====
struct Condition : Node
{
    std::shared_ptr<Node> left;
    Token op_tok;
    std::shared_ptr<Node> right;
    Condition(std::shared_ptr<Node> l, Token op, std::shared_ptr<Node> r)
            : left(std::move(l)), op_tok(std::move(op)), right(std::move(r)) {}
    Condition() = default;
};

// ===== If / While / Print / Assignment / Declaration =====
struct IfStatement : Node
{
    std::shared_ptr<Condition> cond;
    std::vector<std::shared_ptr<Node>> then_body;
    std::vector<std::shared_ptr<Node>> else_body;
    explicit IfStatement(std::shared_ptr<Condition> c,
                std::vector<std::shared_ptr<Node>> thenB = {},
                std::vector<std::shared_ptr<Node>> elseB = {})
            : cond(std::move(c)), then_body(std::move(thenB)), else_body(std::move(elseB)) {}
    IfStatement() = default;
};

struct WhileStatement : Node
{
    std::shared_ptr<Condition> cond;
    std::vector<std::shared_ptr<Node>> body;
    explicit WhileStatement(std::shared_ptr<Condition> c, std::vector<std::shared_ptr<Node>> b = {})
            : cond(std::move(c)), body(std::move(b)) {}
    WhileStatement() = default;
};

struct PrintStatement : Node
{
    Token print_tok;
    std::shared_ptr<Node> expr;
    PrintStatement(Token t, std::shared_ptr<Node> e)
            : print_tok(std::move(t)), expr(std::move(e)) {}
    PrintStatement() = default;
};

struct Assignment : Node
{
    Token identifier;
    std::shared_ptr<Node> expression;
    Assignment(Token id, std::shared_ptr<Node> expr)
            : identifier(std::move(id)), expression(std::move(expr)) {}
    Assignment() = default;
};

struct Declaration : Node
{
    Token type_tok;
    Token identifier;
    std::shared_ptr<Node> init_expr;
    Declaration(Token t, Token id, std::shared_ptr<Node> init = nullptr)
            : type_tok(std::move(t)), identifier(std::move(id)), init_expr(std::move(init)) {}
    Declaration() = default;
};
