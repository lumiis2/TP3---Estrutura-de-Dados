set term postscript eps
set output "out.eps"
set size square
set key bottom right
set title "BiUaiDi Recharging Stations"
set xlabel ""
set ylabel ""
unset xtics 
unset ytics 
plot "origin.gpdat" t "Your location" pt 4 ps 2, "recharge.gpdat" t "", "suggested.gpdat" t "Nearest stations" pt 7 ps 2
