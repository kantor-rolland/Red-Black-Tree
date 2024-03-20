#include <iostream>
#include "piros_fekete_fa.h"

using namespace std;

Elem* LetrehozElem()
{
	Elem* elem = new Elem;
	elem->ertek = 0;
	elem->bal_kov = NULL;
	elem->jobb_kov = NULL;
	elem->elozo = NULL;
	elem->szin = 0; //fekete
	elem->level = true;

	return elem;
}

Elem* LetrehozElem(int ertek)
{
	Elem* Level = LetrehozElem();
	Elem* elem = LetrehozElem();
	Level->elozo = elem;

	elem->ertek = ertek;
	elem->bal_kov = Level;
	elem->jobb_kov = Level;
	elem->elozo = NULL;
	elem->szin = 1; //piros
	elem->level = false;
	
	return elem;
}

void FelszabaditElem(Elem* elem)
{
	delete elem->bal_kov;
	delete elem->jobb_kov;
	//delete elem;
}

Fa* LetrehozFa()
{
	Fa* A = new Fa();
	Elem* level = new Elem();

	A->gyoker = level;
	A->csomopontok_szama = 0;
	return A;
}

void FelszabaditFaCsomopont(Elem* elem)
{
	if (elem->bal_kov->level == false)
	{
		//FelszabaditElem(elem->bal_kov);
		FelszabaditFaCsomopont(elem->bal_kov);
	}
	else if (elem != NULL)
	{
		FelszabaditElem(elem->bal_kov);
	}

	if (elem->jobb_kov->level == false)
	{
		FelszabaditFaCsomopont(elem->jobb_kov);
		//FelszabaditElem(elem->jobb_kov);
	}
	else if (elem != NULL)
	{
		FelszabaditElem(elem->jobb_kov);
	}
	
	delete elem;
}

void FelszabaditFa(Fa* A)
{
	if (A != NULL)
	{
		if (A->gyoker != NULL)
		{
			FelszabaditFaCsomopont(A->gyoker);
		}
		
	}
	delete A;
}

void BalraForgat(Fa* A, Elem* x)
{
	if (A->gyoker == x) //Ha gyoker
	{
		if (x->jobb_kov->level == false)
		{
			Elem* y = x->jobb_kov;

			A->gyoker = y;
			x->jobb_kov = y->bal_kov;
			y->bal_kov = x;
			x->elozo = y;
			y->elozo = NULL; //UJ GYOKER	
		}
	}
	else //Ha nem gyoker -> van elozoje
	{
		if (x->jobb_kov->level == false)
		{
			Elem* y = x->jobb_kov;
			Elem* z = x->elozo;

			if (x == z->bal_kov)
			{
				z->bal_kov = y; //z->jobb=y 
			}
			else
			{
				z->jobb_kov = y;
			}

			x->jobb_kov = y->bal_kov;
			y->bal_kov = x;
			y->elozo = x->elozo; // vagy z
			x->elozo = y;
		}
	}
}

void JobbraForgat(Fa* A, Elem* x)
{
	if (A->gyoker == x)
	{
		if (x->bal_kov->level == false)
		{
			Elem* y = x->bal_kov;

			A->gyoker = y;
			x->bal_kov = y->jobb_kov;
			y->jobb_kov = x;
			x->elozo = y;
			y->elozo = NULL; //UJ GYOKER	
		}
	}
	else //Ha nem gyoker van elozoje
	{
		if (x->bal_kov->level == false)
		{
			Elem* y = x->bal_kov;
			Elem* z = x->elozo;

			z->jobb_kov = y;
			x->bal_kov = y->jobb_kov;
			y->jobb_kov = x;
			y->elozo = x->elozo;
			x->elozo = y;
		}
	}
}

