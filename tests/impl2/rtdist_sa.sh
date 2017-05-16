#!/bin/bash

function save {

    str=$(cat output_rtsa)
    echo "$str"

    rt=$(printf "$str" | grep -o -E 'imp:.*' | cut -d ':' -f2 | tr " " "\n")

    echo "res grep"
    echo "$rt"

    printf "$rt\n" >> $2

}

function run {

    # keep only the name of the instance (not "instances/")
    a=$(printf "$1" | grep -o -E 'instances/.*' |cut -d '/' -f2)

    # echo "$a"

    if [ "$a" != "bestSolutions.txt" ]; then

        # size of the instance
        size=$(printf "$a" | grep -o -E '[-+0-9]+' | cut -d ' ' -f2 | head -1 )
        # echo "size: $size"

        timeLimit=300850

        timeLimit=3000
        # echo "time limit: $timeLimit"

        for s in ${seed[@]}
        do
            name="rtsa/rtsa_$a"
            # echo "name: $name"
            ./../../code/flowshopWCT "--instance" $1 "--algo" "sa" "--seed" $s "--time" $timeLimit > output_rtsa
            save $a $name
        done

    fi
}

# declare -a seed=("12345" "67891" "01112" "13141" "51617" "18192" "02122" "23242" "52627" "28293" "03132" "33343" "53637" "38394" "04142" "43444" "54647" "48495" "51525" "35455" "56575" "85960" "61626" "36465" "66676")

declare -a seed=("12345" "6789")

run $1

rm output_rtsa
