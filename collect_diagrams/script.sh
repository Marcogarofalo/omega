
g++ format.cpp -o format
./format
sed -i 's/{u,u},//g' all_diagrams_formatted.txt
sed -i 's/Gamma5//g' all_diagrams_formatted.txt
sed -i 's/\.//g' all_diagrams_formatted.txt
sed -i 's/trace/ trace/g' all_diagrams_formatted.txt

# for i in $(seq 1 6); do
# 	for j in $(seq 1 6); do
# 		sed -i "s/{i${i}, i${j}}/(${i}${j})/g" all_diagrams_formatted.txt
# 	done
# done
