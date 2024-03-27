if [ $# -gt 1 ]
then
	echo "Nombre d'arguments invalides."
	echo "Syntaxe:"
	echo "	$0				: Demande le nom d'un cours et créée un dossier correspondant"
	echo "	$0 \"<Nom du cours>\"		: Créée un cours portant le nom donné en arguments"
	exit 1
fi

if [ $# -eq 0 ]
then
	echo -n "Nom du cours a créer: "
	read nomCours
else
	nomCours=$1
fi

mkdir "$nomCours"
mkdir "$nomCours/TP"
mkdir "$nomCours/TD"
mkdir "$nomCours/CM"
exit 0