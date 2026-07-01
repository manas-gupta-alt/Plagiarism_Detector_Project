#include <iostream>
#include <string>
#include <vector>
#include "plagiarism_detector.h"

int main() {
    PlagiarismDetector detector;
    std::string line, code;

    std::cout << "Welcome to the Plagiarism Detection System!\n";
    std::cout << "Submit code ending with '$$' on a new line.\n";

    while (true) {
        code.clear();
        std::cout << "\nEnter your code (end with '$$' on a new line):\n";

        // Read multiline code
        while (std::getline(std::cin, line)) {
            if (line == "$$") break;
            code += line + '\n';
        }

        if (code.empty()) continue;

        std::vector<int> result = detector.submit(code);
        int yourID = result.back();  // Last inserted ID is current

        std::cout << "\nYour code was assigned Submission ID: " << yourID << "\n";

        if (result.size() > 1) {
            std::cout << "Plagiarism detected! Your code matches with the following submission(s):\n";
            for (int id : result) {
                if (id != yourID)
                    std::cout << "- Submission ID: " << id << '\n';
            }
        } else {
            std::cout << "No plagiarism detected. You are good to go!\n";
        }
    }

    return 0;
}
