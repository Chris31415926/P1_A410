#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <time.h> 
#include <unistd.h> 
/* #include <errno.h> */ 

/*Her er alle vores konstanter defineret, som bruges igennem programmet, disse værdier er udregnet
og beskrevet i vores rapport.*/

#define BILER_2020 125200 
#define TOTAL_AARLIG_VAEKST 0.027 
#define AARLIG_VAEKST_BRO 0.0117 
#define AARLIG_VAEKST_TUNNEL 0.033 
#define BILER_2020_BRO 34400 
#define BILER_2020_TUNNEL 90800 
#define BILER_2020_BRO_MED_EGHOLM 26900 
#define BILER_2020_TUNNEL_MED_EGHOLM 69600 
#define KAPACITET_BRO 31000 
#define KAPACITET_TUNNEL 80000 
#define KAPACITET_EGHOLM 59000 
 
#define ANTAL_BANER 2 
#define LAENGDE_AF_BRO 105 
 
#define MYLDRETID_VAEGT 0.33 
#define FREE_FLOW_VAEGT 0.02 
#define TIME_PAA_DAG 24 
#define SEK_PAA_TIME 3600 
 
/*Her er en oversigt over vores funktions protyper, som køres igennem inden vi når main.*/

float antal_biler (float vaekst,int antalbiler,float med_egholm);  
float Egholm_linje(int k, int Egholm_trafik); 
void Kapacitet (void);  
void Simulation(void); 
void Bro(long int bro[][LAENGDE_AF_BRO], int Egholmtrafik, int time); 
void search_bro(long int bro[][LAENGDE_AF_BRO]); 
void Fordeling(int fordeling[], int num_af_biler, double vaegt); 
double Unif(int start, int slut); 
 
int main(void){ 
  int num, counter, Egholmtrafik = 28700, year; 
 
  printf("Du har folgende valg muligheder:\n"); 
  printf("1. Trafik pa alle Limfjordsovergange helt til bestemt ar.\n"); 
  printf("2. Trafik pa Limfjordsbroen helt til bestemt ar.\n"); 
  printf("3. Trafik pa Limfjordstunnellen helt til bestemt ar.\n"); 
  printf("4. Trafik pa Egholmslinjen.\n"); 
  printf("5. Tidspunktet hvor overgangeneskapacitet bliver opbrugt.\n"); 
  printf("6. Simulation af broen.\n"); 
  printf("Indtast 0 for at komme ud fra programmet\n"); 
 
  printf("Valg et nummer:"); 
  scanf("%d", &num); 
 
    /*Dette er en while-funktion, der indeholder 6 muligheder, som brugeren kan benytte sig af.
    De første 4 viser tilskrivningen af trafik per år til et bestemt årstal, som bruger vælger
    Nummer 5, viser det årstal hvor kapaciteten af de 3 overgange igen er overskredet
    Nummer 6, er en simulation af trafikken på egholmslinjen. Den kører vha. et for-loop, som kan
    kører til hvilket som helst årstal. man kan kører alle valgmuligheder så længe man vil, da den kun
    stopper hvis man indtaster 0.  */
  while (num != 0){ 
    if (num == 1) 
      antal_biler(TOTAL_AARLIG_VAEKST, BILER_2020, BILER_2020); 
 
    else if (num == 2) 
      antal_biler(AARLIG_VAEKST_BRO, BILER_2020_BRO, BILER_2020_BRO_MED_EGHOLM); 
 
    else if (num == 3) 
      antal_biler(AARLIG_VAEKST_TUNNEL, BILER_2020_TUNNEL, BILER_2020_TUNNEL_MED_EGHOLM); 
 
    else if (num == 4){ 
      printf("Indtast onsket arstal:");  
      scanf("%d",&year);  
      for(counter = 2020; counter<=year; counter++){ 
      Egholmtrafik = Egholm_linje(counter, Egholmtrafik); 
      } 
    } 
 
    else if (num == 5) 
      Kapacitet(); 
     
    else if (num == 6) 
      Simulation(); 
 
    printf("Du har folgende valg muligheder:\n"); 
    printf("1. Trafik pa alle Limfjordsovergange helt til bestemt ar.\n"); 
    printf("2. Trafik pa Limfjordsbroen helt til bestemt ar.\n"); 
    printf("3. Trafik pa Limfjordstunnellen helt til bestemt ar.\n"); 
    printf("4. Trafik pa Egholmslinjen.\n"); 
    printf("5. Tidspunktet hvor overgangeneskapacitet bliver opbrugt.\n"); 
    printf("6. Simulation af broen.\n"); 
    printf("Indtast 0 for at komme ud fra programmet\n"); 
   
    printf("Valg et nummer:"); 
    scanf("%d", &num); 
  } 
  return EXIT_SUCCESS; 
} 
 /*Funktionen antal_biler dækker over valgmulighed 1-3, her udregnes fremskrivningen i trafik per år
 til et bestemt år, den viser antal med og uden egholms trafik.
 Den kører vha. af et for-loop, så længe at i<year, hvor year ikke er assignet, så derfor kan den
 kører til hvilket som helst årstal. Den printer til sidst alle resultater per år. */
