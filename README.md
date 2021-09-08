# Interface polyphonique MIDI-VCO

Micrologiciel pour une carte Arduino faisant l'intermédiraire entre un clavier MIDI et un ensemble d'oscillateurs commandés en tension.

## Installation

### Prérequis

1. Télécharger et installer [Arduino IDE](https://www.arduino.cc/en/software/)
2. Installer les bibliothèques externes nécessaires au projet :
    1. Dans Arduino IDE, aller dans *Croquis* > *Inclure une bibliothèque* > *Gérer les bibliothèques*
    2. Installer la bibliothèque **MCP48xx DAC Library** par **Steve Gkountouvas** (version 0.1.2)
    3. Installer la bibliothèque **MIDI Library** par **Francois Best** et **lathoub** (version 5.0.2)
    4. Installer la bibliothèque **arduinoFFT** par **Enrique Condes** (version 1.5.6)
    5. Installer la bibliothèque **ArduMax MCP41xxx Driver** par **Max Chan** (version 1.0.6)
3. Télécharger le contenu de ce dépôt :
    1. Installer [git](https://git-scm.com/downloads)
    2. Ouvrir l'interpréteur de commandes du système d'exploitation (le [terminal](https://ubuntu.com/tutorials/command-line-for-beginners#3-opening-a-terminal) sur Linux ou MacOS, [Powershell](https://docs.microsoft.com/fr-fr/powershell/scripting/windows-powershell/starting-windows-powershell?view=powershell-7.1) sur Windows)
    3. (*optionnel*) Se déplacer dans le dossier souhaité à l'aide de la commande [`cd`](https://fr.wikipedia.org/wiki/Cd_(commande))
    4. Cloner le dépôt avec la commande suivante :
       ```
       git clone https://github.com/ychalier/midi-vco.git
       ```
    5. (*optionnel*) Changer pour une autre version du code en sélectionnant une autre *branche* (voir [la liste des branches disponibles](https://github.com/ychalier/midi-vco/branches)) avec la commande suivante :
       ```
       cd midi-vco
       git checkout nom-de-la-branche
       ```

### Téléverser un programme

1. __Ouvrir le fichier *midi-vco/main/main.ino* dans Arduino IDE__
2. __Brancher la carte en USB sur l'ordinateur__ 
3. __Vérifier que la communication entre Arduino IDE et la carte fonctionne :__
    1. Dans Arduino IDE, aller dans *Outils* > *Type de carte* et sélectionner *Arduino Mega or Mega 2560*
    2. Dans Arduino IDE, aller dans *Outils* > *Port* et sélectionner le port USB sur lequel la carte est connectée
4. __Dans Arduino IDE, cliquer sur le bouton *Téléverser*__ (icone d'une flèche vers la droite) ; des informations relatives au bon déroulé de l'opération sont affichées en bas de la fenêtre.

#### En cas d'erreur

Si le message d'erreur affiché indique une `Erreur de compilation`, c'est qu'il y a un problème avec le code. Créez [une nouvelle Issue](https://github.com/ychalier/midi-vco/issues) en partageant la totalité du message d'erreur.

Sinon, il se peut que la communication avec la carte soit fautive. Vérifiez que la carte ne soit alimentée que via sa connexion USB avec l'ordinateur, et que ses ports RX soient déconnectés. Si le problème persiste, débranchez/rebranchez la carte et redémarrez Arduino IDE. Il se peut également qu'il faille démarrer Arduino IDE en mode administrateur (ou *sudo*).

## Documentation

Pour en apprendre plus sur le fonctionnement de ce micrologiciel, rendez-vous sur le [wiki](https://github.com/ychalier/midi-vco/wiki), ou lisez la [documentation](https://ychalier.github.io/midi-vco/).
