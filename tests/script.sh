#!/bin/bash

function save {

    str=$(cat output)
    printf "$str\n"

    best=$(printf "$str" | grep -o -E 'Best: [-+0-9.e]+' | cut -d ' ' -f2 )
    t=$(printf "$str" | grep -o -E 'Time: [-+0-9.e]+' | cut -d ' ' -f2 )

    printf "$1;$best;$t\n" >> $2

}

function run {

    # iterative improvement
    for i in ${pivot[@]}
    do
        for j in ${init[@]}
        do
            for k in ${neigh[@]}
            do

                name="pfsp-ii-$i$j$k.txt"
                rm $name

                printf "instance;best;time\n" >> $name

                for f in ../instances/*
                do
                    # keep only the name of the instance (not "instances/")
                    a=$(printf "$f" | grep -o -E 'instances/.*' |cut -d '/' -f2)

                    if [ "$a" != "bestSolutions.txt" ]; then
                        echo $i $j $k
                        ./../code/flowshopWCT $f $i $j $k > output
                        save $a $name
                    fi
                done
            done
        done
    done

    # vnd heuristic
    for i in ${init[@]}
    do
        for j in ${order[@]}
        do

            name="pfsp-vnd-$i$j.txt"
            rm $name

            printf "instance;best;time\n" >> $name

            for f in ../instances/*
            do
                # keep only the name of the instance (not "instances/")
                a=$(printf "$f" | grep -o -E 'instances/.*' |cut -d '/' -f2)

                if [ "$a" != "bestSolutions.txt" ]; then
                    echo $i $j $k
                    ./../code/flowshopWCT $f "--vnd" $i $j > output
                    save $a $name
                fi
            done
        done
    done

}

declare -a pivot=("--firstImp" "--bestImp")
declare -a init=("--srz" "--random")
declare -a neigh=("--transpose" "--exchange" "--insert")
declare -a order=("--exfirst" "--insfirst")

run

rm output
