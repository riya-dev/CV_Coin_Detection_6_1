// cd project6
// cd part1
// g++ -std=c++11 -o l061  -Wall l061.cpp
// ./l061
// convert -compress none circle.png circle.ppm

#include <iostream>
#include <fstream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

int width;
int height;
bool** visited;

int** inbounds(int i, int j, int** votes) {
    if (i >= 0 && i < height)
        if (j >= 0 && j < width)
            votes[i][j] += 1;
    return votes;
}

void notoutofbounds(int** array, int i, int j) {
    if (i > 0 && i < height)
        if (j > 0 && j < width)
            array[i][j] = 3;
}

int** linecase1(int x1, int y1, int x2, int y2, int** votes) { // x1 < x2, y1 < y2, dx < dy = default bresenham's
    //std::cout << x1 << y1 << x2 << y2 << "\n";

    int dx = x2 - x1; // |
    int dy = y2 - y1; // _
    int e = dx - dy;
    int i = x1;

    for (int j = y1; j <= y2; j++) {
        //arr[i][j] = 0; // illuminate
        votes = inbounds(i, j, votes);
        if (e >= 0) {
            i++;
            e -= dy;
        }
        e += dx;
    }
    return votes;
}

int** linecase2(int x1, int y1, int x2, int y2, int** votes) { // x1 < x2 && y1 > y2 && dx > dy
    //std::cout << x1 << y1 << x2 << y2 << "\n";

    int dx = x1 - x2; // |
    int dy = y2 - y1; // _
    int e = dx - dy;
    int i = x1;

    for (int j = y1; j <= y2; j++) {
        //arr[i][j] = 0; // illuminate
        votes = inbounds(i, j, votes);
        if (e >= 0) {
            i--;
            e -= dy;
        }
        e += dx;
    }
    return votes;
}

int** linecase3(int x1, int y1, int x2, int y2, int** votes) { // x1 = x2 && y1 < y2 && dx < dy
    /*for (int i = x1; i < x2; i++)
        votes = inbounds(i, y1, votes);*/
    for (int j = y1; j < y2; j++)
        votes = inbounds(x1, j, votes);
    return votes;
}

// driving force y -> dx > dy

int** linecase4(int x1, int y1, int x2, int y2, int** votes) { //x1 < x2 && y1 < y2 && dx > dy
    //std::cout << "case 4 " << x1 << y1 << " " << x2 << y2 << "\n";

    int dx = x2 - x1; // |
    int dy = y2 - y1; // _
    int e = dy - dx;
    int j = y1;

    for (int i = x1; i <= x2; i++) {
        //arr[i][j] = 0; // illuminate
        votes = inbounds(i, j, votes);
        if (e >= 0) {
            j++;
            e -= dx;
        }
        e += dy;
    }
    return votes;
}

int** linecase5(int x1, int y1, int x2, int y2, int** votes) {
    //std::cout << "case 5 " << x1 << y1 << " " << x2 << y2 << "\n";

    int dx = x2 - x1; // |
    int dy = y1 - y2; // _
    int e = dy - dx;
    int j = y1;

    for (int i = x1; i <= x2; i++) {
        //arr[i][j] = 0; // illuminate
        votes = inbounds(i, j, votes);
        if (e >= 0) {
            j--;
            e -= dx;
        }
        e += dy;
    }
    return votes;
}

int** linecase6(int x1, int y1, int x2, int y2, int** votes) { //y1 == y2 && x1 < x2
    //std::cout << "case 6" << x1 << y1 << " " << x2 << y2 << "\n";

    /*for (int j = y1; j < y2; j++)
        votes = inbounds(x1, j, votes);*/
    for (int i = x1; i < x2; i++)
        votes = inbounds(i, y1, votes);
    return votes;
}

