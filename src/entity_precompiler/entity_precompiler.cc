#include "cpp_parser/parser.hh"

auto c_str = "int main(int, char**) {return 0;}";

int main(int argc, char* argv[]) {
    Cpp::Preprocessor preprocessor("main.cc", c_str);
    auto tokens = preprocessor.process_and_lex();
    Cpp::Parser parser(tokens, "main.cc");
    return 0;
}