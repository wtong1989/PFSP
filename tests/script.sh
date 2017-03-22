#!/bin/bash

function save {

    str=$(cat output)
    printf "$str\n"

    best=$(printf "$str" | grep -o -E 'Best: [-+0-9.e]+' | cut -d ' ' -f2 )
    t=$(printf "$str" | grep -o -E 'Time: [-+0-9.e]+' | cut -d ' ' -f2 )

    printf "$1 $2 $3 $4 $best $t\n" >> res.txt

}

function run {

    for i in ${pivot[@]}
    do
        for j in ${init[@]}
        do
            for k in ${neigh[@]}
            do
                echo $i $j $k
                ./../code/flowshopWCT $1 $i $j $k > output

                a=$(printf "$1" | grep -o -E 'instances/.*' |cut -d '/' -f2)

                save $a $i $j $k
            done
        done
    done

}

declare -a pivot=("--firstImp" "--bestImp")
declare -a init=("--srz" "--random")
declare -a neigh=("--transpose" "--exchange" "--insert")

rm res.txt

for f in ../instances/*
do
   printf $f
   run $f
done


rm output
