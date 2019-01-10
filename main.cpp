#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>
#include <fstream>

#include <unistd.h>

using namespace cv;
using namespace std;

/////////////////////////////////////////////////////////////////////////
////  ///  /////  ///////     ///  /////        ////          ///////////
////  ///  ////    //////  /   //  /////////   ////////   ///////////////
////       ///  /   /////  //   /  ////////  //////////   ///////////////
////  ///  //       /////  ///     ///////  ///////////   ///////////////
////  ///  //  ///  /////  /////   //////       ////          ///////////
/////////////////////////////////////////////////////////////////////////

// Generates the list of characters.
int listOfCharacters(string characterList[]);

// Take 2 images and finds the difference, then computes the average point.
void differenceAverage(Mat img, Mat img2, vector<int> &xAverages, vector<int> &yAverages);

// Find the base points of the stroke order.
void findSequencePoints(string folderPath, Mat total, vector<int> &xAverages, vector<int> &yAverages);

// Fills in the data points.
vector<Point> fillingDataPoints(vector<int> xAverages, vector<int> yAverages, int frequency, double sizeFactor);


int main(int argc, char** argv) {

    int frequency = 1000;
    int speed = 5;
    int maxArraySize = 6000;
    double sizeFactor = 2;

    string characterList[maxArraySize];

    //Get a list of all character names in the array passed.
    int size = listOfCharacters(characterList);

    for (int i = 0; i < size; i++) {

        vector<int> xAverages;
        vector<int> yAverages;

        // Make a new blank image.
        Mat total = imread("total.jpg", IMREAD_COLOR);

        // Access each character name (each character is a group of images contained in one folder.
        string folderPath = "Characters/"+ characterList[i];

        // Find all the average differences (points) between each successive picture.
        findSequencePoints(folderPath, total, xAverages, yAverages);

        for (int i = 0; i < xAverages.size(); i++) {
            xAverages[i] *= sizeFactor;
            yAverages[i] *= sizeFactor;
        }

        // Fill in the gaps between the points and load that data into a vector.
        vector<Point> totalAverages = fillingDataPoints(xAverages, yAverages, frequency, sizeFactor);

        // Make the image visible.
        namedWindow( "Display window", WINDOW_AUTOSIZE );
        imshow( "Display window", total);
        waitKey(0);
        for (int k = 0; k < totalAverages.size(); k++) {
            // The double for loop is to flesh out the image a bit, making it look better.
            for (int i = 0; i < 5 + sizeFactor; i++){
                for (int j = 0; j < 5 + sizeFactor; j++) {
                    total.at<Vec3b>((totalAverages[k].y + i) , (totalAverages[k].x + j)) = {0, 0 ,0};
                }
            }
            // Alters the speed of display.
            if (k%speed == 0) {
                imshow( "Display window", total);
                waitKey(1);
            }
        }

        // Display the image.
        imshow( "Display window", total);
        // Wait for user input before continuing.
        waitKey(0);
    }

    return 0;
}

int listOfCharacters(string characterList[]) {

    ifstream txt("/Users/alexedwards/Desktop/listOfCharacters.rtf");
    string temp;
    int i = 0;
    while (txt >> temp) {
        if (temp.substr(1, 5) == "image"){
            string large = "-large";
            characterList[i] = temp.substr(8, 4) + large + temp.substr(12, 4);
            i++;
        }
    }
    return i;
}

void differenceAverage(Mat img, Mat img2, vector<int> &xAverages, vector<int> &yAverages) {

    int num(0);

    double xTotal(0), yTotal(0);

    // Goes through every pixel of image 1 and image 2.
    for (int y = 0; y < img.rows; y++) {

        for (int x = 0; x < img.cols; x++) {

            Point p = {x, y};

            Vec3b pixel = img.at<Vec3b>(p);
            Vec3b pixel2 = img2.at<Vec3b>(p);

            // If they are the same pixel, that means that it is not important so it is set to white.
            // This isn't important to do for the actual program, it just helps for visualisation.
            if (pixel == pixel2) {
                img2.at<Vec3b>(p) = {255, 255, 255};
            }

            // If they are not the same, then it is a new difference.
            // xTotal will be the sum of all the x-coordinates, which will then be averaged by
            // Average = xTotal / numberOfXCoordinates
            // The same logic applies to the y-coordinates.
            if (pixel2 != pixel) {
                xTotal += x;
                yTotal += y;
                num++;
            }
        }
    }

    int xAvg = (int) (xTotal / num);
    int yAvg = (int) (yTotal/num);

    xAverages.push_back(xAvg);
    yAverages.push_back(yAvg);

}

