#!/usr/bin/env bash

str=$(cat res.csv)
best=$(cat ../instances/bestSolutions.txt)

#printf "$best"

#besti=$(printf "$best" | grep -o -E '50_20_01.*' | cut -d ',' -f2)

#printf "$besti"

rm res2.csv

for line in $str
do

    #line=$(printf "$line" | tr ',' '\n')
    #echo $line
    IFS=',' read -r -a array <<< "$line"

    echo $array

    besti=$(printf "$best" | grep -o -E "${array[0]}.*" | cut -d ',' -f2)

    for elt in "${array[@]}"
    do
        echo -n "$elt," >> res2.csv
    done
    echo "$besti" >> res2.csv

done
