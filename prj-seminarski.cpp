#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <fstream>
#include "prj-seminarski.h"

using namespace std;

class post {
public:
    int idPosta, idRoditelja = 0;
    string naslov = "", tekstPosta;

    void nacrtajPost();
    void nacrtajReply();
};

void prikaziPost(post* listaPostova, int postBr, int brClanovaNiza);

int main()
{
    int izbor, izbor2, indeksNiza = 0, pomocniUpis = 0, brLinija = 0;
    char izborUPostu;
    string nazivFajla = "input.txt", tekstFajla, pomocniString;
    post *seksibojs;

    fstream txtBaza(nazivFajla, ios::out | ios::_Nocreate);

    if (!txtBaza.is_open()) {
        cout << "Nema baze, da li zelite da je napravite? 1. Da, 2. Ne ";
        cin >> izbor;
        if (izbor == 1) {
            fstream txtBaza(nazivFajla);
        }
        else exit(1);
    }
    else {
        while (getline(txtBaza, tekstFajla)) {
            brLinija++;
        }
        txtBaza.close();
        fstream txtBaza(nazivFajla);

        seksibojs = new post[brLinija];
        if (seksibojs == NULL) {
            cout << "greska u dodeli memorije, pokusajte ponovo" << endl;
            exit(1);
        }

        
        while (getline(txtBaza, tekstFajla)) {

            for (int i = 0; i < tekstFajla.length(); i++) {
                if (tekstFajla[i] != '|') {
                    switch (pomocniUpis) {
                    case 0:
                        pomocniString += tekstFajla[i];
                        seksibojs[indeksNiza].idPosta = stoi(pomocniString);
                        break;
                    case 1:
                        pomocniString += tekstFajla[i];
                        seksibojs[indeksNiza].idRoditelja = stoi(pomocniString);
                        break;
                    case 2:
                        seksibojs[indeksNiza].naslov += tekstFajla[i];
                        break;
                    case 3:
                        seksibojs[indeksNiza].tekstPosta += tekstFajla[i];
                        break;
                    default:
                        break;
                    }
                }
                else {

                    pomocniString = "";
                    pomocniUpis++;
                }
            }
            indeksNiza++;
            pomocniUpis = 0;
        }
       
        glavniLoop:
        while (1) {
            system("cls");
            cout << "Dobrodosli na forum! Izaberite koju cete opciju: " << endl;
            cout << "1. Citajte najnovije postove" << endl;
            cout << "2. kreirajte novi post" << endl;
            cout << "3. izadjite iz programa" << endl;

            cin >> izbor;
            system("cls");
            prikazaniPostovi:
            switch (izbor) {
            case 1:
                system("cls");
                for (int i = brLinija - 1; i >= 0; i--) {
                    if (seksibojs[i].idRoditelja == 0) seksibojs[i].nacrtajPost();
                    cout << endl;
                }

                cout << endl << endl << "Ako zelite da se vratite na main menu upisite 0. Ako zelite da vidite neki post upisite njegov broj:";

                cin >> izbor2;
                if (izbor2 == 0) goto glavniLoop;
                else {
                    system("cls");
                    prikaziPost(seksibojs, izbor2, brLinija);
                }
                cout << "Da li biste zeleli da se vratite na proslu stranu? y - da, r - reply na post ";
                cin >> izborUPostu;

                switch (izborUPostu) {
                case 'y': case 'Y':
                    goto prikazaniPostovi;
                    break;
                case 'r': case 'R':
                    break;
                default:
                    break;
                }

                break;
            case 2:
                cout << "Lol zasto mislis da to postoji" << endl;
                break;
            case 3:
                exit(1);
                break;
            default:
                cout << "ti si glupani tupan" << endl;
                break;
            }
        }
    }
    

    txtBaza.close();
    return 0;
}

void post::nacrtajPost() {
    int x = 7, y = 80, pom = 0, idPostaFlag = 0, naslovFlag = 0;
    int duzina = tekstPosta.length();
    string postIDstring = "Post br." + to_string(idPosta);
    

    if (ceil((double)duzina / (y - 4)) > x - 2) {
        if ((y - 4) * (x - 3) < duzina) {
            x = (ceil((double)duzina / (y - 4))) + 4;
        }
        else {
            x = (ceil((double)duzina / (y - 4))) + 5;
        }
    }

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (i == 0 || i == x - 1) {
                std::cout << "#";
            }
            else {
                if (j == 0 || j == y - 1) {
                    std::cout << "#";
                }
                else if (j == 1 || j == y - 2) {
                    std::cout << " ";
                }
                else {
                    if (i == 1) {
                        if (idPostaFlag == 0) {
                            cout << postIDstring;
                            idPostaFlag = 1;
                        }
                        if (idPostaFlag == 1 && j < y - postIDstring.length() - 2) {
                            cout << " ";
                        }
                    }
                    else if (i == 2) {
                        if (naslovFlag == 0) {
                            cout << naslov;
                            naslovFlag = 1;
                        }
                        if (naslovFlag == 1 && j < y - naslov.length() - 2) {
                            cout << " ";
                        }
                    }
                    else if (i >= 3) {
                        if (j == 2 && tekstPosta[pom] == ' ') {
                            if (duzina != pom) {
                                pom++;
                                std::cout << tekstPosta[pom];
                                pom++;

                            }
                            else {
                                std::cout << " ";
                            }
                        }
                        else {
                            if (duzina != pom) {
                                std::cout << tekstPosta[pom];
                                pom++;
                            }
                            else {
                                std::cout << " ";
                            }
                        }
                    }
                }
            }
        }
        std::cout << endl;
    }
}

void post::nacrtajReply() {
    int x = 7, y = 64, pom = 0, idPostaFlag = 0;
    int duzina = tekstPosta.length();
    string postIDstring = "Post br." + to_string(idPosta) + ", Reply na " + to_string(idRoditelja);

    if (ceil((double)duzina / (y - 4)) > x - 2) {
        if ((y - 4) * x - 2 < duzina) {
            x = (ceil((double)duzina / (y - 4))) + 3;
        }
        else {
            x = (ceil((double)duzina / (y - 4))) + 4;
        }
    }

    for (int i = 0; i < x; i++) {
        cout << "\t\t";
        for (int j = 0; j < y; j++) {
            if (i == 0 || i == x - 1) {
                std::cout << "#";
            }
            else {
                if (j == 0 || j == y - 1) {
                    std::cout << "#";
                }
                else if (j == 1 || j == y - 2) {
                    std::cout << " ";
                }
                else {
                    if (i == 1) {
                        if (idPostaFlag == 0) {
                            cout << postIDstring;
                            idPostaFlag = 1;
                        }
                        if (idPostaFlag == 1 && j < y - postIDstring.length() - 2) {
                            cout << " ";
                        }
                    }
                    else if (i >= 2) {
                        if (j == 2 && tekstPosta[pom] == ' ') {
                            if (duzina != pom) {
                                pom++;
                                std::cout << tekstPosta[pom];
                                pom++;

                            }
                            else {
                                std::cout << " ";
                            }
                        }
                        else {
                            if (duzina != pom) {
                                std::cout << tekstPosta[pom];
                                pom++;
                            }
                            else {
                                std::cout << " ";
                            }
                        }
                    }
                }
            }
        }
        std::cout << endl;
    }
}

void prikaziPost(post* listaPostova, int postBr, int brClanovaNiza) {
    int i;
    listaPostova[postBr - 1].nacrtajPost();

    for(i = 0; i < brClanovaNiza; i++) {
        if (listaPostova[i].idRoditelja == postBr) listaPostova[i].nacrtajReply();
    }
}