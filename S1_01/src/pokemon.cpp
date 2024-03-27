#include "pokemon.h"


/* Début du pokémon */


static COORD consS = (COORD){0, 0};
static COORD consP = (COORD){0, 0};


// Permet de mettre les PV des différents pokémons
void pokemon::choix_pokemon (int & a,int & b) {

	std::cin>>a;
	while(std::cin.fail() || (a < 1 || a > 3)) {
		std::cout << "Ceci n'est pas un nombre valide" << std::endl;
		std::cin.clear();
		std::cin.ignore(256,'\n');
		std::cin >> a;
	}

	if (a==1) {
		b=220;
	}
	if (a==2) {
		b=200;
	}
	if (a==3) {
		b=180;
	}

}


// Même chose que pour choix_pokemon mais sans la saisie par l'utilisateur
void pokemon::choix_ordi(int & a, int & b) {
	if (a==1) {
		b=220;
	}
	if (a==2) {
		b=200;
	}
	if (a==3) {
		b=180;
	}
}


// Affichage des différents choix de pokemons
void pokemon::faire_choix (int a) {
	std::cout<<" 1 = Bulbizarre (type Plante)"<<std::endl;
	std::cout<<" 2 = Carapuce (type Eau)"<<std::endl;
	std::cout<<" 3 = Salamèche (type Feu)"<<std::endl;
}


// Affichage du pokemon dans les differents textes grâce à la variable de selection du pokemon
void pokemon::affichage_choix (int a) {
	if (a==1) {
		std::cout<<" Bulbizarre (type Plante)"<<std::endl;
	}
	if (a==2) {
		std::cout<<" Carapuce (type Eau)"<<std::endl;
	}
	if (a==3) {
		std::cout<<" Salamèche (type Feu)"<<std::endl;
	}
}


// Choix de l'attaque du type plante
void pokemon::choix_atk_plante(int & a) {

	std::cout<<" Choisissez une attaque ! "<<std::endl;
	std::cout<<" ----------------------- "<<std::endl;

	std::cout<<" "<<std::endl;


	std::cout<<" 1 = Vive-attaque "<<std::endl;
	std::cout<<" - (de 20 à 25 dmg) "<<std::endl;
	std::cout<<" - - N'échoue jamais "<<std::endl;
	std::cout<<" "<<std::endl;


	std::cout<<" 2 = Vampigraine "<<std::endl;
	std::cout<<" - (de 25 à 30 dmg) "<<std::endl;
	std::cout<<" - - Gain de 10PV à 15PV "<<std::endl;
	std::cout<<" "<<std::endl;



	std::cout<<" 3 = Tranch'Herb "<<std::endl;
	std::cout<<" - (de 30 à 35 dmg) "<<std::endl;
	std::cout<<" "<<std::endl;

	std::cin>>a;
	while(std::cin.fail() || (a < 1 || a > 3)) {
		std::cout << "Ceci n'est pas un nombre valide" << std::endl;
		std::cin.clear();
		std::cin.ignore(256,'\n');
		std::cin >> a;
	}
}


// Choix de l'attaque du type feu
void pokemon::choix_atk_feu(int & a) {

	std::cout<<" Choisissez une attaque ! "<<std::endl;
	std::cout<<" ----------------------- "<<std::endl;

	std::cout<<" "<<std::endl;


	std::cout<<" 1 = Vive-attaque "<<std::endl;
	std::cout<<" - (de 20 à 25 dmg) "<<std::endl;
	std::cout<<" - - N'échoue jamais "<<std::endl;
	std::cout<<" "<<std::endl;

	std::cout<<" 2 = Flammêche "<<std::endl;
	std::cout<<" - (de 50 à 55 dmg) "<<std::endl;
	std::cout<<" "<<std::endl;

	std::cout<<" 3 = Deflagration "<<std::endl;
	std::cout<<" - (de 60 à 70 dmg) "<<std::endl;
	std::cout<<" - - Perte de 30PV "<<std::endl;
	std::cout<<" "<<std::endl;

	std::cin>>a;
	while(std::cin.fail() || (a < 1 || a > 3)) {
		std::cout << "Ceci n'est pas un nombre valide" << std::endl;
		std::cin.clear();
		std::cin.ignore(256,'\n');
		std::cin >> a;
	}
}


