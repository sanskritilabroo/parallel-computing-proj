#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // Include time.h for measuring execution time

unsigned char* read_pgm(const char* filename, int* width, int* height) {
    FILE* f = fopen(filename, "rb"); // Open the file in binary mode
    if (!f) {
        perror("File open failed"); 
        exit(1);
    }

    char magic[3]; 
    fscanf(f, "%2s", magic); 

    if (magic[0] != 'P' || magic[1] != '5') {
        printf("Unsupported Format: %s. Only P5\n", magic);
        fclose(f);
        exit(1);
    }

    int maxval;
    fscanf(f, "%d %d %d", width, height, &maxval); // Read width, height, and max pixel value
    fgetc(f); 

    int size = (*width) * (*height); // Calculate the total number of pixels
    unsigned char* data = (unsigned char*)malloc(size); 

    fread(data, 1, size, f); 
    fclose(f); 

    return data; 
}

// Function to perform 2x2 average pooling on an image
void average_pooling(unsigned char* img, unsigned char* res, int width, int height) {
    int new_width = width / 2;   
    int new_height = height / 2; 

    // Loop through the new smaller image dimensions
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            int sum = 0;
            // Sum the values of the 2x2 block in the original image
            for (int dy = 0; dy < 2; dy++) {
                for (int dx = 0; dx < 2; dx++) {
                    sum += img[(2 * y + dy) * width + (2 * x + dx)];
                }
            }
            res[y * new_width + x] = sum / 4; // Compute the average and store it
        }
    }
}

int main() {
    const char* fname = "Mona_Lisa.pgm";

    int width, height;
    unsigned char* img = read_pgm(fname, &width, &height); 

    int new_width = width / 2;   
    int new_height = height / 2; 
    unsigned char* res = (unsigned char*)malloc(new_width * new_height); 

    // Start timing
    clock_t start = clock();

    // Perform Average Pooling
    average_pooling(img, res, width, height);

    // Stop timing
    clock_t end = clock();

    // Calculate time in milliseconds
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    printf("CPU Execution Time: %.4f ms\n", time_taken);

    
    FILE* out = fopen("Mona_Lisa_pooled.pgm", "wb");
    fprintf(out, "P5\n%d %d\n255\n", new_width, new_height); 
    fwrite(res, 1, new_width * new_height, out); 
    fclose(out);

    free(img);
    free(res); 

    printf("Saved pooled image as Mona_Lisa_pooled.pgm\n"); 
    return 0;
}
