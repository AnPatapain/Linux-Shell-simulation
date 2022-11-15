REPONSES POUR DES QUESTIONS TP1

ex5: Modifier votre fonction pour qu’elle initialise le buffer avec des ’ \0 ’ avant de le
renvoyer. Pourquoi faire cette opération ?

> Si on n'initialise pas le buffer avec des '\0' avant de le renvoyer on peut avoir risque d'avoir certaines
  characteres inconnues si on le ré-utilise après mini_free appel.
  
  Par exemple: Considérons notre buffer n'est pas initialisé avec des '\0'.

  On initialise char* buffer1 = mini_calloc(sizeof(char), 3) 
  buffer1 -> [][][]

  On met des valeurs dans buffer1 par exemple: buffer1[0] = 'a'
                                               buffer1[1] = 'b'

                                               buffer1 -> [a][b][]
  
  Maintenant, on libere le bloc memoire a buffer1 avec mini_free(buffer1)

  On initialise char* buffer2 = mini_calloc(sizeof(char), 3)
  Parce qu'on utilise malloc_liste pour ré-utiliser le bloc mémoire non-utilisé donc maintenant buffer2 est comme suite
  
  buffer2 -> [a][b][]



ex6: Que fait la fonction free en C ? libère-t’elle vraiment la mémoire ?

> La fonction free ne libère pas vraiment le bloc mémoire. Son action réelle est qu'elle ne permet plus au pointeur qui possède 
  le bloc de mémoire de posséder ce bloc de mémoire. Après free(pointeur) appel le pointeur ne possède plus ce bloc memoire



ex12: Proposer une solution (à implémenter dans le main.c pour tester mini_calloc et free 

> On initialise un buffer1 avec trois characteres, on mets des valeurs pour buffer1[0] et buffer1[1], puis on libère 
le bloc mémoire du buffer1. On initialise un buffer2, et on vérifie si buffer2[0] et buffer2[1] ont des valeurs qu'on
a mis précédemment a buffer1. Si oui notre mini_calloc n'est pas correcte.



ex17: Tester vos fonctions dans main.c, en particulier, essayer d’afficher une unique chaîne de caractères qui ne 
contient pas de saut de ligne. Quel problème reste-t-il à régler ?

> Le problème est si la longueur de notre chaîne de caractères est plus courte que le BUF_SIZE le programme n'écrit pas
notre chaîne a stdin


ex20: Ajouter un test dans votre main.c pour cette fonction. Que se passe t’il si

> Si le nombre de caractères saisis est égal à la taille du buffer ? Proposer et implémenter une
solution

Si le nombre de caractères saisis est égal à la taille du buffer, '\0' n'est pas ajouté a la fin du chaîne, donc on 
lit taille_buffer - 1 caractères et on met '\0' a buffer[taille_buffer]


ex22: Quels sont les problèmes de ces fonctions (notamment en terme de sécurité) ? Proposer une correction de ces fonctions.

> Je sais pas

ex34: Que se passe-t’il si le programme se termine alors que le buffer d’écriture 
n’était pas plein ? Ajouter le code permettant de corriger ce problème dans mini_exit (Pen-
sez a ajouter une liste des fichiers ouverts pour pouvoir tous les flusher)

> Dans mini_fwrite l’écriture ne sera déclenchée que lorsque le buffer_write sera plein donc si le buffer_write n'était pas
plein quand le programme se termine des caractères ne sont pas écrit au fichier.
