
/**
  - Constuire le typedef struct en ram à partir des données EEPROM (adressT, adressIO, params)
 *      - Utilisé en reset, watchdog etc...
 * - Include une boucle de MAJ des objets au boot (reprendre l'état précédent)
 */
#define NB_MAX_CYCLES 32

typedef struct {

    uint8_t cyclesMax;    // nombre de cycles totaux (remplissage + vidage)
    uint8_t cycleActuel;  // Numéro du cycle en cours
    uint16_t cycleMinutesIndex;// Compteur de minutes (reset à chaque cycle)
    uint16_t etat;         // 0=arrêté,1=en cours,2=fin trempe,3=manuel,4=pause
    // stockage des temps de chaque cycle (pair remplissage, impair vidage)
    uint8_t cycles[NB_MAX_CYCLES];
    uint16_t cycleMinutes[NB_MAX_CYCLES];
} Trempage;


    
void afficherAccueil();
void regulation();
void retrieveEEPROMdata();
void setCyclesMax(uint8_t cyclesMax);
void setEEPData();
void resetEEPData();

void remplir(uint8_t etat);
void vider(uint8_t etat);
void repriseCycle();

uint8_t getCyclesMax(void);
uint8_t getCycleN(uint8_t n);
void setCycleN(uint8_t n, uint8_t value);
uint8_t getEtat();
void setEtat(uint8_t etat);
uint8_t getCycleActuel();
void setCycleActuel(uint8_t cycleActuel);
uint16_t getCycleMinutes();
void setCycleMinutes(uint16_t cycleMinutes);
void setCycleMinutesIndex(uint16_t index);
uint16_t getCycleMinutesindex();
void setLedEnCours();
void setLedPause();
void setLedFin();