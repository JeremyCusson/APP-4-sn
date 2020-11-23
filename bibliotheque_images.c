/********
Fichier: bibliotheque_images.c
Auteurs: Domingo Palao Munoz
         Charles-Antoine Brunet
Date: 25 octobre 2018
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

    FILE *Donnees_entree;

    strcpy(nom_fichier, "TEST.pgm");

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
	int i = 0, j = 0;
	FILE *flot_sortie;
	
	strcpy(nom_fichier, "ecrire.pgm");
	flot_sortie = fopen(nom_fichier, "w");
	
	if (flot_sortie == NULL)
	{
		printf("Erreur d'ouverture de fichier.\n");
		return ERREUR;
	}
	else
	{
		fprintf(flot_sortie, "#%s;%s;%s", metadonnees.auteur, metadonnees.dateCreation, metadonnees.lieuCreation);
		fprintf(flot_sortie, "P2\n");
		fprintf(flot_sortie, "%d %d\n", lignes, colonnes);
		fprintf(flot_sortie, "%d\n", maxval);
		for (i = 0; i<lignes; i++)
		{
			for (j = 0; j<colonnes; j++)
			{
				fprintf(flot_sortie, "%d ", matrice[i][j]);
			}
		}
	}
	
	if (flot_sortie != NULL)
	{
		 fclose(flot_sortie);
	}
	return OK;
}

int pgm_copier(int matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, 
				int colonnes1, int matrice2[MAX_HAUTEUR][MAX_LARGEUR],
				 int *p_lignes2, int *p_colonnes2)
{
	int i = 0, j = 0;
	
	p_lignes2 = &i;
	p_colonnes2 = &j;
	
	for (i = 0; i<lignes1; i++)
		{
			for (j = 0; j<colonnes1; j++)
			{
				matrice2[*p_lignes2][*p_colonnes2] = matrice1[i][j];
			}
		}
	if (*p_lignes2 > MAX_HAUTEUR || *p_colonnes2 > MAX_LARGEUR)
	{
		return ERREUR;
	}
	
	return OK;
}


int pgm_creer_histogramme(int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
							int lignes, int colonnes, 
							int histogramme[MAX_VALEUR+1])
{
    int compteurPixel = 0;

    for(int i = 0; i < MAX_VALEUR + 1; i++)
    {
        for(int j = 0; j < lignes; j++)
        {
            for(int k = 0; k < colonnes; k++)
            {
                if(matrice[j][k] == i)
                {
                    compteurPixel++;
                }
            }
        }
        histogramme[i] = compteurPixel;
        compteurPixel = 0;
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

/*int pgm_couleur_preponderante(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
								int lignes, int colonnes)
{
	retour = pgm_creer_histogramme(image1, lignes1, colonnes1, histogramme);
	
	int i = 0, couleur_preponderante = 0;
	
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
	
	printf("%d", couleur_preponderante);
	return OK;
}*/

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

/*int pgm_pivoter90(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int *p_lignes, 
					int *p_colonnes, int sens)
{
	if (sens == SENS_HORAIRE)
	{
		
	}
	else
	if (sens == SENS_ANTIHORAIRE
	{
		
	}
	else
	if (sens != SENS_HORAIRE && SENS != SENS_ANTIHORAIRE)
	{
		return ERREUR;
	}
	
	return OK;
}*/
