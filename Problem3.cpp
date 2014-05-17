//
//  Problem.cpp
//  InfoFond
//
//  Created by Nicolas Omer on 14/05/14.
//  Copyright (c) 2014 Nicolas Omer. All rights reserved.
//

#include "Problem3.h"

#define FOR(k, lb, ub) for (int k = (lb) ; (k) <= (ub) ; (k)++)

int I, K, M, g;
Solver s;
std::vector<std::vector<int> > matrix;

int prop (int a, int b, int c){
    return ( ( (a - 1) * g + (b - 1) ) * g + c);
}

int main3(std::vector<std::vector<int> > mat){

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
            std::cout << "Groupe "<<c<<" : ";
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
    std::cout<<"Done existance"<<std::endl;

    std::vector<int> cvec;
    FOR(a, 1, M){
        cvec.clear();
        FOR(c, 1, matrix[a][1]+1){
            cvec.push_back(c);
        }
        setConstraintGroupe(a, matrix[a][1], 0, 1, cvec);
    }
    std::cout<<"Done 1 joueur 1 groupe"<<std::endl;

    FOR(a, 1, M){
        FOR(c, 1, K){
            FOR(b1, 2, matrix[a].size()-1){
                FOR(b2, b1+1, matrix[a].size()-1){
                    // Contrainte ¬ 2 instruments dans le même groupe
                    if(matrix[a][b1] != I && matrix[a][b2] != I){
                        s.addBinary(~Lit(prop(a, matrix[a][b1], c)), ~Lit(prop(a, matrix[a][b2], c)));
                    }
                }
            }
        }
    }
    std::cout<<"Done 1 instru 1 groupe"<<std::endl;

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
                            if (canPlay(a2,b2)){
                                if(a1 != a2 && matrix[a1][b1] != I){
                                    lits.push(Lit(prop(a2, b2, c)));
                                    //std::cout<<"("<<a2<<", "<<b2<<", "<<c<<") \\/ ";
                                }
                                else if(canPlay(a2,b2) && matrix[a1][b1] == I){
                                    lits.push(Lit(prop(a2, b2, c)));
                                    //std::cout<<"("<<a2<<", "<<b2<<", "<<c<<") \\/ ";
                                }
                                else if(a1 == a2 && canPlay(a2,b2) && b2 == I){
                                    lits.push(Lit(prop(a2, b2, c)));
                                    //std::cout<<"("<<a2<<", "<<b2<<", "<<c<<") \\/ ";
                                }
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
                    if (canPlay(a2, matrix[a1][b]) && matrix[a1][b] != I)
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

void setConstraintGroupe(int a, int deep, int current, int beginvar, std::vector<int> c){
    if (current>deep)
    {
        //c est ici un ensemble ge groupe de la taille Max(a)+1
        //On commence par générer toute les combinaison, instrument/groupe parmis cet esemble
        std::vector<std::vector<int> > lits;
        std::vector<int> tmp;
        FOR(k, 0, deep){
            FOR(b, 2, matrix[a].size() - 1){
                tmp.clear();
                tmp.push_back(a);
                tmp.push_back(matrix[a][b]);
                tmp.push_back(c[k]);
                lits.push_back(tmp);
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

void setConstraintGroupeTer(int deep, int current, int beginvar, std::vector<std::vector<int> > litsProp, std::vector<int> listClause){
    if (current>deep)
    {

        vec<Lit> tmp;
        std::vector<int> c;
        FOR(i, 0, deep){
            c.push_back(litsProp[listClause[i]][2]);
        }
        FOR(i, 0, deep){
            if (countVec(c[i], c) > 1){
                return;
                //en fait dans le 2 et le 1 cette condition était double, elle vérifait non seulement qu'un joueur ne peut pas être dans deux groupe
                //mais aussi qu'il ne jouait pas plus d'un instrument pas groupe, ici sous certaine condition il a le droit, nous préférons alors ne plus vérifier ça en même temps
            }
        }
        FOR(i, 0, deep){
            tmp.push(~Lit(prop(litsProp[listClause[i]][0], litsProp[listClause[i]][1], litsProp[listClause[i]][2])));
            //std::cout<<"("<<litsProp[listClause[i]][0]<<", "<<litsProp[listClause[i]][1]<<", "<<litsProp[listClause[i]][2]<<") \\/ ";
        }
        //std::cout<<"FIN"<<std::endl;
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

int countVec(int a, std::vector<int> c){
    int res = 0;
    FOR(i, 0, c.size() - 1){
        if (c[i] == a){
            res += 1;
        }
    }
    return res;
}
