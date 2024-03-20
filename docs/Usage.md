1. Info
  - Port 8002 en
  - Port 8004 fr
  - Port 8006 rus
  - Port 8008 china
  - Port 8010 usa
2. Show the app in general:
  - Open all servers: ./origin_server & ./edge_server & ./cdn_node 8002 & ./cdn_node 8004 & ./cdn_node 8006 & ./cdn_node 8008 & ./cdn_node 8010 &
  - ./client en get-image-file y.png   image flow.
  - ./client fr get-text-file data.txt   text flow.
  - ./client rus get-image-file yy.png     not found.
  - ./client usa get-text-file yy.png        not found.
  - ./client en get-tex-file x  Command unknown

3. Show the system works without origin
  - kill -9 $(lsof -ti:8001)     - close origin.
  - ./client en get-image-file y.png    show the cached file.
  - wait 30 seconds so cache can expire.
  - ./client en get-image-file y.png   should return internal server error.
4. Show the system works without a node
  - kill -9 $(lsof -ti:8002)   - closing en node
  - ./client en get-text-file test.txt   - client will be routed to another node

5. Show the system is in idle without the edge server
  - kill -9 $(lsof -ti:8080)
  - The client will not be useful without the edge server
  - All other apps are on
  - special client which will send the request to a cdn node(8004 let's say).
  - ./edge-server

6. Show the load balancing
  - Reset the system
  - kill -9 $(lsof -ti:8001,8002,8004,8006,8008,8010,8080)
  - ./origin_server & ./edge_server & ./cdn_node 8002 & ./cdn_node 8004 & ./cdn_node 8006 & ./cdn_node 8008 & ./cdn_node 8010 &
  - ./client en get-text-file test.txt & ./client en get-text-file test.txt & ./client en get-text-file test.txt & ./client en get-text-file test.txt & ./client en get-text-file test.txt & ./client en get-text-file test.txt &
  - Because of the 5 seconds delay in the origin server 3 threads will be occupied in the 8002 cdn node.
  - Send another request to that server. You should be redirected to another cdn node.
  - ./client en get-text-file test.txt
  - Send the requests again to see if the cache helps.