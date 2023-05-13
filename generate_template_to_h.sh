#!/bin/sh
SCRIPT_DIR=$(cd $(dirname $0); pwd)
cd ${SCRIPT_DIR}

cd ./src/infrastructure/http/template

rm *.html.h

for template_file in $( ls . | grep .html ); do
    echo $template_file

    xxd -i ${template_file} > ${template_file}.h
done

cd ${SCRIPT_DIR}
cd ./src/infrastructure/http/static
for template_file in $(find . -type f -not -name \*.h); do
    echo $template_file
    xxd -i ${template_file} > ${template_file}.h
done

cd ${SCRIPT_DIR}
cd ./src/infrastructure/mysql/template

rm *.sql.h

for template_file in $( ls . | grep .sql ); do
    echo $template_file

    xxd -i ${template_file} > ${template_file}.h
done