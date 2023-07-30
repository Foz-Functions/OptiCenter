#include <iostream>
#include <fstream>
#include <string>
#include <OpenCV2/OpenCV.hpp>

using namespace cv;
using namespace std;

String Header = R"(
 ____________________________________________________________________________________________________________________________
/ _    _        _                                   _              _____         _    _  _____               _               \
|| |  | |      | |                                 | |            |  _  |       | |  (_)/  __ \             | |              |
|| |  | |  ___ | |  ___   ___   _ __ ___    ___    | |_   ___     | | | | _ __  | |_  _ | /  \/  ___  _ __  | |_   ___  _ __ |
|| |/\| | / _ \| | / __| / _ \ | '_ ` _ \  / _ \   | __| / _ \    | | | || '_ \ | __|| || |     / _ \| '_ \ | __| / _ \| '__||
|\  /\  /|  __/| || (__ | (_) || | | | | ||  __/   | |_ | (_) |   \ \_/ /| |_) || |_ | || \__/\|  __/| | | || |_ |  __/| |   |
| \/  \/  \___||_| \___| \___/ |_| |_| |_| \___|    \__| \___/     \___/ | .__/  \__||_| \____/ \___||_| |_| \__| \___||_|   |
\________________________________________________________________________| |_________________________________________________/
                                                                         |_|)";

int main(int, char**){
    std::cout << Header << endl;
}

/*
    A basic method for inputing a filepath from terminal
    checks to make sure a file exists then makes sure it is a valid image file
*/
Mat getImage() 
{
    String filePath;
    ifstream file;
    Mat image;



    do
    {
        std::cout << "Please enter the file path for your image below or press 'c' to close." << endl;
        cin >> filePath;
        std::cout << "\033[2J\033[H";
        
        image = imread(filePath);

        if (!image.empty())
        {
            return image;
        }
        else if (toLowerCase(filePath) != "c")
        {
            // Breaks out of the loop if the user wishes to cancel
            break;
        }
        else
        {
            std::cout << "The filepath (" << filePath << ") is invalid. Please input a new file path or press 'c' to close." << endl;
        }
        
    } while (true);
    
    exit(0);
}

class Img {
    public:
        Mat baseImage;
        Mat filtImg;        // The filtered image to perform calculations with
        float center[2];    // The center of area in the X axis

    Img (Mat image)
    {
        baseImage = image;
    }

    /**
     * @param thresh How the positive and negative space is determined
     * @param aaRad The anti-Aliasing radius to apply to the image (0 to disable)
    */
    Mat filter(int aaRad)
    {
        cv::cvtColor(baseImage, filtImg, cv::COLOR_BGR2GRAY);

        
        // Apply anti-aliasing if radius is more than 0
        if (aaRad > 0) {
            cv::GaussianBlur(filtImg, filtImg, cv::Size(aaRad, aaRad), 0);
        }
    }

    /**
     * Determines the center point of the image provided
     * likely after it has been filtered
    */
    void setCenter()
    {
        int row, col;
        int volume;
        float volDistX, volDistY;
        cv::MatIterator_<uchar> pixIter, start, end;
        start = filtImg.begin<uchar>();
        end = filtImg.end<uchar>();
        
        // Iterate through the mat using the OpenCV mat iterator for efficiency
        for( pixIter = start; pixIter != end; ++pixIter)
        {
            uchar pixVal = *pixIter;
            ptrdiff_t pos = pixIter - start;

            volume += pixVal;
            volDistX += pos % filtImg.cols * pixVal;
            volDistY += pos / filtImg.cols * pixVal;
        }

        center[0] = volDistX / volume;
        center[1] = volDistY / volume;
    }
};