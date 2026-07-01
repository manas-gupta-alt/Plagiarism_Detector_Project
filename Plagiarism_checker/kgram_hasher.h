 #ifndef KGRAM_HASHER_H
#define KGRAM_HASHER_H

#include <vector>
#include <string>
#include <unordered_set>
#include <functional>
#include <algorithm>

class KGramHasher {
public:
    // Compute adaptive k based on token count
    static int adaptiveK(int tokenCount) {
        if (tokenCount < 30) return 1;   // fallback to k=1 if too short
        if (tokenCount < 100) return 4;
        if (tokenCount < 300) return 5;
        return 6;
    }

    // Generate hash set of all k-grams in tokens
    static std::unordered_set<size_t> generateKGramHashes(
        const std::vector<std::string>& tokens,
        int k
    ) {
        std::unordered_set<size_t> hashSet;
        std::hash<std::string> hasher;

        for (int i = 0; i + k <= tokens.size(); ++i) {
            std::string kgram;
            for (int j = 0; j < k; ++j)
                kgram += tokens[i + j] + " ";
            hashSet.insert(hasher(kgram));
        }

        return hashSet;
    }

    // Compute Jaccard similarity between token sequences
    static double computeSimilarity(
        const std::vector<std::string>& A,
        const std::vector<std::string>& B
    ) {
        if (A.empty() || B.empty()) return 0.0;

        int k = std::min(adaptiveK(A.size()), adaptiveK(B.size()));

        auto hashA = generateKGramHashes(A, k);
        auto hashB = generateKGramHashes(B, k);

        if (hashA.empty() && hashB.empty()) return 1.0;
        if (hashA.empty() || hashB.empty()) return 0.0;

        int intersection = 0;
        for (const auto& h : hashA)
            if (hashB.count(h)) ++intersection;

        int unionSize = hashA.size() + hashB.size() - intersection;
        return static_cast<double>(intersection) / unionSize;
    }
};

#endif
