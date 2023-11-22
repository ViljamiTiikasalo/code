/* Kirjasto
 *
 * Kuvaus:
 * Ohjelma kysyy ensin käyttäjältä lähdetiedoston nimeä ja lukee sieltä tietoa
 * kirjastoista ja kirjastossa olevien kirjojen, kirjailijoiden sekä
 * varausjonojen tiedot ja tallettaa ne paikalliseen tietorakenteeseen.
 * Tiedostosta lukemisen jälkeen ohjelma jää odottamaan käyttäjältä syötettä.
 * Ohjelma hyväksyy syötteeksi kuutta erilaista käskyä: libraries - tulostaa
 * aakkosjärjestyksessä tiedostossa olleet kirjastot. material <kirjaston_nimi>
 * - joka tulostaa kyseisessä kirjastossa olevat kirjat aakkosjärjestyksessä
 * kirjailijan mukaan. books <kirjasto><kirjoittaja> - tulostaa
 * aakkosjärjestyksessä tietyn kirjailijan tietyssä kirjastossa olevat kirjat.
 * reservable <kirjailija><kirjan_nimi> - tulostaa haetun kirjan lyhimmän
 * varausjonon ja kirjastot missä lyhin varausjono on aakkosjärjestyksessä.
 * loanable - tulostaa jokaisen kirjaston jokaisen heti hyllyssä olevan kirjan
 * aakkosjärjestyksessä. quit - joka lopettaa ohjelman toiminnan.
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <bits/stdc++.h>
#include <sstream>

const unsigned int EMPTY = 0;
const int INPUTFILESROWAMOUNT = 4;
const unsigned int ONECOMMAND = 1;
const unsigned int TWOCOMMANDS = 2;
const unsigned int THREECOMMANDS = 3;
const std::string PARAMETERERROR = "Error: wrong number of parameters";

struct Book
{
    std::string author;
    std::string title;
    int reservations;
};

//Saa parametrinaan merkkijonon ja välimerkin. Palauttaa merkkijonosta
//paloitellun vektorin.
std::vector<std::string> split(std::string line, char separator = ';'){
    std::vector<std::string> splitted;
    std::string numbers;
    for(auto i : line){
        if(i != separator){
            numbers.push_back(i);
        }
        else{
            if(numbers.size() != EMPTY){
                splitted.push_back(numbers);
                numbers.clear();
            }
        }
    }
    splitted.push_back(numbers);
    return splitted;
}

//Saa parametrinaan viitteen mappiin johon tallennetaan kaikki tieto
//tiedostosta ja rivin joka sisältää yhden kirjan tiedot.
//Funktio tallentaa ja muokkaa syötettä map tietorakenteeseen sopivaan
//muotoon.
void SaveInContainer(std::map<std::string, std::vector<Book>>& library,
                     std::vector<std::string> oneRow){
    std::string libraryName = oneRow.at(0);
    std::string author = oneRow.at(1);
    std::string title = oneRow.at(2);
    int reservation = 0;

    if(oneRow.at(3) != "on-the-shelf" &&oneRow.at(3) != ""){
        reservation = std::stoi(oneRow.at(3));
    }

    if(library.find(libraryName) == library.end()){
        std::vector<Book> bookInfo;
        struct Book tmp = {author, title, reservation};
        bookInfo.push_back(tmp);
        library[libraryName] = bookInfo;
    }

    else{
        std::vector<Book> booksAtLib = library.at(libraryName);
        int i = 0;
        int mark = 0;
        for(auto& books : booksAtLib){
            if(books.author == author ){
                if( books.title == title){
                    //Tässä päivitetään tietorakenteen sisälle structiin
                    //oikeaan kohtaan, josta pidetään kirjaa i:llä, oikea
                    //varaustilanne, jos kirja löytyy jo kirjastosta.
                    library.at(libraryName).at(i).reservations = reservation;
                    ++mark;
                    break;
                }
            }
            ++i;
        }
        //jos kirja ei ole vielä kirjastossa merkki "mark" on silloin 0
        //ja luodaan kokonaan uusi kirja kirjastoon.
        if (mark == 0){
            struct Book newBook = {author, title, reservation};
            library.at(libraryName).push_back(newBook);
        }
    }
}

//Lukee tiedoston. Saa parametrinaan viitteen pää tietorakenteeseen.
//Palauttaa true jos tiedoston luku on onnistunut. Palauttaa false muuten.
bool ReadInputFile(std::map<std::string, std::vector<Book>>& library){
    std::string inputFile;

    std::cout << "Input file: ";
    getline(std::cin, inputFile);
    std::ifstream inputTxt(inputFile);
    if(not inputTxt){
        std::cout << "Error: input file cannot be opened" << std::endl;
        return false;
    }

    std::string row;
    while(getline(inputTxt, row)){
          std::vector<std::string> splittedRow = split(row);
          if(splittedRow.back() == ""){
              splittedRow.pop_back();
          }
          if(splittedRow.size() != 4){
              std::cout << "Error: empty field" << std::endl;
              inputTxt.close();
              return false;
          }
          SaveInContainer(library, splittedRow);
    }
    inputTxt.close();
    return true;
}

//Tulostaa tietyn kirjaston kaikki kirjat aakkosjärjestyksessä
//saa parametrinaan vektorin, jossa struct rakenteeseen tallennettu
//kirjaston kirjat.
void PrintAllBooks(std::vector<Book> libraryBooks){
    std::vector<std::pair<std::string, std::string>> sortedOrder;
    for(auto& book : libraryBooks){
        sortedOrder.push_back(std::make_pair(book.author, book.title));
    }
    std::sort(sortedOrder.begin(), sortedOrder.end());
    for(auto& x : sortedOrder){
        std::cout << x.first << ": " << x.second << std::endl;
    }
}

//Tulostaa tietyn kirjailijan tietyssä kirjastossa olevat kirjat.
//saa parametrinaan vectorin kirjaston kirjoista ja kirjailijan nimen.
void PrintBooksByAuthor(std::vector<Book> books, std::string authorName){
    std::vector<std::pair<std::string, int>> booksByAuthor;
    for(auto& books : books){
        if(books.author == authorName){
            booksByAuthor.push_back(std::make_pair(books.title,
                                                   books.reservations));
        }
    }

    std::sort(booksByAuthor.begin(), booksByAuthor.end());
    //x on pari joka pitää sisällään book.title ja book.reservations.
    //x.first = title. x.second = reservations.
    for(auto& x: booksByAuthor){
        std::cout << x.first << " --- ";
        if(x.second == 0){
            std::cout << "on the shelf" << std::endl;
        }
        else{
            std::cout << x.second << " reservations" << std::endl;
        }
    }
}

//Tulostaa lyhimmän varausjonon tietylle kirjalle ja missä kirjastoissa on
//tämä lyhin varausjono. saa parametrinaan päätietorakenteen library
//sekä vectorin joka sisältää käyttäjän syöttämät tiedot tekijästä
//ja kirjan nimestä.
void PrintReserveQue(std::map<std::string, std::vector<Book>>& library,
                     std::vector<std::string> inputVec){

    std::vector<std::pair<int, std::string>> listOfReserv;
    std::string author = inputVec.at(1);

    //Muutetaan kirjan nimi yhdeksi merkkijonoksi jos se on vektorissa ollut
    //monessa osassa
    std::stringstream ss;
    for(unsigned int i = 2; i < inputVec.size(); ++i){
        if(i != 2){
            ss<<" ";
        }
        ss<<inputVec[i];
    }
    std::string bookName = ss.str();
    bookName.erase(remove(bookName.begin(), bookName.end(), '"'),
                   bookName.end());

    //lisätään haluttu kirja eri kirjastoista väliaikaiseen vektoriin
    //vertailua varten
    for(auto& pair : library){
        for(auto& books: library.at(pair.first)){
            if(books.author == author && books.title == bookName){
                listOfReserv.push_back(std::make_pair(books.reservations,
                                                      pair.first));
            }
        }
    }
    if(listOfReserv.size() == EMPTY){
        std::cout << "Book is not a library book" << std::endl;
        return;
    }

    //Verrataan kaikkia varausjonoja tietystä kirjasta ja otetaan talteen vain
    //lyhimmän varausjonon omaavia kirjoja
    std::sort(listOfReserv.begin(),listOfReserv.end());
    std::vector<std::pair<int, std::string>> tmpListOfSmallest;
    tmpListOfSmallest.push_back(listOfReserv.at(0));
    for(unsigned int i=0; i<listOfReserv.size()-1; ++i){
        if(tmpListOfSmallest.at(0).first > listOfReserv.at(i+1).first){
            tmpListOfSmallest.clear();
            tmpListOfSmallest.push_back(listOfReserv.at(i+1));
        }
        else if(tmpListOfSmallest.at(0).first == listOfReserv.at(i+1).first){
            tmpListOfSmallest.push_back(listOfReserv.at(i+1));
        }
    }
    if(tmpListOfSmallest.at(0).first == 0){
        std::cout << "on the shelf" << std::endl;
    }
    else if(tmpListOfSmallest.at(0).first >= 100){
        std::cout << "Book is not reservable from any library" << std::endl;
        return;
    }
    else{
        std::cout << tmpListOfSmallest.at(0).first <<
                     " reservations" <<std::endl;
    }

    for (auto& x : tmpListOfSmallest){
        std::cout << "--- " << x.second << std::endl;
    }
}

//tulostaa kaikki kirjat mitkä ovat heti lainattavissa mistään kirjastosta
//saa parametrinaan viitteen päätietorakenteeseen.
void PrintLoanableBooks(std::map<std::string, std::vector<Book>>& library){
    std::vector<std::pair<std::string, std::string>> loanableBooks;
    for(auto& lib : library){
        for(auto& vec : lib.second){
            if(vec.reservations == 0){
                loanableBooks.push_back(std::make_pair(vec.author, vec.title));
            }
        }
    }
    std::sort(loanableBooks.begin(),loanableBooks.end());
    for(auto& x : loanableBooks){
        std::cout << x.first << ": " << x.second << std::endl;
    }
}

//tarkistaa että käyttäjän antaman kirjaston nimi on tiedossa oleva kirjasto
//saa parametrinaan käyttäjän antaman kirjaston nimen sekä viitteen
//tietorakenteeseen jossa on kaikki tieto kirjastoista. palauttaa true jos
//kirjasto on tiedossa, false jos kirjstoa ei löydy tietorakenteesta
bool IsLibraryValid(std::string libraryName, std::map<std::string,
                    std::vector<Book>>& library){
    if(library.find(libraryName) != library.end()){
        return true;
    }
    else{
        std::cout << "Error: unknown library" << std::endl;
        return false;
    }
}
//tarkistaa onko käyttäjän syöttämä kirjailijan nimi tiedossa
//saa parametrinaan käyttäjän syöttämän kirjailijan nimen
//ja viitteen päätietorakenteeseen missä on kaikkien tiedossa olevien
//kirjailijoiden nimet. palauttaa true jos kirjailija on tiedossa,
//false ja tulostus jos kirjailijaa ei tunneta,
bool IsAuthorValid(std::string authorName, std::map<std::string,
                   std::vector<Book>>& library){
    for(auto& pair : library){
        for(auto& book : pair.second){
            if(book.author == authorName){
                return true;
            }

        }
    }
    std::cout << "Error: unknown author" << std::endl;
    return false;
}

//lukee käyttäjän antaman syötteen ja tulkitsee mitä käyttäjä haluaa tehdä.
//Ottaa parametrinaan viitteen päätietorakenteeseen
void ReadUserInput(std::map<std::string, std::vector<Book>>& library){
    std::string userInput = "";
    char separator = ' ';
    while(userInput != "quit"){
        std::cout << "lib> ";
        std::getline(std::cin, userInput);
        std::vector<std::string> splittedUserInput = split(userInput,
                                                           separator);
        std::string command = splittedUserInput.at(0);
        if(command == "libraries" &&
                splittedUserInput.size() == ONECOMMAND){
            for(auto const & pair : library){
                    std::cout << pair.first << std::endl;
            }
        }

        else if(command == "loanable" &&
                splittedUserInput.size() == ONECOMMAND){
            PrintLoanableBooks(library);
        }

        else if(command == "material" &&
                splittedUserInput.size() == TWOCOMMANDS){
            std::string libName = splittedUserInput.at(1);
            if(IsLibraryValid(libName, library)){
                std::vector<Book> oneLibBooks = library.at(libName);
                PrintAllBooks(oneLibBooks);
            }
        }
        else if(command == "material" &&
                splittedUserInput.size() != TWOCOMMANDS){
            std::cout << PARAMETERERROR << std::endl;
        }

        else if(command == "books" &&
                splittedUserInput.size() == THREECOMMANDS){
            std::string libName = splittedUserInput.at(1);
            std::string authorName = splittedUserInput.at(2);

            if(IsLibraryValid(libName, library) &&
                    IsAuthorValid(authorName, library)){
                std::vector<Book> oneLibBooks = library.at(libName);
                PrintBooksByAuthor(oneLibBooks, authorName);
            }
        }
        else if(command == "books" &&
                splittedUserInput.size() != THREECOMMANDS){
            std::cout << PARAMETERERROR << std::endl;
        }

        else if(command == "reservable" &&
                splittedUserInput.size() >= THREECOMMANDS){
            PrintReserveQue(library, splittedUserInput);
        }
        else if(command == "reservable" &&
                splittedUserInput.size() < THREECOMMANDS){
            std::cout << PARAMETERERROR << std::endl;
        }

        else if(command == "quit" && splittedUserInput.size() == ONECOMMAND){
            continue;
        }
        else{
            std::cout << "Error: unknown command" << std::endl;
        }
    }
}

//lyhyt ja ytimekäs main
int main()
{
    std::map<std::string, std::vector<Book>> library = {};

    if(!ReadInputFile(library)){
        return EXIT_FAILURE;
    }
    ReadUserInput(library);
    return EXIT_SUCCESS;
}