void Beszur_Rek(Fa* A, Elem* csomopont, Elem*& ujelem)
{
	if (ujelem->ertek < csomopont->ertek)
	{
		if (csomopont->bal_kov->level == false)
		{
			Beszur_Rek(A, csomopont->bal_kov, ujelem);
		}
		else
		{
			ujelem->elozo = csomopont;
			ujelem->elozo->level = false;
			csomopont->bal_kov = ujelem;
		}
	}
	else
	{
		//if (csomopont->jobb_kov != NULL)
		if (csomopont->jobb_kov->level == false)
		{
			Beszur_Rek(A, csomopont->jobb_kov, ujelem);
		}
		else
		{
			ujelem->elozo = csomopont;
			csomopont->jobb_kov = ujelem;
			ujelem->elozo->level = false;
		}
	}
}

void Beszuras(Fa* A, int ertek)
{
	Elem* ujelem = new Elem(ertek);

	if (A->gyoker->level == 1) //ha ures a fa
	{
		ujelem->elozo = NULL;
		ujelem->szin = 0; //fekete a gyoker
		A->gyoker = ujelem;
		A->gyoker->level = 0;
		A->csomopontok_szama++;
	}
	else
	{
		Beszur_Rek(A, A->gyoker, ujelem); //beszurjuk rendesen
		A->csomopontok_szama++;
	}

	if (ujelem->elozo == NULL || ujelem->elozo->elozo == NULL) //ha csak 1 vagy 2 szintes a fa
	{
		A->gyoker->szin = 0;
		A->gyoker->level = 0;
		return;
	}

	//Javitas
	//3.eset: 2 egymast koveto csomopont piros (a beszur es annak az elozoje(szuloje))

	while (ujelem != A->gyoker && ujelem->elozo->szin != 0)
	{
		if (ujelem->elozo == ujelem->elozo->elozo->bal_kov) //bal leszarmazott
		{
			int szin = 0;

			if (ujelem->elozo->elozo->jobb_kov->level)
			{
				szin = 0;
			}
			else
			{
				szin = ujelem->elozo->elozo->jobb_kov->szin;
			}
			//3.2 eset:
			//if (ujelem->szin == 1 && ujelem->elozo->szin == 1 && ujelem->elozo->elozo->jobb_kov->szin == 1)
			if (szin == 1)
			{
				ujelem->elozo->szin = 0; //szulo
				ujelem->elozo->elozo->jobb_kov->szin = 0; //nagybacsi
				ujelem->elozo->elozo->szin = 1; //nagyszulo

				ujelem = ujelem->elozo->elozo;
			}
			else
			{
				bool volt = false;
				//ha az adott elem jobb leszarmazott, akkor ket forgatast kell vegrahajtani
				if (ujelem == ujelem->elozo->jobb_kov)
				{
					BalraForgat(A, ujelem->elozo);
					volt = true;
				}

				//az ujelem elozoje most megvaltozott
				if (volt)
				{
					JobbraForgat(A, ujelem->elozo);
					ujelem->szin = 0;
					ujelem->jobb_kov->szin = 1;
				}
				else
				{
					JobbraForgat(A, ujelem->elozo->elozo);
					ujelem->elozo->szin = 0;
					ujelem->elozo->jobb_kov->szin = 1;
				}

			}
		}
		else //szimetrikusan
		{
			int szin = 0;

			if (ujelem->elozo->elozo->bal_kov->level)
			{
				szin = 0;
			}
			else
			{
				szin = ujelem->elozo->elozo->bal_kov->szin;
			}
			//3.2 eset:
			//if (ujelem->szin == 1 && ujelem->elozo->szin == 1 && ujelem->elozo->elozo->bal_kov->szin == 1)
			if (szin == 1)
			{
				ujelem->elozo->szin = 0; //szulo
				ujelem->elozo->elozo->bal_kov->szin = 0; //nagybacsi
				ujelem->elozo->elozo->szin = 1; //nagyszulo

				ujelem = ujelem->elozo->elozo;
			}
			else
			{
				bool volt = false;
				//ha az adott elem jobb leszarmazott, akkor ket forgatast kell vegrahajtani
				if (ujelem == ujelem->elozo->bal_kov)
				{
					JobbraForgat(A, ujelem->elozo);
					volt = true;
				}

				//az ujelem elozoje most megvaltozott
				if (volt)
				{
					BalraForgat(A, ujelem->elozo);
					ujelem->szin = 0;
					ujelem->bal_kov->szin = 1;
				}
				else
				{
					BalraForgat(A, ujelem->elozo->elozo);
					ujelem->elozo->szin = 0;
					ujelem->elozo->bal_kov->szin = 1;
				}
			}
		}
	}
	A->gyoker->szin = 0; //a gyoker mindig fekte
}

