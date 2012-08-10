# Filter the file 1
# filter the file 2

INPUT=$1
OUTPUT=$2

TMP_FILE_INF=/tmp/plot.inf.$$.$RANDOM
TMP_FILE_AVG=/tmp/plot.avg.$$.$RANDOM
TMP_FILE_SUP=/tmp/plot.sup.$$.$RANDOM
TMP_FILE_PLOT=/tmp/plot.plot.$$.$RANDOM


if [ -f "$INPUT" ]; then
	echo $TMP_FILE_INF
	echo $TMP_FILE_SUP
	echo $TMP_FILE_AVG
	echo $TMP_FILE_PLOT

	cat $INPUT | grep inf | sed 's/inf //' > $TMP_FILE_INF
	cat $INPUT | grep avg | sed 's/avg //' > $TMP_FILE_AVG
	cat $INPUT | grep sup | sed 's/sup //' > $TMP_FILE_SUP

	echo "plot \"$TMP_FILE_INF\" title \"Borne Inferieure\" w lines, \\" > $TMP_FILE_PLOT
	echo "     \"$TMP_FILE_AVG\" title \"Distance Moyenne\" w steps, \\" >> $TMP_FILE_PLOT
	echo "     \"$TMP_FILE_SUP\" title \"Borne Superieure\" w linespoints" >> $TMP_FILE_PLOT

	if [ "$OUTPUT" != "" ]; then
		echo "set out \"$OUTPUT\"" >> $TMP_FILE_PLOT
		echo "set terminal postscript \\" >> $TMP_FILE_PLOT
		echo "     landscape enhanced color lw 2 \"Helvetica\" 14" >> $TMP_FILE_PLOT
		echo "replot" >> $TMP_FILE_PLOT
	fi

	
	gnuplot -persist $TMP_FILE_PLOT

	rm -f $TMP_FILE_INF
	rm -f $TMP_FILE_AVG
	rm -f $TMP_FILE_SUP
	rm -f $TMP_FILE_PLOT

else 
	echo "Usage : defiplot defi.plot [output.ps]"
fi

