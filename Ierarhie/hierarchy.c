#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"


Tree find(Tree tree, char *searched_name) {
    if(tree == NULL || searched_name == NULL)
        return NULL;
    if(!strcmp(tree->name, searched_name)){
        return tree;
    }
    for(int i = 0; i < tree->direct_employees_no; i++) {
        Tree search = find(tree->team[i], searched_name);
        if(search != NULL)
            return search;
    }
    return NULL;
}

/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */
Tree hire(Tree tree, char *employee_name, char *manager_name) {
    Tree newEmployee = malloc(sizeof(TreeNode));
    newEmployee->name = calloc(30, sizeof(char));
    strcpy(newEmployee->name, employee_name);
    newEmployee->direct_employees_no = 0;
    newEmployee->team = NULL;


    if(tree == NULL && manager_name == NULL) {
        newEmployee->manager = NULL;
        return newEmployee;
    }

    Tree manager = find(tree, manager_name);
    if(manager == NULL) {
        free(newEmployee->name);
        free(newEmployee);
        return tree;
    }
    
    manager->direct_employees_no++;
    manager->team = realloc(manager->team, sizeof(Tree) * manager->direct_employees_no);
    newEmployee->manager = manager;

    int i = manager->direct_employees_no - 2;
    while(i >= 0 && strcmp(manager->team[i]->name, employee_name) > 0) {
        manager->team[i + 1] = manager->team[i];
        i--;
    }
    manager->team[i + 1] = newEmployee;
    return tree;

        
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire(Tree tree, char *employee_name) {
    if(tree == NULL)
        return NULL;
    Tree employee = find(tree, employee_name);
    if(employee == NULL)
        return tree;
    Tree manager = employee->manager;
    if(manager == NULL)
        return tree;

    int pos;
    for(pos = 0; strcmp(manager->team[pos]->name, employee_name); pos++);

    for(; pos < manager->direct_employees_no - 1; pos++) {
        manager->team[pos] = manager->team[pos + 1];
    }
    manager->direct_employees_no--;
    manager->team = realloc(manager->team, sizeof(Tree) * (manager->direct_employees_no + employee->direct_employees_no));

    int i, j;
    for(i = 0; i < employee->direct_employees_no; i++) {
        for(j = manager->direct_employees_no - 1; j >= 0 && strcmp(manager->team[j]->name, employee->team[i]->name) > 0; j--) {
            manager->team[j + 1] = manager->team[j];
        }
        manager->team[j + 1] = employee->team[i];
        manager->direct_employees_no++;
        employee->team[i]->manager = manager;
    }
    free(employee->name);
    free(employee->team);
    free(employee);
    
    return tree;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */
Tree promote(Tree tree, char *employee_name) {
    Tree employee = find(tree, employee_name);
    if(employee == NULL || employee->manager == NULL || employee->manager->manager == NULL) {
        return tree;
    }
    Tree manager = employee->manager;
    tree = fire(tree, employee_name);
    tree = hire(tree, employee_name, manager->manager->name);
    return tree;
}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name) {
    Tree employee = find(tree, employee_name);
    if(employee == NULL || employee->manager == NULL) {
        return tree;
    }
    if(!strcmp(new_manager_name, employee->manager->name)) {
        return tree;
    }
    tree = fire(tree, employee_name);
    tree = hire(tree, employee_name, new_manager_name);
    return tree;
}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name) {
    Tree employee = find(tree, employee_name);
    Tree NewManager = find(tree, new_manager_name);

    if(employee == NULL || NewManager == NULL) {
        return tree;
    }

    if(find(employee, new_manager_name)) {
        return tree;
    }

    Tree OldManager = employee->manager;
    int pos;
    for(pos = 0; strcmp(OldManager->team[pos]->name, employee_name); pos++);

    for(; pos < OldManager->direct_employees_no - 1; pos++) {
        OldManager->team[pos] = OldManager->team[pos + 1];
    }
    OldManager->direct_employees_no--;
    OldManager->team = realloc(OldManager->team, sizeof(Tree) * OldManager->direct_employees_no);

    NewManager->direct_employees_no++;
    NewManager->team = realloc(NewManager->team, sizeof(Tree) * NewManager->direct_employees_no);
    employee->manager = NewManager;
    int i = NewManager->direct_employees_no - 2;
    while(i >= 0 && strcmp(NewManager->team[i]->name, employee_name) > 0) {
        NewManager->team[i + 1] = NewManager->team[i];
        i--;
    }
    NewManager->team[i + 1] = employee;
    return tree;
    
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire_team(Tree tree, char *employee_name) {
    Tree employee = find(tree, employee_name);
    if(employee == NULL || employee->manager == NULL)
        return tree;
    int no = employee->direct_employees_no;
    for(int i = 0; i < no; i++) {
        tree = fire_team(tree, employee->team[0]->name);
    }
    tree = fire(tree, employee_name);
    return tree;
}

/* Functie ajutatoare pentru get_employees_by_manager
 *
 */
void helper_empl_by_manager(char ***v, int *size, Tree tree) {
    (*size)++;
    *v = realloc(*v, *size * sizeof(char*));
    (*v)[*size - 1] = calloc(30, sizeof(char));

    int j;
    for(j = *size - 2; j >=0 && strcmp((*v)[j], tree->name) > 0; j--) {
        strcpy((*v)[j + 1], (*v)[j]);
    }
    strcpy((*v)[j + 1], tree->name);

    int i = 0;
    while(i < tree->direct_employees_no) {
        helper_empl_by_manager(v, size, tree->team[i]);
        i++;
    }
    return;
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */
void get_employees_by_manager(FILE *f, Tree tree, char *employee_name) {
    if(tree == NULL)
        return;
    Tree employee = find(tree, employee_name);
    if(employee == NULL) {
        fprintf(f, "\n");
        return;
    }
    int size = 0;
    char **v = NULL;
    helper_empl_by_manager(&v, &size, employee);
    
    for(int i = 0; i < size; i++) {
        fprintf(f, "%s ", v[i]);
        free(v[i]);
    }
    free(v);
    fprintf(f, "\n");
    return;
}

/* Functie ajutatoare pentru get_employees_by_level
 *
 */
void helper_empl_by_lvl(char ***v, int *size, Tree tree, int level) {
    int i = 0;
    if(level == 0) {
        (*size)++;
        *v = realloc(*v, *size * sizeof(char*));
        (*v)[*size - 1] = calloc(30, sizeof(char));

        int j;
        for(j = *size - 2; j >=0 && strcmp((*v)[j], tree->name) > 0; j--) {
            strcpy((*v)[j + 1], (*v)[j]);
        }
        strcpy((*v)[j + 1], tree->name);
        return;
    }
    else {
        for(i = 0; i < tree->direct_employees_no; i++)
            helper_empl_by_lvl(v, size, tree->team[i], level - 1);
    }
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */
void get_employees_by_level(FILE *f, Tree tree, int level) {
    if(tree == NULL)
        return;
    if(level < 0) {
        fprintf(f, "\n");
        return;
    }

    int size = 0;
    char **v = NULL;
    helper_empl_by_lvl(&v, &size, tree, level);
    
    for(int i = 0; i < size; i++) {
        fprintf(f, "%s ", v[i]);
        free(v[i]);
    }
    free(v);
    fprintf(f, "\n");
    return;
}

/* Functie ajutatoare pentru get_best_manager
 *
 */
void helper_best_manager(char ***v, int *size, Tree tree, int *max) {
    if(tree->direct_employees_no > *max) {
        while(*size > 0) {
            free((*v)[*size - 1]);
            (*size)--;
        }
        *size = 1;
        *v = realloc(*v, *size * sizeof(char*));
        (*v)[*size - 1] = calloc(30, sizeof(char));
        strcpy((*v)[*size - 1], tree->name);
        *max = tree->direct_employees_no;
    }
    else if(tree->direct_employees_no == *max) {
        (*size)++;
        *v = realloc(*v, *size * sizeof(char*));
        (*v)[*size - 1] = calloc(30, sizeof(char));

        int j;
        for(j = *size - 2; j >=0 && strcmp((*v)[j], tree->name) > 0; j--) {
            strcpy((*v)[j + 1], (*v)[j]);
        }
        strcpy((*v)[j + 1], tree->name);
    }

    int i;
    for(i = 0; i < tree->direct_employees_no; i++) {
        helper_best_manager(v, size, tree->team[i], max);
    }
    return;
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void get_best_manager(FILE *f, Tree tree) {
    if(tree == NULL)
        return;
    int size = 0;
    char **v = NULL;
    int max = -1;

    helper_best_manager(&v, &size, tree, &max);

    for(int i = 0; i < size; i++) {
        fprintf(f, "%s ", v[i]);
        free(v[i]);
    }
    free(v);
    fprintf(f, "\n");    
    return;
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
Tree reorganize(Tree tree, char *employee_name) {
    if(tree == NULL)
        return tree;
    Tree employee = find(tree, employee_name);
    if(employee == NULL || employee->manager == NULL)
        return tree;
    Tree manager = employee->manager;
    employee->manager = NULL;
    Tree q = employee;

    //Pun nodul employee ca root intr-un nou copac
    int pos;
    for(pos = 0; strcmp(manager->team[pos]->name, employee_name); pos++);

    for(; pos < manager->direct_employees_no - 1; pos++) {
        manager->team[pos] = manager->team[pos + 1];
    }
    manager->direct_employees_no--;
    manager->team = realloc(manager->team, sizeof(Tree) * manager->direct_employees_no);

    Tree current = manager;
    manager = current->manager;

    while(current != tree) {
        //adaugam nodul si echipa in noul copac
        current->manager = q;
        q->direct_employees_no++;
        q->team = realloc(q->team, sizeof(Tree) * q->direct_employees_no);

        int i = q->direct_employees_no - 2;
        while(i >= 0 && strcmp(q->team[i]->name, current->name) > 0) {
            q->team[i + 1] = q->team[i];
            i--;
        }
        q->team[i + 1] = current;

        //sterg legatura din vechiul copac
        for(pos = 0; strcmp(manager->team[pos]->name, current->name); pos++);
        for(; pos < manager->direct_employees_no - 1; pos++) {
            manager->team[pos] = manager->team[pos + 1];
        }
        manager->direct_employees_no--;
        manager->team = realloc(manager->team, sizeof(Tree) * manager->direct_employees_no);


        q = current;
        current = manager;
        manager = manager->manager;
    }

    //adaug nodul root si echipa lui
    current->manager = q;
    q->direct_employees_no++;
    q->team = realloc(q->team, sizeof(Tree) * q->direct_employees_no);
    int i = q->direct_employees_no - 2;
    while(i >= 0 && strcmp(q->team[i]->name, current->name) > 0) {
        q->team[i + 1] = q->team[i];
        i--;
    }
    q->team[i + 1] = current;

    tree = employee;

    return tree;
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void preorder_traversal(FILE *f, Tree tree) {
    if(tree == NULL)
        return;
    fprintf(f, "%s", tree->name);
    if(tree->manager != NULL)
        fprintf(f, "-%s ", tree->manager->name);
    else
        fprintf(f, " ");
    int i = 0;
    while(i < tree->direct_employees_no) {
        preorder_traversal(f, tree->team[i]);
        i++;
    }
    if(tree->manager == NULL)
        fprintf(f, "\n");
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree) {
    if(tree == NULL)
        return;
    for(int i = 0; i < tree->direct_employees_no; i++) {
        destroy_tree(tree->team[i]);
    }
    free(tree->name);
    free(tree->team);
    free(tree);
    return;
}

