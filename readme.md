### Instructions 
这是一个使用STM32CubeMX创建STM32F103C8T6 FreeRTOS工程的示例,本工程使用 `gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-`编译器进行编译,而且并没有使用STM32CubeMX封装过的FreeRTOS代码,而是使用FreeRTOS源码,配合`ARM_SEGGER_RTT`进行调试.
### PORT
* 首先,使用STM32CubeMX创建工程,根据具体需求配置相应的外设即可,需要注意的是在STM32CubeMX的`Pinout&Configuration`页面下的`System Core`菜单中的`SYS`配置项中,`Timebase Source`选项需要选择TIM1-TIM4中的任意一个,**万万不可选择SysTick,否则程序将不能正常运行*(因为SysTick将作为FreeRTOS的心跳)**.
* `Clock Configuration`页面中的时钟树根据需求需要配置即可.
* `Project Manager`页面配置:
    * `Projet`菜单的`Toolschain/IDE`选择`Makefile`
    * `Code Generator`菜单中的`STM32Cube MCU packages and embedded software packs`选择`Copy all used libraries into the project folder`
    * `Code Generator`菜单中的`STM32Cube MCU packages and embedded software packs`选择`Copy all used libraries into the project folder`
    * `Code Generator`菜单中的`Generated files`勾选`Generate peripheral initialization as a pair of '.c/.h' files per peripheral`
    * 以上直接翻译字面意思即可,没什么好说的
* 完成后点击`DENERATE CODE`即可生成文件
* 使用vscode打开生成的文件夹:
    * 初始化git仓库:
    ```C
    git init
    ```
    * 添加ARM_SEGGER_RTT子模块:
    ```C
    git submodule add git@github.com:Lockie-github/ARM_SEGGER_RTT.git 
    ```
    * 添加FreeRTOS子模块:
    ```C
    git submodule add git@github.com:Lockie-github/RTOS.git FreeRTOS  
    ```
    * 若不能成功添加也可以直接在网页下载两个仓库的压缩包后添加到文件夹中,需要注意的是`RTOS`需要重命名为`FreeRTOS`
    * 打开`ARM_SEGGER_RTT/readme.md`并操作
    * 打开`FreeRTOS/FreeRTOS.md`并操作 
    * 打开`Core/Inc/stm32f1xx_it.h`将`void SVC_Handler(void);``void PendSV_Handler(void);``void SysTick_Handler(void);`三个函数注释/删除掉.
    * 打开`Core/Src/stm32f1xx_it.c`将`void SVC_Handler(void)``void PendSV_Handler(void)``void SysTick_Handler(void)`三个函数注释/删除掉.
        * 因为FreeRTOS已经实现了这三个函数,保留会导致重定义报错.
    * 打开`Core/Src/stm32f1xx_hal_msp.c`将`__HAL_AFIO_REMAP_SWJ_DISABLE();`函数注释/删除掉.
        * 开启此函数会导致无法正常调试.
    * 打开`Makefile`文件
        * 在`TARGET = xxxx`后添加代码:
        ```Makefile
        TARGET = succssful
        ALLCSRC = 
        ALLINC = 
        ```
        * 在`BUILD_DIR = build`后添加代码
        ```Makefile
        BUILD_DIR = build
        include ARM_SEGGER_RTT/segger_rtt.mk
        include FreeRTOS/Source/freertos.mk
        include FreeRTOS/TASK/task.mk
        include self_task/self_task.mk
        ```
         * 在`startup_stm32f103xb.s`后添加代码
        ```Makefile
        ASM_SOURCES =  \
        startup_stm32f103xb.s
        MCU_ID = STM32F103C8
        ```    
        * 将`PREFIX = arm-none-eabi-`修改为本地编译器的地址,例如
        ```Makefile
        PREFIX = /opt/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-
        ```
        * 将`# C includes`与`# compile gcc flags`之间代码修改为
        ```Makefile
        # C includes
        C_INCLUDES =  \
        Core/Inc \
        Drivers/STM32F1xx_HAL_Driver/Inc \
        Drivers/STM32F1xx_HAL_Driver/Inc/Legacy \
        Drivers/CMSIS/Device/ST/STM32F1xx/Include \
        Drivers/CMSIS/Include

        C_SOURCES += $(ALLCSRC)
        C_INCLUDES += $(ALLINC)
        C_INCLUDES := $(patsubst %,-I%,$(C_INCLUDES))
        
        # compile gcc flags
        ```
    * 所有在vscode中修改的代码在本示例中都使用`[操作]`做了标记,直接搜索查看即可
    * 点击`Ctrl + .`快捷键唤起中断,输入`make -j`即可编译,输入`make flash`即可烧录,其余make命令在`Makefile`最后,根据需求使用即可.
