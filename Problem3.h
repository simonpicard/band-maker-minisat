//
//  Problem3.h
//  InfoFond
//
//  Created by Simon Picard, Nicolas Omer
//  Copyright (c) 2014 Simon Picard, Nicolas Omer. All rights reserved.
//

#ifndef __InfoFond__Problem3__
#define __InfoFond__Problem3__

#include "Solver.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

int propP3(int, int, int);
int mainP3(std::vector<std::vector<int> > );
void setConstraintP3();
void displayP3();
bool canPlayP3(int, int);
void setConstraintGroupeP3(int, int, int, int, std::vector<int>);
void setConstraintGroupeTerP3(int, int, int, std::vector<std::vector<int> >, std::vector<int>);
int countVecP3(int, std::vector<int>);

#endif /* defined(__InfoFond__Problem3__) */
