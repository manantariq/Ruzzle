//============================================================================
// Name        : Ruzzle5.cp// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>    //Librerie standard c++
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct word_list {               //Struct contenente la parola trovata
	string P;                   //il punteggio e le coordinate.La struct
	int points;            //verrà usata per creare una list contenete tutte le
	vector<int> coordinate;   //parole trovate
	word_list* nextptr;
};
struct coordinate {   //Struct contenente le coordinate della singola lettera
	char car;   //facente parte della parola ce si sta trovando
	int x;
	int y;
	coordinate* next;
};

void Dizionario(char **mat, word_list* &head, int dim, ifstream &f);
void cerca(string word, char **m, int riga, int col, unsigned int indice,
		bool ESISTE, int &trovato, word_list* &head, coordinate* coord_list,
		int dim);   //Dichiarazione funzioni utilizzate
int punteggio_parola(string parola);
void create_list(word_list* &head, int punti, coordinate* coord_list);
void ordinamento_stampa(word_list* &head, char**mat, int dim,
		ofstream &risultato, int max);

int main(int argc, const char * argv[]) {
	//Verifica che l'utente passi la stringa per la matrice
	if (argv[1] == NULL) { //nel caso in cui non è presente manda in uscita un errore e termina
		cout << "ERROR:STRING FOR RUZZLE MATRIX NOT FOUND" << endl; //il programma
		return 0;
	}

	const char* input_string; //Verifica che l'utente fornisca il numero massimo di parole
	if (argv[4] == NULL) { //da stampare nel file esterno, in caso il numero non sia presente
		cout << "ERROR:NO DECLARATION FOR THE MAX NUMBER OF WORDS" << endl; //manda in uscita
		return 0;   //un errore e termina il programma
	}
	int max = atoi(argv[4]); //trasformazione di const char* in int tramite atoi

	input_string = argv[1];

	unsigned int dim;
	dim = sqrt(strlen(input_string)); //Calcolo la dimensione facendo la radice quadrata della
//lunghezza della stringa passata dall'utente
	int dime = dim;
	if (dim < 4) {
		cout << " ERROR:THE SIZE OF MATRIX IS < 4" << endl; //Se la dimensione è minore di 4
		return 0; //segnala errore.

	}

	if ((strlen(input_string) != dim * dim)) { //Verifica che la stringa inserita sia sufficente
		cout << "ERROR:INSUFICCENT OR PLUS CHARACTER TO CREATE A MATRIX " << dim //per creare la
				<< " * " << dim << endl; //matrice, in caso contrario manda in uscita un errore
		return 0;   // e termina il prgramma
	}

	ifstream f;
	if (argv[2] == NULL) {                       //Si effettua il controllo che
		cout << "ERROR: DICTIONARY NOT FOUND" << endl; //alla funzione viene passato il
		return 0;                           //dizionario altrimenti si manda in
	}                                //uscita un errore e si chiude il programma
	f.open(argv[2]); //Viene aperto il file che nel nostro caso si chiama "list" contenente il risultato

	ofstream risultato;
	if (argv[3] == NULL) { //Si effettua il controllo che venga passato il file esterno su cui
		cout << "ERROR:NO DECLARATION FOR OUTPUT FILE" << endl; //salvare i risultati altrimenti
		return 0;     //si manda in uscita l'errore e si interrompe il programma
	}
	risultato.open(argv[3]);

	char **mat;     //Dichiarazione matrice dinamica

	mat = new char*[dime];     //Viene creata la matrice dinamica
	for (int l = 0; l < dime; l++) {
		mat[l] = new char[l];

	}
	int k = 0;
	for (int i = 0; i < dime; i++) { //attraverso il ciclo for creo la matrice a partire
//dalla stringa inserita dall'utente
		for (int j = 0; j < dime; j++) {
			mat[i][j] = input_string[k];
			k++;
		}
	}

	word_list* head = NULL;

	Dizionario(mat, head, dim, f); //Chiamata della funzione dizionario
	ordinamento_stampa(head, mat, dim, risultato, max); //Chiamata della funzione
	//ordinamento stampa

	cout << endl;
	return 0;
}
//......................................................................................................................
//......................................................................................................................
//......................................................................................................................

