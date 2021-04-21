# Interface polyphonique MIDI-VCO

Micrologiciel pour une carte Arduino faisant l'intermédiraire entre un clavier MIDI et un ensemble d'oscillateurs commandés en tension.

## Installation

### Prérequis

1. Télécharger et installer [Arduino IDE](https://www.arduino.cc/en/software/) (Windows, Mac OS, Linux)
2. Installer les bibliothèques externes nécessaires au projet :
    1. Dans Arduino IDE, aller dans *Croquis* > *Inclure une bibliothèque* > *Gérer les bibliothèques*
    2. Installer la bibliothèque **MCP48xx DAC Library** par **Steve Gkountouvas** (version 0.1.2)
    3. Installer la bibliothèque **MIDI Library** par **Francois Best** et **lathoub** (version 5.0.2)
3. Télécharger le contenu de ce dépôt :
    - *(solution légère)* En téléchargeant [le code au format ZIP](https://github.com/ychalier/m2a/archive/refs/heads/main.zip) et en extrayant l'archive obtenue
    - *(solution lourde)* En installant [git](https://git-scm.com/downloads) et en utilisant la commande `git clone https://github.com/ychalier/midi-vco`

### Téléverser un programme

1. __Ouvrir le fichier *main/main.ino* dans Arduino IDE__
2. __Brancher la carte en USB sur l'ordinateur__ ; afin d'éviter tout problème, il est préférable qu'à ce moment-là la carte ne soit alimentée que via sa connexion USB avec l'ordinateur, et que rien ne soit connecté sur ses ports RX
3. __Vérifier que la communication entre Arduino IDE et la carte fonctionne :__
    1. Dans Arduino IDE, aller dans *Outils* > *Type de carte* et sélectionner si besoin *Arduino Mega or Mega 2560*
    2. Dans Arduino IDE, aller dans *Outils* > *Port* et sélectionner si besoin le port USB sur lequel la carte est connectée
4. __Dans Arduino IDE, cliquer sur le bouton *Téléverser*__ (icone d'une fléche vers la droite) ; des informations relatives au bon déroulé de l'opération sont affichées en bas de la fenêtre. En cas d'erreur, essayer de rebrancher la carte ou de redémarrer Arduino IDE.

## Documentation

Pour en apprendre plus sur le fonctionnement de ce micrologiciel, rendez-vous sur le [wiki](https://github.com/ychalier/midi-vco/wiki).