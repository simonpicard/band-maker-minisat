//
//  Problem.cpp
//  InfoFond
//
//  Created by Nicolas Omer on 14/05/14.
//  Copyright (c) 2014 Nicolas Omer. All rights reserved.
//

#include "Problem2.h"

#define FOR(k, lb, ub) for (int k = (lb) ; (k) <= (ub) ; (k)++)

int I, K, M, g;
Solver s;
std::vector<std::vector<int> > matrix;

int prop (int a, int b, int c){
    return ( ( (a - 1) * g + (b - 1) ) * g + c);
}

int main2(std::vector<std::vector<int> > mat){

    matrix = mat;
    M = matrix[0][0];
    I = matrix[0][1];
    K = matrix[0][2];
    g = max(I, max(K, M));
    FOR(j, 1, (prop(M, I, K)+1)){
        s.newVar();}

    setConstraint();
    s.solve();
    display();

    return 0;
}

void display(){
    if (s.okay()){
        std::cout << "Satifaisable\n"<<std::endl;
        FOR (c, 1, K){
            std::cout << "Groupe "<<c<<": ";
            FOR (b, 1, I){
                FOR (a, 1, M){
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
    FOR(i, 2, matrix[a].size() - 1){
        if (matrix[a][i] == b){
            return true;
        }
    }
    return false;
}

void setConstraint(){

    vec<Lit> lits;
    // Contrainte d'existence
    FOR(a, 1, M){
        lits.clear();
        FOR(b, 2, matrix[a].size() - 1){
            FOR(c, 1, K){
                lits.push(Lit(prop(a, matrix[a][b], c)));
            }
        }
        s.addClause(lits);
    }
    std::cout<<"Done existence"<<std::endl;

    std::vector<int> cvec;
    FOR(a, 1, M){
        cvec.clear();
        FOR(c, 1, matrix[a][1]+1){
            cvec.push_back(c);
            //std::cout<<c<<std::endl;
        }
        setConstraintGroupe(a, matrix[a][1], 0, 1, cvec);
    }
    std::cout<<"Done 1 joueur par groupes ET 1 instru par groupe"<<std::endl;

    /*FOR(a, 1, M){
        FOR(c, 1, K){
            FOR(b1, 2, matrix[a].size()-1){
                FOR(b2, b1+1, matrix[a].size()-1){
                    // Contrainte ¬ 2 instruments dans le même groupe
                    //s.addBinary(~Lit(prop(a, matrix[a][b1], c)), ~Lit(prop(a, matrix[a][b2], c)));
                    //la fct set groupe vérifie deja ceci
                }
            }
        }
    }
    std::cout<<"Done 1 instru 1 groupe"<<std::endl;*/

    FOR(c, 1, K){
        FOR(a1, 1, M){
            FOR(b1, 2, matrix[a1].size()-1){
                FOR(b2, 1, I){
                    if (matrix[a1][b1] != b2){
                        lits.clear();
                        //std::cout<<"("<<a1<<", "<<matrix[a1][b1]<<", "<<c<<") -> ( ";
                        lits.push(~Lit(prop(a1, matrix[a1][b1], c)));
                        FOR(a2, 1, M){
                            // Contrainte tout instrument ou aucun dans chaque groupe
                            if(a1 != a2 && canPlay(a2,b2)){
                                lits.push(Lit(prop(a2, b2, c)));
                                //std::cout<<"("<<a2<<", "<<b2<<", "<<c<<") \\/ ";
                            }
                        }
                        //std::cout<<")"<<std::endl;
                        s.addClause(lits);
                    }
                }
            }
        }
    }
    std::cout<<"Done tout ou aucun instru un groupe"<<std::endl;

    FOR(c, 1, K){
        FOR(a1, 1, M){
            FOR(a2, a1+1, M){
                FOR(b, 2, matrix[a1].size()-1){
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
    std::cout<<"Done 1 instru par musicien par groupe"<<std::endl;
}

void setConstraintGroupe(int a, int deep, int current, int beginvar, std::vector<int> c){
    //if c[0] != k-(c.size()-1)
    if (current>deep)
    {
        //c est ici un ensemble ge groupe de la taille Max(a)+1
        //On commence par générer toute les combinaison, instrument/groupe parmis cet esemble
        std::vector<int> lits;
        FOR(k, 0, deep){
            FOR(b, 2, matrix[a].size() - 1){
                lits.push_back(prop(a, matrix[a][b], c[k]));
            }
        }
        std::vector<int> listClause;
        FOR(i, 0, deep){
            listClause.push_back(i);
        }
        //ensuite on choisi toute les différente Max(a)+1 combinaison possible et on fait un clause Not (GxIx And GyIy ...)
        setConstraintGroupeTer(deep, 0, 1, lits, listClause);
        return;
    }
    FOR(i, beginvar, K){
        setConstraintGroupe(a, deep, current+1, i+1, c);
        FOR (j, current, deep){
            c[j] += 1;
        }
    }
}

void setConstraintGroupeTer(int deep, int current, int beginvar, std::vector<int> litsProp, std::vector<int> listClause){
    if (current>deep)
    {

        vec<Lit> tmp;
        FOR(i, 0, deep){
            tmp.push(~Lit(litsProp[listClause[i]]));
        }
        s.addClause(tmp);

        return;
    }
    FOR(i, beginvar, litsProp.size()){
        setConstraintGroupeTer(deep, current+1, i+1, litsProp, listClause);
        FOR (j, current, deep){
            listClause[j] += 1;
        }
    }
}
