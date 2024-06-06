#include "cameraworker.h"
#include <iostream>
#include <algorithm>
#include <QCoreApplication>

CameraWorker::CameraWorker(CameraMode cameraMode): cMode(cameraMode){
}

void CameraWorker::process() {
    while(running) {
        switch (cMode) {
        case CameraMode::Color:
        {
            emit cameraStatus(CameraStatus::Connecting);
            dai::Pipeline pipeline;
            auto colorCam = pipeline.create<dai::node::ColorCamera>();
            auto camLog = pipeline.create<dai::node::SystemLogger>();
            auto camOut = pipeline.create<dai::node::XLinkOut>();
            auto logOut = pipeline.create<dai::node::XLinkOut>();
            colorCam->setPreviewSize(600, 360);
            colorCam->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
            colorCam->setColorOrder(dai::ColorCameraProperties::ColorOrder::RGB);
            colorCam->setFps(30);
            colorCam->setInterleaved(true);
            camLog->setRate(1);

            camOut->setStreamName("cam");
            logOut->setStreamName("log");

            colorCam->preview.link(camOut->input);
            camLog->out.link(logOut->input);

            try {
                device = std::make_shared<dai::Device>(pipeline);

                auto cam = device->getOutputQueue("cam");
                auto log = device->getOutputQueue("log");

                emit cameraStatus(CameraStatus::Up);

                while(running) {

                    QCoreApplication::processEvents();

                    if (!running) {
                        continue;
                    }

                    auto imgFrame = cam->get<dai::ImgFrame>();
                    cv::Mat mat = imgFrame->getFrame();
                    QImage qimg((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format::Format_RGB888);
                    emit newFrame(qimg);

                    auto systemLog = log->tryGet<dai::SystemInformation>();

                    if (systemLog) {



                        float tempArr[4] = {systemLog->chipTemperature.css, systemLog->chipTemperature.dss, systemLog->chipTemperature.mss, systemLog->chipTemperature.upa};
                        float maxTemp = *std::max_element(tempArr, tempArr + 4);

                        emit cameraTemperature(maxTemp);
                        emit cameraCpuUsage(systemLog->leonCssCpuUsage.average * 100);

                    }

                }
                emit cameraStatus(CameraWorker::CameraStatus::Connecting);
                emit cameraStatus(CameraWorker::CameraStatus::Down);
            } catch(const std::runtime_error &e) {
                emit cameraStatus(CameraStatus::Error);
                running = false;
            }
        }
        break;

        case CameraMode::LowLatency:
        {
            emit cameraStatus(CameraStatus::Connecting);
            dai::Pipeline pipeline;
            auto monoCamLeft = pipeline.create<dai::node::MonoCamera>();
            auto camLog = pipeline.create<dai::node::SystemLogger>();
            auto camOut = pipeline.create<dai::node::XLinkOut>();
            auto logOut = pipeline.create<dai::node::XLinkOut>();

            monoCamLeft->setBoardSocket(dai::CameraBoardSocket::CAM_B);
            monoCamLeft->setResolution(dai::MonoCameraProperties::SensorResolution::THE_400_P);
            monoCamLeft->setFps(30);

            camLog->setRate(1);

            camOut->setStreamName("cam");

            logOut->setStreamName("log");

            monoCamLeft->out.link(camOut->input);
            camLog->out.link(logOut->input);

            try {
                device = std::make_shared<dai::Device>(pipeline);

                auto cam = device->getOutputQueue("cam");
                auto log = device->getOutputQueue("log");

                emit cameraStatus(CameraStatus::Up);

                while(running) {

                    QCoreApplication::processEvents();

                    if (!running) {
                        continue;
                    }

                    auto imgFrame = cam->get<dai::ImgFrame>();
                    cv::Mat mat = imgFrame->getFrame();
                    QImage qimg((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format::Format_Grayscale8);
                    emit newFrame(qimg);

                    auto systemLog = log->tryGet<dai::SystemInformation>();

                    if (systemLog) {



                        float tempArr[4] = {systemLog->chipTemperature.css, systemLog->chipTemperature.dss, systemLog->chipTemperature.mss, systemLog->chipTemperature.upa};
                        float maxTemp = *std::max_element(tempArr, tempArr + 4);

                        emit cameraTemperature(maxTemp);
                        emit cameraCpuUsage(systemLog->leonCssCpuUsage.average * 100);

                    }

                }

                emit cameraStatus(CameraWorker::CameraStatus::Connecting);
                emit cameraStatus(CameraWorker::CameraStatus::Down);
            } catch(const std::runtime_error &e) {
                emit cameraStatus(CameraStatus::Error);
                running = false;
            }


        }
        break;

        case CameraMode::NightVision:
        {
            emit cameraStatus(CameraStatus::Connecting);
            dai::Pipeline pipeline;
            auto monoCamLeft = pipeline.create<dai::node::MonoCamera>();
            auto camLog = pipeline.create<dai::node::SystemLogger>();
            auto camOut = pipeline.create<dai::node::XLinkOut>();
            auto logOut = pipeline.create<dai::node::XLinkOut>();

            monoCamLeft->setBoardSocket(dai::CameraBoardSocket::CAM_B);
            monoCamLeft->setResolution(dai::MonoCameraProperties::SensorResolution::THE_400_P);
            monoCamLeft->setFps(30);

            camLog->setRate(1);

            camOut->setStreamName("cam");

            logOut->setStreamName("log");

            monoCamLeft->out.link(camOut->input);
            camLog->out.link(logOut->input);

            try {
                device = std::make_shared<dai::Device>(pipeline);

                bool laser = device->setIrFloodLightIntensity(1);
                if (laser) {
                    emit cameraLaserStatus(1);
                }

                auto cam = device->getOutputQueue("cam");
                auto log = device->getOutputQueue("log");

                emit cameraStatus(CameraStatus::Up);

                while(running) {

                    QCoreApplication::processEvents();

                    if (!running) {
                        continue;
                    }

                    auto imgFrame = cam->get<dai::ImgFrame>();
                    cv::Mat mat = imgFrame->getFrame();
                    QImage qimg((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format::Format_Grayscale8);
                    emit newFrame(qimg);

                    auto systemLog = log->tryGet<dai::SystemInformation>();

                    if (systemLog) {



                        float tempArr[4] = {systemLog->chipTemperature.css, systemLog->chipTemperature.dss, systemLog->chipTemperature.mss, systemLog->chipTemperature.upa};
                        float maxTemp = *std::max_element(tempArr, tempArr + 4);

                        emit cameraTemperature(maxTemp);
                        emit cameraCpuUsage(systemLog->leonCssCpuUsage.average * 100);

                    }

                }

                emit cameraStatus(CameraWorker::CameraStatus::Connecting);
                emit cameraStatus(CameraWorker::CameraStatus::Down);
            } catch(const std::runtime_error &e) {
                emit cameraStatus(CameraStatus::Error);
                running = false;
            }
        }
        break;

        case CameraMode::DepthVision:
        {
            emit cameraStatus(CameraStatus::Connecting);
            dai::Pipeline pipeline;
            auto monoLeft = pipeline.create<dai::node::MonoCamera>();
            auto monoRight = pipeline.create<dai::node::MonoCamera>();
            auto depth = pipeline.create<dai::node::StereoDepth>();
            auto camLog = pipeline.create<dai::node::SystemLogger>();
            auto camOut = pipeline.create<dai::node::XLinkOut>();
            auto logOut = pipeline.create<dai::node::XLinkOut>();

            camOut->setStreamName("cam");
            logOut->setStreamName("log");

            camLog->setRate(1);

            monoLeft->setResolution(dai::MonoCameraProperties::SensorResolution::THE_800_P);
            monoLeft->setBoardSocket(dai::CameraBoardSocket::CAM_B);

            monoRight->setResolution(dai::MonoCameraProperties::SensorResolution::THE_800_P);
            monoRight->setBoardSocket(dai::CameraBoardSocket::CAM_C);

            depth->setDefaultProfilePreset(dai::node::StereoDepth::PresetMode::HIGH_DENSITY);
            depth->initialConfig.setMedianFilter(dai::MedianFilter::KERNEL_7x7);
            depth->setExtendedDisparity(false);
            depth->setLeftRightCheck(true);

            monoLeft->out.link(depth->left);
            monoRight->out.link(depth->right);
            depth->disparity.link(camOut->input);
            camLog->out.link(logOut->input);

            try {
                device = std::make_shared<dai::Device>(pipeline);

                bool laser = device->setIrLaserDotProjectorIntensity(1);
                if (laser) {
                    emit cameraLaserStatus(1);
                }

                auto cam = device->getOutputQueue("cam");
                auto log = device->getOutputQueue("log");

                emit cameraStatus(CameraStatus::Up);

                while(running) {

                    QCoreApplication::processEvents();

                    if (!running) {
                        continue;
                    }

                    auto imgFrame = cam->get<dai::ImgFrame>();
                    cv::Mat mat = imgFrame->getFrame();

                    mat.convertTo(mat, CV_8UC1, 255/depth->initialConfig.getMaxDisparity());
                    cv::applyColorMap(mat, mat, cv::COLORMAP_OCEAN);

                    QImage qimg((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format::Format_BGR888);
                    emit newFrame(qimg);

                    auto systemLog = log->tryGet<dai::SystemInformation>();

                    if (systemLog) {

                        float tempArr[4] = {systemLog->chipTemperature.css, systemLog->chipTemperature.dss, systemLog->chipTemperature.mss, systemLog->chipTemperature.upa};
                        float maxTemp = *std::max_element(tempArr, tempArr + 4);

                        emit cameraTemperature(maxTemp);
                        emit cameraCpuUsage(systemLog->leonCssCpuUsage.average * 100);

                    }

                }

                emit cameraStatus(CameraWorker::CameraStatus::Connecting);
                emit cameraStatus(CameraWorker::CameraStatus::Down);
            } catch(const std::runtime_error &e) {
                emit cameraStatus(CameraStatus::Error);
                running = false;
            }
        }
            break;

        default:
            if (!device->isClosed()) {
                device->close();
            }
            emit cameraStatus(CameraStatus::Down); // CameraStatus::Down
            break;
        }

        device->close();
        emit cameraTemperature(0);
        emit cameraCpuUsage(0);
        emit cameraLaserStatus(0);
        while (!running) {
            QCoreApplication::processEvents();

            if (running) {
                continue;
            }
        };

    }

}

void CameraWorker::setMode(CameraMode cameraMode) {
    cMode = cameraMode;
}

void CameraWorker::printSystemInformation(dai::SystemInformation info) {
    printf("Ddr used / total - %.2f / %.2f MiB\n", info.ddrMemoryUsage.used / (1024.0f * 1024.0f), info.ddrMemoryUsage.total / (1024.0f * 1024.0f));
    printf("Cmx used / total - %.2f / %.2f MiB\n", info.cmxMemoryUsage.used / (1024.0f * 1024.0f), info.cmxMemoryUsage.total / (1024.0f * 1024.0f));
    printf("LeonCss heap used / total - %.2f / %.2f MiB\n",
           info.leonCssMemoryUsage.used / (1024.0f * 1024.0f),
           info.leonCssMemoryUsage.total / (1024.0f * 1024.0f));
    printf("LeonMss heap used / total - %.2f / %.2f MiB\n",
           info.leonMssMemoryUsage.used / (1024.0f * 1024.0f),
           info.leonMssMemoryUsage.total / (1024.0f * 1024.0f));
    const auto& t = info.chipTemperature;
    printf("Chip temperature - average: %.2f, css: %.2f, mss: %.2f, upa: %.2f, dss: %.2f\n", t.average, t.css, t.mss, t.upa, t.dss);
    printf("Cpu usage - Leon CSS: %.2f %%, Leon MSS: %.2f %%\n", info.leonCssCpuUsage.average * 100, info.leonMssCpuUsage.average * 100);
    printf("----------------------------------------\n");
}

void CameraWorker::stop() {
    running = false;
}

void CameraWorker::restart() {
    running = true;
}
