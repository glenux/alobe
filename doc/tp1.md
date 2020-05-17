# Alobe

## Description

Alobé est un logiciel libres permettant de manipuler de grands graphes non-orientes décrits par la liste de leurs arrêtes. Il possède les
caractéristiques suivantes :

  * Il est distribue sous la licence [GNU General Public License](http://www.gnu.org/copyleft/gpl.html)
  * Il est ecrit en C (bien qu’écrit au départ en C++, comme en témoigne le dépôt subversion) et possède une interface en ligne de commande,
  * Il implémente le filtrage du graphe pour l'extraction d'un sous-ensemble (exercice 2).
  * Il implémente le calcul des degrés (exercice 3)
  * Il implémente les statistiques sur le calcul des degrés (degré moyen, degré maximum, densité du graphe) (exercice 5)
  * Il implémente le stockage des données de façon contiguë en mémoire (en un unique tableau) (exercice 4).
  * Il implémente le calcul des composantes connexes de façon efficace (exercice 5). 
  * Il implémente une méthode de calcul alternatif des composantes connexes (exercice 6 - defi).

### Auteurs

Alobe a ete entierement realise par Glenn ROLLAND
<[glenux@fr.st](mailto:glenux@fr.st)> a l'occasion de travaux pratiques du
cours de Grand Reseaux du Master 2 Ingenierie Informatique \- Systemes,
Reseaux et Internet.

## Pre-requis

Alobé ne nécessite pas de bibliothèques de fonctions particulières pour
fonctionner.

## Se procurer Alobe

Vous pouvez recharger la dernière archive des sources, ou bien directement
la version la plus récente du projet sur le dépôt Subversion du projet.  

## 4. Utiliser Alobe

### 4.1. Compilation

Commencez par decompressez l'archive.

    $ tar -xzvf alobe-0.1.tar.gz

Rendez vous ensuite dans le dossier qui vient d'etre cree lors de la
decompression.

    $ cd alobe-0.1

Puis lancez l'auto-configuration du logiciel, puis la compilation.

    $ ./autogen  
    $ ./configure  
    $ make

Le binaire produits se trouve dans le dossier `src/alobe`


### 4.2. Utilisation

Les binaires de Alobe doivent etre appeles avec la syntaxe suivante:  

    Usage: alobe <commande> <parametres_obligatoires> [options]  

Les commandes sont les suivants:

    -F, --filter
        Filtrage du fichier d'entree pour extraire un sous-graphe.
    -D, --degree 
        Calcul du degre des noeuds du graphe pris en entree.
    -S, --store
        Stockage et remplissage du tableau representant le graphe en memoire.
    -A, --average
        Calcul des statistiques sur les noeuds du graphe d'entree (degre moyen, degre max, densite).
    -C, --connexity
        Calcul des composantes connexes par la methode du tableau unique.
    -E, --defi
        Calcul des composantes connexes par ensembles d'intervalles de noeuds.

Les parametres obligatoires sont les suivants:

    -c, --count <entier>
        Nombre de noeuds du fichier d'entree.
    -s, --size <entier>
        Taille du filtre
    -t, --offset <entier>
        Offset du filtre

Les parames optionnels sont les suivants:

    -i, --input <fichier>
         Le fichier d'entree, "-" designant l'entree standard,
    -o, --output <fichier>
        Le fichier de sorftie, "-" designant la sortie standard.
    -v, --verbose
         Passe l'affichage en mode verbeux.e numero du noeud a lire et afficher a partir du fichier compresse

## 5. Documentation

### 5.1. Code

Vous pouvez trouver la documentation de Alobe dans le dossier doc/html de
l'application, ou en suivant [ce lien](html/index.html).

### 5.2. Remarques sur les differents exercices

#### 5.2.1. Exercice 1

  

#### 5.2.2. Exercice 2

Il est possible de filtrer le graphe d'entree pour extraire un sous ensemble
de noeuds de la façon suivante :

	./src/alobe -F -t 0 -s 30 -i ../web.data.gz -c 701654

Ce qui produit en sortie

    Command -> FILTER  
    Offset -> 0  
    Size -> 30  
    Input Data -> ../web.data.gz  
    Input Index -> 701654  
    Filtering between [ 0 .. 30 ]...  
    0 1  
    0 2  
    1 3  
    1 4  
    1 5  
    1 6  
    1 7  
    1 8  
    1 9  
    1 10  
    1 11  
    1 12  
      
    [...]  
  
( uniquement les noeuds x compris entre 0 <= x < 30 )

#### 5.2.3. Exercice 3

Le calcul du degré des noeuds se fait par la commande suivante :

    ./src/alobe -D -i ../web.data.gz -c 701654

Ce qui produit (en premiere colone l'index du noeud et en seconde le degre):

    Command -> DEGREE  
    Input Data -> ../web.data.gz  
    Input Index -> 701654  
    Computing degree of each node...  
    done  
    0 2  
    1 1194  
    2 77  
    3 496  
    4 227  
    5 339  
    6 337  
    7 340  
    8 337  
    9 10  
    10 16  
    11 31  
    12 15  
    13 22  
      
    [...]

Le calcul du degre est effectue dans un tableau de sizeof(long) * N (ou N est
le nombre de noeuds), initialise a zero, et ou les valeurs des cases sont
incrementees a la lecture des arcs.  

#### 5.2.4. Exercice 4

Le simple stockage du graphe en mémoire ne produit pas de sortie visible, mais
s’exécute en tapant :

    ./src/alobe -S -i ../web.data.gz -c 701654

Le programme commence par calculer les degres, puis initialise un tableau de
taille N (N = nombre de noeuds) pointeurs vers les cases d'un tableau de
taille (M + 3) * sizeof(long) (ou M est la somme des degres des noeuds)
destine a contenir les arcs de chaque noeud. Les 3 cases supplementaires ne
servent qu'au calcul des composates connexes et seront decrites plus loin.  

#### 5.2.5. Exercice 5

Le calcul des statistiques sur les noeuds du graphe se fait de la façon
suivante:

    ./src/alobe -A -i ../web.data.gz -c 701654

    Command -> AVERAGE  
    Input Data -> ../web.data.gz  
    Input Index -> 701654  
    Computing degree of each node...  
    done  
    Degree average: 5.517015  
    Degree maximum: 5331  
    Density : 0.000000  

Cet exercice reutilise la structure de donnees l'exercice 3, et en la
parcourant effectue le calcul.

#### 5.2.6. Exercice 6

Le calcul des composantes connexes se fait de la façon suivante :

    ./src/alobe -C -i ../web.data.gz -c 701654

    Command -> CONNEXITY  
    Input Data -> ../web.data.gz  
    Input Index -> 701654  
    Computing degree of each node...  
    done  
    Filling the Big Table...  
    done  
    Found connex component at 0  
    Found connex component at 9484  
    Found connex component at 15516  
    Found connex component at 17477  
    Found connex component at 20073  
    Found connex component at 20100  
      
    [...]  
      
    Found connex component at 699413  
    Found connex component at 700568  
    Found connex component at 701306  
    Found connex component at 701313  
    Found 970 connex components

Pour le fichier IP.data.gz on obtient :

    Command -> CONNEXITY  
    Input Data -> /home/warbrain/Films/IP.data.gz  
    Input Index -> 467273  
    Computing degree of each node...  
    done  
    Filling the Big Table...  
    done  
    Found connex component at 0  
    Found connex component at 324896  
    Found 2 connex components

Et pour le fichier P2P.data.gz les machines a ma disposition ne possedaient
pas suffisament de memoire...

Le calcul des composantes connexes utilise le meme tableau que l'exercice 4.
Le calcul se fait dans les 3 cases supplementaires :  

  * une case pour le degre du noeud
  * une case pour l'offset du noeud traite
  * une case pour le noeud "pere" dans le parcours en profondeur.

Lorsqu'on parcours un noeud x en provenance de y, on inscrit la reference du
noeud pere dans la case 3, puis pour chaque noeud adjacent non visite, on
indique le noeud adjacent parcouru actuellement puis on parcourt recursivement
le noeud adjacent.

Remarque: jusque la le TP etait programme en C++, et pour un eventeuel gain de
performances il fut reecrit entierement (en conservant les structures de
donnees) en C. Cependant seules 0.3 secondes furent gagnees sur le graphe
web.data.gz... sur un temps de calcul total de 14 sec... (sur un iBook G4 1Ghz
avec 256 Mo de RAM sous GNU/Linux).

#### 5.2.7. Defi

On suppose que dans le graphe du web, les noeud adjacents ont de fortes
chances d'appartenir a la même composante connexe.

Ainsi, pour décrire en mémoire une composante regroupant les nœuds {0, 1, 2
,3 ,4, 5, 6, 7, 8, 9, 10, 14,15,16, 17, 19, 20} nul besoin de stocker en
mémoire autre chose que les ensembles d'intervalles suivants: [0 .. 10] U [14
.. 17] U [19 .. 20] ...

Cependant, les fusions d'ensembles nécessitent de nombreuses recopies de
données et d'allocations de mémoire. L'algorithme s'en trouve par conséquent
fort ralenti...

On peut lancer le défi en tapant :

    ./src/alobe -E -i ../web.data.gz -c 701654

..et admirer les résultats (s'il apparaissent, car il y a encore pas mal de
soucis de pointeurs se baladant librement...)

