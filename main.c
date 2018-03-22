#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int min(int a, int b) {
    if (a < b)
      return a;
    return b;
}

Image gray_scale(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int average = img.pixel[i][j][0] +
                        img.pixel[i][j][1] +
                        img.pixel[i][j][2];
            average /= 3;
            img.pixel[i][j][0] = average;
            img.pixel[i][j][1] = average;
            img.pixel[i][j][2] = average;
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
                    average.red += img.pixel[x][y][0];
                    average.green += img.pixel[x][y][1];
                    average.blue += img.pixel[x][y][2];
                }
            }

            average.red /= size * size;
            average.green /= size * size;
            average.blue /= size * size;

            img.pixel[i][j][0] = average.red;
            img.pixel[i][j][1] = average.green;
            img.pixel[i][j][2] = average.blue;
        }
    }

    return img;
}

Image rotate90right(Image img) {
    Image rotated;

    for (unsigned int i = 0, y = 0; i < img.height; ++i, ++y) {
        for (int j = img.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j][0] = img.pixel[x][y][0];
            rotated.pixel[i][j][1] = img.pixel[x][y][1];
            rotated.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotated;
}

Image invert_colors(Image img){
    Image inverted_colors;

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            inverted_colors.pixel[i][j][0] = 255 - inverted_colors.pixel[i][j][0];
            inverted_colors.pixel[i][j][1] = 255 - inverted_colors.pixel[i][j][1];
            inverted_colors.pixel[i][j][2] = 255 - inverted_colors.pixel[i][j][2];
        }
    }
    return inverted_colors;
}

Image cut_image(Image img, int x, int y, int width, int height) {
    Image cutted;

    cutted.width = width;
    cutted.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cutted.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cutted.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cutted.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }

    return cutted;
}

Image sepia_filter(Image img) {

    for (unsigned int x = 0; x < img.height; ++x) {
        for (unsigned int j = 0; j < img.width; ++j) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[x][j][0];
            pixel[1] = img.pixel[x][j][1];
            pixel[2] = img.pixel[x][j][2];

            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int min_r = (255 >  p) ? p : 255;
            img.pixel[x][j][0] = min_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            min_r = (255 >  p) ? p : 255;
            img.pixel[x][j][1] = min_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            min_r = (255 >  p) ? p : 255;
            img.pixel[x][j][2] = min_r;
        }
    }

    return img;
}

Image read_all_pixels(Image img){
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    return img;
}

Image mirror_image(Image img){
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
            aux1.red = img.pixel[i2][j][0];
            aux1.green = img.pixel[i2][j][1];
            aux1.blue = img.pixel[i2][j][2];

            img.pixel[i2][j][0] = img.pixel[x][y][0];
            img.pixel[i2][j][1] = img.pixel[x][y][1];
            img.pixel[i2][j][2] = img.pixel[x][y][2];

            img.pixel[x][y][0] = aux1.red;
            img.pixel[x][y][1] = aux1.green;
            img.pixel[x][y][2] = aux1.blue;
        }
    }
    return img;
}

void print_pixels(Image img){
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][0],
                                   img.pixel[i][j][1],
                                   img.pixel[i][j][2]);
        }
        printf("\n");
    }
}

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
            case 7: { // Cut Image
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cut_image(img, x, y, width, height);
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
