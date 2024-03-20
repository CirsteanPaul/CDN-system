//
// Created by Paul Cirstean on 12.12.2023.
//

#ifndef CDN_NODE_IO_H
#define CDN_NODE_IO_H
#include <iostream>
#include <unistd.h>

class IO {
private: const static int PAGE_SIZE = 2042;
public: static std::string Read(int fd) {
        char buffer[PAGE_SIZE + 1];
        int bytes_read = 0;
        std::string response = "";
        do {
            bytes_read = read(fd, &buffer, PAGE_SIZE);

            if (bytes_read == -1) {
                return "";
            }
            buffer[bytes_read] = '\0';
            response += buffer;
        } while(bytes_read == PAGE_SIZE);

        return response;
}
public: static int Write(std::string message, int fd) {
        const int size = message.size();
//        int bytes_written = 0;
//        do {
//            bytes_written = write(fd, &me, PAGE_SIZE);
//
//            if (bytes_read == -1) {
//                return "";
//            }
//            buffer[bytes_read] = '\0';
//            std::cout << buffer << '\n';
//            response += buffer;
//
//        } while()
        auto code = write(fd, message.c_str(), message.size());

        if (code == -1) {
            perror("Internal write error");
            return 1;
        }

        return 0;
}
};
#endif //CDN_NODE_IO_H
