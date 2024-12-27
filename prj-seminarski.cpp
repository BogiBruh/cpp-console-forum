#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class post {
public:
    int idPosta = 0, idRoditelja = 0;
    string naslov = "", tekstPosta;

    void nacrtajPost();
    void nacrtajReply();
    string napraviPost(int idPost, int idRoditelj);
    string napraviReply(int idPost, int idRoditelj);
};

void nadjiReplyove(post* listaPostova, int postBr, int brClanovaNiza, vector<int> &queueReplyova);
void izbrisiPost(post*& listaPostova, int idPost, int& brLinija, vector<int> queue);

int main()
{
    int izborPravljenjeFajla, izbor, izborBrisanje, indeksNiza = 0, pomocniUpis = 0, brLinija = 0, brLinijaNaPocetku;
    string nazivFajla = "posts.txt", tekstFajla, pomocniString;
    post* postArray = nullptr, * pomPostArray;
    vector<int> queueReplyova;

    cout << "Ostvarivanje konekcije sa fajlom..." << endl;
    fstream txtBaza(nazivFajla, ios::in | ios::out | ios::_Nocreate);

    if (!txtBaza.is_open()) {
        cout << "Nema baze, da li zelite da je napravite? 1. Da, 2. Ne ";
        cin >> izborPravljenjeFajla;
        if (izborPravljenjeFajla == 1) {
            ofstream txtBaza(nazivFajla);
            goto imaFajla;
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
            cout << "3. izbrisite post" << endl;
            cout << "4. izadjite iz programa" << endl;

            cin >> izbor;
            if (cin.fail()) { // ako se unese karakter na primer program se zbuni i ispisuje konstantno sve iz while loopa, ovo je ciscenje unosa i vracanje nazad
                cin.clear();
                cin.ignore();
                goto glavniLoop;
            }

            system("cls");
        
            switch (izbor) {
            case 1:
                txtBaza.open(nazivFajla);
            prikazaniPostovi:
                system("cls");
                queueReplyova.clear();
                for (int i = brLinija - 1; i >= 0; i--) {
                    if (postArray[i].idRoditelja == 0) {
                        postArray[i].nacrtajPost();
                        queueReplyova.push_back(postArray[i].idPosta);
                        cout << endl;
                    }
                }

                otvoriPost:
                cout << endl <<  "Ako zelite da se vratite na main menu upisite 0. Ako zelite da vidite neki post upisite njegov broj: ";

                cin >> izbor;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore();
                    goto otvoriPost;
                }
                if (izbor == 0) goto glavniLoop;
                else if(izbor <= postArray[brLinija - 1].idPosta && count(queueReplyova.begin(), queueReplyova.end(), izbor)){
                    system("cls");
                    queueReplyova.clear();
                    for (int i = 0; i < brLinija; i++) {
                        if (izbor == postArray[i].idPosta) {
                            postArray[i].nacrtajPost();
                            queueReplyova.push_back(postArray[i].idPosta);
                        }
                    }

                    nadjiReplyove(postArray, izbor, brLinija, queueReplyova);
                    sort(queueReplyova.begin(), queueReplyova.end());

                    for (int x = 0; x < brLinija; x++) {
                        if (count(queueReplyova.begin() + 1, queueReplyova.end(), postArray[x].idPosta)) postArray[x].nacrtajReply();
                    }

                    cout << "Ako zelite da se vratite na proslu stranu upisite 0. Ako zelite da odgovorite na neki od ovih postova upisite njegov broj: ";
                replyuj:
                    cin >> izbor;

                    if (izbor == 0) {
                        goto prikazaniPostovi;
                    }
                    else if (izbor <= postArray[brLinija - 1].idPosta && count(queueReplyova.begin(), queueReplyova.end(), izbor)) {
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

                        pomocniString = postArray[brLinija - 1].napraviReply(postArray[brLinija - 2].idPosta + 1, izbor);

                        txtBaza.close();
                        fstream txtBaza(nazivFajla, ios::out | ios::app);

                        txtBaza << pomocniString;
                        txtBaza.close();
                        txtBaza.open(nazivFajla);
                        break;
                    }
                    else {
                        cout << "Taj post ne postoji ili nije ovde prikazan." << endl;
                        goto replyuj;
                    }
                }
                else {
                    cout << "Taj post ne postoji ili je reply." << endl;
                    goto otvoriPost;
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

                if (brLinija == 1) {
                    pomocniString = postArray[brLinija - 1].napraviPost(1, 0);
                }
                else pomocniString = postArray[brLinija - 1].napraviPost(postArray[brLinija - 2].idPosta + 1, 0);

                txtBaza.close();
                fstream txtBaza(nazivFajla, ios::out | ios::app);
                if (brLinija == 1) pomocniString.erase(0, 1); //brise prvi karakter jer je prvi karakter znak za novi red
                txtBaza << pomocniString;
                txtBaza.close();
                txtBaza.open(nazivFajla);
                break;
            }
            case 3:
                system("cls");
                queueReplyova.clear();
                for (int i = brLinija - 1; i >= 0; i--) {
                    if (postArray[i].idRoditelja == 0) {
                        postArray[i].nacrtajPost();
                        queueReplyova.push_back(postArray[i].idPosta);
                        cout << endl;
                    }
                }
                sort(queueReplyova.begin(), queueReplyova.end());
                cout << "Ako zelite da se vratite na proslu stranu unesite 0; Ako zelite da izbrisete post unesite njegov broj: ";
                
            brisanjePostova:
                cin.ignore();
                cin >> izbor;
                if (izbor == 0) {
                    goto glavniLoop;
                }
                else if (izbor <= postArray[brLinija - 1].idPosta && count(queueReplyova.begin(), queueReplyova.end(), izbor)) {
                    system("cls");
                    for (int i = 0; i < brLinija; i++) {
                        if (postArray[i].idPosta == izbor) postArray[i].nacrtajPost();
                    }
                    cout << endl << "Da li zelite da izbrisete ovaj post? 1 - da, 0 - ne: ";
                    cin >> izborBrisanje;
                    if (izborBrisanje == 1) izbrisiPost(postArray, izbor, brLinija, queueReplyova);
                    else goto glavniLoop;
                }
                else {
                    cout << "Taj post ne postoji ili je reply." << endl;
                    goto brisanjePostova;
                }
                break;
            case 4:
                cout << "Brisanje djubreta iz memorije...";
                delete[] postArray;
                queueReplyova.clear();
                cout << endl << "Zatvaranje konekcije sa bazom fajlova...";
                txtBaza.close();
                cout << endl << "Dovidjenja!" << endl;

                exit(1);
                break;
            default:
                cout << "ti si glupani tupan" << endl;
                goto glavniLoop;
                break;
            }
        }
    }
   
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
                cout << "#";
            }
            else {
                if (j == 0 || j == y - 1) {
                    cout << "#";
                }
                else if (j == 1 || j == y - 2) {
                    cout << " ";
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
                                cout << tekstPosta[pom];
                                pom++;

                            }
                            else {
                                cout << " ";
                            }
                        }
                        else {
                            if (duzina != pom) {
                                cout << tekstPosta[pom];
                                pom++;
                            }
                            else {
                                cout << " ";
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
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
                cout << "#";
            }
            else {
                if (j == 0 || j == y - 1) {
                    cout << "#";
                }
                else if (j == 1 || j == y - 2) {
                    cout << " ";
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
                                cout << tekstPosta[pom];
                                pom++;

                            }
                            else {
                                cout << " ";
                            }
                        }
                        else {
                            if (duzina != pom) {
                                cout << tekstPosta[pom];
                                pom++;
                            }
                            else {
                                cout << " ";
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }
}

void nadjiReplyove(post* listaPostova, int postBr, int brClanovaNiza, vector<int> &queueReplyova) {
    int i;

    for (i = 0; i < brClanovaNiza; i++) {
        if (listaPostova[i].idRoditelja == postBr) {
            queueReplyova.push_back(listaPostova[i].idPosta);
            nadjiReplyove(listaPostova, listaPostova[i].idPosta, brClanovaNiza, queueReplyova);
        }
    }
}

string post::napraviPost(int idPost, int idRoditelj) {
    string generisaniString = "\n", pom = "";
    idPosta = idPost;
    idRoditelja = idRoditelj;
    generisaniString += to_string(idPost) + "|" + to_string(idRoditelj);

    cout << "Unesite naslov vaseg posta(do 76 karaktera):" << endl;
    cin.ignore();
    while (pom.length() == 0 || pom.length() > 76) {
        getline(cin, pom);
        if (pom.length() == 0) cout << "Unesite makar jedan karakter." << endl;
        if (pom.length() > 76) cout << "Preveliki naslov." << endl;
    }
    naslov = pom;
    generisaniString += "|" + pom;

    cout << endl << "Unesite tekst posta:" << endl;
    pom = "";
    while (pom.length() == 0) {
        getline(cin, pom);
        if (pom.length() == 0) cout << "Unesite makar jedan karakter." << endl;
    }
    tekstPosta = pom;
    generisaniString += "|" + pom;

    return generisaniString;
}

string post::napraviReply(int idPost, int idRoditelj) {
    string generisaniString = "\n", pom = "";
    idPosta = idPost;
    idRoditelja = idRoditelj;
    generisaniString += to_string(idPost) + "|" + to_string(idRoditelj);

    generisaniString += "|";

    cout << endl << "Unesite tekst posta:" << endl;
    cin.ignore();
    while (pom.length() == 0) {
        getline(cin, pom);
        if (pom.length() == 0) cout << "Unesite makar jedan karakter." << endl;
    }
    tekstPosta = pom;
    generisaniString += "|" + pom;

    return generisaniString;
}

void izbrisiPost(post *&listaPostova, int idPost, int &brLinija, vector<int> queue) {
    post *pomPostArray = new post[brLinija - 1];
    int brojac = 0;
    string linijaZaFajl = "";

    queue.clear();
    for (int i = 0; i < brLinija; i++) {
        if (listaPostova[i].idPosta == idPost) {
            queue.push_back(listaPostova[i].idPosta);
        }
    }
    nadjiReplyove(listaPostova, idPost, brLinija, queue);

    for (int i = 0; i < brLinija - 1; i++) {
        if (count(queue.begin(), queue.end(), listaPostova[i].idPosta) == 0) {
            pomPostArray[brojac] = listaPostova[i];
            brojac++;
        }
    }

    brLinija = brojac;
    delete[] listaPostova;
    listaPostova = new post[brLinija];

    fstream txtBaza1("posts.txt", ios::out);
    if (txtBaza1.fail()) {
        cout << "jajare na glavare" << endl;
        exit(1);
    }

    for (int i = 0; i < brLinija; i++) {
        listaPostova[i] = pomPostArray[i];
        linijaZaFajl = to_string(listaPostova[i].idPosta) + "|" + to_string(listaPostova[i].idRoditelja) + "|" + listaPostova[i].naslov + "|" + listaPostova[i].tekstPosta + "\n";
        txtBaza1 << linijaZaFajl;
    }

    txtBaza1.close();
}