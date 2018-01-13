#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>

/*******************************************************
IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
DES FONCTIONS
*******************************************************/


/*---------------------------------------
  Proto: 

  
  But: 

  Entrees: 
          --->le tableau des valeurs des pixels de l'image d'origine
	      (les lignes sont mises les unes à la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes à la suite des autres)


  Sortie:

  Rem: 

  Voir aussi:

  ---------------------------------------*/
void ComputeImage(guchar *pucImaOrig, 
		  int NbLine,
		  int NbCol, 
		  guchar *pucImaRes)
{
  int iNbPixelsTotal, iNumPix;
  int iNumChannel, iNbChannels=3; /* on travaille sur des images couleurs*/
  guchar ucMeanPix;

  printf("Segmentation de l'image.... A vous!\n");
  
  iNbPixelsTotal=NbCol*NbLine;
  for(iNumPix=0; iNumPix<iNbPixelsTotal*iNbChannels; iNumPix=iNumPix+iNbChannels) {
    /*moyenne sur les composantes RVB */
    ucMeanPix = (unsigned char)((*(pucImaOrig+iNumPix) + *(pucImaOrig+iNumPix+1) + *(pucImaOrig+iNumPix+2))/3);

    /* sauvegarde du resultat */
    for (iNumChannel=0; iNumChannel<iNbChannels; iNumChannel++)
    {
      *(pucImaRes+iNumPix+iNumChannel) = ucMeanPix;
    }
  }
}

void ClustImage(guchar *pucImaOrig, 
		  int NbLine,
		  int NbCol, 
		  guchar *pucImaRes)
{
  FILE* input = fopen("clusters.csv", "r");

  int nb_clusts = 0;
  fscanf(input, "%d\n", &nb_clusts);

  float clusts[nb_clusts];

  for (int i = 0; i < nb_clusts; ++i)
  {
    clusts[i] = 42.9;
    fscanf(input, "%f\n", &(clusts[i]));
    printf("%f\n", clusts[i]);
  }

  fclose(input);

  int iNbPixelsTotal, iNumPix;
  int iNumChannel, iNbChannels=3; /* on travaille sur des images couleurs*/
  guchar ucMeanPix;

  printf("Assignations aux clusters!\n");
  
  iNbPixelsTotal=NbCol*NbLine;
  for(iNumPix=0; iNumPix<iNbPixelsTotal*iNbChannels; iNumPix=iNumPix+iNbChannels) {
    /*moyenne sur les composantes RVB */
    ucMeanPix = (unsigned char)((*(pucImaOrig+iNumPix) + *(pucImaOrig+iNumPix+1) + *(pucImaOrig+iNumPix+2))/3);
    float dist = abs(ucMeanPix - clusts[0]);
    int min = 0;

    for (int c = 1; c < nb_clusts; c++)
    {
      float dist_ = abs(ucMeanPix - clusts[c]);
      if (dist_ < dist)
      {
        dist = dist_;
        min = c;
      }
    }

    /* sauvegarde du resultat */
    for (iNumChannel=0; iNumChannel<iNbChannels; iNumChannel++)
    {
      *(pucImaRes+iNumPix+iNumChannel) = clusts[min];
    }
  }
}

