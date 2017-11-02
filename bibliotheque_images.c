/********
Fichier: bibliotheque_images.c
Auteurs: Domingo Palao Munoz
         Charles-Antoine Brunet
Date: 27 octobre 2017
Description: Fichier de distribution pour l'APP4snte de S1.
********/

#include "bibliotheque_images.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int pgm_lire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
             int *p_lignes, int *p_colonnes, 
             int *p_maxval, struct MetaData *p_metadonnees)
{
	int n, m, o = 1, a = 0, d = 0, l = 0;						// compteurs
	int lenNomP = 0, lenDateP = 0, lenLieuP = 0;      	// longueur problematique des metadonnees       
	int lenNom, lenDate, lenLieu;						// longueur des metadonnees
	
	char premierligne[256];
	char format[20];
	FILE *flot_data;
	
	flot_data = fopen(nom_fichier, "r");
	p_metadonnees->auteur[0] = '\0';
	p_metadonnees->dateCreation[0] = '\0';
	p_metadonnees->lieuCreation[0] = '\0';
	
	if(flot_data == NULL){
		return ERREUR_FICHIER;
		} else {
			fgets(premierligne, 256, flot_data);         			// lire premiere ligne 
			if(premierligne[0] != '#' && (premierligne[0] != 'P' || premierligne[1] != '2')){
				return ERREUR_FORMAT; 								// mauvais format d'image sans metadonnees
			} else if(premierligne[0] == '#') {
				while(premierligne[o] != ';' && premierligne[o] != '\0' && premierligne[o] != '\n'){
					p_metadonnees->auteur[a] = premierligne[o];
					o++;
					a++;
				}
				o++;
				while(premierligne[o] != ';' && premierligne[o] != '\0' && premierligne[o] != '\n'){
					p_metadonnees->dateCreation[d] = premierligne[o];
					o++;
					d++;
				}
				o++;
				while(premierligne[o] != '\0' && premierligne[o] != '\n'){
					p_metadonnees->lieuCreation[l] = premierligne[o];
					o++;
					l++;
				}
				lenNom = strlen(p_metadonnees->auteur);
				lenDate = strlen(p_metadonnees->dateCreation);
				lenLieu = strlen(p_metadonnees->lieuCreation);
				if(lenNom == lenNomP || lenDate == lenDateP || lenLieu == lenLieuP){
					return ERREUR_FORMAT;                             // mauvaises metadonnees
				
				}				
				fgets(format, 20, flot_data);
				if((format[0] != 'P' || format[1] != '2')){
					return ERREUR_FORMAT;                            // mauvais format d'image avec metadonnees
				}
				} else {
					strcpy(format, premierligne);
				}		
				fscanf(flot_data, "%d", p_colonnes);
				fscanf(flot_data, "%d", p_lignes);
				
				if(*p_colonnes > MAX_LARGEUR || *p_lignes > MAX_HAUTEUR){
					return ERREUR_TAILLE;									// matrice trop grande
				} else {
					fscanf(flot_data, "%d", p_maxval);						// ajout des valeurs de donnees a la matrice
					if(*p_maxval > MAX_VALEUR){
						return ERREUR_FORMAT;								// valeur max trop grande
					} else {
						for(n = 0; n < *p_colonnes; n++){
							for (m = 0; m < *p_lignes; m++){
								fscanf(flot_data,"%d", &matrice[n][m]);
							}
						}
						fclose(flot_data);
						return OK;
						}
	}
}
}


int pgm_ecrire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
               int lignes, int colonnes, 
               int maxval, struct MetaData metadonnees)
			   