void chooselinefunction(int x1, int y1, int x2, int y2, int** votes) {
    // the 6 freaking cases

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    // driving force horizontal

    // case 1
    if (x1 < x2 && y1 < y2 && dx <= dy) {
        //std::cout << "case 1a" << x1 << y1 << " " << x2 << y2 << "\n";
        votes = linecase1(x1, y1, x2, y2, votes);
    }
    else if (x2 < x1 && y2 < y1 && dx <= dy) {
        //std::cout << "case 1b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
        votes = linecase1(x1, y1, x2, y2, votes);
    }

    // case 2
    else if (x1 > x2 && y1 < y2 && dx <= dy) {
        //std::cout << "case 2a" << x1 << y1 << " " << x2 << y2 << "\n";
        votes = linecase2(x1, y1, x2, y2, votes);
    }
    else if (x1 < x2 && y1 > y2 && dx <= dy) {
        //std::cout << "case 2b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
        votes = linecase2(x1, y1, x2, y2, votes);
    }

    // case 3
    else if (x1 == x2 && y1 < y2) { // use higher precedence operator first
        //std::cout << "case 3a" << x1 << y1 << " " << x2 << y2 << "\n";
        votes = linecase3(x1, y1, x2, y2, votes);
    }
    else if (x1 == x2 && y1 > y2) {
        //std::cout << "case 3b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = y1;
        y1 = y2;
        y2 = temp;
        votes = linecase3(x1, y1, x2, y2, votes);
    }

    // driving force y -> dy > dx

    // case 4
    else if (x1 < x2 && y1 < y2 && dx >= dy) {
        //std::cout << "case 4a" << x1 << y1 << " " << x2 << y2 << "\n";
        votes = linecase4(x1, y1, x2, y2, votes);
    }
    else if (x2 < x1 && y2 < y1 && dx >= dy) {
        //std::cout << "case 4b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
        votes = linecase4(x1, y1, x2, y2, votes);
    }

    // case 5
    else if (x1 < x2 && y1 > y2 && dx >= dy) {
        //std::cout << "case 5a" << x1 << y1 << " " << x2 << y2 << "\n";
        votes = linecase5(x1, y1, x2, y2, votes);
    }
    else if (x2 < x1 && y2 > y1 && dx >= dy) {
        //std::cout << "case 5b" << x1 << y1 << " " << x2 << y2 << "\n";
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
        votes = linecase5(x1, y1, x2, y2, votes);
    }

    // case 6
    else if (y1 == y2 && x1 < x2) { // use higher precedence operator first
        votes = linecase6(x1, y1, x2, y2, votes);
    }
    else if (y1 == y2 && x2 < x1) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
        votes = linecase6(x1, y1, x2, y2, votes);
    }
}

void circle(int** array, int xcenter, int ycenter, int R) {
    int y = R;
    int xmax = (int)(R * 0.70710678);
    int ysquare = y * y;
    int ty = (2 * y) - 1;
    int y2_new = ysquare;

    //std::cout << y << " " << xmax << " " << ysquare << " " << ty << " " << y2_new << "\n";

    for (int x = 0; x <= xmax + 1; x++) {
        if ((ysquare - y2_new) >= ty) {
            ysquare -= ty;
            y -= 1;
            ty -= 2;
        }
        notoutofbounds(array, y + xcenter, x + ycenter);
        notoutofbounds(array, -y + xcenter, x + ycenter);
        notoutofbounds(array, y + xcenter, -x + ycenter);
        notoutofbounds(array, -y + xcenter, -x + ycenter);

        notoutofbounds(array, x + xcenter, y + ycenter);
        notoutofbounds(array, -x + xcenter, y + ycenter);
        notoutofbounds(array, x + xcenter, -y + ycenter);
        notoutofbounds(array, -x + xcenter, -y + ycenter);

        y2_new -= (2 * x) - 3;
    }
}

