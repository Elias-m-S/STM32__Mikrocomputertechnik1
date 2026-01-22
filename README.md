# Programmentwurf 1 - STM32G474RE & EduShield

Dieser Programmentwurf umfasst die Implementierung einer sequentiellen LED-Steuerung sowie einer dynamischen 7-Segment-Anzeige. Das Projekt wird auf einem Nucleo-Board NUG474RE unter Verwendung des EduShields umgesetzt.

## Hardware-Anforderungen
* **Target Prozessor:** STM32G474RE auf einem Nucleo Board.
* **E/A-Modul:** EduShield.
* **Einschränkung:** Es sind keine weiteren elektronischen Schaltungen zulässig.

## Funktionale Anforderungen

### LED-Lauflicht (EduShield)
* **Sequenz:** Die LEDs D4 bis D0 leuchten sequentiell für jeweils 1 Sekunde auf.
* **Start:** Die Sequenz startet bei LED D2 unmittelbar bei Betätigung des blauen User Buttons B1.
* **Stopp:** Bei Betätigung des Tasters SW1 erlöschen alle LEDs umgehend.
* **Initialzustand:** Beim Hochfahren bleibt die Leuchtsequenz zunächst aus.
* **Performance:** Das Umschalten zwischen den LEDs erfolgt in weniger als 1 ms.

### 7-Segmentanzeige
* **Eingabe:** Der variable Spannungswert des linken Linearpotentiometers (R1) wird mit 8-Bit Auflösung konvertiert.
* **Logik:** Lineare Umrechnung in den Ganzzahlen-Wertebereich 0 bis 9.
* **Darstellung:**
    * Die linke Anzeige (S1) zeigt den konvertierten Wert.
    * Die rechte Anzeige (S2) ergänzt diesen so, dass die Summe beider Werte stets 9 ergibt.
* **Helligkeit & Dimmen:**
    * Das Dimmen erfolgt über ein PWM-Signal, gesteuert durch das rechte Potentiometer (R2).
    * Der Rohwert von R2 wird quadratisch auf das PWM-Signal abgebildet.

## Software-Architektur
* **Sprache:** C (C99) für ARM GNU Toolchain.
* **Modularisierung:** * `edushield.c/.h`: Beinhaltet die LED-Logik.
    * `segment.c/.h`: Beinhaltet die Logik der 7-Segmentanzeige.
* **Framework:** Initialisierung in `RunState_Init()` und zyklischer Aufruf in `RunState_Cyclic_Running`.
* **Konfiguration:** Peripherie-Konfiguration erfolgt über
