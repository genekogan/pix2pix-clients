

## pix2pixHD-server

(better documentation coming soon)

requires: ofxRunway, ofxHTTP, ofxIO, ofxSSLManager, ofxNetworkUtils, ofxSandboxTracker, ofxCv, ofxPoco

1) install Docker + nvidia-docker

https://docs.docker.com/install/linux/docker-ce/ubuntu/#set-up-the-repository
https://github.com/NVIDIA/nvidia-docker (quickstart)

2) build docker image for pix2pixHD server

    git clone https://github.com/cvalenzuela/pix2pixHD
    cd pix2pixHD
    docker build -t pix2pixhd .

3) place `latest_net_G.pth` into new directory `checkpoints/model_name`.

4) modify `server.py` to point to right checkpoint subdirectory.

5) launch container.

    nvidia-docker run --rm -it -v /home/gene/projects/pix2pixHD:/workspace/pix2pixHD \
     -p 23100:23100 \
     --shm-size 16G \
     pix2pixhd:latest bash

6) start server from container

    python server.py

7) launch openframeworks client
