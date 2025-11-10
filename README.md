# AST-Printer

A simple **pseudo-C++ parser and AST visualizer**, written in C++ as part of the *Formal Approaches, Languages, and Compilers* course project.

This tool reads source code written in a simplified C++-like syntax, performs lexical and syntax analysis, builds an **Abstract Syntax Tree (AST)**, and prints the result in a readable tree structure.

---

## 🧩 Project Structure

```
AST-Printer/
├── Include/
│   ├── ast.hpp          # AST node class definitions
│   ├── parser.hpp       # Parser class declaration
│   └── tokens.hpp       # Token structure and type definitions
├── src/
│   ├── main.cpp         # Entry point: connects scanner, parser, and AST printer
│   ├── parser.cpp       # Recursive-descent parser implementation
│   └── scanner.l        # Flex-based lexical analyzer
├── CMakeLists.txt       # Build configuration
└── read.txt             # Example input file
```

---

## ⚙️ Build & Run

### Requirements
- **C++17 or later**
- **CMake ≥ 3.16**
- **Flex** (for lexical analysis)
- **Unix or Unix-like System**

### Steps
```bash
# 1. Clone the repository
git clone https://github.com/ISEP-Projects-JH/AST-Printer.git
cd AST-Printer

# 2. Build
mkdir build && cd build
cmake ..
cmake --build .

# 3. Run
./compiler
```

---

## 📄 Example Input (`read.txt`)

```cpp
int Va = 5;
int Vb = 2;
Va = Va + Vb;
print(Va);
END
```

---

## 🌳 Example Output

```
Tokens:
int Va = 5 ; int Vb = 2 ; Va = Va + Vb ; print ( Va ) ; END 

AST:
└── Statement
    ├── Statement
    │   ├── Statement
    │   │   ├── Statement
    │   │   │   └── Declaration (int)
    │   │   │       ├── Identifier: Va
    │   │   │       └── Number: 5
    │   │   └── Declaration (int)
    │   │       ├── Identifier: Vb
    │   │       └── Number: 2
    │   └── Assignment (=)
    │       ├── Identifier: Va
    │       └── BinOp (+)
    │           ├── Identifier: Va
    │           └── Identifier: Vb
    └── Print
        └── Identifier: Va
```

---

## 🧠 Key Concepts

* **Lexical Analysis** using Flex to tokenize pseudo-C++ syntax.
* **Recursive Descent Parsing** to apply grammar rules and verify syntax.
* **AST Construction** with node classes (`BinOpNode`, `Declaration`, `Assignment`, etc.).
* **Tree Visualization** using depth-first traversal in `main.cpp`.

---

## 📘 Grammar Summary

```
Program        := Statements END
Statements     := Statement Statements | ε
Statement      := Declaration | Assignment | IfStatement | WhileStatement | PrintStatement
Declaration    := (int | string) Var [= Expr] ;
Assignment     := Var = Expr ;
PrintStatement := print ( Expr ) ;
Expr           := Term { ("+" | "-") Term }
Term           := Factor { ("*" | "/") Factor }
Factor         := IntLit | StringLit | Var | "(" Expr ")"
```

---

## 🧾 License

This project is released for educational purposes under the **MIT License**.

---

## 🧑‍💻 Author

**JeongHan Bae**
[GitHub Profile](https://github.com/JeongHan-Bae)
Course Project Repository: [ISEP-Projects-JH/AST-Printer](https://github.com/ISEP-Projects-JH/AST-Printer)
