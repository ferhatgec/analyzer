// MIT License
//
// Copyright (c) 2023 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//

#include "../include/enigne_analyzer.hpp"
#include "../include/locale/en.hpp"
#include "../libs/idkformat/include/idk.hpp"

#include <fstream>
#include <iostream>

#define __LOCATION__ " \x1b[0;36m" << node->row << "\x1b[0;97m:" << "\x1b[0;93m" << node->column 

void analyzer::load(const std::string& file) noexcept {
    this->file_name = file;

    std::cout << "\x1b[0;92mGetting LANG environment value\x1b[0m\n";
    std::string env = std::string(std::getenv("LANG"));
    
    std::cout << "\x1b[0;92mLocale set to locale_en\x1b[0m\n";
    this->arr = locale_en;

    std::string file_data = "";

    std::cout << "\x1b[0;92mReading file contents\x1b[0m\n";

    helpers::read_file(file, file_data);

    this->main.syntax.raw_file_data = file_data;
    this->main.syntax.tokenize();
    
    this->main.file(this->main.syntax.raw_file_data);
    this->main.parser.parse(this->main.syntax);

    enignelang_intptr data(this->main.parser.ast_main);
    
    data.callback_signal = [&](enignelang_syntax::enignelang_tokens syn, 
                                enignelang_ast* node) {
        this->read(data, syn, node);
    };

    data.parser = this->main.parser;
    data.walk(main.parser.ast_main,
                main.parser.ast_main,
                enignelang_syntax::GlobalNode,
                main.parser.ast_main->func_args);

    std::cout << "\x1b[0;96mRunning the interpreter\x1b[0m\n";
    
    data.start();

    if(!this->no_shell)
        this->shell(data);
}


void analyzer::read(enignelang_intptr& data,
                    enignelang_syntax::enignelang_tokens syn, 
                    enignelang_ast* node) {
    switch(syn) {
        case enignelang_syntax::Print:
        case enignelang_syntax::Exec:
        case enignelang_syntax::IsFile:
        case enignelang_syntax::IsDir: 
        case enignelang_syntax::IsSymlink:
        case enignelang_syntax::PathExists:
        case enignelang_syntax::ReadFile:
        case enignelang_syntax::Length:
        case enignelang_syntax::Absolute:
        case enignelang_syntax::Ceil:
        case enignelang_syntax::Floor:
        case enignelang_syntax::Logarithm:
        case enignelang_syntax::SquareRoot:
        case enignelang_syntax::Pi:
        case enignelang_syntax::Euler:
        case enignelang_syntax::StartsWith:
        case enignelang_syntax::EndsWith:
        case enignelang_syntax::ToUpper:
        case enignelang_syntax::ToLower:
        case enignelang_syntax::CharInput: {
            std::cout << "\x1b[0;94m" << idk::format(this->arr[BuiltInCall], node->name) 
                << __LOCATION__ << "\x1b[0m" << '\n';

            for(auto& val: node->other) {
                this->read(data, val->node_type, val);
            }

            break;
        }
        
        case enignelang_syntax::Return: {
            std::cout << "\x1b[0;94m" << idk::format(this->arr[ReturnWith], 
                ((!node->other.empty()) ? (data.handle_expr(node->other.back()))
                                        : "[empty]")) << __LOCATION__ << "\x1b[0m" << '\n';

            break;
        }

        case enignelang_syntax::FunctionCall: {
            std::cout << "\x1b[0;94m" << idk::format(this->arr[UserDefinedCall], node->name) 
                << __LOCATION__ << "\x1b[0m" << '\n';
            
            break;
        }

        case enignelang_syntax::If:
        case enignelang_syntax::Elif:
        case enignelang_syntax::LoopIf:
        case enignelang_syntax::LoopElif: {
            std::cout << "\x1b[0;94m" << idk::format(this->arr[IfElifLoopIfLoopElifCheck],
                                        node->node_current,
                                        data.handle_expr(node->node_l),
                                        data.handle_expr(node->node_r)) 
                                        << __LOCATION__ << "\x1b[0m" << '\n';

            break;
        }

        case enignelang_syntax::Else: {
            std::cout << "\x1b[0;94m" << this->arr[ElseInterpret] 
                << __LOCATION__ << "\x1b[0m" << '\n';
            
            break;
        }
        
        case enignelang_syntax::Exit: {
            std::cout << "\x1b[0;94m" << 
                idk::format(this->arr[ExitWith], ((node->other.empty()) ? "0" : "1")) 
                << __LOCATION__ << "\x1b[0m" << '\n';

            break;
        }

        case enignelang_syntax::Variant: {
            std::cout << "\x1b[0;94m" << idk::format(this->arr[InitializeVariant], node->name) 
                << __LOCATION__ << "\x1b[0m" << '\n';

            break;
        }

        case enignelang_syntax::LeftBPr: {
            std::cout << "\x1b[0;94m" << this->arr[DefineArray] 
                << __LOCATION__ << "\x1b[0m" << '\n';
            
            break;
        }

        case enignelang_syntax::Element: {
            std::cout << "\x1b[0;94m" << idk::format(this->arr[AccessWithIndexN], 
                                        node->name, 
                                        ((node->other.empty()) 
                                            ? "0" 
                                            : data.handle_expr(node->other.back()))) 
                                            << __LOCATION__ << "\x1b[0m" << '\n';
          
            break;
        }

        case enignelang_syntax::Plus:
        case enignelang_syntax::Minus: 
        case enignelang_syntax::Div:
        case enignelang_syntax::Ast:
        case enignelang_syntax::Mod: {
            std::string ch;

            switch(syn) {
                case enignelang_syntax::Plus: { ch = "+"; break; }
                case enignelang_syntax::Minus: { ch = "-"; break; }
                case enignelang_syntax::Div: { ch = "/"; break; }
                case enignelang_syntax::Ast: { ch = "*"; break; }
                case enignelang_syntax::Mod: { ch = "%"; break; }
            }

            std::cout << "\x1b[0;94m" << idk::format(this->arr[EvaluateExpr], 
                                                        data.handle_expr(node->node_l), 
                                                        ch, 
                                                        data.handle_expr(node->node_r)) 
                                                        << __LOCATION__ << "\x1b[0m" << '\n';

            break;
        }
    }
}

void analyzer::shell(enignelang_intptr& data) {
    std::cout << "Write 'help' to list commands\n";

    std::string command = "";
    bool is_modified = false;

    while(command != "exit") {
        std::cout << "(" << this->file_name << ", " << std::boolalpha << is_modified << ") -> ";
        std::cin >> command;
        
        if(command == "list_functions") {
            for(auto& val: this->main.parser.ast_main->other) {
                if(val->node_type == enignelang_syntax::Function) {
                    std::cout << val->name << "\n";
                }
            }
        } else if(command == "list_variants") {
            for(auto& val: data.global_variants) {
                std::cout << val->name << "\n";
            }
        } else if(command == "reload") {
            this->load(this->file_name);
        } else if(command == "variant_data") {
            std::string variant_name;
            std::cout << "variant: ";
            std::cin >> variant_name;

            for(auto& val: data.global_variants) {
                if(val->name == variant_name) {
                    if(!val->other.empty())
                        std::cout << data.handle_expr(val->other.back()) << '\n';
                    else std::cout << "[empty]\n";

                    break;
                }
            }

        } else if(command == "help") {
            std::cout << "list_functions: lists user defined functions\n" 
                      << "list_variants: lists pre-/user defined variants\n"
                      << "reload: reload the file\n"
                      << "variant_data: get data of variant\n";

        }
    }
}