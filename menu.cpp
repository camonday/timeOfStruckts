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

    void clearTable(){ //czyszcze tabele tzn usuwam ja z pamieci i przypisuje jej null, liczbe elementow w tab zmieniam na 0
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
    }
    for(int i=index; i<cnt;i++) {
        tabTemp[i]=tab[i+1];
    }

    if (tab != NULL) delete[] tab; //zwolnij pamięć zajmowaną przez stare dane
    tab = tabTemp;
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

    ElemList *head = NULL;  //na poczatku lista jest pusta wiec nie ma ani head ani tail
    ElemList *tail = NULL;
    ElemList *temp = NULL;
    int maxIndex=0; //maksymalny indeks jaki mozna dodac

public:

    void display();

    ElemList * look4Index(int index);
    ElemList * look4Value(int value, int dif); //dif jest redundantne by rozroznic funkcje ktora zwraca wskaznik od tej ktora zwraca wartosc bool

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

void List2::addValue(int index, int value) { //dodawanie wartosci

    ElemList *newbie = new ElemList;
    newbie->data = value;
    //newbie->next = NULL; //
    ElemList *nextie = look4Index(index);


    newbie->next = nextie; //newbie is gonna know, he is b4 nextie
    if (nextie != NULL) { //jesli nie dodalismy ogona
        newbie->prev = nextie->prev;
        nextie->prev = newbie;
    } else{//jesli dodalismy ogon
        if (tail != NULL) tail->next=newbie;
        newbie->prev=tail;
        tail = newbie;
    }

    if(newbie->prev != NULL) newbie->prev->next= newbie; //jesli nie dodalismy glowy
    else {//jesli dodalismy glowe
        head = newbie;
    }

    maxIndex++; //zwiekszamy ilosc elementow w tablicy

}

List2::ElemList * List2::look4Index(int index) {
    temp=head;
    if (maxIndex < index){
        //std::cout<<"\nIndex too big, I look for biggest possible index\n";
        index = maxIndex;
    }
    for(int i=0; i<index;i++){
        temp=temp->next;
    }
    //temp jest tym co szukalismy
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
    return temp; //temp jest tym co szukalismy
}

void List2::deleteFromList_byValue(int value) {
    temp = look4Value(value,1);
    deleteFromList_byIndex( -1);
}

void List2::deleteFromList_byIndex(int index) {
    if(index>=0) look4Index(index); //jesli index =-1, to temp jest juz ustawiony na wynik look4value, w przeciwnym wypadku ustawimy go na wynik
                                    // look4Index
    if (temp != NULL){

        if (temp -> next != NULL){ //jesli temp ma potomka to ten potomek stoi teraz za tym za czym stal temp a nie za temp
            temp->next->prev = temp->prev;
        } else {
            tail=temp->prev; //jesli nie ma potomka to byl ogonem wiec teraz ogonem bedzie to co stalo przed temp
        }

        if (temp->prev !=NULL){ //analogicznie jesli temp ma/nie ma rodzica
            temp->prev->next = temp->next;
        } else{
            head = temp -> next;
        }
        maxIndex--; //zmniejszamy liczbe elementów listy

        //clear memory
        delete temp;
        temp = NULL;

    } else { //nie znalezlismy wartosci która mamy usunac
        std::cout << "Nothing to delete "<<maxIndex;
    }
}

void List2::generateList(int size) {
    clearList(); //czyscimy liste
    for (int i=0; i<size; i++){
        addValue(maxIndex,rand());
    }
}

void List2::clearList() {
    while (maxIndex != 0){ //usuwamy kazdy element z kisty tak dlugo, az lista ma zero elementow
        deleteFromList_byIndex(maxIndex-1);
    }
}

int List2::loadFromFile(const string& FileName) {
    clearList(); //czyscimy liste
    fstream file;
    file.open(FileName);

    int size, value;
    file >> size; //pierwsza liczba w pliku to rozmiar listy

    while(size!=maxIndex){
        file >> value;
        addValue(maxIndex, value); //addvalue zwieksza maxIndex wiec petla zatrzyma sie
        //size--;
    }

    file.close();
    return 0;
}


class Heap {

    int *heap = NULL;
    int count=0;// count - ile wezlow jest w stercie
    int tabSize=0;

public:
    int loadFromFile(string FileName);

    bool IsValueInHeap(int val, int ID);

    void addValue(int value);

    void deleteFromHeap();

    void display();

    void generateHeap(int size);

    void clearHeap();

    void upsize();
    void downsize();

    void heap_fix_up(int index);
    void heap_fix_down(int index);

    void printBT(const std::string& prefix, int index, bool isLeft);

};


void Heap::addValue(int value) {
    if(tabSize == count) upsize();
    heap[count]=value;

    heap_fix_up(count);

    count++;
}

void Heap::upsize() {
    tabSize+=10;
    int *tabTemp = new int[tabSize];

    for(int i=0; i<count; i++){
        tabTemp[i]=heap[i];
    }

    if (heap != NULL) delete[] heap; //zwolnij pamięć zajmowaną przez stare dane
    heap = tabTemp;
}

void Heap::display() {
    cout<<endl;
    for(int i=0; i<count; i++) cout<< heap[i]<<" ";

    printBT("",0,false);
    cout<<endl;
}

void Heap::printBT(const std::string& prefix, int index, bool isLeft)
{
    if( index<count )
    {
        std::cout << std::endl <<  prefix;
        std::cout << (isLeft ? "I--" : "'--" );

        // print the value of the node
        std::cout << heap[index] ;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "I   " : "    "), 2*index+1, true);
        printBT( prefix + (isLeft ? "I   " : "    "), 2*index+2, false);
    }
}

