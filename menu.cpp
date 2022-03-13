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
    int maxIndex=0;

public:

    void display();

    ElemList * look4Index(int index);
    ElemList * look4Value(int value, int dif); //dif is

    bool look4Value(int value);

    void addValue(int index, int value);

    void deleteFromList_byValue(int value);
    void deleteFromList_byIndex(int index);

    int loadFromFile(string FileName);

    void generateList(int size);

    void clearList();
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
    ElemList *nextie = look4Index(index - 1);


    newbie->next = nextie; //newbie is gonna know, he is b4 nextie
    if (nextie != NULL) {
        newbie->prev = nextie->prev;
        nextie->prev = newbie;
    } else{
        if (tail != NULL) tail->next=newbie;
        newbie->prev=tail;
        tail = newbie;
    }

    if(newbie->prev != NULL) newbie->prev->next= newbie;
    else {
        head = newbie;
    }

    maxIndex++;

}

List2::ElemList * List2::look4Index(int index) {
    temp=head;
    if (maxIndex < index){
        cout<<"\nIndex too big, I look for biggest possible index\n";
        index = maxIndex;
    }
    for(int i=0; i<index;i++){
        temp=temp->next;
    }
    return temp;
}

bool List2::look4Value(int value) { //szukam wartosci
    if(look4Value(value,1) !=NULL) return true;
    return false;
}

List2::ElemList *List2::look4Value(int value, int dif) { // dif is redundant, used to diferenciate betwenn elemlist and bool function
    temp=head;
    while(temp != NULL){
        if(temp->data == value) break;
        temp = temp->next;
    }
    return temp;
}

void List2::deleteFromList_byValue(int value) {
    temp = look4Value(value,1);
    deleteFromList_byIndex( -1);
}

void List2::deleteFromList_byIndex(int index) {
    if(index>=0) look4Index(index); //jesli index =-1, to temp jest juz ustawiony
    if (temp != NULL){

        if (temp -> next != NULL){
            temp->next->prev = temp->prev;
        } else {
            tail=temp->prev;
        }

        if (temp->prev !=NULL){
            temp->prev->next = temp->next;
        } else{
            head = temp -> next;
        }

        //clear memory
        delete temp;
        temp = NULL;
        maxIndex--;
    } else {
        cout << "Nothing to delete";
    }
}

void List2::generateList(int size) {
    clearList();
    for (int i=0; i<size; i++){
        addValue(maxIndex,rand());
    }
}

void List2::clearList() {
    while (maxIndex != 0){
        deleteFromList_byIndex(maxIndex);
    }
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
List2 myList;

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
    char opt;
    string fileName;
    int index, value;


    do{
        displayMenu("--- LISTA ---");
        opt = getche();
        cout << endl;
        switch (opt){
            case '1': //tutaj wczytytwanie  tablicy z pliku
                cout << " Podaj nazwę zbioru:";
                cin >> fileName;
                //myList.loadFromFile(fileName);
                myList.display();
                break;

            case '2': //tutaj usuwanie elemenu z tablicy
                cout << " podaj wartosc:";
                cin >> value;
                myList.deleteFromList_byValue(value);
                myList.display();
                break;

            case '3': //tutaj dodawanie elemetu do tablicy
                cout << " podaj index:";
                cin >> index;
                cout << " podaj waertość:";
                cin >> value;

                myList.addValue(index,value);
                myList.display();
                break;

            case '4': //tutaj znajdowanie elemetu w tablicy
                cout << " podaj waertość:";
                cin >> value;
                if (myList.look4Value(value))
                    cout << "poadana wartośc jest w tablicy";
                else
                    cout << "poadanej wartości NIE ma w tablicy";
                break;

            case '5':  //tutaj generowanie  tablicy
                cout << "Podaj ilość elementów tablicy:";
                cin >> value;
                myList.generateList(value);
                myList.display();
                break;

            case '6':  //tutaj wyświetlanie tablicy
                myList.display();
                break;

            case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie będzie testowana przez prowadzącego
                // można sobie tu dodać własne case'y
                break;
        }

    } while (opt != '0');
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
