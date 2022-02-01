# Trempage automatisé pour malterie


* [Introduction](#introduction)
* [Tableau](#tableau)
* [Carte électronique](#carte-électronique)

## Introduction

### Schéma général

![general](/images/sch_general.png)

Le système de trempage permet de faire des cycles alternés de remplissage et de vidange, avec des temporisations différentes.
 
 ![trempage](/images/temps_trempage.png)

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

![Tableau](/images/tableau.jpg)

### Schéma électrique

Schéma d’un tableau avec une électrovanne 230VAC en entrée et une vanne motorisée 12VDC en sortie.

![Schéma](/images/sch_tableau.png)

### Notes

Le module de trop plein utlisé est le crouzet 84870200, c’est un module standard industriel disponible autour de 80€.
Le système peut être étendu à plusieurs cuves de trempage en rajoutant un module de trop plein par cuve supplémentaire.


## Carte électronique

### Photo d'un module en cours de régulation

![Carte](/images/carte.jpg)

### Schéma

![Carte](/images/sch_carte.png)

### PCB V1.2

[Fichiers Eagle 7.7.0 avec librairies inclues](/eagleV1.2.rar)

[Fichiers Gerber](/gerber.rar)

### Code

Le code source est disponible sur ce repo, c'est compilé avec le compilateur xc16 de chez microchip
 

