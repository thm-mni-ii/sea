#!/bin/sh
#   Launch runtime tests or plot the results in a convenient way.

This=$(dirname $(readlink -e "$0"))
#From=1e3
#To=1e5

case "$1" in
    run)
        echo "Running tests..."
        nohup bin/main d time-ddfs-std-$From-$To.csv time-ddfs-eff-$From-$To.csv $From $To &
        nohup bin/main b time-bfs-std-$From-$To.csv time-bfs-eff-$From-$To.csv $From $To &
        nohup bin/main c time-cv-std-$From-$To.csv time-cv-eff-$From-$To.csv $From $To &
        nohup bin/main u time-udfs-std-$From-$To.csv time-udfs-eff-$From-$To.csv $From $To &
        nohup bin/main r time-rdfs-std-$From-$To.csv time-rdfs-eff-$From-$To.csv $From $To &
        nohup bin/main o time-opg-std-$From-$To.csv time-opg-eff-$From-$To.csv $From $To &

        nohup bin/main D mem-ddfs-std-$From-$To.csv mem-ddfs-eff-$From-$To.csv $From $To &
        nohup bin/main B mem-bfs-std-$From-$To.csv mem-bfs-eff-$From-$To.csv $From $To &
        nohup bin/main C mem-cv-std-$From-$To.csv mem-cv-eff-$From-$To.csv $From $To &
        nohup bin/main U mem-udfs-std-$From-$To.csv mem-udfs-eff-$From-$To.csv $From $To &
        nohup bin/main R mem-rdfs-std-$From-$To.csv mem-rdfs-eff-$From-$To.csv $From $To &
        nohup bin/main O mem-opg-std-$From-$To.csv mem-opg-eff-$From-$To.csv $From $To &
        ;;
    run-mn)
        echo "Running m/n tests..."
        nohup bin/main /d time-ddfs-std-$From-$To-mn.csv time-ddfs-eff-$From-$To-mn.csv $From $To &
        nohup bin/main /b time-bfs-std-$From-$To-mn.csv time-bfs-eff-$From-$To-mn.csv $From $To &
        nohup bin/main /c time-cv-std-$From-$To-mn.csv time-cv-eff-$From-$To-mn.csv $From $To &
        nohup bin/main /u time-udfs-std-$From-$To-mn.csv time-udfs-eff-$From-$To-mn.csv $From $To &
        nohup bin/main /r time-rdfs-std-$From-$To-mn.csv time-rdfs-eff-$From-$To-mn.csv $From $To &
        nohup bin/main /D mem-ddfs-std-$From-$To-mn.csv mem-ddfs-eff-$From-$To-mn.csv $From $To &
        nohup bin/main /B mem-bfs-std-$From-$To-mn.csv mem-bfs-eff-$From-$To-mn.csv $From $To &
        nohup bin/main /C mem-cv-std-$From-$To-mn.csv mem-cv-eff-$From-$To-mn.csv $From $To &
        nohup bin/main /U mem-udfs-std-$From-$To-mn.csv mem-udfs-eff-$From-$To-mn.csv $From $To &
        nohup bin/main /R mem-rdfs-std-$From-$To-mn.csv mem-rdfs-eff-$From-$To-mn.csv $From $To &
        ;;
    run-chords)
        echo "Running chords tests..."
        nohup bin/main %o time-opg-std-$From-$To-chords.csv time-opg-eff-$From-$To-chords.csv $From $To &
        nohup bin/main %c time-cv-std-$From-$To-chords.csv time-cv-eff-$From-$To-chords.csv $From $To &
        nohup bin/main %O mem-opg-std-$From-$To-chords.csv mem-opg-eff-$From-$To-chords.csv $From $To &
        nohup bin/main %C mem-cv-std-$From-$To-chords.csv mem-cv-eff-$From-$To-chords.csv $From $To &
        ;;
    run-special)
        echo "Running special tests..."
        nohup bin/main +c time-cd-std-$From-$To.csv time-cd-eff-$From-$To.csv $From $To &
        nohup bin/main +s time-racc-std-$From-$To.csv time-racc-eff-$From-$To.csv $From $To &
        nohup bin/main +S time-ssa-std-$From-$To.csv time-ssa-eff-$From-$To.csv $From $To &
        nohup bin/main +g time-vgra-std-$From-$To.csv time-vgra-eff-$From-$To.csv $From $To &
        nohup bin/main :G /dev/null /dev/null $From $To &
        ;;
        
    plot)
        echo "Making plots..."
        for type in time mem; do
            if test $type = mem; then Y='memory (KB)'; else Y='time (s)'; fi
            for alg in ddfs bfs cv udfs rdfs opg; do
                sh $This/plot.sh -r -l xy -y "$Y" -o ${type}_$alg-$From-$To.jpg $type-$alg-std-$From-$To.csv $type-$alg-eff-$From-$To.csv
            done
        done
        ;;
    plot-mn)
        echo "Making m/n plots..."
        for type in time mem; do
            if test $type = mem; then Y='memory (KB)'; else Y='time (s)'; fi
            for alg in ddfs bfs cv udfs rdfs; do
                sh $This/plot.sh -r -l xy -x "outgoing edges/vertex" -y "$Y" -o ${type}_$alg-$From-$To-mn.jpg $type-$alg-std-$From-$To-mn.csv $type-$alg-eff-$From-$To-mn.csv
            done
        done
        ;;
    plot-chords)
        echo "Making chord plots..."
        for type in time mem; do
            if test $type = mem; then Y='memory (KB)'; else Y='time (s)'; fi
            for alg in opg cv; do
                sh $This/plot.sh -r -l xy -x "chords" -y "$Y" -o ${type}_$alg-$From-$To-chords.jpg $type-$alg-std-$From-$To-chords.csv $type-$alg-eff-$From-$To-chords.csv
            done
        done
        ;;
    plot-special)
        echo "Making special plots..."
        sh $This/plot.sh -r -x "read/write operations" -y "time (s)" -o time_cd-$From-$To.jpg time-cd-std-$From-$To.csv time-cd-eff-$From-$To.csv
        sh $This/plot.sh -r -x "read/write operations" -y "time (s)" -o time_racc-$From-$To.jpg time-racc-std-$From-$To.csv time-racc-eff-$From-$To.csv
        sh $This/plot.sh -r -y "memory (KB)" -o mem_ssa-$From-$To.jpg mem-ssa-std-$From-$To.csv mem-ssa-eff-$From-$To.csv
        sh $This/plot.sh -r -x "virtual graph operations" -y "time (s)" -o time_vgra-$From-$To.jpg time-vgra-std-$From-$To.csv time-vgra-eff-$From-$To.csv
        sh $This/plot.sh -y "memory (KB)" -o mem_vgra-$From-$To-split-triang.jpg mem-vgra-*triang.csv
        ;;
    
    *)
        echo "Usage: launch.sh <run|plot>"
        echo "  run:    Run tests for all algorithms in parallel (VERY HIGH performance required!)"
        echo "  plot:   Create plots for all tests (logarithmic and ratio plots)"
        ;;
esac