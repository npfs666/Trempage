# Trempage automatisé pour malterie


* [Introduction](#introduction)
* [Tableau](#tableau)
* [Carte électronique](#carte-électronique)

## Introduction

### Schéma général

![Cuves produit](/images/sch_general.jpg)

Le système de trempage permet de faire des cycles alternés de remplissage et de vidange, avec des temporisations différentes.
 
 ![Cuves produit](/images/diagramme_trempage.jpg)

### Fonctionnement

- Choix du nombre de cycles à faire.
- Choix des durées de remplissage et vidange.
- Détection et arrêt du remplissage quand la cuve est pleine.
- Cycles automatique ou manuel (contrôle manuel possible à tout moment).
- Système de reprise automatique, le cycle actuel reprends après une coupure de courant.
- Possibilité à tout moment de changer le nombre de cycles et leurs durée.

### Données techniques 

La carte peut commander tout type de vannes motorisées / électrovannes, et de n’importe quelle tension.
La cuve de trempage peut être alimentée par le bas ou par un système de douche selon les besoins.
L’utilisation des électrovannes est déconseillé sur l’arrivée d’eau pour éviter les coups de béliers lors de la detection du trop plein, de plus elle ont un diamètre réduit qui réduit le débit d’eau et elles chauffent beaucoup lorscequ’elle sont allumées.
Et sur la sortie il ne faut pas utiliser d’électrovanne.


## Tableau

### Photo d’un tableau avec commande manuelle ou programée

![Cuves produit](/images/tableau.jpg)

### Schéma électrique

Schéma d’un tableau avec une électrovanne 230VAC en entrée et une vanne motorisée 12VDC en sortie.
![Cuves produit](/images/tableau.jpg)

### Notes

Le module de trop plein utlisé est le crouzet 84870200, c’est un module standard industriel disponible autour de 80€.
Le système peut être étendu à plusieurs cuves de trempage en rajoutant un module de trop plein par cuve supplémentaire.


## Carte électronique

### Photo d'un module en cours de régulation
![Cuves produit](/images/tableau.jpg)

### Schéma

photo schéma et lien vers fichiers eagle et gerber

### PCB

photo PCB et lien vers gerber file et eagle

### Code

le code sera uploadé sur github


