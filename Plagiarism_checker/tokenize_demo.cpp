#include <iostream>
#include <string>
#include <vector>
#include "Tokenizer.h"

int main() {
    std::string line, fullCode;

    std::cout << "Enter C++ code (end with $$ on a new line):\n";

    while (std::getline(std::cin, line)) {
        if (line == "$$") break;
        fullCode += line + '\n';
    }

    std::vector<std::string> tokens = Tokenizer::tokenize(fullCode);

    std::cout << "\n--- Tokenized Output ---\n";
    for (const std::string& token : tokens) {
        std::cout << token << std::endl;
    }

    return 0;
}
