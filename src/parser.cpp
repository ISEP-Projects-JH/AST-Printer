#include "parser.hpp"
#include <stdexcept>

static bool is_value(const Token &t, const char *v) { return t.value == v; }

Parser::Parser(TokenArray tokens) : tokens(std::move(tokens))
{
    this->tokens.appendEndIfMissing();
}

void Parser::read_token_pass(const std::string &expected, const std::string &message)
{
    const Token &t = tokens.current();
    if (t.value != expected)
        throw std::runtime_error(message + " in line " + std::to_string(t.line));
    tokens.next();
}

// ===== factor =====
std::shared_ptr<Node> Parser::factor()
{
    const Token &tok = tokens.current();
    if (tok.type == TokenType::IntLit)
    {
        tokens.next();
        return std::make_shared<NumberNode>(tok);
    }
    if (tok.type == TokenType::String)
    {
        tokens.next();
        return std::make_shared<StringNode>(tok);
    }
    if (tok.type == TokenType::Var)
    {
        tokens.next();
        return std::make_shared<IdentifierNode>(tok);
    }
    if (is_value(tok, "("))
    {
        tokens.next();
        auto e = expr();
        if (!is_value(tokens.current(), ")"))
            throw std::runtime_error("Expected ) in line " + std::to_string(tokens.current().line));
        tokens.next();
        return e;
    }
    throw std::runtime_error("Syntax Error: Expected value in line " + std::to_string(tok.line));
}

// ===== term =====
std::shared_ptr<Node> Parser::term()
{
    auto left = factor();
    return left;
}

// ===== expr =====
std::shared_ptr<Node> Parser::expr()
{
    auto left = term();
    while (tokens.current().value == "+" || tokens.current().value == "-")
    {
        Token op = tokens.current();
        tokens.next();
        auto right = term();
        auto bin = std::make_shared<BinOpNode>();
        bin->left = left;
        bin->op_tok = op;
        bin->right = right;
        left = bin;
    }
    return left;
}

// ===== condition =====
std::shared_ptr<Condition> Parser::condition()
{
    auto left = expr();
    Token op = tokens.current();
    if (op.type != TokenType::Comparison)
        throw std::runtime_error("Expected comparison operator in line " + std::to_string(op.line));
    tokens.next();
    auto right = expr();
    return std::make_shared<Condition>(left, op, right);
}

// ===== declaration =====
std::shared_ptr<Node> Parser::declarations()
{
    Token type_tok = tokens.current();
    tokens.next();
    if (tokens.current().type != TokenType::Var)
        throw std::runtime_error("Expected variable name in line " + std::to_string(tokens.current().line));

    Token id = tokens.current();
    tokens.next();

    std::shared_ptr<Node> init = nullptr;
    if (is_value(tokens.current(), "="))
    {
        tokens.next();
        init = expr();
    }

    read_token_pass(";", "Missing ;");
    return std::make_shared<Declaration>(type_tok, id, init);
}

// ===== assignment =====
std::shared_ptr<Node> Parser::assignment()
{
    Token identifier = tokens.current();
    tokens.next();
    read_token_pass("=", "Missing =");
    auto expression = expr();
    read_token_pass(";", "Missing ;");
    auto a = std::make_shared<Assignment>();
    a->identifier = identifier;
    a->expression = expression;
    return a;
}

// ===== print / prints =====
std::shared_ptr<Node> Parser::printing([[maybe_unused]] const std::string &type)
{
    Token print_tok = tokens.current();
    tokens.next();
    read_token_pass("(", "Missing ( after print");
    auto expr_node = expr();
    read_token_pass(")", "Missing ) after print expression");
    read_token_pass(";", "Missing ; after print");
    return std::make_shared<PrintStatement>(print_tok, expr_node);
}

// ===== if statement =====
std::shared_ptr<Node> Parser::if_statement()
{
    tokens.next(); // consume 'if'
    read_token_pass("(", "Missing ( after if");
    auto cond = condition();
    read_token_pass(")", "Missing ) after if condition");
    read_token_pass("{", "Missing { after if");

    auto then_body = std::vector<std::shared_ptr<Node>>();
    auto then_block = statements();
    if (then_block) then_body.push_back(then_block);

    read_token_pass("}", "Missing } after if block");

    std::vector<std::shared_ptr<Node>> else_body;
    if (is_value(tokens.current(), "else"))
    {
        tokens.next();
        read_token_pass("{", "Missing { after else");
        auto else_block = statements();
        if (else_block) else_body.push_back(else_block);
        read_token_pass("}", "Missing } after else block");
    }

    return std::make_shared<IfStatement>(cond, then_body, else_body);
}

// ===== while statement =====
std::shared_ptr<Node> Parser::while_statement()
{
    tokens.next(); // consume 'while'
    read_token_pass("(", "Missing ( after while");
    auto cond = condition();
    read_token_pass(")", "Missing ) after while condition");
    read_token_pass("{", "Missing { after while");

    auto body_nodes = std::vector<std::shared_ptr<Node>>();
    auto body = statements();
    if (body) body_nodes.push_back(body);

    read_token_pass("}", "Missing } after while block");

    return std::make_shared<WhileStatement>(cond, body_nodes);
}

// ===== statements =====
std::shared_ptr<Node> Parser::statements()
{
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right;
    Token t = tokens.current();

    while (t.value != "END" && t.value != "}")
    {
        if (t.type == TokenType::If)
        {
            right = if_statement();
        }
        else if (t.type == TokenType::While)
        {
            right = while_statement();
        }
        else if (t.type == TokenType::StringKw || t.type == TokenType::Int)
        {
            right = declarations();
        }
        else if (t.type == TokenType::Print)
        {
            right = printing("print");
        }
        else if (t.type == TokenType::Prints)
        {
            right = printing("prints");
        }
        else if (t.type == TokenType::Var)
        {
            right = assignment();
        }
        else
        {
            throw std::runtime_error("Syntax Error in line " + std::to_string(t.line));
        }

        auto st = std::make_shared<Statement>(left, right);
        left = st;
        t = tokens.current();
    }

    return left;
}

// ===== root =====
std::shared_ptr<Node> Parser::get_root()
{
    auto root = statements();
    if (tokens.current().type != TokenType::End)
        throw std::runtime_error("Syntax Error at end of program");
    return root;
}
