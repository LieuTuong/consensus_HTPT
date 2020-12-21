#!/bin/bash

num_byzantine=$@
byzantine_arr=()
RANDOM=$$
for i in `seq $num_byzantine`
do
        R=$(($(($RANDOM%7))+1))
        if [[ " ${byzantine_arr[@]} " =~ " ${R} " ]]; then
            expr $num_byzantine + 1;
        else
            byzantine_arr+=( $R )
        fi
done

for t in ${byzantine_arr[@]}
do
  echo $t
done

eval "rm -rf logs/"
eval "cd src/"
eval "make clean > /dev/null"
eval "make > /dev/null"

run="./run"

cp ../conf/7p.conf ../conf/7p1.conf
echo -e "\nlport:1111\nid:1\nbyzantine" >> ../conf/7p1.conf
if [[ " ${byzantine_arr[@]} " =~ " 1 " ]]; then
    echo -e "\nbyzantine" >> ../conf/7p1.conf
fi

cp ../conf/7p.conf ../conf/7p2.conf
echo -e "\nlport:2222\nid:2\nbyzantine" >> ../conf/7p2.conf
if [[ " ${byzantine_arr[@]} " =~ " 2 " ]]; then
    echo -e "\nbyzantine" >> ../conf/7p2.conf
fi

cp ../conf/7p.conf ../conf/7p3.conf
echo -e "\nlport:3333\nid:3" >> ../conf/7p3.conf
if [[ " ${byzantine_arr[@]} " =~ " 3 " ]]; then
    echo -e "\nbyzantine" >> ../conf/7p3.conf
fi

cp ../conf/7p.conf ../conf/7p4.conf
echo -e "\nlport:4444\nid:4" >> ../conf/7p4.conf
if [[ " ${byzantine_arr[@]} " =~ " 4 " ]]; then
    echo -e "\nbyzantine" >> ../conf/7p4.conf
fi

cp ../conf/7p.conf ../conf/7p5.conf
echo -e "\nlport:5555\nid:5" >> ../conf/7p5.conf
if [[ " ${byzantine_arr[@]} " =~ " 5 " ]]; then
    echo -e "\nbyzantine" >> ../conf/7p5.conf
fi

cp ../conf/7p.conf ../conf/7p6.conf
echo -e "\nlport:6666\nid:6" >> ../conf/7p6.conf
if [[ " ${byzantine_arr[@]} " =~ " 6 " ]]; then
    echo -e "\nbyzantine" >> ../conf/7p6.conf
fi

cp ../conf/7p.conf ../conf/7p7.conf
echo -e "\nlport:7777\nid:7" >> ../conf/7p7.conf
if [[ " ${byzantine_arr[@]} " =~ " 7 " ]]; then
    echo -e "\nbyzantine" >> ../conf/7p7.conf
fi


$run ../conf/7p1.conf  &
pid1=$!
$run ../conf/7p2.conf  &
pid2=$!
$run ../conf/7p3.conf  &
pid3=$!
$run ../conf/7p4.conf  &
pid4=$!
$run ../conf/7p5.conf  &
pid5=$!
$run ../conf/7p6.conf  &
pid6=$!
$run ../conf/7p7.conf  &
pid7=$!

sleep 10
wait $pid1
wait $pid2
wait $pid3
wait $pid4
wait $pid5
wait $pid6
wait $pid7

rm ../conf/7p?*.conf

echo -e "\e[1m\e[36mAll processes have been execute.\tView logs file in folder logs.\e[0m"


