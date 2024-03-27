#include "blackjack.h"


/* Début du blackjack */


static COORD consS = (COORD){0, 0};
static COORD consP = (COORD){0, 0};


// Initialiser les tableaux des joueurs 
void blackjack::init ( int a[] , int size) {
	for ( int i=0 ; i<size; ++i) { 
		a[i] = 0 ;
	}
}


// Afficher les cartes des joueurs
void blackjack::cartes (int a[]) {

	for (int i = 0; i <=10 ; i++) {
		if (a[i]==2) {
			std::cout<<" "<<a[i]<<" ";
		}

		if (a[i]==3) {
		std::cout<<" "<<a[i]<<" ";
		}

		if (a[i]==4) {
		std::cout<<" "<<a[i]<<" ";
		}

		if (a[i]==5) {
		std::cout<<" "<<a[i]<<" ";
		}

		if (a[i]==6) {
		std::cout<<" "<<a[i]<<" ";
		}

		if (a[i]==7) {
		std::cout<<" "<<a[i]<<" ";
		}

		if (a[i]==8) {
		std::cout<<" "<<a[i]<<" ";
		}

		if (a[i]==9) {
		std::cout<<" "<<a[i]<<" ";
		}

		if (a[i]==10) {
			std::cout<<" "<<a[i]<<" ";
		}

		if (a[i]==11) {
			std::cout<<" A ";
		
		}

		if (a[i]==12) {
			std::cout<<" V ";
		}

		if (a[i]==13) {
			std::cout<<" D ";
		}

		if (a[i]==14) {
			std::cout<<" R ";
		}
	}
}


// Afficher les règles
void blackjack::regles (int & choix) {

	clearConsole(consS);

	std::cout<<" → Au BlackJack, on joue avec les 52 cartes d'un jeu classique "<<std::endl;
	Sleep(2000);
	std::cout<<" → Vous allez jouer contre moi. Le but du jeu est de se rapprocher le plus de la valeur 21 avec les cartes que l'on va se distribuer. "<<std::endl;
	Sleep(5000);
	std::cout<<" → Toutes les cartes chiffrées prennent leur valeur, les têtes (valet V, dame D, roi R) valent également 10 "<<std::endl;
	Sleep(3000);
	std::cout<<" → L'as vaut 11 ou 1 selon la solution la plus arrangeante  "<<std::endl;
	Sleep(3000);
	std::cout<<" → Vous partez à deux cartes et vous choisissez si vous en voulez une de plus ou pas. "<<std::endl;
	Sleep(3000);
	std::cout<<" → Si vous dépassez 21, vous perdez.  "<<std::endl;
	Sleep(3000);
	std::cout<<" → Si vous arrivez à 21 pile, vous gagnez.  "<<std::endl;
	Sleep(3000);
	std::cout<<" → Si vous decidez d'arrêter avant 21, se sera celui de nous deux qui aura la valeur la plus proche de 21 qui gagnera.  "<<std::endl;
	Sleep(3000);
	std::cout<<" → Si vous avez 21 dès les deux premières cartes, vous gagnez.  "<<std::endl;
	Sleep(3000);

	std::cout<<" Avez vous compris les règles ? (Si Non, les règles vont se réafficher) "<<std::endl;
	std::cout<<" 1 = Oui  Autre = Non  "<<std::endl;
	std::cin>>choix;
	if(std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(256,'\n');
		choix = 0;
	}

	if (choix==0) {
		regles(choix);
	}
} 


// Affichage de fin de jeu
void blackjack::fin (std::string prenom, int partie) {

	clearConsole(consS);
	std::cout<<" Vous avez joué  "<<partie<<" parties "<<std::endl;
	std::cout<<" Merci d'avoir joué  "<<prenom<<std::endl;
	std::cout<<" Au revoir,  "<<prenom<<std::endl;
}


void blackjack::verification_as (int joueur[], int & somme, int tour) {
	for (int j=0; j<=tour; j++ ) {
		if (somme > 21 && joueur[j]==11) {
			joueur[j] = 1;
			somme = somme - 10;
		}
	}
}