Elem* Keres_Rek(Elem* A, int elem)
{
	if (A != NULL && A->level == false) //HA NEM NULL ES NEM LEVEL
	{
		if (A->ertek > elem)
		{
			return Keres_Rek(A->bal_kov, elem);
		}
		else
		{
			if (A->ertek < elem)
			{
				return Keres_Rek(A->jobb_kov, elem);
			}
			else
			{
				if (A->ertek == elem)
				{
					return A;
				}
				else
				{
					return NULL;
				}

			}
		}
	}
	else //nincs benne
	{
		return NULL;
	}

}

void Kereses(Fa* A, int elem)
{
	if (A->gyoker == NULL)
	{
		cout << "A fa ures!" << endl;
	}
	else
	{
		Elem* eredmeny = Keres_Rek(A->gyoker, elem);

		if (eredmeny != NULL)
		{
			cout << "A keresett elem: " << eredmeny->ertek << endl;
			if (eredmeny->elozo != NULL)
			{
				cout << "Szuloje: " << eredmeny->elozo->ertek << endl;
			}
			else
			{
				cout << "Szuloje: - " << endl;
			}

			if (eredmeny->bal_kov)
			{
				cout << "Bal: " << eredmeny->bal_kov->ertek;
			}
			else
			{
				cout << "Bal: - ";
			}

			if (eredmeny->jobb_kov)
			{
				cout << "  Jobb: " << eredmeny->jobb_kov->ertek << endl;
			}
			else
			{
				cout << "  Jobb: - " << endl;
			}


			if (eredmeny->szin == 0)
			{
				cout << "Szine: fekete" << endl;
			}
			else
			{
				cout << "Szine: piros" << endl;
			}
			cout << endl;
		}
		else
		{
			cout << "A keresett elem nem talalhato meg a faban!" << endl;
		}
	}
}

Elem* Minimum_Rek(Elem* csomopont)
{
	if (csomopont != NULL && csomopont->bal_kov->level == false)
	{
		return Minimum_Rek(csomopont->bal_kov);
	}
	else return csomopont;
}

void Minimum(Fa* A)
{
	Elem* eredmeny = Minimum_Rek(A->gyoker);
	cout << "A legkisebb elem: " << eredmeny->ertek << endl;
}

Elem* Maximum_Rek(Elem* csomopont)
{
	if (csomopont != NULL && csomopont->jobb_kov->level == false)
	{
		return Maximum_Rek(csomopont->jobb_kov);
	}
	return csomopont;
}

void Maximum(Fa* A)
{
	Elem* eredmeny = Maximum_Rek(A->gyoker);
	cout << "A legnagyobb elem: " << eredmeny->ertek << endl;
}

void AdottSorszamuElem_Rek(Elem* csomopont, int hanyadik, int& rang)
{
	//int rang = 0;
	if (csomopont->level == false)
	{
		if (rang <= hanyadik)
		{
			AdottSorszamuElem_Rek(csomopont->bal_kov, hanyadik, rang);
			rang++;

			if (rang == hanyadik)
			{
				cout << "Az " << hanyadik << ". elem = " << csomopont->ertek << endl;
			}

			AdottSorszamuElem_Rek(csomopont->jobb_kov, hanyadik, rang);
		}
	}
}

void AdottSorszamuElem(Fa* A, int sorszam)
{
	int s = 0;
	AdottSorszamuElem_Rek(A->gyoker, sorszam, s);
}

void RangElem_Rek(Elem* csomopont, int elem, int& rang)
{
	if (csomopont->level == false)
	{
		RangElem_Rek(csomopont->bal_kov, elem, rang);
		rang++;

		if (csomopont->ertek == elem)
		{
			cout << elem << " elem rangja = " << rang << endl;
		}

		RangElem_Rek(csomopont->jobb_kov, elem, rang);
	}
}

