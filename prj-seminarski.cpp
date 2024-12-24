#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <fstream>

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
    int izbor, izbor2, indeksNiza = 0, pomocniUpis = 0, brLinija = 0, brLinijaNaPocetku;
    char izborUPostu;
    string nazivFajla = "posts.txt", tekstFajla, pomocniString;
    post* postArray = nullptr, * pomPostArray;

    fstream txtBaza(nazivFajla, ios::in | ios::out | ios::_Nocreate);

    if (!txtBaza.is_open()) {
        cout << "Nema baze, da li zelite da je napravite? 1. Da, 2. Ne ";
        std::cin >> izbor;
        if (izbor == 1) {
            fstream txtBaza(nazivFajla, ios::out);
        }
        else {
            exit(1);
        }

        txtBaza.close();
        fstream txtBaza(nazivFajla, ios::in | ios::out);
    }
    else {
        imaFajla:
        while (getline(txtBaza, tekstFajla)) {
            brLinija++;
        }
        brLinijaNaPocetku = brLinija;
        txtBaza.close();
        fstream txtBaza(nazivFajla);

        postArray = new post[brLinija];
        if (postArray == NULL) {
            cout << "greska u dodeli memorije, pokusajte ponovo" << endl;
            exit(1);
        }


        while (getline(txtBaza, tekstFajla)) {

            for (int i = 0; i < tekstFajla.length(); i++) {
                if (tekstFajla[i] != '|') {
                    switch (pomocniUpis) {
                    case 0:
                        pomocniString += tekstFajla[i];
                        postArray[indeksNiza].idPosta = stoi(pomocniString);
                        break;
                    case 1:
                        pomocniString += tekstFajla[i];
                        postArray[indeksNiza].idRoditelja = stoi(pomocniString);
                        break;
                    case 2:
                        postArray[indeksNiza].naslov += tekstFajla[i];
                        break;
                    case 3:
                        postArray[indeksNiza].tekstPosta += tekstFajla[i];
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
                    if (postArray[i].idRoditelja == 0) {
                        postArray[i].nacrtajPost();
                        cout << endl;
                    }
                }

                cout << endl << endl << "Ako zelite da se vratite na main menu upisite 0. Ako zelite da vidite neki post upisite njegov broj:";

                cin >> izbor2;
                if (izbor2 == 0) goto glavniLoop;
                else {
                    system("cls");
                    postArray[izbor2 - 1].nacrtajPost();
                    prikaziPost(postArray, izbor2 - 1, brLinija);
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
            case 2: {
                brLinija++;
                pomPostArray = new post[brLinija];

                for (int i = 0; i < (brLinija - 1); i++) {
                    pomPostArray[i] = postArray[i];
                }

                delete[] postArray;
                postArray = new post[brLinija];

                for (int i = 0; i < (brLinija - 1); i++) {
                    postArray[i] = pomPostArray[i];
                }

                delete[] pomPostArray;

                cout << "Unesite naslov vaseg posta:" << endl;
                cin.ignore();
                getline(cin, postArray[brLinija - 1].naslov);

                cout << endl << "Unesite tekst posta:" << endl;
                getline(cin, postArray[brLinija - 1].tekstPosta);
                postArray[brLinija - 1].idPosta = brLinija;
                postArray[brLinija - 1].idRoditelja = 0;

                txtBaza.close();
                fstream txtBaza(nazivFajla, ios::out | ios::app);

                if (txtBaza.is_open()) {
                    pomocniString = "\n" + to_string(postArray[brLinija - 1].idPosta) + "|" + to_string(postArray[brLinija - 1].idRoditelja) + "|" + postArray[brLinija - 1].naslov + "|" + postArray[brLinija - 1].tekstPosta;
                }

                txtBaza << pomocniString;
                txtBaza.close();
                txtBaza.open(nazivFajla);

                break;
            }
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

    for (i = 0; i < brClanovaNiza; i++) {
        if (listaPostova[i].idRoditelja == postBr + 1) {
            listaPostova[i].nacrtajReply();
            prikaziPost(listaPostova, i, brClanovaNiza);
        }

    }
}