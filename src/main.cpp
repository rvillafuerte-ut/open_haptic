#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <cmath>
#include <HD/hd.h>
#include <Eigen/Dense>
#include <dynamixel_sdk/dynamixel_sdk.h>
#include <pinocchio/fwd.hpp>
#include <pinocchio/spatial/se3.hpp>

struct Sample {
    double position[3];
    double transform[16];
    double timestamp;
};

static std::atomic<bool> g_running{true};
static std::atomic<bool> g_sampleReady{false};
static Sample g_sample;

static HHD g_device = HD_INVALID_HANDLE;

static HDCallbackCode HDCALLBACK servoCallback(void* /*pUserData*/) {
    hdBeginFrame(g_device);
    hdGetDoublev(HD_CURRENT_POSITION, g_sample.position);
    hdGetDoublev(HD_CURRENT_TRANSFORM, g_sample.transform);
    g_sample.timestamp = hdGetSchedulerTimeStamp();
    hdEndFrame(g_device);
    g_sampleReady.store(true, std::memory_order_release);
    return g_running.load(std::memory_order_relaxed) ? HD_CALLBACK_CONTINUE : HD_CALLBACK_DONE;
}

int main(int argc, char** argv) {
    // Eigen test
    Eigen::Vector3d eigenVec(1.0, 2.0, 3.0);
    (void)eigenVec;
    
    // DynamixelSDK test
    dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler("/dev/ttyUSB0");
    dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(2.0);
    (void)portHandler;
    (void)packetHandler;
    
    // Pinocchio test
    pinocchio::SE3 M = pinocchio::SE3::Identity();
    (void)M;

    double durationSec = 5.0;
    double printRateHz = 100.0;
    for (int i = 1; i + 1 < argc; ++i) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            if (argv[i][2] == 'd') durationSec = std::atof(argv[i + 1]);
            else if (argv[i][2] == 'p') printRateHz = std::atof(argv[i + 1]);
        }
    }

    g_device = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(hdGetError())) return 0;

    hdMakeCurrentDevice(g_device);
    hdEnable(HD_FORCE_OUTPUT);
    hdSetSchedulerRate(1000);
    hdStartScheduler();
    if (HD_DEVICE_ERROR(hdGetError())) {
        hdStopScheduler();
        hdDisableDevice(g_device);
        return 0;
    }

    HDSchedulerHandle cbHandle = hdScheduleAsynchronous(servoCallback, nullptr, 0);

    std::thread consumer([&]{
        using namespace std::chrono;
        auto period = duration<double>(1.0 / printRateHz);
        auto next = steady_clock::now();
        while (g_running.load()) {
            next += duration_cast<steady_clock::duration>(period);
            if (g_sampleReady.exchange(false)) {
                const double *t = g_sample.transform;
                double roll  = std::atan2(t[6], t[10]);
                double pitch = std::atan2(-t[2], std::sqrt(t[6]*t[6] + t[10]*t[10]));
                double yaw   = std::atan2(t[1], t[0]);
                std::cout << "t=" << g_sample.timestamp
                          << " pos=[" << g_sample.position[0] << "," << g_sample.position[1] << "," << g_sample.position[2] << "]"
                          << " rpy=[" << roll << "," << pitch << "," << yaw << "]\n";
            }
            std::this_thread::sleep_until(next);
        }
    });

    std::this_thread::sleep_for(std::chrono::duration<double>(durationSec));
    g_running.store(false);

    hdStopScheduler();
    hdUnschedule(cbHandle);
    consumer.join();
    hdDisableDevice(g_device);

    return 0;
}