float antal_biler (float vaekst,int antalbiler,float med_egholm){  
  int n,i,year,y,Y; 
  float v,V; 
 
  printf("Indtast onsket arstal:"); 
  scanf("%d",&year); 
 
  printf("\tYear\tAntal biler uden Egholm\tAntal biler med Egholm\n"); 
  for (i=2020;i<=year;i++){  
    n = i - 2020; 
 
    v=vaekst*antalbiler*n; 
    V=vaekst*med_egholm*n; 
 
    y=antalbiler+v; 
    Y=med_egholm+V; 
 
    printf("\t%d:\t\t%d\t\t%d\n",i,y,Y); 
  }  
       
  return y;  
}  
 /*Dækker over valgmulighed 4, og udregner fremskrivningen i trafik på egholmslinjen per år.
 Dete gøres vha. simple matematiske udregninger med vores konstanter, som er defineret
 i starten af programmet. til sidst printes årstal og trafiktallet til det år.
 */
float Egholm_linje(int k, int Egholm_trafik){ 
  int n; 
  float vbro, ybro, vtunel, ytunel, ekstravaegt_bro = 0, ekstravaegt_tunnel = 0; 
 
  n = k - 2020; 
 
  vbro = AARLIG_VAEKST_BRO * BILER_2020_BRO_MED_EGHOLM * n; 
  ybro = BILER_2020_BRO_MED_EGHOLM + vbro; 
 
  vtunel = AARLIG_VAEKST_TUNNEL * BILER_2020_TUNNEL_MED_EGHOLM * n; 
  ytunel = BILER_2020_TUNNEL_MED_EGHOLM + vtunel; 
 
  if (ybro > KAPACITET_BRO) 
    ekstravaegt_bro = ybro - KAPACITET_BRO; 
  if (ytunel > KAPACITET_TUNNEL) 
    ekstravaegt_tunnel = ytunel - KAPACITET_TUNNEL; 
 
  Egholm_trafik = ekstravaegt_bro + ekstravaegt_tunnel + 28700; 
 
  printf("Aar : %d \t Trafik tallet: %d\n", k, Egholm_trafik); 
 
  return Egholm_trafik; 
} 
 
 /*Dækker over mulighed 5. Kapaciteten per overgang er defineret som en konstant, da hver overgang kun kan håndtere en hvis
 mængde trafik og vi har derfor kigget på vores udregninger og set i hvilket årstal
 dette er overskredet igen. Dette printes, så for hver overgang, hvor der vises, kapacitet
 årstal det overskrevet, og med hvor mange biler */

