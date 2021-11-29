# band-maker-minisat

## Quickstart

```
cmake src
make
./Question 1 test/example_0.txt res1.txt
./Question 2 test/example_1.txt res2.txt
./Question 3 test/example_1.txt res3.txt
```

Explore results in `res*.txt`.

## Goal

The goal of the basic computer science project is to form groups of music under certain constraints. It is about expressing the constraints in conjunctive normal form in order to encode them in the tool to solve the various given problems. For this, we use the * MiniSat * solver which allows to determine, under a set of constraints, if a problem is satisfactory.

# Question 1

## Definition of variables and symbols

In this section, we define the different variables and symbols that we use in the expression of constraints in conjunctive normal form.

- `M` is the number of musicians.
- `I` is the number of instruments.
- `K` is the maximum number of groups.
- `X`<sub>`a, b, c`</sub> is a variable such that for all musician `a`, the instrument `b` in the group `c`, `X`<sub>`a, b, c`</sub>` = true` if and only if the musician `a` plays the instrument `b` in the group `c`.
- `I`<sub>`a`</sub> represents the list of instruments that `a` masters.

### Input file fomat

```
M I K 
1 first_instrument [second_instrument third_isnsturment ... kth_intsrument]
2 first_instrument [second_instrument third_isnsturment ... kth_intsrument]
...
M first_instrument [second_instrument third_isnsturment ... kth_intsrument]
```

Example:
```
3 2 3
1 1
2 2 3
3 2 3
```

## Forme Normale Conjonctive

### Contrainte d’existence

$$\\bigwedge\\limits\_{0\<a\\le M}\\ \\left(\\bigvee\\limits\_{0\<c\\le K}\\ \\bigvee\\limits\_{b\\in I_a}\\ X\_{a,b,c}\\right)$$
Il s’agit simplement de dire que chaque musicien doit jouer un des
instruments qu’il maitrise dans un groupe.

### Un musicien ne peut être que dans un seul groupe

$$\\bigwedge\\limits\_{0\<a\\le M}\\ \\bigwedge\\limits\_{b_1\\in I_a}\\ \\bigwedge\\limits\_{b_2\\in I_a}\\ \\bigwedge\\limits\_{0\<c_1\\le K}\\ \\bigwedge\\limits\_{c_1\<c_2\\le K}\\ \\left(\\neg X\_{a,b_1,c_1} \\vee \\neg X\_{a,b_2,c_2}\\right)$$

Par musicien, nous disons qu’il ne peut pas, à la fois jouer d’un
instrument *b*<sub>1</sub> quelconque dans un groupe *c*<sub>1</sub> et
un instrument *b*<sub>2</sub> quelconque dans un autre groupe
*c*<sub>2</sub>.

### Un instrument par groupe

$$\\bigwedge\\limits\_{0\<a\\le M}\\ \\bigwedge\\limits\_{0\<c\\le K}\\ \\bigwedge\\limits\_{b_1\\in I_a}\\ \\bigwedge\\limits\_{\\substack{b_2\\in I_a \\\\ b_2 \\ne b1}}\\ \\left(\\neg X\_{a,b_1,c} \\vee \\neg X\_{a,b_2,c}\\right)$$

Par musicien, nous disons qu’il ne peut pas à la fois jouer d’un
instrument *b*<sub>1</sub> dans un groupe c et d’un autre instrument
*b*<sub>2</sub> quelconque dans le même groupe c.

### Un seul musicien peut jouer d’un certain instrument dans un groupe

$$\\bigwedge\\limits\_{0\<c\\le K}\\ \\bigwedge\\limits\_{0\<a_1\\le M}\\ \\bigwedge\\limits\_{a_1\<a_2\\le M}\\ \\bigwedge\\limits\_{\\substack{b\\in I\_{a_1} \\\\ b\\in I\_{a_2}}}\\ \\left(\\neg X\_{a_1,b,c} \\vee \\neg X\_{a_2,b,c}\\right)$$

Nous exprimons ici que, dans un même groupe, une personne
*a*<sub>1</sub> et autre *a*<sub>2</sub> ne peuvent pas jouer du même
instrument dans un groupe.

