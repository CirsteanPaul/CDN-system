//
// Created by Paul Cirstean on 29.11.2023.
//

#ifndef ORIGIN_SERVER_IREQUEST_H
#define ORIGIN_SERVER_IREQUEST_H
#include <iostream>
#include "IResponse.h"
#include "../../Shared/Result.h"

class IRequest;

typedef Result<const IResponse*, const std::string> HandleRequestResult;

class IRequest {
public: virtual HandleRequestResult Handle() = 0;
};


#endif //ORIGIN_SERVER_IREQUEST_H
