## Code Style

#### Comments
Comments are to be standard C89 comments **/******/**

###### Non-Compliant Comments
```c
// This is a comment <- Not using /* and */
```

###### Compliant Comments
```c
/* This is a comment */
```

#### Variable Naming
When defining a variable use **camelCase**. When defining a pointer add the **p** prefix.

###### Non-Compilant Variable Names
```c
FInt16 SomeValue = 0; /* This is not camelCase */
```
```c
FInt32 pSomeValue = 0; /* This is incorrect because the type is not a pointer */
```
###### Compliant Variable Names
```c
FInt16 someValue = 0;
```
```c
FInt32* pSomeValue = NULL;
```
```c
FSomeStructure someStructure = {0};
```
#### Type Naming
When defining a type name it should always be in PascalCase, this includes **typedefs** also. A type name should also include an **F** prefix before the name.

###### Non-Compliant Type Names
```c
typedef FSomeStructure Fsomestructure; /* This is not PascalCase */
``` 
```c
typedef struct Number /* There should be an 'F' prefix here */
{
    FInt64 value;
} Number; /* There should also be an 'F' prefix here */
```

###### Compliant Type Names
```c
typedef struct FSomeStructure
{
    FInt64 value;
} FSomeStructure;
```
```c
typedef FInt64 FVector3[4];
```

#### Types
When defining a variable use predefined types instead of the normal C99 types. Include **FTypes.h** when required. Always initialize a variable when defining one.

###### Non-Compliant Types
```c
int someValue; /* The int type is predefined as FInt8, FInt16, FInt32, FInt64. */
```
```c
unsigned long someValue; /* The unsigned int type is predefined as FUInt8, FUInt16, FUInt32, FUInt64 */
```
###### Compliant Types
```c
FInt64 someValue = 0;
```
```c
float someValue = 0;
```
```c
char currentChar = 'C'; 
```

Types that predefinded or dont require the 'F' prefix are, but not limited to, **float**, **char** and **bool**.

#### Function Naming
Function names should always be explanetory for what the function does. The should always be spelled out without abreviations. Function names must also use **PascalCase**. 
The function name format is [TypeName][FunctionName].


###### Non-Compliant Function Names
```c
void doSomething(FInt64 value);
```
```c
void do_something(FInt64 value);
```

###### Compliant Function Names
```c
/*              [    Type    ][Name] */
FSomeStructure* FSomeStructureCreate(FInt64 option);
```
```c
void FSomeStructureDestroy(FSomeStructure** ppSomeStructure);
```


#### Functions
A function should have one task and should do it well. Most functions should relate t' or control a structure/type. Functions should be a part in the whole, if a function looks like it has no reason to exist you probably shouldn't add it. 

#### Functions and Pointers
It is suggested to use a pointer as an input to a function rather than an output whenever possible. If a function create a pointer another function must also be defined that destroys that pointer. A destroy function must the have a *pointer-to-pointer* so that the variable can be nullified. **All deallocations must also nullify the variable.**

###### For Example
```c
FMyStructure* FMyStructureCreate() /* This is the create function for the structure */
{
    /* Do stuff */

    return pMyStructure;
}

void FMyStructureDestroy(FMyStructure** ppMyStructure) /* This is the destroy function */
{
    /* Do stuff */
    FDeallocate(*ppMyStructure);
    *ppMyStructure = NULL; /* This is required to be compliant */
}
```

#### Brackets
Brackets must be included on the **next** line. They are always included after every *if* statement.

###### Non-Compliant Brackets
```c
if (condition) { /* The first bracket must be on the next line */
    DoSomething();
}
```
```c
if (condition) /* There must be brackets on an if statement */
    DoSomething();
```

###### Compliant Brackets
```c
if (condition)
{
    DoSomething();
}
```

#### Explicitness
You should be explicit in your intentions when using *if* statements or any other flow changes. You should be asking a question.

###### Non-Compliant Explicitness
```c
if (pValue) /* This doesn't ask a question */
{
    DoSomething();
}
```

###### Compliant Explicitness
```c
if (pValue == NULL) /* This asks if pValue is NULL */
{
    DoSomething();
}
```

### Style Example
```c
// FExample.h
typedef struct FExample
{
    FInt64 someValue;
} FExample;

FExample* FExampleCreate();
void FExampleDestroy(FExample** ppExample);
```
```c
// FExample.c
#include "FMemory.h"

FExample* FExampleCreate()
{
    FExample* pExample = FAllocateZero(1, sizeof(FExample));
    if (pExample == NULL)
    {
        return NULL; 
    }
    
    /* Set the default values */
    pExample->someValue = 32;
    
    return pExample;
}

void FExampleDestroy(FExample** ppExample)
{
    FExample* pExample = *ppExample;    
    if (pExample == NULL)
    {
        return;
    }    

    FDeallocate(*ppExample);
    *ppExample = NULL;
}
```