if [ $# -gt 1 ]
then
	echo "Nombre d'arguments invalides."
	echo "Syntaxe:"
	echo "	$0				: Demande une chaîne de caractères à rechercher à travers les noms des fichiers et retourne les trois plus volumineux"
	echo "	$0 \"<Nom du cours>\"		: Recherche une chaîne de caractères à travers les noms des fichiers et retourne les trois plus volumineux"
	exit 1
fi

if [ $# -eq 0 ]
then
	echo -n "Chaîne de caractères à rechercher: "
	read chaineChars
else
	chaineChars=$1
fi


listeTrucs=`ls -l | tail -n+2 | sort -n -k5 -r | tr -s ' ' | cut -d' ' -f9- | grep -F "$chaineChars" | head -3`
if [ -z $listeTrucs ]
then
	nbLignes=0
else
	nbLignes=`echo "$listeTrucs" | wc -l`
fi

if [ $nbLignes -ge 3 ]
then
	echo "Les $nbLignes fichiers les plus volumineux sont:"
elif [ $nbLignes -gt 1 ]
then
	echo "$nbLignes résultats trouvés:"
elif [ $nbLignes -eq 1 ]
then
	echo "$nbLignes résultat trouvé:"
else
	echo "Aucun résultat trouvé"
	exit 0
fi

echo "$listeTrucs" | while IFS= read -r line
do
	echo $line
done

exit 0