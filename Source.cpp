#include<iostream>
#include<random>
#include<vector>

using namespace std;

struct Lottokuponki {
	vector<vector<bool>> lottorivit;
};

int Kysy_luku(int, int, string);
int Arvo_satunnaisluku(int, int);
void Tulosta_paavalikko();
vector<bool> Arvo_lottorivi();
void Simuloi_lottokierros(Lottokuponki&, int&, int&, int, int);
void Syota_lottorivi_kasin(Lottokuponki&, int);
void Tulosta_lottorivi(vector<bool>);
void Odota();
void Tyhjenna_ruutu();
int Laske_voittosumma(int);
int Tarkasta_lottokuponki(int, vector<bool>, Lottokuponki&, int, int);
bool Paavalikko(int&, int&, int, int, Lottokuponki&);
void Nayta_ohje();
void Anna_koneen_arpoa_rivit(Lottokuponki&, int);
void Tulosta_kuponki(Lottokuponki&);
void Pelaa_lottoa(Lottokuponki&, int&, int&, int);
void Nollaa_voitot_ja_tappiot(int&, int&);
void Tyhjenna_kuponki(Lottokuponki&);




int main() {

	const int MAX_RIVIT = 100;
	const int MAX_KIERROKSET = 10000;

	int voitot = 0;
	int tappiot = 0;
	setlocale(LC_ALL, "finnish");
	Lottokuponki kuponki;

	while (Paavalikko(voitot, tappiot, MAX_RIVIT, MAX_KIERROKSET, kuponki)) {
		Tyhjenna_ruutu();
	}

}

int Kysy_luku(int min, int max, string prompt= "") {
	//Tulostaa näytölle viestin (prompt) ja kysyy käyttäjältä kokonaisluvun välillä [min, max]

	bool virhe = true;
	int luku;

	do
	{
		cout << prompt;
		cin >> luku;

		//Tarkistus onko annettu luku sallitulla välillä + tyypin tarkistus
		if (!cin.fail() && luku >= min && luku <= max)
			virhe = false;
		else cout << "Virheellinen arvo. Yritä uudelleen.\n";

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

	} while (virhe);
	return luku;
}

int Arvo_satunnaisluku(int min, int max) {
	//Palauttaa satunnaisen kokonaisluvun väliltä [min, max]

	random_device siemen;
	mt19937 generaattori(siemen());
	uniform_int_distribution<> jakauma(min, max);
	return jakauma(generaattori);
}

void Tulosta_paavalikko() {
	cout << "1. Peliohjeet ja -säännöt\n";
	cout << "2. Tee lottorivi käsin\n";
	cout << "3. Lisää koneen arpomia lottorivejä\n";
	cout << "4. Näytä kuponki\n";
	cout << "5. Pelaa lottoa\n";
	cout << "6. Nollaa voitot ja tappiot\n";
	cout << "7. Tyhjennä kuponki\n";
	cout << "8. Lopeta\n";
}

vector<bool> Arvo_lottorivi(){
	//Palauttaa 41 alkiota sisältävän bool-tyypin taulukon, josta 7 satunnaista (indeksiltään nollaa suurempaa)
	//alkiota on arvoltaan true ja loput false

	//Taulukon alustus
	vector<bool> lottorivi;
	for (int i = 0; i < 41; i++)
		lottorivi.push_back(false);

	//Lottonumeroiden arvonta
	for (int i = 0; i < 7; i++) {
		int uusi_numero = 0;

		do {
			uusi_numero = Arvo_satunnaisluku(1, 40);
		} while (lottorivi[uusi_numero]); //tarkistus, ettei arvota samaa numeroa kahteen kertaan
		lottorivi[uusi_numero] = true;
	}

	return lottorivi;
}

void Simuloi_lottokierros(Lottokuponki& kuponki, int& voitot, int& tappiot, int tulostuskynnys, int kierrosnumero){


	int kierroksen_hinta = kuponki.lottorivit.size();
	tappiot += kierroksen_hinta;

	int lisanumero = 0;
	vector<bool> voittonumerot = Arvo_lottorivi();

	do {
		lisanumero = Arvo_satunnaisluku(1, 40);
	} while (voittonumerot[lisanumero]);

	int kierroksen_voitot = Tarkasta_lottokuponki(lisanumero, voittonumerot, kuponki, kierrosnumero, tulostuskynnys);
	voitot += kierroksen_voitot;

	int viimeisin_voittokierros = 1;

	if (kierroksen_voitot > 0)
		viimeisin_voittokierros = kierrosnumero;
}

