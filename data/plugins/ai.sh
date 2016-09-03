#!/bin/bash
sed -n '/^1/p' ai.db > /tmp/humor.txt
HUMOR_RANGE=`cat /tmp/humor.txt | wc -l`
sed -n '/^2/p' ai.db > /tmp/resp-simples.txt
RESP_RANGE=`cat /tmp/resp-simples.txt | wc -l`

NUM=$RANDOM
let "NUM%=HUMOR_RANGE"

LOOP=0;HUMOR=0;
while read LINE; do
	if [ $NUM -eq $LOOP ] ; then
		MENS=`echo $LINE | cut -d';' -f2`

		if [ -f /tmp/today.txt ] ; then
			TODAY=`cat /tmp/today.txt | cut -d';' -f1`
			DATENOW=`date +%D`

			if [ "$DATENOW" != "$TODAY" ] ; then
				echo "`date +%D`;$MENS;" > /tmp/today.txt
			fi
		else
			echo "`date +%D`;$MENS;" > /tmp/today.txt
		fi
	fi

	LOOP=$(($LOOP+1))
done < /tmp/humor.txt
HUMOR=`cat /tmp/today.txt | cut -d';' -f2`

RBUF=$*
if [ "$RBUF" ] ; then
		sed -n "/$HUMOR;$/p" /tmp/resp-simples.txt > /tmp/saida.txt

		SAIDA_TOTAL=`cat /tmp/saida.txt | wc -l`
		NUM=$RANDOM
		let "NUM%=SAIDA_TOTAL"
		NUM=$(($NUM+1))

		SAIDA=`sed -n ${NUM}p /tmp/saida.txt | cut -d';' -f3`
		echo "$SAIDA"
fi
