#include "HelperFuncs.hpp"

int Calmp(int value, int min = 0, int max = 255) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }

    return value;
}

void AddEdgeZeros(const Mat& inputImage, Mat& outputImage, int edgeSize) {
    outputImage = Mat(inputImage.rows + edgeSize * 2, inputImage.cols + edgeSize * 2, inputImage.type(), Scalar(0, 0, 0));

    for (auto i = edgeSize; i < outputImage.rows - edgeSize; ++i) {
        for (auto j = edgeSize; j < outputImage.cols - edgeSize; ++j) {
            outputImage.at<Vec3b>(i, j) = inputImage.at<Vec3b>(i - edgeSize, j - edgeSize);
        }
    }
}

void ReliefTransform(const Mat& inputImage, Mat& outputImage) {
    AddEdgeZeros(inputImage, outputImage);
    const auto temp = outputImage.clone();

    for (auto i = 1; i < outputImage.rows - 1; ++i) {
        for (auto j = 1; j < outputImage.cols - 1; ++j) {
            auto& curr       = outputImage.at<Vec3b>(i, j);
            const auto& next = temp.at<Vec3b>(i + 1, j + 1);
            const auto& prev = temp.at<Vec3b>(i - 1, j - 1);

            curr[0] = prev[0] - next[0] + 128;
            curr[1] = prev[1] - next[1] + 128;
            curr[2] = prev[2] - next[2] + 128;
        }
    }
}

void EngraveTransform(const Mat& inputImage, Mat& outputImage) {
    AddEdgeZeros(inputImage, outputImage);

    for (auto i = 1; i < outputImage.rows - 1; ++i) {
        for (auto j = 1; j < outputImage.cols - 1; ++j) {
            auto& curr       = outputImage.at<Vec3b>(i, j);
            const auto& next = outputImage.at<Vec3b>(i + 1, j + 1);

            curr[0] -= next[0] - 128;
            curr[1] -= next[1] - 128;
            curr[2] -= next[2] - 128;
        }
    }
}

void UniversalTransform(const Mat& inputImage, Mat& outputImage, const Mat& transMatrix) {
    AddEdgeZeros(inputImage, outputImage, transMatrix.rows / 2 - (transMatrix.cols % 2 == 0));
    const auto tempImage = outputImage.clone();

    for (auto i = 1; i < outputImage.rows - 1; ++i) {
        for (auto j = 1; j < outputImage.cols - 1; ++j) {
            auto& curr = outputImage.at<Vec3b>(i, j);
            curr = 0;

            for (auto k = 0; k < transMatrix.rows; ++k) {
                for (auto l = 0; l < transMatrix.cols; ++l) {
                    for (auto m = 0; m < 3; ++m) {
                        curr[m] += static_cast<uchar>(transMatrix.at<double>(k, l)
                            * static_cast<double>(tempImage.at<Vec3b>(i - 1 + k, j - 1 + l)[m]));
                    }
                }
            }
        }
    }
}
