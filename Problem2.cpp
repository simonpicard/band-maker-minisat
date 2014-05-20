//
//  Problem2.cpp
//  InfoFond
//
//  Created by Simon Picard, Nicolas Omer
//  Copyright (c) 2014 Simon Picard, Nicolas Omer. All rights reserved.
//

#include "Problem2.h"

#define FOR(k, lb, ub) for (int k = (lb) ; (k) <= (ub) ; (k)++)

int I2, K2, M2, G2;
Solver S2;
std::vector<std::vector<int> > matrix2;

int propP2(int a, int b, int c){
    return ( ( (a - 1) * G2 + (b - 1) ) * G2 + c);
}

int mainP2(std::vector<std::vector<int> > mat, std::string filename){
    
    matrix2 = mat;
    M2 = matrix2[0][0];
    I2 = matrix2[0][1];
    K2 = matrix2[0][2];
    G2 = max(I2, max(K2, M2));
    FOR(j, 1, (propP2(M2, I2, K2) + 1)){
        S2.newVar();}
    
    setConstraintP2();
    S2.solve();
    displayP2(filename);
    
    return 0;
}

void displayP2(std::string filename){
    if (S2.okay()){
        //std::cout << "Satifaisable\n" << std::endl;
        FOR (c, 1, K2){
            //std::cout << "Groupe " << c << ": ";
            writeInFile(filename, "groupe ");
            writeInFile(filename, c);
            writeInFile(filename, ": ");
            FOR (b, 1, I2){
                FOR (a, 1, M2){
                    if (S2.model[propP2(a, b, c)] == l_True){
                        //std::cout << a << " ";
                        writeInFile(filename, a);
                        writeInFile(filename, " ");
                    }
                }
            }
            //std::cout <<std::endl;
            writeInFile(filename, "\n");
        }
    }
    else{
        //std::cout << "Non Satifaisable";
        writeInFile(filename, "impossible");
    }
}

bool canPlayP2(int a, int b){
    FOR(i, 2, matrix2[a].size() - 1){
        if (matrix2[a][i] == b){
            return true;
        }
    }
    return false;
}

void setConstraintP2(){
    
    vec<Lit> lits;
    // Contrainte d'existence
    FOR(a, 1, M2){
        lits.clear();
        FOR(b, 2, matrix2[a].size() - 1){
            FOR(c, 1, K2){
                lits.push(Lit(propP2(a, matrix2[a][b], c)));
            }
        }
        S2.addClause(lits);
    }
    
    std::vector<int> cvec;
    FOR(a, 1, M2){
        cvec.clear();
        FOR(c, 1, matrix2[a][1] + 1){
            cvec.push_back(c);
        }
        setConstraintGroupeP2(a, matrix2[a][1], 0, 1, cvec);
    }
    
    FOR(c, 1, K2){
        FOR(a1, 1, M2){
            FOR(b1, 2, matrix2[a1].size() - 1){
                FOR(b2, 1, I2){
                    if (matrix2[a1][b1] != b2){
                        lits.clear();
                        lits.push(~Lit(propP2(a1, matrix2[a1][b1], c)));
                        FOR(a2, 1, M2){
                            // Contrainte tout instrument ou aucun dans chaque groupe
                            if(a1 != a2 && canPlayP2(a2, b2)){
                                lits.push(Lit(propP2(a2, b2, c)));
                            }
                        }
                        S2.addClause(lits);
                    }
                }
            }
        }
    }
    
    FOR(c, 1, K2){
        FOR(a1, 1, M2){
            FOR(a2, a1 + 1, M2){
                FOR(b, 2, matrix2[a1].size() - 1){
                    // Contrainte pas deux musiciens qui jouent d'un mÍme instrument dans un mÍme groupe
                    if (canPlayP2(a2, matrix2[a1][b]))
                    {
                        S2.addBinary(~Lit(propP2(a1, matrix2[a1][b], c)), ~Lit(propP2(a2, matrix2[a1][b], c)));
                    }
                }
            }
        }
    }
}

void setConstraintGroupeP2(int a, int deep, int current, int beginvar, std::vector<int> c){
    //if c[0] != k-(c.size()-1)
    if (current > deep)
    {
        // C'est ici un ensemble ge groupe de la taille Max(a)+1
        // On commence par générer toutes les combinaisons, instrument/groupe parmi cet ensemble
        std::vector<int> lits;
        FOR(k, 0, deep){
            FOR(b, 2, matrix2[a].size() - 1){
                lits.push_back(propP2(a, matrix2[a][b], c[k]));
            }
        }
        std::vector<int> listClause;
        FOR(i, 0, deep){
            listClause.push_back(i);
        }
        // Ensuite on choisit toutes les différentes Max(a)+1 combinaisons possibles et on fait un clause Not (GxIx And GyIy ...)
        setConstraintGroupeTerP2(deep, 0, 1, lits, listClause);
        return;
    }
    FOR(i, beginvar, K2){
        setConstraintGroupeP2(a, deep, current + 1, i + 1, c);
        FOR (j, current, deep){
            c[j] += 1;
        }
    }
}

void setConstraintGroupeTerP2(int deep, int current, int beginvar, std::vector<int> litsProp, std::vector<int> listClause){
    if (current > deep)
    {
        vec<Lit> tmp;
        FOR(i, 0, deep){
            tmp.push(~Lit(litsProp[listClause[i]]));
        }
        S2.addClause(tmp);
        
        return;
    }
    FOR(i, beginvar, litsProp.size()){
        setConstraintGroupeTerP2(deep, current + 1, i + 1, litsProp, listClause);
        FOR (j, current, deep){
            listClause[j] += 1;
        }
    }
}
