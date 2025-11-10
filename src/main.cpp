#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include "tokens.hpp"
#include "parser.hpp"

extern void scan_string_to_tokens(const std::string &src, std::vector<Token> &out);

// ======== AST printer ========
void print_ast(const std::shared_ptr<Node> &node, std::string_view prefix = "", bool isLast = true)
{
    if (!node)
        return;

    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");

    // print by val
    if (auto num = std::dynamic_pointer_cast<NumberNode>(node))
    {
        std::cout << "Number: " << num->tok.value << "\n";
    }
    else if (auto str = std::dynamic_pointer_cast<StringNode>(node))
    {
        std::cout << "String: " << str->tok.value << "\n";
    }
    else if (auto id = std::dynamic_pointer_cast<IdentifierNode>(node))
    {
        std::cout << "Identifier: " << id->tok.value << "\n";
    }
    else if (auto bin = std::dynamic_pointer_cast<BinOpNode>(node))
    {
        std::cout << "BinOp (" << bin->op_tok.value << ")\n";
        print_ast(bin->left, std::string(prefix) + (isLast ? "    " : "│   "), false);
        print_ast(bin->right, std::string(prefix) + (isLast ? "    " : "│   "), true);
    }
    else if (auto cond = std::dynamic_pointer_cast<Condition>(node))
    {
        std::cout << "Condition (" << cond->op_tok.value << ")\n";
        print_ast(cond->left, std::string(prefix) + (isLast ? "    " : "│   "), false);
        print_ast(cond->right, std::string(prefix) + (isLast ? "    " : "│   "), true);
    }
    else if (auto asg = std::dynamic_pointer_cast<Assignment>(node))
    {
        std::cout << "Assignment (=)\n";
        print_ast(std::make_shared<IdentifierNode>(asg->identifier), std::string(prefix) + (isLast ? "    " : "│   "), false);
        print_ast(asg->expression, std::string(prefix) + (isLast ? "    " : "│   "), true);
    }
    else if (auto decl = std::dynamic_pointer_cast<Declaration>(node))
    {
        std::cout << "Declaration (" << decl->type_tok.value << ")\n";
        print_ast(std::make_shared<IdentifierNode>(decl->identifier), std::string(prefix) + (isLast ? "    " : "│   "), false);
        if (decl->init_expr)
            print_ast(decl->init_expr, std::string(prefix) + (isLast ? "    " : "│   "), true);
    }
    else if (auto print = std::dynamic_pointer_cast<PrintStatement>(node))
    {
        std::cout << "Print\n";
        print_ast(print->expr, std::string(prefix) + (isLast ? "    " : "│   "), true);
    }
    else if (auto wh = std::dynamic_pointer_cast<WhileStatement>(node))
    {
        std::cout << "While\n";
        print_ast(wh->cond, std::string(prefix) + (isLast ? "    " : "│   "), false);
        for (size_t i = 0; i < wh->body.size(); ++i)
            print_ast(wh->body[i], std::string(prefix) + (isLast ? "    " : "│   "), i == wh->body.size() - 1);
    }
    else if (auto iff = std::dynamic_pointer_cast<IfStatement>(node))
    {
        std::cout << "IfStatement\n";
        print_ast(iff->cond, std::string(prefix) + (isLast ? "    " : "│   "), false);
        for (size_t i = 0; i < iff->then_body.size(); ++i)
            print_ast(iff->then_body[i], std::string(prefix) + (isLast ? "    " : "│   "), i == iff->then_body.size() - 1);

        if (!iff->else_body.empty())
        {
            std::cout << prefix << (isLast ? "    " : "│   ") << "└── Else\n";
            for (size_t i = 0; i < iff->else_body.size(); ++i)
                print_ast(iff->else_body[i], std::string(prefix) + (isLast ? "        " : "│   "), i == iff->else_body.size() - 1);
        }
    }
    else if (auto st = std::dynamic_pointer_cast<Statement>(node))
    {
        std::cout << "Statement\n";
        if (st->left)
            print_ast(st->left, std::string(prefix) + (isLast ? "    " : "│   "), false);
        if (st->right)
            print_ast(st->right, std::string(prefix) + (isLast ? "    " : "│   "), true);
    }
    else
    {
        std::cout << "Unknown Node\n";
    }
}

// ======== main ========
int main()
{
    std::ifstream fin("../read.txt");
    if (!fin)
    {
        std::cerr << "Cannot open read.txt\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string input = buffer.str();
    std::vector<Token> toks;

    try
    {
        scan_string_to_tokens(input, toks);

        TokenArray arr;
        for (auto &t : toks)
            arr.push(t);

        std::cout << "Tokens:\n";
        for (auto &t : toks)
            std::cout << t.value << " ";
        std::cout << "\n\nAST:\n";

        Parser parser(arr);
        auto root = parser.get_root();
        print_ast(root);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
