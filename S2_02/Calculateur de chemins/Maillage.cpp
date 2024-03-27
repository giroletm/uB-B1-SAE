#include "Maillage.h"

#include <iostream>
#include <limits>
#include <vector>
#include <float.h>
#include <math.h>


/****************/
/*** Maillage ***/
/* Constructeur */
/****************/

/// Construit un objet Maillage à partir d'une map et d'un nombre de mailles par unité
Maillage::Maillage(Map * map, float nbMailleUnite) {
	// Initialisation des valeurs par défaut
	nbNoeuds = 0;
	noeuds = NULL;
	nbArcs = 0;
	arcs = NULL;

	this->nbMailleUnite = nbMailleUnite;

	// Vérification de la validité de la map passée
	if(map->Invalide())
		return;

	// Calcul des bordures de la map
	float minX = std::numeric_limits<float>::infinity();
	float maxX = -std::numeric_limits<float>::infinity();
	float minY = std::numeric_limits<float>::infinity();
	float maxY = -std::numeric_limits<float>::infinity();
	
	for(uint i = 0; i < map->nbPolygones; i++) {
		for(uint j = 0; j < map->polygones[i]->nbPoints; j++) {
			Point2D * p = map->polygones[i]->points[j]; 
			if(p->x < minX)
			{
				minX = p->x;
			}
			if(p->y < minY)
			{
				minY = p->y;
			}
			if(p->x > maxX)
			{
				maxX = p->x;
			}
			if(p->y > maxY)
			{
				maxY = p->y;
			}
		}
	}

	float hauteur = maxY - minY;
	float largeur = maxX - minX;

	// Nombre d'unités par maille	
	float nbUniteMaille = 1.0f/nbMailleUnite;

	// Calcul du nombre de mailles (= nombre de noeuds)
	int nbMailleX = (int)ceil(largeur / nbUniteMaille);
	int nbMailleY = (int)ceil(hauteur / nbUniteMaille);
	nbNoeuds = nbMailleX * nbMailleY;

	// Allocation du tableau de noeuds
	noeuds = new Noeud[nbNoeuds];

	// Calcul des valeurs de chaque noeud
	for(int x = 0; x < nbMailleX; x++) {
		for(int y = 0; y < nbMailleY; y++) {
			int indiceMaillage = (y * nbMailleX) + x;

			noeuds[indiceMaillage].x = minX + (nbUniteMaille * x);
			noeuds[indiceMaillage].y = minY + (nbUniteMaille * y);
			noeuds[indiceMaillage].biome = map->BiomeDePoint(noeuds[indiceMaillage].x, noeuds[indiceMaillage].y);
		}
	}

	// Estimation à la hausse du nombre d'arcs. À la hausse car les arcs inexistants dû à des noeuds obstacles ne sont pas pris en compte
	nbArcs = 0;

	if(nbMailleX == 1 || nbMailleY == 1) {
		nbArcs = (nbNoeuds - 1) * 2;
	}
	else {
		int nbInt = (nbMailleX-2) * (nbMailleY-2);
		int nbCoins = 4;
		int nbBord = ((nbMailleX * 2) + (nbMailleY * 2)) - nbCoins;

		nbArcs = (nbInt * 4) + (nbBord * 3) + (nbCoins * 2);
	}
	
	// Allocation et calcul des arcs
	arcs = new Arc[nbArcs];
	int arcActuel = 0;
	for(int y = 0; y < nbMailleY; y++) {
		for(int x = 0; x < nbMailleX; x++) {
			int indiceMaillage = (y * nbMailleX) + x;

			// Selon si on est sur un bord ou non, on exclut ou pas certaines directions d'arcs
			bool xPlus1 = (x < (nbMailleX-1));
			bool xMoins1 = (x > 0);
			bool yPlus1 = (y < (nbMailleY-1));
			bool yMoins1 = (y > 0);

			// On mets ces booléens dans un tableau pour les traiter avec un for pour éviter la redondance
			bool directions[4] = { xPlus1, xMoins1, yPlus1, yMoins1 };
			for(int i = 0; i < 4; i++) {
				if(!directions[i])
					continue;

				// Choix des noeuds de l'arc
				arcs[arcActuel].n1 = &noeuds[indiceMaillage];
				switch(i) {
					case 0: // xPlus1
						arcs[arcActuel].n2 = &noeuds[(y * nbMailleX) + (x+1)];
						break;
					case 1: // xMoins1
						arcs[arcActuel].n2 = &noeuds[(y * nbMailleX) + (x-1)];
						break;
					case 2: // yPlus1
						arcs[arcActuel].n2 = &noeuds[((y+1) * nbMailleX) + x];
						break;
					case 3: // yMoins1
						arcs[arcActuel].n2 = &noeuds[((y-1) * nbMailleX) + x];
						break;
					default:
						break;
				}

				// On vérifie le poids entre les deux noeuds: si celui-ci est négatif, c'est que l'un des noeuds est un obstacle, auquel cas on exclut l'arc
				arcs[arcActuel].poids = CalculPoids(arcs[arcActuel].n1, arcs[arcActuel].n2);
				if(arcs[arcActuel].poids < 0) 
					nbArcs--; // Décrémentation du nombre total d'arcs. Cela gâche donc de l'espace dans le tableau d'arcs, mais faire le calcul des poids également au moment du calcul du nombre d'arcs aurait été gourmand pour pas grand chose. On peut se passer de quelques octets.
				else
					arcActuel++; // Incrémentation de l'arc dans lequel écrire
			}	
		}
	}
}