// Choix de l'attaque du type eau
void pokemon::choix_atk_eau(int & a) {

	std::cout<<" Choisissez une attaque ! "<<std::endl;
	std::cout<<" ----------------------- "<<std::endl;
	std::cout<<" "<<std::endl;

	std::cout<<" 1 = Vive-attaque "<<std::endl;
	std::cout<<" - (de 20 à 25 dmg) "<<std::endl;
	std::cout<<" - - N'échoue jamais "<<std::endl;
	std::cout<<" "<<std::endl;

	std::cout<<" 2 = Vibraqua "<<std::endl;
	std::cout<<" - (de 40 à 45 dmg) "<<std::endl;
	std::cout<<" "<<std::endl;

	std::cout<<" 3 = Hydroqueue "<<std::endl;
	std::cout<<" - (0 dmg) "<<std::endl;
	std::cout<<" - - Prochaine attaque à +20 dmg "<<std::endl;
	std::cout<<" "<<std::endl;

	std::cin>>a;
	while(std::cin.fail() || (a < 1 || a > 3)) {
		std::cout << "Ceci n'est pas un nombre valide" << std::endl;
		std::cin.clear();
		std::cin.ignore(256,'\n');
		std::cin >> a;
	}
}


// Chance de coup critique et d'esquive
void pokemon::coup_critique_et_esquive ( int & b, int & c) {

	int a = rand() % 9;
	if (a==8) {
		c = c + 10 ;
		b = b - 10 ;
		std::cout<<" "<<std::endl;
		std::cout<<" ||   Coup critique !   || "<<std::endl;
		std::cout<<" "<<std::endl;
		Sleep(1000);

	}
	else if (a==7) {
		b = b + c;
		c = 0;
		std::cout<<" "<<std::endl;
		std::cout<<" ||    Esquivé !   || "<<std::endl;
		std::cout<<" "<<std::endl;
		Sleep(1000);


	}
}


// Affichage et application des avantages liés aux types des pokemons
void pokemon::avantage_type (int pok, int pokad, int & atk)  {

	if ( pok == 1 ) {
		if (pokad==3) {
			atk = atk -  (atk*0.3) ;
			std::cout<<" "<<std::endl;
			std::cout<<" Pas très efficace... "<<std::endl;
			std::cout<<" Les dégats du type Plante sont réduits de 30% contre le type Feu "<<std::endl;
			std::cout<<" "<<std::endl;
		}
		if (pokad==2) {
			atk = atk +  (atk*0.3) ;
			std::cout<<" "<<std::endl;
			std::cout<<" Super efficace "<<std::endl;
			std::cout<<" Les dégats du type Plante sont augmentés de 30% contre le type Eau "<<std::endl;
			std::cout<<" "<<std::endl;
		}
		
	}



	if ( pok == 2 ) {
		if (pokad==1) {
			atk = atk -  (atk*0.3) ;
			std::cout<<" "<<std::endl;
			std::cout<<" Pas très efficace... "<<std::endl;
			std::cout<<" Les dégats du type Eau sont réduits de 30% contre le type Plante "<<std::endl;
			std::cout<<" "<<std::endl;
		}
		if (pokad==3) {
			atk = atk +  (atk*0.3) ;
			std::cout<<" "<<std::endl;
			std::cout<<" Super efficace "<<std::endl;
			std::cout<<" Les dégats du type Eau sont augmentés de 30% contre le type Feu "<<std::endl;
			std::cout<<" "<<std::endl;
		}
		
	}



	if ( pok == 3 ) {
		if (pokad==2) {
			atk = atk -  (atk*0.3)  ;
			std::cout<<" "<<std::endl;
			std::cout<<" Pas très efficace... "<<std::endl;
			std::cout<<" Les dégats du type Feu sont réduits de 30% contre le type Eau "<<std::endl;
			std::cout<<" "<<std::endl;
		}
		if (pokad==1) {
			atk = atk +  (atk*0.3)  ;
			std::cout<<" "<<std::endl;
			std::cout<<" Super efficace "<<std::endl;
			std::cout<<" Les dégats du type Feu sont augmentés de 30% contre le type Plante "<<std::endl;
			std::cout<<" "<<std::endl;
		}
		
	}
} 


