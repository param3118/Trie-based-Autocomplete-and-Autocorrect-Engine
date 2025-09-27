Trie-Based Autocomplete & Autocorrect Engine
📌 Project Overview

This project is an implementation of a dictionary engine using a Trie data structure in C.
It supports:

✅ Word insertion & deletion

✅ Spell checking

✅ Autocomplete (suggestions based on prefix)

✅ Autocorrect (using Levenshtein distance)

✅ Concatenation check (word formed by combining dictionary words)

The engine reads a dictionary file (dict.txt), processes queries from an input file (input.txt), and writes results to output.txt.

⚙️ Features

Add Word

Inserts a new word into the Trie.

Delete Word

Removes a word from the Trie if it exists.

Spell Check

Checks if a word exists in the dictionary.

Updates the word’s usage rank if found.

Autocomplete

Suggests words from the dictionary based on a given prefix.

Words are sorted by rank and lexicographical order.

Autocorrect

Suggests the closest matching word using Levenshtein distance (edit distance ≤ 3).

Concatenation Check

Verifies if a word can be formed by concatenating two or more dictionary words.

📂 Project Structure
📦 Trie-Autocomplete-Autocorrect
 ┣ 📜 main.c              # Main C source code
 ┣ 📜 dict.txt            # Dictionary words (input for Trie)
 ┣ 📜 input.txt           # Query inputs
 ┣ 📜 output.txt          # Results of queries
 ┣ 📜 README.md           # Documentation


📄 File Descriptions

dict.txt → Contains the dictionary words (one word per line).

input.txt → Contains queries in the format:

Q
<query_type> <word>



where:

Q = number of queries

query_type = operation type (1–6, see below)

word = input word/prefix for the operation

output.txt → Contains the results of each query.

🔢 Query Types
Code	Operation	Example Input	Example Output
1	Add Word	1 hello	(word added, no output)
2	Delete Word	2 hello	Deleted.
3	Spell Check	3 world	1- spell check
4	Autocomplete	4 hel	hello
help
helium
5	Autocorrect	5 wrld	world
6	Concatenation Check	6 helloworld	1- helloworld is a concatenation of two or more dictionary words.
🛠️ Compilation & Execution
1. Compile the program:
gcc main.c -o trie_engine

2. Run the program:
./trie_engine


The program will:

Load dictionary from dict.txt

Process queries from input.txt

Write results into output.txt

📘 Example
dict.txt
hello
help
helium
world
low
hell
cat
dog

input.txt
6
3 hello
4 he
5 wrld
6 helloworld
2 hello
4 he

output.txt (generated)
1- spell check
hello
helium
help
world
1- helloworld is a concatenation of two or more dictionary words.
Deleted.
helium
help

📊 Data Structures Used

TrieNode

typedef struct TrieNode {
    struct TrieNode *children[26]; // Pointers for 'a'–'z'
    int isEndOfWord;              // Marks end of word
    int rank;                     // Frequency-based ranking
} TrieNode;


WordRank (for autocomplete ranking)

typedef struct WordRank {
    char word[MAX_WORD_LENGTH];
    int rank;
} WordRank;

📚 Algorithms Used

Trie Traversal → For insertion, deletion, and search.

Levenshtein Distance → For autocorrect.

Dynamic Programming (DP) → For concatenation check.

Sorting (qsort) → For ranked autocomplete suggestions.

🚀 Future Improvements

Support uppercase words automatically (case-insensitive).

Load large dictionary files efficiently.

Add frequency-based learning from user history.

Allow returning unlimited autocomplete results instead of a fixed MAX_OUTPUT_WORDS.

👨‍💻 Author

Paramjeet Singh
M.Tech Student, IIT Kharagpur (CSE)
Passionate about algorithms, ML, and full-stack development.