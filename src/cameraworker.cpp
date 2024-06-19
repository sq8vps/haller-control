#include "cameraworker.h"
#include <iostream>
#include <algorithm>
#include <QCoreApplication>
#include <cmath>

CameraWorker::CameraWorker(CameraMode cameraMode): cMode(cameraMode){
}

void CameraWorker::process() {
    int ok_col = 2000;
    int warning_col = 1500;
    int critical_col = 700;

    while(running) {
        bool col = collision;
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
            colorCam->setInterleaved(false);
            camLog->setRate(1);

            camOut->setStreamName("cam");
            logOut->setStreamName("log");

            colorCam->preview.link(camOut->input);
            camLog->out.link(logOut->input);

            if (col) {
                colorCam->setFps(15);
                colorCam->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
                auto monoCamLeft = pipeline.create<dai::node::MonoCamera>();
                monoCamLeft->setBoardSocket(dai::CameraBoardSocket::CAM_B);
                monoCamLeft->setResolution(dai::MonoCameraProperties::SensorResolution::THE_720_P);

                auto monoCamRight = pipeline.create<dai::node::MonoCamera>();
                monoCamRight->setBoardSocket(dai::CameraBoardSocket::CAM_C);
                monoCamLeft->setResolution(dai::MonoCameraProperties::SensorResolution::THE_720_P);

                auto stereo = pipeline.create<dai::node::StereoDepth>();
                stereo->setDefaultProfilePreset(dai::node::StereoDepth::PresetMode::HIGH_ACCURACY);
                stereo->initialConfig.setMedianFilter(dai::MedianFilter::KERNEL_7x7);
                stereo->initialConfig.setConfidenceThreshold(50);
                stereo->setExtendedDisparity(true);
                stereo->setLeftRightCheck(true);

                monoCamLeft->out.link(stereo->left);
                monoCamRight->out.link(stereo->right);

                auto slc = pipeline.create<dai::node::SpatialLocationCalculator>();
                slc->inputConfig.setWaitForMessage(false);
                for (int x = 0; x < 10; x++) {
                    for (int y = 0; y < 6; y++) {
                        dai::SpatialLocationCalculatorConfigData config;
                        config.depthThresholds.lowerThreshold = 200;
                        config.depthThresholds.upperThreshold = 10000;
                        dai::Point2f topLeft(((float)x + 0.5)*0.09, ((float)y +0.5)*0.15);
                        dai::Point2f bottomRight(((float)x + 1.5)*0.09, ((float)y +1.5)*0.15);
                        config.roi = dai::Rect(topLeft, bottomRight);
                        config.calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MEDIAN;
                        slc->initialConfig.addROI(config);
                    }
                }

                stereo->depth.link(slc->inputDepth);
                stereo->setDepthAlign(dai::CameraBoardSocket::CAM_A);

                auto slcOut = pipeline.create<dai::node::XLinkOut>();
                slcOut->setStreamName("collision");
                slc->out.link(slcOut->input);

            }

            try {
                device = std::make_shared<dai::Device>(pipeline);

                auto cam = device->getOutputQueue("cam", 4, false);
                auto log = device->getOutputQueue("log");
                std::shared_ptr<dai::DataOutputQueue> collision;

                if (col) {
                    collision = device->getOutputQueue("collision", 4, false);
                }

                emit cameraStatus(CameraStatus::Up);

                while(running) {

                    QCoreApplication::processEvents();

                    if (!running) {
                        continue;
                    }

                    auto imgFrame = cam->get<dai::ImgFrame>();
                    cv::Mat mat = imgFrame->getCvFrame();

                    if (col) {
                        auto colIn = collision->tryGet<dai::SpatialLocationCalculatorData>();
                        if (colIn) {
                            int warn_sig = 0;
                            int critical_sig = 0;
                            auto colData = colIn->getSpatialLocations();
                            for (auto col : colData) {
                                auto roi = col.config.roi;
                                roi = roi.denormalize(mat.cols, mat.rows);

                                auto xmin = (int)roi.topLeft().x;
                                auto ymin = (int)roi.topLeft().y;
                                auto xmax = (int)roi.bottomRight().x;
                                auto ymax = (int)roi.bottomRight().y;

                                auto coords = col.spatialCoordinates;
                                float distance = sqrt(coords.x*coords.x + coords.y*coords.y + coords.z*coords.z);
                                std::shared_ptr<cv::Scalar> color;

                                if (distance == 0) {
                                    continue;
                                } else if(distance < critical_col) {
                                    critical_sig++;
                                    color = std::make_shared<cv::Scalar>(0, 0, 255);
                                } else if(distance < warning_col) {
                                    warn_sig++;
                                    color = std::make_shared<cv::Scalar>(0, 140, 255);
                                } else {
                                    continue;
                                }


                                cv::rectangle(mat, cv::Rect(cv::Point(xmin, ymin), cv::Point(xmax, ymax)), *color, cv::FONT_HERSHEY_SIMPLEX);
                                std::stringstream depthX;
                                depthX << std::fixed << std::setprecision(2) << distance/1000 << " m";
                                cv::putText(mat, depthX.str(), cv::Point(xmin + 10, ymin + 20), cv::FONT_HERSHEY_SIMPLEX, 0.3, *color, 0.2);
                            }

                            if (critical_sig > 0) {
                                emit cameraCollision(CollisionState::Critical);
                            } else if (warn_sig > 0) {
                                emit cameraCollision(CollisionState::Warning);
                            } else {
                                emit cameraCollision(CollisionState::Ok);
                            }

                        }
                        QImage qimg((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format::Format_BGR888);
                        emit newFrame(qimg);
                    } else {
                        QImage qimg((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format::Format_BGR888);
                        emit newFrame(qimg);
                    }



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

                bool laser = false;
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

void CameraWorker::setCollision(bool collision_) {
    collision = collision_;
}
