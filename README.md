Pasca Robert-Paul 315CB

Pentru a comprima o imagine de tip .ppm, am creat un program care utilizeaza un arbore cuaternar. Pentru a construi arborele, am utilizat o matrice de pixeli, fiecare element stocand valorile de culoare RGB corespunzatoare. Am creat o functie recursiva care utilizeaza mean-ul si culorile medii ale unui bloc (formulele de medie aritmetica din cerinta) pentru a construi arborele cuaternar in functie de similaritatea blocurilor.
Pentru a usura crearea unui nod simplu sau colorat, am initializat informatia unui nod cu tipul void * si am convertit-o in functie de tipul de date corespunzator prin utilizarea unui cast. Am folosit, de asemenea, un enum pentru a face mai usor de identificat tipul de nod in cod.

Cerinta 1:
In aceasta functie, am utilizat functiile prezentate in cursurile si laboratoarele referitoare la arbori binari, cu singura diferenta fiind numarul de noduri.

Cerinta 2:
Pentru a satisface aceasta cerinta, am observat ca introducerea in fisier se bazeaza pe parcurgerea arborelui in latime. Pentru a implementa aceasta functionalitate, am utilizat o coada pentru a retine nodurile. Cand nodurile sunt extrase din coada, fiilor lor sunt adaugati in coada.

Cerinta 3:
La aceasta cerinta, am reusit sa reconstruiesc arborele prezent in fisierul comprimat (scris prin parcurgere in latime). Am folosit o coada pentru a retine valorile nodurilor care nu sunt colorate (adica nu sunt frunze). Dupa ce am reconstruit arborele cuaternar, am reconstruit si matricea de pixeli, pentru a usura crearea fisierului .ppm.

Cerinta 1 : 20
Cerinta 2 : 30.0
Cerinta 3 : 30.0
Total     : 80.0
Bonus     : 20.0
Felicitari! Ai punctajul maxim: 80p! :)