Sure, here's the markdown format for you to copy:

```markdown
# 🧱 Static Linking

Static linking involves linking libraries directly into the executable, resulting in a standalone binary file.

## 🛠️ Steps

1. **Compile your library source files to get object files** 🛠️
   
   ```bash
   gcc -c <source_files>
   ```
   
   This will compile the source file into object files.

2. **Archive the object files** 📦
   
   ```bash
   ar rcs liblibrary.a library.o
   ```

   - **r**: Insert the files member into archive (with replacement). 
   - **c**: Create the archive.
   - **s**: Write an object-file index into the archive, or update an existing one.

3. **Compile your project files with this library static way** 🏗️

   ```bash
   gcc -c main.c -I ./Include 
   ```
   
   Get the main object file, specifying the include folder.

   ```bash
   gcc main.o Lib/lib[name].a -o main_static.elf
   ```

   Note: You can make static linking for all libraries by using `-static` when compiling.

# 🔗 Dynamic Linking

Dynamic linking involves linking libraries at runtime, allowing shared libraries to be used by multiple programs.

## 🛠️ Steps

1. **Compile the library source files using position independent code flag** 🚀

   ```bash
   gcc -c -fpic *.c
   ```
   
   The `-fpic` flag indicates position independent code, allowing the system loader to place it at any position in the RAM.

2. **Produce the dynamic library** 📚
   
   ```bash
   gcc -shared -o lib[name].so *.o
   ```

   This will produce the dynamic library. Notice that dynamic lib has `.so` extension.

3. **Link with your application** 🧩

   ```bash
   gcc main.c -I./Include -L./Lib -l[name] 
   ```

   - `-L./lib` is the location of the dynamic library.
   - `-l[name]` is the name of the library (it will expand to `libname.so`).

# 🔄 Differences 

```bash
ldd main_static
ldd main_dynamic
ldd main_all_static
```

- `ldd` command will specify which libraries are being linked dynamically.

- Also, the size of statically linked program is larger.
