#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$(cd .. && pwd)"
TMP_DIR="tmp_files"
RESULT_DIR="results"

pushd "$SRC_DIR"
	mkdir $TMP_DIR
	mkdir $RESULT_DIR
	for px in {0..29}
	do
		for py in {0..32}
		do
			x=`expr $px + 1`
			y=`expr $py + 1`
			map_pos="$(cat ./map/barto-big.csv | head -n $x | tail -n 1 | cut -d',' -f$y)"
			if [[ $map_pos == "x" ]]; then
				echo "x" > "$RESULT_DIR/main_${px}_${py}.log"
			elif [[ $map_pos == "g" ]]; then
				echo "g" > "$RESULT_DIR/main_${px}_${py}.log"
			else
				sed "s/Position start_position = {0, 14};/Position start_position = {$px, $py};/g" ./src/barto_big.c > "$TMP_DIR/main_${px}_${py}.c"
				if [[ $1 == "noise" ]]; then
					sed -i "s/Velocity start_velocity = {0, 0};/Velocity start_velocity = {`python3 scripts/gen_ini_velocity.py barto_big ${px} ${py}`};/g" "$TMP_DIR/main_${px}_${py}.c"
				fi

			fi

		done
	done

	#Now we parallely run all these files and record SAFE/WARNING output only
	pushd $TMP_DIR
		echo "Starting Parallel Jobs"
		parallel -j $(nproc) "clam.py {} --inline --crab-check=assert --crab-dom=boxes --crab-add-invariants=all --crab-track=arr --crab-do-not-print-invariants --crab-widening-delay=20 > $SRC_DIR/$RESULT_DIR/\$(basename {} .c).log " ::: *.c
	popd

	#Club all output together
	pushd $RESULT_DIR
		for px in {0..29}
		do
			for py in {0..32}
			do
				safe=$(cat main_${px}_${py}.log  | grep "safe" | cut -d' ' -f1)
				warn=$(cat main_${px}_${py}.log  | grep "warn" | cut -d' ' -f1)
				lines=$(wc -l main_${px}_${py}.log | cut -d' ' -f1)

				if [[ $lines == 1 ]]; then
					cat main_${px}_${py}.log >> results.txt
				elif [[ $safe == 2 && $warn == 0 ]]; then
					echo "1" >> results.txt
				elif [[ $safe == 1 && $warn == 1 ]]; then
					echo "0" >> results.txt
				elif [[ $safe == 0 && $warn == 2 ]]; then
					echo "2" >> results.txt
				else #Should never reach here
					echo "2" >> results.txt
				fi

			done
		done

		rm /deepsymbol/info_*.json tmp_files/summary.txt tmp_files/bound.log
		rm *.log
		rm -rf ${SRC_DIR}/${TMP_DIR}
		awk -f ${SCRIPT_DIR}/makeCSV.awk results.txt > results.csv
	popd 
popd
