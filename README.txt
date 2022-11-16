Pour tester, utilisez text.txt pour tester mini_cp

++++++++++++++++++++++++++++++++++MINI_SHELL MANUAL+++++++++++++++++++++++++++++++++++++
+ mini_touch file_name: Creer fichier                                                  +
+ mini_cp source_file des_file: Copier source_file to des_file                         +
+ mini_echo line: Ecrire line to stdout                                                +
+ mini_cat file_name: Ecrire fichier to stdout                                         +
+ mini_head -n <number> file_name: Ecrire number premieres lignes du fichier a stdout  +
+ mini_tail -n <number> line: Ecrire number dernieres lignes du fichier a stdout       +
+ mini_clean file_name: Vider fichier s'il en existe et creer un fichier sinon         +
+ mini_grep pattern file_name: Afficher tous les lignes contenant pattern              +
+ mini_wc file_name: Afficher le nombre de mots dans un fichier                        +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


REPONSES POUR DES QUESTIONS TP1

ex5: Modifier votre fonction pour qu’elle initialise le buffer avec des ’ \0 ’ avant de le
renvoyer. Pourquoi faire cette opération ?

> Si on n'initialise pas le buffer avec des '\0' avant de le renvoyer on peut risquer d'avoir certaines
  characteres inconnues si on le ré-utilise en utilisant mini_calloc après appel mini_free().
  
  Par exemple: Considérons notre buffer n'est pas initialisé avec des '\0'.

  On initialise char* buffer1 = mini_calloc(sizeof(char), 3) 
  buffer1 -> [][][]

  On met des valeurs dans buffer1 par exemple: buffer1[0] = 'a'
                                               buffer1[1] = 'b' 

                                               buffer1 -> [a][b][]
  
  Maintenant, on libère le bloc memoire a buffer1 avec mini_free(buffer1)

  On initialise char* buffer2 = mini_calloc(sizeof(char), 3)
  Parce qu'on utilise malloc_liste pour ré-utiliser le bloc mémoire non-utilisé donc maintenant buffer2 est comme suite
  
  buffer2 -> [a][b][]



ex6: Que fait la fonction free en C ? libère-t’elle vraiment la mémoire ?

> La fonction free ne libère pas vraiment le bloc mémoire. Son action réelle est qu'elle ne permet plus au pointeur qui possède 
  le bloc de mémoire de posséder ce bloc de mémoire. Après appel free(pointeur) le pointeur ne possède plus ce bloc memoire,
  càd il n'est plus autorisé d'écrire et de lire sur ce bloc mémoire



ex12: Proposer une solution (à implémenter dans le main.c pour tester mini_calloc et free 

> On initialise un buffer1 avec trois characteres, on mets des valeurs pour buffer1[0] et buffer1[1], puis on libère 
le bloc mémoire du buffer1 en utilisant mini_free(buffer1). On initialise un buffer2, et on vérifie si buffer2[0] 
et buffer2[1] ont des valeurs qu'on a mis précédemment. Si oui notre mini_calloc n'est pas correcte.



ex17: Tester vos fonctions dans main.c, en particulier, essayer d’afficher une unique chaîne de caractères qui ne 
contient pas de saut de ligne. Quel problème reste-t-il à régler ?

> Le problème est si la longueur de notre chaîne de caractères est plus courte que le BUF_SIZE le programme n'écrit pas
notre chaîne a stdin


ex20: Ajouter un test dans votre main.c pour cette fonction. Que se passe t’il si le nombre de caractères saisis est égal à la taille du buffer ? Proposer et implémenter une
solution

> Si le nombre de caractères saisis est égal à la taille du buffer, '\0' n'est pas ajouté a la fin du chaîne, donc on 
lit taille_buffer - 1 caractères et on met '\0' a buffer[taille_buffer]


ex22: Quels sont les problèmes de ces fonctions (notamment en terme de sécurité) ? Proposer une correction de ces fonctions.

> Avec mini_strcpy on ne peut pas savoir la longueur de la chaîne destinée, donc si la longueur de la chaîne
  source est plus longue que celle de la chaîne destinée. On risque d'avoir "buffer overflow", c'est notamment
  dangereux en terme de sécurité car les hackeurs peut utilise "buffer overflow" pour mettre des codes executable
  a notre machine.

  -> creer mini_strncpy qui prend n comme nombre caractères pour copier 


ex34: Que se passe-t’il si le programme se termine alors que le buffer d’écriture 
n’était pas plein ? Ajouter le code permettant de corriger ce problème dans mini_exit (Pen-
sez a ajouter une liste des fichiers ouverts pour pouvoir tous les flusher)

> Dans mini_fwrite l’écriture ne sera déclenchée que lorsque le buffer_write sera plein donc si le buffer_write n'était pas
plein quand le programme se termine des caractères ne sont pas écrit au fichier.
