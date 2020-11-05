/********
Fichier: bibliotheque_images.c
Auteurs: Jérémy Cusson
         Thomas St_Gelais
Date: 03 novembre 2020
Description: Fichier de distribution pour GEN145.
********/
#include <string.h>
#include "bibliotheque_images.h"

int pgm_lire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
             int *p_lignes, int *p_colonnes, 
             int *p_maxval, struct MetaData *p_metadonnees)
{
	int Caractere = 0;
	int compteur = 0;
	int lignes = 0;
	int colonnes = 0;
	int maxval = 0;
	char format[2];
	
	FILE *Donnees_entree = NULL;
	
	Donnees_entree = fopen(nom_fichier, "r");
	
	if(Donnees_entree == NULL)
	{
		return ERREUR_FICHIER;
		fclose(Donnees_entree);
	}
	else
	if(Donnees_entree != NULL)
	{	
		if(fgetc(Donnees_entree) == '#')
		{
			while(Caractere != ';')
			{
				Caractere = fgetc(Donnees_entree);
				if(Caractere != ';' && Caractere != '#')
				{
					p_metadonnees->auteur[compteur] = Caractere;
					compteur++;
				}
				else
				if(Caractere == '\n')
				{
					return ERREUR_FORMAT;
				}
				
			}
			compteur = 0;
			do
			{
				Caractere = fgetc(Donnees_entree);
				if(Caractere != ';')
				{
					p_metadonnees->dateCreation[compteur] = Caractere;
					compteur++;
				}
				else
				if(Caractere == '\n')
				{
					return ERREUR_FORMAT;
				}
			}while(Caractere != ';');
			compteur = 0;
			do
			{
				Caractere = fgetc(Donnees_entree);
				if(Caractere != ';')
				{
					p_metadonnees->lieuCreation[compteur] = Caractere;
					compteur++;
				}
			}while(Caractere != '\n');
		}
		else
			rewind(Donnees_entree);
		fscanf(Donnees_entree, "%s", format);
		if(format[0] != 'P' && format[1] != '2')
		{
			return ERREUR_FORMAT;
		}
		
		fscanf(Donnees_entree, "%d %d", &lignes, &colonnes);
		*p_lignes = lignes;
		*p_colonnes = colonnes;
		
		fscanf(Donnees_entree, "%d", &maxval);
		*p_maxval = maxval;
		
		if(*p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR || *p_maxval > MAX_VALEUR)
		{
			return ERREUR_TAILLE;
		}
		
		for(int i = 0; i < *p_lignes; i++)
		{
			for(int j = 0; j < *p_colonnes; j++)
			{
				fscanf(Donnees_entree, "%d ", &matrice[i][j]);
			}
		}
	}
	
	fclose(Donnees_entree);
	
    return OK;
}

int pgm_ecrire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
               int lignes, int colonnes, 
               int maxval, struct MetaData metadonnees)
{
    FILE *Donnees_sortie = fopen(nom_fichier, "w");
	
	if(Donnees_sortie == NULL)
	{
		return ERREUR_FICHIER;
	}
	if(metadonnees.auteur == NULL)
		fprintf(Donnees_sortie, "#%s;%s;%s\n", metadonnees.auteur, metadonnees.dateCreation, metadonnees.lieuCreation);
		
	fprintf(Donnees_sortie, "%s\n", "P2");
	fprintf(Donnees_sortie, "%d %d\n", lignes, colonnes);
	fprintf(Donnees_sortie, "%d\n", maxval);
	
	for(int i = 0; i < lignes; i++)
	{
		for(int j = 0; j < colonnes; j++)
		{
			fprintf(Donnees_sortie, "%d ", matrice[i][j]);
		}
		fprintf(Donnees_sortie, "\n");
	}
	
	fclose(Donnees_sortie);
	
	return OK;
}

int pgm_creer_histogramme(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int lignes, int colonnes, int histogramme[MAX_VALEUR+1])
{
	int pixel;
	
	for(int i = 0; i < MAX_VALEUR + 1; i++)
	{
		histogramme[i] = 0;
	}
	
	for(int i = 0; i < lignes; i++)
	{
		for(int j = 0; j < colonnes; j++)
		{
			pixel = matrice[i][j];
			histogramme[pixel] += 1;
		}
	}
	
	for(int i = 0; i < MAX_VALEUR + 1; i++)
	{
		if(histogramme[i] > MAX_VALEUR || histogramme[i] < 0)
		{
			return ERREUR;
		}
	}
	
	return OK;
}

int pgm_creer_negatif(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int lignes,
                        int colonnes, int maxval)
{
    int valeurPixel = 0, i = 0, j = 0;

    for (i = 0; i<lignes; i++)
    {
        for (j = 0; j<colonnes; j++)
        {
            valeurPixel = matrice[i][j];
            valeurPixel = maxval - valeurPixel;
            matrice[i][j] = valeurPixel;
            if (valeurPixel<0 || valeurPixel>maxval)
            {
                return ERREUR;
            }
        }
    }

    return OK;
}

