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

4.The *returnAddress* type is userd by the Java virtual machine's jsr, ret, and jsr_w instructions.The returnAddress type does not correspond to any Java programming language type and cannot be modified by the running  program<br>
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
#### 2.5.4 Method Area
1.The method area is analogous to the storage area for compiled code of a convetional language or analogous to the "text segment in an operating system process. It stores per-class structures such as the runtime constant pool,field and method data, and the code for methods and constructors, including the special methods used in class and instance initialzation and interface initialization.<br>
2.The method area may be of a fixed size or may be expanded as required by the computation and may be contracted if a larger method area becomes unnessary.
3.If memory in the method area cannot be made available to satisfy an allocation request, the Java virtual machine throws an **OutOfMemoryError**.
#### 2.5.5 Runtime Cnnstant Pool
1.A *runtime constant pool* is a per-class or per-interface runtime representation of the constant_pool table in a class file.
2.Each runtime constant pool is allocated from the Java virtual machine's method area.
3.When creating a class or interface, if the construction of the runtime constant pool requires more memory than can be made available in the method area of the Java virtual machine, the Java virtual machine throws an **OutOfMemoryError**
#### 2.5.6 Native Method Stacks
1.Java virtual machine implementations that cannot load native methods and that do not themselves rely on conventional stacks.If supplied, native method stacks are typically allocated per thread when each thread is created.<br>
2.If the computation in a thread requires a larget native method stack than is permitted, the Java virtual machine throws a StackOverflowError<br>
3.If native method stacks can be dynamically expanded and native method stack expansion is attempted but insufficient memory can be made a available, or if insufficient menmory can be made available to create the initial native method stack for new thread, the Java virtual machine throws an OutOfMemoryError.
### 2.6 Frames
1.A frame is used to store data and partial results, as well as to perform dynamic linking,return values for methods, and dispatch exceptions.<br>
2.A new frame is created each time a method is invoked.A frame is destoryed when its method invocation completes,whether that completion is normal or abrupt.<br>
### 2.7 Representation of Objects
In some of Oracle's implementations of the Java virtual machine, a reference to a class instance is a pointer to a handle that is itself a pair of pointers:one to a table containing the methods of the object and a pointer to the Class object that represents the type of the object, and the other to the memory allocated from the heap for the object data.
### 2.8 Float-Point Arithmetic
#### 2.8.1 Java Virtual Machine FLoating-Point Arithmetic and IEEE 754
### 2.10 Exceptions 
An exception in the Java virtual machine is represented by an instance of the class Throwable or one of its subclasses.<br>
The Java virtual machine throws an exception for one of three reasons:<br>
* An athrow instruction was executed<br>
* An abnormal execution condition was sychronously detected by the Java virtual machine.<br>
* An asychronous exception occurred becasue:<br>
1. The stop method of class Thread or ThreadGroup was invoked, or
2. An internal error occurred in the Java virtual machine implementation.

A Java virtual machine may permit a small but bouded amount of execution to occur before an asynchronous exception is thrown.<br><br>
Exceptions thrown by the Java virtual machine are precise:when the transfer of control takes place, all effects of the instructions exectued before the point from which the exception is thrown must appear to have taken place.<br><br>
Each method in the Java virtual machine may be associated with zero or more exception handlers.<br>

### 2.11 Instruction Set Summary
A Java virtual machine instruction consists of a one-byte opcode specifying the operation to be performed, followed by zero or more operands supplying arguments or that are used by the operation.Many instructions have no operands and consist only of an opcode.
#### 2.11.1 Types and the Java Virtual Machine
For the majority of typed instructions, the instruction type is represented explicitly in the opcode mnemonic by a letter: *i* for an **int** operation, and *l* for **long**, s for short, b for byte, c for char, f for float, d for double, and a for reference.<br>
Certain Java virtual machine instructions such as *pop* adn *swap* operate on the operand stack without regard to type; however, such instructions are constrained to use only on values of certain categoried of computational types, also given in the following table<br>
Actual type | Computational type | Category
:---- | :----- | :---:
boolean | int | category1
byte | int | cateogry1
char | int | cateogry1
short | int | cateogry1
int | int | cateogry1
float | float | cateogry1
reference | reference | category1
returnAddress | returnAddress | cateogry1
long | long | cateogry2
double | doubel |cateogry2


#### 2.11.2 Load and Store Instructions
The load and sotre instructions transfer values between the local variables and operand stack of a Java virtual machine frame.<br>
Instructions that access fields of objects and elements of arrays also transfer data to and from the operand stack.<br>
#### 2.11.3 Arithmetic Instructions 
The arithmetic instructions compute a result that is typically a function of two values on the operand stack,pushing the result back on the operand stack.The arithmetic instructions are as follows:
* Add: iadd, ladd, fadd, dadd
* Substract: isub, lsub, fsub, dsub
* Multiply:imul, lmul, fmul, dmul
* Divide:idiv, ldiv, fdiv, ddiv
* Remainder:irem, lrem, frem, drem.
* Negate:ineg, lneg, fneg, dneg.
* Shift:ishl, ishr, iushr, lshl, lshr, lushr
* Bitwise OR:ior, lor
* Bitwise AND:iand, land.
* Bitwise exclusive OR:ixor, lxor.
* Local variable increment:iinc.
* Comparision:dcmpg, dcmpl, fcmpg, fcmpl, lcmp
#### 2.11.4 Type Conversion Instructions
The type conversion instructions allow conversion betewwn Java virtaul machine numeric types.These may be used to implement explicit conversions in user code or to mitigate the lack of orthogonality in the instruction set of the 
Java virtual machine.<br>
The Java virtual machine directly supports the following widening numeric conversions:
* int to long, float, or double
* long to foat or double 
* float to double

