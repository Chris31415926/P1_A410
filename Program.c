#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
/* #include <windows.h> */

#define LAENGDE_AF_BRO 105
#define BILER_2020 125200
#define TOTAL_AARLIG_VAEKST 0.027
#define AARLIG_VAEKST_BRO 0.0117
#define AARLIG_VAEKST_TUNNEL 0.033
#define BILER_2020_BRO 34400
#define BILER_2020_TUNNEL 90800
#define BILER_2020_BRO_MED_EGHOLM 26900
#define BILER_2020_TUNNEL_MED_EGHOLM 69600
#define BILER_2020_EGHOLM 28700
#define KAPACITET_BRO 31000
#define KAPACITET_TUNNEL 80000
#define KAPACITET_EGHOLM 59000
#define MYLDRETID_VAEGT 0.33
#define FREE_FLOW_VAEGT 0.02
#define TIME_PAA_DAG 24
#define SEK_PAA_TIME 3600

int Antal_biler (float vaekst, int antalbiler, float med_egholm); 
int Egholm_linje(int k, int egholm_trafik);
void Kapacitet (int antal_baner); 
void Simulation(int print_overgang, int baner);
void Et_aar_simulation(int antal_baner);
void Bro(long int bro[][LAENGDE_AF_BRO], int Egholmtrafik, int time, int print_overgang, int aar, long int Tabel[][4], int antal_baner);
void search_bro(long int bro[][LAENGDE_AF_BRO], int antal_baner);
void Fordeling(int fordeling[], int num_af_biler, double vaegt);
double Unif(int start, int slut);

int main(void){
  int input, egholmtrafik = BILER_2020_EGHOLM, year, antal_baner, counter;
  
  printf("Du har foelgende muligheder:\n");
  printf("1. Trafik paa alle Limfjordsovergange helt til et bestemt ar.\n");
  printf("2. Trafik paa Limfjordsbroen helt til et bestemt aar.\n");
  printf("3. Trafik paa Limfjordstunnelen helt til et bestemt aar.\n");
  printf("4. Trafik paa Egholmslinjen helt til et bestemt aar.\n");
  printf("5. Tidspunktet hvor overgangenes kapacitet bliver opbrugt.\n");
  printf("6. Fulde simulation af Egholmlinjen.\n");
  printf("7. Resultat for simulation af Egholmlinjen.\n");
  printf("8. Simulation til et bestemt aar og bestemt time.\n");
  printf("Indtast 0 for at komme ud fra programmet\n");

  printf("Valg et nummer:");
  scanf("%d", &input);

  while (input != 0){
    if (input == 1){
      Antal_biler(TOTAL_AARLIG_VAEKST, BILER_2020, BILER_2020);}

    else if (input == 2){
      Antal_biler(AARLIG_VAEKST_BRO, BILER_2020_BRO, BILER_2020_BRO_MED_EGHOLM);}

    else if (input == 3){
      Antal_biler(AARLIG_VAEKST_TUNNEL, BILER_2020_TUNNEL, BILER_2020_TUNNEL_MED_EGHOLM);}

    else if (input == 4){
      printf("Indtast onsket arstal:"); 
      scanf(" %d",&year); 
      
      for(counter = 2020; counter<=year; counter++){
        egholmtrafik = Egholm_linje(counter, egholmtrafik);
        
        printf("Aar : %d \t Trafik tallet: %d\n", counter, egholmtrafik);
      }
    }
    else{
      printf("Indtast antal baner paa Egholmlinjen:");
      scanf(" %d",&antal_baner);
    }
    if (input == 5){
      Kapacitet(antal_baner);
    }
    
    else if (input == 6){
      Simulation(1,antal_baner);
    }

    else if (input == 7){
      Simulation(0,antal_baner);
    }
    
    else if (input == 8){
      Et_aar_simulation(antal_baner);
    }

    printf("\n\n");

    printf("Du har foelgende muligheder:\n");
    printf("1. Trafik paa alle Limfjordsovergange helt til et bestemt ar.\n");
    printf("2. Trafik paa Limfjordsbroen helt til et bestemt aar.\n");
    printf("3. Trafik paa Limfjordstunnelen helt til et bestemt aar.\n");
    printf("4. Trafik paa Egholmslinjen helt til et bestemt aar.\n");
    printf("5. Tidspunktet hvor overgangenes kapacitet bliver opbrugt.\n");
    printf("6. Fulde simulation af Egholmlinjen.\n");
    printf("7. Resultat for simulation af Egholmlinjen.\n");
    printf("8. Simulation til et bestemt aar og bestemt time.\n");
    printf("Indtast 0 for at komme ud fra programmet\n");
  
    printf("Valg et nummer:");
    scanf(" %d", &input);
  }

  return EXIT_SUCCESS;
}

