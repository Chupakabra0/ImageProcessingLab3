#pragma once
#include "MainHeader.hpp"

void ReliefTransform(const Mat& inputImage, Mat& outputImage);
void EngraveTransform(const Mat& inputImage, Mat& outputImage);
void AddGreyColor(const Mat& inputImage, Mat& outputImage);

void UniversalTransform(const Mat& inputImage, Mat& outputImage, const Mat& transMatrix);

void AddEdgeZeros(const Mat& inputImage, Mat& outputImage, int edgeSize = 1);
void AddEdgeClosest(const Mat& inputImage, Mat& outputImage, int edgeSize = 1);

void RemoveEdge(const Mat& inputImage, Mat& outputImage, int edgeSize = 1);