{
	int n, m;						// compteurs
	int lenNomP = 0, lenDateP = 10, lenLieuP = 0;      	// longueur problematique des metadonnees       
	int lenNom, lenDate, lenLieu;						// longueur des metadonnees
	
	//char premierligne[256];
	//char format[20];
	FILE *flot_data;
	
	flot_data = fopen(nom_fichier, "w");
	
	if(flot_data == NULL){
		return ERREUR_FICHIER;
		} else {
			lenNom = strlen(metadonnees.auteur);
			lenDate = strlen(metadonnees.dateCreation);
			lenLieu = strlen(metadonnees.lieuCreation);
			if(lenNom != lenNomP || lenDate == lenDateP || lenLieu != lenLieuP){
			fprintf(flot_data,"#%s;%s;%s\n", metadonnees.auteur, metadonnees.dateCreation, metadonnees.lieuCreation);
			}
			fprintf(flot_data,"P2\n");
			fprintf(flot_data,"%d %d\n", lignes, colonnes);
			fprintf(flot_data,"%d\n", maxval);
			
			for(n = 0; n < lignes; n++){
				for(m = 0; m < colonnes; m++){
					if(m != colonnes - 1){
						fprintf(flot_data,"%d ", matrice[n][m]);
					} else {
						fprintf(flot_data,"%d", matrice[n][m]);
					}
					
				}
				fprintf(flot_data,"\n");				
			}
		}
			
	fclose(flot_data);		
    return OK;
}

int pgm_copier( int matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, int colonnes1,
		int matrice2[MAX_HAUTEUR][MAX_LARGEUR], int *p_lignes2, int *p_colonnes2){
	int n, m;
	
	if(lignes1 > MAX_HAUTEUR|| colonnes1 > MAX_LARGEUR){
		return ERREUR;
	} else {
		*p_lignes2 = lignes1;
		*p_colonnes2 = colonnes1;
		for(m = 0; m < lignes1; m++){
			for(n = 0; n < colonnes1; n++){
				matrice2[m][n] = matrice1[m][n];
			}
		}
	}
	// s'assurer que les boucles ont complete leurs iterations
	if(n > colonnes1 && m > lignes1){
			return ERREUR;
	} else {
		return OK;
	}	
}

int pgm_creer_histogramme(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
		int lignes, int colonnes, int histogramme[MAX_VALEUR+1]){

	int m, n;       // compteurs
	int val_actuelle;    // stockage de la valeur en lecture
	if(lignes > MAX_HAUTEUR|| colonnes > MAX_LARGEUR){
		return ERREUR;
	} else {
		for(m = 0; m < MAX_HAUTEUR; m++){
			for(n = 0; n < MAX_LARGEUR; n++){
				val_actuelle = matrice[m][n];
				histogramme[val_actuelle]++;
			}
		}
	}
	// s'assurer que les boucles ont complete leurs iterations
	if(n>colonnes && m >lignes){
			return ERREUR;
	} else {
		return OK;
	}		
}

int pgm_creer_histogramme_termine(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
		int lignes, int colonnes, int histogramme[MAX_VALEUR+1]){

	int m, n;       // compteurs
	int val_actuelle;    // stockage de la valeur en lecture
	if(lignes > MAX_HAUTEUR|| colonnes > MAX_LARGEUR){
		return ERREUR;
	} else {
		for(m = 0; m < MAX_HAUTEUR; m++){
			for(n = 0; n < MAX_LARGEUR; n++){
				val_actuelle = matrice[m][n];
				histogramme[val_actuelle]++;
			}
		}
		histogramme[val_actuelle+1] = -1;                        // on veut un symbole de fin
	}
	// s'assurer que les boucles ont complete leurs iterations
	if(n>colonnes && m >lignes){
			return ERREUR;
	} else {
		return OK;
	}		
}
		
int pgm_couleur_preponderante(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
	int lignes, int colonnes)
{
	int n = 1, val_prep, retour, ind_val_prep = 0;
	int histo[MAX_VALEUR+1];
	
	if(lignes > MAX_HAUTEUR|| colonnes > MAX_LARGEUR){
		return ERREUR;
	} else {
		retour = pgm_creer_histogramme_termine(matrice, lignes, colonnes, histo);
		if(retour < 0){
			return ERREUR;
		} else {
			val_prep = histo[0];
			while(histo[n] != -1){
				if (histo[n] > val_prep){
					val_prep = histo[n];
					ind_val_prep = n;
				}
				n++;
			}
		}

	}	
	//printf("", ind_val_prep);
	return ind_val_prep;
}