void Dizionario(char **mat, word_list* &head, int lung,	//Funzione Dizionario
		ifstream &f) {
	string word;//stringa in cui vengono salvate le singole parole del dizionario
	int i = 0;
	int j = 0;
	unsigned int indice = 0;
	bool ESISTE = false;
	int trovato = 0;
	coordinate* coord_list = NULL;

	if (f) {

		while (getline(f, word)) {
			for (int i = 0; i < lung; i++) {//Ciclo for che rende tutte le lettere della
				for (int j = 0; j < lung; j++) {//matrice in lettere minuscole
					mat[i][j] = tolower(mat[i][j]);
				}
			}
			cerca(word, mat, i, j, indice, ESISTE, trovato, head,//invocazione della funzione
					coord_list, lung);	//cerca

			word.clear();//Pulisco la stringa in cui inserire una nuova parola del dizionario
		}
	} else
		//Vengono controllati eventuali problemi nell'apertura del file "list.txt"
		cout << "File non aperto correttamente!" << endl;

}
//......................................................................................................................
//......................................................................................................................
//......................................................................................................................

void cerca(string parola, char **m, int riga, int col, unsigned indice,
		bool esiste, int &trovato, word_list* &head, coordinate* coord_list,
		int dim) {
	int finito = 0;
	char temp;

	if (esiste == false) {
		for (int k = 0; k < dim; k++) {	//Ciclo per la ricerca dell'esistenza della parola
			for (int n = 0; n < dim; n++) {	//passta dal dizionario nella matrice
				if (parola[0] == m[k][n]) {
					esiste = true;
					indice++;
					temp = m[k][n];	//Salvo la casella della matrice testata per poi poterla ripristinare.
					m[k][n] = ' ';//Assegno alla casella della matrice controllata un valore vuoto
//in modo che la lettera non venga contata al ciclo successivo

					coordinate* newnode = new coordinate;//creazione di un nuovo nodo alla lista
					newnode->x = k;	//dove salvo le coordinate della lettera trovata, la lettera
					newnode->y = n;	//contenuta nella cella della matrice per il ripristino
					newnode->car = temp;
					newnode->next = NULL;
					coord_list = newnode;

					cerca(parola, m, k, n, indice, esiste, trovato, head,
							coord_list, dim);
					m[k][n] = temp;	//Ripristino della matrice originale.
				}
				indice = 0;
				esiste = false;
				coord_list = NULL;
			}
		}
		finito = 1;
	}
	if ((indice < parola.size()) && (finito != 1)) {
		coordinate* currentnode = coord_list;
		while (currentnode->next != NULL) {	//Ciclo per posizionare il puntatore currentnode sullo
			currentnode = currentnode->next;	//ultimo nodo della lista
		}
		//controllo orizzontale sinistra a destra
		if (col < dim - 1) {//Si effettua controllo per non sforare dalla matrice
			if (parola[indice] == m[riga][col + 1]) {//Condizione in cui si verifica che sia presente
				temp = m[riga][col + 1];//la lettera che compone la mia parola
				m[riga][col + 1] = ' ';	//Se la lettera corrisponde a quella cercata nella matrice
				indice++;//viene sostituita con uno spazio vuoto in modo che la lettera non sia riutilizzabile
				col++;	//per la parola che si sta cercando.

				coordinate* newnode = new coordinate;
				newnode->car = temp;//Se la lettera corrisponde si crea un nuovo nodo alla lista
				newnode->x = riga;//coordinate in cui vengono salvate le coordinate e il carattere trovato
				newnode->y = col;
				newnode->next = NULL;
				currentnode->next = newnode;

				cerca(parola, m, riga, col, indice, true, trovato, head,
						coord_list, dim);
				indice--;
				col--;
				m[riga][col + 1] = temp;
			}
		}
		//Il metodo di ricerca visto sopra viene poi applicato per tutte le otto direzioni possibili all'interno
		//della matrice
		//controllo verticale dall'alto al basso
		if (riga < dim - 1) {
			if (parola[indice] == m[riga + 1][col]) {
				temp = m[riga + 1][col];
				m[riga + 1][col] = ' ';
				indice++;
				riga++;

				coordinate* newnode = new coordinate;
				newnode->car = temp;
				newnode->x = riga;
				newnode->y = col;
				newnode->next = NULL;
				currentnode->next = newnode;

				cerca(parola, m, riga, col, indice, true, trovato, head,
						coord_list, dim);
				indice--;
				riga--;
				m[riga + 1][col] = temp;
			}
		}
		//controllo orizzontale da destra a sinistra
		if (col - 1 >= 0) {
			if (parola[indice] == m[riga][col - 1]) {
				temp = m[riga][col - 1];
				m[riga][col - 1] = ' ';
				indice++;
				col--;

				coordinate* newnode = new coordinate;
				newnode->car = temp;
				newnode->x = riga;
				newnode->y = col;
				newnode->next = NULL;
				currentnode->next = newnode;

				cerca(parola, m, riga, col, indice, true, trovato, head,
						coord_list, dim);
				indice--;
				col++;
				m[riga][col - 1] = temp;
			}
		}
		//controllo verticale dal basso all'alto
		if (riga - 1 >= 0) {
			if (parola[indice] == m[riga - 1][col]) {
				temp = m[riga - 1][col];
				m[riga - 1][col] = ' ';
				indice++;
				riga--;

				coordinate* newnode = new coordinate;
				newnode->car = temp;
				newnode->x = riga;
				newnode->y = col;
				newnode->next = NULL;
				currentnode->next = newnode;

				cerca(parola, m, riga, col, indice, true, trovato, head,
						coord_list, dim);
				indice--;
				riga++;
				m[riga - 1][col] = temp;

			}
		}
		//Diagonalmente dallâ€™alto in basso da sinistra a destra
		if ((riga < dim - 1) && (col < dim - 1)) {
			if (parola[indice] == m[riga + 1][col + 1]) {
				temp = m[riga + 1][col + 1];
				m[riga + 1][col + 1] = ' ';
				indice++;
				riga++;
				col++;

				coordinate* newnode = new coordinate;
				newnode->car = temp;
				newnode->x = riga;
				newnode->y = col;
				newnode->next = NULL;
				currentnode->next = newnode;

				cerca(parola, m, riga, col, indice, true, trovato, head,
						coord_list, dim);
				indice--;
				riga--;
				col--;
				m[riga + 1][col + 1] = temp;
			}
		}
		//Diagonalmente dallâ€™alto in basso da destra a sinistra
		if ((riga < dim - 1) && (col - 1 > 0)) {
			if (parola[indice] == m[riga + 1][col - 1]) {
				temp = m[riga + 1][col - 1];
				m[riga + 1][col - 1] = ' ';
				indice++;
				riga++;
				col--;

				coordinate* newnode = new coordinate;
				newnode->car = temp;
				newnode->x = riga;
				newnode->y = col;
				newnode->next = NULL;
				currentnode->next = newnode;

				cerca(parola, m, riga, col, indice, true, trovato, head,
						coord_list, dim);
				indice--;
				riga--;
				col++;
				m[riga + 1][col - 1] = temp;
			}
		}
		//Diagonalmente dal basso in alto da sinistra a destra
		if ((riga - 1 >= 0) && (col < dim - 1)) {
			if (parola[indice] == m[riga - 1][col + 1]) {
				temp = m[riga - 1][col + 1];
				m[riga - 1][col + 1] = ' ';
				indice++;
				riga--;
				col++;

				coordinate* newnode = new coordinate;
				newnode->car = temp;
				newnode->x = riga;
				newnode->y = col;
				newnode->next = NULL;
				currentnode->next = newnode;

				cerca(parola, m, riga, col, indice, true, trovato, head,
						coord_list, dim);
				indice--;
				riga++;
				col--;
				m[riga - 1][col + 1] = temp;
			}
		}
		//Diagonalmente dal basso in alto da destra a sinistra
		if ((riga - 1 >= 0) && (col - 1 >= 0)) {
			if (parola[indice] == m[riga - 1][col - 1]) {
				temp = m[riga - 1][col - 1];
				m[riga - 1][col - 1] = ' ';
				indice++;
				riga--;
				col--;

				coordinate* newnode = new coordinate;
				newnode->car = temp;
				newnode->x = riga;
				newnode->y = col;
				newnode->next = NULL;
				currentnode->next = newnode;

				cerca(parola, m, riga, col, indice, true, trovato, head,
						coord_list, dim);
				indice--;
				riga++;
				col++;
				m[riga - 1][col - 1] = temp;
			}
		}
	}
	if (indice == parola.size()) {//Se l'indice corrisponde alla lunghezza della parola
		trovato++;	//allora la parola è contenuta nella matrice
		create_list(head, punteggio_parola(parola), coord_list);//invocazione della funzione
		esiste = false;	//creat_list.
		indice = 0;
	}
}
//......................................................................................................................
//......................................................................................................................
//......................................................................................................................

