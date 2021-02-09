# Vivado-SDK-Basic-IP-Integrator
# Summary
The project analyses different functions of Vivado’s SDK IP Integrator. For this basic IP integrator was explored. A block design with two GPIO interfaces was created in the Vivado software. An SDK FreeRTOS template was imported and was modified to input switches and output LEDs. Programmed the system to let the LEDs behave certain way according to inputs from switches by switching between tasks. The motivation for this analysis was to understand concept of task management in Embedded Systems. The results were as expected. Successfully synthesized the block design and the system behaved the way desired.
# Introduction
The object of the project was to control the behavior of LEDs on the zybo board using the switches using FeeRTOS tasks.
Task 1: Create a FreeRTOS task TaskLED that has the LED count and display start at 0 (0000) and increase as a somewhat random sequence as listed then repeats the sequence starting at 0 (with a delay of approximately 1 second and at the Idle task priority +1 (tskIDLE_PRIORITY+1). The sequence is as follows:
0000, 1001, 0110, 1010, 0001, 0101, 1000, 0100, 1100, 1110, 0010, 1011, 1101, 0011, 0111, 1111
Task 2: Create a FreeRTOS task TaskSW that reads the SWs at the Idle task priority+ 1. Within the FreeRTOS task TaskSW if SW0 is ON and no other SWs are ON then TaskLED is suspended (vTaskSuspend()). If SW1 is ON and no other SWs are ON then TaskLED is resumed (vTaskResume()).
# Discussion
Created a block design consisting of ZYNQ7 processing system, processor system reset, AXI Interconnect and two AXI GPIOs. The diagram of the design is shown below:
 
Figure 1. Diagram of the Hardware design
It can be observed that two GPIO blocks are created to control switches and LEDs. HDL wrapper file was created and generated bitstream. The design was then exported to SDK. Imported the template ‘Hello world’ file and modified it to perform the specified tasks. 
Two tasks were created using the xTaskCreate function. They were TaskLED and TaskSW. Both tasks were given the same priority, tskIDLE_PRIORITY+1. The code snippet below shows how this was accomplished
 
Figure 2. Code to create tasks
To accomplish task 1, a TaskLED function was defined. Within this function, an infinite loop was created. Added different LED sequences as previously defined with a 1 second delay between them. The delay was created first by converting seconds to equivalent ticks using the pdMS_TO_TICKS function and using it as a parameter inside vTaskDelay function. A code snippet of the task function TaskLED is provided below:
 
Figure 3. Task TaskLED function
The images below shows how the LEDs behave when no switches are ON.
 
Figure 4. When no switches are ON

 
Figure 5. When no switches are ON
 
Figure 6. When no switches are ON

 
Figure 7. When no switches are ON
For Task 2, TaskSW function defined. Inside the function an infinite loop was created with switch-case statements that checks status of the switches. A code snippet of this task function is given below:
 
Figure 12. Task TaskSW function
The images below shows the behavior of LEDs when SW0 and SW1 are ON/OFF
 
Figure 8. When SW0 in ON
It can be seen that the TaskLED is suspended by turning ON all the LEDs are staying in that state. Images below shows how LEDs resume with the sequence when SW1 is turned ON. 
 
Figure 9. When SW1 is ON
 
Figure 10. When SW1 is ON
 
Figure 11. When SW1 is ON

# Conclusions
Successfully generated a hardware design using Vivado software and implemented the tasks on Zybo board by programming in C using SDK. The main object of the lab was to create tasks and manage them based of the specifications. The project results met the objectives as expected. 
