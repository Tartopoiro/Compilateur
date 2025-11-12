A partir du code décrit esentiellement dans le .y, ./Makefile produit
un executable ./lang qui compile stdin vers stdout. Une façon simple
de compiler un fichier source vers un fichier cible consiste alors à
rediriger les entrées et sorties.


le script bash runComp fait celà. Pour compiler un code source
ex.myc
en un code cible
ex_pcode.c
il suffit de lancer
runComp ex


