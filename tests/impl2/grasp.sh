#!/bin/bash

function save {

    str=$(cat output)
    printf "$str\n"

    best=$(printf "$str" | grep -o -E 'Best: [-+0-9.e]+' | cut -d ' ' -f2 )

    printf "$1;$best\n" >> $2

}

function run {

    for f in ../../instances/*
    do
        # keep only the name of the instance (not "instances/")
        a=$(printf "$f" | grep -o -E 'instances/.*' |cut -d '/' -f2)

        # echo "$a"

        if [ "$a" != "bestSolutions.txt" ]; then

            # size of the instance
            size=$(printf "$a" | grep -o -E '[-+0-9]+' | cut -d ' ' -f2 | head -1 )
            # echo "size: $size"

            timeLimit=-1
            if [ $size == 100 ]; then
                timeLimit=281946
            else
                timeLimit=30085
            fi

            # timeLimit=2

            # echo "time limit: $timeLimit"

            for s in ${seed[@]}
            do
                name="sa/sa_seed_$s"
                # echo "name: $name"
                ./../../code/flowshopWCT "--instance" $f "--algo" "sa" "--seed" $s "--time" $timeLimit > output
                save $a $name
            done

        fi
    done

}

declare -a seed=("12345" "67891" "01112" "13141" "51617")

run

rm output
