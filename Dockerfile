############################################################
# Dockerfile to build SABCEMM container images
# Based on Ubuntu
############################################################

# Set the base image to sabcemm_base
FROM sabcemm/sabcemm_base:1.0

# File Author / Maintainer
MAINTAINER Simon Cramer

ENV DEBIAN_FRONTEND noninteractive

# Update the repository sources list
RUN apt-get update
RUN apt-get upgrade -y


################## BEGIN SABCEMM ######################

RUN git clone https://github.com/SABCEMM/SABCEMM.git /home/SABCEMM
RUN git -C '/home/SABCEMM' submodule init
RUN git -C '/home/SABCEMM' submodule update

RUN cd /home/SABCEMM/ && git pull && mkdir build
RUN cd /home/SABCEMM/build && cmake .. -DWITH_HDF5:BOOL=ON -DWITH_INTEL_MKL:BOOL=ON -DWITH_OPENMP:BOOL=ON -DWITH_PROGBAR:BOOL=OFF
RUN cd /home/SABCEMM/build && make


##################### INSTALLATION END #####################

ENTRYPOINT ["/bin/bash"]
