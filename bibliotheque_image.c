/********
Fichier: bibliotheque_images.c
Auteurs: Jérémy Cusson cusj3102
         Thomas St-Gelais stgt0901
Date: 03 octobre 2020
Description: Fichier de distribution pour GEN145.
********/
#include <string.h>
#include "bibliotheque_images.h"

// Fonction pôur les images noirs et blancs

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
				if(Caractere == -1)
				{
					return ERREUR_FORMAT;
				}
			}while(Caractere != ';' && Caractere != -1);
			compteur = 0;
			do
			{
				Caractere = fgetc(Donnees_entree);
				if(Caractere != ';' && Caractere != '\n')
				{
					p_metadonnees->lieuCreation[compteur] = Caractere;
					compteur++;
				}
			}while(Caractere != '\n' && Caractere != -1);
		}
		else
		{
			rewind(Donnees_entree);
		}
		fscanf(Donnees_entree, "%s", format);
		if(format[0] != 'P' && format[1] != '2')
		{
			return ERREUR_FORMAT;
		}
		
		fscanf(Donnees_entree, "%d %d", &colonnes, &lignes);
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
	
	if(metadonnees.auteur != NULL)
	{
		fprintf(Donnees_sortie, "#%s;%s;%s\n", metadonnees.auteur, metadonnees.dateCreation, metadonnees.lieuCreation);
	}
		
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

int pgm_copier(int matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, 
				int colonnes1, int matrice2[MAX_HAUTEUR][MAX_LARGEUR],
				 int *p_lignes2, int *p_colonnes2)
{
	int i = 0, j = 0;

	*p_lignes2 = lignes1;
	*p_colonnes2 = colonnes1;
	
	if (*p_lignes2 > MAX_HAUTEUR || *p_colonnes2 > MAX_LARGEUR)
	{
		return ERREUR;
	}

	for (i = 0; i < lignes1; i++)
		{
			for (j = 0; j < colonnes1; j++)
			{
				matrice2[i][j] = matrice1[i][j];
			}
		}

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

int pgm_couleur_preponderante(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
								int lignes, int colonnes)
{
	
	int i = 0, couleur_preponderante = 0;
	int histogramme[MAX_VALEUR + 1];
	pgm_creer_histogramme(matrice, lignes, colonnes, histogramme);
	
	for (i = 0; i<MAX_VALEUR; i++)
	{
		if (histogramme[i]>=histogramme[couleur_preponderante])
		{
			couleur_preponderante = i;
		}
	}
	
	if (couleur_preponderante<0 || couleur_preponderante>MAX_VALEUR)
	{
		return ERREUR;
	}
	
	return couleur_preponderante;
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

int pgm_extraire(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, int colonnes1, int lignes2, int colonnes2, int *p_lignes, int *p_colonnes)
{
	if(lignes1 >= *p_lignes || colonnes1 >= *p_colonnes || lignes2 >= *p_lignes || colonnes2 >= *p_colonnes)
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

int pgm_sont_identiques(int matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, int colonnes1, int matrice2[MAX_HAUTEUR][MAX_LARGEUR], int lignes2, int colonnes2)
{
	if(lignes1 != lignes2 || colonnes1 != colonnes2)
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

int pgm_pivoter90(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int *p_lignes, int *p_colonnes, int sens)
{
	int transValeur;
	int copieSens[MAX_HAUTEUR][MAX_LARGEUR];
	
	if(sens == SENS_HORAIRE)
	{
		for(int i = 0; i < *p_lignes; i++)
		{
			for(int j = 0; j < *p_colonnes; j++)
			{
				copieSens[j][*p_lignes - i - 1] = matrice[i][j];
			}
		}
	}
	else
	if(sens == SENS_ANTIHORAIRE)
	{
		for(int i = 0; i < *p_lignes; i++)
		{
			for(int j = 0; j < *p_colonnes; j++)
			{
				copieSens[*p_colonnes - j - 1][i] = matrice[i][j];
			}
		}
	}
	else
	{
		return ERREUR;
	}
	
	transValeur = *p_lignes;
	*p_lignes = *p_colonnes;
	*p_colonnes = transValeur;
	
	for(int i = 0; i < *p_lignes; i++)
	{
		for(int j =0; j < *p_colonnes; j++)
		{
			matrice[i][j] = copieSens[i][j];
		}
	}
	
	return OK;
}

// Fonction pour les images en couleur

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
				if(Caractere == -1)
				{
					return ERREUR_FORMAT;
				}
			}while(Caractere != ';' && Caractere != -1);
			compteur = 0;
			do
			{
				Caractere = fgetc(Donnees_entree);
				if(Caractere != ';' && Caractere != '\n')
				{
					p_metadonnees->lieuCreation[compteur] = Caractere;
					compteur++;
				}
			}while(Caractere != '\n' && Caractere != -1);
		}
		else
			rewind(Donnees_entree);
		fscanf(Donnees_entree, "%s", format2);
		if(format2[0] != 'P' || format2[1] != '3')
		{
			return ERREUR_FORMAT;
		}
		
		fscanf(Donnees_entree, "%d %d", &colonnes, &lignes);
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
	FILE *ppm_sortie = NULL;
	ppm_sortie = fopen(nom_fichier, "w");
	
	if(ppm_sortie == NULL)
	{
		return ERREUR_FICHIER;
	}
	if(lignes > MAX_LARGEUR || colonnes > MAX_HAUTEUR)
	{
		return ERREUR_TAILLE;
	}
	
	if(metadonnees.auteur != NULL)
		fprintf(ppm_sortie, "#%s;%s;%s\n", metadonnees.auteur, metadonnees.dateCreation, metadonnees.lieuCreation);
		
	fprintf(ppm_sortie, "%s\n", "P3");
	fprintf(ppm_sortie, "%d %d \n", colonnes, lignes);
	fprintf(ppm_sortie, "%d\n", maxval);
	
	for(int i = 0; i < lignes; i++)
	{
		for(int j = 0; j < colonnes; j++)
		{
			fprintf(ppm_sortie, "%d %d %d ", matrice[i][j].valeurR, matrice[i][j].valeurG, matrice[i][j].valeurB);
		}
	}
	
	fclose(ppm_sortie);
	
	return OK;
}

int ppm_copier(struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, int colonnes1, struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR], int *p_lignes2, int *p_colonnes2)
{
	*p_lignes2 = lignes1;
	*p_colonnes2 = colonnes1;
	
	for(int i = 0; i < MAX_HAUTEUR; i++)
	{
		for(int j = 0; j < MAX_LARGEUR; j++)
		{
			matrice2[i][j].valeurR = 0;
			matrice2[i][j].valeurG = 0;
			matrice2[i][j].valeurB = 0;
		}
	}
	
	for(int i = 0; i < lignes1; i++)
	{
		for(int j = 0; j < colonnes1; j++)
		{
			matrice2[i][j].valeurR = matrice1[i][j].valeurR;
			matrice2[i][j].valeurG = matrice1[i][j].valeurG;
			matrice2[i][j].valeurB = matrice1[i][j].valeurB;
		}
	}
	
	return OK;
}

