//
//  main.cpp
//  InfoFond
//
//  Created by Nicolas Omer on 14/05/14.
//  Copyright (c) 2014 Nicolas Omer. All rights reserved.
//

#include <iostream>
#include "Parser.h"
#include "Problem.h"
#include <vector>

int main(int argc, const char * argv[])
{
    
    std::vector<std::vector<int> > matrix;
    matrix = parse("/Users/Rapace/Desktop/InfoFond/InfoFond/exemple1.txt");
    
    std::cout<<"Done parsing"<<std::endl;
    
    mainP(matrix);
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