//
//  Problem.h
//  InfoFond
//
//  Created by Nicolas Omer on 14/05/14.
//  Copyright (c) 2014 Nicolas Omer. All rights reserved.
//

#ifndef __InfoFond__Problem2__
#define __InfoFond__Problem2__
#include "Solver.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

int prop (int, int, int);
int main2(std::vector<std::vector<int> > );
void setConstraint();
void display();
void setConstraintGroupe(int a, int deep, int current, int beginvar, std::vector<int> c);
void setConstraintGroupeTer(int deep, int current, int beginvar, std::vector<int> litsProp, std::vector<int> listClause);

#endif /* defined(__InfoFond__Problem__) */
