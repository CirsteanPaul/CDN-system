# CDN - Content Delivery Network

## Description

CDN is a simulation of a content delivery network comprising four main servers: Client, CDN-manager, CDN-node, and origin server. This application allows clients to receive text files and images via TCP/IP connection. The network features built-in health checks and auto-repair options, enhancing system resilience. Additionally, the project follows an architecture similar to an MVC application, utilizing the Result pattern and dependency injection. Although the servers' locations are simulated, their coordinates mimic real-world scenarios.

## Languages and Utilities Used

- C++
- TCP

## Implementation Details

The application involves four principal actors:

- **Client:** An external actor who can send requests to the system to retrieve text files or images (static content). The client must implement logic for request redirection.
- **CDN-manager:** The brain of the system, possessing knowledge of all CDN-nodes (their statuses, the number of client servers, etc.). It redirects the client to the best CDN node capable of providing the requested information. The CDN-manager maintains direct connections with all CDN nodes to access real-time status information.
- **CDN-node:** Responsible for delivering specific data to the client, featuring an internal cache system that enhances query speeds.
- **Origin server:** The actual server of users, analogous to the origin server in CDN services like CloudFlare. It connects to the CDN infrastructure.

For more details, refer to the [implementation documentation](https://github.com/CirsteanPaul/CDN-system/blob/main/docs/Docuemntation.pdf).

## Available Commands

- There are no special commands for this project; each server runs as an instance of a process.
- **Client Usage:** Specify the client's location (en, fr, usa, rus, china), the command (get-text-file, get-image-file), and the file path (e.g., `./client en get-text-file ./data.txt`).
- **CDN-node Usage:** Specify the port. Note: The specified port will be used for the application, and the next port (e.g., if you input 8000, the next port used will be 8001) will be utilized for synchronization.
- **CDN-manager:** No arguments required; it always runs on port 8080.
- **Origin Server:** No arguments required; it always runs on port 8001.

## Examples

Refer to the [usage documentation](https://github.com/CirsteanPaul/CDN-system/blob/main/docs/Usage.md) for examples.
