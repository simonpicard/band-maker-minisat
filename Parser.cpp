//
//  Parser.cpp
//  InfoFond
//
//  Created by Nicolas Omer on 14/05/14.
//  Copyright (c) 2014 Nicolas Omer. All rights reserved.
//

#include "Parser.h"

std::vector<std::vector<int> > parse (std::string filename){
    std::ifstream file;
    std::string line;
    std::vector<int> header;
    std::vector<int> temp;
    std::vector<std::vector<int> > matrix;
    const char * c = filename.c_str();

    file.open(c);

    if(file.is_open()){
        int tmp = 0;

        getline(file, line);

        for(int i = 0; i < line.size()-1; ++i){
            if (line[i] != ' '){
                tmp = tmp*10 + atoi(&line[i]);
            }
            else{
                header.push_back(tmp);
                tmp = 0;
            }
        }
        header.push_back(tmp);
        tmp = 0;

        matrix.push_back(header);

        while (getline(file, line)) {
            for(int j = 0; j < line.size()-1; ++j){
                if (line[j] == ' '){
                    temp.push_back(tmp);
                    tmp = 0;
                }

                else{
                    tmp = tmp*10 + atoi(&line[j]);
                }
            }
            temp.push_back(tmp);
            tmp = 0;
            matrix.push_back(temp);
            temp.clear();
        }
    }
    return matrix;
}
