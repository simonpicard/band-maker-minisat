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
#include <fstream>

int main(int argc, const char * argv[])
{
    std::vector<std::vector<int> > matrix;

    // à adapter pour le path absolu
    matrix = parse("D:/Users/Simon/Desktop/Projet Git/infofon/exemples/Q2/exemple3.txt");
    std::cout << "Done parsing" << std::endl;

    mainP2(matrix);

    return 0;
}
void writeInFile(std::string filename, std::string data){

    std::ofstream fichier;
    fichier.open(filename.c_str(), std::ios::out | std::ios::app);

    if(fichier)
    {
        fichier << data;

        fichier.close();
    }
    else
        std::cerr << "Impossible d'ouvrir le fichier" << std::endl;
}