void Kapacitet(void){  
  printf("\tForbindelsernes navne\tKapacitet\tArstal\t\tAntal biler i arstallet\n");  
  printf("\tLimfjordsbro\t\t%d\t\t%d\t\t%d\n",KAPACITET_BRO,2034,31306);  
  printf("\tLimfjordstunnel\t\t%d\t\t%d\t\t%d\n",KAPACITET_TUNNEL,2025,81084);  
  printf("\tEgholmlinjen\t\t%d\t\t%d\t\t%d\n",KAPACITET_EGHOLM,2038,61207);  
  return ;  
}  
 /* mangler forklaring */
void Simulation(void){ 
  int i, j, k, time; 
  int Egholmtrafik = 28700; 
  long int bro[ANTAL_BANER][LAENGDE_AF_BRO]; 
   
  printf("Simulationen starter med at køre fra år 2030\n"); 
 
  for (k = 2020; k < 2030; k++){ 
    Egholmtrafik = Egholm_linje(k, Egholmtrafik); 
    printf("\n"); 
  } 
 
  for (k = 2030; Egholmtrafik < KAPACITET_EGHOLM; k++){ 
    for (i = 0; i < ANTAL_BANER; i++){ 
      for (j = 0; j < LAENGDE_AF_BRO; j++){ 
        bro[i][j] = 0; 
 
        printf("%ld", bro[i][j]); 
        if (j == (LAENGDE_AF_BRO - 1)) 
          printf("\n"); 
      } 
    } 
    Egholmtrafik = Egholm_linje(k, Egholmtrafik); 
    printf("\n"); 
 
    sleep(1); 
 
    for (time = 0; time < TIME_PAA_DAG; time++){ 
      Bro(bro, Egholmtrafik/2, time); 
    } 
     
    printf("\e[1;1H\e[2J"); 
  } 
} 
 /* mangler forklaring */
void Bro(long int bro[][LAENGDE_AF_BRO], int Egholmtrafik, int time){ 
  int sek, bane, plads, koe, ny_bil = 0, foerste_plads; 
  int *fordeling; 
  int Fordeling_free_flow[(int)(Egholmtrafik * FREE_FLOW_VAEGT)]; 
  int Fordeling_myldretid[(int)(Egholmtrafik * MYLDRETID_VAEGT)]; 
 
  if (time == 7 || time == 15){ 
    /* fordeling = Fordeling(Egholmtrafik, MYLDRETID_VAEGT); */ 
    Fordeling(Fordeling_myldretid, Egholmtrafik, MYLDRETID_VAEGT); 
    fordeling = Fordeling_myldretid; 
  } 
  else{ 
    /* fordeling = Fordeling(Egholmtrafik, FREE_FLOW_VAEGT); */ 
    Fordeling(Fordeling_free_flow, Egholmtrafik, FREE_FLOW_VAEGT); 
    fordeling = Fordeling_free_flow; 
  } 
 
  for (sek = 0; sek < SEK_PAA_TIME; sek++){ 
    search_bro(bro); 
 
    while (koe != 0){ 
      bane = 0; 
      foerste_plads = 0; 
      while (bane < ANTAL_BANER){ 
        if (bro[bane][0] == 0) 
          foerste_plads++; 
        bane++; 
      } 
 
      if (foerste_plads != 0){ 
        bro[rand() % foerste_plads][0] = 1; 
        koe--; 
      } 
      else{ 
        break; 
      } 
    } 
 
    while (fordeling[ny_bil] == sek){ 
      bane = 0; 
      foerste_plads = 0; 
 
      while (bane < ANTAL_BANER){ 
        if (bro[bane][0] == 0) 
          foerste_plads++; 
        bane++; 
      } 
 
      if (foerste_plads != 0) 
        bro[rand() % foerste_plads][0] = 1; 
      else 
        koe++; 
 
      ny_bil++; 
    } 
 
    /* system("close"); */ 
    printf("\e[1;1H\e[2J"); 
 
    printf("Den %d time.\n", time); 
    for (bane = 0; bane < ANTAL_BANER; bane++){ 
      for (plads = 0; plads < LAENGDE_AF_BRO; plads++){ 
        printf("%ld", bro[bane][plads]); 
      } 
      printf("\n"); 
    } 
    printf("\n\n"); 
    printf("Antal biler i koe: %d", koe); 
    sleep(0.1); 
  } 
 
  /* free(fordeling); */ 
} 
 
 /*Searh_bro funktionen har vores bro array som input og kører så længe 3 for-loops er sande, herefter
 tjekker efter der findes pladser deri, som er = 1,
 som er det samme som en bil. Hvis dette er kører den halvvejs op til næste 1 tal. Den breaker ud af
 funktionen, hvis vi overskrider længden af broen. */