// Sous-programme principal qui distribue les cartes dans les tableaux 
void blackjack::distribution (int joueur[], int ordi[], int & choix, int & tour, int & somme, int & partie, std::string prenom, int & seuil) {

	if (tour==0) {
		joueur[0] = (rand() % 13) + 2 ;

		if (joueur[0]==14 || joueur[0]==12 || joueur[0]==13) {
			somme = somme + 10;         
		}


		else {  
			somme = somme + joueur[0];
		}
	}
		
		
	clearConsole(consS);

	tour = tour + 1;


	std::cout<<" Voici vos cartes  "<<std::endl;
	joueur[tour] = (rand() % 13) + 2 ;


	cartes(joueur); 

	if (joueur[tour]==14 || joueur[tour]==12 || joueur[tour]==13) {
		somme = somme + 10;         
	}
	else  { 
		somme = somme + joueur[tour];
	} 


	verification_as(joueur,somme, tour);
		
	std::cout<<"  "<<std::endl;
	std::cout<<"  "<<std::endl;

	std::cout<<" La somme de vos cartes est de :  "<<somme;
		
	if (somme == 21) {
		std::cout<<"  "<<std::endl;
		std::cout<<"  "<<std::endl;
		std::cout<<" Vous avez gagné avec 21 "<<std::endl;
		std::cout<<" Voulez-vous rejouer 1 = Oui  Autre = Non  "<<std::endl;
		std::cin>>choix;
		if(std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(256,'\n');
			choix = 0;
		}

		if (choix == 1)  {
					tour = 0;
					somme = 0;
					partie = partie + 1;
					init(joueur, 10);
					init(ordi, 10);
					distribution(joueur,ordi,choix,tour,somme,partie,prenom,seuil);
		}
		else {
			fin(prenom,partie);
		}	   
	}


	if (somme > 21) {
		std::cout<<"  "<<std::endl;
		std::cout<<"  "<<std::endl;
		std::cout<<" Vous avez "<<somme<<std::endl;
		std::cout<<" Vous avez perdu "<<std::endl;
		std::cout<<" Voulez-vous rejouer 1 = Oui  Autre = Non  "<<std::endl;
		std::cin>>choix;
		if(std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(256,'\n');
			choix = 0;
		}

		if (choix == 1)  {
					tour = 0;
					somme = 0;
					partie = partie + 1;
					init(joueur, 10);
					init(ordi, 10);
					distribution(joueur,ordi,choix,tour,somme,partie,prenom,seuil);
		}
		else {
					fin(prenom,partie);
		}
	}
		

	int sommeordi = 0;
	ordi[tour] = ((rand() % 13) + 2);
	sommeordi = sommeordi  + ordi[tour] ;
	verification_as(ordi, sommeordi, tour);
	
	if (sommeordi >= seuil) {
		if (sommeordi > 21) {
			std::cout<<"  "<<std::endl;
			std::cout<<" J'ai "<<sommeordi<<std::endl;
			std::cout<<" Vous avez "<<somme<<std::endl;
			std::cout<<" Vous avez gagné "<<std::endl;
			std::cout<<" Voulez-vous rejouer 1 = Oui  Autre = Non  "<<std::endl;
			std::cin>>choix;
			if(std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(256,'\n');
				choix = 0;
			}

			if (choix == 1)  {
						tour = 0;
						somme = 0;
						partie = partie + 1;
						seuil = ((rand() % 5) + 16);
						init(joueur, 10);
						init(ordi, 10);
						distribution(joueur,ordi,choix,tour,somme,partie,prenom,seuil);
			}
			else {
				fin(prenom,partie);
			}

		}

		if ( sommeordi > somme ) {

			clearConsole(consS);

			std::cout<<"  "<<std::endl;
			std::cout<<" J'ai "<<sommeordi<<std::endl;
			std::cout<<" Vous avez "<<somme<<std::endl;
			std::cout<<" Vous avez perdu "<<std::endl;
			std::cout<<" Voulez-vous rejouer 1 = Oui  Autre = Non  "<<std::endl;
			std::cin>>choix;
			if(std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(256,'\n');
				choix = 0;
			}

			if (choix == 1)  {
				tour = 0;
				somme = 0;
				partie = partie + 1;
				seuil = ((rand() % 5) + 16);
				init(joueur, 10);
				init(ordi, 10);
				distribution(joueur,ordi,choix,tour,somme,partie,prenom,seuil);
			}
			else {
				fin(prenom,partie);
			}
		}

		if ( sommeordi < somme ) {	
			clearConsole(consS);
			std::cout<<"  "<<std::endl;
			std::cout<<"  "<<std::endl;
			std::cout<<" J'ai "<<sommeordi<<std::endl;
			std::cout<<" Vous avez "<<somme<<std::endl;
			std::cout<<" Vous avez gagné "<<std::endl;
			std::cout<<" Voulez-vous rejouer 1 = Oui  Autre = Non  "<<std::endl;
			std::cin>>choix;
			if(std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(256,'\n');
				choix = 0;
			}

			if (choix == 1)  {
				tour = 0;
				somme = 0;
				partie = partie + 1;
				seuil = ((rand() % 5) + 16);
				init(joueur, 10);
				init(ordi, 10);
				distribution(joueur,ordi,choix,tour,somme,partie,prenom,seuil);
			}
			else {
				fin(prenom,partie);
			}
		}

		if ( sommeordi == somme ) {
			clearConsole(consS);
			std::cout<<"  "<<std::endl;
			std::cout<<"  "<<std::endl;
			std::cout<<" J'ai "<<sommeordi<<std::endl;
			std::cout<<" Vous avez "<<somme<<std::endl;
			std::cout<<" Égalité "<<std::endl;
			std::cout<<" Voulez-vous rejouer 1 = Oui  Autre = Non  "<<std::endl;
			std::cin>>choix;
			if(std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(256,'\n');
				choix = 0;
			}

			if (choix == 1)  {
				tour = 0;
				somme = 0;
				partie = partie + 1;
				seuil = ((rand() % 5) + 16);
				init(joueur, 10);
				init(ordi, 10);
				distribution(joueur, ordi, choix, tour, somme, partie,prenom,seuil);
			}
			else {
				fin(prenom,partie);
			}
		}
	}
	else {  
		std::cout<<"  "<<std::endl;
		std::cout<<" Voulez-vous continuer 1 = Oui  Autre = Non  "<<std::endl;
		std::cin>>choix;
		if(std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(256,'\n');
			choix = 0;
		}
	}

	if (choix==1) {
		tour = tour + 1;
		distribution(joueur, ordi, choix, tour,somme, partie,prenom,seuil);
	}
		
	else {
		if ( sommeordi > somme ) {

			clearConsole(consS);

			std::cout<<"  "<<std::endl;
			std::cout<<" J'ai "<<sommeordi<<std::endl;
			std::cout<<" Vous avez "<<somme<<std::endl;
			std::cout<<" Vous avez perdu "<<std::endl;
			std::cout<<" Voulez-vous rejouer 1 = Oui  Autre = Non  "<<std::endl;
			std::cin>>choix;
			if(std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(256,'\n');
				choix = 0;
			}

			if (choix == 1)  {
				tour = 0;
				somme = 0;
				partie = partie + 1;
				seuil = ((rand() % 5) + 16);
				init(joueur, 10);
				init(ordi, 10);
				distribution(joueur,ordi,choix,tour,somme,partie,prenom,seuil);
			}
			else {
				fin(prenom,partie);
			}
			
		}
			
		else if ( sommeordi < somme ) {
			clearConsole(consS);
			std::cout<<"  "<<std::endl;
			std::cout<<"  "<<std::endl;
			std::cout<<" J'ai "<<sommeordi<<std::endl;
			std::cout<<" Vous avez "<<somme<<std::endl;
			std::cout<<" Vous avez gagné "<<std::endl;
			std::cout<<" Voulez-vous rejouer 1 = Oui  Autre = Non  "<<std::endl;
			std::cin>>choix;
			if(std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(256,'\n');
				choix = 0;
			}

			if (choix == 1)  {
				tour = 0;
				somme = 0;
				partie = partie + 1;
				seuil = ((rand() % 5) + 16);
				init(joueur, 10);
				init(ordi, 10);
				distribution(joueur,ordi,choix,tour,somme,partie,prenom,seuil);
			}
			else {
				fin(prenom,partie);
			}
		}

		else if ( sommeordi == somme ) {
			clearConsole(consS);
			std::cout<<"  "<<std::endl;
			std::cout<<"  "<<std::endl;
			std::cout<<" J'ai "<<sommeordi<<std::endl;
			std::cout<<" Vous avez "<<somme<<std::endl;
			std::cout<<" Égalité "<<std::endl;
			std::cout<<" Voulez-vous rejouer 1 = Oui  Autre = Non  "<<std::endl;
			std::cin>>choix;
			if(std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(256,'\n');
				choix = 0;
			}

			if (choix == 1)  {
				tour = 0;
				somme = 0;
				partie = partie + 1;
				seuil = ((rand() % 5) + 16);
				init(joueur, 10);
				init(ordi, 10);
				distribution(joueur, ordi, choix, tour, somme, partie,prenom,seuil);
			}
			else {
				fin(prenom,partie);
			}
		}
	}
}

	

