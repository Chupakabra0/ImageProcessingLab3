#include "MainHeader.hpp"
#include "HelperFuncs.hpp"

std::unique_ptr<argparse::ArgumentParser> InitArgs(int argc, char** argv) {
    auto parser = std::make_unique<argparse::ArgumentParser>();
    parser->add_argument("-f", "--filepath")
        .required().help("specify the input image file");

    try {
        parser->parse_args(argc, argv);
    }
    catch (const std::exception&) {
        throw;
    }

    return parser;
}

int main(int argc, char** argv) {
    try {
        cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

        auto argParser = InitArgs(argc, argv);
        auto filePath  = argParser->get<std::string>("-f");

        auto oldImage = std::make_unique<Mat>(imread(filePath, IMREAD_ANYCOLOR | IMREAD_ANYDEPTH));
        auto newImage = std::make_unique<Mat>();

        auto transMatrix = std::make_unique<Mat>(3, 3, CV_64FC1, 0.0);

        transMatrix->at<double>(1, 1) = 1.0;
        transMatrix->at<double>(2, 2) = -1.0;

        imshow("Before", *oldImage);
        waitKey();

        UniversalTransform(*oldImage, *newImage, *transMatrix);

        imshow("After:", *newImage);
        waitKey();

        imwrite(std::format("mod_{}", filePath), *newImage);

        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }
}
