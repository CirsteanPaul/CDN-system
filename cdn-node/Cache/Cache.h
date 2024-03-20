//
// Created by Paul Cirstean on 04.12.2023.
//

#ifndef CDN_NODE_CACHE_H
#define CDN_NODE_CACHE_H

#include <iostream>
#include <unordered_map>

enum FileType {
    ImageFile,
    TextFile,
    None
};
class File {
public: std::string content;
public: std::time_t timestamp;
};
class Cache {
private: std::unordered_map<std::string, File*> text_map;
private: std::unordered_map<std::string, File*> image_map;

public: static std::shared_ptr<Cache> GetInstance();
public: File* CheckCache(std::string, FileType);
public: void AddItemToCache(std::string, std::string, FileType);

private: static std::shared_ptr<Cache> instance;

};


#endif //CDN_NODE_CACHE_H
