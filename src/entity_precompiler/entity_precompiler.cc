#include <iostream>
#include <fstream>
#include <map>
#include "cpp_parser/parser.hh"
#include "cpp_parser/util.hh"
#include "cpp_parser/traverse_ast.hh"

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

    std::ifstream input_file(input_file_path);
    if(!input_file.is_open()) {
        outln("Unable to open {}", input_file_path);
        return -1;
    }

    // Load the text
    std::string input_file_contents((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();

    Cpp::Preprocessor preprocessor(input_file_path, input_file_contents);
    auto tokens = preprocessor.process_and_lex();
    Cpp::Parser parser(tokens, input_file_path);

    struct EntityDefinition {
        std::string name;
    };

    using NamespaceDefinition = std::string;

    std::multimap<NamespaceDefinition, EntityDefinition> entity_definitions;

    NamespaceDefinition current_namespace;

    auto translation_unit = parser.parse();
    traverse_ast_tree(translation_unit, [&current_namespace, &entity_definitions] (Cpp::AstNodePtr node_ptr) {
        std::visit(Cpp::overloaded{
                [&current_namespace, &entity_definitions](Cpp::StructOrClassDeclaration const* soc) {
                    outln("StructOrClass {}", soc->full_name());
                    entity_definitions.emplace(current_namespace, EntityDefinition{std::string(soc->full_name())});
                    },
                [&current_namespace](Cpp::NamespaceDeclaration const* nd) {
                    outln("Namespace {}", nd->full_name());
                    current_namespace = nd->full_name();
                    },
                [](auto const* node) {
                }
        }, node_ptr);
    });

    outln("Num entities: {}", entity_definitions.size());

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