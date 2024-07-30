#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/opengl.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

cv::VideoCapture cap;
cv::Size patternSize(6, 9);
std::vector<cv::Point2f> corners;
bool found = false;

void drawCube(cv::Mat& frame, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs,
              const cv::Mat& rvec, const cv::Mat& tvec) {
    float cubeSize = 3.0f; // Ajustamos el tamaño del cubo
    std::vector<cv::Point3f> cubePoints = {
        {-cubeSize/2, -cubeSize/2, 0}, {cubeSize/2, -cubeSize/2, 0}, {cubeSize/2, cubeSize/2, 0}, {-cubeSize/2, cubeSize/2, 0},
        {-cubeSize/2, -cubeSize/2, -cubeSize}, {cubeSize/2, -cubeSize/2, -cubeSize}, {cubeSize/2, cubeSize/2, -cubeSize}, {-cubeSize/2, cubeSize/2, -cubeSize}
    };
    std::vector<cv::Point2f> imagePoints;
    cv::projectPoints(cubePoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

    // Definimos colores "girlies"
    std::vector<cv::Scalar> colors = {
        cv::Scalar(255, 182, 193),  // Pink
        cv::Scalar(255, 192, 203),  // Light Pink
        cv::Scalar(255, 105, 180),  // Hot Pink
        cv::Scalar(255, 20, 147),   // Deep Pink
        cv::Scalar(219, 112, 147),  // Pale Violet Red
        cv::Scalar(255, 0, 255)     // Magenta
    };

    // Dibujamos las caras del cubo con colores
    std::vector<std::vector<int>> faces = {
        {0,1,2,3}, {4,5,6,7}, {0,1,5,4}, {2,3,7,6}, {0,3,7,4}, {1,2,6,5}
    };
    for (size_t i = 0; i < faces.size(); ++i) {
        std::vector<cv::Point> facePoints;
        for (int idx : faces[i]) {
            facePoints.push_back(imagePoints[idx]);
        }
        cv::fillConvexPoly(frame, facePoints, colors[i]);
    }

    // Dibujamos las líneas del cubo
    std::vector<std::vector<int>> lines = {{0,1,2,3,0}, {4,5,6,7,4}, {0,4}, {1,5}, {2,6}, {3,7}};
    for (const auto& line : lines) {
        for (size_t i = 0; i < line.size() - 1; ++i) {
            cv::line(frame, imagePoints[line[i]], imagePoints[line[i+1]], cv::Scalar(255, 255, 255), 2);
        }
    }
}

int main(int argc, char** argv) {
    cap.open(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    cv::Mat frame;
    cap >> frame;

    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 
        frame.cols / 2, 0, frame.cols / 2,
        0, frame.cols / 2, frame.rows / 2,
        0, 0, 1);
    cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        found = cv::findChessboardCorners(gray, patternSize, corners,
            cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.1));

            cv::drawChessboardCorners(frame, patternSize, corners, found);

            std::vector<cv::Point3f> obj;
            for (int i = 0; i < patternSize.height; ++i) {
                for (int j = 0; j < patternSize.width; ++j) {
                    obj.push_back(cv::Point3f(j, i, 0));
                }
            }

            cv::Mat rvec, tvec;
            cv::solvePnP(obj, corners, cameraMatrix, distCoeffs, rvec, tvec);

            // Ajustamos la posición del cubo al centro del patrón
            cv::Mat rotMat;
            cv::Rodrigues(rvec, rotMat);
            cv::Mat centerPoint = (cv::Mat_<double>(3,1) << (patternSize.width-1)/2.0, (patternSize.height-1)/2.0, 0);
            cv::Mat centerPointWorld = rotMat * centerPoint + tvec;
            tvec = centerPointWorld - rotMat * (cv::Mat_<double>(3,1) << 0, 0, 0);

            drawCube(frame, cameraMatrix, distCoeffs, rvec, tvec);
        }

        cv::imshow("AR Cube", frame);
        if (cv::waitKey(1) == 27) break;
    }

    return 0;
}