The Java virtual machine also directly supports the following narrowing numeric conversions:
* int to byte, short, or char
* long to int
* float to int or long
* double to int, long, or float

In a narrowing numeric conversion of a floating-point value to an integral type T, where T is either int or long, the floating-point value is converted as follows:<br>
1. If T is long and this integer value can be represented as a long, then the result is the long value V.
2. Otherwise, if the floating-point value is not an infinity, the floating-point value is rounded to an integer value V using IEEE 754 round towards zero mode. There are two cases:
* If T is long and this integer value can be represented as a long, then the result is the long value V
* If T is of type int and this integer value can be represented as an int, then the result is the int value V.
3. Otherwise:
* Either the value must be too small, and the result is the smallest representable value of type int or long.
* Or the value must be too large, and the result is the largest representeable value of type int or long.

#### 2.11.5 Object Creation and Manipulation
Although both class instances and arrays are objects, the Java virtaul machine creates and manipulates class instances and arrays using distinct sets of instructions:
* Create a new class instance: *new*
* create a new array: *newarray*， *anewarray*, *multianewarray*
* Access fields of classes(static fields, known as class variables) and fields of class instances(non-static fields, known as instance variables): *getfield*, *putfield*, *getstatic*, *putstatic*
* Load an array component onto the operand stack:baload, caload, saload, iaload, laload, faload, daload, aaload
* Store a value from the operand stack as an array component:bastore, castore, sasotre, iasotre, lastore, fastore, dastore, aastore.
* Get the length of array :arraylength
* Check properties of class instances or arrays:instanceof, checkcast.
#### 2.11.6 Operand Stack Management Instructions
A number of instructions are provided for the direct manipulation of the operand stack:pop, pop2, dup, dup2, dup_x1, dup2_x1, dup_x2, dup2_x2, swap.
#### 2.11.7 Control Transfer Instructions
The control transfer instuctions conditionally or unconditionally cause the Java virtual machine to continue excution with an instruction other than the one following the control transfer instuction.They are:
* conditional branch: ifeq, ifne, iflt, ifle, ifge, ifnull, ifnonnull, if_icmpeq, if_icmpne, if_icmplt, if_icmple, if_icmpgt if_icmpge, if_acmpeq, if_qcmpne.
* Compound conditional branch: tableswitch, lookupswitch.
* Unconditional branch: goto, goto_w, jsr, jsr_w, ret.
#### 2.11.8 Method Invocation and Return Instructions
The following five instructions invoke methods:
* invokevirtual invokes an instance method of an object, dispatching on the (virtual) type of the object.This is the normal method dispatch in the Java programming language.
* invokeinterface invokes an interface method, searching the methods implemented by the particular runtime object to find the appropriate method.
* invokespecial invokes an instance method requiring special handling, whether an instance initialization method, a private method, or a superclass method.
* invokestatic invokes a class(static) method in a named class.
* invokedynamic invokes the method which is the target of the call site object bound to the invokedynamic instruction.The call site object was bound to a specific lexical occurrence of the invokedynamic instuction by the Java virtual machine as a result of running a bootstrap method before the first execution of the instruction.Therefore, each occurrence of an invokedynamic instruction has a unique linkage state, unlike the other instructions which invoke methods.
#### 2.11.9 Throwing Exceptions
An exceptoin is thrown programmatically using the athrow instruction.Exceptions can also be thrown by various Java virtual machine instructions if they detect an abnormal condition.
#### 2.11.10 Synchrnization
1.The Java virtual machine supports synchronization of both methods and sequences of instructions within a method by a single synchronization construct:the monitor.<br>
2.Method-level synchronization is performed implicitly, as part of method invocation and return.<br>
3.Synchronization of sequences of instructions is typically used to encode the synchronized block of the Java programming language.The Java virtual machine supplies the monitorenter and monitorexit instructions to support such language constructs.<br>
4.Structured locking is the situation when, during method invocation, every exit on a given monitor matches a preceding entry on that monitor.Since there is no assurance that all code submitted to the Java virtual machine will perform structured locking, implementations of the Java virtual machine are permitted but note required to enforce both of the following two rules guaranteeing structured locking.<br>
Let *T* be a thread and *M* be a monitor.Then:<br>
1. The number of monitor entries performed by *T* on *M* during a method invocation must equal the number of monitor exits performed by *T* on *M* during the method invocation whether the method invocation completes normally or abruptly.
2. At no point during a method invocation may the number of monitor exits performed by *T* on *M* since the method invocation exceed the number of monitor entries perfromed by *T* on *M* since the method invocation.
 ### 2.12 Class libraries
 The Java virtual machine must provide sufficient support for the implementation of the class libraries of the associated platform.Some of the classes in these libraries cannot be implemented without the cooperation of the Java virtual machine.<br>
 Classes that might require special support from the Java virtual machine include those that support:
 * Reflection, such as the classes in the package java.lang.reflect and the class Class.
 * Loading and creation of a class or interface.The most obvious example is the class ClassLoader.
 * Linking and initialization of a class or interface.The example classes cited above fall into this category as well
 * Security, such as the classes in the package java.security and other classes such SecurityManager
 * Multithreading, such as the class Thread.
 * Weak references, such as the classes i the pacakge java.lang.ref

 ### 2.13 Public Design, Private Implementation
 The class file format and the instruction set are vital to the hardware, operating system, and implementation-independence of the Java virtual machine.<br>


## Chapter 3 Compiling for the Java Virtual Machine 

## Chapter 4 The class File Format 

## Chapter 5 Loading, Linking, and Initializing

## Chapter 6 The Java Virtual Machine Instruction Set

## Chapter 7 Opcode Mnemonics by Opcode