int pgm_eclaircir_noircir(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
int lignes, int colonnes, int maxval, int valeur){

	int m, n, val;       // compteurs
	
	if(lignes > MAX_HAUTEUR|| colonnes > MAX_LARGEUR || maxval > MAX_VALEUR){
		return ERREUR;
	} else {
		for(m = 0; m < MAX_HAUTEUR; m++){
			for(n = 0; n < MAX_LARGEUR; n++){
				val = matrice[m][n] + valeur;
				if(val < 0){
					val = 0;
				} else if(val > maxval) {
					val = maxval;
				}
				matrice[m][n] = val;
			}
		}
		
	}
	return OK;
}

int pgm_creer_negatif(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
	int lignes, int colonnes, int maxval){
		
	int m, n;       // compteurs
	
	if(lignes > MAX_HAUTEUR|| colonnes > MAX_LARGEUR || maxval > MAX_VALEUR){
		return ERREUR;
	} else {
		for(m = 0; m < MAX_HAUTEUR; m++){
			for(n = 0; n < MAX_LARGEUR; n++){
				matrice[m][n] = maxval - matrice[m][n];
			}
		}
	}
		
		
	return OK;
}

int pgm_extraire(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int lignes1,
int colonnes1, int lignes2, int colonnes2, int *p_lignes, int *p_colonnes){
	int deltaX, deltaY;
	int m, n;
	
	//printf("%d\t", *p_colonnes);
	//printf("%d\n", *p_lignes);
	
	if(lignes1 < 0 || colonnes1 < 0 || lignes2 <= lignes1 || colonnes2 <= colonnes1 || lignes2 > *p_lignes ||
		colonnes2 > *p_colonnes || *p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR){
		return ERREUR;
	} else {
		deltaX = colonnes2 - colonnes2;
		deltaY = lignes2 - lignes2;
		int matTemp[deltaY][deltaX];
		
		for(m = 0; m < deltaY; m++){
			for(n = 0; n < deltaX; n++){
				matTemp[m][n] = matrice[colonnes1 + m][lignes1 + n];
			}
		}
		for(m = 0; m < deltaY; m++){
			for(n = 0; n < deltaX; n++){
				matrice[m][n] = matTemp[m][n];
			}
		}
		*p_lignes = deltaY;
		*p_colonnes = deltaX;
		
	}
	return OK;
}

int pgm_reduire(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
				int *p_lignes, int *p_colonnes){
					
	int n1, m1, k;
	int n2, m2, l;
	int n3, m3;
	int tempLignes, tempColonnes;
	
	if(*p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR){
		return ERREUR;
	} else {
		
		if(*p_lignes%2 ==1){
			tempLignes = *p_lignes - 1; 
		} else {
			tempLignes = *p_lignes;
		}
		if(*p_colonnes%2 ==1){
			tempColonnes = *p_colonnes - 1; 
		} else {
			tempColonnes = *p_colonnes;
		}
		
		int tempMat1[tempLignes][tempColonnes/2];
		int tempMat2[tempLignes/2][tempColonnes/2];
		
		for(n1 = 0; n1 < tempLignes; n1++){
			k = 0;
			for(m1 = 0; m1 < tempColonnes; m1 += 2){
				tempMat1[n1][k] = (matrice[n1][m1] + matrice[n1][m1+1])/2; 
				k++;
			}
		}
		for(n2 = 0; n2 < tempLignes; n2 += 2){
			l = 0;
			for(m2 = 0; m2 < tempColonnes/2; m2++){
				tempMat2[l][m2] = (tempMat1[n2][m2] + tempMat1[n2+1][m2])/2;
				l++;
			}
		}
		for(n3 = 0; n3 < tempLignes/2; n3++){
			for(m3 = 0; m3 < tempColonnes/2; m3++){
				matrice[n3][m3] = tempMat2[n3][m3];
			}
		}
		*p_colonnes = tempColonnes/2;
		*p_lignes = tempLignes/2;
 
	
	}
	
	

	
	return OK;
}

