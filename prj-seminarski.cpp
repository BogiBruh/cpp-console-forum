/* Aleksandar Bogunovic NRT-23/23 28.12.2024
 * FORUM u konzoli, principalno radi kako bih napravio forum u HTML-u i Javascriptu
 * Moguce je pisati, brisati i odgovarati(replyovati) na postove, koji se kontrolisu u fajlu posts.txt i u
 * dinamicki alociranom nizu klase postArray.
 */
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
//DEKLARACIJA KLASE
class post {
public:
    int idPosta = 0, idRoditelja = 0;
    string naslov = "", tekstPosta = "";

    void nacrtajPost();
    void nacrtajReply();
    string napraviPost(int idPost, int idRoditelj);
    string napraviReply(int idPost, int idRoditelj);
};
//DEKLARACIJA FUNKCIJA koje, iako manipulisu sa postovima, nisam znao da projektujem u samoj klasi
void nadjiReplyove(post* listaPostova, int postBr, int brClanovaNiza, vector<int> &queueReplyova);
void izbrisiPost(post*& listaPostova, int idPost, int& brLinija, vector<int>& queue);
int countSkraceno(int izbor, vector<int> queue);
int countSkracenoPlus1(int izbor, vector<int> queue);
void realociranjeNiza(int &brLinija, post* &postArray);

