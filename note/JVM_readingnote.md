# JVM Reading note
## Chapter 1 Introduction
### nothing to talk about

## Chapter 2 The Structure of the Java VIrtual Machine
### 2.2 Data types
1.The java virtual machine operates on two kinds of types:primitive types and reference types
### 2.3 Primitive Types and Values
1.The primitive data types supported by the Java virtual machine are the numeric types, the boolea type and the returnAddress type.<br>
2.The numeric types consist of the integral types and the floating-point types<br>
3.Float-point value set parameters:<br>
Parameter | float | float-extended-exponent | double | double-extended-exponent
:-: | :-------------: | :------------: | :---:    | :---:
N | 24  | 24 | 53 | 53 
K | 8  | >=11 | 11 | >=15
Emax | +127  | >=+1023 | +1023 | >=16383  
Emin | +126  | >=-1022 | -1022 | =<-16382 
4.The *returnAddress* type is userd by the Java virtual machine's jsr, ret, and jsr_w instructions.The returnAddress type does not correspond to any Java programming language type and cannot be modified by the running  program
5.Expressions in the Java rpogramming language that operate on boolean values are compiled to use values of the Java machine int data type.
### 2.4 Reference Types and Values
1.There  are three kinds of reference types:class types, array types and interface tyes.<br>
2.The component type of an array type may itself be an array type.<br>
3.A *reference* value may also be the special null reference, a reference to no object, which will be denoted here by null.<br>
4.The java virtual machine specification does not mandate a concrete value encoding null.
### 2.5 Runtime Data Areas
#### 2.5.1 The pc Register
1.The PC (program counter) contains the address of the Java virtual machine instruction currently being executed.<br>
2.If the method currently being executed bu the thread is native, the value of the Java virtual machine's pc register is undefined.
#### 2.5.2 Java Virtual Machine Stacks
1.The following exceptional conditions are associated with Java Virtual machine statcks:<br>
* if the computation in a thread requires a larger Java virtual machine statck than is permitted, the Java virtual machine throws a StackOverflowError.
* If Java virtual machine stacks can be dynamically expanded, and expansion is attempted but insufficient memory can be made available to effect the expansion, or if insufficient memory can be made available to create the initial Java virtual machine stack for a new thread, the Java virtual machine throws an OutOfMemoryError.
#### 2.5.3 Heap
1.The heap is created on virtual machine start-up.Heap storage for objcts is reclaimed by an automatic storage management system(known as a garbae collector)<br>
2.The heap may be of a fixed sieze or may be expanded as required by the computation and may be contracted of a larger heap becomes unnecessary.<br>
3.The following exceptional condition is associated with the heap:
* If a computation requires more heap than can be made available by the automatic storage management system, the Java virtual machine throws an OutOfMemoryError
#### 2.5.4 Mehtod Area
1.The method area is analogous to the storage area for compiled code of a convetional language or analogous to the "text segment in an operating system process. It stores per-class structures such as the runtime constant pool,field and method data, and the code for methods and constructors, including the special methods used in class and instance initialzation and interface initialization.<br>
2.The method area may be of a fixed size or may be expanded as required by the computation and may be contracted if a larger method area becomes unnessary.
3.If memory in the method area cannot be made available to satisfy an allocation request, the Java virtual machine throws an **OutOfMemoryError**.
#### 2.5.5 Runtime Cnnstant Pool
1.A *runtime constant pool* is a per-class or per-interface runtime representation of the constant_pool table in a class file.
2.Each runtime constant pool is allocated from the Java virtual machine's mehtod area.
3.When creating a class or interface, if the construction of the runtime constant pool requires more memory than can be made available in the method area of the Java virtual machine, the Java virtual machine throws an **OutOfMemoryError**
#### 2.5.6 Native Mehtod Stacks
1.Java virtual machine implementations that cannot load native methods and that do not themselves rely on conventional stacks.If supplied, native method stacks are typically allocated per thread when each thread is created.<br>
2.If the computation in a thread requires a larget native mehtod stack than is permitted, the Java virtual machine throws a StackOverflowError<br>
3.If native method stacks can be dynamically expanded and native method stack expansion is attempted but insufficient memory can be made a available, or if insufficient menmory can be made available to create the initial native method stack for new thread, the Java virtual machine throws an OutOfMemoryError.
### 2.6 Frames
1.A frame is used to store data and partial results, as well as to perform dynamic linking,return values for methods, and dispatch exceptions.<br>
2.A new frame is created each time a method is invoked.A frame is destoryed when its method invocation completes,whether that completion is normal or abrupt.<br>
3.
## Chapter 3 Compiling for the Java Virtual Machine 

## Chapter 4 The class File Format 

## Chapter 5 Loading, Linking, and Initializing

## Chapter 6 The Java Virtual Machine Instruction Set

## Chapter 7 Opcode Mnemonics by Opcode