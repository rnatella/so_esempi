FROM ubuntu:20.04
RUN apt update 
ARG DEBIAN_FRONTEND=noninteractive
RUN apt install -y apache2 
RUN apt install -y apache2-utils 
RUN apt clean 
RUN echo "CIAO MONDO" > /var/www/html/ciao.html
EXPOSE 80
CMD ["apache2ctl", "-D", "FOREGROUND"]