int pgm_sont_identiques(int matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, int colonnes1,
						int matrice2[MAX_HAUTEUR][MAX_LARGEUR], int lignes2, int colonnes2){
	int n, m; 
	
	if(lignes1 != lignes2|| colonnes1 != colonnes2 || lignes1 > MAX_HAUTEUR|| colonnes1 > MAX_LARGEUR ){
		return ERREUR;
	} else if (lignes1 != lignes2 || colonnes1 != colonnes2){
		return DIFFERENTES;
	} else {
		for(m = 0; m < lignes1; m++){
			for(n = 0; n < colonnes1; n++){
				if(matrice1[m][n] != matrice2[m][n]){
					return DIFFERENTES;
				}
			}
		}
	}
	return IDENTIQUES;
}

int pgm_pivoter90(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
				int *p_lignes, int *p_colonnes, int sens){
	// int m, n;
	// int tempMat[p_lignes][p_colonnes];
	
	// if(*p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR){
		// return ERREUR;
	// } else {
		// if(sens == 1){
			// for(m = 0; m < p_lignes; m++){
				// for(n = 0; n < p_colonnes; n++){
					// matrice[m][n] = tempMat[*p_lignes - 1 - n][m];
				// } 
					
				// }
			// } else {
				// for(m = 0; m < p_lignes; m++){
					// for(n = 0; n < p_colonnes; n++){
						// matrice[m][n] = tempMat[][];
					// } 
					
				// }
				
			// }
	// }
		
	
	return 0;
}

int ppm_lire(char nom_fichier[], struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR],
int *p_lignes, int *p_colonnes, int *p_maxval, struct MetaData *metadonnees){
	
	int n, m, o = 1, a = 0, d = 0, l = 0;						// compteurs
	int lenNomP = 0, lenDateP = 10, lenLieuP = 0;      	// longueur problematique des metadonnees       
	int lenNom, lenDate, lenLieu;						// longueur des metadonnees
	
	char premierligne[256];
	char format[20];
	FILE *flot_data;
	
	flot_data = fopen(nom_fichier, "r");
	metadonnees->auteur[0] = '\0';
	metadonnees->dateCreation[0] = '\0';
	metadonnees->lieuCreation[0] = '\0';
	
	if(flot_data == NULL){
		return ERREUR_FICHIER;
		} else {
			fgets(premierligne, 256, flot_data);         			// lire premiere ligne 
			if(premierligne[0] != '#' && (premierligne[0] != 'P' || premierligne[1] != '3')){
				return ERREUR_FORMAT; 								// mauvais format d'image sans metadonnees
			} else if(premierligne[0] == '#') {
				while(premierligne[o] != ';' && premierligne[o] != '\0' && premierligne[o] != '\n'){
					metadonnees->auteur[a] = premierligne[o];
					o++;
					a++;
				}
				o++;
				while(premierligne[o] != ';' && premierligne[o] != '\0' && premierligne[o] != '\n'){
					metadonnees->dateCreation[d] = premierligne[o];
					o++;
					d++;
				}
				o++;
				while(premierligne[o] != '\0' && premierligne[o] != '\n'){
					metadonnees->lieuCreation[l] = premierligne[o];
					o++;
					l++;
				}
				lenNom = strlen(metadonnees->auteur);
				lenDate = strlen(metadonnees->dateCreation);
				lenLieu = strlen(metadonnees->lieuCreation);
				if(lenNom == lenNomP || lenDate != lenDateP || lenLieu == lenLieuP){
					return ERREUR_FORMAT;                             // mauvaises metadonnees
				
				}				
				fgets(format, 20, flot_data);
				if((format[0] != 'P' || format[1] != '3')){
					return ERREUR_FORMAT;                            // mauvais format d'image avec metadonnees
				}
				} else {
					strcpy(format, premierligne);
				}		
				fscanf(flot_data, "%d", p_colonnes);
				fscanf(flot_data, "%d", p_lignes);
				
				if(*p_colonnes > MAX_LARGEUR || *p_lignes > MAX_HAUTEUR){
					return ERREUR_TAILLE;									// matrice trop grande
				} else {
					fscanf(flot_data, "%d", p_maxval);						// ajout des valeurs de donnees a la matrice
					if(*p_maxval > MAX_VALEUR){
						return ERREUR_FORMAT;								// valeur max trop grande
					} else {
						for(n = 0; n < *p_colonnes; n++){
							for (m = 0; m < *p_lignes; m++){
								fscanf(flot_data, "%d", &matrice[n][m].valeurR);
								fscanf(flot_data, "%d", &matrice[n][m].valeurG);
								fscanf(flot_data, "%d", &matrice[n][m].valeurB);
							}
						}
						fclose(flot_data);
						return OK;
						}
	}
}
}

