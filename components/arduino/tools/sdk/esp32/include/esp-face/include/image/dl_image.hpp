#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "dl_define.hpp"
#include "dl_variable.hpp"
#include "dl_math_matrix.hpp"

namespace dl
{
    namespace image
    {
        typedef enum
        {
            IMAGE_RESIZE_BILINEAR = 0, /*<! Resize image by taking bilinear of four pixels */
            IMAGE_RESIZE_MEAN = 1,     /*<! Resize image by taking mean of four pixels */
            IMAGE_RESIZE_NEAREST = 2   /*<! Resize image by taking the nearest pixel */
        } resize_type_t;

        /**
         * @brief Convert RGB888 pixel to Gray.
         * 
         * @param red   red value
         * @param green green value
         * @param blue  blue value
         * @return gray value
         */
        inline uint8_t convert_pixel_rgb888_to_gray(int red, int green, int blue)
        {
            int temp = (red * 38 + green * 75 + blue * 15) >> 7;
            return DL_CLIP(temp, 0, 255);
        }

        /**
         * @brief Convert RGB565 pixel to RGB888.
         * 
         * @tparam T supports all integer types
         * @param input  pixel value in RGB565
         * @param output pixel value in RGB888
         */
        template <typename T>
        inline void convert_pixel_rgb565_to_rgb888(uint16_t input, T *output)
        {
            output[0] = (input & 0x1F00) >> 5;                           // blue
            output[1] = ((input & 0x7) << 5) | ((input & 0xE000) >> 11); // green
            output[2] = input & 0xF8;                                    // red
        }

        /**
         * @brief Convert RGB565 pixel to Gray.
         * 
         * @param input pixel value in RGB565
         * @return pixel value in Gray
         */
        inline uint8_t convert_pixel_rgb565_to_gray(uint16_t input)
        {
            int blue = (input & 0x1F00) >> 5;                            // blue
            int green = ((input & 0x7) << 5) | ((input & 0xE000) >> 11); // green
            int red = input & 0xF8;                                      // red

            return convert_pixel_rgb888_to_gray(red, green, blue);
        }

        /**
         * @brief Crop a patch from image and resize and store to destination image.
         * If the cropping box is out of image, destination image will be padded with edge.
         * 
         * The outer rectangle is the entire output image.
         * The inner rectangle is where the resized image will be stored.
         * In other world, this function could help you do padding while resize image.
         *               ___________________________(dst_w)__________________
         *              |         ___________________________                |
         *              |        |(x_start, y_start)         |               | 
         *              |        |                           |               | 
         *              |        |                           |               | 
         *       (dst_h)|        |                           |               | 
         *              |        |                           |               | 
         *              |        |                           |               | 
         *              |        |___________________________|(x_end, y_end) | 
         *              |____________________________________________________| 
         * 
         * @tparam T suppot all integer types
         * @param dst_image     pointer of destination(output) image
         * @param dst_width     destination image width
         * @param dst_channel   destination image channel number
         * @param dst_y_start   start y of resized image in destination image
         * @param dst_y_end     end y of resized image in destination image
         * @param dst_x_start   start x of resized image in destination image
         * @param dst_x_end     end x of resized image in destination image
         * @param src_image     pointer of source image
         * @param src_height    source image height
         * @param src_width     source image width
         * @param src_channel   source image channel
         * @param src_y_start   start y of resized image in source image
         * @param src_y_end     end y of resized image in source image
         * @param src_x_start   start x of resized image in source image
         * @param src_x_end     end x of resized image in source image
         * @param resize_type   one of IMAGE_RESIZE_BILINEAR or IMAGE_RESIZE_MEAN or IMAGE_RESIZE_NEAREST
         * @param shift_left    bit left shift number implemented on output
         */
        template <typename T>
        void crop_and_resize(T *dst_image,
                             int dst_width,
                             int dst_channel,
                             int dst_y_start, int dst_y_end,
                             int dst_x_start, int dst_x_end,
                             uint16_t *src_image,
                             int src_height,
                             int src_width,
                             int src_channel,
                             int src_y_start, int src_y_end,
                             int src_x_start, int src_x_end,
                             resize_type_t resize_type = IMAGE_RESIZE_NEAREST,
                             int shift_left = 0);

