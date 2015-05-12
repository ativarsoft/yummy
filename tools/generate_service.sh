#!/bin/bash

#
# yummy/tools/generate_service.sh
#
#  Copyright (C) 2013-2015  Mateus de Lima Oliveira
#

# this script needs bash to evaluate echo backslash escapes.

# $1: in

echo ''
echo '#include <cheese.h>'
echo '#include "debug.h"'
echo ''

sed -e '/^$/d; s/#define //; s/ *[0-9]*$//' "$1" > tmp
sed -f - tmp << "EOF"
s/.*/\L&/;
s/^/static inline void /;
s/$/(int a, void *b, void **c, int d)\n{\n\tstruct args_s\n\t{\n\t};\n\t\n\ttype *r = b;\n\tstruct args_s *args = (struct args_s *) c;\n\t\n\tDEBUG_TRACE();\n\tDEBUG_CHECK_NUM_ARGS(d, );\n}\n/;
EOF

echo "static int service_dispatch(int a, void *b, void **c, int d)"
echo "{"
echo -e "\tswitch (a) {"
while read a
do
echo -e "\tcase $a:"
echo -e "\t$(echo $a | sed -e 's/.*/\L&/')(a, b, c, d);"
echo -e "\tbreak;"
done < tmp
echo -e "\tdefault:"
echo -e "\tDEBUG_UNKNOWN_MESSAGE();"
echo -e "\treturn 1;"
echo -e "\t}"
echo "}"

rm tmp
