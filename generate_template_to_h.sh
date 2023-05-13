#!/bin/sh
SCRIPT_DIR=$(cd $(dirname $0); pwd)
cd ${SCRIPT_DIR}

cd ./src/infrastructure/http/template

find . -type f -name \*.h -exec rm {} \;

for template_file in $(find . -type f -not -name \*.h); do
    echo $template_file
    xxd -i ${template_file: 2} > ${template_file}.h
done

cd ${SCRIPT_DIR}
cd ./src/infrastructure/http/static

find . -type f -name \*.h -exec rm {} \;

for template_file in $(find . -type f -not -name \*.h); do
    echo $template_file
    xxd -i ${template_file: 2} > ${template_file}.h
done

cd ${SCRIPT_DIR}
cd ./src/infrastructure/mysql/template

find . -type f -name \*.h -exec rm {} \;

for template_file in $(find . -type f -not -name \*.h); do
    echo $template_file
    xxd -i ${template_file: 2} > ${template_file}.h
done
