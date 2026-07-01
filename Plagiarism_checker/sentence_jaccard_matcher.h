#ifndef SENTENCE_JACCARD_MATCHER_H
#define SENTENCE_JACCARD_MATCHER_H

#include <vector>
#include <string>
#include <unordered_set>
#include <functional>

class SentenceJaccardMatcher {
public:
    // Hash a sentence into a string
    static size_t hashSentence(const std::vector<std::string>& sentence) {
        std::string concat;
        for (const auto& token : sentence)
            concat += token + " ";
        return std::hash<std::string>{}(concat);
    }

    static double computeSimilarity(
        const std::vector<std::vector<std::string>>& A,
        const std::vector<std::vector<std::string>>& B
    ) {
        std::unordered_set<size_t> setA, setB;
        for (const auto& s : A) setA.insert(hashSentence(s));
        for (const auto& s : B) setB.insert(hashSentence(s));

        int intersection = 0;
        for (auto h : setA) {
            if (setB.count(h)) ++intersection;
        }

        int unionSize = setA.size() + setB.size() - intersection;
        return unionSize == 0 ? 0.0 : static_cast<double>(intersection) / unionSize;
    }
};

#endif
