#include <iostream>
#include <unistd.h>
#include "Client/Client.h"
#include "Contracts/IResponse.h"
#include "Contracts/IRequest.h"
#include "../Shared/IO/IO.h"
#include "Contracts/ImageShower.h"

auto logger = Logger::GetInstance().get();
int edge_server_port = 8080;
Result<IResponse*, std::string> SendRequestToEdge(int, IRequest* request);

int main(int argc, char *argv[]) {
    logger->setPrefix("CLIENT");
    logger->SetLogPreferences("", LogLevel::DEBUG, LogOutput::CONSOLE);

    if(argc != 4) {
        std::cout << "Usage " << argv[0] << " <location> " << " <command> " << " <payload>\n";
        exit(EXIT_FAILURE);
    }

    std::string location = argv[1];
    std::string command = argv[2], payload = argv[3];

    auto request = IRequest::Create(std::string(command + " " + payload + " " + location));
    auto result = SendRequestToEdge(edge_server_port, request);
    std::cout << "\n\nFinal response: ";
    if (result.IsFailure()) {
        std::cout << result.Error();
        return 0;
    }
    if (result.Value()->statusCode != 200) {
        std::cout << result.Value()->content;
        return 0;
    }

    if (request->command == "get-image-file") {
        ImageShower::ShowImage(result.Value()->content);
        return 0;
    }
    if (request->command == "get-text-file") {
        std::cout << result.Value()->content;
    }
    return 0;
}

Result<IResponse*, std::string> SendRequestToEdge(int edge_port, IRequest* request) {
    auto client = new Client(LOCALHOST, edge_port);
    int result = client->Connect();
    if (result == -1) {
        exit(EXIT_FAILURE);
    }
    if(result == -2) {
        logger->LogError("Edge server is not on.", "");
        return Result<IResponse*, std::string>("Edge is not connected");
    }
    if (write(client->GetDescriptor(), request->GetLocatedMessage().c_str(), request->GetLocatedMessage().size()) <= 0) {
        logger->LogError("Can't write to edge server");
        return Result<IResponse*, std::string>("Edge is not connected");
    }

    std::string response_buffer = IO::Read(client->GetDescriptor());
    if (response_buffer.size() == 0) {
        logger->LogError("Edge server didn't respond");
        return Result<IResponse*, std::string>("Edge is not connected");
    }

    if (close(client->GetDescriptor()) == -1) {
        logger->LogError("Channel with edge server cannot be closed");
        perror("Closing edge server");
        return Result<IResponse*, std::string>("Edge server error");
    }

    auto response = new IResponse(response_buffer);

    if (response->statusCode == 302) {
        logger->LogInformation("Sending request to cdn node {PORT}", "", response->content);
        auto client = new Client(LOCALHOST, atoi(response->content.c_str()));
        int result = client->Connect();
        if (result == -1) {
            exit(EXIT_FAILURE);
        }
        if(result == -2) {
            logger->LogError("CDN server is not on.", "");
        }

        if (write(client->GetDescriptor(), request->GetMessage().c_str(), request->GetMessage().size()) <= 0) {
            logger->LogError("Assigned cdn not connected");
            return Result<IResponse*, std::string>("Cdn is not connected");
        }

        std::string buffer = IO::Read(client->GetDescriptor());
        if (close(client->GetDescriptor()) == -1) {
            logger->LogError("Assigned cdn not connected");
            perror("Closing cdn node");
            return Result<IResponse*, std::string>("Cdn error");
        }

        if (buffer.size() == 0) {
            return Result<IResponse*, std::string>("500 Server failed to respond");
        }
        return Result<IResponse*, std::string>(new IResponse(buffer));
    } else {
        return Result<IResponse*, std::string>(response_buffer);
    }
}
