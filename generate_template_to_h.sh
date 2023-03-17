#!/bin/sh
cd ./src/infrastructure/http/template

rm *.html.h

for template_file in $( ls . | grep .html ); do
    echo $template_file

    xxd -i ${template_file} > ${template_file}.h
done
