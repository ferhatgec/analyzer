// MIT License
//
// Copyright (c) 2023 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//

#include "../include/enigne_analyzer.hpp"
#include <cstring>

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cout << "enigne::analyzer [arg] {file}\n"
                  << "[arg] = \n" 
                  << "-no_shell : disable shell\n";
        return 1;
    }

    analyzer val;
    
    if(argv[1][0] == '-' && (::strcmp(argv[1], "-no_shell") == 0)) {
        val.no_shell = true;

        if(argc < 3) {
            std::cout << "file input not specified\n";
            return 1;
        }

        const std::string file(argv[2]);
        val.load(file);
        return 0;
    }

    const std::string file(argv[1]);
    val.load(file);
    
    return 0;
}