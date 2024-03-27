@echo off
chcp 65001 >nul

SET matiere=
SET /p matiere=Nom de l'enseignement: 

mkdir %matiere%
mkdir %matiere%\Leçons
mkdir %matiere%\Exercices