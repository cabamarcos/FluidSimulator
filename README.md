# Proyecto_arqui


## Instalaciones previas necesarias para el funcionamiento del programa

<details>
<summary>En caso de usar WSL</summary>
    Editar el archivo <b>/etc/wsl.conf</b>

    [boot]
    systemd=true
</details>

### Instalar dependencias para la compilación

```bash
sudo apt install build-essential
sudo snap install cmake --classic
```

### Respecto a la instalación de clang y llvm

Es necesario primero importar la clave GPG de LLVM, para ello, llvm tiene un ejecutable que lo hace automáticamente.

```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 18
sudo apt install -y clang-tidy
```

## Compilación
    
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

```bash
g++ read_particles.cpp -o read_particles -std=c++20
```

## Ejecución

### Para ejecutar los tests

```bash
./build/utest/utest
```

### Para ejecutar el programa

```bash
./build/fluidapp
```

### Para ejecutar el programa de lectura de particulas

```bash
./read_particles <archivo>
```