#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

using namespace std;

// Initialize random number generator in .cpp file for ODR reasons
std::mt19937 Random::rng;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function declarations go at the top of the file so we can call them
// anywhere in our program, such as in main or in other functions.
// Most other function declarations are in the included header
// files.

// When you add a new helper function, make sure to declare it up here!

/**
 * Print instructions for using the program.
 */
void printMenu();
void decryptSubstCipherFileCommand(const QuadgramScorer& scorer);

int main() {
  Random::seed(time(NULL));
  string command;

  vector<string> dict;
  ifstream dictFile("dictionary.txt");
  string word;
  while (getline(dictFile, word)) {
    dict.push_back(word);
  }
  dictFile.close();

  vector<string> quadgrams;
  vector<int> counts;
  ifstream quadFile("english_quadgrams.txt");
  string line;
  while (getline(quadFile, line)) {
    size_t comma = line.find(',');
    string quad = line.substr(0, comma);
    int count = stoi(line.substr(comma + 1));
    quadgrams.push_back(quad);
    counts.push_back(count);
  }
  quadFile.close();

  QuadgramScorer scorer(quadgrams, counts);

  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  do {
    printMenu();
    cout << endl << "Enter a command (case does not matter): ";

    // Use getline for all user input to avoid needing to handle
    // input buffer issues relating to using both >> and getline
    getline(cin, command);
    cout << endl;

    if (command == "R" || command == "r") {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));
    } else if (command == "C" || command == "c") {
      caesarEncryptCommand();
    } else if (command == "D" || command == "d") {
      caesarDecryptCommand(dict);
    } else if (command == "A" || command == "a") {
      applyRandSubstCipherCommand();
    } else if (command == "E" || command == "e") {
      computeEnglishnessCommand(scorer);
    } else if (command == "S" || command == "s") {
      decryptSubstCipherCommand(scorer);
    } else if (command == "F" || command == "f") {
      decryptSubstCipherFileCommand(scorer);
    }

    cout << endl;

  } while (!(command == "x" || command == "X") && !cin.eof());

  return 0;
}

void printMenu() {
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "F - Decrypt Substitution Cipher from File" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "X - Exit Program" << endl;
}

// "#pragma region" and "#pragma endregion" group related functions in this file
// to tell VSCode that these are "foldable". You might have noticed the little
// down arrow next to functions or loops, and that you can click it to collapse
// those bodies. This lets us do the same thing for arbitrary chunks!
#pragma region CaesarEnc

char rot(char c, int amount) {
  // TODO: student
  int index = ALPHABET.find(c);
  index = (index + amount) % 26;
  if (index < 0) index += 26;
  return ALPHABET[index];
}

string rot(const string& line, int amount) {
  // TODO: student
  string result = "";
  for (char c : line) {
    if (isalpha(c)) {
      result += rot(toupper(c), amount);
    } else if (isspace(c)) {
      result += c;
    }
  }
  return result;
}

void caesarEncryptCommand() {
  // TODO: student
  string text, amount;
  cout << "Enter the text to encrypt: ";
  getline(cin, text);
  cout << "Enter the number of characters to rotate by: ";
  getline(cin, amount);
  cout << rot(text, stoi(amount)) << endl;
}

#pragma endregion CaesarEnc

#pragma region CaesarDec

void rot(vector<string>& strings, int amount) {
  // TODO: student
  for (int i = 0; i < strings.size(); i++) {
    strings[i] = rot(strings[i], amount);
  }
}

string clean(const string& s) {
  // TODO: student
  string result = "";
  for (char c : s) {
    if (isalpha(c)) {
      result += toupper(c);
    }
  }
  return result;
}

vector<string> splitBySpaces(const string& s) {
  // TODO: student
  vector<string> words;
  string word = "";

  for (char c : s) {
    if (isspace(c)) {
      if (word != "") {
        words.push_back(clean(word));
        word = "";
      }
    } else {
      word += c;
    }
  }

  if (word != "") {
    words.push_back(clean(word));
  }
  return words;
}

string joinWithSpaces(const vector<string>& words) {
  // TODO: student
  string result = "";
  for (int i = 0; i < words.size(); i++) {
    result += words[i];
    if (i < words.size() - 1) {
      result += " ";
    }
  }
  return result;
}

int numWordsIn(const vector<string>& words, const vector<string>& dict) {
  int count = 0;
  for (const string& word : words) {
    for (const string& dictWord : dict) {
      if (word == dictWord) {
        count++;
        break;
      }
    }
  }
  return count;
}

