#include "capture_utils.h"

cv::Mat apply_circle_mask(const cv::Mat& input){
    cv::Mat mask=cv::Mat::zeros(input.size(),CV_8UC1);
    cv::circle(mask,{mask.cols/2,mask.rows/2},std::min(mask.cols,mask.rows)/2,cv::Scalar(255),-1);
    cv::Mat output; input.copyTo(output,mask); return output;
}

void detect_hsv_contours(const cv::Mat& bgra, std::vector<std::vector<cv::Point>>& out, int min_area, cv::Scalar low, cv::Scalar high){
    out.clear(); if(bgra.empty()) return;
    cv::Mat bgr,hsv,mask; cv::cvtColor(bgra,bgr,cv::COLOR_BGRA2BGR); cv::cvtColor(bgr,hsv,cv::COLOR_BGR2HSV);
    cv::inRange(hsv,low,high,mask);
    cv::morphologyEx(mask,mask,cv::MORPH_OPEN,cv::getStructuringElement(cv::MORPH_ELLIPSE,{3,3}));
    cv::morphologyEx(mask,mask,cv::MORPH_CLOSE,cv::getStructuringElement(cv::MORPH_ELLIPSE,{5,5}));
    std::vector<std::vector<cv::Point>> cs; cv::findContours(mask,cs,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    for(auto& c:cs) if(cv::contourArea(c)>=min_area) out.push_back(std::move(c));
}
