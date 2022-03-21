// menu.cpp : Defines the entry point for the console application.
//


#include<conio.h>
#include<string>
#include<iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

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

    void clearTable(){
        delete(tab);
        tab=NULL;
        cnt=0;
    };
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
        //std::cout<<tabTemp[i]<<" ";
    }

    if (tab != NULL) delete[] tab; //zwolnij pamięć zajmowaną przez stare dane
    //for(int i=0; i<cnt;i++) std::cout<<tabTemp[i];
    tab = tabTemp;
}

void Table::display() {
    cout<<endl;
    for (int i = 0; i < cnt; i++) std::cout << tab[i]<<" ";
}

void Table::generateTable(int size) {
    clearTable();
    while (size){
        addValue(0,rand());
        size--;
    }
}

void Table::deleteFromTable(int index) {
    cnt--;
    int *tabTemp = new int[cnt];

    for(int i=0; i<index; i++) {
        tabTemp[i]=tab[i];
        cout<<"\ntab: "<<tab[i]<<" tabtemp: "<<tabTemp[i];
    }
    for(int i=index; i<cnt;i++) {
        tabTemp[i]=tab[i+1];
        cout<<"\ntab: "<<tab[i+1]<<" tabtemp: "<<tabTemp[i];
    }
    cout<<"\n tablica temp: \n";
    for(int i=0; i<cnt;i++) std::cout<<tabTemp[i];

    if (tab != NULL) delete[] tab; //zwolnij pamięć zajmowaną przez stare dane
    cout<<"\n tablica temp: \n";
    for(int i=0; i<cnt;i++) std::cout<<tabTemp[i];
    tab = tabTemp;
    cout<<"\n tablica: \n";
    for(int i=0; i<cnt;i++) std::cout<<tab[i];
}

int Table::loadFromFile(string FileName) {
    clearTable();
    fstream file;
    file.open(FileName);

    int size, value;
    file >> size;
    while(size){
        file >> value;

        addValue(cnt, value);
        size--;
    }

    file.close();
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

    int loadFromFile(const string& FileName);

    void generateList(int size);

    void clearList();
};

void List2::display() { //wyswietlanie listy od przodu i tylu
    temp=head;
    while(temp != NULL)
    {
        std::cout << temp->data << " ";
        temp  = temp->next;
    }
    std::cout << "\n";
    temp = tail;
    while(temp != NULL)
    {
        std::cout << temp->data << " ";
        temp  = temp->prev;
    }
}

void List2::addValue(int index, int value) {

    ElemList *newbie = new ElemList;
    newbie->data = value;
    newbie->next = NULL; //jesli chce
    ElemList *nextie = look4Index(index);


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
        std::cout<<"\nIndex too big, I look for biggest possible index\n";
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
        maxIndex--;

        //clear memory
        delete temp;
        temp = NULL;

    } else {
        std::cout << "Nothing to delete "<<maxIndex;
    }
}

void List2::generateList(int size) {
    clearList();
    for (int i=0; i<size; i++){
        addValue(maxIndex,rand());
        //std::cout<<".";
    }
}

void List2::clearList() {
    while (maxIndex != 0){

        deleteFromList_byIndex(maxIndex-1);
    }
}

int List2::loadFromFile(const string& FileName) {
    clearList();
    fstream file;
    file.open(FileName);

    int size, value;
    file >> size;

    while(size){
        file >> value;
        addValue(maxIndex, value);
        size--;
    }

    file.close();
    return 0;
}


void displayMenu(const string& info)
{
    std::cout << std::endl;
    std::cout << info << std::endl;
    std::cout << "1.Wczytaj z pliku" << std::endl;
    std::cout << "2.Usun" << std::endl;
    std::cout << "3.Dodaj" << std::endl;
    std::cout << "4.Znajdz" << std::endl;
    std::cout << "5.Utworz losowo" << std::endl;
    std::cout << "6.Wyswietl" << std::endl;
    std::cout << "7.Test (pomiary)" << std::endl;
    std::cout << "0.Powrot do menu" << std::endl;
    std::cout << "Podaj opcje:";
}


