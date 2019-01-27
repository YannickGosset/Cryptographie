// -*- coding: utf-8 -*-

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       // log, pow

typedef unsigned char uchar;

/* La clef courte K utilisée est formée de 16 octets nuls */
int longueur_de_la_clef = 16 ;
uchar K[16] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;

/* Résultat du TP précédent : diversification de la clef courte K en une clef étendue W */

int Nr = 10, Nk = 4;
int longueur_de_la_clef_etendue = 176;
uchar W[176] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63,
  0x9B, 0x98, 0x98, 0xC9, 0xF9, 0xFB, 0xFB, 0xAA, 0x9B, 0x98, 0x98, 0xC9, 0xF9, 0xFB, 0xFB, 0xAA,
  0x90, 0x97, 0x34, 0x50, 0x69, 0x6C, 0xCF, 0xFA, 0xF2, 0xF4, 0x57, 0x33, 0x0B, 0x0F, 0xAC, 0x99,
  0xEE, 0x06, 0xDA, 0x7B, 0x87, 0x6A, 0x15, 0x81, 0x75, 0x9E, 0x42, 0xB2, 0x7E, 0x91, 0xEE, 0x2B,
  0x7F, 0x2E, 0x2B, 0x88, 0xF8, 0x44, 0x3E, 0x09, 0x8D, 0xDA, 0x7C, 0xBB, 0xF3, 0x4B, 0x92, 0x90,
  0xEC, 0x61, 0x4B, 0x85, 0x14, 0x25, 0x75, 0x8C, 0x99, 0xFF, 0x09, 0x37, 0x6A, 0xB4, 0x9B, 0xA7,
  0x21, 0x75, 0x17, 0x87, 0x35, 0x50, 0x62, 0x0B, 0xAC, 0xAF, 0x6B, 0x3C, 0xC6, 0x1B, 0xF0, 0x9B,
  0x0E, 0xF9, 0x03, 0x33, 0x3B, 0xA9, 0x61, 0x38, 0x97, 0x06, 0x0A, 0x04, 0x51, 0x1D, 0xFA, 0x9F,
  0xB1, 0xD4, 0xD8, 0xE2, 0x8A, 0x7D, 0xB9, 0xDA, 0x1D, 0x7B, 0xB3, 0xDE, 0x4C, 0x66, 0x49, 0x41,
  0xB4, 0xEF, 0x5B, 0xCB, 0x3E, 0x92, 0xE2, 0x11, 0x23, 0xE9, 0x51, 0xCF, 0x6F, 0x8F, 0x18, 0x8E
};

/* Le bloc à chiffrer aujourd'hui: 16 octets nuls */
uchar State[16] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00} ;

uchar Default_State[16] = {
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00} ;

uchar SB_State[16] = {
  0x00, 0x01, 0x02, 0x03, 
  0x00, 0x01, 0x02, 0x03, 
  0x00, 0x01, 0x02, 0x03, 
  0x00, 0x01, 0x02, 0x03};

uchar SR_State[16] = {
  0xA0, 0xB0, 0xC0, 0xD0,
  0xA1, 0xB1, 0xC1, 0xD1,
  0xA2, 0xB2, 0xC2, 0xD2,
  0xA3, 0xB3, 0xC3, 0xD3};

uchar MC_State[16] = {
  0x0E, 0x0B, 0x0D, 0x09,
  0x0B, 0x0E, 0x0B, 0x0D,
  0x0D, 0x0B, 0x0E, 0x0B,
  0x09, 0x0D, 0x0B, 0x0E};

uchar ARK_State[16] = {
  0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF};

/* Déclaration des 4 transformations à implémenter */
void SubBytes(void);
void ShiftRows(void);
void MixColumns(void);
void AddRoundKey(int r);
/* Programme principal */

void chiffrer(void);
void afficher_le_bloc(uchar *M);

int main(void) {/*
  memmove(State,SB_State,sizeof(State));
  printf("Le bloc \"SB_State\" en entrée vaut : \n");
  afficher_le_bloc(State);
  SubBytes();
  printf("Le bloc \"SB_State\" en sortie vaut : \n");
  afficher_le_bloc(State);*/
/*
  memmove(State,SR_State,sizeof(State));
  printf("Le bloc \"SR_State\" en entrée vaut : \n");
  afficher_le_bloc(State);
  ShiftRows();
  printf("Le bloc \"SR_State\" en sortie vaut : \n");
  afficher_le_bloc(State);*/
/*
  memmove(State,MC_State,sizeof(State));
  printf("Le bloc \"MC_State\" en entrée vaut : \n");
  afficher_le_bloc(State);
  MixColumns();
  printf("Le bloc \"MC_State\" en sortie vaut : \n");
  afficher_le_bloc(State);
*/
  memmove(State,ARK_State,sizeof(State));
  printf("Le bloc \"ARK_State\" en entrée vaut : \n");
  afficher_le_bloc(State);
  AddRoundKey(1);
  printf("Le bloc \"ARK_State\" en sortie vaut : \n");
  afficher_le_bloc(State);

/*
  memmove(State,Default_State,sizeof(State));
  printf("Le bloc \"State\" en entrée vaut : \n");
  afficher_le_bloc(State);
  chiffrer();
  printf("Le bloc \"State\" en sortie vaut : \n");
  afficher_le_bloc(State);*/
  exit(EXIT_SUCCESS);
}

