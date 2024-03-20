//
// Created by Paul Cirstean on 12.12.2023.
//

#ifndef CDN_NODE_IMAGEREQUEST_H
#define CDN_NODE_IMAGEREQUEST_H

#include <iostream>
#include "IRequest.h"
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

static inline bool is_base64( unsigned char c )
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(uchar const* bytes_to_encode, unsigned int in_len)
{
    std::string ret;

    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--)
    {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i <4); i++)
            {
                ret += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
        {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
        {
            ret += base64_chars[char_array_4[j]];
        }

        while ((i++ < 3))
        {
            ret += '=';
        }
    }

    return ret;
}

std::string mat2str(const cv::Mat& m)
{
    int params[3] = {0};
    params[0] = IMWRITE_JPEG_QUALITY;
    params[1] = 100;

    vector<uchar> buf;
    bool code = cv::imencode(".jpg", m, buf, std::vector<int>(params, params+2));
    uchar* result = reinterpret_cast<uchar*> (&buf[0]);

    return base64_encode(result, buf.size());

}

class ImageRequest : public IRequest {
private: const std::string absolute_path = "/Users/paulcirstean/server-data/image/";
public: const std::string path;
public: ImageRequest(std::string path) : path(absolute_path + path) {}

public:
    HandleRequestResult Handle() {
        cv::Mat image = cv::imread(path);
        if (image.empty()) {
            return HandleRequestResult (new IResponse(404, "Not found"));
        }
        auto x = mat2str(image);

        return HandleRequestResult(new IResponse(200, x));
}
};
#endif //CDN_NODE_IMAGEREQUEST_H