Table myTab; //myTab może być dynamiczna, może byc zadeklarowana w manu_table
List2 myList;
//int values[];
//Table indexes;
steady_clock::time_point timeStart, timeEnd;
duration<double> timeTemp, timeSum;

void menu_table()
{
    char opt;
    string fileName;
    int index, value;


    do{
        displayMenu("--- TABLICA ---");
        opt = getche();
        std::cout << std::endl;
        switch (opt){
            case '1': //tutaj wczytytwanie  tablicy z pliku
                std::cout << " Podaj nazwę zbioru:";
                std::cin >> fileName;
                myTab.loadFromFile(fileName);
                myTab.display();
                break;

            case '2': //tutaj usuwanie elemenu z tablicy
                std::cout << " podaj index:";
                std::cin >> index;
                myTab.deleteFromTable(index);
                myTab.display();
                break;

            case '3': //tutaj dodawanie elemetu do tablicy
                std::cout << " podaj index:";
                std::cin >> index;
                std::cout << " podaj waertość:";
                std::cin >> value;

                myTab.addValue(index,value);
                myTab.display();
                break;

            case '4': //tutaj znajdowanie elemetu w tablicy
                std::cout << " podaj waertość:";
                std::cin >> value;
                if (myTab.IsValueInTable(value))
                    std::cout << "poadana wartośc jest w tablicy";
                else
                    std::cout << "poadanej wartości NIE ma w tablicy";
                break;

            case '5':  //tutaj generowanie  tablicy
                std::cout << "Podaj ilość elementów tablicy:";
                std::cin >> value;
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
    int population, trial;


    do{
        displayMenu("--- LISTA ---");
        opt = getche();
        std::cout << std::endl;
        switch (opt){
            case '1': //tutaj wczytytwanie  tablicy z pliku
                std::cout << " Podaj nazwę zbioru:";
                std::cin >> fileName;
                myList.loadFromFile(fileName);
                myList.display();
                break;

            case '2': //tutaj usuwanie elemenu z tablicy
                std::cout << " podaj wartosc:";
                std::cin >> value;
                myList.deleteFromList_byValue(value);
                myList.display();
                break;

            case '3': //tutaj dodawanie elemetu do tablicy
                std::cout << " podaj index:";
                std::cin >> index;
                std::cout << " podaj waertość:";
                std::cin >> value;

                myList.addValue(index,value);
                myList.display();
                break;

            case '4': //tutaj znajdowanie elemetu w tablicy
                std::cout << " podaj waertość:";
                std::cin >> value;
                if (myList.look4Value(value))
                    std::cout << "poadana wartośc jest w tablicy";
                else
                    std::cout << "poadanej wartości NIE ma w tablicy";
                break;

            case '5':  //tutaj generowanie  tablicy
                std::cout << "Podaj ilość elementów tablicy:";
                std::cin >> value;
                myList.generateList(value);
                myList.display();
                break;

            case '6':  //tutaj wyświetlanie tablicy
                myList.display();
                break;

            case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie będzie testowana przez prowadzącego
                // można sobie tu dodać własne case'y

               std::cout<<"\nCo testujesz?"
                     "\n1) Dodaj do przodu"
                     "\n2) dodaj w losowe miejsce"
                     "\n3) dodaj na koniec"
                     "\n4) usun z przodu"
                     "\n5) usun losowe"
                     "\n6) usun z tylu"
                     "\n7) szukaj";

                trial = 100;
                int *values = new int[trial];
                int *indexes = new int [trial];
                std::cin>> opt;
                std::cout << "Podaj wielkosc populacji";
                std::cin >> population;
               switch(opt) {
                   case '1':

                   myList.generateList(population);
                   //dodawanie na poczatek
                   for (int i = 1; i <= 100; i++) {
                       for(int j =0; j<=trial;j++) values[j]=rand();

                       std::cout << std::endl << i;
                       //std::cout << " prev start: " << timeStart. << std::endl;
                       timeStart = steady_clock::now();

                       for (int j = 0; j < trial; j++) {
                           myList.addValue(0, values[i]);
                       }

                       timeEnd = steady_clock::now();

                       timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                       timeSum += timeTemp;
                       std::cout << " timeTemp " << timeTemp.count();
                   }

                   std::cout << "\nZrobilismy 100 populacji wielkosci: " << population;
                   std::cout << "\n Dla kazdej populacji bylo tyle dodawan: " << trial;
                   std::cout << "\n Laczny czas to: ";
                   std::cout << timeSum.count() << "sekund";
                   break;

                   case '2':

                       //dodawanie na losowe
                       for (int i = 1; i <= 100; i++) {
                           myList.generateList(population);
                           for(int j =0; j<=trial;j++) values[j]=rand();
                           for(int j =0; j<=trial;j++) indexes[j]=rand() % population;

                           std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               myList.addValue(indexes[i], values[i]);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           std::cout << " timeTemp " << timeTemp.count();
                       }

                       std::cout << "\nZrobilismy 100 populacji wielkosci: " << population;
                       std::cout << "\n Dla kazdej populacji bylo tyle dodawan: " << trial;
                       std::cout << "\n Laczny czas to: ";
                       std::cout << timeSum.count() << " sekund";
                       break;

                   case '3':

                       //dodawanie na koniec
                       for (int i = 1; i <= 100; i++) {
                           myList.generateList(population);
                           for(int j =0; j<=trial;j++) values[j]=rand();

                           std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               myList.addValue(population+j, values[i]);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           std::cout << " timeTemp " << timeTemp.count();
                       }

                       std::cout << "\nZrobilismy 100 populacji wielkosci: " << population;
                       std::cout << "\n Dla kazdej populacji bylo tyle dodawan: " << trial;
                       std::cout << "\n Laczny czas to: ";
                       std::cout << timeSum.count() << " sekund";
                       break;

                   case '4':
                       //usun z przodu
                       for (int i = 1; i <= 100; i++) {
                           myList.generateList(population);

                           std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               myList.deleteFromList_byIndex(0);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           std::cout << " timeTemp " << timeTemp.count();
                       }

                       std::cout << "\nZrobilismy 100 populacji wielkosci: " << population;
                       std::cout << "\n Dla kazdej populacji bylo tyle usuniec: " << trial;
                       std::cout << "\n Laczny czas to: ";
                       std::cout << timeSum.count() << " sekund";
                       break;

                   case '6':
                       //usun z tylu
                       for (int i = 1; i <= 100; i++) {
                           myList.generateList(population);

                           std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               myList.deleteFromList_byIndex(population-j);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           std::cout << " timeTemp " << timeTemp.count();
                       }

                       std::cout << "\nZrobilismy 100 populacji wielkosci: " << population;
                       std::cout << "\n Dla kazdej populacji bylo tyle usuniec: " << trial;
                       std::cout << "\n Laczny czas to: ";
                       std::cout << timeSum.count() << " sekund";
                       break;

                   case '7':
                       //wyszukaj
                       for (int i = 1; i <= 100; i++) {
                           myList.generateList(population);
                           for(int j =0; j<=trial;j++) values[j]=rand();

                           std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               myList.look4Value(values[j]);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           std::cout << " timeTemp " << timeTemp.count();
                       }

                       std::cout << "\nZrobilismy 100 populacji wielkosci: " << population;
                       std::cout << "\n Dla kazdej populacji bylo tyle wyszukiwan: " << trial;
                       std::cout << "\n Laczny czas to: ";
                       std::cout << timeSum.count() << " sekund";
                       break;

               }
               opt =0;
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
        std::cout << std::endl;
        std::cout << "==== MENU GLOWNE ===" << std::endl;
        std::cout << "1.Tablica" << std::endl;
        std::cout << "2.Lista" << std::endl;
        std::cout << "3.Kopiec" << std::endl;
        std::cout << "0.Wyjscie" << std::endl;
        std::cout << "Podaj opcje:";
        option = getche();
        std::cout << std::endl;

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
