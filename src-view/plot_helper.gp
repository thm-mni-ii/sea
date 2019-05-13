# (Used by plot.sh)

# Plot a RuntimeTest CSV file with gnuplot:
#	gnuplot -e "infiles='mytest1.csv mytest2.csv'" -e "outfile='myoutput.jpg'" -e "title='My tests'" -e "xlabel='order'" -c plot_helper.gp

set title title
set grid xtics ytics
set autoscale xfix
set terminal jpeg size 800,800
set datafile separator ","
stats "< paste -d, ".infiles using ($3/$6) skip 1 nooutput name "ratio"
if(exists("logx")) set logscale x
if(exists("logy")) set logscale y
if (!exists("outfile")) {
	outfile="runtime-plot.jpg"
	print "Generating output to 'runtime-plot.jpg'."
}
set output outfile
if (!exists("infiles")) {
    print "No infiles given."
}
set xlabel xlabel
set ylabel ylabel
set style data lines
if(exists("ratio")) {
    set label sprintf("ratio std/eff: %4.2f",ratio_mean) at screen 0.85,0.9 center boxed font "Helvetica,14"
}
if(ylabel eq "time (s)") {
    set label sprintf("total time : %4.2f min",0+system("awk -F, '{ S+=$3+$6 } END { print S }' ".infiles)) at screen 0.23,0.95 center font "Helvetica,14"
}
plot for [f in infiles] f using 1:3 skip 1 title f smooth unique
