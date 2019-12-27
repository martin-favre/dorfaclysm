cd out
rm *.out.*
valgrind --tool=callgrind ./Vidya
# kcachegrind outfile