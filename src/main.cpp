//
//  main.cpp
//  InfoFond
//
//  Created by Simon Picard, Nicolas Omer
//  Copyright (c) 2014 Simon Picard, Nicolas Omer. All rights reserved.
//

#include <iostream>
#include "Parser.h"
#include "Problem.h"
#include "Problem2.h"
#include "Problem3.h"
#include <vector>

int main(int argc, const char * argv[]){
    if (argc != 4){
        std::cout << "Utilisation : ./Question question_i fichier_d'entrée fichier_de_sortie" << std::endl;
    }
    
    else{
        
        std::vector<std::vector<int> > matrix;
        std::string question = argv[1];
        std::string output = argv[3];
        matrix = parse(argv[2]);
        
        //std::cout << "Done parsing" << std::endl;
        
        char number = question[question.size() - 1];
        
        if(number == '1'){
            mainP1(matrix, output);
        }
        else if(number == '2'){
            mainP2(matrix, output);
        }
        else if(number == '3'){
            mainP3(matrix, output);
        }
        else{
            std::cout << "Utilisation : ./Question question_i fichier_d'entrée fichier_de_sortie" << std::endl;
        }
    }
    
    return 0;
}


