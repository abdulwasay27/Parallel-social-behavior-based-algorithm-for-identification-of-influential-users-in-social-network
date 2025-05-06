FROM ubuntu:22.04

# Install MPI, build tools, CMake, and SSH server
RUN apt-get update && apt-get install -y --no-install-recommends \
    openmpi-bin openmpi-common libopenmpi-dev \
    cmake make g++ \
    openssh-server \
  && rm -rf /var/lib/apt/lists/*

# Ensure the SSH daemon directory exists
RUN mkdir -p /var/run/sshd

# Generate SSH key for root and set up authorized_keys
RUN mkdir -p /root/.ssh && chmod 700 /root/.ssh \
 && ssh-keygen -t rsa -N "" -f /root/.ssh/id_rsa \
 && cat /root/.ssh/id_rsa.pub >> /root/.ssh/authorized_keys \
 && chmod 600 /root/.ssh/authorized_keys

# Allow root SSH login
RUN sed -i 's/#PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config

# Allow OpenMPI to run as root (since Docker containers often run as root)
ENV OMPI_ALLOW_RUN_AS_ROOT=1 OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# Expose SSH port
EXPOSE 22

# Set working directory and copy project files
WORKDIR /app
COPY . .

# Build the MPI project using CMake
RUN mkdir -p build && cd build && cmake .. && make \
 && cp psb_algorithm /app

# By default, run the SSH daemon
CMD ["/usr/sbin/sshd", "-D"]
