 #ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>

class Tokenizer {
public:
    static std::unordered_set<std::string> typeKeywords;
    static std::unordered_set<std::string> containerKeywords;
    static std::unordered_set<std::string> controlKeywords;

    static void init() {
        typeKeywords = {"int", "char", "string", "bool", "double"};
        containerKeywords = {
            "vector", "map", "unordered_map", "set", "pair",
            "queue", "stack", "deque", "priority_queue", "list"
        };
        controlKeywords = {
            "if", "else", "for", "while", "return", "break",
            "continue", "switch", "case", "do", "goto"
        };
    }

    static std::string removeComments(const std::string& code) {
        std::string result;
        bool singleLine = false, multiLine = false;

        for (size_t i = 0; i < code.size(); ++i) {
            if (singleLine && code[i] == '\n') {
                singleLine = false;
                result += '\n';
            } else if (multiLine && code[i] == '*' && i + 1 < code.size() && code[i + 1] == '/') {
                multiLine = false;
                ++i;
            } else if (singleLine || multiLine) {
                continue;
            } else if (code[i] == '/' && i + 1 < code.size()) {
                if (code[i + 1] == '/') {
                    singleLine = true;
                    ++i;
                } else if (code[i + 1] == '*') {
                    multiLine = true;
                    ++i;
                } else {
                    result += code[i];
                }
            } else {
                result += code[i];
            }
        }

        return result;
    }

    static std::string stripWhitespace(const std::string& code) {
        std::string stripped;
        for (char c : code) {
            if (!std::isspace(c))
                stripped += c;
        }
        return stripped;
    }

    static bool isIdentifierStart(char c) {
        return std::isalpha(c) || c == '_';
    }

    static bool isIdentifierChar(char c) {
        return std::isalnum(c) || c == '_';
    }

    static std::vector<std::string> tokenize(const std::string& codeRaw) {
        init(); // initialize keyword sets
        std::string clean = stripWhitespace(removeComments(codeRaw));
        std::vector<std::string> tokens;
        size_t i = 0;

        while (i < clean.size()) {
            if (isIdentifierStart(clean[i])) {
                std::string word;
                while (i < clean.size() && isIdentifierChar(clean[i]))
                    word += clean[i++];

                if (containerKeywords.count(word)) {
                    tokens.push_back("CONTAINER");
                } else if (typeKeywords.count(word)) {
                    tokens.push_back("TYPE");
                } else if (controlKeywords.count(word)) {
                    tokens.push_back(word); // keep as-is
                } else {
                    // check for FUNC (lookahead for '(')
                    if (i < clean.size() && clean[i] == '(')
                        tokens.push_back("FUNC");
                    else
                        tokens.push_back("VAR");
                }
            }
            else if (std::isdigit(clean[i])) {
                std::string num;
                while (i < clean.size() && std::isdigit(clean[i]))
                    num += clean[i++];
                tokens.push_back("NUM");
            }
            else {
                // multi-char operators
                if (i + 1 < clean.size()) {
                    std::string two = clean.substr(i, 2);
                    if (two == "<<" || two == ">>" || two == "<=" || two == ">=" ||
                        two == "==" || two == "!=" || two == "++" || two == "--") {
                        tokens.push_back(two);
                        i += 2;
                        continue;
                    }
                }
                tokens.push_back(std::string(1, clean[i++]));
            }
        }

        return tokens;
    }
};

// Static variable definitions
std::unordered_set<std::string> Tokenizer::typeKeywords;
std::unordered_set<std::string> Tokenizer::containerKeywords;
std::unordered_set<std::string> Tokenizer::controlKeywords;

#endif
