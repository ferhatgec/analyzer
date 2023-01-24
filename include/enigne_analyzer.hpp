// MIT License
//
// Copyright (c) 2023 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//

#pragma once

#include <enigne/enigne_lang_main.hpp>
#include <enigne/enigne_lang_parse.hpp>
#include <enigne/enigne_lang_intptr.hpp>
#include "locale/locale.hpp"
#include <fstream>

class analyzer {
    enignelang main;
    std::string file_name;
    std::array<std::string, __LOCALE_SIZE__> arr;
public:
    bool no_shell = false;

    analyzer() = default;
    ~analyzer() = default;

    void load(const std::string& file) noexcept;
    void read(enignelang_intptr& data,
        enignelang_syntax::enignelang_tokens syn, enignelang_ast* node);
    
    void shell(enignelang_intptr& data);
};

namespace helpers {
    static inline 
    void read_file(const std::string& file_name, std::string& copy_to) noexcept {
        std::ifstream file(file_name);

        if(!file) {
            copy_to.clear();
            file.close();
            return;
        }

        for(std::string temp; std::getline(file, temp); 
            copy_to.append(temp + "\n"))
            ;
        
        file.close();
    }
}