void Syota_lottorivi_kasin(Lottokuponki& kuponki, int vapaat_rivit) {
	// Kysyy käyttäjältä 7 lottonumeroa ja palauttaa bool-tyypin taulukon,
	// jossa valittujen numeroiden indeksiä vastaavat alkiot ovat arvoltaan true
	
	Tyhjenna_ruutu();

	if (vapaat_rivit <= 0) {
		cout << "Kupongille ei mahdu enempää rivejä.\n";
		return;
	}

	cout << "Syötä ensimmäinen numero: ";
	int uusi_numero;

	//Taulukon alustus
	vector<bool> lottorivi;
	for (int i = 0; i < 41; i++)
		lottorivi.push_back(false);
	

	for (int i = 0; i < 7; i++) {

		if (i > 0) {
			cout << "Tämänhetkinen rivi: ";
			Tulosta_lottorivi(lottorivi);
			cout << "Syötä seuraava numero: ";
			
		}

		do {
			uusi_numero = Kysy_luku(1, 40);
			if (lottorivi[uusi_numero])
				cout << "Antamasi numero on jo valittu. Anna toinen numero: ";
		} while (lottorivi[uusi_numero]);

		lottorivi[uusi_numero] = true;
		Tyhjenna_ruutu();
	}

	kuponki.lottorivit.push_back(lottorivi);
	cout << "Syöttämäsi rivi: ";
	Tulosta_lottorivi(lottorivi);
	Odota();
}

void Tulosta_lottorivi(vector<bool> lottorivi) {
	// Tulostaa bool-tyyppisenä taulukkona annetun lottorivin näytölle

	for (int i = 1; i < lottorivi.size(); i++)
		if (lottorivi[i])
			cout << i << " ";

	cout << endl;
}

void Odota() {
	//Jää odottamaan käyttäjältä syötettä ennen kuin antaa ohjelman jatkua
	cout << "Jatka painamalla ENTER";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return;
}

void Tyhjenna_ruutu() {
	//"Tyhjentää" ruudun tulostamalla sata tyhjää riviä
	for (int i = 0; i < 100; i++)
		cout << endl;
	return;
}

int Laske_voittosumma(int osumat) {
	// Parametri osumat kertoo montako numeroa rivillä oli oikein ja lisönumero, oliko lisänumero oikein.
	// Paluuarvo on tuloksella saatava voittosumma

	int voittosumma;
	enum Voittoluokat {KOLME_JA_LISA = 3, NELJA, VIISI, KUUSI, SEITSEMAN, KUUSI_JA_LISA};



	switch (osumat) {
	case KOLME_JA_LISA: voittosumma = 2;
		break;
	case NELJA: voittosumma = 10;
		break;
	case VIISI: voittosumma = 50;
		break;
	case KUUSI: voittosumma = 2000;
		break;
	case KUUSI_JA_LISA: voittosumma = 100000;
		break;
	case SEITSEMAN: voittosumma = 5000000;
		break;
	default: voittosumma = 0;
	}

	return voittosumma;
}

int Tarkasta_lottokuponki(int lisanumero, vector<bool> voittonumerot_bool, Lottokuponki& kuponki, 
	int kierrosnumero, int tulostuskynnys) {

	// Ottaa parametrinä kierroksen voittonumerot, lisänumeron ja lottokupongin.
	// Palauttaa summan, jonka kupongilla voittaa
	
	int voittosumma = 0;

	// Muunnetaan voittonumerot kokonaislukuvektorimuotoon
	vector<int> voittonumerot_int;

	for (int i = 1; i < 40; i++)
		if (voittonumerot_bool[i])
			voittonumerot_int.push_back(i);

	// Verrataan voittonumeroita kupongin jokaiseen riviin ja lisätään mahdollinen voitto
	// juoksevaan summaan

	for (vector<bool> rivi : kuponki.lottorivit) {
		int osumat = 0;
		bool lisanumero_oikein = rivi[lisanumero];



		for (int numero : voittonumerot_int)
			if (rivi[numero])
				osumat++;


		if (osumat == 3 && !lisanumero_oikein)
			osumat = 0;
		if (osumat == 6 && lisanumero_oikein)
			osumat = 8;

		int rivin_voitto = Laske_voittosumma(osumat);
		voittosumma += rivin_voitto;

		if (osumat >= tulostuskynnys && (tulostuskynnys < 7 || osumat == 7)) {
			cout << endl;

			cout << "Kierros " << kierrosnumero << endl;

			cout << "Voittonumerot: ";
			for (int numero : voittonumerot_int)
				cout << numero << " ";
			cout << "Lisänumero: " << lisanumero << endl;

			cout << "Kupongin rivi: ";
			Tulosta_lottorivi(rivi);
			cout << "Voitto: " << rivin_voitto << " euroa. \n";
		}

	}

	return voittosumma;
}

