#!/bin/sh
if [ $# -eq 0 ]
  then
    echo "Please supply the name of your executable like ./autoTest.sh ./executableName"
    exit 0
fi

GREEN='\033[0;32m'
RED='\033[0;31m'
NORMAL='\033[0m'

for i in $(find . -name '*.output');
do
    # echo $i
    testName=$(echo $i | sed 's/.output//') 
    $1 < "$testName.input" > temp.gameout
    testOutput=$(diff -w  $i ./temp.gameout)
    if [ ${#testOutput} -gt 0 ]
      then 
        echo "${RED}$i is incorrect${NORMAL}"
    else 
        echo "${GREEN}$i is correct${NORMAL}"
    fi
done
rm ./temp.gameout
exit 0