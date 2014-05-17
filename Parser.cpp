//
//  Parser.cpp
//  InfoFond
//
//  Created by Simon Picard, Nicolas Omer
//  Copyright (c) 2014 Simon Picard, Nicolas Omer. All rights reserved.
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
                tmp *= 10;
                tmp += int(line[i])-48;
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
                    tmp *= 10;
                    tmp += int(line[j])-48;
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