### Tous les instruments dans un groupe ou aucun

$$\\bigwedge\\limits\_{0\<c\\le K}\\ \\bigwedge\\limits\_{0\<a_1\\le M}\\ \\bigwedge\\limits\_{b_1\\in I\_{a_1}}\\ \\bigwedge\\limits\_{\\substack{0\<b_2\\le I \\\\ b_2 \\ne b_1}}\\ \\left(\\neg X\_{a_1,b_1,c} \\bigvee\\limits\_{\\substack{0\<a_2\\le M \\\\ a_2 \\ne a_1 \\\\ b_2 \\in I{a_2}}}\\ X\_{a_2,b_2,c}\\right)$$

Ici nous voulons exprimer la contrainte groupe complet ou vide, pour ce
faire nous utilisons une implication qui dit que, dans chaque groupe, si
un musicien *a*<sub>1</sub> joue d’un instrument *b*<sub>1</sub>, alors
ceci implique que, pour chaque autre instrument *b*<sub>2</sub>, un des
autres musiciens maîtrisant ce dernier, *a*<sub>2</sub>, le joue dans ce
groupe.

# Question 2

## Définition des variables et symboles

-   *M**a**x*<sub>*a*</sub> représente le nombre maximum de groupe dans
    lequel le musicien *a* peut jouer.

-   \|*I*<sub>*a*</sub>\| représente la cardinalité de *I*<sub>*a*</sub>
    soit le nombre d’instruments dont *a* peut se servir.

Les variables et symboles restants sont identiques à ceux de la question
1.

## Forme Normale Conjonctive

### Un musicien ne peut pas jouer dans plus de *M**a**x*<sub>*a*</sub> groupes

Pour implémenter cela, nous allons étendre la solution "un groupe
maximum" qui, pour rappel, voulait exprimer
¬(*g**r**o**u**p**e*<sub>1</sub>∧*g**r**o**u**p**e*<sub>2</sub>) ∧ ¬(*g**r**o**u**p**e*<sub>2</sub>∧*g**r**o**u**p**e*<sub>3</sub>)…
Dans la conjonction se trouveront *M**a**x*<sub>*a*</sub> + 1 littéraux.
Pour se faire, on commence par générer tous les sous-ensembles de groupe
de taille *M**a**x*<sub>*a*</sub> + 1 et toutes les combinaisons de
*I*<sub>*a*</sub> de taille *M**a**x*<sub>*a*</sub> + 1. Car un joueur
ne peut pas jouer d’un même instrument dans différents groupes ni de
différents instruments dans différents groupes.

A la seconde étape, on génère toutes les combinaisons d’identifiants
(*t* et *s*) et finalement, il ne reste qu’à choisir une des
combinaisons avec les variables *x* et *y*.

Cette contrainte est exprimée par la première ligne de la forme normale
conjonctive suivante. On peut illustrer cela par un exemple où *K* = 3
et *M**a**x*<sub>*a*</sub> + 1 = 2  
  
$$\\bigwedge\\limits\_{0\<a\\le M}\\ \\bigwedge\\limits\_{0\<c_1\\le K}\\ \\bigwedge\\limits\_{c_1\<c_2\\le K}\\ \\ldots\\ \\bigwedge\\limits\_{c\_{{Max_a}}\<c\_{Max\_{a}+1}\\le K}\\ \\bigwedge\\limits\_{b_1 \\in I_a}\\ \\bigwedge\\limits\_{b_2 \\in I_a}\\ \\ldots\\  \\bigwedge\\limits\_{b\_{Max_a+1} \\in I_a}$$

$$\\bigwedge\\limits\_{0\<t\_{1}\\le Max_a+1}\\  \\ldots\\  \\bigwedge\\limits\_{0\<t\_{Max_a+1}\\le Max_a+1}\\ \\bigwedge\\limits\_{0\<s_1\\le Max_a+1}\\  \\ldots\\  \\bigwedge\\limits\_{0\<s\_{Max_a+1}\\le Max_a+1}\\\\$$