void search_bro(long int bro[][LAENGDE_AF_BRO]){ 
  int i, j, g; 
 
  for (i = 0; i < ANTAL_BANER; i++){ 
    for (j = 0; j < LAENGDE_AF_BRO; j++){ 
      if (bro[i][j] == 1){ 
        bro[i][j] = 0; 
 
        for (g = 1; g <= 4; g++){ 
          if (j + g >= LAENGDE_AF_BRO){ 
            break; 
          } 
 
          if (bro[i][j + g] == 1){ 
            bro[i][j + g / 2] = 1; 
            j = j + g - 1; 
            break; 
          } 
        } 
 
        if (g >= 5 && j + g < LAENGDE_AF_BRO){ 
          bro[i][j + g - 1] = 1; 
          j = j + g - 1; 
        } 
      } 
    } 
  } 
} 
 /* mangler forklaring */
void Fordeling(int fordeling[], int num_af_biler, double vaegt){ 
  int i, N = num_af_biler * vaegt; 
  /* int *Poi_fordeling = (int *)malloc(N * sizeof(int)); */ 
  int counter, ikke_faerdig_sorteret = 0, min, omflytnings_plads; 
  int *Array_temp = (int *)malloc(N * sizeof(int)); 
 
  /* if (Poi_fordeling == NULL) */ 
  if (fordeling == NULL){ 
    printf("Fatal error.\n"); 
    exit(EXIT_FAILURE); 
  } 
 
  srand(time(NULL)); 
 
  for (i = 0; i < N; i++){ 
    fordeling[i] = Unif(0, SEK_PAA_TIME); 
    /* Poi_fordeling[i] = Unif(0, SEK_PAA_TIME); */ 
  } 
 
  /* qsort(Poi_fordeling, N, sizeof(double), cmpfunc); */ 
 
  while (ikke_faerdig_sorteret < N - 1){ 
    min = SEK_PAA_TIME; 
 
    for (counter = ikke_faerdig_sorteret; counter < N; counter++){ 
      /* if (Poi_fordeling[counter] < min) */ 
      if (fordeling[counter] < min){ 
        /* min = Poi_fordeling[counter]; */ 
        min = fordeling[counter]; 
        omflytnings_plads = counter; 
      } 
    } 
 
    if (omflytnings_plads != ikke_faerdig_sorteret){ 
      /* Array_temp[ikke_faerdig_sorteret] = Poi_fordeling[ikke_faerdig_sorteret]; 
      Poi_fordeling[ikke_faerdig_sorteret] = Poi_fordeling[omflytnings_plads]; 
      Poi_fordeling[omflytnings_plads] = Array_temp[ikke_faerdig_sorteret]; */ 
      Array_temp[ikke_faerdig_sorteret] = fordeling[ikke_faerdig_sorteret]; 
      fordeling[ikke_faerdig_sorteret] = fordeling[omflytnings_plads]; 
      fordeling[omflytnings_plads] = Array_temp[ikke_faerdig_sorteret]; 
    } 
 
    ikke_faerdig_sorteret++; 
  } 
 
  free(Array_temp); 
 
  /* return Poi_fordeling; */ 
} 
 
double Unif(int start, int slut){ 
  double u; 
  u = (rand() % 1000001) / 1000000.0; 
  u = u * (slut - start) + start; 
  return u; 
} 