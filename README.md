# Arkanoid

##Introduction

Arkanoid est un casse brique développé en binôme dans le cadre du module de programmation en C au CNAM. Le jeu a été développé en C 
et l'interface graphique a été géré à l'aide de la librairie SDL 1.2.

## Instructions
Pour l’affichage et le graphisme on utilisera le code d’exemple et les ressources disponibles sur le site :
http://igg.unistra.fr/people/thery/CNAM_SDA/

Vous utiliserez uniquement la copie de sprite à l’aide de SDL_BlitSurface. 

Les niveaux seront stockés dans des fichiers textes (grille de codes de brique)

Les données de la balle (position, direction, vitesse) seront calculé et stockés en réel pour plus de précision, même si les positions à l’écran sont entières (pixel)

Les objets à l’écran (hormis la balle) étant tous rectangulaires, l’algorithme de détection de collision se fait simplement en comparant x_min, x_max, y_min, y_max avec la position de la balle. Les briques étant alignées régulièrement,

Si on touche quelque-chose de vertical, la coordonnée x de la vitesse s’inverse.
Si on touche quelque-chose d’horizontal, la coordonnée y de la vitesse s’inverse.

Vous prendrez en compte la vitesse (horizontale) du vaisseau pour changer la direction du rebond.

Options: deux joueurs, ombres, traces derrière la balle, disparition progressive des briques.
distribution bonus/malus non aléatoire, etc ...

##Présentation du rendu

[Présentation](https://cfaiformation-my.sharepoint.com/:p:/g/personal/ctreser_cfai-formation_fr/EfpzKUaow_dJrNUPeu-gz20B7u5K_woibFyhfsBVlUBAIQ?e=6vxv49)