void differenceBetweenTwoImages(vector<string> fileNames, vector<int> &xAverages, vector<int> &yAverages ) {

    for(int i = 1; i < fileNames.size(); i++) {

        // Load the images.
        Mat im = imread(fileNames[i]);
        Mat im2 = imread(fileNames[i - 1]);

        // This finds a point the point that represents the average
        // of all the points that are different between the two images.
        differenceAverage(im, im2, xAverages, yAverages);
    }
}


void findSequencePoints(String folderPath, Mat total, vector<int> &xAverages, vector<int> &yAverages) {

    // Need to load the first 10, then 11-99
    // If not, the the glob function loads them in alphanumerical order and puts 10 before 1
    // e.g. 1.jpg is After 10.jpg
    // As such, three folders are needed representing the first 9 images, then 10-99, and so on.

    vector<String> fileNames1_9;
    vector<String> fileNames10_99;
    vector<String> fileNames100_999;

    // Easier change of folder location without changing file type wanted.

    string path1 = folderPath + "/1/*.png";
    string path2 = folderPath + "/2/*.png";
    string path3 = folderPath + "/3/*.png";


    glob(path1, fileNames1_9, false);
    glob(path2, fileNames10_99, false);
    glob(path3, fileNames100_999, false);


    differenceBetweenTwoImages(fileNames1_9, xAverages, yAverages);
    differenceBetweenTwoImages(fileNames10_99, xAverages, yAverages);
    differenceBetweenTwoImages(fileNames100_999, xAverages, yAverages);

}

// Fills in the points between the base differences.
vector<Point> fillingDataPoints(vector<int> xAverages, vector<int> yAverages, int frequency, double sizeFactor) {

    vector<Point> totalDataPoints;

    // Dependent on the number of data points, the spacing between the points will be different.
    // Default changeValue (the amount that will be successively added).
    double changeValue = 1.6*sizeFactor;

    if ((xAverages.size() > 70) && (xAverages.size() <= 100)) {
        changeValue = 1.4*sizeFactor;
    }
    if ((xAverages.size() > 40) && (xAverages.size() <= 50)) {
        changeValue = 1.6*sizeFactor;
    }
    if (xAverages.size() <= 40) {
        changeValue = 1.8*sizeFactor;
    }

    // Using y = mx+b.
    // Take two elements
    // Think of the first element as the origin (0, 0).
    // Find the slope between the two points.
    // The data points allotted to fill that space will be the frequency/averages.size()
    // Add the slope*(data points + average[0]).

    int numPoints = (int) floor(frequency/xAverages.size());

    for (int i = 1; i < xAverages.size(); i++){
        double xDiff = xAverages[i] - xAverages[i-1];
        double yDiff = yAverages[i] - yAverages[i -1];
        double xIncrement = xDiff / numPoints;
        double yIncrement = yDiff / numPoints;

        // If the xIncrement is not within the range, that likely means that a new stroke has occured.
        // (Since a new stroke will generally occur far away from the previous point).
        // As such, only the gaps between points 'of the same stroke' will be filled in.
        if ((xIncrement <= changeValue) && (xIncrement >= -changeValue &&
                                            (yIncrement <= changeValue) && (yIncrement >= -changeValue))) {
            for (int j = 0; j < numPoints; j++) {
                int xPoint = (int) xAverages[i-1] + xIncrement * j;
                int yPoint = (int) yAverages[i-1] + yIncrement * j;
                totalDataPoints.push_back({xPoint, yPoint});
            }
        }
    }

    //
    return totalDataPoints;
}