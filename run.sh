#!/bin/bash

#!/bin/bash
echo "select the operation ************"
echo "  1) Test 2 process, 100 msg/process"
echo "  2) Test 4 process, 100 msg/process"
echo "  3) Test 15 process, 150 msg/process"
echo "  4) Quit"



eval "rm -rf logs/"
eval "cd src/"
eval "make clean > /dev/null"
eval "make > /dev/null"
eval "rm ../conf/7p?*.conf > /dev/null"

run="./run"

cp ../conf/7p.conf ../conf/7p1.conf
echo -e "\nlport:1111\nid:1" >> ../conf/7p1.conf

cp ../conf/7p.conf ../conf/7p2.conf
echo -e "\nlport:2222\nid:2" >> ../conf/7p2.conf


cp ../conf/7p.conf ../conf/7p3.conf
echo -e "\nlport:3333\nid:3" >> ../conf/7p3.conf


cp ../conf/7p.conf ../conf/7p4.conf
echo -e "\nlport:4444\nid:4" >> ../conf/7p4.conf


cp ../conf/7p.conf ../conf/7p5.conf
echo -e "\nlport:5555\nid:5" >> ../conf/7p5.conf


cp ../conf/7p.conf ../conf/7p6.conf
echo -e "\nlport:6666\nid:6" >> ../conf/7p6.conf

cp ../conf/7p.conf ../conf/7p7.conf
echo -e "\nlport:7777\nid:7" >> ../conf/7p7.conf

read TH   

if (( $TH==2 )) || (( $TH==4 ));then
    echo "tuong xau xau"
    echo -e "\nbyzantine" >> ../conf/7p1.conf

    echo -e "\nbyzantine" >> ../conf/7p2.conf
fi

if (( $TH==3 || $TH==5 ));then
    echo "tuong xau xau xau"
    echo -e "\nbyzantine" >> ../conf/7p1.conf
    echo -e "\nbyzantine" >> ../conf/7p2.conf
    echo -e "\nbyzantine" >> ../conf/7p3.conf
    echo -e "\nbyzantine" >> ../conf/7p4.conf
    echo -e "\nbyzantine" >> ../conf/7p5.conf
fi

sleep 1

if (( $TH==1 || $TH==2 || $TH==3 ));then
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
else
    $run ../conf/7p1.conf 1 &
    pid1=$!
    $run ../conf/7p2.conf 1 &
    pid2=$!
    $run ../conf/7p3.conf 1 &
    pid3=$!
    $run ../conf/7p4.conf 1 &
    pid4=$!
    $run ../conf/7p5.conf 1 &
    pid5=$!
    $run ../conf/7p6.conf 1 &
    pid6=$!
    $run ../conf/7p7.conf 1 &    
    pid7=$!
fi

sleep 3
wait $pid1
wait $pid2
wait $pid3
wait $pid4
wait $pid5
wait $pid6
wait $pid7

echo -e "\e[1m\e[36mAll processes have been execute.\tView logs file in folder logs.\e[0m"


