# Alobe (TP2)

## 1\. Description

Alobe est un logiciel libre permettant de manipuler de grands graphes non-
orientes decrits par la liste de leurs arretes. Il possede les
caracteristiques suivantes :

  * Il est distribue sous la licence [GNU General Public License](http://www.gnu.org/copyleft/gpl.html)
  * Il est ecrit en C (bien qu'ecrit au depart en C++, comme en temoigne le depot subversion) et possede une interface en ligne de commande,
  * Il implemente le calcul de la distance d'un noeud a tous les autres (exercice 1).
  * Il fournit en sortie des donnees permettant de tracer la distribution des distance a un noeud donne (exercice 2)
  * Il fournit en sortie les donnees permettant de tracer l'evolution de l'estimation de la distance moyenne en fonction du nombre de parcours effectues. (exercice 3)
  * Il implemente le calcul de la borne inferieure du diametre, en prenant la distance maximale d'un noeud donne a tous les autres (exercice 4).
  * Il implemente le calcul de la borne superieure du diametre, en prenant la distance maximale dans l'arbre du parcours en largeur (exercice 5). 
  * Il fournit en sortie des donnees permettant de tracer les courbes des meilleurs bornes inferieures et superieure en fonction du nombre de parcours effectues. (exercice 6 - defi).

### 1.1. Auteurs

Alobe a ete entierement realise par Glenn ROLLAND
<[glenux@fr.st](mailto:glenux@fr.st)> a l'occasion de travaux pratiques du
cours de Grand Reseaux du Master 2 Ingenierie Informatique \- Systemes,
Reseaux et Internet.

## 2\. Pre-requis

Alobe ne necessite pas de bibliotheques de fonctions particulieres pour
fonctionnner.

## 3\. Se procurer Alobe

Vous pouvez telecharger la derniere archive des sources, ou bien directement
la version la plus recente du projet sur le depot Subversion du projet.  

### 3.1. L'archive des sources

Elle est disponible a l'adresse :  
<http://glenux2.free.fr/pub/projets/Alobe/archives/>  

### 3.2. Le depot Subversion

Afin d'obtenir les sources les plus a jour, vous pouvez utiliser le logiciel
de controle de sources Subversion

$ svn checkout http://repository.glenux.ath.cx/svn/Cours/M2/Grand_Reseaux/TP1/

Il n'y a pas de mot de passe, il suffit donc de presser la touche "Entree"
pour l'utilisateur "anonymous", si ce dernier vous est demande.

## 4\. Utiliser Alobe

### 4.1. Compilation

Commencez par decompressez l'archive.

$ tar -xzvf alobe-0.2.tar.gz

Rendez vous ensuite dans le dossier qui vient d'etre cree lors de la
decompression.

$ cd alobe-0.2

Puis lancez l'auto-configuration du logiciel, puis la compilation.

$ ./autogen  
$ ./configure  
$ make

Le binaire produits se trouve dans le dossier :

  *  src/alobe

### 4.2. Utilisation

Les binaires de Alobe doivent etre appeles avec la syntaxe suivante:  

Usage: alobe <commande> <parametres_obligatoires> [options]  

Les commandes sont les suivants:

-I, --tp2distance
    Calcule les distances a partir du noeud donne.
-J, --tp2distanceplot 
    Donne la distribution des distances a partir du noeud donne.
-L, --tp2distevolution
    Donne l'evolution de l'estimation de la distance moyenne.pour un noeud donne, ou au hasard, en fonction du nombre d'iterations.
-M, --tp2limitinf
    Calcule la borne inferieure du diametre pour un noeud donne ou au hasard, en fonction du nombre d'iterations.
-N, --tp2limitsup
    Calcule la borne superieure du diametre pour un noeud donne ou au hasard, en fonction du nombre d'iterations.
-O, --tp2defi
    Fournit les donnees permettant de tracer les courbes de meilleures bornes inferieures et superieures du diametre en fonction du nombre d'iteration.

Les parametres obligatoires sont les suivants:

-c, --count <entier>
    Nombre de noeuds du fichier d'entree.

Les parames optionnels sont les suivants:

-i, --input <fichier>
     Le fichier d'entree, "-" designant l'entree standard,
-o, --output <fichier>
    Le fichier de sorftie, "-" designant la sortie standard.
-v, --verbose
     Passe l'affichage en mode verbeux.e numero du noeud a lire et afficher a partir du fichier compresse
-r, --root <entier>
    Noeur servant de racine a la premiere iteration.
-n, --iterations <entier>
    Nombre d'iterations a effectuer.

## 5\. Documentation

### 5.1. Code

Vous pouvez trouver la documentation de Alobe dans le dossier doc/html de
l'application, ou en suivant [ce lien](html/index.html).

### 5.2. Remarques sur les differents exercices

#### 5.2.1. Exercice 1

On calcule la distance d'un noeud (le 24 par exemple) a tous les autres, ainsi
que la moyenne de toutes ses distances, par la commande suivante:

 ./alobe -I -i web.data.gz -o result.txt -c 701654 -r 24

Ce qui produit le fichier result.txt suivant :

Maximum distance : 1  
Average distance : 0.666667

#### 5.2.2. Exercice 2

On obtient la distribution des distances pour un noeud donne (le 24 par
exemple) de la façon suivante:

$ ./alobe -J -i web.data.gz -o result.txt -c 701654 -r 24  

Ce qui produit en sortie

0 1  
1 336  
2 3017  
3 21100  
4 89398  
5 146225  
6 145567  
7 118491  
8 77830  
9 47189  
10 21247  
11 8628  
12 1550  
13 532  
14 112  
15 7  
16 4

Soit le graphique suivant :  
![exercice 2 plot](exercice2.png)

#### 5.2.3. Exercice 3

On trace l'evolution de l'estimation de la distance moyenne (en fonction du
nombre d'iteration) par la commande suivante:

 ./alobe -L -i web.data.gz -o result.txt -c 701654 -n 100 -r 24  

Ce qui produit en sortie:

0 6.228710  
1 7.560919  
2 9.514071  
3 9.537433  
4 9.504442  
5 9.567365  
6 9.542382  
7 9.429151  
8 9.426282  
9 9.566440  
10 9.583777  
11 9.450484  
12 9.548250  
13 9.503499  
14 9.508191  
15 9.475249  
16 9.297400  
17 9.210398  
[...]

Soit sous forme graphique :  
![exo 3 plot](exercice3.png)

  

#### 5.2.4. Exercice 4

Pour calculer la borne inferieure, on fera:

./alobe -M -i web.data.gz -o result.txt -c 701654 -n 5 -r 24

Iteration 0 \-- choosing root 24  
             \-- borne inferieure 17  
Iteration 1 -- choosing root 60401  
             \-- borne inferieure 18  
Iteration 2 -- choosing root 700018  
             \-- borne inferieure 24  
Iteration 3 -- choosing root 77852  
             \-- borne inferieure 24  
Iteration 4 -- choosing root 45944  
             \-- borne inferieure 24  

  

#### 5.2.5. Exercice 5

Pour le calcul de la borne superieure, on fait :

./alobe -N -i ~/web.data.gz -o result.txt -c 701654 -n 10 -r 24

Iteration 0 \-- choosing root 24  
             \-- borne superieure 33  
Iteration 1 -- choosing root 96542  
             \-- borne superieure 33  
Iteration 2 -- choosing root 49208  
             \-- borne superieure 33  
Iteration 3 -- choosing root 436498  
             \-- borne superieure 33  
Iteration 4 -- choosing root 309990  
             \-- borne superieure 32  
Iteration 5 -- choosing root 538890  
             \-- borne superieure 32  
Iteration 6 -- choosing root 266656  
             \-- borne superieure 32  
Iteration 7 -- choosing root 529998  
             \-- borne superieure 32  
Iteration 8 -- choosing root 140145  
             \-- borne superieure 32  
Iteration 9 -- choosing root 640316  
             \-- borne superieure 32  
  

#### 5.2.6. Exercice 6 - Defi

Le cumul des courbes precedentes se fait par :

$ ./alobe -O -i web.data.gz -o result.txt -c 701654 -n 100 -r 24

Puis :

$ ./defiplot.sh result.txt

Pour obtenir : ![defi plot](defi.png)

Remarque: pour le defi, il aurait fallu en plus utiliser une heuristique de
choix des noeuds permettant de faire converger les deux courbes bornant le
diametre au plus vite. Par exemple, choisir les noeuds par degre decroissant
dans la composante connexe, en supposant qu'un noeud a fort degre comme racine
donne un arbre plus plat et donc la borne supperieure par la meme occasion...

  

