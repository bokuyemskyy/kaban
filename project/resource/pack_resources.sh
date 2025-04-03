echo "" > ../include/resources.hpp
for file in *; do
    echo "$file"
    xxd -i "$file" >> ../include/resources.hpp
done
