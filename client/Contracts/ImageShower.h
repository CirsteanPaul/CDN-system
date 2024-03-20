//
// Created by Paul Cirstean on 12.12.2023.
//

#ifndef CLIENT_IMAGESHOWER_H
#define CLIENT_IMAGESHOWER_H

#include <iostream>
#include <opencv2/opencv.hpp>

const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

class ImageShower{
    static inline bool is_base64( unsigned char c )
    {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    static std::string base64_decode(std::string const& encoded_string)
    {
        int in_len = encoded_string.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        std::string ret;

        while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
        {
            char_array_4[i++] = encoded_string[in_]; in_++;

            if (i == 4)
            {
                for (i = 0; i < 4; i++)
                {
                    char_array_4[i] = base64_chars.find(char_array_4[i]);
                }

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                {
                    ret += char_array_3[i];
                }

                i = 0;
            }
        }

        if (i)
        {
            for (j = i; j < 4; j++)
            {
                char_array_4[j] = 0;
            }

            for (j = 0; j < 4; j++)
            {
                char_array_4[j] = base64_chars.find(char_array_4[j]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++)
            {
                ret += char_array_3[j];
            }
        }

        return ret;
    }

    static cv::Mat str2mat(const std::string& s)
    {
        // Decode data
        std::string decoded_string = base64_decode(s);
        std::vector<uchar> data(decoded_string.begin(), decoded_string.end());

        cv::Mat img = imdecode(data, cv::IMREAD_UNCHANGED);
        return img;
    }

public: static void ShowImage(std::string base64) {
        auto y = str2mat(base64);

        imshow("image", y);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
};
#endif //CLIENT_IMAGESHOWER_H
