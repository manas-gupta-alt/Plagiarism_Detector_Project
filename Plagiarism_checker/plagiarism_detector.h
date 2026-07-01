 #ifndef PLAGIARISM_DETECTOR_H
#define PLAGIARISM_DETECTOR_H

#include "tokenizer.h"
#include "token_trie.h"
#include "sentence_matcher.h"
#include "sentence_jaccard_matcher.h"
#include "dsu.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

class PlagiarismDetector {
private:
    TokenTrie trie;
    std::vector<std::vector<std::string>> allTokens;
    std::vector<std::vector<std::vector<std::string>>> allSentences;
    DSU dsu;
    int submissionCount = 0;

    // Thresholds
    double trieThreshold = 0.6;
    double sentenceSlidingThreshold = 0.75;
    double sentenceJaccardThreshold = 0.85;

public:
    PlagiarismDetector() : dsu(1000) {}

    std::vector<int> submit(const std::string& code) {
        Tokenizer::init();
        std::vector<std::string> tokens = Tokenizer::tokenize(code);
        std::vector<std::vector<std::string>> sentences = SentenceMatcher::splitIntoSentences(tokens);

        int id = submissionCount++;
        allTokens.push_back(tokens);
        allSentences.push_back(sentences);

        std::unordered_set<int> matchedIDs;

        // ---- Phase 1: Suffix Trie Match ----
        std::vector<int> trieMatches = trie.getMatchingIDs(tokens, trieThreshold);
        for (int sid : trieMatches) {
            matchedIDs.insert(dsu.find(sid));
            dsu.unite(id, sid);
        }

        // ---- Phase 2: Sentence Sliding Match ----
        for (int i = 0; i < allSentences.size() - 1; ++i) {
            if (dsu.find(i) == dsu.find(id)) continue;

            double sim = SentenceMatcher::slidingMatch(allSentences[i], sentences);
            if (sim >= sentenceSlidingThreshold) {
                matchedIDs.insert(dsu.find(i));
                dsu.unite(id, i);
            }
        }

        // ---- Phase 3: Sentence Jaccard Match ----
        for (int i = 0; i < allSentences.size() - 1; ++i) {
            if (dsu.find(i) == dsu.find(id)) continue;

            double sim = SentenceJaccardMatcher::computeSimilarity(allSentences[i], sentences);
            if (sim >= sentenceJaccardThreshold) {
                matchedIDs.insert(dsu.find(i));
                dsu.unite(id, i);
            }
        }

        // Insert tokens into Trie for future comparisons
        trie.insert(tokens, id);

        // Return all IDs in the same DSU component
        std::vector<int> result;
        int root = dsu.find(id);
        for (int i = 0; i < submissionCount; ++i) {
            if (dsu.find(i) == root)
                result.push_back(i);
        }

        return result;
    }
};

#endif
