if [ $# -gt 1 ]
then
	echo "Nombre d'arguments invalides."
	echo "Syntaxe:"
	echo "	$0				: Demande la consigne a mettre et l'écrit dans le bashrc"
	echo "	$0 \"<consigne>\"		: Écrit dans le bashrc la consigne donnée"
	exit 1
fi

if [ $# -eq 0 ]
then
	echo -n "Consigne à écrire: "
	read consigne
else
	consigne=$1
fi

echo "echo $consigne" > ~/.consigne

exit 0