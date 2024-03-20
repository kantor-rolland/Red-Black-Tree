#pragma once
#include <iostream>
#include <string>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

typedef struct Elem
{
	int ertek;

	Elem* bal_kov;
	Elem* jobb_kov;
	Elem* elozo;
	int szin;
	bool level;

	Elem() //alapertelmezett
	{
		this->ertek = 0;
		bal_kov = NULL;
		jobb_kov = NULL;
		elozo = NULL;
		szin = 0; //fekete
		level = true;
	}
	Elem(int ertek) //konstruktor
	{
		Elem* Level = new Elem();
		Level->elozo = this;

		this->ertek = ertek;
		bal_kov = Level;
		jobb_kov = Level;
		elozo = NULL;
		szin = 1; //piros
		level = false;
	}

}Elem;

typedef struct Fa
{
	Elem* gyoker;
	//Elem* TNULL;
	int csomopontok_szama;

	Fa()
	{
		this->gyoker = NULL;
		this->csomopontok_szama = 0;
	}
}Fa;


Elem* LetrehozElem();
Elem* LetrehozElem(int);
void FelszabaditElem(Elem* elem);

Fa* LetrehozFa();
void FelszabaditFaCsomopont(Elem* elem);
void FelszabaditFa(Fa* A);

void Beszuras(Fa*, int);
void Beszur_Rek(Fa*, Elem*, Elem*&);

void Kereses(Fa*, int);
Elem* Keres_Rek(Elem*, int); //nem csak az erteket teritsuk vissza, hanem az egesz csomopontot

void Minimum(Fa*);
Elem* Minimum_Rek(Elem*);

void Maximum(Fa*);
Elem* Maximum_Rek(Elem*);

int Magassag(Fa*);

void AdottSorszamuElem_Rek(Elem*, int, int&);
void AdottSorszamuElem(Fa*, int);

void RangElem_Rek(Elem*, int, int&);
void Rang(Fa*, int);

void Inorder(Elem*);
void Postorder(Elem*);
void Preorder(Elem*);


void BalraForgat(Fa*, Elem*);
void JobbraForgat(Fa*, Elem*);

Elem* NagyobbLegkisebb(Elem*);
Elem* KisebbLegnagyobb(Elem*);
void Torol(Fa*, int);

void Torles_Javitas(Fa*, Elem*&);
void Transplant(Fa*, Elem*, Elem*);
void TorolElem(Fa*, Elem*);
void Torol(Fa*, int);