int pgm_sont_identiques(int matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, int colonnes1, int matrice2[MAX_HAUTEUR][MAX_LARGEUR], int lignes2, int colonnes2)
{
	if(lignes1 != lignes2 && colonnes1 != colonnes2)
	{
		return ERREUR;
	}
	
	for(int i = 0; i < lignes1; i++)
	{
		for(int j = 0; j < colonnes1; j++)
		{
			if(matrice1[i][j] != matrice2[i][j])
			{
				return DIFFERENTES;
			}
		}
	}
	return IDENTIQUES;
}

int pgm_eclaircir_noircir(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int lignes, int colonnes, int maxval, int valeur)
{
	for(int i = 0; i < lignes; i++)
	{
		for(int j = 0; j < colonnes; j++)
		{
			if((matrice[i][j] + valeur) > maxval)
			{
				matrice[i][j] = maxval;
			}
			else
			if((matrice[i][j] + valeur) < 0)
			{
				matrice[i][j] = 0;
			}
			else
			{
				matrice[i][j] += valeur;
			}
		}
	}
	return OK;
}

int pgm_extraire(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, int colonnes1, int lignes2, int colonnes2, int *p_lignes, int *p_colonnes)
{
	if(lignes1 > *p_lignes || colonnes1 > *p_colonnes || lignes2 > *p_lignes || colonnes2 > *p_colonnes)
	{
		return ERREUR_TAILLE;
	}
	
	for(int i = lignes1; i <= lignes2; i++)
	{
		for(int j = colonnes1; j <= colonnes2; j++)
		{
			matrice[i - lignes1][j - colonnes1] = matrice[i][j];
		}
	}
	
	*p_lignes = lignes2 - lignes1 + 1;
	*p_colonnes = colonnes2 - colonnes1 + 1;
	
	return OK;
}

int ppm_lire(char nom_fichier[], struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], int *p_lignes, int *p_colonnes, int *p_maxval, struct MetaData *p_metadonnees)
{
	int Caractere = 0;
	int compteur = 0;
	int lignes = 0;
	int colonnes = 0;
	int maxval = 0;
	char format2[2];
	
	FILE *Donnees_entree = NULL;
	
	Donnees_entree = fopen(nom_fichier, "r");
	
	if(Donnees_entree == NULL)
	{
		return ERREUR_FICHIER;
		fclose(Donnees_entree);
	}
	else
	if(Donnees_entree != NULL)
	{	
		if(fgetc(Donnees_entree) == '#')
		{
			while(Caractere != ';')
			{
				Caractere = fgetc(Donnees_entree);
				if(Caractere != ';' && Caractere != '#')
				{
					p_metadonnees->auteur[compteur] = Caractere;
					compteur++;
				}
				else
				if(Caractere == '\n')
				{
					return ERREUR_FORMAT;
				}
				
			}
			compteur = 0;
			do
			{
				Caractere = fgetc(Donnees_entree);
				if(Caractere != ';')
				{
					p_metadonnees->dateCreation[compteur] = Caractere;
					compteur++;
				}
				else
				if(Caractere == '\n')
				{
					return ERREUR_FORMAT;
				}
			}while(Caractere != ';');
			compteur = 0;
			do
			{
				Caractere = fgetc(Donnees_entree);
				if(Caractere != ';')
				{
					p_metadonnees->lieuCreation[compteur] = Caractere;
					compteur++;
				}
			}while(Caractere != '\n');
		}
		else
			rewind(Donnees_entree);
		fscanf(Donnees_entree, "%s", format2);
		if(format2[0] != 'P' || format2[1] != '3')
		{
			return ERREUR_FORMAT;
		}
		
		fscanf(Donnees_entree, "%d %d", &lignes, &colonnes);
		*p_lignes = lignes;
		*p_colonnes = colonnes;
		
		fscanf(Donnees_entree, "%d", &maxval);
		*p_maxval = maxval;
		
		if(*p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR || *p_maxval > MAX_VALEUR)
		{
			return ERREUR_TAILLE;
		}
		
		for(int i = 0; i < *p_lignes; i++)
		{
			for(int j = 0; j < *p_colonnes; j++)
			{
				fscanf(Donnees_entree, "%d %d %d ", &matrice[i][j].valeurR, &matrice[i][j].valeurG, &matrice[i][j].valeurB);
			}
		}
	}
	
	fclose(Donnees_entree);
	
	return OK;
}

int ppm_ecrire(char nom_fichier[], struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], int lignes, int colonnes, int maxval, struct MetaData metadonnees)
{
	FILE *Donnees_sortie = fopen(nom_fichier, "w");
	
	if(Donnees_sortie == NULL)
	{
		return ERREUR_FICHIER;
	}
	if(metadonnees.auteur == NULL)
		fprintf(Donnees_sortie, "#%s;%s;%s\n", metadonnees.auteur, metadonnees.dateCreation, metadonnees.lieuCreation);
		
	fprintf(Donnees_sortie, "%s\n", "P3");
	fprintf(Donnees_sortie, "%d %d\n", lignes, colonnes);
	fprintf(Donnees_sortie, "%d\n", maxval);
	
	for(int i = 0; i < lignes; i++)
	{
		for(int j = 0; j < colonnes; j++)
		{
			fprintf(Donnees_sortie, "%d %d %d ", matrice[i][j].valeurR, matrice[i][j].valeurG, matrice[i][j].valeurB);
		}
		fprintf(Donnees_sortie, "\n");
	}
	
	fclose(Donnees_sortie);
	
	return OK;
}