int Antal_biler (float vaekst, int antal_biler, float antal_biler_med_egholm){ 
  int aar, slut_aar, ny_maengde_biler, ny_maengde_biler_med_egholm, ikke_start_aar=0;
  
  printf("Indtast onsket arstal:");
  scanf(" %d",&slut_aar);

  printf("Aar \t\tAntal biler uden Egholm \tAntal biler med Egholm\n");

  for (aar=2020; aar<=slut_aar; aar++){ 
    ny_maengde_biler = antal_biler + vaekst*antal_biler*ikke_start_aar;
    antal_biler = ny_maengde_biler;

    ny_maengde_biler_med_egholm = antal_biler_med_egholm + vaekst*antal_biler_med_egholm*ikke_start_aar;
    antal_biler_med_egholm = ny_maengde_biler_med_egholm;
    
    printf("%d: \t\t\t%d \t\t\t\t%d\n", aar, ny_maengde_biler, ny_maengde_biler_med_egholm);

    ikke_start_aar = 1;
  } 

  return ny_maengde_biler_med_egholm;
} 

int Egholm_linje(int aar, int egholm_trafik){
  int ikke_start_aar=0, counter;
  float biler_bro_med_egholm = BILER_2020_BRO_MED_EGHOLM, biler_tunnel_med_egholm = BILER_2020_TUNNEL_MED_EGHOLM;
  float ekstravaegt_bro = 0, ekstravaegt_tunnel = 0;

  for(counter=2020; counter<=aar; counter++){
    if(counter!=2020){
      ikke_start_aar=1;
    }

    biler_bro_med_egholm = biler_bro_med_egholm + AARLIG_VAEKST_BRO * biler_bro_med_egholm * ikke_start_aar;
    biler_tunnel_med_egholm = biler_tunnel_med_egholm + AARLIG_VAEKST_TUNNEL * biler_tunnel_med_egholm * ikke_start_aar;
  
    if (biler_bro_med_egholm > KAPACITET_BRO)
      ekstravaegt_bro = biler_bro_med_egholm - KAPACITET_BRO;

    if (biler_tunnel_med_egholm > KAPACITET_TUNNEL)
      ekstravaegt_tunnel = biler_tunnel_med_egholm - KAPACITET_TUNNEL;

    egholm_trafik = ekstravaegt_bro + ekstravaegt_tunnel + BILER_2020_EGHOLM;
  }/* 
  printf("Aar : %d \t Trafik tallet: %d\n", aar, egholm_trafik); */

  return egholm_trafik;
}

void Kapacitet(int antal_baner){ 
  int biler_bro_med_egholm, biler_tunnel_med_egholm, egholm_trafik;
  int ikke_start_aar=0, ekstravaegt_bro=0, ekstravaegt_tunnel=0, out_bro=0, out_tunel=0;
  int aar_bro, aar_egholm, aar_tunnel, egholm_kapacitet, bro_resultat, tunnel_resultat;

  biler_bro_med_egholm = BILER_2020_BRO_MED_EGHOLM;
  biler_tunnel_med_egholm = BILER_2020_TUNNEL_MED_EGHOLM;

  egholm_kapacitet = KAPACITET_EGHOLM / 2 * antal_baner;
       
  for(aar_egholm=2020; egholm_trafik<egholm_kapacitet; aar_egholm++){
    if(aar_egholm!=2020){
      ikke_start_aar=1;}

    biler_bro_med_egholm = biler_bro_med_egholm + AARLIG_VAEKST_BRO * biler_bro_med_egholm * ikke_start_aar;
    if(biler_bro_med_egholm>KAPACITET_BRO){
      if(out_bro==0){
        aar_bro = aar_egholm;
        bro_resultat = biler_bro_med_egholm;
        out_bro = 1;
      }

      ekstravaegt_bro = biler_bro_med_egholm - KAPACITET_BRO;
    }

    biler_tunnel_med_egholm = biler_tunnel_med_egholm + AARLIG_VAEKST_TUNNEL * biler_tunnel_med_egholm *ikke_start_aar;

    if(biler_tunnel_med_egholm>KAPACITET_TUNNEL){
      if(out_tunel==0){
        aar_tunnel = aar_egholm;
        tunnel_resultat = biler_tunnel_med_egholm;
        out_tunel=1;
      }
      ekstravaegt_tunnel = biler_tunnel_med_egholm - KAPACITET_TUNNEL;
    }
  
    egholm_trafik = ekstravaegt_bro + ekstravaegt_tunnel + BILER_2020_EGHOLM;
  }
 
  printf("\tForbindelsernes navne\tKapacitet\tArstal\t\tAntal biler i arstallet\n"); 
  printf("\tLimfjordsbro\t\t%d\t\t%d\t\t%d\n", KAPACITET_BRO, aar_bro, bro_resultat); 
  printf("\tLimfjordstunnel\t\t%d\t\t%d\t\t%d\n", KAPACITET_TUNNEL, aar_tunnel, tunnel_resultat);
  printf("\tEgholmlinjen\t\t%d\t\t%d\t\t%d\n", egholm_kapacitet, aar_egholm-1, egholm_trafik);
} 

