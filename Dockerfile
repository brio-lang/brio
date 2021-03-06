FROM ubuntu:bionic

LABEL maintainer="hi@trentseed.com"

RUN apt-get -y update

RUN apt-get -y install make g++ libcurl4-openssl-dev libfcgi-dev

WORKDIR /opt/brio-lang

COPY . /opt/brio-lang

RUN ["make", "all"]

CMD ["/opt/brio-lang/bin/brio", "-i"]