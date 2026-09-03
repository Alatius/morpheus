FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

# Install dependencies for compiling and for the data generation scripts
RUN apt-get update -qq && apt-get install -qq -y \
    build-essential \
    flex \
    perl

ADD . /morpheus
WORKDIR /morpheus

# fix line endings / make executable for the perl script (still needed)
RUN chmod +x stemlib/Greek/getentities.pl && \
    sed -i 's/\r$//' stemlib/Greek/getentities.pl

RUN make && \
    echo "salve" | MORPHLIB=stemlib bin/cruncher -L