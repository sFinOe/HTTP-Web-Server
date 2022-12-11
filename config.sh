#!/bin/bash

export _python_path=$(which python3)
export _node_path=$(which node)
export _user_path=$(pwd)

echo "
server 
{         

	include			config/mime.types;

	listen           6060;
	host			127.0.0.1;
	client_max_body_size 10m;

	error_page_400	"$_user_path"/public_html/html_pages/error_400.html;
	error_page_403	"$_user_path"/public_html/html_pages/error_403.html;
	error_page_404	"$_user_path"/public_html/html_pages/error_404.html;
	error_page_405	"$_user_path"/public_html/html_pages/error_405.html;
	error_page_413	"$_user_path"/public_html/html_pages/error_413.html;
	error_page_500	"$_user_path"/public_html/html_pages/error_500.html;

	page_200_ok			"$_user_path"/public_html/html_pages/200_ok.html;
	page_200_delete		"$_user_path"/public_html/html_pages/delete_200.html;
	page_201_created	"$_user_path"/public_html/html_pages/created_201.html;
	page_204_no_content	"$_user_path"/public_html/html_pages/no_content_204.html;

	location / 
	{
		root "$_user_path"/public_html;
		index index.html;
		allow [GET];
		autoindex on;
	}

	location /upload {

		root "$_user_path"/public_html;
		allow_upload on;
		upload_at "$_user_path"/public_html/trash;
		allow [POST, GET];
	}

	location /trash
	{
		root "$_user_path"/public_html;
		autoindex off;
		allow [GET, DELETE];
	}

	location /cgi-bin
	{
		root "$_user_path"/public_html;
		type_cgi js;
		cgi_bin "$_node_path";
		allow [GET, POST];

	}
	location /delete
	{
		root "$_user_path"/public_html;
		allow [GET, DELETE];
	}
	location /redirect
	{
		root "$_user_path"/public_html;
		return_301 https://disposkill.com;
	}
}
" > config/default.conf

echo "
server 
{         

	include			config/mime.types;

	listen           6060;
	host			127.0.0.1;
	client_max_body_size 10m;

	error_page_400	"$_user_path"/public_html/html_pages/error_400.html;
	error_page_403	"$_user_path"/public_html/html_pages/error_403.html;
	error_page_404	"$_user_path"/public_html/html_pages/error_404.html;
	error_page_405	"$_user_path"/public_html/html_pages/error_405.html;
	error_page_413	"$_user_path"/public_html/html_pages/error_413.html;
	error_page_500	"$_user_path"/public_html/html_pages/error_500.html;

	page_200_ok			"$_user_path"/public_html/html_pages/200_ok.html;
	page_200_delete		"$_user_path"/public_html/html_pages/delete_200.html;
	page_201_created	"$_user_path"/public_html/html_pages/created_201.html;
	page_204_no_content	"$_user_path"/public_html/html_pages/no_content_204.html;

	location / 
	{
		root "$_user_path"/public_html;
		index index.html;
		allow [GET];
		autoindex on;
	}

	location /upload {

		root "$_user_path"/public_html;
		allow_upload on;
		upload_at "$_user_path"/public_html/trash;
		allow [POST, GET];
	}

	location /trash
	{
		root "$_user_path"/public_html;
		autoindex off;
		allow [GET, DELETE];
	}

	location /cgi-bin
	{
		root "$_user_path"/public_html;
		type_cgi js, py;
		cgi_bin_js "$_node_path";
		cgi_bin_py "$_python_path";
		allow [GET, POST];

	}
	location /delete
	{
		root "$_user_path"/public_html;
		allow [GET, DELETE];
	}
	location /login 
	{
		root "$_user_path"/public_html;
		allow [GET, POST];
	}
	location /signup
	{
		root "$_user_path"/public_html;
		allow [GET, POST];
	}
	location /game
	{
		root "$_user_path"/public_html;
		allow [GET];
	}
	location /img_convertor
	{
		root "$_user_path"/public_html;
		allow [GET, POST];
	}
	location /generate_image
	{
		root "$_user_path"/public_html;
		allow [GET, POST];
	}
	location /redirect
	{
		root "$_user_path"/public_html;
		return_301 https://disposkill.com;
	}
}
" > config/bonus.conf

echo "types {

    html                                    text/html;
    css                                     text/css;
    xml                                     text/xml;
    gif                                     image/gif;
    jpg                                     image/jpeg;
    js                                      application/javascript;
    atom                                    application/atom+xml;
    rss                                     application/rss+xml;             

    mml                                     text/mathml;
    txt                                     text/plain;
    jad                                     text/vnd.sun.j2me.app-descriptor;
    wml                                     text/vnd.wap.wml;
    htc                                     text/x-component;

    png                                     image/png;
    tif                                     image/tiff;
    wbmp                                    image/vnd.wap.wbmp;
    ico                                     image/x-icon;
    jng                                     image/x-jng;
    bmp                                     image/x-ms-bmp;
    svg                                     image/svg+xml;
    webp                                    image/webp;

    woff                                    application/font-woff;
    jar                                     application/java-archive;
    json                                    application/json;
    hqx                                     application/mac-binhex40;
    doc                                     application/msword;
    pdf                                     application/pdf;
    ps                                      application/postscript;
    rtf                                     application/rtf;
    m3u8                                    application/vnd.apple.mpegurl;
    xls                                     application/vnd.ms-excel;
    eot                                     application/vnd.ms-fontobject;
    ppt                                     application/vnd.ms-powerpoint;
    wmlc                                    application/vnd.wap.wmlc;
    kml                                     application/vnd.google-earth.kml+xml;
    kmz                                     application/vnd.google-earth.kmz;
    7z                                      application/x-7z-compressed;
    cco                                     application/x-cocoa;
    jardiff                                 application/x-java-archive-diff;
    jnlp                                    application/x-java-jnlp-file;
    run                                     application/x-makeself;
    pl                                      application/x-perl;
    prc                                     application/x-pilot;
    rar                                     application/x-rar-compressed;
    rpm                                     application/x-redhat-package-manager;
    sea                                     application/x-sea;
    swf                                     application/x-shockwave-flash;
    sit                                     application/x-stuffit;
    tcl                                     application/x-tcl;
    der                                     application/x-x509-ca-cert;
    xpi                                     application/x-xpinstall;
    xhtml                                   application/xhtml+xml;
    xspf                                    application/xspf+xml;
    zip                                     application/zip;

    bin                                     application/octet-stream;
    deb                                     application/octet-stream;
    dmg                                     application/octet-stream;
    iso                                     application/octet-stream;
    msi                                     application/octet-stream;

    docx                                    application/vnd.openxmlformats-officedocument.wordprocessingml.document;
    xlsx                                    application/vnd.openxmlformats-officedocument.spreadsheetml.sheet;
    pptx                                    application/vnd.openxmlformats-officedocument.presentationml.presentation;

    mid                                     audio/midi;
    mp3                                     audio/mpeg;
    ogg                                     audio/ogg;
    m4a                                     audio/x-m4a;
    ra                                      audio/x-realaudio;

    3gpp                                    video/3gpp;
    ts                                      video/mp2t;
    mp4                                     video/mp4;
    mpeg                                    video/mpeg;
    mov                                     video/quicktime;
    webm                                    video/webm;
    flv                                     video/x-flv;
    m4v                                     video/x-m4v;
    mng                                     video/x-mng;
    asx                                     video/x-ms-asf;
    wmv                                     video/x-ms-wmv;
    avi                                     video/x-msvideo;
}" > config/mime.types