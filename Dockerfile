# Install compiler
FROM ubuntu 
# Install cmake
RUN apt-get update && apt-get -y install cmake
# Set description
LABEL description = "Container for use with visual studio"
# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /usr/src/IPAddress
# Specify work directory
WORKDIR /usr/src/IPAddress
# Check CMake version
CMD [ "cmake --version" ]
# Run CMake and build cmake generated files 
RUN cmake --version && cmake . && cmake --build .
# Run tests 
