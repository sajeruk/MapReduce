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
    	*mode* - "map"/"reduce". Режим, в котором будет выполняться указанный в аргументах скрипт.  
    	*src_file*/*dst_file* - Формат входного и выходного файла: tsv (tab-separated-values):  
              key \t value \n  
              key \t value \n   
       + **map** **mode**  
          Вызывает map-скрипт для каждой строки входного файла.  
       + **reduce** **mode**  
          Сортирует входные данные по ключам с помощью внешней сортировки MergeSort и вызывает reduce-скрипт для для всех значений соответсвующих одному ключу.  
  1. **Map script**  
     **Run**: ./map_script  
     Работает со стандартными потоками. Формат входа и выхода:  
     			key \t value \n  
	 Разбивает значение на отдельные слова относительно пробела в качестве сепаратора. Выдает на выход слова длины больше, чем 2, со значением, равным входному ключу.  
  1. **Reduce** **script**  
     **Run**: ./map_script  
	  Работает со стандартными потоками. Формат входа и выхода:  
     			key \t value \n  
			key \t value \n  
	  Выводит ключ со всеми значениями, разделенными '#'. 
  1. **Wikipedia** **script**  
    **Run**: ./wikipedia_script  
	  Работает со стандартными потоками. Формат входа и выхода:  
     			key \t value \n  
			key \t value \n  
	Принимает на вход таблицу с пустым значением и url статьи англоязычной википедии в ключе. По названию статьи, извлеченному из конца url, получает основную информацию со страницы и выводит ее в формате "url \t text \n".  
  1. **Inverse_index** **script**  
    **Run**: ./inverse_index_script "path_to_mapreduce_script" "path_to_wikipedia_script" "path_to_map_script" "path_to_reduce_script" src_file dst_file  
    	*src_file*/*dst_file* - Формат входного и выходного файла: tsv (tab-separated-values):  
              key \t value \n  
              key \t value \n   
  	Входной файл должен содержать таблицу с пустым значением и url статьи англоязычной википедии в ключе. В выходном файле находятся все слова, встречающиеся в статьях, со списком всех статей, в которых слово содержится, разделенных '#'.
