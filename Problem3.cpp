//
//  Problem3.cpp
//  InfoFond
//
//  Created by Simon Picard, Nicolas Omer
//  Copyright (c) 2014 Simon Picard, Nicolas Omer. All rights reserved.
//

#include "Problem3.h"

#define FOR(k, lb, ub) for (int k = (lb) ; (k) <= (ub) ; (k)++)

int I3, K3, M3, G3;
Solver S3;
std::vector<std::vector<int> > matrix3;

int propP3(int a, int b, int c){
    return ( ( (a - 1) * G3 + (b - 1) ) * G3 + c);
}

int mainP3(std::vector<std::vector<int> > mat, std::string filename){
    
    matrix3 = mat;
    M3 = matrix3[0][0];
    I3 = matrix3[0][1];
    K3 = matrix3[0][2];
    G3 = max(I3, max(K3, M3));
    FOR(j, 1, (propP3(M3, I3, K3) + 1)){
        S3.newVar();}
    
    setConstraintP3();
    S3.solve();
    displayP3(filename);
    
    return 0;
}

void displayP3(std::string filename){
    if (S3.okay()){
        FOR (c, 1, K3){
            //std::cout << "Groupe " << c << " : ";
            writeInFile(filename, "groupe ");
            writeInFile(filename, c);
            writeInFile(filename, ": ");
            FOR (b, 1, I3){
                FOR (a, 1, M3){
                    if (S3.model[propP3(a, b, c)] == l_True){
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

bool canPlayP3(int a, int b){
    FOR(i, 2, matrix3[a].size() - 1){
        if (matrix3[a][i] == b){
            return true;
        }
    }
    return false;
}

void setConstraintP3(){
    
    vec<Lit> lits;
    // Contrainte d'existence
    FOR(a, 1, M3){
        lits.clear();
        FOR(b, 2, matrix3[a].size() - 1){
            FOR(c, 1, K3){
                lits.push(Lit(propP3(a, matrix3[a][b], c)));
            }
        }
        S3.addClause(lits);
    }
    
    std::vector<int> cvec;
    FOR(a, 1, M3){
        cvec.clear();
        FOR(c, 1, matrix3[a][1] + 1){
            cvec.push_back(c);
        }
        setConstraintGroupeP3(a, matrix3[a][1], 0, 1, cvec);
    }
    
    FOR(a, 1, M3){
        FOR(c, 1, K3){
            FOR(b1, 2, matrix3[a].size() - 1){
                FOR(b2, b1 + 1, matrix3[a].size() - 1){
                    // Contrainte ¨ 2 instruments dans le même groupe
                    if(matrix3[a][b1] != I3 && matrix3[a][b2] != I3){
                        S3.addBinary(~Lit(propP3(a, matrix3[a][b1], c)), ~Lit(propP3(a, matrix3[a][b2], c)));
                    }
                }
            }
        }
    }
    
    FOR(c, 1, K3){
        FOR(a1, 1, M3){
            FOR(b1, 2, matrix3[a1].size() - 1){
                FOR(b2, 1, I3){
                    if (matrix3[a1][b1] != b2){
                        lits.clear();
                        lits.push(~Lit(propP3(a1, matrix3[a1][b1], c)));
                        FOR(a2, 1, M3){
                            // Contrainte tout instrument ou aucun dans chaque groupe
                            if (canPlayP3(a2, b2)){
                                if(a1 != a2 && matrix3[a1][b1] != I3){
                                    lits.push(Lit(propP3(a2, b2, c)));
                                }
                                else if(matrix3[a1][b1] == I3){
                                    lits.push(Lit(propP3(a2, b2, c)));
                                }
                                else if(a1 == a2 && b2 == I3){
                                    lits.push(Lit(propP3(a2, b2, c)));
                                }
                            }
                        }
                        S3.addClause(lits);
                    }
                }
            }
        }
    }
    
    FOR(c, 1, K3){
        FOR(a1, 1, M3){
            FOR(a2, a1 + 1, M3){
                FOR(b, 2, matrix3[a1].size() - 1){
                    // Contrainte pas deux musiciens qui jouent d'un même instrument dans un même groupe
                    if (canPlayP3(a2, matrix3[a1][b]) && matrix3[a1][b] != I3)
                    {
                        S3.addBinary(~Lit(propP3(a1, matrix3[a1][b], c)), ~Lit(propP3(a2, matrix3[a1][b], c)));
                    }
                }
            }
        }
    }
}

void setConstraintGroupeP3(int a, int deep, int current, int beginvar, std::vector<int> c){
    if (current > deep)
    {
        // C'est ici un ensemble ge groupe de la taille Max(a)+1
        // On commence par générer toutes les combinaisons, instrument/groupe parmi cet ensemble
        std::vector<std::vector<int> > lits;
        std::vector<int> tmp;
        FOR(k, 0, deep){
            FOR(b, 2, matrix3[a].size() - 1){
                tmp.clear();
                tmp.push_back(a);
                tmp.push_back(matrix3[a][b]);
                tmp.push_back(c[k]);
                lits.push_back(tmp);
            }
        }
        std::vector<int> listClause;
        FOR(i, 0, deep){
            listClause.push_back(i);
        }
        // ensuite on choisit toutes les différentes Max(a)+1 combinaisons possibles et on fait un clause Not (GxIx And GyIy ...)
        setConstraintGroupeTerP3(deep, 0, 1, lits, listClause);
        return;
    }
    FOR(i, beginvar, K3){
        setConstraintGroupeP3(a, deep, current + 1, i + 1, c);
        FOR (j, current, deep){
            c[j] += 1;
        }
    }
}

void setConstraintGroupeTerP3(int deep, int current, int beginvar, std::vector<std::vector<int> > litsProp, std::vector<int> listClause){
    if (current > deep)
    {
        vec<Lit> tmp;
        std::vector<int> c;
        FOR(i, 0, deep){
            c.push_back(litsProp[listClause[i]][2]);
        }
        FOR(i, 0, deep){
            if (countVecP3(c[i], c) > 1){
                return;
                // en fait dans le 2 et le 1 cette condition était double, elle vérifait non seulement qu'un joueur ne pouvait pas être dans deux groupe
                // mais aussi qu'il ne jouait pas plus d'un instrument pas groupe, ici sous certaine condition il a le droit, nous préférons alors ne plus vérifier Áa en même temps
            }
        }
        FOR(i, 0, deep){
            tmp.push(~Lit(propP3(litsProp[listClause[i]][0], litsProp[listClause[i]][1], litsProp[listClause[i]][2])));
        }
        S3.addClause(tmp);
        
        return;
    }
    FOR(i, beginvar, litsProp.size()){
        setConstraintGroupeTerP3(deep, current + 1, i + 1, litsProp, listClause);
        FOR (j, current, deep){
            listClause[j] += 1;
        }
    }
}

int countVecP3(int a, std::vector<int> c){
    int res = 0;
    FOR(i, 0, c.size() - 1){
        if (c[i] == a){
            res += 1;
        }
    }
    return res;
}