        /**
         * @brief Crop a patch from image and resize and store to destination image.
         * If the cropping box is out of image, destination image will be padded with edge.
         * 
         * The outer rectangle is the entire output image.
         * The inner rectangle is where the resized image will be stored.
         * In other world, this function could help you do padding while resize image.
         *               ___________________________(dst_w)__________________
         *              |         ___________________________                |
         *              |        |(x_start, y_start)         |               | 
         *              |        |                           |               | 
         *              |        |                           |               | 
         *       (dst_h)|        |                           |               | 
         *              |        |                           |               | 
         *              |        |                           |               | 
         *              |        |___________________________|(x_end, y_end) | 
         *              |____________________________________________________| 
         * 
         * @tparam T suppot all integer types
         * @param dst_image     pointer of destination(output) image
         * @param dst_width     destination image width
         * @param dst_channel   destination image channel number
         * @param dst_y_start   start y of resized image in destination image
         * @param dst_y_end     end y of resized image in destination image
         * @param dst_x_start   start x of resized image in destination image
         * @param dst_x_end     end x of resized image in destination image
         * @param src_image     pointer of source image
         * @param src_height    source image height
         * @param src_width     source image width
         * @param src_channel   source image channel
         * @param src_y_start   start y of resized image in source image
         * @param src_y_end     end y of resized image in source image
         * @param src_x_start   start x of resized image in source image
         * @param src_x_end     end x of resized image in source image
         * @param resize_type   one of IMAGE_RESIZE_BILINEAR or IMAGE_RESIZE_MEAN or IMAGE_RESIZE_NEAREST
         * @param shift_left    bit left shift number implemented on output
         */
        template <typename T>
        void crop_and_resize(T *dst_image,
                             int dst_width,
                             int dst_channel,
                             int dst_y_start, int dst_y_end,
                             int dst_x_start, int dst_x_end,
                             uint8_t *src_image,
                             int src_height,
                             int src_width,
                             int src_channel,
                             int src_y_start, int src_y_end,
                             int src_x_start, int src_x_end,
                             resize_type_t resize_type = IMAGE_RESIZE_NEAREST,
                             int shift_left = 0);

        /**
         * @brief Draw a filled rectangle on RGB888 image.
         * 
         * @param image        pointer of input image
         * @param image_height height of input image
         * @param image_width  width of input image
         * @param x1           left up corner x
         * @param y1           left up corner y
         * @param x2           right bottom corner x
         * @param y2           right bottom corner y
         * @param color        0x    00|       00|       00|       00
         *                     reserved|channel 0|channel 1|channel 2 
         */
        void draw_filled_rectangle(uint8_t *image, const uint32_t image_height, const uint32_t image_width,
                                   uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                                   const uint32_t color = 0x00FF0000);

        /**
         * @brief Draw a filled rectangle on RGB565 image.
         * 
         * @param image        pointer of input image
         * @param image_height height of input image
         * @param image_width  width of input image
         * @param x1           left up corner x
         * @param y1           left up corner y
         * @param x2           right bottom corner x
         * @param y2           right bottom corner y
         * @param color        0b         000|    00000|    00000|           000
         *                     channel 1[2:0]|channel 0|channel 2|channel 1[5:3] 
         */
        void draw_filled_rectangle(uint16_t *image, const uint32_t image_height, const uint32_t image_width,
                                   uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                                   const uint16_t color = 0b0001111100000000);

        /**
         * @brief Draw a point on RGB888 image.
         * 
         * @param image        pointer of input image
         * @param image_height height of input image
         * @param image_width  width of input image
         * @param x            point x
         * @param y            point y
         * @param size         size of point
         * @param color        0x    00|       00|       00|       00
         *                     reserved|channel 0|channel 1|channel 2 
         */
        void draw_point(uint8_t *image, const uint32_t image_height, const uint32_t image_width,
                        const uint32_t x, const uint32_t y, const uint32_t size,
                        const uint32_t color = 0x00FF0000);

        /**
         * @brief Draw a point on RGB565 image.
         * 
         * @param image        pointer of input image
         * @param image_height height of input image
         * @param image_width  width of input image
         * @param x            point x
         * @param y            point y
         * @param size         size of point
         * @param color        0b         000|    00000|    00000|           000
         *                     channel 1[2:0]|channel 0|channel 2|channel 1[5:3] 
         */
        void draw_point(uint16_t *image, const uint32_t image_height, const uint32_t image_width,
                        const uint32_t x, const uint32_t y, const uint32_t size,
                        uint16_t color = 0b0001111100000000);

        /**
         * @brief Draw a hollow rectangle on RGB888 image.
         * 
         * @param image        pointer of input image
         * @param image_height height of input image
         * @param image_width  width of input image
         * @param x1           left up corner x
         * @param y1           left up corner y
         * @param x2           right bottom corner x
         * @param y2           right bottom corner y
         * @param color        0x    00|       00|       00|       00
         *                     reserved|channel 0|channel 1|channel 2 
         */
        void draw_hollow_rectangle(uint8_t *image, const uint32_t image_height, const uint32_t image_width,
                                   uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                                   uint32_t color = 0x00FF0000);

