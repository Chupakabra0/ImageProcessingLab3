#pragma once
#include "MainHeader.hpp"

void ReliefTransform(const Mat& inputImage, Mat& outputImage);
void EngraveTransform(const Mat& inputImage, Mat& outputImage);

void AddEdgeZeros(const Mat& inputImage, Mat& outputImage, int edgeSize = 1);
