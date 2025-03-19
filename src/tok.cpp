#include <string> 
#include <unordered_set>

#define whitespace ' '

enum class TokenType { id, type, int_literal, punctuation };

std::unordered_set<std::string> types = { "int" };
std::unordered_set<std::string> punctuations = { "=", ";" , "+" };

using Token = std::pair<TokenType,std::string>;

class Tokenizer { 
    std::optional<Token> currToken;
    std::optional<Token> nextToken;
    std::string& input;
    size_t pos = 0;

    inline bool eatable() { 
        return pos < input.size() && input[pos] != whitespace; 
        return pos < input.size() && input[pos] != whitespace && input[pos] != ';'; 
    }

    Token getNextToken() {
        Token t;
        while (pos < input.size()) {
            while (input[pos] == whitespace) pos++;
            if (isalpha(input[pos])) {
                while (pos < input.size() && isalnum(input[pos])) {
                    t.second.push_back(input[pos]);
                    pos++;
                }
                t.first = types.contains(t.second) ? TokenType::type : TokenType::id;
                break;
            } else if (isdigit(input[pos])) {
                t.first = TokenType::int_literal;
                while (pos < input.size() && isdigit(input[pos])) {
                    t.second += input[pos];
                    pos++;
                }
                break;
            } else {
                t.first = TokenType::punctuation;
                while (eatable()) { 
                    t.second += input[pos++];
                }
                break;
            }
        }
        return t;
    }

public:
    Tokenizer(std::string& input): input(input) { 
        nextToken = getNextToken();
    }

    std::optional<Token> curr() {
        return currToken;
    }

    std::optional<Token> eat() {
        currToken = nextToken;
        nextToken = getNextToken();
        return currToken;
    }

    std::optional<Token> peek() {
        return nextToken;
    }
};