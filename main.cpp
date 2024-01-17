/**
 * @file main.cpp
 * @author Mehmet Ekemen (ekemenms@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <CLI/CLI.hpp>
#include <algorithm>
#include <cctype>
#include <memory>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

namespace
{
    namespace SaveFile
    {
        /**
         * @class SaveStrategy
         * @brief Abstract base class for implementing different image saving strategies.
         *
         * SaveStrategy defines a common interface for all concrete image saving strategies.
         * It allows for polymorphic saving of images in various formats like PNG, JPEG, BMP, TGA, etc.
         * This class follows the Strategy design pattern.
         */
        class SaveStrategy
        {
        public:
            /**
             * Virtual destructor for SaveStrategy.
             */
            virtual ~SaveStrategy() = default;

            /**
             * Pure virtual function to save an image.
             *
             * @param path The file path where the image will be saved.
             * @param img The image data in the form of a vector of unsigned chars.
             * @param width The width of the image in pixels.
             * @param height The height of the image in pixels.
             */
            virtual void Save(const std::string& path, const std::vector<unsigned char>& img, int width,
                              int height) = 0;
        };

        /**
         * @class PngSaveStrategy
         * @brief Concrete strategy for saving images in PNG format.
         *
         * Inherits from SaveStrategy and implements the Save function to handle PNG image saving.
         */
        class PngSaveStrategy : public SaveStrategy
        {
        public:
            /**
             * Saves an image in PNG format.
             * Overrides the Save method from SaveStrategy.
             */
            void Save(const std::string& path, const std::vector<unsigned char>& img, int width, int height) override
            {
                stbi_write_png(path.c_str(), width, height, 1, img.data(), width);
            }
        };

        /**
         * @class JpegSaveStrategy
         * @brief Concrete strategy for saving images in JPEG format.
         *
         * Inherits from SaveStrategy and implements the Save function to handle JPEG image saving.
         */
        class JpegSaveStrategy : public SaveStrategy
        {
        public:
            /**
             * Saves an image in JPEG format.
             * Overrides the Save method from SaveStrategy.
             */
            void Save(const std::string& path, const std::vector<unsigned char>& img, int width, int height) override
            {
                stbi_write_jpg(path.c_str(), width, height, 1, img.data(), 100);
            }
        };

        /**
         * @class BmpSaveStrategy
         * @brief Concrete strategy for saving images in BMP format.
         *
         * Inherits from SaveStrategy and implements the Save function to handle BMP image saving.
         */
        class BmpSaveStrategy : public SaveStrategy
        {
        public:
            /**
             * Saves an image in BMP format.
             * Overrides the Save method from SaveStrategy.
             */
            void Save(const std::string& path, const std::vector<unsigned char>& img, int width, int height) override
            {
                stbi_write_bmp(path.c_str(), width, height, 1, img.data());
            }
        };

        /**
         * @class TgaSaveStrategy
         * @brief Concrete strategy for saving images in TGA format.
         *
         * Inherits from SaveStrategy and implements the Save function to handle TGA image saving.
         */
        class TgaSaveStrategy : public SaveStrategy
        {
        public:
            /**
             * Saves an image in TGA format.
             * Overrides the Save method from SaveStrategy.
             */
            void Save(const std::string& path, const std::vector<unsigned char>& img, int width, int height) override
            {
                stbi_write_tga(path.c_str(), width, height, 1, img.data());
            }
        };

    } // namespace SaveFile

    /**
     * @class ImageProcessor
     * @brief Abstract base class for image processing strategies.
     *
     * This class serves as an interface for different image processing techniques.
     * It defines a common interface for all concrete image processing classes.
     * The ProcessImage function must be implemented by all derived classes
     * to perform specific image processing operations.
     */
    class ImageProcessor
    {
    public:
        /**
         * Pure virtual function for processing an image.
         *
         * @param img The image to be processed, represented as a vector of unsigned chars.
         * @param width The width of the image in pixels.
         * @param height The height of the image in pixels.
         * @param channels The number of color channels in the image.
         */
        virtual void ProcessImage(std::vector<unsigned char>& img, int width, int height, int channels) = 0;
        /**
         * @brief Virtual destructor for the ImageProcessor class.
         */
        virtual ~ImageProcessor() = default;
    };

    /**
     * @class BlackAndWhiteProcessor
     * @brief Concrete class for converting images to black and white.
     *
     * Inherits from ImageProcessor and implements the ProcessImage function
     * to convert color images into black and white images.
     * This class uses multithreading to improve the performance of the conversion process.
     */
    class BlackAndWhiteProcessor : public ImageProcessor
    {
    public:
        /**
         * Processes the image to convert it to black and white.
         * Overrides the ProcessImage method from ImageProcessor.
         *
         * The function converts the color image to grayscale by averaging
         * the color channels for each pixel. It uses multithreading to
         * process different segments of the image concurrently.
         *
         * @param img Reference to the image vector that will be processed.
         * @param width The width of the image in pixels.
         * @param height The height of the image in pixels.
         * @param channels The number of color channels in the image.
         */
        void ProcessImage(std::vector<unsigned char>& img, int width, int height, int channels) override
        {
            std::vector<unsigned char> outputImage(width * height);

            auto processPixel = [&](int start, int end) -> void {
                for (int i = start; i < end; i += channels) {
                    int grayScale = 0;
                    for (int j = 0; j < channels; ++j) {
                        grayScale += img[i + j];
                    }
                    grayScale /= channels;
                    outputImage[i / channels] = static_cast<unsigned char>(grayScale);
                }
            };

            unsigned int nThreads = std::thread::hardware_concurrency();
            std::vector<std::thread> threads(nThreads);
            int pixels = width * height;
            int chunkSize = pixels / nThreads;

            for (unsigned int i = 0; i < nThreads; ++i) {
                int start = i * chunkSize * channels;
                int end = (i == nThreads - 1) ? pixels * channels : (i + 1) * chunkSize * channels;
                threads[i] = std::thread(processPixel, start, end);
            }

            for (auto& th : threads) {
                if (th.joinable()) {
                    th.join();
                }
            }

            img = std::move(outputImage);
        }
    };

    /**
     * @class ImageConverter
     * @brief Converts images between different formats and applies processing.
     *
     * The ImageConverter class is responsible for loading an image, applying
     * processing to it via an ImageProcessor, and then saving it in a desired format.
     * It supports multiple image formats for saving, including PNG, JPEG, BMP, and TGA.
     */
    class ImageConverter
    {
    public:
        /**
         * Constructor for ImageConverter.
         * Initializes the converter with paths and an image processor.
         *
         * @param inputPath Path to the input image file.
         * @param outputPath Path where the converted image will be saved.
         * @param processor A unique pointer to an ImageProcessor for image processing.
         */
        ImageConverter(const std::string& inputPath, const std::string& outputPath,
                       std::unique_ptr<ImageProcessor> processor)
            : inputPath(inputPath), outputPath(outputPath), processor(std::move(processor))
        {
            using namespace SaveFile;
            strategies["png"] = std::make_unique<PngSaveStrategy>();
            strategies["jpg"] = std::make_unique<JpegSaveStrategy>();
            strategies["jpeg"] = std::make_unique<JpegSaveStrategy>();
            strategies["bmp"] = std::make_unique<BmpSaveStrategy>();
            strategies["tga"] = std::make_unique<TgaSaveStrategy>();
        }

        /**
         * Converts the image from the input path, processes it, and saves it to the output path.
         * This function will load the image, apply the processing, and then save it
         * in the format determined by the output file's extension.
         *
         * @throws std::runtime_error if image loading, processing, or saving fails.
         */
        void ConvertImage()
        {
            int width, height, channels;
            unsigned char* imgData = stbi_load(inputPath.c_str(), &width, &height, &channels, 0);
            if (imgData == nullptr) {
                throw std::runtime_error("Error loading image");
            }

            std::unique_ptr<unsigned char[], void (*)(void*)> img(imgData, stbi_image_free);
            
            std::vector<unsigned char> imageVector(img.get(), img.get() + width * height * channels);

            processor->ProcessImage(imageVector, width, height, channels);

            SaveImage(outputPath, imageVector, width, height);
        }

    private:
        std::string inputPath;                     ///< Path to the input image.
        std::string outputPath;                    ///< Path to save the converted image.
        std::unique_ptr<ImageProcessor> processor; ///< Unique pointer to the image processor.
        std::unordered_map<std::string, std::unique_ptr<SaveFile::SaveStrategy>>
            strategies; ///< Map of file extension to corresponding save strategies.

        /**
         * Saves the processed image to the specified path.
         *
         * @param path Path where the image will be saved.
         * @param img Processed image data.
         * @param width Width of the image.
         * @param height Height of the image.
         * @throws std::runtime_error if the image format is unsupported.
         */

        void SaveImage(const std::string& path, const std::vector<unsigned char>& img, int width, int height)
        {
            std::string extension = GetFileExtension(path);
            auto it = strategies.find(extension);
            if (it != strategies.end()) {
                it->second->Save(path, img, width, height);
            } else {
                throw std::runtime_error("Unsupported image format");
            }
        }
        /**
         * Extracts the file extension from the given file name.
         *
         * @param fileName Name of the file.
         * @return Lowercase string of the file extension.
         * @throws std::runtime_error if the file extension cannot be determined.
         */
        std::string GetFileExtension(const std::string& fileName)
        {
            size_t dotPos = fileName.find_last_of('.');
            if (dotPos == std::string::npos)
                throw std::runtime_error("An error was encountered while finding the file extension.");

            auto ext = fileName.substr(dotPos + 1);
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            return ext;
        }
    };
} // namespace

int main(int argc, const char* argv[])
{
    CLI::App app;

    std::string inputFilePath, outputFilePath;
    app.add_option("-i, --input", inputFilePath, "Input image file path")->required();
    app.add_option("-o,--output", outputFilePath, "Output image file path")->required();

    CLI11_PARSE(app, argc, argv);

    try {
        auto processor = std::make_unique<BlackAndWhiteProcessor>();
        ImageConverter converter(inputFilePath, outputFilePath, std::move(processor));
        converter.ConvertImage();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
