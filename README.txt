REPONSES POUR DES QUESTIONS TP1

ex5: Modifier votre fonction pour qu’elle initialise le buffer avec des ’ \0 ’ avant de le
renvoyer. Pourquoi faire cette opération ?

> Si on ne initialise le buffer avec des '\0' avant de le renvoyer on peut avoir risque d'avoir certaines
  characteres inconnues si on le ré-utilise après mini_free appel
  