void Simulation(int print_overgang,int antal_baner){
  int counter_baner, counter_laengde, aar, time, tabel_taeller, kapacitet;
  int egholmtrafik = BILER_2020_EGHOLM;
  long int bro[antal_baner][LAENGDE_AF_BRO], Tabel[100][4];
  
  printf("Simulationen starter med at kore fra aar 2030\n");

  for (aar = 2020; aar < 2030; aar++){
    egholmtrafik = Egholm_linje(aar, egholmtrafik);
  }

  kapacitet = KAPACITET_EGHOLM / 2 * antal_baner;

  for (aar = 2030; egholmtrafik < kapacitet; aar++){
    for (counter_baner = 0; counter_baner < antal_baner; counter_baner++){
      for (counter_laengde = 0; counter_laengde < LAENGDE_AF_BRO; counter_laengde++){
        bro[counter_baner][counter_laengde] = 0;

        if(print_overgang==1){
          printf("%ld", bro[counter_baner][counter_laengde]);
          if (counter_laengde == (LAENGDE_AF_BRO - 1))
            printf("\n");
        }
      }
    }

    egholmtrafik = Egholm_linje(aar, egholmtrafik);
    
    printf("Aar : %d \t Trafik tallet: %d\n", aar, egholmtrafik);

    Tabel[aar-2020][0] = aar;
    Tabel[aar-2020][1] = egholmtrafik;
    Tabel[aar-2020][2] = 0;
    Tabel[aar-2020][3] = 0;

    /* Sleep(1000); */
    sleep(3);
    
    for (time = 0; time < TIME_PAA_DAG; time++){
      Bro(bro, egholmtrafik/2, time, print_overgang, aar, Tabel, antal_baner);
    }
    
    if(print_overgang==1){
    /* printf("\e[1;1H\e[2J"); */
    system("clear");
    }
  }

  printf("\nAarstal\tMaengden af biler\tSamlet koe for dagen\tBiler i koe som har ventet laengere tid\n");
  for(tabel_taeller=2030; tabel_taeller<aar; tabel_taeller++){
    printf("%ld\t", Tabel[tabel_taeller-2020][0]);
    printf("%ld\t\t\t", Tabel[tabel_taeller-2020][1]);
    printf("%ld\t\t\t", Tabel[tabel_taeller-2020][2]);
    printf("%ld\n", Tabel[tabel_taeller-2020][3]);
  }
  printf("\n");
}

void Et_aar_simulation(int antal_baner){ 
  int counter_baner, counter_laengde, aar,klok=0 ; 
  int egholmtrafik = BILER_2020_EGHOLM; 
  long int bro[antal_baner][LAENGDE_AF_BRO], Tabel[100][4]; 
  
  printf("Indtast det aar, fra 2020 og efter, som du vil koere simulation over:\n"); 
  scanf("%d", &aar);
  printf("Indtast tidspunktet, som du onsker at koere simulationen over:\n");
  scanf("%d",&klok);
  
  for (counter_baner = 0; counter_baner < antal_baner; counter_baner++){ 
    for (counter_laengde = 0; counter_laengde < LAENGDE_AF_BRO; counter_laengde++){ 
      bro[counter_baner][counter_laengde] = 0; 
 
      printf("%ld", bro[counter_baner][counter_laengde]); 
      if (counter_laengde == (LAENGDE_AF_BRO - 1)) 
        printf("\n"); 
    } 
  } 
  
  egholmtrafik = Egholm_linje(aar, egholmtrafik); 

  Tabel[0][0] = aar;
  Tabel[0][1] = egholmtrafik;
  Tabel[0][2] = 0;
  Tabel[0][3] = 0;
  
  printf("\n"); 
 
  sleep(2); 
 
  Bro(bro, egholmtrafik/2, klok, 1, 2020, Tabel, antal_baner); 
  
  printf("\nAarstal\tMaengden af biler\tSamlet koe for dagen\tBiler i koe som har ventet længere tid\n");
  printf("%ld\t", Tabel[0][0]);
  printf("%ld\t\t\t", Tabel[0][1]);
  printf("%ld\t\t\t", Tabel[0][2]);
  printf("%ld\n", Tabel[0][3]);
  printf("\n");
} 

