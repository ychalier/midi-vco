# Protocole MIDI

Quelques remarques et notes relatives au protocole MIDI. Sources :

- <https://fr.wikipedia.org/wiki/Musical_Instrument_Digital_Interface#Messages_MIDI>
- <http://multimedia.uqam.ca/audio/docu/MIDI_JDSPG.pdf>

## Début et de fin de note

Les messages *note-on* et *note-off* encodent la note sur 7 bits, en nombre de demi-tons à partir de C-1. Voici quelques notes :

C-1 | A0  | A1  | A2  | A3  | A4  | A5  | A6  | A7  | A8  | A9  | G9
--- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | ---
  0 |   9 |  21 |  33 |  45 |  57 |  69 |  81 |  93 | 105 | 117 | 127

Les valeurs de *vélocité* sont à ignorer car nous ne contrôlons pas le volume.

## Variation de la hauteur

Les messages *pitch-bend* sont accompagnés d'un nombre signé de 14 bits (donc entre -8192 et 8191),
pour indiquer une variation de la hauteur de la note actuellement jouée.
Cette consigne est relative à une plage de variation définie dans le programme, par exemple
entre -2 demi-tons et +2 demi-tons.
