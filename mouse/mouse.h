#ifndef MOUSE_H
#define MOUSE_H

#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <winsock2.h>
#include <Windows.h>

#include <mutex>
#include <atomic>
#include <chrono>
#include <vector>
#include <utility>
#include <queue>
#include <thread>
#include <condition_variable>

#include "rzctl.h"

class MouseThread
{
private:
    double screen_width;
    double screen_height;
    double fov_x;
    double fov_y;
    double max_distance;
    double center_x;
    double center_y;

    std::atomic<bool> mouse_pressed{ false };

    RZControl* rzctl;


    std::mutex                    queueMtx;
    std::condition_variable       queueCv;
    const size_t                  queueLimit = 5;
    std::thread                   moveWorker;
    std::atomic<bool>             workerStop{ false };

    std::vector<std::pair<double, double>> futurePositions;
    std::mutex                    futurePositionsMutex;

public:
    std::mutex input_method_mutex;

    MouseThread(
        int  resolution,
        int  fovX,
        int  fovY,
        RZControl* rzctl = nullptr
    );
    ~MouseThread();

    void updateConfig(
        int resolution,
        int fovX,
        int fovY
    );

    void releaseMouse();

    void setRzctlMouse(RZControl* newRzctl);

};

#endif // MOUSE_H