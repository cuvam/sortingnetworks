#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    ////////////////////////////////////////
    // Create random array

    int arr[64];
    for (int i = 0; i < 64; i++)
        arr[i] = rand() % 100;

    ////////////////////////////////////////
    // Obtain connections
    
    // Input .txt file must be in the format of "[(n,n),(n,n),...,(n,n)]" for each line
    // Such as the examples shown at https://bertdobbelaere.github.io/sorting_networks_extended.html
    FILE *fp = fopen("net64.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open file\n");
        return 1;
    }
    int layersMax = 16;
    int numLayers = 0;
    int **layers = malloc(sizeof(int*)*layersMax);  
    int *layerSizes = malloc(sizeof(int)*layersMax);
    int *layerMaxes = malloc(sizeof(int)*layersMax);
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c != '[') continue;
        
        if (numLayers == layersMax) {
            layersMax += 32;
            layers = realloc(layers, sizeof(int*)*layersMax);
            layerSizes = realloc(layerSizes, sizeof(int)*layersMax);
            layerMaxes = realloc(layerMaxes, sizeof(int)*layersMax);
        }

        layerMaxes[numLayers] = 16;
        layerSizes[numLayers] = 0;
        layers[numLayers] = malloc(sizeof(int)*2*layerMaxes[numLayers]);
        while (c != ']' && c != EOF) {
            if (layerSizes[numLayers] == layerMaxes[numLayers]) {
                layerMaxes[numLayers] += 16;
                layers[numLayers] = realloc(layers[numLayers], sizeof(int)*2*layerMaxes[numLayers]);
            }
            c = fgetc(fp); // c is now (
            char numbuf[256] = {0};
            int numbuflen = 0;
            while (isdigit((c = fgetc(fp))) && numbuflen < 255) {
                numbuf[numbuflen++] = c;
            }
            // c is now ,
            numbuf[numbuflen] = '\0';
            int a = atoi(numbuf);
            numbuflen = 0;
            while (isdigit((c = fgetc(fp))) && numbuflen < 255) {
                numbuf[numbuflen++] = c;
            }
            // c is now )
            numbuf[numbuflen] = '\0';
            int b = atoi(numbuf);
            if (a >= 64 || b >= 64) { fclose(fp); printf("Error: Invalid index\n"); return 1; }
            layers[numLayers][layerSizes[numLayers]*2] = a;
            layers[numLayers][(layerSizes[numLayers]*2) + 1] = b;
            c = fgetc(fp); // c is now either ',' (continue reading) or ']' (end of layer) 

            layerSizes[numLayers]++;
        }
        numLayers++;
    }
    fclose(fp);

    //////////////////////////////////
    // Perform compare-swaps
    
    for (int layer = 0; layer < numLayers; layer++) {
        for (int pair = 0; pair < layerSizes[layer]; pair++) {
            /* printf("(%i,%i) ", layers[layer][pair*2], layers[layer][pair*2+1]); */
            int a = layers[layer][pair*2];
            int b = layers[layer][pair*2+1];
            if (arr[a] > arr[b]) {
                int t = arr[a]; 
                arr[a] = arr[b];
                arr[b] = t;
            }
        }
    }

    ////////////////////////////////////////
    // Free pointers 
    for (int i = 0; i < numLayers; i++) {
        free(layers[i]); // Indices up to maxLayers weren't alloced and don't need to be freed
    }
    free(layers);
    free(layerMaxes);
    free(layerSizes);

    ////////////////////////////////////////
    // Output

    for (int i = 0; i < 64; i++) {
        printf("%i ", arr[i]);
    }
    printf("\n");
}