int** maskx(int** grayscale) {
    int** maskX;
    maskX = new int* [height];
    for (int i = 0; i < height; i++)
        maskX[i] = new int[width];

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            maskX[i][j] = 0;

    for (int i = 1; i < height - 1; ++i) {
        for (int j = 1; j < width - 1; ++j) {
            int a = grayscale[i - 1][j - 1] * -1;
            int b = grayscale[i - 1][j] * 0;
            int c = grayscale[i - 1][j + 1] * 1;

            int d = grayscale[i][j - 1] * -2;
            int e = grayscale[i][j] * 0;
            int f = grayscale[i][j + 1] * 2;

            int g = grayscale[i + 1][j - 1] * -1;
            int h = grayscale[i + 1][j] * 0;
            int k = grayscale[i + 1][j + 1] * 1;

            maskX[i][j] = a + b + c + d + e + f + g + h + k;
        }
    }
    return maskX;
}

int** masky(int** grayscale) {
    int** maskY;
    maskY = new int* [height];
    for (int i = 0; i < height; i++)
        maskY[i] = new int[width];

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            maskY[i][j] = 0;

    for (int i = 1; i < height - 1; ++i) {
        for (int j = 1; j < width - 1; ++j) {
            int a = grayscale[i - 1][j - 1] * -1;
            int b = grayscale[i][j - 1] * 0;
            int c = grayscale[i + 1][j - 1] * 1;

            int d = grayscale[i - 1][j] * -2;
            int e = grayscale[i][j] * 0;
            int f = grayscale[i + 1][j] * 2;

            int g = grayscale[i - 1][j + 1] * -1;
            int h = grayscale[i][j + 1] * 0;
            int k = grayscale[i + 1][j + 1] * 1;

            maskY[i][j] = a + b + c + d + e + f + g + h + k;
        }
    }
    return maskY;
}

int** recursive(int** threshold, int i, int j) {
    /*
     i - 1, j - 1       i - 1, j        i - 1, j + 1
     i, j - 1           i, j            i, j + 1
     i + 1, j - 1       i + 1, j        i + 1, j + 1
    */
    if (threshold[i][j] == 0)
        return threshold;
    else if (visited[i][j] == true)
        return threshold;
    else {
        visited[i][j] = true;
        if (threshold[i][j] == 1 || threshold[i][j] == 2)
            threshold[i][j] = 2;
        //upper left
        if (visited[i - 1][j - 1] == false)
            recursive(threshold, i - 1, j - 1);
        //top
        if (visited[i - 1][j] == false)
            recursive(threshold, i - 1, j);
        //upper right
        if (visited[i - 1][j + 1] == false)
            recursive(threshold, i - 1, j + 1);
        //left
        if (visited[i][j - 1] == false)
            recursive(threshold, i, j - 1);
        //right
        if (visited[i][j + 1] == false)
            recursive(threshold, i, j + 1);
        //lower left
        if (visited[i + 1][j - 1] == false)
            recursive(threshold, i + 1, j - 1);
        //bottom
        if (visited[i + 1][j] == false)
            recursive(threshold, i + 1, j);
        //lower right
        if (visited[i + 1][j + 1] == false)
            recursive(threshold, i + 1, j + 1);
    }
    return threshold;
}

int** hysteresis(int** threshold) {
    visited = new bool* [height];
    for (int i = 0; i < height; i++)
        visited[i] = new bool[width];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            visited[i][j] = false;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (threshold[i][j] == 2) {
                visited[i][j] = true;
                threshold = recursive(threshold, i, j);
            }
        }
    }
    return threshold;
}

