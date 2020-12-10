#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#define biler_2020 125200
#define total_arlig_vaekst 0.027
#define arlig_vaekst_bro 0.0117
#define arlig_vaekst_tunnel 0.033
#define biler_2020_bro 34400
#define biler_2020_tunnel 90800
#define biler_2020_bro_med_Egholm 26900
#define biler_2020_tunnel_med_Egholm 69600
#define kapacitet_bro    31000
#define kapacitet_tunnel 80000
#define kapacitet_egholm 59000




float antal_biler (float vaekst,int antalbiler,float med_egholm);
float Egholm_linje(void);
void Kapacitet (void);


int main (void){
    int num;
    
    printf("Du har folgende valg muligheder:\n1.Trafik pa alle Limfjordsovergange helt til bestemt ar.\n2.Trafik pa Limfjordsbroen helt til bestemt ar.\n3.Trafik pa Limfjordstunnellen helt til bestemt ar.\n4.Trafik pa Egholmslinjen.\n5.Tidspunktet hvor overgangeneskapacitet bliver opbrugt.\nIndtast 0 for at komme ud fra programmet\n");
    printf("Valg et nummer:");
    scanf("%d",&num);
    while (num!=0){
        if(num==1){
            antal_biler(total_arlig_vaekst,biler_2020,biler_2020);}
        if(num==2){
            antal_biler(arlig_vaekst_bro,biler_2020_bro,biler_2020_bro_med_Egholm);}
        if(num==3){
            antal_biler(arlig_vaekst_tunnel,biler_2020_tunnel,biler_2020_tunnel_med_Egholm);}
        if(num==4){
            Egholm_linje();}
        if(num==5){
            Kapacitet ();
           }


    printf("Du har folgende valg muligheder:\n1.Trafik pa alle Limfjordsovergange helt til bestemt ar.\n2.Trafik pa Limfjordsbroen helt til bestemt ar.\n3.Trafik pa Limfjordstunnellen helt til bestemt ar.\n4.Trafik pa Egholmslinjen.\n5.Tidspunktet hvor overgangeneskapacitet bliver opbrugt.\nIndtast 0 for at komme ud fra programmet\n");
    printf("Valg et nummer:");
    scanf("%d",&num);}    
    return 0;
}

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
            printf("\t%d:\t\t%d\t\t%d\n",i,y,Y);}
     
    return y;
}

float Egholm_linje(void){
    int n,i,year,Yegholm,ybro,ytunel;
    float vbro,vtunel,ekstravaegt_bro=0,ekstravaegt_tunnel=0;
    printf("Indtast onsket arstal:");
        scanf("%d",&year);
        printf("\tYear\t\tAntal biler pa bro\tAntal biler pa tunnel\tAntal biler pa Egholmlinjen\n");
        for (i=2020;i<=year;i++){
            n = i - 2020;
            vbro=arlig_vaekst_bro*biler_2020_bro_med_Egholm*n;
            ybro=biler_2020_bro_med_Egholm+vbro;
            vtunel=arlig_vaekst_tunnel*biler_2020_tunnel_med_Egholm*n;
            ytunel=biler_2020_tunnel_med_Egholm+vtunel;
            Yegholm=0;
            if(ybro>kapacitet_bro){
                ekstravaegt_bro=ybro-kapacitet_bro;}
            if(ytunel>kapacitet_tunnel){
               ekstravaegt_tunnel=ytunel-kapacitet_tunnel;}

            Yegholm=ekstravaegt_bro+ekstravaegt_tunnel+28700;
            printf("\t%d:\t\t%d\t\t\t%d\t\t\t%d\n",i,ybro,ytunel,Yegholm);
        }
            
            return Yegholm;

}
void Kapacitet (void){
    printf("\tForbindelsernes navne\tKapacitet\tArstal\t\tAntal biler i arstallet\n");
    printf("\tLimfjordsbro\t\t%d\t\t%d\t\t%d\n\tLimfjordstunnel\t\t%d\t\t%d\t\t%d\n\tEgholmlinjen\t\t%d\t\t%d\t\t%d\n",kapacitet_bro,2034,31306,kapacitet_tunnel,2025,81084,kapacitet_egholm,2038,61207);
return ;
}
