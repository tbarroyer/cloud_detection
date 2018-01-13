# include <gtk/gtk.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

# include "compute.h"
# include "math.h"

void train_kmean(int nb_images, GdkPixbuf** images, int nb_clust) {
  float clust[nb_clust];
  for (int i = 0; i < nb_clust; ++i)
    clust[i] = rand() % 256;

  int n_examples = 0;
  for (int i = 0; i < nb_images; ++i) {
    int nbCol  = gdk_pixbuf_get_width(images[i]); 
    int nbLine = gdk_pixbuf_get_height(images[i]);

    n_examples += nbCol * nbLine;
  }

  printf("%d datas\n", n_examples);

  int* data_clust = calloc(n_examples, sizeof(int));
  int* data_value = calloc(n_examples, sizeof(int));

  for (int epoch = 0; epoch < 20; ++epoch) {

    // Assign a cluster for each pixel
    int i_data = 0;
    for (int i = 0; i < nb_images; ++i) {
      int nbCol  = gdk_pixbuf_get_width(images[i]); 
      int nbLine = gdk_pixbuf_get_height(images[i]);
  
      int iNbPixelsTotal = nbCol * nbLine;
    
      guchar* img = gdk_pixbuf_get_pixels(images[i]);
    
      for (int iNumPix = 0; iNumPix < iNbPixelsTotal * 3; iNumPix = iNumPix + 3) {
        float m_dist = abs(*(img + iNumPix) - clust[0]);

        data_clust[i_data] = 0;
        data_value[i_data] = *(img + iNumPix);

        for (int cl = 1; cl < nb_clust; cl++) {
          float dist = abs(*(img + iNumPix) - clust[cl]);
          if (dist < m_dist) {
            data_clust[i_data] = cl;
            m_dist = dist;
          }
        }
        i_data++;
      }
    }

    // Compute mean for each cluster
    
    int size_cl[nb_clust];
    for (int i = 0; i < nb_clust; ++i) {
      size_cl[i] = 0;
      clust[i] = 0;
    }

    for (int i = 0; i < n_examples; ++i) {
      size_cl[data_clust[i]]++;
      clust[data_clust[i]] += data_value[i];
    }

    for (int i = 0; i < nb_clust; ++i)
    {
      clust[i] /= size_cl[i];
      printf("clust %d: %f\t", i, clust[i]);
    }
    printf("\n");
  }

  FILE* output = fopen("clusters.csv", "w");
  fprintf(output, "%d\n", nb_clust);
  for (int i = 0; i < nb_clust; ++i)
    fprintf(output, "%f\n", clust[i]);
  fclose(output);
}

GdkPixbuf** get_images(char* path, int* nb_images) {
  DIR           *d;
  struct dirent *dir;
  *nb_images = 0;
  d = opendir(path);

  if (!d) {
    printf("Wrong directory given\n");
    exit(1);
  }

  GdkPixbuf** out = NULL;

  if (d) {

    size_t size = 0;

    // Read all images in directory
    while ((dir = readdir(d)) != NULL) {
      if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")) {
        size++;
        out = realloc(out, size * sizeof(GdkPixbuf*));

        char file[1000];
        strcpy(file, path);
        strcat(file, "/");
        strcat(file, dir->d_name);

        printf("Reading file:  %s\n", file);

        GtkWidget *image  = gtk_image_new_from_file(file);
        if (image == NULL) {
          printf("Wrong file.\n");
          exit(1);
        }

        GdkPixbuf* pGdkPixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
        if (pGdkPixbuf == NULL) {
          printf("Wrong file.\n");
          exit(1);
        }

        // Greyscale all images

        int NbCol  = gdk_pixbuf_get_width(pGdkPixbuf);
        int NbLine = gdk_pixbuf_get_height(pGdkPixbuf);

        GdkPixbuf* pGdkPixbufImaRes = gdk_pixbuf_copy(pGdkPixbuf);

        guchar* pucImaOrig = gdk_pixbuf_get_pixels(pGdkPixbuf);
        guchar* pucImaRes   = gdk_pixbuf_get_pixels(pGdkPixbufImaRes);

        ComputeImage(pucImaOrig, NbLine, NbCol, pucImaRes);
      
        out[size - 1] = pGdkPixbufImaRes;
        

        *nb_images = *nb_images + 1;
      }
    }
    closedir(d);
  }

  return out;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("No directory given\n");
    return 1;
  }

  int nb = 0;
  GdkPixbuf** images = get_images(argv[1], &nb);

  train_kmean(nb, images, 2);

  printf("%d images read\n", nb);

  return 0;
}
