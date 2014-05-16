//
//  main.cpp
//  InfoFond
//
//  Created by Nicolas Omer on 14/05/14.
//  Copyright (c) 2014 Nicolas Omer. All rights reserved.
//

#include <iostream>
#include "Parser.h"
#include "Problem3.h"
#include <vector>
#include <fstream>

int main(int argc, const char * argv[])
{


    std::vector<std::vector<int> > matrix;
    matrix = parse("D:/Users/Simon/Desktop/Projet Git/infofon/exemples/Q3/exemple1.txt");

    std::cout<<"Done parsing"<<std::endl;

    main3(matrix);
    /*
    for (int i = 0 ; i < matrix.size() ; i++){
        for(int j = 0 ; j < matrix[i].size() ; ++j){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    */
    return 0;
}

void writeInFile(std::string filename, std::string data){
    
    std::ofstream fichier(filename, std::ios::out | std::ios::app);
    
    if(fichier)
    {
        fichier << data;
        
        fichier.close();
    }
    else
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}