 #ifndef TOKEN_TRIE_H
#define TOKEN_TRIE_H

#include <unordered_map>
#include <vector>
#include <string>

class TokenTrie {
private:
    struct TrieNode {
        std::unordered_map<std::string, TrieNode*> children;
        bool isEnd = false;
        std::vector<int> submissionIDs; // stores all submission IDs ending here
    };

    TrieNode* root;

    void collectSubmissionIDs(TrieNode* node, std::vector<int>& result) const {
        if (node->isEnd)
            result.insert(result.end(), node->submissionIDs.begin(), node->submissionIDs.end());

        for (const auto& [_, child] : node->children)
            collectSubmissionIDs(child, result);
    }

public:
    TokenTrie() {
        root = new TrieNode();
    }

    void insert(const std::vector<std::string>& tokens, int submissionID) {
        std::vector<std::string> reversed(tokens.rbegin(), tokens.rend());
        TrieNode* node = root;
        for (const std::string& token : reversed) {
            if (!node->children.count(token))
                node->children[token] = new TrieNode();
            node = node->children[token];
        }
        node->isEnd = true;
        node->submissionIDs.push_back(submissionID);
    }

    int getPrefixMatchLength(const std::vector<std::string>& reversedTokens) const {
        TrieNode* node = root;
        int matchLength = 0;
        for (const std::string& token : reversedTokens) {
            if (node->children.count(token)) {
                node = node->children[token];
                ++matchLength;
            } else {
                break;
            }
        }
        return matchLength;
    }

    double prefixSimilarity(const std::vector<std::string>& tokens) const {
        std::vector<std::string> reversed(tokens.rbegin(), tokens.rend());
        int matchLength = getPrefixMatchLength(reversed);
        return static_cast<double>(matchLength) / tokens.size();
    }

    // Returns all submission IDs whose suffix match length >= threshold × size of tokens
    std::vector<int> getMatchingIDs(const std::vector<std::string>& tokens, double threshold = 0.6) const {
        std::vector<std::string> reversed(tokens.rbegin(), tokens.rend());
        TrieNode* node = root;
        int matchLength = 0;
        std::vector<int> result;

        for (const std::string& token : reversed) {
            if (node->children.count(token)) {
                node = node->children[token];
                ++matchLength;
                if (matchLength >= threshold * tokens.size()) {
                    collectSubmissionIDs(node, result);
                    break;
                }
            } else {
                break;
            }
        }

        return result;
    }
};

#endif
