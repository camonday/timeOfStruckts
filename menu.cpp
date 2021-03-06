// menu.cpp : Defines the entry point for the console application.
//


#include<conio.h>
#include<string>
#include<iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

using namespace std;
using namespace std::chrono;

//przykładowa implementacja tablicy ale tylko jest TYLKO szkielet
//staramy się unikać używania funkcji we/wy w klasie (opócz metody  display)
class Table
{
    int *tab=NULL;
    int cnt=0; //ilość elementów w tablicy
public:

    int loadFromFile(string FileName);

    bool IsValueInTable(int val);

    void addValue(int index, int value);

    void deleteFromTable(int index);

    void display();

    void generateTable(int size);

    void clearTable(){ //czyszcze tabele tzn usuwam ja z pamieci i przypisuje jej null, liczbe elementow w tab zmieniam na 0
        if(tab!=NULL){
            delete(tab);
            tab=NULL;
        }
        cnt=0;
    };
};

bool Table::IsValueInTable(int val) {
    for (int i = 0; i < cnt; i++) {
        if (tab[i] == val){
            return true;
        }
    }
    return false;
}

void Table::addValue(int index, int value) {

    cnt++;
    int *tabTemp = new int[cnt];

    for(int i=0; i<index; i++) tabTemp[i]=tab[i];//
    tabTemp[index]=value;//


    for(int i=index, temp; i<cnt-1;i++){
        tabTemp[i+1]=tab[i];
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

    void deleteFromHeap(int index);

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

void Heap::deleteFromHeap(int index) {
    if (index < count) {
        count--;
        heap[index] = heap[count]; //poniewaz indeksujemy od zera to indeks ostatniego elementu to liczba elementow -1
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
        return IsValueInHeap(val,2*ID+2);
    }else return false;
}

int Heap::loadFromFile(string FileName) {
    clearHeap();
    fstream file;
    file.open(FileName);

    int size, value;
    file >> size;
    while(size){
        file >> value;
        addValue(value);
        size--;
    }

    file.close();
    return 0;
}

struct Comma final : std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
};

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
int populations[] = {5000, 8000, 10000, 16000, 20000, 40000, 60000,100000};

steady_clock::time_point timeStart, timeEnd;
duration<double> timeTemp, timeSum;

void menu_table()
{
    char opt;
    string fileName;
    int index, value;
    int trial = 100, population;


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


                std::ofstream file("test_tablicy.txt");
                file.imbue(std::locale(std::locale::classic(), new Comma));
                int *values = new int[trial];
                int *indexes = new int [trial];
                for(int n=0;n<8;n++){
                    population=populations[n];
                    cout<<n;
                    Table tablice[100];
                    cout<<"\nGeneruje tablice ";
                    for(int i=0; i<100; i++){

                        tablice[i].generateTable(population);
                        cout<<i<<" ";
                    }



                    cout<<"\ndodawanie na poczatek";
                    timeSum-=timeSum;
                    for (auto & i : tablice) {
                        for(int j =0; j<=trial;j++) values[j]=rand();

                        //std::cout << std::endl << i;
                        timeStart = steady_clock::now();

                        for (int j = 0; j < trial; j++) {
                            i.addValue(0, values[j]);
                        }

                        timeEnd = steady_clock::now();

                        timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                        timeSum += timeTemp;
                        //std::cout << " timeTemp " << timeTemp.count();
                    }

                    file << "\nZrobilismy 100 populacji wielkosci: " << population;
                    file << "\n Dla kazdej populacji bylo tyle dodawan na poczatek: " << trial;
                    file << "\n Laczny czas to: ";
                    file << timeSum.count() << "sekund\n";




                    cout<<"dodawanie na losowe";
                    timeSum-=timeSum;
                    for (auto & i : tablice) {
                        for(int j =0; j<=trial;j++) values[j]=rand();
                        for(int j =0; j<=trial;j++) indexes[j]=rand() % population;

                        //std::cout << std::endl << i;
                        timeStart = steady_clock::now();

                        for (int j = 0; j < trial; j++) {
                            i.addValue(indexes[j], values[j]);
                        }

                        timeEnd = steady_clock::now();

                        timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                        timeSum += timeTemp;
                        //std::cout << " timeTemp " << timeTemp.count();
                    }

                    file << "\nZrobilismy 100 populacji wielkosci: " << population;
                    file << "\n Dla kazdej populacji bylo tyle dodawan w losowe: " << trial;
                    file << "\n Laczny czas to: ";
                    file << timeSum.count() << " sekund\n";




                    cout<<"dodawanie na koniec";
                    timeSum-=timeSum;
                    for (auto & i : tablice) {
                        //myTab.generateTable(population);
                        for(int j =0; j<=trial;j++) values[j]=rand();

                        //std::cout << std::endl << i;
                        timeStart = steady_clock::now();

                        for (int j = 0; j < trial; j++) {
                            i.addValue(population+j-1, values[j]);
                        }

                        timeEnd = steady_clock::now();

                        timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                        timeSum += timeTemp;
                        //std::cout << " timeTemp " << timeTemp.count();
                    }

                    file << "\nZrobilismy 100 populacji wielkosci: " << population;
                    file << "\n Dla kazdej populacji bylo tyle dodawan na koniec: " << trial;
                    file << "\n Laczny czas to: ";
                    file << timeSum.count() << " sekund\n";



                    cout<<"usun z przodu";
                    timeSum-=timeSum;
                    for (auto & i : tablice) {
                       // myTab.generateTable(population);

                        //std::cout << std::endl << i;
                        timeStart = steady_clock::now();

                        for (int j = 0; j < trial; j++) {
                            i.deleteFromTable(0);
                        }

                        timeEnd = steady_clock::now();

                        timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                        timeSum += timeTemp;
                        //std::cout << " timeTemp " << timeTemp.count();
                    }

                    file << "\nZrobilismy 100 populacji wielkosci: " << population;
                    file << "\n Dla kazdej populacji bylo tyle usuniec z poczatku: " << trial;
                    file << "\n Laczny czas to: ";
                    file << timeSum.count() << " sekund\n";



                    cout<<"usun z losowe";
                    timeSum-=timeSum;
                    for (auto & i : tablice) {
                       // myTab.generateTable(population);
                        for(int j =0; j<=trial;j++) indexes[j]=rand() % (population-101);

                        //std::cout << std::endl << i;
                        timeStart = steady_clock::now();

                        for (int j = 0; j < trial; j++) {
                            i.deleteFromTable(indexes[j]);
                        }

                        timeEnd = steady_clock::now();

                        timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                        timeSum += timeTemp;
                        //std::cout << " timeTemp " << timeTemp.count();
                    }

                    file << "\nZrobilismy 100 populacji wielkosci: " << population;
                    file << "\n Dla kazdej populacji bylo tyle usuniec: " << trial;
                    file << "\n Laczny czas to: ";
                    file << timeSum.count() << " sekund\n";



                    cout<<"usun z tylu";
                    timeSum-=timeSum;
                    for (auto & i : tablice) {
                        //myTab.generateTable(population);

                        //std::cout << std::endl << i;
                        timeStart = steady_clock::now();

                        for (int j = 0; j < trial; j++) {
                            i.deleteFromTable(population-j-1);
                        }

                        timeEnd = steady_clock::now();

                        timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                        timeSum += timeTemp;
                        //std::cout << " timeTemp " << timeTemp.count();
                    }

                    file << "\nZrobilismy 100 populacji wielkosci: " << population;
                    file << "\n Dla kazdej populacji bylo tyle usuniec: " << trial;
                    file << "\n Laczny czas to: ";
                    file << timeSum.count() << " sekund\n";



                    cout<<"wyszukaj";
                    timeSum-=timeSum;

                    for(int j =0; j<=trial;j++) values[j]=rand();
                    //high_resolution_clock::time_point t1 = high_resolution_clock::now();
                    //typedef std::chrono::high_resolution_clock Clock;
                    //auto t1 = Clock::now();

                    double timex=0;
                                            //auto t1 = std::chrono::high_resolution_clock::now();
                    for (auto & i : tablice) {
                        //myTab.generateTable(population);


                        //std::cout << std::endl << i;

                        StartCounter();
                        for (int j = 0; j < trial; j++) {
                            for(int k=0;k<100;k++){ i.IsValueInTable(values[j]+k);}
                        }
                        timex+= GetCounter();
                        //ti



                        //timeSum += time_span;
                        //std::cout << " timeTemp " << time_span.count();
                    }
                    //high_resolution_clock::time_point t2 = high_resolution_clock::now();
                    //auto t2 = Clock::now();
                      //                 std::cout << (t2-t1).count() << '\n';
                    //auto t2 = std::chrono::high_resolution_clock::now();

                    cout<<timex;
                    file<< "\nZrobilismy 100 populacji wielkosci: " << population;
                    file<< "\n Dla kazdej populacji bylo tyle wyszukiwan: " << trial*100;
                    file<< "\n Laczny czas to: ";
                    file<< timex << " sekund";


                }
                file.close();
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
                std::cout << " podaj wartość:";
                std::cin >> value;

                myList.addValue(index,value);
                myList.display();
                break;

            case '4': //tutaj znajdowanie elemetu w tablicy
                std::cout << " podaj wartość:";
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
                std::ofstream file("test_listy.txt");
                file.imbue(std::locale(std::locale::classic(), new Comma));
                trial = 100;
                int *values = new int[trial];
                int *indexes = new int [trial];
                List2 listy[100];
                for(int n=0;n<8;n++){
                    population=populations[n];
                    cout<<n;
                    cout<<"\nGeneruje listy ";

                    for(int i=0; i<100; i++){
                        listy[i].generateList(population);
                        cout<<i<<" ";
                    }


                    timeSum-=timeSum;
                   cout<<" dodawanie na poczatek";
                   for (auto & i : listy) {
                       //i.generateList(population);
                       for(int j =0; j<=trial;j++) values[j]=rand();

                       //std::cout << std::endl << i;
                       //std::cout << " prev start: " << timeStart. << std::endl;
                       timeStart = steady_clock::now();

                       for (int j = 0; j < trial; j++) {
                           i.addValue(0, values[j]);
                       }

                       timeEnd = steady_clock::now();

                       timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                       timeSum += timeTemp;
                       //std::cout << " timeTemp " << timeTemp.count();
                   }

                   file << "\nZrobilismy 100 populacji wielkosci: " << population;
                   file << "\n Dla kazdej populacji bylo tyle dodawan: " << trial;
                   file << "\n Laczny czas to: ";
                   file << timeSum.count() << "sekund";
                   //break;

                   //case '2':
                    timeSum-=timeSum;
                       cout<<"dodawanie na losowe";
                       for (auto & i : listy) {
                       //i.generateList(population);
                           for(int j =0; j<=trial;j++) values[j]=rand();
                           for(int j =0; j<=trial;j++) indexes[j]=rand() % population;

                          // std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               i.addValue(indexes[j], values[j]);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           //std::cout << " timeTemp " << timeTemp.count();
                       }

                       file << "\nZrobilismy 100 populacji wielkosci: " << population;
                       file << "\n Dla kazdej populacji bylo tyle dodawan: " << trial;
                       file << "\n Laczny czas to: ";
                       file << timeSum.count() << " sekund";
                       //break;

                   //case '3':
                    timeSum-=timeSum;
                       cout<<"dodawanie na koniec";
                       for (auto & i : listy) {
                       //i.generateList(population);
                           for(int j =0; j<=trial;j++) values[j]=rand();

                           //std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               i.addValue(population+j-1, values[j]);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           //std::cout << " timeTemp " << timeTemp.count();
                       }

                       file << "\nZrobilismy 100 populacji wielkosci: " << population;
                       file << "\n Dla kazdej populacji bylo tyle dodawan: " << trial;
                       file << "\n Laczny czas to: ";
                       file << timeSum.count() << " sekund";
                       //break;

                   //case '4':
                    timeSum-=timeSum;
                      cout<<"usun z przodu";
                       for (auto & i : listy) {
                       //i.generateList(population);

                           //std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               i.deleteFromList_byIndex(0);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           //std::cout << " timeTemp " << timeTemp.count();
                       }

                       file<< "\nZrobilismy 100 populacji wielkosci: " << population;
                       file<< "\n Dla kazdej populacji bylo tyle usuniec: " << trial;
                       file<< "\n Laczny czas to: ";
                       file<< timeSum.count() << " sekund";
                       //break;

                   //case '5':
                    timeSum-=timeSum;
                      cout<<"usun z losowe";
                       for (auto & i : listy) {
                       //i.generateList(population);
                           for(int j =0; j<=trial;j++) indexes[j]=rand() % (population-101);

                           //std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               i.deleteFromList_byIndex(indexes[j]);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           //std::cout << " timeTemp " << timeTemp.count();
                       }

                       file << "\nZrobilismy 100 populacji wielkosci: " << population;
                       file << "\n Dla kazdej populacji bylo tyle usuniec: " << trial;
                       file << "\n Laczny czas to: ";
                       file << timeSum.count() << " sekund";
                       //break;

                   //case '6':
                    timeSum-=timeSum;
                       cout<<"usun z tylu";
                       for (auto & i : listy) {
                       //i.generateList(population);

                           //std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               i.deleteFromList_byIndex(population-j-1);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           //std::cout << " timeTemp " << timeTemp.count();
                       }

                       file << "\nZrobilismy 100 populacji wielkosci: " << population;
                       file << "\n Dla kazdej populacji bylo tyle usuniec: " << trial;
                       file << "\n Laczny czas to: ";
                       file << timeSum.count() << " sekund";
                       //break;

                   //case '7':
                       cout<<"wyszukaj\n";
                       timeSum-=timeSum;
                       for (auto & i : listy) {
                       //i.generateList(population);
                           for(int j =0; j<=trial;j++) values[j]=rand();

                           //std::cout << std::endl << i;
                           timeStart = steady_clock::now();

                           for (int j = 0; j < trial; j++) {
                               i.look4Value(values[j]);
                           }

                           timeEnd = steady_clock::now();

                           timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                           timeSum += timeTemp;
                           //std::cout << " timeTemp " << timeTemp.count();
                       }

                      file << "\nZrobilismy 100 populacji wielkosci: " << population;
                      file << "\n Dla kazdej populacji bylo tyle wyszukiwan: " << trial;
                      file << "\n Laczny czas to: ";
                      file << timeSum.count() << " sekund";
                       //break;



               }file.close();
               opt =0;
                break;



        }

    } while (opt != '0');
}

