#ifndef SENTENCE_MATCHER_H
#define SENTENCE_MATCHER_H

#include <vector>
#include <string>
#include <cmath>

class SentenceMatcher {
public:
    // Computes sliding match score between token sentences
    static double slidingMatch(
        const std::vector<std::vector<std::string>>& A,
        const std::vector<std::vector<std::string>>& B,
        int window = 1
    ) {
        int matchCount = 0;
        for (int i = 0; i < A.size(); ++i) {
            bool found = false;
            for (int j = std::max(0, i - window); j <= std::min((int)B.size() - 1, i + window); ++j) {
                if (A[i] == B[j]) {
                    found = true;
                    break;
                }
            }
            if (found) ++matchCount;
        }

        return A.empty() ? 0.0 : static_cast<double>(matchCount) / A.size();
    }

    // Helper: Convert token vector to sentence-wise token blocks
    static std::vector<std::vector<std::string>> splitIntoSentences(const std::vector<std::string>& tokens) {
        std::vector<std::vector<std::string>> sentences;
        std::vector<std::string> current;

        for (const auto& token : tokens) {
            current.push_back(token);
            if (token == ";") {
                sentences.push_back(current);
                current.clear();
            }
        }

        if (!current.empty()) sentences.push_back(current);  // In case no semicolon at end
        return sentences;
    }
};

#endif