bool Paavalikko(int& voitot, int& tappiot, int MAX_RIVIT, int MAX_KIERROKSET, Lottokuponki& kuponki) {
	cout << "LOTTOSIMULAATTORI\n\n";

	int saldo = voitot - tappiot;
	int kupongin_rivit = kuponki.lottorivit.size();

	if (kupongin_rivit > 0)
		cout << "Kupongillasi on " << kupongin_rivit << " rivi(ä).\n";

	if (saldo < 0)
		cout << "Olet tappiolla " << saldo * -1 << " euroa.\n";

	if (saldo > 0)
		cout << "Olet voitolla " << saldo << " euroa.\n";

	cout << "\nValitse seuraavista toiminnoista:\n";
	Tulosta_paavalikko();
	int vapaat_rivit = MAX_RIVIT - kuponki.lottorivit.size();

	int valinta = Kysy_luku(1, 8, "Valinta: ");

	switch (valinta)
	{
	case 1: Nayta_ohje();
		break;
	case 2: Syota_lottorivi_kasin(kuponki, vapaat_rivit);
		break;
	case 3: Anna_koneen_arpoa_rivit(kuponki, vapaat_rivit);
		break;
	case 4: Tulosta_kuponki(kuponki);
		break;
	case 5: Pelaa_lottoa(kuponki, voitot, tappiot, MAX_KIERROKSET);
		break;
	case 6: Nollaa_voitot_ja_tappiot(voitot, tappiot);
		break;
	case 7: Tyhjenna_kuponki(kuponki);
		break;
	case 8: return false;
	}

	return true;

}



void Nayta_ohje() {
	Tyhjenna_ruutu();
	cout << "Lotossa yrität arvata oikein 7 satunnaisesti arvottua numeroa 40:stä. Joka kierroksella arvotaan myös lisänumero, ";
	cout << "jonka arvaaminen antaa suuremman voiton, mikäli arvaat 3 tai 6 voittonumeroista oikein. Lisää kupongillesi rivejä ";
	cout << "seuraamalla valikon ohjeita. Kupongilla voi olla max. 100 riviä. Jokainen rivi maksaa euron per kierros.\n\n";
	cout << "Voittotaulukko on seuraavanlainen:\n";
	cout << "3 oikein + lisänumero: " << Laske_voittosumma(3) << " euroa\n";

	for (int i = 4; i < 7; i++)
		cout << i << " oikein: " << Laske_voittosumma(i) << " euroa\n";

	cout << "6 oikein + lisänumero: " << Laske_voittosumma(8) << " euroa\n";
	cout << "Seitsemän oikein (päävoitto): 5 000 000 euroa\n";

	Odota();
}

void Anna_koneen_arpoa_rivit(Lottokuponki& kuponki, int max_rivit) {
	Tyhjenna_ruutu();
	cout << "Kupongille mahtuu vielä " << max_rivit << " riviä.\n";
	int rivien_maara = Kysy_luku(0, max_rivit, "Montako riviä haluat arpoa (0 = peruuta): ");
	for (;rivien_maara > 0; rivien_maara--)
		kuponki.lottorivit.push_back(Arvo_lottorivi());
}

void Tulosta_kuponki(Lottokuponki& kuponki) {
	Tyhjenna_ruutu();

	cout << "Lottokupongin rivit:\n";
	for (vector<bool> rivi : kuponki.lottorivit)
		Tulosta_lottorivi(rivi);
	Odota();
	Tyhjenna_ruutu();
}

void Pelaa_lottoa(Lottokuponki& kuponki, int& voitot, int& tappiot, int MAX_KIERROKSET) {
	Tyhjenna_ruutu();

	if (kuponki.lottorivit.empty()) {
		cout << "Kupongillasi ei ole yhtään riviä\n";
		Odota();
		return;
	}

	cout << "Montako kierrosta pelataan (max: " << MAX_KIERROKSET << ", 0 = peruuta): ";
	int kierroksien_maara = Kysy_luku(0, MAX_KIERROKSET);

	if (kierroksien_maara == 0)
		return;



	cout << "Valitse tulostusasetukset\n";
	cout << "1. Älä tulosta minkään yksittäisen rivin tuloksia.\n";

	cout << "2. Tulosta kaikki voittorivit";
	if (kuponki.lottorivit.size() * kierroksien_maara > 25000)
		cout << " (HUOM voi aiheuttaa epilepsiakohtauksen/koneen kyykkäämisen)";

	cout << "\n3. Tulosta vain rivit, joilla on 5 oikein tai enemmän.\n";
	cout << "4. Tulosta vain rivit, joilla on päävoitto.\n";

	int tulostuskynnys = Kysy_luku(1, 4);

	switch (tulostuskynnys) {
		case 1: tulostuskynnys = 99;
			break;
		case 2: tulostuskynnys = 3;
			break;
		case 3: tulostuskynnys = 5;
			break;
		case 4: tulostuskynnys = 7;
			break;
	}


	Tyhjenna_ruutu();

	cout << "Simuloidaan...\n";

	for (int kierros = 1; kierros < kierroksien_maara + 1; kierros++)
		Simuloi_lottokierros(kuponki, voitot, tappiot, tulostuskynnys, kierros);

	cout << "Simuloitu " << kierroksien_maara << " kierrosta.\n";

	Odota();
}

void Nollaa_voitot_ja_tappiot(int& voitot, int& tappiot){
	voitot = 0;
	tappiot = 0;
}

void Tyhjenna_kuponki(Lottokuponki& kuponki){
	kuponki.lottorivit.clear();
}
