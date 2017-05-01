# MapReduce
 Модель для обработки большого объема данных, которые не помещаются в оперативную память.
## Dependencies
1. Boost C++ library
2. Boost.Process library
3. Wikipedia 1.4.0 Python (https://pypi.python.org/pypi/wikipedia/)
## Compile and run
  Для компиляции флаг -lboost_system и -std=c++11 (11 стандарт и выше).
## Included scrpits
  1. Mapreduce script
  2. Map script
  3. Reduce script
  4. Wikipedia script
  5. Inverse_index script
## Script description 
  1. **Mapreduce** **script**  
     **Run**: ./mapreduce mode "path_to_script" src_file dst_file  
    	>> *mode* - "map"/"reduce". Режим, в котором будет выполняться указанный в аргументах скрипт.  
    	>> *src_file*/*dst_file* - Формат входного и выходного файла: tsv (tab-separated-values):  
              key \t value \n  
              key \t value \n   
       + **map** **mode**  
          Вызывает map-скрипт для каждой строки входного файла.  
       + **reduce** **mode**  
          Сортирует входные данные по ключам с помощью внешней сортировки MergeSort и вызывает reduce-скрипт для для всех значений соответсвующих одному ключу.  
  2. **Map script**  
     **Run**: ./map_script  
     Работает со стандартными потоками. Формат входа и выхода:  
                key \t value \n  
                key \t value \n   
     
    
