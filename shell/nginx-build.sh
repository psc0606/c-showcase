#!/bin/sh

pcre="pcre-8.39"
openssl="openssl-1.1.0c"
zlib="zlib-1.2.8"
cd ./addon
if [ -d $pcre ]; then
	rm -fr $pcre
fi

if [ -d $openssl ]; then
	rm -fr $openssl
fi

if [ -d $zlib]; then
	rm -fr $zlib
fi

tar -xvf $pcre.tar.bz2
tar -zxvf $openssl.tar.gz
tar -zxvf $zlib.tar.gz
cd -

./configure \
	--prefix=/etc/nginx \
	--sbin-path=/usr/sbin/nginx \
	--conf-path=/etc/nginx/nginx.conf \
	--error-log-path=/var/log/nginx/error.log \
	--http-log-path=/var/log/nginx/access.log \
	--pid-path=/var/run/nginx.pid \
	--lock-path=/var/run/nginx.lock \
	--http-client-body-temp-path=/var/cache/nginx/client_temp \
	--http-proxy-temp-path=/var/cache/nginx/proxy_temp \
	--http-fastcgi-temp-path=/var/cache/nginx/fastcgi_temp \
	--http-uwsgi-temp-path=/var/cache/nginx/uwsgi_temp \
	--http-scgi-temp-path=/var/cache/nginx/scgi_temp \
	--user=nginx \
	--group=nginx \
	--with-http_ssl_module \
	--with-http_realip_module \
	--with-http_addition_module \
	--with-http_sub_module \
	--with-http_dav_module \
	--with-http_flv_module \
	--with-http_mp4_module \
	--with-http_gunzip_module \
	--with-http_gzip_static_module \
	--with-http_random_index_module \
	--with-http_secure_link_module \
	--with-http_stub_status_module \
	--with-http_auth_request_module \
	--with-mail \
	--with-mail_ssl_module \
	--with-file-aio \
	--with-pcre=./addon/$pcre \
	--with-openssl=./addon/$openssl \
	--with-zlib=./addon/"$zlib"
#    --with-ld-opt="-lstdc++" \
#    --with-ld-opt="-ldl" \

#	--with-http_spdy_module \
#	--with-ipv6 \
#	--with-debug \

make -j 4
#make install

