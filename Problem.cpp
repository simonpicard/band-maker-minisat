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
    FOR(j, 1, (prop(m, i, k)+1)){
        s.newVar();}
    setConstraint();
    s.solve();
    display();

    return 0;
}

void display(){
    if (s.okay()){
        std::cout << "Satifaisable\n"<<std::endl;
        FOR (c, 1, k){
            std::cout << "Groupe "<<c<<": ";
            FOR (b, 1, i){
                FOR (a, 1, m){
                    if (s.model[prop(a,b,c)] == l_True){
                        std::cout <<a<<" ";
                    }
                }
            }
            std::cout <<std::endl;
        }
    }
    else{
        std::cout << "Non Satifaisable";
    }
}

bool canPlay(int a, int b){
    FOR(i, 1, matrix[a].size() - 1){
        if (matrix[a][i] == b){
            return true;
        }
    }
    return false;
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
        FOR(b1, 1, matrix[a].size() - 1){
            FOR(b2, 1, matrix[a].size() - 1){
                FOR(c1, 1, k){
                    FOR(c2, c1+1, k){
                        // Contrainte 1 joueur dans un seul groupe
                        s.addBinary(~Lit(prop(a, matrix[a][b1], c1)), ~Lit(prop(a, matrix[a][b2], c2)));
                    }
                }
            }
        }
    }
    std::cout<<"Done 1 joueur par groupe ET 1 instrument joué par groupe"<<std::endl;
    /*FOR(a, 1, m){
        FOR(c, 1, k){
            FOR(b1, 1, matrix[a].size()-1){
                FOR(b2, b1+1, matrix[a].size()-1){
                    // Contrainte ¬ 2 instruments dans le même groupe
                    //s.addBinary(~Lit(prop(a, matrix[a][b1], c)), ~Lit(prop(a, matrix[a][b2], c)));
                }
            }
        }
    }
    std::cout<<"Done 1 instru 1 groupe"<<std::endl;*/

    FOR(c, 1, k){
        FOR(a1, 1, m){
            FOR(b1, 1, matrix[a1].size()-1){
                FOR(b2, 1, i){
                    if (matrix[a1][b1] != b2){
                        lits.clear();
                        //std::cout<<"("<<a1<<", "<<matrix[a1][b1]<<", "<<c<<") -> ( ";
                        lits.push(~Lit(prop(a1, matrix[a1][b1], c)));
                        FOR(a2, 1, m){
                            // Contrainte tout instrument ou aucun dans chaque groupe
                            if(a1 != a2 && canPlay(a2,b2)){
                                lits.push(Lit(prop(a2, b2, c)));
                                //std::cout<<"("<<a2<<", "<<b2<<", "<<c<<") \\/ ";
                            }
                        }
                        //std::cout<<")"<<std::endl;
                        s.addClause(lits);
                    }
                    //std::cout<<") "<<std::endl;
                }
            }
        }
    }
    std::cout<<"Done tout ou aucun instru un groupe"<<std::endl;
    FOR(c, 1, k){
        FOR(a1, 1, m){
            FOR(a2, a1+1, m){
                FOR(b, 1, matrix[a1].size()-1){
                    // Contrainte pas deux musiciens qui jouent d'un même instrument dans un même groupe
                    if (canPlay(a2, matrix[a1][b]))
                    {
                        s.addBinary(~Lit(prop(a1, matrix[a1][b], c)), ~Lit(prop(a2, matrix[a1][b], c)));
                        //std::cout<<"¬(("<<a1<<", "<<matrix[a1][b]<<", "<<c<<") /\\ ("<<a2<<", "<<matrix[a1][b]<<", "<<c<<"))"<<std::endl;
                    }
                }
            }
        }
    }
    std::cout<<"Done 1 instru/musicien 1 groupe"<<std::endl;
}
