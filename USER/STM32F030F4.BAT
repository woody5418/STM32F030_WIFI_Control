SET PATH=D:\keil_v5\ARM\ARMCC\Bin;C:\ProgramData\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;D:\Program Files\Java\jdk\bin;C:\Windows\system32\config\systemprofile\.dnx\bin;C:\Program Files\Microsoft DNX\Dnvm\;C:\Program Files\Microsoft SQL Server\130\Tools\Binn\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;F:\Program Files (x86)\ssh
SET CPU_TYPE=STM32F030F4
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=STM32F030F4
SET CPU_CLOCK=0x00B71B00
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\main.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_it.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\system_stm32f0xx.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\websocket.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\esp8266.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\espjson.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\airkiss.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\usart1.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\timer3.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\led.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\timer2.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\key.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\adc1.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\timer14.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\delay.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "..\obj\startup_stm32f030x8._ia"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_adc.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_crc.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_exti.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_flash.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_gpio.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_misc.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_rcc.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_rtc.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_syscfg.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_tim.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f0xx_usart.__i"
"D:\keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "..\OBJ\STM32F0308_Demo.lnp"
"D:\keil_v5\ARM\ARMCC\Bin\fromelf.exe" "..\OBJ\STM32F0308_Demo.axf" --i32combined --output "..\OBJ\STM32F0308_Demo.hex"
