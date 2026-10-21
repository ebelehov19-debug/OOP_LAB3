# Лабораторная работа №3 — вариант 4

Команды для Ubuntu выполняются из папки с `CMakeLists.txt`.

## Сборка и запуск

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/lab_03
./build/lab_03_tests
ctest --test-dir build --output-on-failure
```

## ASan и UBSan

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
cmake --build build-asan
ASAN_OPTIONS=detect_leaks=1 ./build-asan/lab_03_tests
ASAN_OPTIONS=detect_leaks=1 ctest --test-dir build-asan --output-on-failure
```
