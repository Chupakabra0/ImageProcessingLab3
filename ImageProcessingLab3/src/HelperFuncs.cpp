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
    if (edgeSize == 0) {
        outputImage = inputImage;
        return;
    }

    outputImage = Mat(inputImage.rows + edgeSize * 2, inputImage.cols + edgeSize * 2, inputImage.type(), Scalar(0, 0, 0));

    for (auto i = edgeSize; i < outputImage.rows - edgeSize; ++i) {
        for (auto j = edgeSize; j < outputImage.cols - edgeSize; ++j) {
            outputImage.at<Vec3b>(i, j) = inputImage.at<Vec3b>(i - edgeSize, j - edgeSize);
        }
    }
}

void AddEdgeClosest(const Mat& inputImage, Mat& outputImage, int edgeSize) {
    if (edgeSize == 0) {
        outputImage = inputImage;
        return;
    }

    AddEdgeZeros(inputImage, outputImage, edgeSize);

    for (auto i = edgeSize; i < outputImage.cols - edgeSize; ++i) {
        for (auto j = 0; j < edgeSize; ++j) {
            outputImage.at<Vec3b>(j, i) = outputImage.at<Vec3b>(edgeSize, i);
            outputImage.at<Vec3b>(outputImage.rows - edgeSize + j, i) = outputImage.at<Vec3b>(outputImage.rows - edgeSize - 1, i);
        }
    }

    for (auto i = 0; i < outputImage.rows; ++i) {
        for (auto j = 0; j < edgeSize; ++j) {
            outputImage.at<Vec3b>(i, j) = outputImage.at<Vec3b>(i, edgeSize);
            outputImage.at<Vec3b>(i, outputImage.cols - edgeSize + j) = outputImage.at<Vec3b>(i, outputImage.cols - edgeSize - 1);
        }
    }
}

void RemoveEdge(const Mat& inputImage, Mat& outputImage, int edgeSize) {
    if (edgeSize == 0) {
        outputImage = inputImage;
        return;
    }

    outputImage = Mat(inputImage.rows - 2 * edgeSize, inputImage.cols - 2 * edgeSize, inputImage.type(), Scalar(0, 0, 0));

    for (auto i = 0; i < outputImage.rows; ++i) {
        for (auto j = 0; j < outputImage.cols; ++j) {
            outputImage.at<Vec3b>(i, j) = inputImage.at<Vec3b>(i + edgeSize, j + edgeSize);
        }
    }
}

void ReliefTransform(const Mat& inputImage, Mat& outputImage) {
    AddEdgeZeros(inputImage, outputImage);
    auto temp = outputImage.clone();

    for (auto i = 1; i < temp.rows - 1; ++i) {
        for (auto j = 1; j < temp.cols - 1; ++j) {
            auto& curr       = temp.at<Vec3b>(i, j);
            const auto& next = outputImage.at<Vec3b>(i + 1, j + 1);
            const auto& prev = outputImage.at<Vec3b>(i - 1, j - 1);

            curr[0] = prev[0] - next[0] + 128;
            curr[1] = prev[1] - next[1] + 128;
            curr[2] = prev[2] - next[2] + 128;
        }
    }

    RemoveEdge(temp, outputImage);
}

void EngraveTransform(const Mat& inputImage, Mat& outputImage) {
    AddEdgeZeros(inputImage, outputImage);
    auto temp = outputImage.clone();

    for (auto i = 1; i < temp.rows - 1; ++i) {
        for (auto j = 1; j < temp.cols - 1; ++j) {
            auto& curr       = temp.at<Vec3b>(i, j);
            const auto& next = temp.at<Vec3b>(i + 1, j + 1);

            curr[0] -= next[0] - 128;
            curr[1] -= next[1] - 128;
            curr[2] -= next[2] - 128;
        }
    }

    RemoveEdge(temp, outputImage);
}

void AddGreyColor(const Mat& inputImage, Mat& outputImage) {
    outputImage = inputImage.clone();

    for (auto i = 0; i < outputImage.rows; ++i) {
        for (auto j = 0; j < outputImage.cols; ++j) {
            outputImage.at<Vec3b>(i, j)[0] += 128;
            outputImage.at<Vec3b>(i, j)[1] += 128;
            outputImage.at<Vec3b>(i, j)[2] += 128;
        }
    }
}

void UniversalTransform(const Mat& inputImage, Mat& outputImage, const Mat& transMatrix) {
    const auto edgeSize = transMatrix.rows / 2;
    AddEdgeClosest(inputImage, outputImage, edgeSize);
    auto temp = outputImage.clone();

    for (auto i = edgeSize; i < outputImage.rows - edgeSize; ++i) {
        for (auto j = edgeSize; j < outputImage.cols - edgeSize; ++j) {
            Vec<int, 3> curr = 0;

            for (auto k = 0; k < transMatrix.rows; ++k) {
                for (auto l = 0; l < transMatrix.cols; ++l) {
                    for (auto m = 0; m < curr.rows; ++m) {
                        curr[m] += static_cast<int>(transMatrix.at<double>(k, l)
                            * static_cast<double>(outputImage.at<Vec3b>(i - edgeSize + k, j - edgeSize + l)[m]));
                    }
                }
            }

            // TODO: remove it to another method
            for (auto m = 0; m < curr.rows; ++m) {
                if (curr[m] > std::numeric_limits<uchar>::max()) {
                    curr[m] = std::numeric_limits<uchar>::max();
                }
                if (curr[m] < std::numeric_limits<uchar>::min()) {
                    curr[m] = std::numeric_limits<uchar>::min();
                }
            }

            temp.at<Vec<uchar, 3>>(i, j) = Vec<uchar, 3>(curr);
        }
    }

    RemoveEdge(temp, outputImage, edgeSize);
}
