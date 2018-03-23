#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height]

    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
} Image;

// Prototypes

int max(int num1, int num2);
int min(int num1, int num2);
Image gray_scale(Image img);
Image blur(Image img);
Image rotate90right(Image img);
Image invert_colors(Image img);
Image cut_image(Image img);
Image sepia_filter(Image img);
Image read_all_pixels(Image img);
Image mirror_image(Image img);
void print_pixels(Image img);
void new_colors_values(Pixel *img, Pixel *value);
int calc_average (Image *img, int i, int j);

int main() {
    Image img;

    // read type of image
    char image_type[4];
    scanf("%s", image_type);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    img = read_all_pixels(img);

    int n_options;
    scanf("%d", &n_options);

    for(int i = 0; i < n_options; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: {
                img = gray_scale(img);
                break;
            }
            case 2: {
                img = sepia_filter(img);
                break;
            }
            case 3: {
                img = blur(img);
                break;
            }
            case 4: { // Rotate image
                int repetitions = 0;
                scanf("%d", &repetitions);
                repetitions %= 4;
                for (int j = 0; j < repetitions; ++j) {
                    img = rotate90right(img);
                }
                // img = rotate_image(img) --> segmentation error;
                break;
            }
            case 5: {
                img = mirror_image(img);
                break;
            }
            case 6: {
                img = invert_colors(img);
                break;
            }
            case 7: {
                img = cut_image(img);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");

    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    print_pixels(img);

    return 0;
}

int max(int num1, int num2) {
    if (num1 > num2)
        return num1;
    return num2;
}

int min(int num1, int num2) {
    if (num1 < num2)
      return num1;
    return num2;
}

void new_colors_values(Pixel *img, Pixel *value) {
    img->red = value->red;
    img->green = value->green;
    img->blue = value->blue;
}

int calc_average (Image *img, int i, int j){
    int average = img->pixel[i][j].red +
                img->pixel[i][j].green +
                img->pixel[i][j].blue;
      average /= 3;
      return average;
}

Image gray_scale(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int average = calc_average(&img, i, j);
            img.pixel[i][j].red = average;
            img.pixel[i][j].green = average;
            img.pixel[i][j].blue = average;
        }
    }

    return img;
}

Image blur(Image img) {
    int size = 0;
    scanf("%d", &size);

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel average = {0, 0, 0};

            int min_height = min(img.height - 1, i + size/2);
            int min_width = min(img.width - 1, j + size/2);

            for(int x = max(0, i - size/2); x <= min_height; ++x) {
                for(int y = max(0, j - size/2); y <= min_width; ++y) {
                    average.red += img.pixel[x][y].red;
                    average.green += img.pixel[x][y].green;
                    average.blue += img.pixel[x][y].blue;
                }
            }

            average.red /= size * size;
            average.green /= size * size;
            average.blue /= size * size;

            new_colors_values(&img.pixel[i][j], &average);
        }
    }

    return img;
}

Image rotate90right(Image img) {
    Image rotated;

    for (unsigned int i = 0, y = 0; i < img.height; ++i, ++y) {
        for (int j = img.width - 1, x = 0; j >= 0; --j, ++x) {
            new_colors_values(&rotated.pixel[i][j], &img.pixel[x][y]);
        }
    }

    return rotated;
}

Image invert_colors(Image img) {
    Image inverted_colors;

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            inverted_colors.pixel[i][j].red = 255 - inverted_colors.pixel[i][j].red;
            inverted_colors.pixel[i][j].green = 255 - inverted_colors.pixel[i][j].green;
            inverted_colors.pixel[i][j].blue = 255 - inverted_colors.pixel[i][j].blue;
        }
    }
    return inverted_colors;
}

Image cut_image(Image img) {
    Image cutted;

    int x, y;
    scanf("%d %d", &x, &y);

    int width, height;
    scanf("%d %d", &width, &height);

    cutted.width = width;
    cutted.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            new_colors_values(&cutted.pixel[i][j], &img.pixel[i + y][j + x]);
        }
    }

    return cutted;
}

Image sepia_filter(Image img) {

    for (unsigned int x = 0; x < img.height; ++x) {
        for (unsigned int j = 0; j < img.width; ++j) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[x][j].red;
            pixel[1] = img.pixel[x][j].green;
            pixel[2] = img.pixel[x][j].blue;

            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int min_r = min(255,  p);
            img.pixel[x][j].red = min_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            min_r = min(255,  p);
            img.pixel[x][j].green = min_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            min_r = min(255,  p);
            img.pixel[x][j].blue = min_r;
        }
    }

    return img;
}

Image read_all_pixels(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].red,
                                 &img.pixel[i][j].green,
                                 &img.pixel[i][j].blue);

        }
    }

    return img;
}

Image mirror_image(Image img) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = img.width, height = img.height;

    if (horizontal == 1) width /= 2;
    else height /= 2;

    for (int i2 = 0; i2 < height; ++i2) {
        for (int j = 0; j < width; ++j) {
            int x = i2, y = j;

            if (horizontal == 1) y = img.width - 1 - j;
            else x = img.height - 1 - i2;

            Pixel aux1;

            new_colors_values(&aux1, &img.pixel[i2][j]);
            new_colors_values(&img.pixel[i2][j], &img.pixel[x][y]);
            new_colors_values(&img.pixel[x][y], &aux1);
        }
    }
    return img;
}

void print_pixels(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].red,
                                   img.pixel[i][j].green,
                                   img.pixel[i][j].blue);
        }
        printf("\n");
    }
}

// Image rotate_image(Image img){
//     int repetitions = 0;
//
//     scanf("%d", &repetitions);
//     repetitions %= 4;
//
//     for (int j = 0; j < repetitions; ++j) {
//         img = rotate90right(img);
//     }
//     return img;

// }
