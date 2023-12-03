#include <iostream>
#include <fstream>
#include "cpp_parser/parser.hh"

auto c_str = "int main(int, char**) {return 0;}";

int main(int argc, char* argv[]) {
    std::vector<std::string> arguments;
    for(std::size_t i = 0; i < argc; ++i) {
        arguments.push_back(argv[i]);
    }

    if(arguments.size() < 3) {
        outln("Usage: entity-precompiler <dst-file> <src-file1> [src-file2] ...");
        outln("Error: At least two arguments required.");
        return -1;
    }

    auto input_file_path = arguments[1];
    auto output_file_path = arguments[2];

    outln("arguments: {}\ninput: {}\noutput: {}", arguments.size(), input_file_path, output_file_path);

    Cpp::Preprocessor preprocessor(input_file_path, c_str);
    auto tokens = preprocessor.process_and_lex();
    Cpp::Parser parser(tokens, input_file_path);

    std::ofstream output(output_file_path);
    if(output.is_open()) {
        output << "success\n";
        output.close();
        outln("entity-precompiler compeleted successfully!");
    }
    else
        outln("Unable to open {} for output.", output_file_path);
    return 0;
}