int ppm_ecrire(char nom_fichier[], struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR],
int lignes, int colonnes, int maxval, struct MetaData metadonnees){
	int n, m;						// compteurs
	int lenNomP = 0, lenDateP = 0, lenLieuP = 0;      	// longueur problematique des metadonnees       
	int lenNom, lenDate, lenLieu;						// longueur des metadonnees
	
	
	FILE *flot_data;
	
	flot_data = fopen(nom_fichier, "w");
	
	if(flot_data == NULL){
		return ERREUR_FICHIER;
		} else {
			lenNom = strlen(metadonnees.auteur);
			lenDate = strlen(metadonnees.dateCreation);
			lenLieu = strlen(metadonnees.lieuCreation);
			if(lenNom != lenNomP || lenDate != lenDateP || lenLieu != lenLieuP){
			fprintf(flot_data,"#%s;%s;%s\n", metadonnees.auteur, metadonnees.dateCreation, metadonnees.lieuCreation);
			}
			fprintf(flot_data,"P3\n");
			fprintf(flot_data,"%d %d\n", lignes, colonnes);
			fprintf(flot_data,"%d\n", maxval);
			
			for(n = 0; n < lignes; n++){
				for(m = 0; m < colonnes; m++){
					if(m != colonnes - 1){
						fprintf(flot_data, "%d ", matrice[n][m].valeurR);
						fprintf(flot_data, "%d ", matrice[n][m].valeurG);
						fprintf(flot_data, "%d ", matrice[n][m].valeurB);
					} else {
						fprintf(flot_data, "%d ", matrice[n][m].valeurR);
						fprintf(flot_data, "%d ", matrice[n][m].valeurG);
						fprintf(flot_data, "%d", matrice[n][m].valeurB);
					}
					
				}
				fprintf(flot_data,"\n");				
			}
		}
			
	fclose(flot_data);		
    return OK;
}

int ppm_copier(
				struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR],int lignes1,int colonnes1,
				struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR],int *p_lignes2,int *p_colonnes2){
	int n, m;
	
	if(lignes1 > MAX_HAUTEUR|| colonnes1 > MAX_LARGEUR){
		return ERREUR;
	} else {
		*p_lignes2 = lignes1;
		*p_colonnes2 = colonnes1;
		for(m = 0; m < lignes1; m++){
			for(n = 0; n < colonnes1; n++){
				matrice2[m][n].valeurR = matrice1[m][n].valeurR;
				matrice2[m][n].valeurG = matrice1[m][n].valeurG;
				matrice2[m][n].valeurB = matrice1[m][n].valeurB;
			}
		}
	}
	// s'assurer que les boucles ont complete leurs iterations
	if(n > colonnes1 && m > lignes1){
			return ERREUR;
	} else {
		return OK;
	}	
	return 0;
}

int ppm_sont_identiques(
						struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, int colonnes1,
						struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR], int lignes2, int colonnes2){
	int n, m; 
	
	if(lignes1 != lignes2|| colonnes1 != colonnes2 || lignes1 > MAX_HAUTEUR|| colonnes1 > MAX_LARGEUR ){
		return ERREUR;
	} else if (lignes1 != lignes2 || colonnes1 != colonnes2){
		return DIFFERENTES;
	} else {
		for(m = 0; m < lignes1; m++){
			for(n = 0; n < colonnes1; n++){
				if(matrice1[m][n].valeurR != matrice2[m][n].valeurR){
					return DIFFERENTES;
				}
				if(matrice1[m][n].valeurG != matrice2[m][n].valeurG){
					return DIFFERENTES;
				}
				if(matrice1[m][n].valeurB != matrice2[m][n].valeurB){
					return DIFFERENTES;
				}
			}
		}
	}
	return IDENTIQUES;
	return 0;
}

int ppm_pivoter90(struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR],
int *p_lignes, int *p_colonnes, int sens){
	return 0;
}