$$\\bigwedge\\limits\_{0\<y\\le Max_a+1}\\  \\left( \\bigvee\\limits\_{0\<z\\le Max_a+1}\\  \\neg X\_{a,b\_{s\_{y}},c\_{t\_{z}}}\\right)$$

Nous nous rendons compte qu’avec cette contrainte, nous vérifions aussi
la contrainte *Un instrument par groupe* puisque parfois, pour tout *y*,
*s*<sub>*y*</sub> sera identique pour toute la clause, dès lors nous
n’avons plus besoin de cette contrainte.  
Pour le reste, les autres contraintes sont identiques.  
Le nombre de clauses crée sera égal à :  
$$\\sum\_{a=1}^{M}\\ \\left( {K \\choose Max_a+1}\\ \* \\ \|I_a\|^{Max_a+1}\\ \*\\ \\left(Max_a+1\\right)^{Max_a+1}\\ \*\\ \\left(Max_a+1\\right)^{Max_a+1}\\ \*\\  \\left(Max_a+1\\right) \\right)$$

Soit  
$$\\sum\_{a=1}^{M}\\ \\left( {K \\choose Max_a+1}\\ \* \\ \|I_a\|^{Max_a+1}\\ \*\\ \\left(Max_a+1\\right)^{2\*(Max_a+1)+1}\\ \\right)$$

Sachant que chaque clause est composée des *M**a**x*<sub>*a*</sub>
littéraux. Cependant, il y aura des doublons dans les clauses générées,
mais le solveur MiniSat se chargera de les supprimer pour nous[^1].

# Question 3

## Définition des variables

-   L’instrument qui a comme valeur *I* est toujours l’instrument
    *chant*.

Les variables et les symboles sont identiques à ceux de la question 2.

## Forme Normale Conjonctive

### Un seul musicien peut jouer d’un certain instrument dans un groupe

C’est toujours vrai sauf quand l’instrument considéré est le chant.

$$\\bigwedge\\limits\_{0\<c\\le K}\\ \\bigwedge\\limits\_{0\<a_1\\le M}\\ \\bigwedge\\limits\_{a_1\<a_2\\le M}\\ \\bigwedge\\limits\_{\\substack{b\\in I\_{a_1} \\\\ b\\in I\_{a_2} \\\\ b \\ne I}}\\ \\left(\\neg X\_{a_1,b,c} \\vee \\neg X\_{a_2,b,c}\\right)$$

### Tous les instruments dans un groupe ou aucun

La contrainte reste vraie, cependant si un musicien utilise un autre
instrument que sa voix, ceci peut impliquer qu’il chante et inversement
(s’il maîtrise ces instruments bien surs).  
$$\\bigwedge\\limits\_{0\<c\\le K}\\ \\bigwedge\\limits\_{0\<a_1\\le M}\\ \\bigwedge\\limits\_{b_1\\in I\_{a_1}}\\ \\bigwedge\\limits\_{\\substack{0\<b_2\\le I \\\\ b_2 \\ne b_1}}\\ \\left(\\neg X\_{a_1,b_1,c} \\ \\bigvee\\limits\_{\\substack{0\<a_2\\le M \\\\ b_2 \\in I{a_2}}}\\left( \\
\\bigvee\\limits\_{\\substack{a_2 \\ne a_1 \\\\ b_1 \\ne I}}\\  X\_{a_2,b_2,c}\\ 
\\bigvee\\limits\_{\\substack{b_1 = I}}\\ X\_{a_2,b_2,c}\\ 
\\bigvee\\limits\_{\\substack{a_2 = a_1 \\\\ b_2 = I}}\\ X\_{a_2,b_2,c}\\right)\\right)$$

Dans la parenthèse la plus imbriquée, le premier grand ou est le même
que précédemment, le second signifie que, si *a*<sub>1</sub> chante,
alors *a*<sub>2</sub> peut chanter aussi et le dernière représente le
fait que si *a*<sub>1</sub> joue d’un instrument et qu’il sait chanter,
il peut faire les deux.

### Un instrument par groupe

La contrainte *Un instrument par groupe* et la contrainte *Un musicien
ne peut pas jouer dans plus de *M**a**x*<sub>*a*</sub> groupes* ne
peuvent plus être combinées, puisque la première n’est plus toujours
vraie depuis l’arrivée de la composante du chant.  
Commençons par réintroduire cette dernière :

