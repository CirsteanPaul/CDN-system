//
// Created by Paul Cirstean on 29.11.2023.
//

#include <unistd.h>
#include "TextRequest.h"
#include <fstream>

HandleRequestResult TextRequest::Handle() {
    int returnedCode = access(path.c_str(), R_OK);
    if (returnedCode) {
        perror("Error");
        return HandleRequestResult(strerror(errno));
    }

    std::ifstream ifs(path.c_str());
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    ifs.close();
    const IResponse* response = new IResponse(200, content);
    return HandleRequestResult(response);
}