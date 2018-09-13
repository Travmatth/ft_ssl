i="./ft_ssl"
if ! [ -e "$i" ]
	then
	echo "put ft_ssl here!"
	exit
fi

# INPUT PARSE

scriptname="test.sh"

if ! [ "$#" -gt 0 ]
then
	echo "USAGE: sh runtest.sh number_of_tests [-nows]"
	echo "    -nows    a flag that disables newlines for every 64 symbol in base64"
	exit
fi
if [ "$#" -eq 2 ]
then
	if [ "$2" == "-nows" ]
		then
		scriptname="test_noWSbase64.sh"
	else
		echo "invalid flag "$2
		exit
	fi
fi

# PRE
unt=$1
((unt++))
echo "If the file is not empty, HAHAHHA LOH\nThe plaintext is the text provided below" > fails
echo "and both of its opening and closing '\"' should be excluded" >> fails
echo "" >> fails
cp fails fails_bonus
count=0
count_bonus=0
make -C ./resources/ test

i="./resources/files/"
if ! [ -e "$i" ]
	then
	mkdir ./resources/files
fi
i="./resources/results/"
if ! [ -e "$i" ]
	then
	mkdir ./resources/results
fi

echo '\033[1;34m'"================ TESTING ================"'\033[0;37m'

# MANDATORY TESTING
for ((i = 1; i < unt; i++))
do
	printf $i"|"

	touch differ
	touch differ_bonus

	#one step of test
	./resources/tester
	key=`cat ./resources/files/key`
	iv=`cat ./resources/files/iv`
	sh ./resources/bash_scripts/$scriptname $key $iv
	err=`wc -l < differ`
	#if there is a difference in mandatory part
	if ! [ "$err" -eq "0" ]
	then
		count=$((count + err))
		echo " ===================== " >> fails
		printf ">>> TEXT \"" >> fails
		cat ./resources/files/plaintext >> fails
		echo "\"" >> fails
		printf ">>> KEY = " >> fails
		cat ./resources/files/key >>fails
		echo "" >> fails
		printf ">>> IV  = " >> fails
		cat ./resources/files/iv >>fails
		echo "" >> fails
		echo ">>> FAILED:" >> fails
		cat differ >>fails
		echo "" >> fails
	fi
	err=`wc -l < differ_bonus`
	#if there is a difference in bonus part
	if ! [ "$err" -eq "0" ]
	then
		count_bonus=$((count_bonus + err))
		echo " ===================== " >> fails_bonus
		printf ">>> TEXT \"" >> fails_bonus
		cat ./resources/files/plaintext >> fails_bonus
		echo "\"" >> fails_bonus
		printf ">>> KEY = " >> fails_bonus
		cat ./resources/files/key >> fails_bonus
		echo "" >> fails_bonus
		printf ">>> IV  = " >> fails_bonus
		cat ./resources/files/iv >>fails_bonus
		echo "" >> fails_bonus
		echo ">>> FAILED:" >> fails_bonus
		cat differ_bonus >> fails_bonus
		echo "" >> fails_bonus
	fi

	rm differ
	rm differ_bonus
done

rm temp

# RESULT PRINT
if ! [ "$unt" -gt 1 ]
then
	exit
fi

echo '\033[0;36m'"\n=====| MANDATORY |====="'\033[0m'
if ! [ "$count" -eq "0" ]
then
	unt1=$((unt - 1))
	unt1=$((unt1 * 10))
	echo '\033[0;31m'$count" FAILS!"'\033[0m'" out of "$unt1" tests"
else
	echo '\033[0;32m'"          OK :)"'\033[0m'
fi

echo '\033[0;36m'"\n=====|   BONUS   |====="'\033[0m'
if ! [ "$count_bonus" -eq "0" ]
then
	unt1=$((unt - 1))
	unt1=$((unt1 * 8))
	echo '\033[0;31m'$count_bonus" FAILS!"'\033[0m'" out of "$unt1" tests"
else
	echo '\033[0;32m'"          OK :)"'\033[0m'
fi