/***************/
/** Maillage ***/
/* Destructeur */
/***************/

/// Détruit un objet Maillage
Maillage::~Maillage() {
	delete noeuds;
	delete arcs;
}



/************/
/* Maillage */
/* Invalide */
/************/

/// Renvoie true si le maillage calculé est invalide
bool Maillage::Invalide() {
	return this->nbNoeuds == 0 || this->nbArcs == 0 || this->noeuds == NULL || this->arcs == NULL;
}


/***************/
/** Maillage ***/
/* CreerChemin */
/***************/

/// Retourne un chemin de noeuds à partir d'un numéro de noeud d'arrivé et un tableau de prédécesseurs
Noeud** Maillage::CreerChemin(int numeroNoeudArrivee, int* pred) {
	// Calcul du nombre d'éléments du chemin
	int nchemins=1;
	int predarrive = pred[numeroNoeudArrivee]; 
	
	while(predarrive>-1){
		predarrive = pred[predarrive];
		nchemins++;
	}

	// Allocation du tableau de noeuds du chemin selon le nombre trouvé (+1 élément pour signaler la fin du tableau)
	Noeud** chemins = new Noeud*[nchemins+1]; 
	chemins[nchemins] = NULL;

	// Copie les noeuds concernés à l'aide du tableau de prédécesseurs dans le chemin à retourner
	int nbNoeudActuel = numeroNoeudArrivee;
	while(nbNoeudActuel > -1) {
		chemins[nchemins-1] = &noeuds[nbNoeudActuel];
		nchemins--;
		nbNoeudActuel = pred[nbNoeudActuel];
	}

	// Retour du chemin calculé
	return chemins;
}


/***************/
/** Maillage ***/
/* CalculPoids */
/***************/

/// Retourne le poids entre deux noeuds
float Maillage::CalculPoids(Noeud * n1, Noeud * n2) {
	// Si l'un des noeuds est un obstacle, on renvoie un poids négatif
	if(n1->biome->poidsEntrant < 0 || n1->biome->poidsInterieur < 0 || n2->biome->poidsEntrant < 0 || n2->biome->poidsInterieur < 0)
		return -1.0f;

	// Calcul de la distance euclidienne entre les deux points
	float dist = CalculDistEuclidienne(n1, n2);

	// Si on est dans le même biome
	if(n1->biome == n2->biome){
		return n1->biome->poidsInterieur * dist;
	}
	// Si on change de biome
	else{
		return (((n1->biome->poidsInterieur + n2->biome->poidsInterieur) * dist) / 2.0f) + n2->biome->poidsEntrant;
	}
}


/*************************/
/******* Maillage ********/
/* CalculDistHeuristique */
/*************************/

