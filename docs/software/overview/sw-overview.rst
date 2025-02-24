.. include:: ./../../macros.txt
.. include:: ./../../units.txt

.. _SOFTWARE_STRUCTURE_OVERVIEW:

###########################
Software Structure Overview
###########################

.. highlight:: C

*******
Startup
*******

The startup code begins in the function ``_c_int00()`` in
``app/main/fstartup.c``. After initialization of the main
microcontroller registers, memory, system clock and interrupts,
the C function ``main()`` is called.
In ``general/main.c``, interrupts are enabled and the initializations of the
microcontroller unit, peripherals and software modules are done (e.g.,
hardware modules like SPI and DMA). The OS is than started.The steps are
indicated by the global variable
``os_boot``. At the end of the main function, the operating system resources
(tasks, events, queues, mutex) are configured in ``OS_InitializeTasks()``
(``os/os.c``) and the scheduler is started.
All configured tasks (FreeRTOS threads) are then started depending on
their priority. The successful activation of the tasks is indicated by
``os_boot = OS_RUNNING``.

The OS-scheduler first calls the highest priority task. All other cyclic tasks
are blocked in a while-loop until the initialization of this
task finishes. At the beginning of the task, ``FTSK_UserCodeEngineInit()`` is
called. In this function, the database is initialized. Once finished, this
is indicated by ``os_boot = OS_ENGINE_RUNNING``. The function
``FTSK_UserCodeEngine()`` is then called, where the diagnostic module and the
database are managed.

Once ``os_boot = OS_ENGINE_RUNNING``, the 1ms cyclic task is unblocked. The
function ``FTSK_UserCodePreCyclicTasksInitialization()`` is called once first.
This function is called when the OS and the database are running but before
the cyclic tasks run. Once ``FTSK_UserCodePreCyclicTasksInitialization()`` has
finished, all cyclic tasks are unblocked from there while-loop and run
periodically.

****************
Operating System
****************

A critical section must be entered with the function
``OS_EnterTaskCritical()`` and exited with the function
``OS_ExitTaskCritical()``.

The operating system configuration can be found in the file
``FreeRTOSConfig.h``. It should not be written directly. The configuration
has to be made with |halcogen|. Changes to this file will be overwritten by
|halcogen|. A detailed explanation of the parameters is given
at https://www.freertos.org/a00110.html.

*********************
Software Architecture
*********************

The database runs with the highest priority in the system and provides
asynchronous data exchange for the whole system.
:numref:`database-data-exchange` shows the data exchanges implemented via the
database.

   .. figure:: img/database.png
      :alt: Database data exchange
      :name: database-data-exchange
      :width: 100 %
      :align: center

      Asynchronous data exchange with the |foxbms| database

:numref:`foxbms-2-structure` shows the main structure of |foxbms|.

   .. figure:: img/engine.png
      :alt: |foxbms| structure
      :name: foxbms-2-structure
      :width: 100 %
      :align: center

      Main tasks in |foxbms|

The two key modules used are:

 - ``SYS``
 - ``BMS``

``SYS`` has a lower priority than the database and a higher priority than
``BMS``. Both modules are implemented as a state machine, with a trigger
function that implements the transition between the states. The trigger
functions of ``SYS`` and ``BMS`` are called in ``FTSK_UserCodeCyclic10ms()``.

``SYS`` controls the operating state of the system. It starts the other
state machines (e.g., ``BMS``).

``BMS`` gathers info on the system via the database and takes decisions based
on this data. The BMS is driven via CAN. Requests are made via CAN to go
either in STANDBY mode (i.e. contactors are open) or in NORMAL mode (i.e.
contactors are closed). A safety feature is that these requests must be sent
periodically every 100ms. ``BMS`` retrieves the state requests received via
CAN from the database and analyses them. If the requests are not sent
correctly, this means that the controlling unit driving the BMS has a
problem and the correctness of the orders sent to the BMS may not be given
anymore. As a consequence, in this case ``BMS`` makes a call to ``CONT``
to open the contactors. Currently, ``BMS`` checks the cell voltages, the cell
temperatures and the global battery current. If one of these physical
quantities violates the safe operating area, ``BMS`` makes the
corresponding call to ``CONT`` to open the contactors. ``BMS`` is
started via an initial state request made in ``SYS``.

A watchdog instance is needed in case one of the aforementioned tasks hangs.
This watchdog is made by the System Monitor module which monitors all
important tasks (e.g., ``Database``, ``SYS``, ``BMS``): if any of the
monitored tasks hangs, this will be detected.

A last barrier is present in case all the preceding measures fail: the
hardware watchdog timer. In case it is not triggered periodically, it resets
the system.

**********
Diagnostic
**********

The ``DIAG`` module is designed to report problems on the whole system. The
events that trigger the ``DIAG`` module have to be defined by the user. The
event handler ``DIAG_Handler(...)`` has to be called when the event is
detected. The way the system reacts to a ``Diag`` event is defined via a
callback function or by the caller according the return value.

***************************
Data stored in the database
***************************

The following data is stored in the database:

  - Cell voltages
  - Cell temperatures
  - SOX (Battery state, contains e.g., State-of-Charge)
  - Balancing control
  - Balancing feedback
  - Current sensor measurements (includes pack voltages at different points)
  - Hardware information
  - Last state request made to the BMS
  - Minimum, maximum and average values for voltages and temperatures
  - Measurement from isolation monitor
  - Interface to communicate via I2C with extra functionalities on slave
    (e.g., EEPROM, port expander)
  - Result from open-wire check on slaves
  - LTC device parameter
  - LTC accuracy
  - Error state of the BMS (i.e. error flags, set to 0 or 1)
  - MSL, maximum safety limits
  - RSL, recommended safety limits
  - MOL, maximum operating limits
  - Calculated values for moving average
  - Contactor feedback
  - Interlock feedback
  - BMS state (e.g., standby, normal, charge, error)
  - Current limits calculated for State-of-Function
  - Voltages read on GPIOs of the slaves
