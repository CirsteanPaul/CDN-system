//
// Created by Paul Cirstean on 29.11.2023.
//

#ifndef SHARED_RESULT_H
#define SHARED_RESULT_H

template<typename TValue, typename TError>
class Result {
private: bool isSuccess = false;
private: bool isFailure = false;
private: std::unique_ptr<std::variant<TValue, TError>> variants = std::make_unique<std::variant<TValue, TError>>();
public:Result(TValue value) {
    auto results = std::variant<TValue, TError> (value);
    variants = std::make_unique<std::variant<TValue, TError>>(results);
    isSuccess = true;
    isFailure = false;
};
public:Result(TError error) {
        auto results = std::variant<TValue, TError> (error);
        variants = std::make_unique<std::variant<TValue, TError>>(results);
        isSuccess = false;
        isFailure = true;
};

public: bool IsSuccess() const { return isSuccess; }
public: bool IsFailure() const { return isFailure; }
public: const TValue Value() const { return std::get<TValue>(*variants.get()); }
public: const TError Error() const { return std::get<TError> (*variants.get()); }
};
#endif //SHARED_RESULT_H
