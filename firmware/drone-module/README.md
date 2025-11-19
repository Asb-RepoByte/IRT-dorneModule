## Thoughts 

So I finally decided on a workflow to follow, I'm really proud of the following structure, code and hardware that I've done

## Old Approach
The *firmware* was separated into two directories one for the *drone-module* and the other for *ground-module*, but after some coding and experimentation I found out that it's not a good approach, for multiple reasons:
1. both the *drone-module* and *ground-module* share about 40% of the code (a lot of duplication)
2. the code they are is tightly related, so with just copy pasting it's proan to errors
3. also changing some code or behavior require two build process that take a lot of time and make itteration slower
4. the git repo will have a lot of duplication and make it heavy on platformIO

## New Approach
All the *firmware* is going to be inside of the firmware/drone-module directory, all the tasks will be encapsulated by function (i.g esp32 process), when the *esp32* mdoule start it'll check what role it has, and depending on that the apropriate functions will be called. This approach has bunck of benefits:
1. each task is encapsulated in a function.
2. modular each member of the team can work on a specific function.
3. one build process, after building and flashing once, neither are required again.
4. faster iteration, due to lack of overhead.

## How to contribute
>[!NOTE]
> simple != easy
Now that we have a modular approach we can use, contributing to the modules is really **Simple**
1. one must create a folder inside drone-module/lib/ for the task that he/she want's to handle, if it doesn't already exist
2. at least two files must be created inside the folder one `.cpp` and a corresponding `.h` file.
3. the function that handle the task must follow this prototype `void somethingTask(void *parameter)`.
4. the function must be declare inside of the `.h` file and implemented in `.cpp`.
5. Last step is to include the `.h` file in the *drone-module/main.cpp* and calling the `xTaskCreate` function

Example:
```cpp
if (role) { // meaning the module is a ground module
    // the third parameter is how much RAM this task take in term of word
    // so if the sytem is 32bit(4byte) system, the following function will consume 4 * 4069 = 16272 byte)  ~16kB  of RAM
    // to put that into perspective ESP32 has a 500KB of RAM
    // so this parameter must be assinged with care, the following is just a generic value that is not too big not too low
    xTaskCreate(functionTask, "task-name", 4069, NULL, 1, NULL);

}
```

>[!WARNING]
> this is a generic example, that might not be suitable for all cases.