int punteggio_parola(string parola) {
	int punteggio = 0;
	unsigned i = 0;
	for (i = 0; i < parola.size(); i++) {//Ciclo per assegnare punteggio alla parola
		if (((parola[i] >= 'a') && (parola[i] <= 'g'))//se la parola non è composta da lettere come
				|| ((parola[i] >= 'i') && (parola[i] <= 'p'))//h,u,q,z,w,y,x ogni lettera vale un punto
				|| ((parola[i] >= 'r') && (parola[i] <= 't'))
				|| (parola[i] == 'v')) {
			punteggio += 1;
		} else if ((parola[i] == 'h') || (parola[i] == 'q')	//Se la parola è formata da lettere come h,u,q,z queste
				|| (parola[i] == 'u') || (parola[i] == 'z')) {//lettere varranno 2 punti ciascuna
			punteggio += 2;
		} else if ((parola[i] == 'w') || (parola[i] == 'x')	//Se la parola è formata da lettere
				|| (parola[i] == 'y')) {//come w,x,y queste lettere varranno tre punti ciascuna
			punteggio += 3;
		}
	}

	punteggio = punteggio * i;//il punteggio totale corrisponde al prodotto della lungezza della
	return punteggio;//intera parola con la somma dei punti delle singole lettere che la compongono
}	//la funzione ha come valore di ritorno il punteggio.
//......................................................................................................................
//......................................................................................................................
//......................................................................................................................

