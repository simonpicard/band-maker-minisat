//
//  Problem2.h
//  InfoFond
//
//  Created by Simon Picard, Nicolas Omer
//  Copyright (c) 2014 Simon Picard, Nicolas Omer. All rights reserved.
//

#ifndef __InfoFond__Problem2__
#define __InfoFond__Problem2__

#include "Solver.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

int propP2(int, int, int);
int mainP2(std::vector<std::vector<int> > );
void setConstraintP2();
void displayP2();
bool canPlayP2(int, int);
void setConstraintGroupeP2(int, int, int, int, std::vector<int>);
void setConstraintGroupeTerP2(int, int, int, std::vector<int>, std::vector<int>);

#endif /* defined(__InfoFond__Problem2__) */
