Functie custom find(Tree tree, char *searched_name)

Foarte importanta, e folosita aproape in fiecare task.
Returneaza un pointer catre nodul cu numele searched_name, pe care il cauta recursiv.
In search retin rezultatul apelului recursiv.
Cand el e diferit de NULL, inseamna ca a fost gasit nodul, si se termina functia.

Functia preorder_transversal

Printez nodul curent iar apoi parcurg recursiv angajatii lui.
Printez \n doar la final:
    Adica cand ma reintorc la root (employee->manager == NULL) 
    Dupa forul de parcurgere al angajatilor.

OBS:
    In majoritatea functiilor am if-uri care verifica daca datele sunt corecte:
    Adica, cand un parametru e un angajat sau manager, trebuie vazut daca el exista in tree.
    Tree != NULL
    etc.

**************************************************************************************************************************

Functia hire

In primul rand, aloc toata memoria necesara unui nou nod.
Folosesc functia find ca sa gasesc managerul.
Cresc direct_employees_no.
Realoc memoria managerului cu un + 1 ca sa pot sa inserez inca un nod.
Caut cu strcmp locul potrivit pentru noul nod ca sa fie ordonat alfabetic.

**************************************************************************************************************************

Functia fire

Folosesc functia find ca sa gasesc angajatul.
Gasesc pe ce pozitie din echipa managerului se afla angajatul.
Shiftez vectorul la stanga ca sa il sterg.
Realoc memorie pentru manager ca sa pot acomoda echipa celui dat afara.
Reiau algoritmul de la hire de inserare alfabetic pentru toata echipa.

**************************************************************************************************************************

Functia promote

Mai intai verific ca sunt minim pe nivelul 2.
Ii dau fire angajatului de pe pozitia initiala.
Ii dau hire la loc in ehipa managerului de deasupra.

**************************************************************************************************************************

Functia move_employee

Mai intai verific ca sunt minim pe nivelul 1.
Ii dau fire angajatului de pe pozitia initiala.
Ii dau hire la loc in ehipa noului manager.

**************************************************************************************************************************

Functia move_team

Ca sa mut o echipa trebuie mutat doar managerul si pastrate legaturile cu echipa.
Algoritmul seamana cu un fire si un hire.
Mai intai scot nodul din echipa curenta. (ca la fire)
Inserez nodul la noul manager. (ca la hire)

**************************************************************************************************************************

Functia fire_team

Se aplica functia fire recursiv pe echipa.
Se parcurge in postordine <=> managerul e dat afara dupa echipa.
Apelul recursiv se face pe manager->team[0] mereu
    > fire muta toate elementele la stanga.

**************************************************************************************************************************

Functia get_employees_by_manager

Functia are o alta functie atasata: helper_empl_by_manager.
Practic, eu vreau sa obtin toti angajatii de sub un nod recursiv.
Recursivitatea are loc in functia helper.
Primeste ca parametru un vector unde voi retine angajatii si sizeul lui v.
Adaug alfabetic in vector dupa acelasi principiu de la hire.

**************************************************************************************************************************

Functia get_employees_by_level

Am din nou o functie helper care merge pe acelasi principiu.
Helper primeste si nivelul drept parametru.
Apelul recursiv se face cu level - 1.
Atunci cand level e 0, inseamna ca am coborat suficient si adaug nodurile in v.

**************************************************************************************************************************

Functia get_best_manager

Am din nou o functie helper care merge pe acelasi principiu.
Helper primeste si o variabila max care retine cel mai mare numar de employees.
In v, retin toti managerii cu numarul maxim de angajati.
Daca nodul curent are mai multi angajati decat max:
    sterg ce era in vector cu free
    refac sizeul = 1
    adagu nodul curent
Daca nodul curent are numarul de angajati egal cu max, doar adaug nodul.

**************************************************************************************************************************

Functia reorganize

Algoritmul se asemenea mult cu bucati din hire si fire.
Iau nodul pe care vreau sa il fac root cu tot cu echipa, il leg la NULL, si il sterg din arborele curent.
Incep sa urc in vechiul copac pana cand ajung la root.
Fiecare nod e scos impreuna cu echipa lui din vechiul copac si adaugat in noul copac.
Variabilele current si manager se plimba in vechiul copac.
Variabila q coboara in noul copac, ca sa adaug noile noduri.
La final, tree devine employeeul dat ca parametru. 