// Affichage des dégats subits
void pokemon::aff_dmg (int a, int b) {

	affichage_choix(a);
	std::cout<<" a subit "<<b<<" dégats"<<std::endl;
	std::cout<<" "<<std::endl;
	Sleep(1000);
}


// Attaque Vive-Attaque (n'échoue jamais, dégats basiques)
void pokemon::vive_attaque (int & a,int & b, int pok) {

	a = a + ( (rand() % 6) + 20);
	b = b - a;

	aff_dmg(pok,a);
}


//  Attaque Déflagration (Perte de 30PV par l'utilisateur, gros dégats)
void pokemon::deflagration (int & a,int & b, int & c, int pok, int pokad) {

	a = (rand() % 11) + 60;
	b = b - a ;
	c = c - 30;

	affichage_choix(pokad);
	std::cout<<" a subit 30 dégats"<<std::endl;
	std::cout<<"  "<<std::endl;

	Sleep(1000);

	coup_critique_et_esquive (b,a);
	avantage_type (pokad,pok,a);
	aff_dmg(pok,a);
}


// Attaque Flammêche (Attaque normale)
void pokemon::flammeche (int & a,int & b, int pok, int pokad) {

	a = (rand() % 6) + 50;
	b = b - a ;
   

	coup_critique_et_esquive (b,a);
	avantage_type (pokad,pok,a);
	aff_dmg(pok,a);
}


// Attaque Tranch'Herb (Attaque normale)
void pokemon::trancherb (int & a,int & b, int pok, int pokad) {

	a = (rand() % 6) + 30;
	b = b - a ;

	coup_critique_et_esquive (b,a);
	avantage_type (pokad,pok,a);
	aff_dmg(pok,a);
}


// Attaque Vampigraine (Gain de PV par l'utilisateur, légers dégats)
void pokemon::vampigraine (int & a,int & b,int & c, int pok, int pokad) {

	a = (rand() % 6) + 25;
	b = b - a ;
	int y = (rand() % 6) + 10;
	c = c + y ;

	affichage_choix(pokad);
	std::cout<<" a gagné "<< y <<" PV"<<std::endl;
	std::cout<<"  "<<std::endl;
	Sleep(1000);

	coup_critique_et_esquive (b,a);
	avantage_type (pokad,pok,a);
	aff_dmg(pok,a);
}


//Attaque Vibraqua (Attaque normale)
void pokemon::vibraqua (int & a,int & b, int pok, int pokad) {

	a = a + (rand() % 6) + 40;
	b = b - a ;
   

	coup_critique_et_esquive (b,a);
	avantage_type (pokad,pok,a);
	aff_dmg(pok,a);
}


// Attaque Hydroqueue (20 de dégats de plus au tour suivant, plusieurs Hydroqueue cumulable)
void pokemon::hydroqueue (int & a,int & b, int pok) {

	int c = 0;
	
	aff_dmg(pok,c);
}