        /**
         * @brief Draw a hollow rectangle on RGB565 image.
         * 
         * @param image        pointer of input image
         * @param image_height height of input image
         * @param image_width  width of input image
         * @param x1           left up corner x
         * @param y1           left up corner y
         * @param x2           right bottom corner x
         * @param y2           right bottom corner y
         * @param color        0b         000|    00000|    00000|           000
         *                     channel 1[2:0]|channel 0|channel 2|channel 1[5:3] 
         */
        void draw_hollow_rectangle(uint16_t *image, const uint32_t image_height, const uint32_t image_width,
                                   uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                                   const uint16_t color = 0b0001111100000000);

        /**
         * @brief Detect target moving by activated detection point number. Each cross in the figure below is a detection point.
         * Once abs(frame_1_detection_point[i] - frame_2_detection_point[i]) > threshold, this detection point is activated.
         * This function will return the number of activated detection point.
         * 
         *         __stride__________________________
         *         |        |        |        |   |
         *  stride |        |        |        |   |
         *         |        |        |        |   |
         *         |________|________|________|   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |________|________|________| height
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |________|________|________|   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |________|________|________|___|___
         *         |                          |
         *         |__________width___________|
         *         |                          |
         * 
         * Time consumption:
         * Frame shape = (240, 240)
         * Both frame are in PSRAM
         * On ESP32-S3 with CPU 240MHz, QSPI 80MHz
         * 
         * stride  latency
         *      1  28316us
         *      2   8770us
         *      4   3622us
         *      8   1990us
         *     16    880us
         *     32    260us
         * 
         * 
         * In a application, outside this function, threshold of activated detection point number is needed.
         * Once activated detection point number > number_threshold, this two frame are judged target moved.
         * How to determine the number_threshold?
         * Let's assume that the minimize shape of target is (target_min_height, target_max_width).
         * Then, the number_threshold = [target_min_height / stride] * [target_max_width / stride] * ratio,
         * where ratio is in (0, 1), the smaller the ratio is, the more sensitive the detector is, the more false detected.
         * 
         * 
         * @param f1        one frame in RGB565
         * @param f2        another frame in RGB565
         * @param height    height of frame
         * @param width     width of frame
         * @param stride    stride of detection point, the smaller the stride is, the more reliable the detector is.
         * @param threshold activation threshold of each detection point
         * @return activated detection point number 
         */
        uint32_t get_moving_point_number(uint16_t *f1, uint16_t *f2, const uint32_t height, const uint32_t width, const uint32_t stride, const uint32_t threshold = 5);

        /**
         * @brief Detect target moving by activated detection point number. Each cross in the figure below is a detection point.
         * Once abs(frame_1_detection_point[i] - frame_2_detection_point[i]) > threshold, this detection point is activated.
         * This function will return the number of activated detection point.
         * 
         *         __stride__________________________
         *         |        |        |        |   |
         *  stride |        |        |        |   |
         *         |        |        |        |   |
         *         |________|________|________|   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |________|________|________| height
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |________|________|________|   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |        |        |        |   |
         *         |________|________|________|___|___
         *         |                          |
         *         |__________width___________|
         *         |                          |
         * 
         * 
         * In a application, outside this function, threshold of activated detection point number is needed.
         * Once activated detection point number > number_threshold, this two frame are judged target moved.
         * How to determine the number_threshold?
         * Let's assume that the minimize shape of target is (target_min_height, target_max_width).
         * Then, the number_threshold = [target_min_height / stride] * [target_max_width / stride] * ratio,
         * where ratio is in (0, 1), the smaller the ratio is, the more sensitive the detector is, the more false detected.
         * 
         * 
         * @param f1        one frame in RGB888
         * @param f2        another frame in RGB888
         * @param height    height of frame
         * @param width     width of frame
         * @param stride    stride of detection point, the smaller the stride is, the more reliable the detector is.
         * @param threshold activation threshold of each detection point
         * @return activated detection point number 
         */
        uint32_t get_moving_point_number(uint8_t *f1, uint8_t *f2, const uint32_t height, const uint32_t width, const uint32_t stride, const uint32_t threshold = 5);


        template <typename T>
        void warp_affine(dl::Tensor<T> *input, dl::Tensor<T> *output, dl::math::Matrix<float> *M_inv);
        template <typename T>
        void warp_affine(uint16_t *input, std::vector<int> shape, dl::Tensor<T> *output, dl::math::Matrix<float> *M_inv);

    } // namespace image
} // namespace dl