$$\\bigwedge\\limits\_{0\<a\\le M}\\ \\bigwedge\\limits\_{0\<c\\le K}\\ \\bigwedge\\limits\_{\\substack{b_1\\in I_a \\\\ b_1 \\ne I}}\\ \\bigwedge\\limits\_{\\substack{b_2\\in I_a \\\\ b_2 \\ne b1 \\ne I}}\\ \\left(\\neg X\_{a,b_1,c} \\vee \\neg X\_{a,b_2,c}\\right)$$

Il s’agit de la même expression que dans la première question sauf que
*b*<sub>1</sub> et *b*<sub>2</sub> ne peuvent pas valoir *I*, le
*chant*.

### Un musicien ne peut pas jouer dans plus de *M**a**x*<sub>*a*</sub> groupes

Cette contrainte reste vraie, sauf pour l’instrument *I* et il ne faut
plus introduire en même temps la contrainte décrite ci-dessus. Voici la
forme normale conjonctive :  
$$\\bigwedge\\limits\_{0\<a\\le M}\\ \\bigwedge\\limits\_{0\<c_1\\le K}\\ \\bigwedge\\limits\_{c_1\<c_2\\le K}\\ \\ldots\\ \\bigwedge\\limits\_{c\_{{Max_a}}\<c\_{Max\_{a}+1}\\le K}\\ \\bigwedge\\limits\_{\\substack{b_1 \\in I_a \\\\ b_1 \\ne I}}\\ \\bigwedge\\limits\_{\\substack{b_2 \\in I_a \\\\ b_2 \\ne I}}\\ \\ldots\\  \\bigwedge\\limits\_{\\substack{b\_{Max_a+1} \\in I_a \\\\ b\_{Max_a} \\ne I}}$$

$$\\bigwedge\\limits\_{0\<t\_{1}\\le Max_a+1}\\  \\ldots\\  \\bigwedge\\limits\_{t\_{Max_a}\<t\_{Max_a+1}\\le Max_a+1}\\ \\bigwedge\\limits\_{0\<s_1\\le Max_a+1}\\  \\ldots\\  \\bigwedge\\limits\_{0\<s\_{Max_a+1}\\le Max_a+1}\\\\$$

$$\\bigwedge\\limits\_{0\<y\\le Max_a+1}\\  \\left( \\bigvee\\limits\_{0\<z\\le Max_a+1}\\  \\neg X\_{a,b\_{s\_{y}},c\_{t\_{z}}}\\right)$$

Ici, nous ne prenons plus tous les ensembles d’indices de *c*, , *t*
mais uniquement les ensembles différents, nous nous retrouvons alors
avec un coefficient binomial ${Max_a \\choose Max_a}$ qui vaut 1, en
effet il s’agit simplement de la suite des indices ce qui nous donne
bien ce qu’on souhaite, chaque *c*<sub>*t*</sub> sera différent des
autres.

<span id="addendum" label="addendum">\[addendum\]</span>

Dans ce cas, les clauses générées auront pour but unique d’introduire
une contrainte *au plus k* et il y en aura :

$$\\sum\_{a=1}^{M}\\ \\left( {K \\choose Max_a+1}\\ \* \\ \|I_a\|^{Max_a+1}\\ \*\\ \\left(Max_a+1\\right)\\ \*\\ \\left(Max_a+1\\right)^{Max_a+1}\\ \*\\  \\left(Max_a+1\\right) \\right)$$

Soit  
$$\\sum\_{a=1}^{M}\\ \\left( {K \\choose Max_a+1}\\ \* \\ \|I_a\|^{Max_a+1}\\ \*\\ \\left(Max_a+1\\right)^{Max_a+3}\\ \\right)$$

Il est important de noter qu’on omettra l’instrument *chant* dans
l’ensemble *I*<sub>*a*</sub>.

[^1]: addendum au nombre de clauses créé à la section
    <a href="#addendum" data-reference-type="ref" data-reference="addendum">[addendum]</a>