int pokemon::main() {
	clearConsole(); // Vider la console

	std::cout << "Pokémon - Ryan" << std::endl;
	getConsolePos(&consS);
	

	int pokemon_joueur;
	int pokemon_ordi;
	int pv_joueur;
	int pv_ordi;
	int attaque_joueur;
	int attaque_ordi;
	int choix;
	int hydro1=0;
	int hydro2=0;

	clearConsole(consS);

	std::cout<<" Veuillez choisir votre pokémon"<<std::endl;
	faire_choix(pokemon_joueur);
	choix_pokemon (pokemon_joueur,pv_joueur);

	std::cout<<" "<<std::endl;

	pokemon_ordi = (rand() % 3) + 1;
	choix_ordi (pokemon_ordi,pv_ordi);

	std::cout<<" "<<std::endl;

	std::cout<<" Vous avez choisi : "<<std::endl;
	affichage_choix (pokemon_joueur);
	std::cout<<" Vous avez "<<pv_joueur<<" PV"<<std::endl;

	std::cout<<" "<<std::endl;

	std::cout<<" Vous combattez contre : "<<std::endl;
	affichage_choix (pokemon_ordi);
	std::cout<<" Votre adversaire à "<<pv_ordi<<" PV"<<std::endl;

	std::cout<<" "<<std::endl;

	clearConsole(consS);


	std::cout<<" ---------------------- "<<std::endl;
	std::cout<<" QUE LE COMBAT COMMENCE ! "<<std::endl;
	std::cout<<" ---------------------- "<<std::endl;

	Sleep(1000);



	while (pv_joueur>0 && pv_ordi>0) {

		std::cout<<" "<<std::endl;
		affichage_choix(pokemon_joueur);
		std::cout<<pv_joueur<<" PV"<<std::endl;
		std::cout<<" "<<std::endl;

		std::cout<<" VS "<<std::endl;

		std::cout<<" "<<std::endl;
		affichage_choix(pokemon_ordi);
		std::cout<<pv_ordi<<" PV"<<std::endl;
		std::cout<<" "<<std::endl;

		Sleep(1000);


		attaque_joueur = 0 ;
		attaque_ordi = 0;

		if (pokemon_joueur==1) {
			choix_atk_plante(choix);
			std::cout<<" "<<std::endl;
			Sleep(500);
			clearConsole(consS);
			Sleep(500);


			if (choix==1) {
				affichage_choix(pokemon_joueur);
				std::cout<<" utilise Vive-Attaque"<<std::endl;
				vive_attaque(attaque_joueur, pv_ordi, pokemon_ordi);

			}
			if (choix==2) {
				affichage_choix(pokemon_joueur);
				std::cout<<" utilise Vampigraine"<<std::endl;
				vampigraine(attaque_joueur, pv_ordi, pv_joueur, pokemon_ordi, pokemon_joueur);
			}
			if (choix==3) {
				affichage_choix(pokemon_joueur);
				std::cout<<" utilise Tranch'Herb"<<std::endl;
				trancherb(attaque_joueur, pv_ordi, pokemon_ordi, pokemon_joueur);

			}
		}

		if (pokemon_joueur==2) {
			choix_atk_eau(choix);
			attaque_joueur = attaque_joueur + hydro1;
			std::cout<<" "<<std::endl;
			Sleep(500);
			clearConsole(consS);
			Sleep(500);



			if (choix==1) {
				affichage_choix(pokemon_joueur);
				std::cout<<" utilise Vive-Attaque"<<std::endl;
				vive_attaque(attaque_joueur, pv_ordi, pokemon_ordi);
				hydro1=0;
				
			}
			if (choix==2) {
				affichage_choix(pokemon_joueur);
				std::cout<<" utilise Vibraqua"<<std::endl;
				vibraqua(attaque_joueur, pv_ordi, pokemon_ordi, pokemon_joueur);
				hydro1=0;
				
			}
			if (choix==3) {
				affichage_choix(pokemon_joueur);
				std::cout<<" utilise Hydroqueue"<<std::endl;
				hydroqueue(attaque_joueur, pv_ordi, pokemon_ordi);
				hydro1=hydro1+20;
				
			}
		}


		if (pokemon_joueur==3) {
			choix_atk_feu(choix);
			std::cout<<" "<<std::endl;
			Sleep(500);
			clearConsole(consS);
			Sleep(500);

			if (choix==1) {
				affichage_choix(pokemon_joueur);
				std::cout<<" utilise Vive-Attaque"<<std::endl;
				vive_attaque(attaque_joueur, pv_ordi, pokemon_ordi);
			
			}
			if (choix==2) {
				affichage_choix(pokemon_joueur);
				std::cout<<" utilise Flammeche"<<std::endl;
				flammeche(attaque_joueur, pv_ordi, pokemon_ordi, pokemon_joueur);
			
			}   
			if (choix==3) {
				affichage_choix(pokemon_joueur);
				std::cout<<" utilise Déflagration"<<std::endl;
				deflagration(attaque_joueur, pv_ordi, pv_joueur, pokemon_ordi, pokemon_joueur);
				
			}
		}


		if (pv_ordi<=0) {
			clearConsole(consS);

			affichage_choix(pokemon_ordi);
			std::cout<<" est KO !"<<std::endl;
			std::cout<<" |==  VOUS AVEZ GAGNÉ !  ==|"<<std::endl;
			return 0;
		}
		Sleep(1000);



		if (pokemon_ordi==1) {
			choix = rand() % 4;
			std::cout<<" "<<std::endl;

			if (choix==1) {
				affichage_choix(pokemon_ordi);
				std::cout<<" (IA) utilise Vive-Attaque"<<std::endl;
				vive_attaque(attaque_ordi, pv_joueur, pokemon_joueur);

			}
			if (choix==2) {
				affichage_choix(pokemon_ordi);
				std::cout<<" (IA) utilise Vampigraine"<<std::endl;
				vampigraine(attaque_ordi, pv_joueur, pv_ordi, pokemon_joueur, pokemon_ordi);
			
			}
			if (choix==3) {
				affichage_choix(pokemon_ordi);
				std::cout<<" (IA) utilise Tranch'Herb"<<std::endl;
				trancherb(attaque_ordi, pv_joueur, pokemon_joueur, pokemon_ordi);
			

			}
		}

		if (pokemon_ordi==2) {
			choix = rand() % 4;
			attaque_ordi = attaque_ordi + hydro2;
			std::cout<<" "<<std::endl;

			if (choix==1) {
				affichage_choix(pokemon_ordi);
				std::cout<<" (IA) utilise Vive-Attaque"<<std::endl;
				vive_attaque(attaque_ordi, pv_joueur, pokemon_joueur);
				hydro2=0;
				
			}
			if (choix==2) {
				affichage_choix(pokemon_ordi);
				std::cout<<" (IA) utilise Vibraqua"<<std::endl;
				vibraqua(attaque_ordi, pv_joueur, pokemon_joueur, pokemon_ordi);
				hydro2=0;
				
			}
			if (choix==3) {
				affichage_choix(pokemon_ordi);
				std::cout<<" (IA) utilise Hydroqueue"<<std::endl;
				hydroqueue(attaque_ordi, pv_joueur, pokemon_joueur);
				hydro2=hydro2+20;
				
			}
		}



		if (pokemon_ordi==3) {
			choix = rand() % 4;
			std::cout<<" "<<std::endl;

			if (choix==1) {
				affichage_choix(pokemon_ordi);
				std::cout<<" (IA) utilise Vive-Attaque"<<std::endl;
				vive_attaque(attaque_ordi, pv_joueur, pokemon_joueur);
				
			
			}
			if (choix==2) {
				affichage_choix(pokemon_ordi);
				std::cout<<" (IA) utilise Flammeche"<<std::endl;
				flammeche(attaque_ordi, pv_joueur, pokemon_joueur, pokemon_ordi);
				
			
			}   
			if (choix==3) {
				affichage_choix(pokemon_ordi);
				std::cout<<" (IA) utilise Deflagration"<<std::endl;
				deflagration(attaque_ordi, pv_joueur, pv_ordi, pokemon_joueur, pokemon_ordi);
				
				
			}
		}
		
		Sleep(1000);


	} //Fin de while

	Sleep(500);
	clearConsole(consS);
	Sleep(500);
	std::cout<<" "<<std::endl;


	if (pv_ordi<=0) {
		affichage_choix(pokemon_ordi);
		std::cout<<" est KO !"<<std::endl;
		std::cout<<" |==  VOUS AVEZ GAGNÉ !  ==|"<<std::endl;
	}
	else {
		affichage_choix(pokemon_joueur);
		std::cout<<" est KO !"<<std::endl;
		std::cout<<" |==  VOUS AVEZ PERDU...  ==|"<<std::endl;
	}


	return 0;
}