/// Retourne une distance heuristique entre deux noeuds (calculée par distance de Manhattan à partir des poids entre chaque noeuds entre ceux sélectionnés)
float Maillage::CalculDistHeuristique(Noeud* n1, Noeud* n2) {
	// Nombre d'unités par maille
	float nbUniteMaille = 1.0f / this->nbMailleUnite;

	// Stockage du noeud de départ et initialisation des coordonnées actuelles et de la distance totale à retourner
	Noeud* currentNode = n1;
	float currX = n1->x;
	float currY = n1->y;
	float totalDist = 0.0f;

	// Calcul de la distance euclidienne entre les deux noeuds, puis normalisation de celle-ci et multiplication par le nombre d'unités entre chaque maille
	float dist = CalculDistEuclidienne(n1, n2);
	float padX = ((n2->x - n1->x) / dist) * nbUniteMaille;
	float padY = ((n2->y - n1->y) / dist) * nbUniteMaille;

	// Tant qu'on n'a pas atteint le noeud d'arrivée, on avance en direction de celui-ci en ajoutant les poids reliant aux noeuds alentours de notre chemin
	while(currentNode != n2) {
		// Obtention du prochain noeud
		Noeud* nextNoeud = currentNode;
		while(nextNoeud == currentNode) {
			currX += padX;
			currY += padY;
			nextNoeud = &this->noeuds[this->ObtenirIndexNoeudProche({currX, currY})];
		}

		// Calcul de son poids, si positif alors ajout
		float poids = CalculPoids(currentNode, nextNoeud);
		if(poids > 0.0f)
			totalDist += poids;

		// Passage au noeud suivant
		currentNode = nextNoeud;
	}

	// Renvoi de la distance totale
	return totalDist;
}


/*************************/
/******* Maillage ********/
/* CalculDistEuclidienne */
/*************************/

/// Retourne la distance euclidienne entre deux noeuds
float Maillage::CalculDistEuclidienne(Noeud * n1, Noeud * n2) {
	float dX = (n1->x - n2->x);
	float dY = (n1->y - n2->y);
	return sqrt(dX*dX + dY*dY);
}


/***************************/
/******** Maillage *********/
/* ObtenirIndexNoeudProche */
/***************************/

/// Retourne l'index du noeud ayant les coordonnées les plus proches de celles du point donné en paramètre
int Maillage::ObtenirIndexNoeudProche(Point2D pt) {
	// Nombre d'unités par maille divisé par deux
	float nbUM2 = (1.0f / this->nbMailleUnite) / 2.0f;

	// Retour de l'index du noeud étant à une distance inférieure ou égale à moins de la moitié du nombre d'unités par maille divisé par deux des coordonnées du point passé en paramètre
	for(int i = 0; i < nbNoeuds; i++)
		if(pt.x >= this->noeuds[i].x-nbUM2 && pt.x <= this->noeuds[i].x+nbUM2
			&& pt.y >= this->noeuds[i].y-nbUM2 && pt.y <= this->noeuds[i].y+nbUM2)
			return i;

	// Si aucun trouvé, renvoi de -1
	return -1;
}


/*******************/
/**** Maillage *****/
/* CalculerCheminD */
/*******************/

