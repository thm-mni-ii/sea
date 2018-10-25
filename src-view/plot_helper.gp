# (Used by plot.sh)

# Plot a RuntimeTest CSV file with gnuplot:
#	gnuplot -e "infiles='mytest1.csv mytest2.csv'" -e "outfile='myoutput.jpg'" -e "title='My tests'" -c plot_helper.gp

if (!exists("title")) title="Runtime test"
set title title
set grid xtics ytics
set terminal jpeg size 800,800
if (!exists("outfile")) {
	outfile="runtime-plot.jpg"
	print "Generating output to 'runtime-plot.jpg'."
}
set output outfile
if (!exists("ratio")) {
	if (!exists("infiles")) {
		print "No infiles given."
	}
	set xlabel "order"
	set ylabel "time (".scale.")"
	set style data lines
	plot for [basename in infiles] basename.".csv" using 1:3 "%lf,%lf,%lf" skip 1 title basename
} else {
	set xlabel "order"
	set ylabel "factor"
	plot "< paste ".infile1.".csv ".infile2.".csv" using 1:($3/$6) "%lf,%lf,%lf %lf,%lf,%lf" skip 1
}
