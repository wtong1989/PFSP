declare -a pivot=("--firstImp" "--bestImp")
declare -a init=("--srz" "--random")
declare -a neigh=("--transpose" "--exchange" "--insert")

#for f in ../instances/*
#do
#    printf $f
#    printf '\n'
#done

#echo ${pivot[1]}

for i in ${pivot[@]}
do
    for j in ${init[@]}
    do
        for k in ${neigh[@]}
        do
            echo $i $j $k
            ./../code/flowshopWCT ../instances/50_20_01 $i $j $k > output
        done
    done
done
