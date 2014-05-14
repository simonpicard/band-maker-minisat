//
//  Problem.cpp
//  InfoFond
//
//  Created by Nicolas Omer on 14/05/14.
//  Copyright (c) 2014 Nicolas Omer. All rights reserved.
//

#include "Problem.h"

#define FOR(k, lb, ub) for (int k = (lb) ; (k) <= (ub) ; (k)++)

int i, k, m, g;
Solver s;
std::vector<std::vector<int> > matrix;

int prop (int a, int b, int c){
    return ( ( (a - 1) * g + (b - 1) ) * g + c);
}

int mainP(std::vector<std::vector<int> > mat){

    matrix = mat;
    m = matrix[0][0];
    i = matrix[0][1];
    k = matrix[0][2];
    g = max(i, max(k, m));
    FOR(j, 1, (prop(m, i, k))){
        s.newVar();}
    setConstraint();
    std::cout << s.nClauses();
    s.solve();
    display();
    
    return 0;
}

void display(){
    if (s.okay()){
        std::cout << "Satifaisable";
    }
    else{
        std::cout << "Non Satifaisable";
    }
}

bool canPlay(int a, int b){
    
    return (std::find(matrix[a].begin(), matrix[a].end(), b) != matrix[a].end());
}

void setConstraint(){

    vec<Lit> lits;
    // Contrainte d'existence
    FOR(a, 1, m){
        lits.clear();
        FOR(b, 1, matrix[a].size() - 1){
            FOR(c, 1, k){
                lits.push(Lit(prop(a, matrix[a][b], c)));
            }
        }
        s.addClause(lits);
    }
    std::cout<<"Done existance"<<std::endl;
    FOR(a, 1, m){
        FOR(b, 1, matrix[a].size() - 1){
            FOR(c1, 1, k){
                FOR(c2, c1+1, k){
                    // Contrainte 1 joueur dans un seul groupe
                    s.addBinary(~Lit(prop(a, matrix[a][b], c1)), ~Lit(prop(a, matrix[a][b], c2)));
                }
            }
        }
    }
    std::cout<<"Done 1 joueur 1 groupe"<<std::endl;
    FOR(a, 1, m){
        FOR(c, 1, k){
            FOR(b1, 1, matrix[a].size()-1){
                FOR(b2, b1+1, matrix[a].size()-1){
                    // Contrainte ¬ 2 instruments dans le même groupe
                    s.addBinary(~Lit(prop(a, matrix[a][b1], c)), ~Lit(prop(a, matrix[a][b2], c)));
                }
            }
        }
    }
    std::cout<<"Done 1 instru 1 groupe"<<std::endl;
    
    FOR(a, 1, m){
        FOR(c, 1, k){
            FOR(b1, 1, matrix[a].size()-1){
                FOR(b2, 1, matrix[a].size()-1){
                    // Contrainte tout instrument ou aucun dans chaque groupe
                    if(b1 != b2){
                        s.addBinary(~Lit(prop(a, matrix[a][b1], c)), Lit(prop(a, matrix[a][b2], c)));
                    }
                }
            }
        }
    }
    std::cout<<"Done tout ou aucun instru un groupe"<<std::endl;
    FOR(c, 1, k){
        FOR(a1, 1, m){
            FOR(a2, 1, m){
                FOR(b, 1, matrix[a1].size()-1){
                    // Contrainte pas deux musiciens qui jouent d'un même instrument dans un même groupe
                    if (canPlay(a2, matrix[a1][b]))
                    {
                        s.addBinary(~Lit(prop(a1, matrix[a1][b], c)), ~Lit(prop(a2, matrix[a1][b], c)));
                    }
                }
            }
        }
    }
    std::cout<<"Done 1 instru/musicine 1 groupe"<<std::endl;
}