/// Renvoie le plus court chemin entre deux noeuds sous la forme d'un tableau de noeuds à l'aide de l'algorithme de Dijkstra
Noeud** Maillage::CalculerCheminD(int numeroNoeudDepart, int numeroNoeudArrivee) {
	// Allocation des tableaux des distances, des marques et des prédécesseurs
	float* tabDistances = new float[nbNoeuds];
	bool* marque = new bool[nbNoeuds];
	int* pred = new int[nbNoeuds]; 

	// Initialisation des trois tableaux à leur valeur par défaut
	for(int i = 0; i<nbNoeuds; i++){
		tabDistances[i] = std::numeric_limits<float>::infinity();
		marque[i] = false;
		pred[i] = -1;
	}

	// Distance du noeud de départ à 0
	tabDistances[numeroNoeudDepart] = 0;

	// On continue le traitement tant qu'il reste des noeuds à traiter
	while(true) {
		// Détermination du noeud à traiter: celui avec la plus petite distance
		int numATraiter = -1;
		float numMin = std::numeric_limits<float>::infinity();
		for(int i = 0; i < nbNoeuds; i++){
			if(marque[i] == false){
				if (tabDistances[i] < numMin){
					numMin = tabDistances[i];
					numATraiter = i;
				}
			}
		}

		// Si tout est traité, on casse la boucle
		if(numATraiter < 0){
			break;
		}

		// Itération à travers chaque arc partant du noeud en cours de traitement
		for(int i = 0; i < nbArcs; i++){
			if(arcs[i].n1 == &noeuds[numATraiter]) {
				// Noeud voisin du noeud en cours de traitement
				Noeud* na = arcs[i].n2;

				// Détermination du numéro du noeud voisin
				int j;
				for(j = 0; j < nbNoeuds; j++){
					if(na == &noeuds[j]){
						break;
					}
				}
				// Si celui-ci n'est pas marqué, calcul de la nouvelle distance
				if(marque[j] == false){
					float dist = tabDistances[numATraiter];
					float distna = tabDistances[j];
					float dist_pot_na = dist + arcs[i].poids;

					// Si la nouvelle distance est meilleure que la première, alors on la conserver et on met à jour le prédécesseur du noeud voisin
					if(dist_pot_na < distna){
						tabDistances[j] = dist_pot_na;
						pred[j] = numATraiter;
					}
				}
			}
		}

		// On marque comme traité le noeud actuel
		marque[numATraiter] = true;
	}

	// Création d'un chemin à partir du tableau de prédécesseurs
	Noeud** chemins = CreerChemin(numeroNoeudArrivee, pred);

	// Destruction des différents tableaux utilisés
	delete[] tabDistances;
	delete[] marque;
	delete[] pred;

	// Renvoi du chemin calculé
	return chemins;
}


/*******************/
/**** Maillage *****/
/* CalculerCheminA */
/*******************/
// Cette implémentation est basée sur celle de https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h
// Elle a cependant été beaucoup modifiée et adaptée: basée ne veut pas dire copiée-collée.

