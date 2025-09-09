#pragma once
#include <opencv2/opencv.hpp>
cv::Mat apply_circle_mask(const cv::Mat& input);
void detect_hsv_contours(const cv::Mat& bgra, std::vector<std::vector<cv::Point>>& out, int min_area=120, cv::Scalar low=cv::Scalar(145,125,125), cv::Scalar high=cv::Scalar(165,255,255));
