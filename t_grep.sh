#!/bin/bash

# Тестовые файлы
echo -e "Hello World\nThis is a test file.\nThis line contains the word test.\n\nThis is a new line.\nAnother test line." > file1.txt
echo -e "Another test file.\nWith some content.\nThis line contains the word test.\n\nAnd some empty lines.\nTest again." > file2.txt
echo -e "Test file for grep.\nThis line should match.\nThis line should not match.\nAnother line with test." > file3.txt

# Функция для выполнения теста
run_test() {
    local flags="$1"
    local pattern="$2"
    local input_file="$3"
    local grep_output="grep_output_${flags// /_}.txt"
    local custom_grep_output="custom_grep_output_${flags// /_}.txt"

    grep $flags "$pattern" "$input_file" > "$grep_output"
    ./s21_grep $flags "$pattern" "$input_file" > "$custom_grep_output"
    
    diff "$grep_output" "$custom_grep_output"
    if [ $? -eq 0 ]; then
        echo "Test with flags '$flags' and pattern '$pattern' passed."
    else
        echo "Test with flags '$flags' and pattern '$pattern' failed."
    fi

    # Удаление временных файлов
    rm "$grep_output" "$custom_grep_output"
}

# Запуск тестов с одиночными флагами
run_test "-e" "test" "file1.txt"
run_test "-i" "TEST" "file1.txt"
run_test "-v" "test" "file1.txt"
run_test "-c" "test" "file2.txt"
run_test "-l" "test" "file2.txt"
run_test "-n" "test" "file3.txt"
run_test "-h" "test" "file3.txt"
run_test "-s" "nonexistent" "file1.txt"

# Удаление тестовых файлов
rm file1.txt file2.txt file3.txt