void Rang(Fa* A, int elem)
{
	int rang = 0;

	RangElem_Rek(A->gyoker, elem, rang);
}

int Magassag(Fa* A)
{
	Elem* seged = A->gyoker;
	int h = 0;
	while (seged != NULL && seged->level == false)
	{
		if (seged->szin == 0)
		{
			h++;
		}
		seged = seged->bal_kov; //mindegy melyik agat nezzuk
	}

	return h;
}

void Inorder(Elem* csomopont)
{
	if (csomopont->level == false)
	{
		Inorder(csomopont->bal_kov);
		if (csomopont != NULL)
		{
			if (csomopont->szin == 1)
			{
				cout << csomopont->ertek << " - PIROS" << endl;
			}
			else
			{
				cout << csomopont->ertek << " - FEKETE" << endl;
			}
		}
		Inorder(csomopont->jobb_kov);
	}
}

void Postorder(Elem* csomopont)
{
	if (csomopont->level != true)
	{
		Postorder(csomopont->bal_kov);
		Postorder(csomopont->jobb_kov);
		//cout << csomopont->ertek << ' ';
		if (csomopont != NULL)
		{
			if (csomopont->szin == 1)
			{
				cout << csomopont->ertek << " - PIROS" << endl;
			}
			else
			{
				cout << csomopont->ertek << " - FEKETE" << endl;
			}
		}
	}
}

void Preorder(Elem* csomopont)
{
	if (csomopont->level != true)
	{
		// << csomopont->ertek << ' ';
		if (csomopont != NULL)
		{
			if (csomopont->szin == 1)
			{
				cout << csomopont->ertek << " - PIROS" << endl;
			}
			else
			{
				cout << csomopont->ertek << " - FEKETE" << endl;
			}
		}
		Preorder(csomopont->bal_kov);
		Preorder(csomopont->jobb_kov);
	}
}

Elem* NagyobbLegkisebb(Elem* csomopont)
{
	if (csomopont->jobb_kov->level != true)
	{
		Elem* y = Minimum_Rek(csomopont->jobb_kov);
		//cout << "Nagyobb legkisebb: " << y->ertek << endl;
		return y;
	}
	return csomopont;
}

Elem* KisebbLegnagyobb(Elem* csomopont)
{
	if (csomopont->jobb_kov->level != true)
	{
		Elem* y = Maximum_Rek(csomopont->jobb_kov);
		//cout << "Kisebb legnagyobb: " << y->ertek << endl;
		return y;
	}
	return csomopont;
}

void Transplant(Fa* A, Elem* x, Elem* y)
{
	//if (x->elozo->level == false )
	if (x->elozo != NULL) //HA NEM GYOKER
	{
		if (x == x->elozo->bal_kov) //bal v jobb
		{
			x->elozo->bal_kov = y;
		}
		else
		{
			x->elozo->jobb_kov = y;
		}
	}
	else //HA GYOKER
	{
		A->gyoker = y;
	}
	y->elozo = x->elozo;
}

