#include <iostream>
#include <time.h>
#include <vector>

#include "piros_fekete_fa.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

Fa* Fa_generalasa(int n)
{
	Fa* fa = LetrehozFa();
	vector<bool>frek(11); //1->10
	for (int i = 0; i < 10; i++)
	{
		frek[i] = false;
	}

	bool ok = false;
	int csp;

	for (int i = 0; i < n; i++) //felepitjuk a grafot
	{
		ok = false;
		while (!ok)
		{
			//generalunk egy csomopontot
			csp = rand() % 10 + 1; //1->10

			if (frek[csp] == false) //ne ismetlodjon a csomopont
			{
				frek[csp] = true;
				ok = true;
			}
		}

		Beszuras(fa, csp);
		//Inorder(proba->gyoker);
		//Preorder(fa->gyoker);
		//cout << endl;
	}

	return fa;
}

void jatek2(Fa* A, int eletek_szama, int csomopontok_szama, int& pontszam)
{
	int tipp_csp, tipp_szin;
	int elozo_talalat = 0;
	pontszam = 0;

	while (eletek_szama != 0 && csomopontok_szama != 0)
	{
		cout << "Eletek szama: " << eletek_szama << endl;
		cout << "Pontok szama: " << pontszam << endl << endl;
		cout << "Adjon meg egy tippet!" << endl;
		cout << " - csomopont erteke: ";
		cin >> tipp_csp;
		cout << " - csomopont szine: "; //FEKETE - 0  es PIROS - 1
		cin >> tipp_szin;

		if (tipp_csp > 0 && tipp_csp <= 10 && (tipp_szin == 0 || tipp_szin == 1))
		{
			Elem* seged = Keres_Rek(A->gyoker, tipp_csp);

			if (seged != NULL) //van ilyen erteku csomopont
			{
				if (seged->szin == tipp_szin)
				{
					cout << "Talalat!" << endl;
					TorolElem(A, seged);
					elozo_talalat++;

					pontszam = pontszam + elozo_talalat * 100;

					if (elozo_talalat >= 3)
					{
						eletek_szama++;
					}

					csomopontok_szama--;
				}
				else
				{
					//cout << "Hibas szin!" << endl;
					cout << "Nincs talalat!" << endl;
					eletek_szama--;
					elozo_talalat = 0;
				}
			}
			else
			{
				cout << "Nincs talalat!" << endl;
				elozo_talalat = 0;
				eletek_szama--;
			}
			//cout << "Fa: " << endl;
			//Inorder(A->gyoker);
			//cout << endl;
		}
		else
		{
			cout << "Nem megfelelo tipp! A csomopontok 1 es 10 kozottiek lehetnek, a szin vagy 0(FEKETE) vagy 1(PIROS)!" << endl;
		}
		cout << "------------------------" << endl;
		//cout << endl << endl;
	}
}

void adott_kor(int szint, int& aktualis_pontszam)
{
	int eletek_szama = 0;
	int csomopontok_szama = 0;
	aktualis_pontszam = 0;

	if (szint == 0)
	{
		eletek_szama = 20;
		csomopontok_szama = rand() % 3 + 3; //3->6
	}

	if (szint == 1)
	{
		eletek_szama = 15;
		csomopontok_szama = rand() % 3 + 4; //4->7
	}

	if (szint == 2)
	{
		eletek_szama = 10;
		csomopontok_szama = rand() % 3 + 5; //5->8
	}
	vector<bool>frek(csomopontok_szama + 1);

	Fa* fa = Fa_generalasa(csomopontok_szama);

	cout << "FONTOS TUDNIVALOK: " << endl;
	cout << "A fa " << fa->csomopontok_szama << " csomopontot tartalmaz!" << endl;
	//cout << "Legkisebb elem: " << Minimum(fa) << endl;
	//cout << "Legnagyobb elem: " << Maximum(fa) << endl;
	cout << "A fa gyokere: " << fa->gyoker->ertek << endl;
	Minimum(fa);
	Maximum(fa);
	cout << endl;

	int tipp_csp, tipp_szin;
	int elozo_talalat = 0;

	jatek2(fa, eletek_szama, csomopontok_szama, aktualis_pontszam);

	//FelszabaditFa(fa); most mar megy
}

void jatek()
{
	cout << "Udvozlunk a jatekban!" << endl << endl;
	
	int igen_nem;
	bool jatszott = false;
	bool ok = true;
	int ossz_pontszam = 0;
	int korok_szama = 1;

	while (ok) //ameddig tart a jatek
	{
		if (!jatszott)
		{
			cout << "Szeretne jatszodni? (Igen - 1, Nem - 0): " << endl;
		}
		else
		{
			cout << "Szeretne meg egyet jatszodni? (Igen - 1, Nem - 0): " << endl;
		}
		cout << "Valasz: ";
		cin >> igen_nem;
		cout << endl;

		if (igen_nem == 1)
		{
			cout << "Szuper!" << endl;
			cout << "Valaszd ki a nehezsegi szintet: " << endl;
			cout << " - konnyu - 0" << endl;
			cout << " - kozepes - 1" << endl;
			cout << " - nehez - 2" << endl;
			cout << "Valasztott szint: ";
			int szint = 0;
			cin >> szint;
			cout << endl;

			//ha jo a szint
			jatszott = true;

			cout << "INDUL AZ " << korok_szama << ". FORDULO! SOK SIKERT!" << endl;
			int pontszam = 0;
			adott_kor(szint, pontszam);
			ossz_pontszam = ossz_pontszam + pontszam;

			cout << korok_szama << ". forduloban elert pontszam: " << pontszam << endl;
			cout << "Eddig elert osszes pontszam: " << ossz_pontszam << endl << endl;

		}
		else
		{
			cout << "Nagyon sajnaljuk! Varjuk legkozelebb!" << endl;
			ok = false;
			if (jatszott)
			{
				cout << "Az elert pontszamod: " << ossz_pontszam << endl;
			}
		}

		korok_szama++;
	}
	
}

int main()
{
	srand(time(NULL));

	jatek();
}