void Heap::heap_fix_up(int index) {

    if (index > 0) {

        int parentID = (index-1)/2;

        if (heap[parentID] < heap[index]) {
            heap[parentID] += heap[index];
            heap[index] = heap[parentID] - heap[index];
            heap[parentID]-=heap[index];

            heap_fix_up(parentID);
        }
    }
}

void Heap::deleteFromHeap() {
    if (count > 0) {
        count--;
        heap[0] = heap[count]; //poniewaz indeksujemy od zera to indeks ostatniego elementu to liczba elementow -1
        if (count <= tabSize - 10) downsize();

        heap_fix_down(0);
    }
}

void Heap::downsize() {
    tabSize-=10;
    int *tabTemp = new int[tabSize];

    for(int i=0; i<count; i++){
        tabTemp[i]=heap[i];
    }

    if (heap != NULL) delete[] heap; //zwolnij pamięć zajmowaną przez stare dane
    heap = tabTemp;

}

void Heap::heap_fix_down(int index) {
    int bigKidID = 2*index +1;
    int otherKidID = bigKidID +1;
    //sprawdzmy czy ma dzieci
    if(otherKidID>count){ //jesli jest równa, to ma tylko lewe dziecko
        return;
    }
    if(otherKidID<count){ //teraz sprawdzamy czy po lewej stronie faktycznie jest wieksze dziecko
        if(heap[otherKidID]>heap[bigKidID]){
            bigKidID=otherKidID; //big kid napewno ma indeks wiekszego dziecka
        }
    }
    //teraz zamiana
    if(heap[index]<heap[bigKidID]){
        swap(heap[index],heap[bigKidID]);
    }else return;

    heap_fix_down(bigKidID);

}

void Heap::clearHeap() {
    if (heap != NULL){
        delete heap;
        heap = NULL;
        count = 0;
        tabSize = 0;
    }

}

void Heap::generateHeap(int size) {
    clearHeap();
    while (count<size){
        addValue(rand());
    }

}

bool Heap::IsValueInHeap(int val, int ID) {
    if(ID <count){
        if (heap[ID]==val)return true;
        if (heap[ID]<val) return false;
        if (IsValueInHeap(val,2*ID+1)) return true;
        return IsValueInHeap(val,2*ID+1);
    }else return false;
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
Heap myHeap;

steady_clock::time_point timeStart, timeEnd;
duration<double> timeTemp, timeSum;

void menu_table()
{
    char opt;
    string fileName;
    int index, value;
    int trial, population;


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

                        myTab.generateTable(population);
                        //dodawanie na poczatek
                        for (int i = 1; i <= 100; i++) {
                            for(int j =0; j<=trial;j++) values[j]=rand();

                            std::cout << std::endl << i;
                            //std::cout << " prev start: " << timeStart. << std::endl;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                myTab.addValue(0, values[i]);
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
                            myTab.generateTable(population);
                            for(int j =0; j<=trial;j++) values[j]=rand();
                            for(int j =0; j<=trial;j++) indexes[j]=rand() % population;

                            std::cout << std::endl << i;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                myTab.addValue(indexes[i], values[i]);
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
                            myTab.generateTable(population);
                            for(int j =0; j<=trial;j++) values[j]=rand();

                            std::cout << std::endl << i;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                myTab.addValue(population+j, values[i]);
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
                            myTab.generateTable(population);

                            std::cout << std::endl << i;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                myTab.deleteFromTable(0);
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

                    case '5':
                        //usun z losowe
                        for (int i = 1; i <= 100; i++) {
                            myTab.generateTable(population);
                            for(int j =0; j<=trial;j++) indexes[j]=rand() % population;

                            std::cout << std::endl << i;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                myTab.deleteFromTable(indexes[j]);
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
                            myTab.generateTable(population);

                            std::cout << std::endl << i;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                myTab.deleteFromTable(population-j);
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
                            myTab.generateTable(population);
                            for(int j =0; j<=trial;j++) values[j]=rand();

                            std::cout << std::endl << i;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                myTab.IsValueInTable(values[j]);
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

                   case '5':
                       //usun z losowe
                       for (int i = 1; i <= 100; i++) {
                           myList.generateList(population);
                           for(int j =0; j<=trial;j++) indexes[j]=rand() % population;

                           std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               myList.deleteFromList_byIndex(indexes[j]);
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
    char opt;
    string fileName;
    int index, value;
    int population, trial;


    do {
        displayMenu("--- KOPIEC ---");
        opt = getche();
        std::cout << std::endl;
        switch (opt) {
            case '1': //tutaj wczytytwanie  tablicy z pliku
                std::cout << " Podaj nazwę zbioru:";
                std::cin >> fileName;
                myList.loadFromFile(fileName);
                myList.display();
                break;

            case '2': //tutaj usuwanie elemenu z tablicy
                myHeap.deleteFromHeap();
                myHeap.display();
                break;

            case '3': //tutaj dodawanie elemetu do tablicy
                std::cout << " podaj waertość:";
                std::cin >> value;

                myHeap.addValue(value);
                myHeap.display();
                break;

            case '4': // znajdz
                std::cout << " podaj waertość:";
                std::cin >> value;

                if (myHeap.IsValueInHeap(value, 0)) {
                    std::cout <<"Jest" ;
                }else std::cout<<"Nie ma";
                break;

            case '5': //utwórz losowo
                std::cout << " podaj waertość:";
                std::cin >> value;

                myHeap.generateHeap(value);
                myHeap.display();
                break;
        }
    }while(opt!='0');
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