// Affichage et choix du démarrage
void blackjack::demarrer (int joueur[],int ordi[], int & tour, int & choix, int & somme, int & partie, std::string prenom, int & seuil) {
	tour=0;

	std::cout<<" Êtes-vous prêt ? 1 = Oui  Autre = Non  "<<std::endl;
	std::cin>>choix;
	if(std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(256,'\n');
		choix = 0;
	}

	if (choix==1) {
		distribution(joueur, ordi, choix, tour, somme, partie, prenom, seuil);
	}
	else {
		demarrer(joueur, ordi, tour,choix,somme,partie,prenom,seuil); 
	}
}

int blackjack::main() {
	clearConsole(); // Vider la console

	std::cout << "Blackjack - Ryan" << std::endl;
	getConsolePos(&consS);


	std::string prenom;
	int joueur[10];
	int ordi[10];
	int choix;
	int tour=0;
	int somme=0;

	init(joueur, 10);
	init(ordi, 10);
	
	std::cout<<" Quel est votre prénom ? "<<prenom<<std::endl;
	std::cin>>prenom;

	clearConsole(consS);


	std::cout<<" Bonjour "<<prenom<<std::endl;
	Sleep(1000);
	std::cout<<" Nous allons jouer au BlackJack ensemble "<<std::endl;
	Sleep(1000);

	std::cout<<" Voulez vous lire les règles ? 1 = Oui  Autre = Non " <<std::endl;
	std::cin>>choix;
	if(std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(256,'\n');
		choix = 0;
	}

	if (choix==1) {
		regles(choix);
	}

	clearConsole(consS);

	std::cout<<" Alors nous allons commencer ! "<<std::endl;
	Sleep(1000);

	int partie = 1;
	int seuil = ((rand() % 5) + 16);

	demarrer(joueur, ordi, tour, choix, somme, partie, prenom, seuil);

	return 0;
}