void part1(int low, int high, std::string filename) {
    //std::cout << low << " " << high << " " << filename << "\n";
    // command prompt: ./l061 -L 110 -H 200 -F myimage.ppm
    // ./l061 -L 10 -H 80 -F circle.ppm

    std::ifstream og;

    std::string suffix = ".ppm";
    if (filename.size() >= suffix.size() && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0)
        og.open(filename);
    else
        og.open("image.ppm");

    if (low == 0 && high == 0) {
        low = 20;
        high = 90;
    }

    std::string next;
    if (og.is_open()) {
        og >> next;
        og >> next;
        width = std::stoi(next);
        og >> next;
        height = std::stoi(next);
        og >> next;
        //int maxcolor = std::stoi(next);
        //og >> next;

        // make array
        int** cc;
        cc = new int* [height];
        for (int i = 0; i < height; i++)
            cc[i] = new int[width * 3];

        int** arrayg;
        arrayg = new int* [height];
        for (int i = 0; i < height; i++)
            arrayg[i] = new int[width];

        int r;
        int g;
        int b;
        int intensity = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                og >> next;
                r = std::stoi(next);
                og >> next;
                g = std::stoi(next);
                og >> next;
                b = std::stoi(next);
                intensity = (int)((r + g + b) / 3);
                arrayg[i][j] = intensity; //intensity
                cc[i][j * 3] = r;
                cc[i][j * 3 + 1] = g;
                cc[i][j * 3 + 2] = b;
            }
        }

        int** maskX;
        maskX = new int* [height];
        for (int i = 0; i < height; i++)
            maskX[i] = new int[width];

        int** maskY;
        maskY = new int* [height];
        for (int i = 0; i < height; i++)
            maskY[i] = new int[width];

        maskX = maskx(arrayg);
        maskY = masky(arrayg);

        //MAGNITUDE

        int** magnitude;
        magnitude = new int* [height];
        for (int i = 0; i < height; i++)
            magnitude[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                magnitude[i][j] = (int)(sqrt((maskX[i][j] * maskX[i][j]) + (maskY[i][j] * maskY[i][j])));

        //DOUBLE THRESHOLD

        int** threshold;
        threshold = new int* [height];
        for (int i = 0; i < height; i++)
            threshold[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                if (magnitude[i][j] < low) //20
                    threshold[i][j] = 0;
                else if (magnitude[i][j] < high) //120
                    threshold[i][j] = 1; // weak edge
                else
                    threshold[i][j] = 2; // strong edge
            }

        // HYSTERESIS

        // any 1 connected to a 2 through 1s or 2s will be promoted to a 2

        threshold = hysteresis(threshold); //returns with 2s 1s and 0s

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                if (threshold[i][j] == 2)
                    threshold[i][j] = 1;
                else
                    threshold[i][j] = 0;
            }

        //DIRECTION

        int** direction;
        direction = new int* [height];
        for (int i = 0; i < height; i++)
            direction[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                direction[i][j] = atan2(maskY[i][j], maskX[i][j]) * 180 / M_PI;

        int** roundedangle;
        roundedangle = new int* [height];
        for (int i = 0; i < height; i++)
            roundedangle[i] = new int[width];

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (direction[i][j] < 22.5)
                    roundedangle[i][j] = 0;
                else if (direction[i][j] < 67.5)
                    roundedangle[i][j] = 45;
                else if (direction[i][j] < 112.5)
                    roundedangle[i][j] = 90;
                else if (direction[i][j] < 157.5)
                    roundedangle[i][j] = 135;
                else
                    roundedangle[i][j] = 0;
            }
        }

        int** nms;
        nms = new int* [height];
        for (int i = 0; i < height; i++)
            nms[i] = new int[width];

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (roundedangle[i][j] == 0) {
                    if (i > 0 && i < height - 1) {
                        if (magnitude[i][j] >= magnitude[i - 1][j] && magnitude[i][j] >= magnitude[i + 1][j])
                            nms[i][j] = 1;
                        else
                            nms[i][j] = 0;
                    }
                }
                else if (roundedangle[i][j] == 45) {
                    if (i > 0 && i < height - 1 && j > 0 && j < width - 1) {
                        if (magnitude[i][j] >= magnitude[i + 1][j - 1] && magnitude[i][j] >= magnitude[i - 1][j + 1])
                            nms[i][j] = 1;
                        else
                            nms[i][j] = 0;
                    }
                }
                else if (roundedangle[i][j] == 90) {
                    if (j > 0 && j < width - 1) {
                        if (magnitude[i][j] >= magnitude[i][j - 1] && magnitude[i][j] >= magnitude[i][j + 1])
                            nms[i][j] = 1;
                        else
                            nms[i][j] = 0;
                    }
                }
                else {// if (roundedangle[i][j] == 135) {
                    if (i > 0 && i < height - 1 && j > 0 && j < width - 1) {
                        if (magnitude[i][j] >= magnitude[i - 1][j - 1] && magnitude[i][j] >= magnitude[i + 1][j + 1])
                            nms[i][j] = 1;
                        else
                            nms[i][j] = 0;
                    }
                }
            }
        }

        int** edges;
        edges = new int* [height];
        for (int i = 0; i < height; i++)
            edges[i] = new int[width];

        //nms and threshold
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (nms[i][j] == 1 && threshold[i][j] == 1)
                    edges[i][j] = 1;
                else
                    edges[i][j] = 0;
            }
        }

        std::ofstream imagef;
        imagef.open("imagef.ppm");
        imagef << "P3 " << width << " " << height << " 1\n";

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (edges[i][j] == 1)
                    imagef << 1 << " " << 1 << " " << 1 << " "; // white
                else
                    imagef << 0 << " " << 0 << " " << 0 << " "; // black
            }
            imagef << std::endl;
        }
        imagef << "\n";
        imagef.close();

        int** votes;
        votes = new int* [height];
        for (int i = 0; i < height; i++)
            votes[i] = new int[width];
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                votes[i][j] = 0;

        double m;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (edges[i][j] == 1) {
                    m = tan(direction[i][j] * M_PI / 180.0); // <- thats radians

                    //double yi = (m * -i) + j;
                    double yi = (0 - i + m * j) / m;
                    //double yf = m * (height - i - 1) + j;
                    double yf = (height - 1 - i + m * j) / m;

                    chooselinefunction(0, (int)yi, height - 1, (int)yf, votes);
                }
            }
        }

        /*std::ofstream votesmatrix;
        votesmatrix.open("untitled.txt");

        for(int i = 0; i< height; i++){
            for(int j = 0; j < width; j++){
                votesmatrix << votes[i][j] << " ";
            }
            votesmatrix << "\n";
        }
        votesmatrix << "\n";*/

        int maximum = 0;

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                if (votes[i][j] > maximum)
                    maximum = votes[i][j];

        std::ofstream imagev;
        imagev.open("imagev.ppm");
        imagev << "P3 " << width << " " << height << " " << maximum << "\n";

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                imagev << votes[i][j] << " " << votes[i][j] << " " << votes[i][j] << " "; // white
            imagev << std::endl;
        }
        imagev << "\n";
        imagev.close();

        std::ofstream imageCC;
        imageCC.open("imageCC.ppm");
        imageCC << "P3 " << width << " " << height << " 255\n";

        int** centers;
        centers = new int* [height];
        for (int i = 0; i < height; i++)
            centers[i] = new int[width];

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (votes[i][j] > 40) {
                    circle(centers, i, j, 1);
                    circle(centers, i, j, 2);
                    circle(centers, i, j, 3);
                    circle(centers, i, j, 4);
                }
            }
        }

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (centers[i][j] == 3)
                    imageCC << 255 << " " << 0 << " " << 0 << " "; // red
                else
                    imageCC << cc[i][j * 3] << " " << cc[i][j * 3 + 1] << " " << cc[i][j * 3 + 2] << " "; // rest
            }
            imageCC << std::endl;
        }
        imageCC << "\n";
        imageCC.close();

        og.close();
    }
    else
        std::cout << "Unable to open file";
}

int main(int argc, char* argv[]) {
    part1(atoi(argv[2]), atoi(argv[4]), std::string(argv[6]));
}