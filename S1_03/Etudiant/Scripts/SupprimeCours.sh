if [ $# -gt 1 ]
then
	echo "Nombre d'arguments invalides."
	echo "Syntaxe:"
	echo "	$0				: Demande le nom d'un cours et supprime un dossier correspondant"
	echo "	$0 \"<Nom du cours>\"		: Supprime un cours portant le nom donné en arguments"
	exit 1
fi

if [ $# -eq 0 ]
then
	echo -n "Nom du cours a supprimer: "
	read nomCours
else
	nomCours=$1
fi

if [ ! -d "$nomCours" ]
then
	echo "Il n'existe aucun cours de ce nom"
	exit 1
fi

rm -r "$nomCours"
exit 0
