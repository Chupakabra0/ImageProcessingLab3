#include "MainHeader.hpp"
#include "HelperFuncs.hpp"

int main(int argc, char** argv) {
    auto oldImage = std::make_unique<Mat>(imread("img.png", IMREAD_ANYCOLOR | IMREAD_ANYDEPTH));
    auto newImage = std::make_unique<Mat>();

    imshow("Before", *oldImage);
    waitKey();
    EngraveTransform(*oldImage, *newImage);
    imshow("After:", *newImage);
    waitKey();

    return EXIT_SUCCESS;
}