void Bro(long int bro[][LAENGDE_AF_BRO], int egholmtrafik, int time, int print_overgang, int aar, long int Tabel[][4], int antal_baner){
  int sek, bane, plads, koe, ny_bil = 0, foerste_plads, samlet_koe = 0, koe_der_har_ventet_laengere = 0;
  int *fordeling;
  int Fordeling_free_flow[(int)(egholmtrafik * FREE_FLOW_VAEGT)];
  int Fordeling_myldretid[(int)(egholmtrafik * MYLDRETID_VAEGT)];
 
  
  if (time == 7 || time == 15){
    Fordeling(Fordeling_myldretid, egholmtrafik, MYLDRETID_VAEGT);
    fordeling = Fordeling_myldretid;
  }
  else{
    Fordeling(Fordeling_free_flow, egholmtrafik, FREE_FLOW_VAEGT);
    fordeling = Fordeling_free_flow;
  }

  for (sek = 0; sek < SEK_PAA_TIME; sek++){
    if(print_overgang==1){
      printf("Den %d time.\t Det %d sek.\n", time, sek);
    }

    search_bro(bro,antal_baner);

    while (koe != 0){
      bane = 0;
      foerste_plads = 0;
      while (bane < antal_baner){
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
    koe_der_har_ventet_laengere+=koe;

    while (fordeling[ny_bil] == sek){
      bane = 0;
      foerste_plads = 0;

      while (bane < antal_baner){
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
    samlet_koe+=koe;
    
    if(print_overgang == 1){
      printf("Antal biler i koe: %d\n", koe);
    
      for (bane = 0; bane < antal_baner; bane++){
        for (plads = 0; plads < LAENGDE_AF_BRO; plads++){
          printf("%ld", bro[bane][plads]);
        }
        printf("\n");
      }
      printf("\n\n");

      /* system("cls"); */ 
      printf("\e[1;1H\e[2J");

      /* sleep(0.1); */
    }

    Tabel[aar-2020][2] += samlet_koe;
    Tabel[aar-2020][3] += koe_der_har_ventet_laengere;
  }
}

void search_bro(long int bro[][LAENGDE_AF_BRO],int antal_baner){
  int counter_baner, counter_laengde, kig_frem;

  for (counter_baner = 0; counter_baner < antal_baner; counter_baner++){
    for (counter_laengde = 0; counter_laengde < LAENGDE_AF_BRO; counter_laengde++){
      if (bro[counter_baner][counter_laengde] == 1){
        bro[counter_baner][counter_laengde] = 0;

        for (kig_frem = 1; kig_frem <= 4; kig_frem++){
          if (counter_laengde + kig_frem >= LAENGDE_AF_BRO){
            break;
          }

          if (bro[counter_baner][counter_laengde + kig_frem] == 1){
            bro[counter_baner][counter_laengde + kig_frem / 2] = 1;
            counter_laengde = counter_laengde + kig_frem - 1;
            break;
          }
        }

        if (kig_frem >= 5 && counter_laengde + kig_frem < LAENGDE_AF_BRO){
          bro[counter_baner][counter_laengde + kig_frem - 1] = 1;
          counter_laengde = counter_laengde + kig_frem - 1;
        }
      }
    }
  }
}

void Fordeling(int fordeling[], int num_af_biler, double vaegt){
  int counter_tilfaeldig_tal, num_af_biler_paa_time = num_af_biler * vaegt, temp_var;
  int counter, ikke_faerdig_sorteret = 0, min, omflytnings_plads;
  int *Array_temp = (int *)malloc(num_af_biler_paa_time * sizeof(int));

  if (fordeling == NULL){
    printf("Fatal error.\n");
    exit(EXIT_FAILURE);
  }

  /* srand(time(NULL)); */
  srand(SEK_PAA_TIME*TIME_PAA_DAG);

  for (counter_tilfaeldig_tal = 0; counter_tilfaeldig_tal < num_af_biler_paa_time; counter_tilfaeldig_tal++){
    fordeling[counter_tilfaeldig_tal] = Unif(0, SEK_PAA_TIME);
  }

  /* qsort(Poi_fordeling, N, sizeof(double), cmpfunc); */

  while (ikke_faerdig_sorteret < num_af_biler_paa_time - 1){
    min = SEK_PAA_TIME;

    for (counter = ikke_faerdig_sorteret; counter < num_af_biler_paa_time; counter++){
      if (fordeling[counter] < min){
        min = fordeling[counter];
        omflytnings_plads = counter;
      }
    }

    if (omflytnings_plads != ikke_faerdig_sorteret){
      temp_var = fordeling[ikke_faerdig_sorteret];
      fordeling[ikke_faerdig_sorteret] = fordeling[omflytnings_plads];
      fordeling[omflytnings_plads] = temp_var;
    }

    ikke_faerdig_sorteret++;
  }

  free(Array_temp);
}

double Unif(int start, int slut){
  double u;
  u = (rand() % 10001) / 10000.0;
  u = u * (slut - start) + start;
  return u;
}
