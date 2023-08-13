#include "cpp_parser/parser.hh"

auto c_str = "int main(int, char**) {return 0;}";

int main(int argc, char* argv[]) {
    std::vector<std::string> arguments;
    for(std::size_t i = 0; i < argc; ++i) {
        arguments.push_back(argv[i]);
    }

    if(arguments.size() < 2) {
        outln("Usage: entity-precompiler <dst-file> <src-file1> [src-file2] ...");
        outln("Error: At least two arguments required.");
        return -1;
    }

    Cpp::Preprocessor preprocessor("main.cc", c_str);
    auto tokens = preprocessor.process_and_lex();
    Cpp::Parser parser(tokens, "main.cc");
    return 0;
}