int main()
{
    //DEKLARACIJA PROMENLJIVIH, nazivao sam ih da budu sto jasnije za dalje pisanje koda
    int izborPravljenjeFajla, izbor, izborBrisanje, indeksNiza = 0, pomocniUpis = 0, brLinija = 0; 
    string nazivFajla = "posts.txt", tekstFajla, pomocniString;
    post* postArray = nullptr;
    vector<int> queueReplyova;

    cout << "Ostvarivanje konekcije sa fajlom..." << endl;
    fstream txtBaza(nazivFajla, ios::in | ios::out | ios::_Nocreate); //otvara fajl ako postoji, i za upis i za ispis

    if (!txtBaza.is_open()) { //provera da li je fajl dobro otvoren, ako nije otvara ga kao ofstream jer to kreira fajl
        cout << "Nema baze, da li zelite da je napravite? 1. Da, 2. Ne ";
        cin >> izborPravljenjeFajla;
        if (izborPravljenjeFajla == 1) {
            ofstream txtBaza(nazivFajla);
            goto imaFajla;
        }
        else {
            exit(1);
        }
    }
    else {
        imaFajla:
        while (getline(txtBaza, tekstFajla)) { //broji koliko linija ima u fajlu, da bi ispod dinamicki dodelio tacno onoliko mesta u nizu kolko
            brLinija++;                        //je potrebno
        }

        txtBaza.close();
        fstream txtBaza(nazivFajla);

        postArray = new post[brLinija];
        if (postArray == NULL) { //error handling ako je neuspesno alocirana memorija
            cout << "greska u dodeli memorije, pokusajte ponovo" << endl;
            exit(1);
        }


        while (getline(txtBaza, tekstFajla)) { // unos podataka u postArray
            if (tekstFajla == "\n") break; // ima negde neuhvaceni slucaj gde upise u fajl znak za novi red bespotrebno pa cisto da se ne izignorise da se ne napravi post br 0
            for (int i = 0; i < tekstFajla.length(); i++) {
                if (tekstFajla[i] != '|') { // for loop ide karakter po karakter do znaka | jer je format upisa u fajlu
                    switch (pomocniUpis) {  // idPosta|idRoditelja|naslov|tekstPosta
                    case 0:
                        pomocniString += tekstFajla[i];
                        postArray[indeksNiza].idPosta = stoi(pomocniString); // malo neefikasno jer ide karakter po karakter u pomocni string i
                        break; // pise-brise nakon castovanja u int u promenljivu klase taj string ali je to najlaksi nacin koji sam testirao
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
                        postArray[indeksNiza].tekstPosta += tekstFajla[i]; // cisto ako u tekstu ima | da ubaci ostatak teksta, iako to ne radi za naslov
                        break;
                    }
                }
                else {
                    pomocniString = ""; // ako je karakter | resetuje pomocniString
                    pomocniUpis++;     // i povecava brojac za switch case
                }
            }
            indeksNiza++;    // na kraju stringa u fajlu poveca brojac indeksNiza jer je to onda sledeci karakter
            pomocniUpis = 0; // resetuje na 0 za switch
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
            if (cin.fail()) { // ako se unese karakter na primer program se zbuni i ispisuje konstantno sve iz while loopa, ovo je ciscenje 
                cin.clear(); //  unosa i vracanje nazad
                cin.ignore();
                goto glavniLoop;
            }

            system("cls");
        
            switch (izbor) { // switch za izbor opcija sa main menua
            case 1: // 1. Citajte najnovije postove
            prikazaniPostovi:
                system("cls");
                queueReplyova.clear(); // obrise sve iz vektora da ubacuje sta treba
                for (int i = brLinija - 1; i >= 0; i--) {
                    if (postArray[i].idRoditelja == 0) { //ako je idRoditelja == 0 onda je to post a ne reply, pa ga crta i ubaci u vektor
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
                //        ako je izbor < zadnjeg idPosta                      ako je u vektoru
                else if(izbor <= postArray[brLinija - 1].idPosta && countSkraceno(izbor, queueReplyova)){
                    // funkcija za prikaz posta i replyova na sam post
                    system("cls");
                    queueReplyova.clear(); // clearuje ga da bi ubacio samo ono sto se prikaze na "novoj strani"
                    for (int i = 0; i < brLinija; i++) { // mora da se prodje kroz sve za slucaj da je obrisano proizvoljno postova iza njega
                        if (izbor == postArray[i].idPosta) {
                            postArray[i].nacrtajPost();
                            queueReplyova.push_back(postArray[i].idPosta); // nacrta samo taj post i doda ga u queue
                        }
                    }

                    nadjiReplyove(postArray, izbor, brLinija, queueReplyova); //nadje sve replyove za njegov id, koji je takodje izbor
                    sort(queueReplyova.begin(), queueReplyova.end()); //sortira u rastucem redosledu queue, jer ne pamtim vreme replyovanja

                    for (int x = 0; x < brLinija; x++) { // prolazi kroz sve i crta one koji su u queue-u ne racunajuci prvi post
                        if (countSkracenoPlus1(postArray[x].idPosta, queueReplyova)) postArray[x].nacrtajReply();
                    }

                    cout << "Ako zelite da se vratite na proslu stranu upisite 0, ako zelite da odgovorite na neki od ovih postova upisite njegov" << endl;
                    cout << "broj, a da izbrisete reply unesite negativan broj posta(npr. da izbrisete reply br.3 upisete -3): ";
                replyuj:
                    cin >> izbor;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        goto replyuj;
                    }

                    if (izbor == 0) {
                        goto prikazaniPostovi;
                    }
                        //      izbor < najveci idPost                izbor pozitivan     izbor u vektoru
                    else if (izbor <= postArray[brLinija - 1].idPosta && izbor > 0 && countSkraceno(izbor, queueReplyova)) {
                        // funkcija za replyovanje

                        realociranjeNiza(brLinija, postArray);

                        //pomocni string da se ubaci u fajl se kreira pored popunjavanja zadnjeg clana niza novim postom
                        pomocniString = postArray[brLinija - 1].napraviReply(postArray[brLinija - 2].idPosta + 1, izbor);
                        // ima idPosta za jedan veci od proslog i idRoditelja je izbor

                        // da se ne lazemo, mrzim ovaj deo koda. Nisam uspeo drugacije da uradim pa sam morao da 
                        txtBaza.close(); // zatvorim fajl
                        fstream txtBaza(nazivFajla, ios::out | ios::app); // otvorim da se appenduje
                        txtBaza << pomocniString; // ubacim taj generisani string dobrog formata
                        txtBaza.close(); // zatvorim fajl
                        txtBaza.open(nazivFajla); // i otvorim ga najnormalnije
                        //verovatno moze bolje ali bolji nacini koje sam smislio nisu radili pa sam morao da ostavim ovo
                        break;
                        // kraj funkcije za prikaz posta i replyova na sam post
                    }
                    else if (izbor <= postArray[brLinija - 1].idPosta && izbor < 0 && countSkraceno(abs(izbor), queueReplyova)) {
                        //funkcija za brisanje replya
                        system("cls");
                        for (int i = 0; i < brLinija; i++) {
                            if (postArray[i].idPosta == abs(izbor)) postArray[i].nacrtajReply(); // nacrta reply za brisanje
                        }
                        cout << endl << "Da li zelite da izbrisete ovaj reply? 1 - da, 0 - ne: ";
                        cin >> izborBrisanje;
                        if (izborBrisanje == 1) izbrisiPost(postArray, abs(izbor), brLinija, queueReplyova);
                        else goto glavniLoop;
                    }
                    else {
                        // pogresan unos za replyovanje/brisanje replya
                        cout << "Taj post ne postoji ili nije ovde prikazan." << endl;
                        goto replyuj;
                    }
                }
                else {
                    // pogresan unos za prikaz posta
                    cout << "Taj post ne postoji ili je reply." << endl;
                    goto otvoriPost;
                }

                break;
            case 2: { // 2. kreirajte novi post
                realociranjeNiza(brLinija, postArray);

                if (brLinija == 1) { // ako je brLinija == 1 to znaci da je samo jednom uradilo getline gore u inicijalizaciji, prvi idPosta je 1
                    pomocniString = postArray[brLinija - 1].napraviPost(1, 0);
                }
                //inace je idPosta najveci idPosta + 1
                else pomocniString = postArray[brLinija - 1].napraviPost(postArray[brLinija - 2].idPosta + 1, 0);

                txtBaza.close();
                fstream txtBaza(nazivFajla, ios::out | ios::app);
                if (brLinija == 1) pomocniString.erase(0, 1); //brise prvi karakter jer je prvi karakter znak za novi red
                txtBaza << pomocniString;
                txtBaza.close();
                txtBaza.open(nazivFajla);
                break;
            }
            case 3: // 3. izbrisite post
                system("cls");
                queueReplyova.clear();
                for (int i = brLinija - 1; i >= 0; i--) { // prolazi kroz sve postove i crta one kojima je idRoditelja == 0 da bi video korisnik
                    if (postArray[i].idRoditelja == 0) {  // koji post da obrise 
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
                else if (izbor <= postArray[brLinija - 1].idPosta && countSkraceno(izbor, queueReplyova)) {
                    system("cls");
                    for (int i = 0; i < brLinija; i++) { // crta post za dodatnu proveru dal je dobar izabran
                        if (postArray[i].idPosta == izbor) postArray[i].nacrtajPost();
                    }
                    cout << endl << "Da li zelite da izbrisete ovaj post? 1 - da, 0 - ne: ";
                    cin >> izborBrisanje;
                    if (izborBrisanje == 1) izbrisiPost(postArray, izbor, brLinija, queueReplyova);
                    else goto glavniLoop; // vraca na glavni meni
                }
                else { // los unos
                    cout << "Taj post ne postoji ili je reply." << endl;
                    goto brisanjePostova;
                }
                break;
            case 4: // 4. izadjite iz programa
                cout << "Brisanje djubreta iz memorije...";
                delete[] postArray;
                queueReplyova.clear();
                cout << endl << "Zatvaranje konekcije sa bazom fajlova...";
                txtBaza.close();
                cout << endl << "Dovidjenja!" << endl;

                exit(1);
                break;
            default:
                cout << "Pogresan unos. Unesite opciju koja je prikazana." << endl;
                // ova poruka se mozda nece videti, ali ne mogu da stavim label nize jer se nekad program vraca sa velikog ispisa pa mora cls
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

    // y - 4 zato sto se ispisuje # tekst # a ne #tekst#, x - 2 zbog gornjeg i donjeg zida
    if (ceil((double)duzina / (y - 4)) > x - 2) {
        if ((y - 4) * (x - 3) < duzina) {
            x = (ceil((double)duzina / (y - 4))) + 4; // dodaje 4 na rezultat deljenja da bi se takodje uracunao postIDstring i naslov
        }
        else {
            x = (ceil((double)duzina / (y - 4))) + 5; // dodaje 5 ako ima jos karaktera u tekstu posta koje ne popunjavaju ceo red
        }
    }

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (i == 0 || i == x - 1) { // gornji i donji zid da se nacrta samo granica
                cout << "#";
            }
            else {
                if (j == 0 || j == y - 1) { // levi i desni zid
                    cout << "#";
                }
                else if (j == 1 || j == y - 2) { // razmak od zida
                    cout << " ";
                }
                else {
                    if (i == 1) { // za ispis broja posta
                        if (idPostaFlag == 0) { // provera da li je vec ispisan postIDstring, ako nije pise ga ako jeste stavlja samo razmake
                            cout << postIDstring;
                            idPostaFlag = 1;
                        }
                        if (idPostaFlag == 1 && j < y - postIDstring.length() - 2) { // -2 za razmak i #
                            cout << " ";
                        }
                    }
                    else if (i == 2) {
                        if (naslovFlag == 0) { // ista logika kao if odma gore
                            cout << naslov;
                            naslovFlag = 1;
                        }
                        if (naslovFlag == 1 && j < y - naslov.length() - 2) {
                            cout << " ";
                        }
                    }
                    else if (i >= 3) { // ispis samog teksta posta, karakter po karakter
                        if (j == 2 && tekstPosta[pom] == ' ') { // ako je prvi karakter razmak preskace ga
                            if (duzina != pom) {
                                pom++;
                                cout << tekstPosta[pom];
                                pom++;

                            }
                            else { // ako nema vise karaktera ispisuje razmake do kraja
                                cout << " ";
                            }
                        }
                        else {
                            if (duzina != pom) { 
                                cout << tekstPosta[pom];
                                pom++;
                            }
                            else { // ako nema vise karaktera ispisuje razmake do kraja
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

    // y - 4 zato sto se ispisuje # tekst # a ne #tekst#, x - 2 zbog gornjeg i donjeg zida
    if (ceil((double)duzina / (y - 4)) > x - 2) {
        if ((y - 4) * x - 2 < duzina) {
            x = (ceil((double)duzina / (y - 4))) + 3; // dodaje 3 na rezultat deljenja da bi se takodje uracunao postIDstring
        }
        else {
            x = (ceil((double)duzina / (y - 4))) + 4; // dodaje 4 ako ima jos karaktera u tekstu posta koje ne popunjavaju ceo red
        }
    }

    for (int i = 0; i < x; i++) {
        cout << "\t\t"; // razmak od ivice prozora konzole, da se razazna sta je post a sta reply
        for (int j = 0; j < y; j++) {
            if (i == 0 || i == x - 1) { // gornji i donji zid da se nacrta samo granica
                cout << "#";
            }
            else {
                if (j == 0 || j == y - 1) { // levi i desni zid
                    cout << "#";
                }
                else if (j == 1 || j == y - 2) { // razmak od zida
                    cout << " ";
                }
                else {
                    if (i == 1) { // za ispis broja posta
                        if (idPostaFlag == 0) { // provera da li je vec ispisan postIDstring, ako nije pise ga ako jeste stavlja samo razmake
                            cout << postIDstring;
                            idPostaFlag = 1;
                        }
                        if (idPostaFlag == 1 && j < y - postIDstring.length() - 2) { // -2 za razmak i #
                            cout << " ";
                        }
                    }
                    else if (i >= 2) { // ispis samog teksta posta, karakter po karakter 
                        if (j == 2 && tekstPosta[pom] == ' ') { // ako je prvi karakter razmak preskace ga
                            if (duzina != pom) {
                                pom++;
                                cout << tekstPosta[pom];
                                pom++;

                            }
                            else { // ako nema vise karaktera ispisuje razmake do kraja
                                cout << " ";
                            }
                        }
                        else {
                            if (duzina != pom) {
                                cout << tekstPosta[pom];
                                pom++;
                            }
                            else { // ako nema vise karaktera ispisuje razmake do kraja
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

    for (i = 0; i < brClanovaNiza; i++) { // svaki clan listaPostova proverava da li je reply na postBr
        if (listaPostova[i].idRoditelja == postBr) {
            queueReplyova.push_back(listaPostova[i].idPosta);
            nadjiReplyove(listaPostova, listaPostova[i].idPosta, brClanovaNiza, queueReplyova); // rekurzivan poziv za svaki ubacen, da se nadju i za njega replyovi
        }                                                                                       // jer je sve to deo istog posta
    }
}

string post::napraviPost(int idPost, int idRoditelj) { // funkcija za ubacivanje i u postArray i u tekst fajl preko generisanog stringa
    string generisaniString = "\n", pom = ""; // generisani string krece sa \n jer kada se otvori fajl za unos on ubacuje na kraj teksta umesto u novi red
    idPosta = idPost;
    idRoditelja = idRoditelj;
    generisaniString += to_string(idPost) + "|" + to_string(idRoditelj); // odmah se dodaju idPost i idRoditelj jer su oni pozvani u funkciji

    cout << "Unesite naslov vaseg posta(do 76 karaktera):" << endl; // zato sto post ima y = 80, od cega oduzimamo 4 zbog "#  #"
    cin.ignore();
    while (pom.length() == 0 || pom.length() > 76) {
        getline(cin, pom);
        if (pom.length() == 0) cout << "Unesite makar jedan karakter." << endl;
        if (pom.length() > 76) cout << "Preveliki naslov." << endl;
    }
    naslov = pom;
    generisaniString += "|" + pom; // izmedju svakog podatka dodamo | zbog formata unosa u fajl(idPosta|idRoditelja|naslov|tekst)

    cout << endl << "Unesite tekst posta:" << endl;
    pom = "";
    while (pom.length() == 0) {
        getline(cin, pom);
        if (pom.length() == 0) cout << "Unesite makar jedan karakter." << endl; // po testiranju nacrtajPost i nacrtajReply se lome tek na 13000 karaktera, a sumnjam da ce se ijedan korisnik raspisati
    }
    tekstPosta = pom;
    generisaniString += "|" + pom;

    return generisaniString; // uneo je podatke u postArray i vraca string koji se u main funkciji stavlja u string koji ide u fajl
}

string post::napraviReply(int idPost, int idRoditelj) { // funkcija za ubacivanje i u postArray i u tekst fajl preko generisanog stringa
    string generisaniString = "\n", pom = ""; // generisani string krece sa \n jer kada se otvori fajl za unos on ubacuje na kraj teksta umesto u novi red
    idPosta = idPost;
    idRoditelja = idRoditelj;
    generisaniString += to_string(idPost) + "|" + to_string(idRoditelj); // odmah se dodaju idPost i idRoditelj jer su oni pozvani u funkciji

    generisaniString += "|"; // posto replyovi nemaju naslove samo odmah dodajem |

    cout << endl << "Unesite tekst posta:" << endl;
    cin.ignore();
    while (pom.length() == 0) {
        getline(cin, pom);
        if (pom.length() == 0) cout << "Unesite makar jedan karakter." << endl; // po testiranju nacrtajPost i nacrtajReply se lome tek na 13000 karaktera, a sumnjam da ce se ijedan korisnik raspisati
    }
    tekstPosta = pom;
    generisaniString += "|" + pom;

    return generisaniString; // uneo je podatke u postArray i vraca string koji se u main funkciji stavlja u string koji ide u fajl
}

void izbrisiPost(post *&listaPostova, int idPost, int &brLinija, vector<int>& queue) {
    post *pomPostArray = new post[brLinija - 1];
    int brojac = 0;
    string linijaZaFajl = "";

    queue.clear();
    for (int i = 0; i < brLinija; i++) {
        if (listaPostova[i].idPosta == idPost) {
            queue.push_back(listaPostova[i].idPosta); // u queue stavlja post i njegovi replyovi
        }
    }
    nadjiReplyove(listaPostova, idPost, brLinija, queue);
    for (int i = 0; i < brLinija; i++) {
        if (countSkraceno(listaPostova[i].idPosta, queue) == 0) { // prepisuje u pomPostArray sve postove koji nisu u queue-u, i pravi brojac koji ce biti novi brLinija
            pomPostArray[brojac] = listaPostova[i];
            brojac++;
        }
    }

    brLinija = brojac;
    delete[] listaPostova;
    listaPostova = new post[brLinija]; // realocira memoriju u listaPostova, iliti postArray

    fstream txtBaza1("posts.txt", ios::out);
    if (txtBaza1.fail()) {
        cout << "Fajl nije dobro otvoren." << endl;
        exit(1);
    }

    for (int i = 0; i < brLinija; i++) { // za svaki clan niza listaPostova generise string da ubaci u fajl, otvoren u ios::out modu da izbrise sve tako da od pocetka upisuje
        listaPostova[i] = pomPostArray[i];
        linijaZaFajl = to_string(listaPostova[i].idPosta) + "|" + to_string(listaPostova[i].idRoditelja) + "|" + listaPostova[i].naslov + "|" + listaPostova[i].tekstPosta + "\n";
        txtBaza1 << linijaZaFajl;
    }

    txtBaza1.close();
}

int countSkraceno(int izbor, vector<int> queue) {
    return count(queue.begin(), queue.end(), izbor);
}
//ove dve funkcije su cisto da skrate posao
int countSkracenoPlus1(int izbor, vector<int> queue) {
    return count(queue.begin() + 1, queue.end(), izbor);
}

void realociranjeNiza(int &brLinija, post *&postArray) {
    post* pomPostArray;
    
    brLinija++;
    pomPostArray = new post[brLinija]; // poveca brLinija, alocira pomocni niz

    for (int i = 0; i < (brLinija - 1); i++) {
        pomPostArray[i] = postArray[i]; // prekopira sve u pomocni
    }

    delete[] postArray;
    postArray = new post[brLinija]; // realocira postArray da bude jedan veci

    for (int i = 0; i < (brLinija - 1); i++) {
        postArray[i] = pomPostArray[i]; // vrati sve nazad i postArray ima jedno dodatno mesto
    }

    delete[] pomPostArray; // brise pomocni da oslobodi memoriju
}