void Torles_Javitas(Fa* A, Elem*& x)
{
	Elem* y;

	while (A->gyoker != x && x->szin != 1)
	{
		if (x == x->elozo->bal_kov) //bal leszarmazott
		{
			y = x->elozo->jobb_kov; //testver

			if (y->szin == 1) //ha piros
			{
				y->szin = 0;
				x->elozo->szin = 1;
				BalraForgat(A, x->elozo);
				//Balra(A, x->elozo);
				y = x->elozo->jobb_kov;

				if (x->elozo->jobb_kov->elozo != x->elozo)
				{
					x->elozo->jobb_kov->elozo = x->elozo;
				}
			}

			if (y->level || (y->bal_kov->szin == 0 && y->jobb_kov->szin == 0))
			{
				y->szin = 1;
				x = x->elozo;
			}
			else
			{
				if (y->jobb_kov->szin == 0) //megnezni a szin valtast
				{
					y->bal_kov->szin = 0;
					y->szin = 1;
					JobbraForgat(A, y);
					//Jobbra(A, y);
					y = x->elozo->jobb_kov;
				}
				y->szin = x->elozo->szin;
				x->elozo->szin = 0;
				y->jobb_kov->szin = 0;
				BalraForgat(A, x->elozo);
				//Balra(A, x->elozo);
				x = A->gyoker;
			}
		}
		else //ez szimetrikusan
		{
			y = x->elozo->bal_kov; //testver

			if (y->szin == 1)
			{
				y->szin = 0;
				x->elozo->szin = 1;
				BalraForgat(A, x->elozo);
				//Balra(A, x->elozo);
				y = x->elozo->bal_kov;
			}

			if (y->jobb_kov->szin == 0 && y->bal_kov->szin == 0) //ha a ket leszarmazott szine megegyezik
			{
				y->szin = 1; //piros lesz
				x = x->elozo;
			}
			else
			{
				if (y->bal_kov->szin == 0) //megnezni a szin valtast
				{
					y->jobb_kov->szin = 0;
					y->szin = 1;
					JobbraForgat(A, y);
					//Jobbra(A, y);
					y = x->elozo->bal_kov;
				}

				y->szin = x->elozo->szin;
				x->elozo->szin = 0;
				y->bal_kov->szin = 0;
				BalraForgat(A, x->elozo);
				//Balra(A, x->elozo);
				x = A->gyoker;
			}

		}
	}
	x->szin = 0; //gyoker mindig fekete

}

void TorolElem(Fa* A, Elem* x)
{
	//x - a torlendo elem
	Elem* masolat = x;
	Elem* z = new Elem(); //level
	int szin = x->szin;

	if (x->bal_kov->level == true)
	{
		//if (x->jobb_kov->level == false)
		{
			z = x->jobb_kov;
			Transplant(A, x, x->jobb_kov);
		}
	}
	else if (x->jobb_kov->level == true)
	{
		//if (x->bal_kov->level == false)
		{
			z = x->bal_kov;
			Transplant(A, x, x->bal_kov);
		}
	}
	else //ket leszarmazott
	{
		//masolat = NagyobbLegkisebb(x->jobb_kov); //megkapjuk a successorjat
		masolat = NagyobbLegkisebb(x);

		//if (masolat != NULL)
		{
			szin = masolat->szin;
			z = masolat->jobb_kov;

			if (masolat->elozo == x)
			{
				z->elozo = masolat;
			}
			else
			{
				Transplant(A, masolat, masolat->jobb_kov);
				masolat->jobb_kov = x->bal_kov;
				masolat->bal_kov->elozo = masolat;
			}

			//transplant(A, masolat, masolat->jobb_kov);
			Transplant(A, x, masolat);
			//masolat->jobb_kov = x->bal_kov;


			if (masolat == x->bal_kov)
			{
				masolat->jobb_kov = x->jobb_kov;
				masolat->jobb_kov->elozo = masolat;
				masolat->szin = x->szin;
			}
			else if (masolat == x->jobb_kov)
			{
				masolat->bal_kov = x->bal_kov;
				masolat->bal_kov->elozo = masolat;
				masolat->szin = x->szin;
			}
			else //ha nem volt szomszed 
			{
				masolat->bal_kov = x->bal_kov;
				masolat->bal_kov->elozo = masolat;
				masolat->jobb_kov = x->jobb_kov;
				masolat->jobb_kov->elozo = masolat;
				masolat->szin = x->szin;
			}
		}
	}

	delete x;

	if (szin == 0)//ha fekete akkor baj van
	{
		Torles_Javitas(A, z);
	}

}

void Torol(Fa* A, int elem)
{
	Elem* x = Keres_Rek(A->gyoker, elem);

	if (x == NULL || x->level == true)
	{
		cout << "Az nem talalhato meg a afaban" << endl;
	}
	else
	{
		TorolElem(A, x);
		A->csomopontok_szama--;
	}
}