void caesarDecryptCommand(const vector<string>& dict) {
  // TODO: student
  string text;
  cout << "Enter the text to Caesar decrypt: ";
  getline(cin, text);

  vector<string> words = splitBySpaces(text);
  bool foundDecryption = false;

  for (int shift = 0; shift < 26; shift++) {
    vector<string> rotated = words;
    rot(rotated, shift);

    int matchCount = numWordsIn(rotated, dict);

    if (matchCount * 2 > rotated.size()) {
      cout << joinWithSpaces(rotated) << endl;
      foundDecryption = true;
    }
  }

  if (!foundDecryption) {
    cout << "No good decryptions found" << endl;
  }
}

#pragma endregion CaesarDec

#pragma region SubstEnc

string applySubstCipher(const vector<char>& cipher, const string& s) {
  // TODO: student
  string result = "";

  for (char c : s) {
    if (isalpha(c)) {
      char upperC = toupper(c);
      int index = ALPHABET.find(upperC);
      result += cipher[index];
    } else {
      result += c;
    }
  }
  return result;
}

void applyRandSubstCipherCommand() {
  // TODO: student
  string text;
  cout << "Enter the text to apply a random substitution cipher to: ";
  getline(cin, text);

  vector<char> cipher = genRandomSubstCipher();
  cout << applySubstCipher(cipher, text) << endl;
}

#pragma endregion SubstEnc

#pragma region SubstDec

double scoreString(const QuadgramScorer& scorer, const string& s) {
  // TODO: student
  string clean = "";
  for (char c : s) {
    if (isalpha(c)) {
      clean += toupper(c);
    }
  }

  double score = 0.0;

  // Need at least 4 characters for a quadgram
  if (clean.length() >= 4) {
    for (int i = 0; i <= (int)clean.length() - 4; i++) {
      string quad = clean.substr(i, 4);
      score += scorer.getScore(quad);
    }
  }

  return score;
}

void computeEnglishnessCommand(const QuadgramScorer& scorer) {
  // TODO: student
  string text;
  cout << "Enter a string for englishness scoring: ";
  getline(cin, text);

  double score = scoreString(scorer, text);
  cout << score << endl;
}

vector<char> hillClimb(const QuadgramScorer& scorer, const string& ciphertext) {
  // TODO: student
  vector<char> key = genRandomSubstCipher();
  double bestScore = scoreString(scorer, applySubstCipher(key, ciphertext));

  int noImp = 0;

  while (noImp < 1000) {
    int i = Random::randInt(25);
    int j = Random::randInt(25);
    while (i == j) {
      j = Random::randInt(25);
    }

    vector<char> newKey = key;
    char temp = newKey[i];
    newKey[i] = newKey[j];
    newKey[j] = temp;

    double newScore = scoreString(scorer, applySubstCipher(newKey, ciphertext));

    if (newScore > bestScore) {
      key = newKey;
      bestScore = newScore;
      noImp = 0;
    } else {
      noImp++;
    }
  }

  return key;
}

vector<char> decryptSubstCipher(const QuadgramScorer& scorer,
                                const string& ciphertext) {
  // TODO: student
  vector<char> bestKey;
  double bestScore = -999999999.0;

  for (int run = 0; run < 25; run++) {
    vector<char> key = hillClimb(scorer, ciphertext);
    double score = scoreString(scorer, applySubstCipher(key, ciphertext));

    if (score > bestScore) {
      bestScore = score;
      bestKey = key;
    }
  }

  return bestKey;
}

void decryptSubstCipherCommand(const QuadgramScorer& scorer) {
  // TODO: student
  string text;
  cout << "Enter the text to decrypt: ";
  getline(cin, text);

  vector<char> key = decryptSubstCipher(scorer, text);
  cout << applySubstCipher(key, text) << endl;
}

void decryptSubstCipherFileCommand(const QuadgramScorer& scorer) {
  string inputFile, outputFile;
  cout << "Enter the input file name: ";
  getline(cin, inputFile);
  cout << "Enter the output file name: ";
  getline(cin, outputFile);

  ifstream inFile(inputFile);
  string ciphertext = "";
  string line;
  while (getline(inFile, line)) {
    ciphertext += line + "\n";
  }
  inFile.close();

  vector<char> key = decryptSubstCipher(scorer, ciphertext);
  string plaintext = applySubstCipher(key, ciphertext);

  ofstream outFile(outputFile);
  outFile << plaintext;
  outFile.close();
}

#pragma endregion SubstDec
