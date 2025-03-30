#include "Prs.h"
#include <string>

using namespace AST;

int main() { 
    std::print("{}\n", std::string(*Parser("(a+a)*a").expr()));
}