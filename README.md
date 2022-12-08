# SNOT

Safe Night Out Together

## Discord

Join het discord kanaal voor alle info en vragen: <https://discord.gg/XazaEC6M>

## Dop Design

Voorlopig hebben we een aantal designs gehad. Het laatste design is de blauwe dop.
Zie discord INFO/Design voor verdere info.
## PCB Design

Voor het elektronisch en PCB schema terug te vinden ga je naar discord: INFO/PCB-Design

## Code

Voor het binnenhalen van de code en overige:
- git clone git@github.com:vives-project-xp/Snot.git

De volledige code is terug te vinden onder de file 'mbed'.
Indien je de schakeling wil testen zonder de NFC maar enkel met drukknoppen moet je de file 'cap' gebruiken

Voor het compileren van de code:
- Open powershell
- Conda create mbed (zie installation guide (mini)conda)
- Conda activate mbed (zie installation guide (mini)conda voor installatie extensies)
- mbed compile -f

## Benodigdheden Software

- Installeren van Visual Studio Code: <https://code.visualstudio.com/> // + Benodigde extenties (zou moeten automatisch aanbieden welke je nodig hebt)
- Installeren van python: <https://www.academicsoftware.eu/software/27/265> // Take the lastest STABLE release
- Installeren van (mini)conda: <https://lab-microcontrollers.netlify.app/lab-1/#goal>
- Installeren van mbed: <https://lab-microcontrollers.netlify.app/lab-1/#mbed>
- Installeren van KiCad: <https://www.kicad.org/download/windows/> // CERN Switzerland
- Installeren van SolidEdge: <https://www.academicsoftware.eu/software/25/292>

Zorg er ook voor dat miniconda is toegevoegd aan je omgevings variabelen: bv. C:\Users\thybr\miniconda3\Scripts

## Benodigdheden Hardware

- Cap + valse bodem + dekseltje motor 
- STM32L467RG voor compilatie
- PN532 (versie blauw) voor scannen van de NFC
- Zelfgemaakte PCB wanneer werken voor STM te vervangen (zou moeten in cap passen)
- PN532 (versie rood) voor het scannen van de NFC (zou moeten in de cap passen)
- Alle bestelde hardware + deze solderen op de zelf gemaakte PCB