/// Renvoie un chemin entre deux noeuds sous la forme d'un tableau de noeuds à l'aide de l'algorithme A*
Noeud** Maillage::CalculerCheminA(int numeroNoeudDepart, int numeroNoeudArrivee) {
	// La destination est-elle un obstacle ?
	if (this->noeuds[numeroNoeudDepart].biome->poidsEntrant < 0.0f || this->noeuds[numeroNoeudDepart].biome->poidsInterieur < 0.0f) {
		Noeud** chemins = new Noeud*[1]; 
		chemins[0] = NULL;
		return chemins;
	}

	// La destination est-elle égale au départ ?
	if (numeroNoeudDepart == numeroNoeudArrivee) {
		Noeud** chemins = new Noeud*[2]; 
		chemins[0] = &this->noeuds[numeroNoeudDepart];
		chemins[1] = NULL;
		return chemins;
	}

	// Initialisation des tableaux: coûts, marques & prédecesseurs
	float *gCosts = new float[this->nbNoeuds];
	float *hCosts = new float[this->nbNoeuds];
	float *fCosts = new float[this->nbNoeuds];
	bool *marque = new bool[this->nbNoeuds];
	int *pred = new int[this->nbNoeuds];

	// Initialisation des valeurs par défaut
	for(int i = 0; i < this->nbNoeuds; i++) {
		fCosts[i] = FLT_MAX;
		gCosts[i] = FLT_MAX;
		hCosts[i] = FLT_MAX;
		pred[i] = -1;
		marque[i] = false;
	}

	// Initialisation des valeurs pour le noeud de départ
	int currNum = numeroNoeudDepart;
	Noeud* currNode = &this->noeuds[numeroNoeudDepart];
	fCosts[numeroNoeudDepart] = 0.0f;
	gCosts[numeroNoeudDepart] = 0.0f;
	hCosts[numeroNoeudDepart] = 0.0f;

	// Création de la liste des noeuds à traiter (noeuds ouverts)
	std::vector<int> aTraiter;  
	aTraiter.emplace_back(currNum);

	bool destinationFound = false;

	// Tant que la destination n'a pas été trouvée, qu'il y a des noeuds à traiter et qu'il n'y a pas plus de noeuds à traiter de que noeuds (failsafe), on traite
	while (!destinationFound && !aTraiter.empty() && aTraiter.size() < this->nbNoeuds) {
		// Liste des noeuds prioritaires. Cette liste doit être réduite à un élément à traiter
		std::vector<std::vector<int>::iterator> lowestNodes;

		// Ajout à la liste les noeuds dont le fCost est le plus petit (en général un noeud, mais il se peut que plusieurs noeuds aient le même fCost)
		float temp = FLT_MAX;
		for (std::vector<int>::iterator it = aTraiter.begin(); it != aTraiter.end(); it = next(it)) {
			int n = *it;
			if (fCosts[n] <= temp) {
				if (fCosts[n] < temp) {
					lowestNodes.clear();
					temp = fCosts[n];
				}

				lowestNodes.push_back(it);
			}
		}

		// Si il y a plus d'un noeud avec le fCost minimal, on prends le premier ayant le hCost le plus petit
		if(lowestNodes.size() > 1) {
			std::vector<int>::iterator hIt;

			temp = FLT_MAX;
			for (std::vector<int>::iterator it = aTraiter.begin(); it != aTraiter.end(); it = next(it)) {
				int n = *it;
				if (hCosts[n] < temp) {
					temp = hCosts[n];
					hIt = it;
				}
			}

			lowestNodes.clear();
			lowestNodes.push_back(hIt);
		}

		// Il ne reste qu'un élément à la liste des noeuds prioritaires, récupération et supression de la liste de noeuds à traiter
		currNum = *lowestNodes[0];
		currNode = &this->noeuds[currNum];
		aTraiter.erase(lowestNodes[0]);

		// Marquage du noeud en cours de traitement
		marque[currNum] = true;

		// Pour chaque voisin parant du noeud en cours de traitement
		for(int i = 0; i < this->nbArcs; i++) {
			if(&this->noeuds[currNum] == this->arcs[i].n1) {
				// Obtention du voisin
				Noeud* ndF = this->arcs[i].n2;

				// Si le voisin n'est pas dans un obstacle (failsafe)
				if (ndF->biome->poidsEntrant > 0.0f && ndF->biome->poidsInterieur > 0.0f) {
					// Récupération du numéro de noeud du voisin
					int ndFNum = -1;
					for(ndFNum = 0; ndFNum < this->nbNoeuds; ndFNum++)
						if(ndF == &this->noeuds[ndFNum])
							break;

					// Si le noeud voisin est l'arrivée, alors on a trouvé notre destination
					if (numeroNoeudArrivee == ndFNum)
					{
						pred[ndFNum] = currNum;
						destinationFound = true;
						break;
					}
					// Sinon, si le noeud voisin n'est pas marqué, on le recalcule
					else if (marque[ndFNum] == false)
					{
						// Calcul des nouveaux coûts
						double gNew = gCosts[currNum] + this->arcs[i].poids;
						double hNew = CalculDistHeuristique(ndF, &this->noeuds[numeroNoeudArrivee]);
						double fNew = gNew + hNew;

						// Si le nouveau fCost est meilleur que le précédent (ou s'il n'y a pas de précédent), on met à jour le voisin et on l'ajoute à la liste de noeuds à traiter
						if (fCosts[ndFNum] == FLT_MAX ||
							fCosts[ndFNum] > fNew)
						{
							// Mise à jour des coûts et du prédécesseur du voisin
							fCosts[ndFNum]  = fNew;
							gCosts[ndFNum]  = gNew;
							hCosts[ndFNum]  = hNew;
							pred[ndFNum] = currNum;

							// Ajout du voisin à la liste de noeuds à traiter
							aTraiter.emplace_back(ndFNum);
						}
					}
				}
			}
		}
	}

	// Si aucune destination n'a été trouvée, on retourne (cela arrive si la destination est encerclée d'obstacles)
	Noeud** chemins;
	if (!destinationFound) {
		chemins = new Noeud*[1]; 
		chemins[0] = NULL;
	}
	// Sinon, création d'un chemin à partir du tableau de prédécesseurs
	else 
		chemins = CreerChemin(numeroNoeudArrivee, pred);
	
	// Destruction des différents tableaux utilisés
	delete[] gCosts;
	delete[] hCosts;
	delete[] fCosts;
	delete[] marque;
	delete[] pred;

	// Renvoi du chemin calculé
	return chemins;
}
