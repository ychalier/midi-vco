# m2a

Programme pour une carte Arduino faisant l'intermédiraire entre un clavier MIDI et un ensemble d'oscillateurs analogiques.

## Contenu

Liste des programmes actuellement implémentés.

Nom | Description
--- | -----------
[`cvgate`](cvgate/cvgate.ino) | Génération des signaux *CV* et *GATE*
[`keyboard`](keyboard/keyboard.ino) | Interface MIDI
[`main`](main/) | Programme principal

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
