# m2a

Programme pour une carte Arduino faisant l'intermédiraire entre un clavier MIDI et un ensemble d'oscillateurs analogiques.

## Contenu

Liste des programmes actuellement implémentés.

Nom | Description
--- | -----------
[`cvgate`](cvgate/cvgate.ino) | Génération des signaux *CV* et *GATE*
[`keyboard`](keyboard/keyboard.ino) | Interface MIDI
[`main`](main/) | Programme principal

## Installation

### Prérequis

1. Télécharger et installer [Arduino IDE](https://www.arduino.cc/en/software/) (Windows, Mac OS, Linux)
2. Installer les bibliothèques externes nécessaires au projet :
    1. Dans Arduino IDE, aller dans *Croquis* > *Inclure une bibliothèque* > *Gérer les bibliothèques*
    2. Installer la bibliothèque **MCP48xx DAC Library** par **Steve Gkountouvas** (version 0.1.2)
    3. Installer la bibliothèque **MIDI Library** par **Francois Best** et **lathoub** (version 5.0.2)
3. Télécharger le contenu de ce dépôt :
    - *(solution légère)* En téléchargeant [le code au format ZIP](https://github.com/ychalier/m2a/archive/refs/heads/main.zip) et en extrayant l'archive obtenue
    - *(solution lourde)* En installant [git](https://git-scm.com/downloads) et en utilisant la commande `git clone https://github.com/ychalier/m2a`

### Téléverser un programme

1. Ouvrir le fichier souhaité dans Arduino IDE (par exemple, `main/main.ino`)
2. Brancher la carte en USB sur l'ordinateur ; afin d'éviter tout problème, il est préférable qu'à ce moment-là la carte ne soit alimentée que via sa connexion USB avec l'ordinateur, et que rien ne soit connecté sur ses ports RX
3. Vérifier que la communication entre Arduino IDE et la carte fonctionne :
    1. Dans Arduino IDE, aller dans *Outils* > *Type de carte* et sélectionner si besoin *Arduino Mega or Mega 2560*
    2. Dans Arduino IDE, aller dans *Outils* > *Port* et sélectionner si besoin le port USB sur lequel la carte est connectée
4. Dans Arduino IDE, cliquer sur le bouton *Téléverser* (icone d'une fléche vers la droite) ; des informations relatives au bon déroulé de l'opération sont affichées en bas de la fenêtre. En cas d'erreur, essayer de rebrancher la carte ou de redémarrer Arduino IDE.

## Feuille de route

- [x] ~~Contrôle des convertisseurs numériques/analogiques~~
- [x] ~~Contrôle des signaux *GATE*~~
- [x] ~~Test de l'interface MIDI~~
- [ ] **Intégrer l'interface MIDI au système de génération de signal**
- [ ] Intégrer les éléments supplémentaires de l'interface utilisateur

## Documentation

Liste des documents de références.

- [Arduino MEGA 2560](https://www.robotshop.com/media/files/pdf/arduinomega2560datasheet.pdf)
- [Convertisseurs numériques/analogiques MCP4822](https://ww1.microchip.com/downloads/en/DeviceDoc/20002249B.pdf)
- [Bibliothèque Arduino *SPI*](https://www.arduino.cc/en/reference/SPI)
- [Bibliothèque Arduino *MCP48xx DAC*](https://www.arduino.cc/reference/en/libraries/mcp48xx-dac-library/)
- [Bibliothèque Arduino *MIDI*](https://github.com/FortySevenEffects/arduino_midi_library) ([documentation](https://fortyseveneffects.github.io/arduino_midi_library/), illustration sur [YouTube](https://www.youtube.com/playlist?list=PL4_gPbvyebyH2xfPXePHtx8gK5zPBrVkg))

Voir [MIDI.md](MIDI.md) pour des informations plus précises sur le protocole MIDI.
