#include "utils.hpp"

float eucledianDistance(const std::pair<float, float>& a, const std::pair<float, float>& b) {
    return std::sqrt(((a.first - b.first) * (a.first - b.first)) + ((a.second - b.second) * (a.second - b.second)));
}

char fileToChar(File file) { return static_cast<char>('a' + file); }
char rankToChar(Rank rank) { return static_cast<char>('1' + rank); }
Rank charToRank(char c) { return static_cast<Rank>(c - '1'); }
File charToFile(char c) { return static_cast<File>(c - 'a'); }

std::string squareToString(Square s) { return {fileToChar(getFile(s)), rankToChar(getRank(s))}; }

Square stringToSquare(const std::string& s) { return createSquare(charToFile(s[0]), charToRank(s[1])); }

std::string moveToString(Move move) { return squareToString(getFrom(move)) + squareToString(getTo(move)); }

Move stringToMove(const std::string& str) {
    return createMove(stringToSquare(str.substr(0, 2)), stringToSquare(str.substr(2, 2)), 0);
}