int ppm_sont_identiques(struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, int colonnes1, struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR], int lignes2, int colonnes2)
{
	if(lignes1 != lignes2 || colonnes1 != colonnes2)
	{
		return ERREUR;
	}
	
	for(int i = 0; i < lignes1; i++)
	{
		for(int j = 0; j < colonnes1; j++)
		{
			if(matrice1[i][j].valeurR != matrice2[i][j].valeurR || matrice1[i][j].valeurG != matrice2[i][j].valeurG|| matrice1[i][j].valeurB != matrice2[i][j].valeurB)
			{
				return DIFFERENTES;
			}
		}
	}
	return IDENTIQUES;
}

int ppm_pivoter90(struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], int *p_lignes, int *p_colonnes, int sens)
{
	int transValeur;
	struct RGB copieSens[MAX_HAUTEUR][MAX_LARGEUR];
	
	if(sens == SENS_HORAIRE)
	{
		for(int i = 0; i < *p_lignes; i++)
		{
			for(int j = 0; j < *p_colonnes; j++)
			{
				copieSens[j][*p_lignes - i - 1].valeurR = matrice[i][j].valeurR;
				copieSens[j][*p_lignes - i - 1].valeurG = matrice[i][j].valeurG;
				copieSens[j][*p_lignes - i - 1].valeurB = matrice[i][j].valeurB;
			}
		}
	}
	else
	if(sens == SENS_ANTIHORAIRE)
	{
		for(int i = 0; i < *p_lignes; i++)
		{
			for(int j = 0; j < *p_colonnes; j++)
			{
				copieSens[*p_colonnes - j - 1][i].valeurR = matrice[i][j].valeurR;
				copieSens[*p_colonnes - j - 1][i].valeurG = matrice[i][j].valeurG;
				copieSens[*p_colonnes - j - 1][i].valeurB = matrice[i][j].valeurB;
			}
		}
	}
	else
	{
		return ERREUR;
	}
	
	transValeur = *p_lignes;
	*p_lignes = *p_colonnes;
	*p_colonnes = transValeur;
	
	for(int i = 0; i < *p_lignes; i++)
	{
		for(int j =0; j < *p_colonnes; j++)
		{
			matrice[i][j].valeurR = copieSens[i][j].valeurR;
			matrice[i][j].valeurG = copieSens[i][j].valeurG;
			matrice[i][j].valeurB = copieSens[i][j].valeurB;
		}
	}
	
	return OK;
}
