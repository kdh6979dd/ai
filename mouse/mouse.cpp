#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <winsock2.h>
#include <Windows.h>

#include <cmath>
#include <algorithm>
#include <chrono>
#include <mutex>
#include <atomic>
#include <vector>

#include "mouse.h"
#include "capture.h"
#include "sunone_aimbot_cpp.h"
#include "rzctl.h"

MouseThread::MouseThread(
    int resolution,
    int fovX,
    int fovY,
    RZControl* rzControl)
    : screen_width(resolution),
    screen_height(resolution),
    fov_x(fovX),
    fov_y(fovY),
    max_distance(std::hypot(resolution, resolution) / 2.0),
    center_x(resolution / 2.0),
    center_y(resolution / 2.0),
    rzctl(rzControl) {


void MouseThread::updateConfig(
    int resolution,
    int fovX,
    int fovY
)
{
    screen_width = screen_height = resolution;
    fov_x = fovX;  fov_y = fovY;

    center_x = center_y = resolution / 2.0;
    max_distance = std::hypot(resolution, resolution) / 2.0;

}

MouseThread::~MouseThread()
{
    workerStop = true;
    queueCv.notify_all();
    if (moveWorker.joinable()) moveWorker.join();
}

void MouseThread::releaseMouse()
{
    std::lock_guard<std::mutex> lock(input_method_mutex);

    if (mouse_pressed)
    {
        if (rzctl)
        {
            rzctl->mouseClick(MouseClick::LEFT_UP);
        }
        mouse_pressed = false;
    }
}

void MouseThread::setRzctlMouse(RZControl* newRzctl)
{
    std::lock_guard<std::mutex> lock(input_method_mutex);
    rzctl = newRzctl;
};