void afficher_le_bloc(uchar *M) {
  for (int i=0; i<4; i++) { // Lignes 0 à 3
    printf("          ");
    for (int j=0; j<4; j++) { // Colonnes 0 à 3
      printf ("%02X ", M[4*j+i]); 
    }
    printf("\n");
  }
}


void chiffrer(void){
  int i;
  AddRoundKey(0);
  for (i = 1; i < Nr; i++) {
    SubBytes();
    ShiftRows();
    MixColumns();
    AddRoundKey(i);
  }
  SubBytes();
  ShiftRows();
  AddRoundKey(Nr);
}

/* Table de substitution déjà utilisée lors du TP précédent */

uchar SBox[256] = {
  0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
  0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
  0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
  0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
  0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
  0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
  0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
  0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
  0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
  0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
  0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
  0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
  0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
  0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
  0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
  0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};


/* Fonction mystérieuse qui calcule le produit de deux octets */

uchar gmul(uchar a, uchar b) {
  uchar p = 0;
  uchar hi_bit_set;
  int i;
  for(i = 0; i < 8; i++) {
    if((b & 1) == 1)
      p ^= a;
    hi_bit_set = (a & 0x80);
    a <<= 1;
    if(hi_bit_set == 0x80)
      a ^= 0x1b;
    b >>= 1;
  }
  return p & 0xFF;
}

/* Partie à compléter pour ce TP */

void SubBytes(void){
  for(int i = 0; i < longueur_de_la_clef; ++i){
      int cell = State[i]; //hex to int...
      State[i] = SBox[cell];
  }
};

void ShiftRows(void){
  int posNew;
  int posOld;
  for(int i = 1; i < 4; ++i){
    int line = i;
    for(int k = 0; k < line; ++k){
      for(int j = 0; j < 4; ++j){
        //ShiftOnce(line);

        posNew = i+j*4;
        if(j < line)
          posOld = longueur_de_la_clef - 4*line + posNew;
        else
          posOld = posNew - 4;
        uchar temp = State[posNew];
        printf ("temp = %02X ", temp);
        printf ("new = %02X ", State[posNew]);
        printf ("old = %02X ", State[posOld]);
        
        printf("%d , %d\n", posNew, posOld);
        State[posNew] = State[posOld];
        if(j < 3){
          State[posOld] = temp;
        }else{
          State[posNew] = temp;
        }
      }
    }
  }
};

uchar MixCol_State[16] = {
  0x02, 0x03, 0x01, 0x01,
  0x01, 0x02, 0x03, 0x01,
  0x01, 0x01, 0x02, 0x03,
  0x03, 0x01, 0x02, 0x02};


void MixColumns(void){

};

void AddRoundKey(int r){
  for(int i = 0; i < longueur_de_la_clef; ++i){
    State[i] = State[i] ^ W[(r*16)+i];
  }
};

/* Pour compiler:
  $ make
  gcc aes.c -o aes -lm -std=c99
  $ ./aes
*/


/* Quelques tests pour les fonctions à compléter

Test de SubBytes():
Le bloc "State" en entrée vaut :
          00 00 00 00
          01 01 01 01
          02 02 02 02
          03 03 03 03
Le bloc "State" en sortie vaut :
          63 63 63 63
          7C 7C 7C 7C
          77 77 77 77
          7B 7B 7B 7B

Test de ShiftRows():
Le bloc "State" en entrée vaut :
          A0 A1 A2 A3
          B0 B1 B2 B3
          C0 C1 C2 C3
          D0 D1 D2 D3
Le bloc "State" en sortie vaut :
          A0 A1 A2 A3
          B1 B2 B3 B0
          C2 C3 C0 C1
          D3 D0 D1 D2

Test de MixColumns():
Le bloc "State" en entrée vaut :
          0E 0B 0D 09
          09 0E 0B 0D
          0D 09 0E 0B
          0B 0D 09 0E
Le bloc "State" en sortie vaut :
          01 00 00 00
          00 01 00 00
          00 00 01 00
          00 00 00 01

Test de AddRoundKey():
La clef de ronde 1 vaut :
          62 62 62 62
          63 63 63 63
          63 63 63 63
          63 63 63 63
Le bloc "State" en entrée vaut :
          FF FF FF FF
          FF FF FF FF
          FF FF FF FF
          FF FF FF FF
Le bloc "State" en sortie vaut :
          9D 9D 9D 9D
          9C 9C 9C 9C
          9C 9C 9C 9C
          9C 9C 9C 9C

Test final: chiffrement du bloc nul:
Le bloc "State" en entrée vaut :
          00 00 00 00
          00 00 00 00
          00 00 00 00
          00 00 00 00
Le bloc "State" en sortie vaut :
          66 EF 88 CA
          E9 8A 4C 34
          4B 2C FA 2B
          D4 3B 59 2E
*/