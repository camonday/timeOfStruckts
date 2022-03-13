// menu.cpp : Defines the entry point for the console application.
//


#include<conio.h>
#include<string>
#include<iostream>
using namespace std;


//przykładowa implementacja tablicy ale tylko jest TYLKO szkielet
//staramy się unikać używania funkcji we/wy w klasie (opócz metody  display)
class Table
{
    int *tab;
    int cnt=0; //ilość elementów w tablicy
public:

    int loadFromFile(string FileName);

    bool IsValueInTable(int val);

    void addValue(int index, int value);

    void deleteFromTable(int index);

    void display();

    void generateTable(int size);

};

bool Table::IsValueInTable(int val) {
    for (int i = 0; i < cnt; i++) {
        if (tab[i] == val) return true;
    }
    return false;
}

void Table::addValue(int index, int value) {

    cnt++;
    int *tabTemp = new int[cnt];

    for(int i=0; i<index; i++) tabTemp[i]=tab[i];
    tabTemp[index]=value;


    for(int i=index, temp; i<cnt-1;){
        temp = tab[i];
        tabTemp[++i]=temp;
        cout<<tabTemp[i]<<" ";
    }

    if (tab != NULL) delete[] tab; //zwolnij pamięć zajmowaną przez stare dane
    for(int i=0; i<cnt;i++) cout<<tabTemp[i];
    tab = tabTemp;
}

void Table::display() {
    for (int i = 0; i < cnt; i++) cout << tab[i];
}

void Table::generateTable(int size) {

}

void Table::deleteFromTable(int index) {

}

int Table::loadFromFile(string FileName) {
    return 0;
}

class List2 {

    struct ElemList{
        int data;
        ElemList *prev;
        ElemList *next;
    };

    ElemList *head = NULL;
    ElemList *tail = NULL;
    ElemList *temp = NULL;

public:

    void display();

    ElemList * look4Index(int index);

    bool look4Value(int value);

    void addValue(int index, int value);

    void deleteFromList(int index);

    bool IsValueInList(int val);

    int loadFromFile(string FileName);

    void generateTable(int size);
};

void List2::display() { //wyswietlanie listy od przodu i tylu
    temp=head;
    while(temp != NULL)
    {
        cout << temp->data << " ";
        temp  = temp->next;
    }
    cout << "\n";
    temp = tail;
    while(temp != NULL)
    {
        cout << temp->data << " ";
        temp  = temp->prev;
    }
}

void List2::addValue(int index, int value) {

    ElemList *newbie = new ElemList;
    newbie->data = value;
    newbie->next = NULL; //jesli chce
    ElemList *previe = look4Index(index-1);

    newbie->prev = previe;
    if (previe != NULL) {
    newbie->next = previe->next;
    previe->next = newbie;
    } else head = newbie;
    if(newbie->next != NULL) newbie->next->prev= newbie;
    else tail = newbie;

}

List2::ElemList * List2::look4Index(int index) {
    temp=head;
    for(int i=0; i<=index;i++) temp=temp->next;
    return temp;
}

bool List2::look4Value(int value) { //szukam wartosci
    temp=head;
    while(temp != NULL){
        if(temp->data == value) return true;
        temp = temp->next;
    }
    return false;
}


void displayMenu(string info)
{
    cout << endl;
    cout << info << endl;
    cout << "1.Wczytaj z pliku" << endl;
    cout << "2.Usun" << endl;
    cout << "3.Dodaj" << endl;
    cout << "4.Znajdz" << endl;
    cout << "5.Utworz losowo" << endl;
    cout << "6.Wyswietl" << endl;
    cout << "7.Test (pomiary)" << endl;
    cout << "0.Powrot do menu" << endl;
    cout << "Podaj opcje:";
}


Table myTab; //myTab może być dynamiczna, może byc zadeklarowana w manu_table

void menu_table()
{
    char opt;
    string fileName;
    int index, value;


    do{
        displayMenu("--- TABLICA ---");
        opt = getche();
        cout << endl;
        switch (opt){
            case '1': //tutaj wczytytwanie  tablicy z pliku
                cout << " Podaj nazwę zbioru:";
                cin >> fileName;
                myTab.loadFromFile(fileName);
                myTab.display();
                break;

            case '2': //tutaj usuwanie elemenu z tablicy
                cout << " podaj index:";
                cin >> index;
                myTab.deleteFromTable(index);
                myTab.display();
                break;

            case '3': //tutaj dodawanie elemetu do tablicy
                cout << " podaj index:";
                cin >> index;
                cout << " podaj waertość:";
                cin >> value;

                myTab.addValue(index,value);
                myTab.display();
                break;

            case '4': //tutaj znajdowanie elemetu w tablicy
                cout << " podaj waertość:";
                cin >> value;
                if (myTab.IsValueInTable(value))
                    cout << "poadana wartośc jest w tablicy";
                else
                    cout << "poadanej wartości NIE ma w tablicy";
                break;

            case '5':  //tutaj generowanie  tablicy
                cout << "Podaj ilość elementów tablicy:";
                cin >> value;
                myTab.generateTable(value);
                myTab.display();
                break;

            case '6':  //tutaj wyświetlanie tablicy
                myTab.display();
                break;

            case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie będzie testowana przez prowadzącego
                // można sobie tu dodać własne case'y
                break;
        }

    } while (opt != '0');
}

void menu_list()
{
    //analogicznie jak menu_table()
}

void menu_heap()
{
    //analogicznie jak menu_table()
}

int main(int argc, char* argv[])
{

    char option;
    do{
        cout << endl;
        cout << "==== MENU GLOWNE ===" << endl;
        cout << "1.Tablica" << endl;
        cout << "2.Lista" << endl;
        cout << "3.Kopiec" << endl;
        cout << "0.Wyjscie" << endl;
        cout << "Podaj opcje:";
        option = getche();
        cout << endl;

        switch (option){
            case '1':
                menu_table();
                break;

            case '2':
                menu_list();
                break;

            case '3':
                menu_heap();
                break;
        }

    } while (option != '0');


    return 0;
}
