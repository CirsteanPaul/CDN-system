//
// Created by Paul Cirstean on 04.12.2023.
//

#include "Cache.h"

std::shared_ptr<Cache> Cache::instance;

std::shared_ptr<Cache> Cache::GetInstance() {
    if (instance == nullptr) {
        instance = std::shared_ptr<Cache>(new Cache());
    }

    return instance;
}

File* Cache::CheckCache(std::string key, FileType type) {
    if (type == FileType::ImageFile) {
        auto file = image_map.find(key);
        if (file == image_map.end()) {
            return nullptr;
        }

        if(difftime(file->second->timestamp, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) < 0) {
            image_map.erase(file);
            return nullptr;
        }
        return file->second;
    }

    auto file = text_map.find(key);
    if (file == text_map.end()) {
        return nullptr;
    }

    if(difftime(file->second->timestamp, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) < 0) {
        text_map.erase(file);
        return nullptr;
    }

    return file->second;
}

void Cache::AddItemToCache(std::string name, std::string content, FileType type) {
    auto new_file = new File();
    new_file->content = content;
    new_file->timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + std::chrono::seconds(30));
    if (type == FileType::ImageFile) {
        image_map[name] = new_file;
    } else {
        text_map[name] = new_file;
    }
}