void create_list(word_list* &head, int punti, coordinate* coord_list) {
	coordinate* cur_node = coord_list;//puntatore che punta alla testa di coordinate
	string parola;	//stringa in cui viene salvata la parola trovata

	while (cur_node != NULL) {//ciclo che assegna a parola le lettere che la compongono
		parola += cur_node->car;	//contenute nella lista coordinate.
		cur_node = cur_node->next;
	}
	cur_node = coord_list;

	if (head == NULL) {	//Viene creata la lista delle parole trovate nella matrice
		word_list* newnode = new word_list;
		newnode->P = parola;//assegna a p la parola ricavata dalla lista coordinate.
		newnode->points = punti;	//assegna i punti della parola trovata.
		while (cur_node != NULL) {
			newnode->coordinate.push_back(cur_node->x);	//assegna a coordinate le coordinate
			newnode->coordinate.push_back(cur_node->y);	//della parola contenute nella lista coordinate
			cur_node = cur_node->next;
		}
		newnode->nextptr = NULL;//aggiornamento del puntatore al prossimo elemento
		head = newnode;	//aggiornamento del puntatore head
	} else {
		word_list* currentnode = head;//medesimo procediento di prima solo che adesso head non
		while (currentnode->nextptr != NULL) {	//è uguale a null.
			currentnode = currentnode->nextptr;
		}
		word_list* newnode = new word_list;
		newnode->P = parola;
		newnode->points = punti;
		while (cur_node != NULL) {
			newnode->coordinate.push_back(cur_node->x);
			newnode->coordinate.push_back(cur_node->y);
			cur_node = cur_node->next;
		}
		newnode->nextptr = NULL;
		currentnode->nextptr = newnode;
	}
}
//......................................................................................................................
//......................................................................................................................
//......................................................................................................................
void ordinamento_stampa(word_list* &head, char** mat, int dim,
		ofstream &risultato, int max) {
	word_list* currentnode = head;
	word_list* firstnode = head;
	word_list* maxnode = head;
	int m;

	risultato << "MATRICE" << endl;	//Viene stampata nel file di uscita la matrice inserita
//dall'utente.
	for (int i = 0; i < dim; i++) {
		risultato << endl;
		for (int j = 0; j < dim; j++) {
			risultato << setw(5) << mat[i][j];
		}
	}
	risultato << endl << endl;

	risultato << "N°" << "\t" << "Parola" << "\t" << "Punteggio" << "\t"
			<< "Coordinate" << endl << endl;

	int i = 1;
	int cont = 1;

	while ((m != max) && (currentnode != NULL)) {//viene stampata la lista contenente le parole
		firstnode = head;//trovate nella matrice in ordine crescente di punteggio.
		maxnode = head;
		while (firstnode != NULL) {
			if (firstnode->points > maxnode->points) {
				maxnode = firstnode;
			}
			firstnode = firstnode->nextptr;
		}
		if (maxnode->points != 0) {
			risultato << i++ << "\t" << maxnode->P << " - " << maxnode->points
					<< " - ";
			for (auto c : maxnode->coordinate) {
				if (cont == 2) {
					risultato << c << ");";
					cont = 0;
				} else {
					risultato << "(" << c << ",";
				}

				cont++;
			}
			cont = 1;
			risultato << endl;
		}
		m++;
		maxnode->points = 0;
		currentnode = currentnode->nextptr;
	}

	delete currentnode;  //Vengono cancellati tutti i puntatori
	delete firstnode;
	delete maxnode;
}