void menu_heap()
{
    char opt;
    string fileName;
    int value;
    int population, trial;


    do {
        displayMenu("--- KOPIEC ---");
        opt = getche();
        std::cout << std::endl;
        switch (opt) {
            case '1': //tutaj wczytywanie tablicy z pliku
                std::cout << " Podaj nazwę zbioru:";
                std::cin >> fileName;
                myHeap.loadFromFile(fileName);
                myHeap.display();
                break;

            case '2': //tutaj usuwanie elementu z tablicy
                std::cout << " Podaj indeks:";
                std::cin >> value;
                myHeap.deleteFromHeap(value);
                myHeap.display();
                break;

            case '3': //tutaj dodawanie elementu do tablicy
                std::cout << " podaj wartość:";
                std::cin >> value;

                myHeap.addValue(value);
                myHeap.display();
                break;

            case '4': // znajdź
                std::cout << " podaj wartość:";
                std::cin >> value;

                if (myHeap.IsValueInHeap(value, 0)) {
                    std::cout <<"Jest" ;
                }else std::cout<<"Nie ma";
                break;

            case '5': //utwórz losowo
                std::cout << " podaj wartość:";
                std::cin >> value;

                myHeap.generateHeap(value);
                myHeap.display();
                break;

            case '6': //wyswietl
                myHeap.display();
                break;

            case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie będzie testowana przez prowadzącego
                // można sobie tu dodać własne case'y

                std::ofstream file("test_sterty.txt");
                file.imbue(std::locale(std::locale::classic(), new Comma));
                trial = 100;
                int *values = new int[trial];
                Heap sterty[100];

                for(int n=0;n<8;n++){
                    population=populations[n];
                    cout<<n;
                    cout<<"\nGeneruje sterty ";

                    for(int i=0; i<100; i++){
                        sterty[i].generateHeap(population);
                        cout<<i<<" ";
                    }

                    cout<<"//case '1':";
                    timeSum-=timeSum;
                        for (auto & i : sterty) {
                            //myHeap.generateHeap(population);
                            for(int j =0; j<=trial;j++) values[j]=rand();

                            //std::cout << std::endl << i;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                i.addValue(values[j]);
                            }

                            timeEnd = steady_clock::now();

                            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                            timeSum += timeTemp;
                            //std::cout << " timeTemp " << timeTemp.count();
                        }

                        file << "\nZrobilismy 100 populacji wielkosci: " << population;
                        file << "\n Dla kazdej populacji bylo tyle dodawan: " << trial;
                        file << "\n Laczny czas to: ";
                        file << timeSum.count() << " sekund";
                        //break;

                    cout<<"case '2':";
                    timeSum-=timeSum;
                    for (auto & i : sterty) {
                        //myHeap.generateHeap(population);

                            //std::cout << std::endl << i;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                i.deleteFromHeap(0);
                            }

                            timeEnd = steady_clock::now();

                            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                            timeSum += timeTemp;
                            //std::cout << " timeTemp " << timeTemp.count();
                        }

                        file << "\nZrobilismy 100 populacji wielkosci: " << population;
                        file << "\n Dla kazdej populacji bylo tyle usuniec: " << trial;
                        file << "\n Laczny czas to: ";
                        file << timeSum.count() << " sekund";
                        //break;

                    cout<<"case '3':\n";
                    timeSum-=timeSum;
                    for (auto & i : sterty) {
                        //myHeap.generateHeap(population);
                            for(int j =0; j<=trial;j++) values[j]=rand();

                            //std::cout << std::endl << i;
                            timeStart = steady_clock::now();

                            for (int j = 0; j < trial; j++) {
                                i.IsValueInHeap(values[j], 0);
                            }

                            timeEnd = steady_clock::now();

                            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                            timeSum += timeTemp;
                            //std::cout << " timeTemp " << timeTemp.count();
                        }

                        file << "\nZrobilismy 100 populacji wielkosci: " << population;
                        file << "\n Dla kazdej populacji bylo tyle wyszukan: " << trial;
                        file << "\n Laczny czas to: ";
                        file << timeSum.count() << " sekund";
                        //break;


                }file.close();
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
