 # Plagiarism Detection Engine

A modular, high-performance C++ project to detect plagiarism in source code by combining lexical analysis, token normalization, structural similarity, and clustering.

##  Overview

This engine tokenizes code using a custom lexical analyzer and detects plagiarized submissions using a **multi-phase detection pipeline** involving:

* **Suffix-based matching using Token Trie** for fast prefix similarity.
* **Sentence-level Jaccard and sliding window analysis** to account for reordered or slightly altered logic.
* **Clustering using Disjoint Set Union (DSU)** for grouping related submissions.

## Features

* **C++ STL and OOP-based modular design**
* **Tokenizer** to normalize identifiers, keywords, types, containers, etc.
* **Token Trie** for fast suffix-level prefix match (O(L) complexity)
* **Sentence-based Jaccard and Sliding Matchers** for robust detection under code reordering and insertions
* **DSU-based clustering** to group all related plagiarized submissions
* Interactive CLI mode for real-time submission and result logging

## 📁 Project Structure

```
.
├── main.cpp
├── tokenizer.h
├── token_trie.h
├── tokenize_demo.cpp
├── sentence_matcher.h
├── sentence_jaccard_matcher.h
├── dsu.h
├── plagiarism_detector.h
├── kgram_hasher.h
└── README.md
```

## Detection Strategy

1. **Token Normalization**

   * Types (`int`, `char`, `string`) → `TYPE`
   * Containers (`vector`, `map`, `stack`) → `CONTAINER`
   * Identifiers → `VAR` / `FUNC`

2. **Phase 1: Trie-Based Suffix Check**

   * Reverse-tokenized code stored in a `TokenTrie`
   * Detect submissions with ≥ 60% common suffix instantly

3. **Phase 2: Sentence-Level Check** *(only if trie fails)*

   * Code is split into tokenized sentences (delimited by `;`)
   * Checked via:

     * **Sliding Sentence Matcher** (within ±1 window)
     * **Jaccard Similarity Matcher** (adaptive k-gram hashing)

4. **Clustering**

   * Submissions sharing a match are grouped using DSU

## Testing & Accuracy

* Evaluated over **50+ C++ codes** with:

  * Variable renaming
  * Dummy insertions
  * Reordering of code blocks
  * Comment manipulation
* Achieved **80–90% plagiarism detection accuracy** with **no significant false positives**

## Usage

1. Compile using any C++17+ compiler:

   ```bash
   g++ -std=c++17 main.cpp tokenize_demo.cpp -o plagiarism_detector
   ```

2. Run and interact:

   ```bash
   ./plagiarism_detector
   ```

3. Input code in the terminal (end input with `$$` on a new line).

##  Example

```
Input your C++ code. End with $$:
int main() {
  int a = 5;
  cout << a;
  return 0;
}
$$

Your code (submission 12) matches with: 5, 7, 10
```

##  Dependencies

* Pure C++, **no external libraries**
* Uses standard STL headers (`<vector>`, `<string>`, `<unordered_set>`, etc.)

##  License

MIT License

---

## Author
**Abhinav Jha**
